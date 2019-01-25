/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files
#include "DetDesc/Condition.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IDetDataSvc.h"
#include "GaudiKernel/Map.h"

#include <DetDesc/ConditionAccessorHolder.h>

#include <cmath>
#include <vector>

namespace DetCondTest {

  /** @class TestConditionAlg TestConditionAlg.h component/TestConditionAlg.h
   *
   *  Simple algorithm that prints the requested conditions at every event.
   *
   *  @author Marco CLEMENCIC
   *  @date   2008-06-27
   */
  class TestConditionAlg : public GaudiAlgorithm {
  public:
    /// Standard constructor
    using GaudiAlgorithm::GaudiAlgorithm;

    StatusCode initialize() override; ///< Algorithm initialization
    StatusCode execute() override;    ///< Algorithm execution
    StatusCode finalize() override;   ///< Algorithm finalization

  protected:
    void i_dump();

    Gaudi::Property<std::vector<std::string>> m_condPaths{
        this, "Conditions", {}, "list of paths to conditions in the detector transient store"};
    Gaudi::Property<bool>                     m_loadAtInit{this, "LoadDuringInitialize", false,
                                       "load the requested conditions already during the initialization"};
    Gaudi::Property<std::vector<std::string>> m_conditionDeps{
        this,
        "ConditionsDependencies",
        {},
        "declare dependencies between objects (as a list of strings 'A -> B') "
        "to indicate that the condition A depends on B."};

    /// Container of the conditions to print
    GaudiUtils::Map<std::string, Condition*> m_conditions;

    struct CondUpdateMonitor {
      virtual ~CondUpdateMonitor() = default;
      CondUpdateMonitor( MsgStream& _log, std::string _path ) : log{_log}, path{std::move( _path )} {}
      virtual StatusCode handler() {
        log << MSG::INFO << path << " was updated" << endmsg;
        return StatusCode::SUCCESS;
      }
      MsgStream&  log;
      std::string path;
    };
    std::vector<CondUpdateMonitor> m_updateMonitors;
  };

  namespace {
    void printDepsError( MsgStream& stream, size_t lineNo, const std::string& msg, const std::string& line ) {
      stream << "Syntax error in item " << lineNo << " of ConditionsDependencies: " << msg << endmsg;
      stream << "  '" << line << "'" << endmsg;
    }
  } // namespace
  //=============================================================================
  // Initialization
  //=============================================================================
  StatusCode TestConditionAlg::initialize() {
    StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
    if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm

    if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

    m_updateMonitors.reserve( m_condPaths.size() );
    for ( const auto& path : m_condPaths ) {
      // C++ 17 version
      auto mon = &m_updateMonitors.emplace_back( msgStream(), path );
      updMgrSvc()->registerCondition( mon, path, &CondUpdateMonitor::handler, m_conditions[path] );
      registerCondition<TestConditionAlg>( mon );
    }

    std::size_t cnt = 0;
    for ( const auto& deps : m_conditionDeps ) {
      std::string::size_type pos = deps.find( "->" );
      if ( pos == std::string::npos ) {
        printDepsError( error(), cnt, "missing '->'", deps );
        return StatusCode::FAILURE;
      }
      std::string::size_type p0, p1;
      p0 = deps.find_first_not_of( " \n\r\t" );
      p1 = deps.find_last_not_of( " \n\r\t", pos - 1 ) + 1;
      std::string first( deps, p0, p1 - p0 );
      if ( first.empty() ) {
        printDepsError( error(), cnt, "missing first argument", deps );
        return StatusCode::FAILURE;
      }
      p0 = deps.find_first_not_of( " \n\r\t", pos + 2 );
      p1 = deps.find_last_not_of( " \n\r\t" ) + 1;
      std::string second( deps, p0, p1 - p0 );
      if ( second.empty() ) {
        printDepsError( error(), cnt, "missing second argument", deps );
        return StatusCode::FAILURE;
      }
      info() << "Declaring dependency of '" << first << "' on '" << second << "'" << endmsg;
      updMgrSvc()->registerCondition( getDet<Condition>( first ), second );
      ++cnt;
    }

    if ( m_loadAtInit ) {
      sc = updMgrSvc()->update( this );
      info() << "Conditions loaded at initialize" << endmsg;
      if ( sc.isSuccess() ) {
        i_dump();
      } else
        return sc;
    }
    return StatusCode::SUCCESS;
  }

  //=============================================================================
  // Main execution
  //=============================================================================
  StatusCode TestConditionAlg::execute() {

    if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;
    i_dump();

    return StatusCode::SUCCESS;
  }

  //=============================================================================
  //  Finalize
  //=============================================================================
  StatusCode TestConditionAlg::finalize() {

    if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Finalize" << endmsg;

    updMgrSvc()->unregister( this );
    m_conditions.clear();
    m_condPaths.clear();

    return GaudiAlgorithm::finalize(); // must be called after all other actions
  }

  //=============================================================================
  //  Print the conditions
  //=============================================================================
  void TestConditionAlg::i_dump() {
    info() << "Requested Conditions:\n";
    for ( const auto& item : m_conditions ) { info() << "--- " << item.first << "\n" << *( item.second ) << "\n"; }
    info() << endmsg;
  }

  //=============================================================================

  /** Small algorithm that runs a fake event loop during finalize to scan the
   *  values of the conditions (see bug #74255).
   *
   *  @author Marco CLEMENCIC
   *  @date   2010-10-25
   */
  class FinalizationEvtLoop : public GaudiAlgorithm {
  public:
    /// Standard constructor
    FinalizationEvtLoop( const std::string& name, ISvcLocator* pSvcLocator ) : GaudiAlgorithm( name, pSvcLocator ) {
      declareProperty( "Conditions", m_condPaths, "list of paths to conditions in the detector transient store" );
      declareProperty( "InitialTime", m_initTime = 0, // 1970-01-01 00:00:00UTC
                       "First event time of the fake event loop" );
      declareProperty( "FinalTime", m_finalTime = m_initTime + 10000000000LL, // init + 10s
                       "Final time of the loop" );
      declareProperty( "Step", m_step = 1000000000LL, // 1s
                       "Step of the loop" );
    }

    virtual ~FinalizationEvtLoop() {}

    StatusCode initialize() override {
      StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
      if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm

      if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

      std::vector<std::string>::const_iterator path;
      for ( path = m_condPaths.begin(); path != m_condPaths.end(); ++path ) {
        registerCondition<FinalizationEvtLoop>( *path, m_conditions[*path], NULL );
      }

      return StatusCode::SUCCESS;
    }

    StatusCode execute() override { return StatusCode::SUCCESS; }

    StatusCode finalize() override {
      Gaudi::Time           t( m_initTime );
      const Gaudi::Time     fin( m_finalTime );
      const Gaudi::TimeSpan step( m_step );

      SmartIF<IDetDataSvc> dds( detSvc() );
      for ( ; t < fin; t += step ) {
        dds->setEventTime( t );
        info() << "Update for event time " << t << endmsg;
        if ( updMgrSvc()->newEvent().isSuccess() ) {
          info() << "Requested Conditions:\n";
          GaudiUtils::Map<std::string, Condition*>::iterator it;
          for ( it = m_conditions.begin(); it != m_conditions.end(); ++it ) {
            info() << "--- " << it->first << "\n" << *( it->second ) << "\n";
          }
          info() << endmsg;
        } else {
          error() << "Failure updating" << endmsg;
          return StatusCode::FAILURE;
        }
      }

      return StatusCode::SUCCESS;
    }

  private:
    /// First event time of the fake event loop
    Gaudi::Time::ValueType m_initTime;
    /// First event time of the fake event loop
    Gaudi::Time::ValueType m_finalTime;
    /// First event time of the fake event loop
    Gaudi::Time::ValueType m_step;

    /// Names of the conditions to print
    std::vector<std::string> m_condPaths;

    /// Container of the conditions to print
    GaudiUtils::Map<std::string, Condition*> m_conditions;
  };

  /** Test algorithm that triggers the bug #80076
   *  https://savannah.cern.ch/bugs/?80076
   */
  class bug_80076 : public TestConditionAlg {
  public:
    /// Constructor.
    bug_80076( const std::string& name, ISvcLocator* pSvcLocator ) : TestConditionAlg( name, pSvcLocator ) {}

    /// Override the initialize to ensure that the conditions are already loaded
    /// during the initialize.
    StatusCode initialize() override {
      StatusCode sc = TestConditionAlg::initialize();
      if ( sc.isFailure() ) return sc;

      std::vector<std::string>::const_iterator path;
      for ( path = m_condPaths.begin(); path != m_condPaths.end(); ++path ) {
        // this ensures that the objects are loaded in the transient store
        exist<DataObject>( detSvc(), *path );
      }

      return sc;
    }
  };

  struct CondAccessExample : LHCb::DetDesc::ConditionAccessorHolder<Gaudi::Algorithm> {
    // inherit base class costructor
    using LHCb::DetDesc::ConditionAccessorHolder<Gaudi::Algorithm>::ConditionAccessorHolder;

    ConditionAccessor<Condition> m_cond{this, "CondPath", "TestCondition"};

    StatusCode execute( const EventContext& ctx ) const override {
      const auto& cctx = getConditionContext( ctx );
      const auto& cond = m_cond.get( cctx );
      info() << "condition value: " << cond << endmsg;
      return StatusCode::SUCCESS;
    }
  };

  struct CondAccessExampleWithDerivation : LHCb::DetDesc::ConditionAccessorHolder<Gaudi::Algorithm> {
    // inherit base class costructor
    using LHCb::DetDesc::ConditionAccessorHolder<Gaudi::Algorithm>::ConditionAccessorHolder;

    struct MyData {
      double p1, p2, v;
    };

    Gaudi::Property<std::string> m_srcPath{this, "Source", "TestCondition"};
    ConditionAccessor<MyData>    m_cond{this, "Target", "DerivedCondition"};

    struct ConditionTransformation {
      virtual ~ConditionTransformation() = default;

      // actual transformation code
      static MyData transform( const Condition& cond ) {
        const auto p1 = cond.param<double>( "par1" );
        const auto p2 = cond.param<double>( "par2" );
        return MyData{p1, p2, std::sqrt( p1 ) + 2.0 * p2 * p2};
      }

      void registerDerivation( IUpdateManagerSvc* ums, IDataProviderSvc* dds, const ConditionKey& in_path,
                               const ConditionKey& out_path ) {
        DataObject* obj = nullptr;
        StatusCode  sc  = dds->retrieveObject( in_path, obj );
        if ( !sc ) throw GaudiException( "failed to retrieve " + in_path, "ConditionTransformation", sc );
        input = dynamic_cast<Condition*>( obj );
        if ( !input ) throw GaudiException( "wrong type for " + in_path, "ConditionTransformation", sc );

        auto tmp = std::make_unique<Condition>();
        sc       = dds->registerObject( out_path, tmp.get() );
        if ( !sc ) throw GaudiException( "failed to add " + out_path, "ConditionTransformation", sc );
        output = tmp.release();

        ums->registerCondition( this, in_path, &ConditionTransformation::handler, input );
        ums->registerCondition( output, this );
      }

      // boilerplate
      Condition* input  = nullptr;
      Condition* output = nullptr;

      StatusCode handler() {
        output->payload = transform( *input );
        return StatusCode::SUCCESS;
      }
    } m_transformer;

    StatusCode initialize() override {
      const auto sc = LHCb::DetDesc::ConditionAccessorHolder<Gaudi::Algorithm>::initialize();
      if ( !sc ) return sc;

      m_transformer.registerDerivation( service<IUpdateManagerSvc>( "UpdateManagerSvc" ), detSvc(), m_srcPath,
                                        m_cond.key() );

      return sc;
    }

    StatusCode execute( const EventContext& ctx ) const override {
      const auto& cctx = getConditionContext( ctx );
      const auto& cond = m_cond.get( cctx );
      info() << "condition value: {\n  p1: " << cond.p1 << "\n  p2: " << cond.p2 << "\n  v:  " << cond.v << "\n}"
             << endmsg;
      return StatusCode::SUCCESS;
    }
  };
} // namespace DetCondTest

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( DetCondTest::TestConditionAlg )
DECLARE_COMPONENT( DetCondTest::FinalizationEvtLoop )
DECLARE_COMPONENT( DetCondTest::bug_80076 )
DECLARE_COMPONENT( DetCondTest::CondAccessExample )
DECLARE_COMPONENT( DetCondTest::CondAccessExampleWithDerivation )
