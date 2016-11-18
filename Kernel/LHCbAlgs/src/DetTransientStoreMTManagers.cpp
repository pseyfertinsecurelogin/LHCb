#include "GaudiKernel/Parsers.h"
#include "GaudiKernel/Time.h"

namespace Gaudi {
  namespace Parsers {
    StatusCode parse(Gaudi::Time& output, const std::string& input) {
      double t;
      const StatusCode s = parse(t, input);
      if ( s ) output = static_cast<Gaudi::Time::ValueType>(t * 1E9);
      return s;
    }
    StatusCode parse(Gaudi::TimeSpan& output, const std::string& input) {
      double t;
      const StatusCode s = parse(t, input);
      if ( s ) output = static_cast<Gaudi::TimeSpan::ValueType>(t * 1E9);
      return s;
    }
  }
}

#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiAlg/Transformer.h"
#include "GaudiAlg/Producer.h"
#include "GaudiAlg/Consumer.h"
#include "DetDesc/ICondIOVResource.h"
#include "Event/ODIN.h"

#include <mutex>

namespace LHCb {
  namespace DetDesc {
    /**
     * @brief Algorithm to reserve/lock IOVs for event time in ODIN.
     */
    class ReserveDetDescForEvent: public Gaudi::Functional::Transformer<ICondIOVResource::IOVLock(const ODIN&)> {
    public:
      ReserveDetDescForEvent( const std::string& name, ISvcLocator* pSvcLocator ) :
        Transformer( name, pSvcLocator,
                     KeyValue("ODIN", LHCb::ODINLocation::Default),
                     KeyValue("IOVLock", ICondIOVResource::IOVLock::DefaultLocation) ) {}
      StatusCode initialize() override {
        auto sc = Transformer::initialize();
        if ( !sc ) return sc;
        // make sure we get the ICondIOVResource (AKA UpdateManagerSvc) during initialize
        m_IOVresource = SmartIF<ICondIOVResource>( updMgrSvc() );
        if ( ! m_IOVresource ||
             // and that we can access the IDataManagerSvc interface of the detector service
             ( m_preloadGeometry && ! detSvc().as<IDataManagerSvc>() ) ) sc = StatusCode::FAILURE;
        return sc;
      }
      StatusCode start() override {
        auto sc = Transformer::start();
        if ( !sc ) return sc;
        if ( m_preloadGeometry ) {
          auto mgr = detSvc().as<IDataManagerSvc>(); // this is for sure not null because I checked at initialize
          detSvc()->addPreLoadItem( mgr->rootName() + "*" ).ignore();
          detSvc()->preLoad().ignore();
        }
        return sc;
      }
      ICondIOVResource::IOVLock operator() (const ODIN& odin) const override {
        return m_IOVresource->reserve( odin.eventTime() );
      }
      StatusCode finalize() override {
        m_IOVresource.reset();
        return Transformer::finalize();
      }
    private:
      Gaudi::Property<bool> m_preloadGeometry {this, "PreloadGeometry", true,
                                               "if we need to trigger a preload of the geometry during start"};
      SmartIF<ICondIOVResource>  m_IOVresource;
    };
    DECLARE_COMPONENT(ReserveDetDescForEvent)
  }
  namespace Tests {
    /// Test algorithm to produce fake ODIN instances with event time starting from
    /// m_start and inceremented at every event by m_step.
    class FakeEventTimeProducer: public Gaudi::Functional::Producer<ODIN()> {
    public:
      FakeEventTimeProducer( const std::string& name, ISvcLocator* pSvcLocator ) :
        Producer( name, pSvcLocator,
                  KeyValue("ODIN", LHCb::ODINLocation::Default) ) {}
      StatusCode start() override {
        m_currentTime = m_start.value().ns();
        return Producer::start();
      }
      ODIN operator() () const override {
        ODIN odin;
        odin.setEventTime(Gaudi::Time{m_currentTime.fetch_add(m_step.value().ns())});
        return {odin};
      }

    private:

      mutable std::atomic<Gaudi::Time::ValueType> m_currentTime{0};

      Gaudi::Property<Gaudi::Time> m_start {this, "Start", Gaudi::Time::epoch(), "first time to use (seconds)"};
      Gaudi::Property<Gaudi::TimeSpan> m_step {this, "Step", Gaudi::TimeSpan{1, 0}, "increment to add at each step (seconds)"};
    };
    DECLARE_COMPONENT(FakeEventTimeProducer)

    /// Test algorithm to print the event time in ODIN.
    class PrintCurrentEventTime: public Gaudi::Functional::Consumer<void(const ODIN&)> {
    public:
      PrintCurrentEventTime( const std::string& name, ISvcLocator* pSvcLocator ) :
        Consumer( name, pSvcLocator,
                  KeyValue("ODIN", LHCb::ODINLocation::Default) ) {}
      void operator() (const ODIN& odin) const override {
        info() << "Current event time: " << odin.eventTime() << endmsg;
      }
    };
    DECLARE_COMPONENT(PrintCurrentEventTime)
  }
}
