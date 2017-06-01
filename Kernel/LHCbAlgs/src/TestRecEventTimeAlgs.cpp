#include <functional>
#include <string>
#include <vector>

#include "Event/RecHeader.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IEventTimeDecoder.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/IService.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/Incident.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/Time.h"

namespace LHCbAlgsTest
{

  class CreateFakeRecHeader final : public extends1<Service, IIncidentListener>
  {

  public:

    CreateFakeRecHeader(const std::string& name, ISvcLocator *pSvcLocator) :
      base_class(name, pSvcLocator)
    {
      declareProperty("EventTimeDecoder", m_timeDecoderName = "FakeEventTime",
                      "Tool to use to generate the event times for the fake RecHeader");
      declareProperty("RecHeaderLocation",
                      m_recHeaderLoc = LHCb::RecHeaderLocation::Default,
                      "Alternative location for RecHeader.");
    }

    virtual ~CreateFakeRecHeader() = default;

    StatusCode initialize() override
    {
      StatusCode sc = base_class::initialize();
      if (sc.isFailure())
        return sc;

      SmartIF<IToolSvc> toolSvc(serviceLocator()->service("ToolSvc"));
      if (!toolSvc) return StatusCode::FAILURE;

      sc = toolSvc->retrieveTool(m_timeDecoderName, m_timeDecoder, this);
      if (!sc.isSuccess()) return sc;

      SmartIF<IIncidentSvc> incidentSvc(serviceLocator()->service("IncidentSvc"));
      if (!incidentSvc) return StatusCode::FAILURE;

      incidentSvc->addListener(this, IncidentType::BeginEvent);

      m_dataProvider = serviceLocator()->service("EventDataSvc");

      return sc;
    }

    void handle(const Incident&) override
    {
      Gaudi::Time t = m_timeDecoder->getTime();
      info() << "Adding RecHeader with event time " << t << endmsg;
      LHCb::RecHeader * h = new LHCb::RecHeader();
      h->setGpsTime(t.ns()/1000);
      m_dataProvider->registerObject(m_recHeaderLoc, h);
    }

  private:

    /// Type of the IEventTimeDecoder instance.
    std::string m_timeDecoderName;

    /// IEventTimeDecoder instance
    IEventTimeDecoder *m_timeDecoder = nullptr;

    /// Location of the RecHeader in the transient store.
    std::string m_recHeaderLoc;

    /// Pointer to the event transient store.
    SmartIF<IDataProviderSvc> m_dataProvider;
  };

}

DECLARE_NAMESPACE_SERVICE_FACTORY( LHCbAlgsTest, CreateFakeRecHeader )
