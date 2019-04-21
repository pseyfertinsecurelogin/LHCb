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
#include "HLTControlFlowMgr.h"
#include "GaudiKernel/IDataSelector.h"
#include "GaudiKernel/SerializeSTL.h"
#ifdef NDEBUG
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  define GSL_UNENFORCED_ON_CONTRACT_VIOLATION
#endif
#include <gsl/pointers>
#ifdef NDEBUG
#  pragma GCC diagnostic pop
#endif

// Instantiation of a static factory class used by clients to create instances of this service
DECLARE_COMPONENT( HLTControlFlowMgr )
namespace GaudiUtils {

  // operator<< overloads for variant nodes
  std::ostream& operator<<( std::ostream& os, VNode const& node ) { return os << getNameOfVNode( node ); }

  // be able to print vectors of ordered nodes correctly
  template <typename T>
  std::ostream& operator<<( std::ostream& os, gsl::not_null<T*> const& ptr ) {
    return os << *ptr;
  }

  template <nodeType Type>
  std::ostream& operator<<( std::ostream& os, CompositeNode<Type> const& node ) {
    return os << node.m_name;
  }

  std::ostream& operator<<( std::ostream& os, NodeState const& state ) {
    return os << state.executionCtr << "|" << state.passed;
  }

  template <typename Key, typename Compare, typename Allocator>
  std::ostream& operator<<( std::ostream& os, std::set<Key, Compare, Allocator> const& x ) {
    return GaudiUtils::details::ostream_joiner( os << "{", x, ", " ) << "}";
  }
} // namespace GaudiUtils
namespace {

  using GaudiUtils::operator<<;

  template <typename fun>
  class Wrapper final : public tbb::task {
    fun m_f;

  public:
    Wrapper( fun f ) : m_f( std::move( f ) ) {}
    tbb::task* execute() override {
      m_f();
      return nullptr;
    }
  };

  template <typename fun>
  void enqueue( fun&& f ) {
    tbb::task* task = new ( tbb::task::allocate_root() ) Wrapper{std::forward<fun>( f )};
    tbb::task::enqueue( *task );
  }

  // observe threads to be able to join in the end
  struct task_observer final : public tbb::task_scheduler_observer {
    task_observer() { observe( true ); }

    std::atomic<int> m_thread_count{0};

    void on_scheduler_entry( bool ) override { m_thread_count++; }

    void on_scheduler_exit( bool ) override { m_thread_count--; }
  };
} // namespace

StatusCode HLTControlFlowMgr::initialize() {
  using Clock = std::chrono::high_resolution_clock;

  info() << "Start initialization" << endmsg;

  auto start_time = Clock::now();

  StatusCode sc = Service::initialize();
  if ( !sc.isSuccess() ) {
    error() << "Failed to initialize Service Base class." << endmsg;
    return StatusCode::FAILURE;
  }

  // FIXME why EventDataSvc? rather give a proper name
  // Setup access to event data services
  m_evtDataMgrSvc = serviceLocator()->service<IDataManagerSvc>( "EventDataSvc" );
  if ( !m_evtDataMgrSvc ) {
    fatal() << "Error retrieving EventDataSvc interface IDataManagerSvc." << endmsg;
    return StatusCode::FAILURE;
  }

  m_whiteboard = serviceLocator()->service<IHiveWhiteBoard>( "EventDataSvc" );
  if ( !m_whiteboard ) {
    fatal() << "Error retrieving EventDataSvc interface IHiveWhiteBoard." << endmsg;
    return StatusCode::FAILURE;
  }
  // Obtain the IProperty of the ApplicationMgr
  IProperty* appMgrProperty = serviceLocator()->service<IProperty>( "ApplicationMgr" );
  if ( !appMgrProperty ) {
    fatal() << "IProperty interface not found in ApplicationMgr." << endmsg;
    return StatusCode::FAILURE;
  }
  // We do not expect a Event Selector necessarily being declared
  setProperty( appMgrProperty->getProperty( "EvtSel" ) ).ignore();

  if ( m_evtsel.length() == 0 ) {
    m_evtSelector = serviceLocator()->service<IEvtSelector>( "EventSelector" );
  } else if ( m_evtsel == "NONE" ) {
    m_evtSelector = nullptr;
    warning() << "Unable to locate service \"EventSelector\" " << endmsg;
    warning() << "No events will be processed from external input." << endmsg;
  } else {
    m_evtSelector = serviceLocator()->service<IEvtSelector>( m_evtsel );
    if ( !m_evtSelector ) {
      fatal() << "IEventSelector.\"" << m_evtsel << "\" not found." << endmsg;
      return StatusCode::FAILURE;
    }
  }

  // Setup access to histogramming services
  m_histoDataMgrSvc = serviceLocator()->service<IDataManagerSvc>( "HistogramDataSvc" );
  if ( !m_histoDataMgrSvc ) {
    fatal() << "Error retrieving HistogramDataSvc." << endmsg;
    return sc;
  }
  // Setup histogram persistency
  m_histoPersSvc = serviceLocator()->service<IConversionSvc>( "HistogramPersistencySvc" );
  if ( !m_histoPersSvc ) {
    warning() << "Histograms cannot not be saved - though required." << endmsg;
    return sc;
  }

  m_algExecStateSvc = serviceLocator()->service<IAlgExecStateSvc>( "AlgExecStateSvc" );
  if ( !m_algExecStateSvc ) {
    fatal() << "Error retrieving AlgExecStateSvc" << endmsg;
    return StatusCode::FAILURE;
  }

  m_databroker = serviceLocator()->service<IDataBroker>( "HiveDataBrokerSvc" );
  if ( !m_databroker ) {
    fatal() << "Error retrieving HiveDataBrokerSvc" << endmsg;
    return StatusCode::FAILURE;
  }

  // Clearly inform about the level of concurrency
  info() << "Concurrency level information:" << endmsg;
  info() << " o Number of events slots: " << m_whiteboard->getNumberOfStores() << endmsg;
  info() << " o TBB thread pool size: " << m_threadPoolSize << endmsg;

  // ------------------------------- scheduling -------------------------------------------------------
  // configure the lines
  buildLines();

  // configure the order of nodes
  configureScheduling();

  // build the vector of states (to be copied into each thread)
  buildNodeStates();

  // build the m_printableDependencyTree for monitoring
  registerStructuredTree();
  registerTreePrintWidth();

  // print out the nodes + states
  if ( msgLevel( MSG::DEBUG ) ) {
    for ( std::size_t i = 0; i < m_allVNodes.size(); ++i ) {
      debug() << std::left << "initialize " << std::setw( 25 ) << m_allVNodes[i] << " with state " << m_NodeStates[i]
              << endmsg;
    }
  }

  // print out high level nodes + their children
  if ( msgLevel( MSG::DEBUG ) ) {
    for ( auto& vnode : m_allVNodes ) {
      std::visit( overload{[&]( auto& node ) {
                             debug() << std::left << "children of " << std::setw( 12 ) << node.getType() << " "
                                     << std::setw( 25 ) << node.m_name << ": " << node.m_children << endmsg;
                           },
                           []( BasicNode& ) {}},
                  vnode );
    }
  }
  if ( msgLevel( MSG::DEBUG ) ) debug() << m_compositeCFProperties << endmsg;
  auto end_time = Clock::now();

  info() << "---> End of Initialization. "
         << "This took " << std::chrono::duration_cast<std::chrono::milliseconds>( end_time - start_time ).count()
         << " ms" << endmsg;

  return sc;

  // ------------------------------- scheduling end ---------------------------------------------------
}

StatusCode HLTControlFlowMgr::finalize() {
  StatusCode sc;

  // print the counters
  info() << boost::format{"\n | Name of Algorithm %|51t| | Execution Count\n"};
  for ( auto const& [ctr, name] : Gaudi::Functional::details::zip::range( m_AlgExecCounters, m_AlgNames ) ) {
    ctr.print( info(), name ) << '\n';
  }
  info() << endmsg;

  // print the counters
  info() << buildPrintableStateTree( m_NodeStateCounters ).str() << endmsg;

  // Save Histograms Now
  if ( m_histoPersSvc ) {
    IDataSelector objects;
    sc = m_histoDataMgrSvc->traverseTree( [&objects]( IRegistry* pReg, int ) {
      DataObject* obj = pReg->object();
      if ( obj && obj->clID() != CLID_StatisticsFile ) {
        objects.push_back( obj );
        return true;
      }
      return false;
    } );
    if ( sc.isSuccess() ) {
      // skip /stat entry!
      sc = std::accumulate( begin( objects ), end( objects ), sc, [&]( StatusCode s, auto const& i ) {
        IOpaqueAddress* pAddr = nullptr;
        StatusCode      iret  = m_histoPersSvc->createRep( i, pAddr );
        if ( iret.isSuccess() ) i->registry()->setAddress( pAddr );
        return s.isFailure() ? s : iret;
      } );
      sc = std::accumulate( begin( objects ), end( objects ), sc, [&]( StatusCode s, auto const& i ) {
        IRegistry* reg  = i->registry();
        StatusCode iret = m_histoPersSvc->fillRepRefs( reg->address(), i );
        return s.isFailure() ? s : iret;
      } );
      if ( sc.isSuccess() ) {
        info() << "Histograms converted successfully according to request." << endmsg;
      } else {
        error() << "Error while saving Histograms." << endmsg;
      }
    } else {
      error() << "Error while traversing Histogram data store" << endmsg;
    }
  }

  StatusCode sc2 = Service::finalize();
  return sc.isFailure() ? sc2.ignore(), sc : sc2;
}

StatusCode HLTControlFlowMgr::executeEvent( void* createdEvts_IntPtr ) {
  // Leave the interface intact and swallow this C trick.
  int& createdEvts = *reinterpret_cast<int*>( createdEvts_IntPtr );

  auto evtContext = std::make_unique<EventContext>();
  evtContext->set( createdEvts, m_whiteboard->allocateStore( createdEvts ) );
  m_algExecStateSvc->reset( *evtContext );

  // giving the scheduler states to the evtContext, so that they are
  // globally accessible within an event
  // copy of states happens here!
  evtContext->emplaceExtension<SchedulerStates>( m_NodeStates, m_AlgStates );

  StatusCode sc = m_whiteboard->selectStore( evtContext->slot() );
  if ( sc.isFailure() ) {
    fatal() << "Slot " << evtContext->slot() << " could not be selected for the WhiteBoard\n"
            << "Impossible to create event context" << endmsg;
    return StatusCode::FAILURE;
  }

  if ( m_evtSelector ) {
    StatusCode declEvtRootSc = declareEventRootAddress();
    if ( declEvtRootSc.isFailure() ) { // We ran out of events!
      createdEvts = -1;                // Set created event to a negative value: we finished!
      return StatusCode::SUCCESS;
    }
  } else {
    sc = m_evtDataMgrSvc->setRoot( "/Event", new DataObject() );
    if ( !sc.isSuccess() ) { warning() << "Error declaring event root DataObject" << endmsg; }
  }

  // Now add event to the scheduler
  if ( msgLevel( MSG::VERBOSE ) )
    verbose() << "Event " << evtContext->evt() << " submitting in slot " << evtContext->slot() << endmsg;

  enqueue( [evtContext = std::move( evtContext ), createdEvts, this]() mutable {
    auto& [NodeStates, AlgStates] = evtContext->getExtension<SchedulerStates>();

    Gaudi::Hive::setCurrentContext( evtContext.get() );

    SmartIF<IProperty> appmgr( serviceLocator() );

    for ( AlgWrapper& toBeRun : m_definitlyRunTheseAlgs ) {
      try {
        auto sc = toBeRun.execute( *evtContext, AlgStates );
        if ( UNLIKELY( !sc.isSuccess() ) ) {
          m_algExecStateSvc->updateEventStatus( true, *evtContext );
          break;
        }
      } catch ( ... ) {
        m_algExecStateSvc->updateEventStatus( true, *evtContext );
        fatal() << "ERROR: Event failed in Algorithm " << toBeRun.name() << endmsg;
        Gaudi::setAppReturnCode( appmgr, Gaudi::ReturnCode::UnhandledException );
        break;
      }
    }

    for ( gsl::not_null<VNode*> execNode : m_orderedNodesVec ) {
      std::visit( overload{[&, ns = std::ref( NodeStates ), as = std::ref( AlgStates )]( BasicNode& bNode ) {
                             if ( bNode.requested( ns.get() ) ) {
                               bNode.execute( ns.get(), as.get(), *evtContext, m_algExecStateSvc, appmgr );
                               bNode.notifyParents( ns.get() );
                             }
                           },
                           []( ... ) {}},
                  *execNode );
    }
    m_algExecStateSvc->updateEventStatus( false, *evtContext );

    // printing
    if ( msgLevel( MSG::VERBOSE ) && createdEvts % m_printFreq == 0 ) {
      verbose() << buildPrintableStateTree( NodeStates ).str() << endmsg;
      verbose() << buildAlgsWithStates( AlgStates ).str() << endmsg;
    }

    // update algorithm execution counters
    for ( auto const& [ctr, as] : Gaudi::Functional::details::zip::range( m_AlgExecCounters, AlgStates ) )
      if ( as ) ctr++;

    // update node state counters
    for ( auto const& [ctr, ns] : Gaudi::Functional::details::zip::range( m_NodeStateCounters, NodeStates ) )
      if ( ns.executionCtr == 0 ) ctr += ns.passed; // only add when actually executed

    // update scheduler state
    promoteToExecuted( std::move( evtContext ) );
    // Gaudi::Hive::setCurrentContextEvt( -1 );

    return nullptr;
  } );

  createdEvts++;
  return StatusCode::SUCCESS;
}

StatusCode HLTControlFlowMgr::stopRun() {
  // Set the application return code
  auto appmgr = serviceLocator()->as<IProperty>();
  if ( Gaudi::setAppReturnCode( appmgr, Gaudi::ReturnCode::ScheduledStop ).isFailure() ) {
    error() << "Could not set return code of the application (" << Gaudi::ReturnCode::ScheduledStop << ")" << endmsg;
  }
  return StatusCode::SUCCESS;
}

StatusCode HLTControlFlowMgr::nextEvent( int maxevt ) {
  // Calculate runtime
  using Clock = std::chrono::high_resolution_clock;

  // Reset the application return code.
  auto appmgr = serviceLocator()->as<IProperty>();
  Gaudi::setAppReturnCode( appmgr, Gaudi::ReturnCode::Success, true ).ignore();

  // create m_evtSelContext used internally in executeEvent and more
  // precisely in declareEventRootAddress. Cannot be passed through the interface
  // without breaking other schedulers
  if ( m_evtSelector ) {
    StatusCode sc = m_evtSelector->createContext( m_evtSelContext );
    if ( !sc.isSuccess() ) {
      fatal() << "Can not create the event selector Context." << endmsg;
      return sc;
    }
  }

  // create th tbb thread pool
  tbb::task_scheduler_init tbbSchedInit( m_threadPoolSize.value() + 1 );
  task_observer            taskObsv{};

  using namespace std::chrono_literals;

  int createdEvts = 0;
  // Run the first event before spilling more than one
  bool newEvtAllowed = false;

  // adjust timing start counter
  if ( m_startTimeAtEvt < 0 || m_startTimeAtEvt > m_stopTimeAfterEvt ) {
    if ( maxevt > 0 ) {
      if ( m_startTimeAtEvt < -1 || m_startTimeAtEvt > m_stopTimeAfterEvt ) {
        warning() << "StartTimeAtEvt set faulty, changing to .1*maxevt" << endmsg;
      }
      m_startTimeAtEvt = static_cast<int>( .1 * maxevt );
    } else {
      if ( m_startTimeAtEvt < -1 || m_startTimeAtEvt > m_stopTimeAfterEvt ) {
        warning() << "StartTimeAtEvt set faulty, changing to 10*evtslots" << endmsg;
      }
      m_startTimeAtEvt = 10 * m_whiteboard->getNumberOfStores();
    }
  }

  // adjust timing stop counter
  if ( maxevt > 0 ) {
    if ( m_stopTimeAfterEvt < 0 ) { // default
      if ( m_stopTimeAfterEvt < -1 ) { warning() << "StopTimeAfterEvt set faulty, changing to .9*maxevt" << endmsg; }
      m_stopTimeAfterEvt = static_cast<int>( .9 * maxevt );
    } else if ( m_stopTimeAfterEvt > maxevt - 1 ) {
      warning() << "StopTimeAfterEvt too big, changing to last event (maxevt - 1)" << endmsg;
      m_stopTimeAfterEvt = maxevt - 1;
    }
  } else {
    if ( m_stopTimeAfterEvt < 0 ) { // default
      if ( m_stopTimeAfterEvt < -1 ) { warning() << "StopTimeAfterEvt set faulty, changing to last element" << endmsg; }
      m_stopTimeAfterEvt = std::numeric_limits<int>::max();
    }
  }

  info() << "Will measure time between events " << m_startTimeAtEvt.value() << " and " << m_stopTimeAfterEvt.value()
         << " (stop might be some events later)" << endmsg;

  auto okToStartNewEvt = [&] {
    return ( newEvtAllowed || createdEvts == 0 ) && // Launch the first event alone
                                                    // The events are not finished with an unlimited number of events
           createdEvts >= 0 &&
           // The events are not finished with a limited number of events
           ( createdEvts < maxevt || maxevt < 0 ) &&
           // There are still free slots in the whiteboard
           m_whiteboard->freeSlots() > 0;
  };

  auto maxEvtNotReached = [&] { return maxevt < 0 || m_finishedEvt < (unsigned int)maxevt; };

  info() << "Starting loop on events" << endmsg;
  std::optional<decltype( Clock::now() )> startTime, endTime;

  while ( maxEvtNotReached() ) {
    std::unique_lock<std::mutex> lock{m_createEventMutex};
    if ( m_createEventCond.wait_for( lock, 2ms, okToStartNewEvt ) ) {
      // in some cases, due to concurrency, we do not stop exactly at the given value.
      if ( UNLIKELY( static_cast<unsigned int>( m_stopTimeAfterEvt ) <= m_finishedEvt - 1 && !endTime &&
                     m_finishedEvt > 0 ) ) {
        m_stopTimeAfterEvt = m_finishedEvt - 1;
        endTime            = Clock::now();
      }
      if ( UNLIKELY( m_startTimeAtEvt == createdEvts ) ) startTime = Clock::now();
      StatusCode sc = executeEvent( &createdEvts );
      if ( !sc.isSuccess() ) return StatusCode::FAILURE; // else we have an success --> exit loop
      if ( createdEvts == -1 ) break;
      newEvtAllowed = true;
    }
  } // end main loop on finished events

  if ( !endTime ) {
    endTime            = Clock::now();
    m_stopTimeAfterEvt = m_finishedEvt - 1;
  }

  tbbSchedInit.terminate();             // non blocking
  while ( taskObsv.m_thread_count > 0 ) // this is our "threads.join()" alternative
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

  delete m_evtSelContext;
  m_evtSelContext = nullptr;

  if ( UNLIKELY( !startTime ) ) {
    info() << "---> Loop over " << m_finishedEvt << " Events Finished - "
           << " WSS " << System::mappedMemory( System::MemoryUnit::kByte ) * 1. / 1024. << ", timing failed.."
           << endmsg;
  } else {
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>( *endTime - *startTime ).count();

    info() << "---> Loop over " << m_finishedEvt << " Events Finished - "
           << " WSS " << System::mappedMemory( System::MemoryUnit::kByte ) * 1. / 1024. << ", timed "
           << m_stopTimeAfterEvt.value() - m_startTimeAtEvt.value() + 1 << " Events: " << totalTime << " ms"
           << ", Evts/s = " << ( m_stopTimeAfterEvt.value() - m_startTimeAtEvt.value() + 1 ) * 1. / totalTime * 1e3
           << endmsg;
  }
  return StatusCode::SUCCESS;
}

StatusCode HLTControlFlowMgr::declareEventRootAddress() {
  IOpaqueAddress* pAddr = nullptr;
  StatusCode      sc    = m_evtSelector->next( *m_evtSelContext );
  if ( sc.isSuccess() ) {
    // Create root address and assign address to data service
    sc = m_evtSelector->createAddress( *m_evtSelContext, pAddr );
    if ( !sc.isSuccess() ) {
      sc = m_evtSelector->next( *m_evtSelContext );
      if ( sc.isSuccess() ) {
        sc = m_evtSelector->createAddress( *m_evtSelContext, pAddr );
        if ( !sc.isSuccess() ) warning() << "Error creating IOpaqueAddress." << endmsg;
      }
    }
  }
  if ( !sc.isSuccess() ) {
    info() << "No more events in event selection " << endmsg;
    return StatusCode::FAILURE;
  }
  sc = m_evtDataMgrSvc->setRoot( "/Event", pAddr );
  if ( !sc.isSuccess() ) { warning() << "Error declaring event root address." << endmsg; }
  return StatusCode::SUCCESS;
}

/**
 * It can be possible that an event fails. In this case this method is called.
 * It dumps the state of the scheduler, drains the actions (without executing
 * them) and events in the queues and returns a failure.
 */
StatusCode HLTControlFlowMgr::eventFailed( EventContext* eventContext ) const {
  fatal() << "*** Event " << eventContext->evt() << " on slot " << eventContext->slot() << " failed! ***" << endmsg;
  std::ostringstream ost;
  m_algExecStateSvc->dump( ost, *eventContext );
  info() << "Dumping Alg Exec State for slot " << eventContext->slot() << ":\n" << ost.str() << endmsg;
  return StatusCode::FAILURE;
}

void HLTControlFlowMgr::promoteToExecuted( std::unique_ptr<EventContext> eventContext ) const {
  // Check if the execution failed
  if ( m_algExecStateSvc->eventStatus( *eventContext ) != EventStatus::Success )
    eventFailed( eventContext.get() ).ignore();
  int si = eventContext->slot();

  // Schedule the cleanup of the event
  if ( m_algExecStateSvc->eventStatus( *eventContext ) == EventStatus::Success ) {
    if ( msgLevel( MSG::VERBOSE ) )
      verbose() << "Event " << eventContext->evt() << " finished (slot " << si << ")." << endmsg;
  } else {
    fatal() << "Failed event detected on " << *eventContext << endmsg;
  }

  if ( msgLevel( MSG::VERBOSE ) )
    verbose() << "Clearing slot " << si << " (event " << eventContext->evt() << ") of the whiteboard" << endmsg;

  StatusCode sc = m_whiteboard->clearStore( si );
  if ( !sc.isSuccess() ) warning() << "Clear of Event data store failed" << endmsg;
  sc = m_whiteboard->freeStore( si );
  if ( !sc.isSuccess() ) error() << "Whiteboard slot " << eventContext->slot() << " could not be properly cleared";
  ++m_finishedEvt;
  m_createEventCond.notify_all();
}

// scheduling functionality----------------------------------------------------

void HLTControlFlowMgr::buildLines() { // here lines are configured, filled into the node vector m_allVNodes and
                                       // pointers are adjusted

  std::vector<std::string> allChildNames;

  for ( auto const& cfprop : m_compositeCFProperties.value() ) {
    // NodeDef: ["name", "type", ["child1", "child2"], bool ordered]
    // create CompositeNodes with this configuration
    // TODO better dispatch?
    nodeType const nt = nodeTypeNames_inv.at( cfprop.type );
    switch ( nt ) {
    case nodeType::LAZY_AND:
      m_allVNodes.emplace_back( std::in_place_type<CompositeNode<nodeType::LAZY_AND>>, cfprop.name, cfprop.children,
                                cfprop.ordered );
      break;
    case nodeType::LAZY_OR:
      m_allVNodes.emplace_back( std::in_place_type<CompositeNode<nodeType::LAZY_OR>>, cfprop.name, cfprop.children,
                                cfprop.ordered );
      break;
    case nodeType::NONLAZY_OR:
      m_allVNodes.emplace_back( std::in_place_type<CompositeNode<nodeType::NONLAZY_OR>>, cfprop.name, cfprop.children,
                                cfprop.ordered );
      break;
    case nodeType::NONLAZY_AND:
      m_allVNodes.emplace_back( std::in_place_type<CompositeNode<nodeType::NONLAZY_AND>>, cfprop.name, cfprop.children,
                                cfprop.ordered );
      break;
    case nodeType::NOT:
      assert( cfprop.children.size() == 1 );
      m_allVNodes.emplace_back( std::in_place_type<CompositeNode<nodeType::NOT>>, cfprop.name, cfprop.children,
                                cfprop.ordered );
      break;
    default:
      throw GaudiException( "nodeType" + nodeTypeNames.at( nt ) + " does not exist.", __func__, StatusCode::FAILURE );
    }
    for ( auto& childname : cfprop.children ) { allChildNames.emplace_back( childname ); }
  }

  // all childs that are not yet in m_allVNodes are obviously BasicNodes, so add them
  for ( std::string const& childname : allChildNames ) {
    if ( std::all_of( begin( m_allVNodes ), end( m_allVNodes ),
                      [&]( VNode& node ) { return getNameOfVNode( node ) != childname; } ) ) {
      m_allVNodes.emplace_back( BasicNode{childname, msgStream()} );
    }
  }

  // Now we resolve the names of children to pointers
  childrenNamesToPointers( m_allVNodes );

  // detect the motherOfAllNodes (the highest node)
  auto i             = std::find_if( begin( m_allVNodes ), end( m_allVNodes ), [&]( VNode const& node ) {
    return std::all_of( begin( allChildNames ), end( allChildNames ),
                        [&]( std::string_view name ) { return name != getNameOfVNode( node ); } );
  } );
  m_motherOfAllNodes = &*i;

  if ( msgLevel( MSG::DEBUG ) ) {
    for ( auto const& vnode : m_allVNodes ) {
      std::visit( overload{[&]( auto const& node ) {
                             debug() << std::left << "children of " << std::setw( 13 ) << node.getType() << " "
                                     << std::setw( 15 ) << node.m_name << ": " << node.m_children << endmsg;
                           },
                           []( BasicNode const& ) {}},
                  vnode );
    }
  }
}

void HLTControlFlowMgr::configureScheduling() {

  // find all basic nodes by traversing the tree recursively
  auto       allBasics     = reachableBasics( gsl::not_null{m_motherOfAllNodes} );
  auto const allComposites = reachableComposites( gsl::not_null{m_motherOfAllNodes} );

  std::set<std::vector<gsl::not_null<VNode*>>> additionalEdges;

  // translate user edges, defined only by names, to edges of pointers to VNodes
  for ( auto& edge : m_userDefinedEdges ) {
    assert( edge.size() == 2 );
    additionalEdges.emplace( std::vector{gsl::not_null{findVNodeInContainer( edge[0], m_allVNodes )},
                                         gsl::not_null{findVNodeInContainer( edge[1], m_allVNodes )}} );
  }

  using Algorithm = Gaudi::Algorithm;
  // get data dependencies right
  // barrier inputs
  std::vector<std::vector<Algorithm*>> BarrierInputs{m_BarrierAlgNames.size()};
  // fill them

  std::transform( begin( m_BarrierAlgNames ), end( m_BarrierAlgNames ), begin( BarrierInputs ),
                  [&]( auto const& name ) { return m_databroker->algorithmsRequiredFor( name ); } );

  // which ones are also explicit CF nodes?
  std::vector<std::set<gsl::not_null<VNode*>>> explicitDataDependencies{BarrierInputs.size()};

  for ( std::size_t i = 0; i != BarrierInputs.size(); ++i ) {
    for ( Algorithm const* alg : BarrierInputs[i] ) {
      auto node = std::find_if( begin( allBasics ), end( allBasics ),
                                [&]( VNode const* vnode ) { return getNameOfVNode( *vnode ) == alg->name(); } );
      if ( node != std::end( allBasics ) ) { explicitDataDependencies[i].emplace( *node ); }
    }
  }

  std::vector<Algorithm*> allAlgos; // temporarily save/count all algorithms
  allAlgos.reserve( 1000 );         // whatever, 1000 is nice
  // fill algorithms that should definitly run
  for ( std::string const& algname : m_definitlyRunThese ) {
    for ( Algorithm* alg : m_databroker->algorithmsRequiredFor( algname ) ) {
      if ( std::find( begin( allAlgos ), end( allAlgos ), alg ) == end( allAlgos ) ) {
        m_definitlyRunTheseAlgs.emplace_back( alg, allAlgos.size() );
        allAlgos.emplace_back( alg );
      }
    }
  }

  for ( gsl::not_null<VNode*> vnode : allBasics ) {
    std::visit( overload{[&]( BasicNode& node ) {
                           // plug in data dependencies
                           auto reqAlgs = m_databroker->algorithmsRequiredFor( node.m_name, m_BarrierAlgNames );
                           node.m_RequiredAlgs.reserve( reqAlgs.size() );

                           for ( Algorithm* alg : reqAlgs ) {
                             auto index = std::find( begin( allAlgos ), end( allAlgos ), alg );
                             node.m_RequiredAlgs.emplace_back( alg, std::distance( begin( allAlgos ), index ) );
                             if ( index == end( allAlgos ) ) { allAlgos.emplace_back( alg ); }
                           }

                           // additional edges for the barrier. Since there is optional input for the barrier, we need
                           // to add additional edges from each input to each output of the barrier to make sure
                           // stuff runs in the right order. Keep in mind that datadependencies are cut away if they are
                           // optional (stopperalg in m_databroker)
                           for ( AlgWrapper const& reqAlg : node.m_RequiredAlgs ) {
                             for ( std::size_t i = 0; i != m_BarrierAlgNames.size(); ++i ) {
                               if ( reqAlg.name() == m_BarrierAlgNames[i] ) {
                                 for ( gsl::not_null<VNode*> explicitDD : explicitDataDependencies[i] ) {
                                   additionalEdges.emplace( std::vector{explicitDD, vnode} );
                                 }
                               }
                             }
                           }
                         },
                         []( ... ) {}},
                *vnode );
  }

  m_AlgNames.reserve( allAlgos.size() );
  std::transform( begin( allAlgos ), end( allAlgos ), std::back_inserter( m_AlgNames ),
                  []( auto const* alg ) { return alg->name(); } );

  m_AlgStates = decltype( m_AlgStates )( allAlgos.size() );

  m_AlgExecCounters = decltype( m_AlgExecCounters )( allAlgos.size() );

  // end of Data depdendency handling

  // fill the m_parent list of all nodes
  addParentsToAllNodes( allComposites );

  // get all unwrapped control flow edges
  auto allEdges = findAllEdges( gsl::not_null{m_motherOfAllNodes}, additionalEdges );

  // print out the edges
  if ( msgLevel( MSG::DEBUG ) ) debug() << "edges: " << allEdges << endmsg;

  // print out the edges
  if ( msgLevel( MSG::DEBUG ) ) debug() << "additional edges: " << additionalEdges << endmsg;

  // resolve all CF dependencies
  m_orderedNodesVec = resolveDependencies( allBasics, allEdges );

  // print out the order
  if ( msgLevel( MSG::DEBUG ) ) debug() << "ordered nodes: " << m_orderedNodesVec << endmsg;
}

void HLTControlFlowMgr::buildNodeStates() {

  m_NodeStates.reserve( m_allVNodes.size() );
  int helper_index = 0;

  for ( auto& vNode : m_allVNodes ) {
    std::visit( overload{[&]( BasicNode& node ) {
                           node.m_NodeID = helper_index++;
                           m_NodeStates.emplace_back( 1, true );
                         },
                         [&]( auto& node ) {
                           node.m_NodeID = helper_index++;
                           m_NodeStates.emplace_back( node.m_children.size(), true );
                         }},
                vNode );
  }

  // prepare counters
  m_NodeStateCounters = decltype( m_NodeStateCounters )( m_NodeStates.size() );
}

// monitoring and printing functions --------------------------------------------------

// defines the tree to print out, where each element of the vector shall be one line
// we will later append execution and passed states to these lines to print
void HLTControlFlowMgr::registerStructuredTree() {
  assert( m_printableDependencyTree.empty() );
  std::stringstream ss;

  auto print_indented = [&]( VNode const* vnode, int const currentIndent, auto& itself ) -> void {
    // to recursively call this lambda, use auto& itself
    std::visit( overload{[&]( auto const& node ) {
                           std::stringstream ss;
                           ss << std::string( currentIndent, ' ' ) << node.getType() << ": " << node.m_name << " ";
                           m_printableDependencyTree.emplace_back( ss.str() );
                           m_mapPrintToNodeStateOrder.emplace_back( node.m_NodeID ); // to later print it
                           for ( VNode* child : node.m_children ) { itself( child, currentIndent + 1, itself ); }
                         },
                         [&]( BasicNode const& node ) {
                           std::stringstream ss;
                           ss << std::string( currentIndent, ' ' ) << node.m_name << " ";
                           m_printableDependencyTree.emplace_back( ss.str() );
                           m_mapPrintToNodeStateOrder.emplace_back( node.m_NodeID );
                         }},
                *vnode );
  };
  print_indented( m_motherOfAllNodes, 0, print_indented );
}

// determines the size of the largest string in the structured tree vector, to print neatly afterwards
void HLTControlFlowMgr::registerTreePrintWidth() {
  assert( !m_printableDependencyTree.empty() );
  m_maxTreeWidth = ( *std::max_element( begin( m_printableDependencyTree ), end( m_printableDependencyTree ),
                                        []( std::string_view s, std::string_view t ) { return s.size() < t.size(); } ) )
                       .size();
}

// build the full tree
template <typename printable>
std::stringstream HLTControlFlowMgr::buildPrintableStateTree( std::vector<printable> const& states ) const {
  assert( !m_printableDependencyTree.empty() );
  std::stringstream ss;
  ss << '\n';
  for ( auto const& [treeEntry, printToStateIndex] :
        Gaudi::Functional::details::zip::range( m_printableDependencyTree, m_mapPrintToNodeStateOrder ) ) {
    ss << std::left << std::setw( m_maxTreeWidth + 1 ) << treeEntry << states[printToStateIndex] << '\n';
  }
  return ss;
}

template std::stringstream
HLTControlFlowMgr::buildPrintableStateTree<NodeState>( std::vector<NodeState> const& states ) const;

// build the AlgState printout
std::stringstream HLTControlFlowMgr::buildAlgsWithStates( std::vector<uint16_t> const& states ) const {
  std::stringstream ss;
  ss << '\n';
  for ( auto const& [name, state] : Gaudi::Functional::details::zip::range( m_AlgNames, states ) ) {
    ss << std::left << std::setw( 20 ) << name << state << '\n';
  }
  return ss;
}
