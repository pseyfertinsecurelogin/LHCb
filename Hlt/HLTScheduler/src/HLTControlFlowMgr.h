/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
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

// FW includes
#include "GaudiKernel/Algorithm.h"
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
#include "GaudiAlg/FunctionalDetails.h"

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <map>
#include <sstream>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>

//tbb
#include "tbb/task_scheduler_init.h"
#include "tbb/task.h"

//locals
#include "ControlFlowNode.h"
#include "HistogramAgent.h"


class HLTControlFlowMgr : public extends<Service, IEventProcessor>
{

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
  StatusCode executeEvent( void* par ) override;
  /// implementation of IEventProcessor::executeRun()
  StatusCode executeRun( int maxevt ) override { return nextEvent( maxevt ); }
  /// implementation of IEventProcessor::stopRun()
  StatusCode stopRun() override;

private:
  /// Declare the root address of the event
  StatusCode declareEventRootAddress();

  /// Method to check if an event failed and take appropriate actions
  StatusCode eventFailed( EventContext* eventContext ) const;

  /// Algorithm promotion
  void promoteToExecuted( std::unique_ptr<EventContext> eventContext ) const;

  void buildLines();
  //configuring the execution order
  void configureScheduling();
  //build per-thread state-vector
  void buildNodeStates();


private:
  Gaudi::Property<std::string> m_histPersName{this, "HistogramPersistency", "", ""};
  Gaudi::Property<std::string> m_evtsel{this, "EvtSel", "", ""};
  Gaudi::Property<uint16_t> m_threadPoolSize{this, "ThreadPoolSize", -1, "Size of the threadpool initialised by TBB"};
  Gaudi::Property<std::string> m_whiteboardSvcName{this, "WhiteboardSvc", "EventDataSvc", "The whiteboard name"};
  Gaudi::Property<std::string> m_dotfile{
      this, "DotFile", {}, "Name of file to dump dependency graph; if empty, do not dump"};
  Gaudi::Property<int> m_printFreq{this, "PrintFreq", 1, "Print Frequency for the full algorithm tree"};
  Gaudi::Property<std::vector<NodeDefinition>> m_compositeCFProperties{
      this, "CompositeCFNodes", {}, "Specification of composite CF nodes"};
  Gaudi::Property<std::vector<std::string>> m_BarrierAlgNames{
      this, "BarrierAlgNames", {}, "Names of Barrier (Gather) Algorithms"};
  Gaudi::Property<std::vector<std::vector<std::string>>> m_userDefinedEdges{
      this, "AdditionalCFEdges", {}, "Additional Control Flow Edges defined by the User \
                                      (format: [ [before, after], [before2, after2] ])"};
  Gaudi::Property<std::set<std::string>> m_definitlyRunThese{
      this, "AdditionalAlgs", {}, "Add algs that do not participate in the control flow but should\
                                   definitly run, like e.g. a callgrindprofile" };


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
  mutable std::atomic<unsigned int> m_finishedEvt{0};
  /// condition variable to wake up main thread when we need to create a new event
  mutable std::condition_variable m_createEventCond;
  /// mutex assoiciated with m_createEventCond condition variable
  std::mutex m_createEventMutex;

  /// event selector context
  IEvtSelector::Context* m_evtSelContext{nullptr};

  //state vectors for each event, once filled, then copied per event
  std::vector<NodeState> m_NodeStates;
  std::vector<uint16_t> m_AlgStates;

  //all controlflownodes
  std::vector<VNode> m_allVNodes;
  //all nodes to execute in ordered manner
  std::vector<VNode*> m_orderedNodesVec;
  //highest node
  VNode* m_motherOfAllNodes;

  std::vector<AlgWrapper> m_definitlyRunTheseAlgs;

  //for printing
  //printable dependency tree (will be built during initialize
  std::vector<std::string> m_printableDependencyTree;
  std::vector<std::string> m_AlgNames;
  //map order of print to order of m_NodeStates and m_allVNodes
  std::vector<int> m_mapPrintToNodeStateOrder;
  //maximum width of the dependencytree
  int m_maxTreeWidth;

  //functions to create m_printableDependencyTree
  void registerStructuredTree();
  void registerTreePrintWidth();
  //runtime adding of states to print tree and states
  public:
  std::stringstream buildStructuredTreeWithStates(std::vector<NodeState> const & states) const;
  std::stringstream buildAlgsWithStates(std::vector<uint16_t> const & states) const;
};