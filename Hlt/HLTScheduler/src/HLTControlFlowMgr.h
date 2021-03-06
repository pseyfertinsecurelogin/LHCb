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
// @author Niklas Nolte
// much of that code is stolen from Sebastien Ponce's HLTEventLoopMgr
#pragma once

// The following MUST be included before GaudiKernel/Parsers.h,
// which means very early on in the compilation unit.
#include "CFNodePropertiesParse.h"
#include "ThreadPool.h"
// FW includes
#include "GaudiAlg/FunctionalDetails.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/Counters.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/DataSvc.h"
#include "GaudiKernel/EventContext.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IAlgExecStateSvc.h"
#include "GaudiKernel/IAlgorithm.h"
#include "GaudiKernel/IDataBroker.h"
#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/IEvtSelector.h"
#include "GaudiKernel/IHiveWhiteBoard.h"
#include "GaudiKernel/Memory.h"
#include "GaudiKernel/ThreadLocalContext.h"

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iomanip>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

// tbb
#include "tbb/task.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/task_scheduler_observer.h"

// locals
#include "ControlFlowNode.h"

class HLTControlFlowMgr final : public extends<Service, IEventProcessor> {

public:
  /// Standard Constructor
  using extends::extends;

  /// implementation of IService::initialize
  StatusCode initialize() override;
  /// implementation of IService::reinitialize
  StatusCode reinitialize() override { return StatusCode::FAILURE; }
  /// implementation of IService::finalize
  StatusCode finalize() override;

  /// implementation of IEventProcessor::nextEvent
  StatusCode nextEvent( int maxevt ) override;
  /// implementation of IEventProcessor::executeEvent(void* par)
  StatusCode executeEvent( EventContext&& evtContext ) override;
  /// implementation of IEventProcessor::executeRun()
  StatusCode executeRun( int maxevt ) override { return nextEvent( maxevt ); }
  /// implementation of IEventProcessor::stopRun()
  StatusCode stopRun() override;
  /// implementation of IEventProcessor::createEventContext
  EventContext createEventContext() override;

private:
  /// Declare the root address of the event
  std::optional<IOpaqueAddress*> declareEventRootAddress();

  /// Method to check if an event failed and take appropriate actions
  StatusCode eventFailed( EventContext& eventContext ) const;

  /// Algorithm promotion
  void promoteToExecuted( EventContext&& eventContext ) const;

  void buildLines();
  // configuring the execution order
  void configureScheduling();
  // build per-thread state-vector
  void buildNodeStates();

  // helper to release context
  inline StatusCode releaseEvtSelContext() {
    StatusCode sc = StatusCode::SUCCESS;
    auto       c  = m_evtSelContext.release();
    if ( c ) {
      if ( m_evtSelector ) {
        sc = m_evtSelector->releaseContext( c );
      } else {
        delete c;
      }
    }
    return sc;
  }

  // helper to create context
  inline StatusCode createEvtSelContext() {
    auto sc = releaseEvtSelContext();
    if ( sc ) {
      decltype( m_evtSelContext )::pointer c = nullptr;
      sc = ( m_evtSelector ? m_evtSelector->createContext( c ) : StatusCode::FAILURE );
      m_evtSelContext.reset( c );
    }
    return sc;
  }

  // functions to create m_printableDependencyTree
  void registerStructuredTree();
  void registerTreePrintWidth();

private:
  int m_nextevt = 0;

  mutable std::atomic<uint16_t> m_failed_evts_detected = 0;
  mutable bool                  m_shutdown_now         = false;

  std::unique_ptr<ThreadPool> m_debug_pool = nullptr;

private:
  Gaudi::Property<std::string> m_histPersName{this, "HistogramPersistency", "", ""};
  Gaudi::Property<std::string> m_evtsel{this, "EvtSel", "", ""};
  Gaudi::Property<int> m_threadPoolSize{this, "ThreadPoolSize", -1, "Size of the threadpool initialised by TBB"};
  Gaudi::Property<int> m_printFreq{this, "PrintFreq", 1, "Print Frequency for the full algorithm tree"};
  Gaudi::Property<std::vector<NodeDefinition>> m_compositeCFProperties{
      this, "CompositeCFNodes", {}, "Specification of composite CF nodes"};
  Gaudi::Property<std::vector<std::string>> m_BarrierAlgNames{
      this, "BarrierAlgNames", {}, "Names of Barrier (Gather) Algorithms"};
  Gaudi::Property<std::vector<std::vector<std::string>>> m_userDefinedEdges{
      this, "AdditionalCFEdges", {}, "Additional Control Flow Edges defined by the User \
                                      (format: [ [before, after], [before2, after2] ])"};
  Gaudi::Property<std::set<std::string>> m_definitelyRunThese{
      this, "AdditionalAlgs", {}, "Add algs that do not participate in the control flow but should\
                                   definitely run, like e.g. a callgrindprofile"};

  Gaudi::Property<int> m_startTimeAtEvt{this, "StartTimeAtEvt", -1, "start timing at this event. Counting from 0. \
                                        Default choice is deduced from #slots and #evts \
                                        to be reasonably far away from the beginning of processing"};
  Gaudi::Property<int> m_stopTimeAtEvt{this, "StopTimeAtEvt", -1, "stop timing at this event. Counting from 0. \
                                          Default choice is deduced from #slots and #evts \
                                          to be reasonably far away from the end of processing"};

  Gaudi::Property<int> m_stopAfterNFailures{this, "StopAfterNFailures", 3,
                                            "Stop processing if this number of consecutive event failures happened"};

  Gaudi::Property<std::size_t> m_minNameColWidth{this, "MinNameColumnWidth", 46u,
                                                 "Minimum width of component name comlumn in timing table"};
  Gaudi::Property<std::size_t> m_maxNameColWidth{this, "MaxNameColumnWidth", 100u,
                                                 "Maximum width of component name comlumn in timing table"};

  Gaudi::Property<bool> m_createTimingTable{
      this, "CreateTimingTable", true,
      "Activates the use of internal timing counters needed to create a final timing table"};
  // this property is mainly needed to support execution of old algorithms that need to be called via SysExecute like
  // the ones that inherit from DVCommonBase
  Gaudi::Property<bool> m_EnableLegacyMode{
      this, "EnableLegacyMode", false,
      "Call SysExecute of an algorithm. If false algorithms will be called via execute which is faster."};

  /// Reference to the Event Data Service's IDataManagerSvc interface
  IDataManagerSvc* m_evtDataMgrSvc = nullptr;
  /// Reference to the Event Selector
  IEvtSelector* m_evtSelector = nullptr;
  /// Reference to the Histogram Data Service
  IDataManagerSvc* m_histoDataMgrSvc = nullptr;
  /// Reference to the Histogram Persistency Service
  IConversionSvc* m_histoPersSvc = nullptr;
  /// Reference to the Whiteboard
  IHiveWhiteBoard* m_whiteboard = nullptr;
  /// Reference to the AlgExecStateSvc
  IAlgExecStateSvc* m_algExecStateSvc = nullptr;
  // the used databroker
  IDataBroker* m_databroker = nullptr;

  /// atomic count of the number of finished events
  mutable std::atomic<uint32_t> m_finishedEvt{0};
  /// condition variable to wake up main thread when we need to create a new event
  mutable std::condition_variable m_createEventCond;
  /// mutex assoiciated with m_createEventCond condition variable
  std::mutex m_createEventMutex;

  /// event selector context
  std::unique_ptr<IEvtSelector::Context> m_evtSelContext;

  // state vectors for each event, once filled, then copied per event
  std::vector<NodeState>                                       m_NodeStates;
  std::vector<AlgState>                                        m_AlgStates;
  std::vector<Gaudi::Accumulators::AveragingCounter<uint64_t>> m_TimingCounters;
  std::vector<Gaudi::Accumulators::BinomialCounter<uint32_t>>  m_NodeStateCounters;

public:
  using SchedulerStates = decltype( std::pair{m_NodeStates, m_AlgStates} );

private:
  // all controlflownodes
  std::vector<VNode> m_allVNodes;
  // all nodes to execute in ordered manner
  std::vector<gsl::not_null<VNode*>> m_orderedNodesVec;
  // highest node
  VNode* m_motherOfAllNodes = nullptr;

  std::vector<AlgWrapper> m_definitelyRunTheseAlgs;

  // for printing
  // printable dependency tree (will be built during initialize
  std::vector<std::string> m_printableDependencyTree;
  std::vector<std::string> m_AlgNames;
  // map order of print to order of m_NodeStates and m_allVNodes
  std::vector<int> m_mapPrintToNodeStateOrder;
  // maximum width of the dependency tree
  int m_maxTreeWidth{};

  // runtime adding of states to print tree and states
public:
  template <typename printable>
  std::stringstream buildPrintableStateTree( std::vector<printable> const& states ) const;
  std::stringstream buildAlgsWithStates( std::vector<AlgState> const& states ) const;

  // to be able to check which states belong to which node (from the outside)
  auto getNodeNamesWithIndices() {
    std::vector<std::tuple<std::string, int>> names_indices;
    names_indices.reserve( m_allVNodes.size() );
    for ( auto const& vnode : m_allVNodes ) {
      names_indices.emplace_back( std::visit(
          []( auto const& node ) {
            return std::tuple{node.m_name, node.m_NodeID};
          },
          vnode ) );
    }
    return names_indices;
  }
};
