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
// @author Niklas Nolte, Katya Govorkova
#pragma once

// globals
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <gsl/pointers>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <stdlib.h>
#include <thread>
#include <time.h>
#include <variant>
#include <vector>
#include <x86intrin.h>

// locals
#include "CFNodeType.h"

// GaudiCore
#include "GaudiAlg/FunctionalDetails.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/AppReturnCode.h"
#include "GaudiKernel/FunctionalFilterDecision.h"
#include "GaudiKernel/MsgStream.h"

#include "Kernel/STLExtensions.h"

struct NodeState {
  uint16_t executionCtr;
  bool     passed;
};

struct AlgState {
  bool isExecuted;
  bool filterPassed;
};

struct AlgWrapper {
  using Algorithm = Gaudi::Algorithm;
  Algorithm* m_alg{nullptr};
  uint16_t   m_executedIndex{};
  bool       m_callSysExecute{};

  AlgWrapper( Algorithm* algo, uint16_t index, bool callSysExecute )
      : m_alg( algo ), m_executedIndex( index ), m_callSysExecute( callSysExecute ) {
    assert( m_alg != nullptr );
  }

  bool isExecuted( LHCb::span<AlgState const> AlgoStates ) const { return AlgoStates[m_executedIndex].isExecuted; }

  bool getFilterPassed( LHCb::span<AlgState const> AlgoStates ) const {
    return AlgoStates[m_executedIndex].filterPassed;
  }

  void setIndex( uint16_t i ) { m_executedIndex = i; }

  void execute( EventContext& evtCtx, LHCb::span<AlgState> AlgoStates ) const {
    m_alg->whiteboard()->selectStore( evtCtx.valid() ? evtCtx.slot() : 0 ).ignore();

    auto ret = m_callSysExecute ? m_alg->sysExecute( evtCtx ) : m_alg->execute( evtCtx );

    if ( ret == Gaudi::Functional::FilterDecision::PASSED or ret == Gaudi::Functional::FilterDecision::FAILED or
         ret == StatusCode::SUCCESS ) {
      bool filterpassed =
          ret == Gaudi::Functional::FilterDecision::PASSED
              ? true
              : ( ret == Gaudi::Functional::FilterDecision::FAILED ? false
                                                                   : m_alg->execState( evtCtx ).filterPassed() );

      AlgoStates[m_executedIndex] = {true, filterpassed};
      return;
    }
    throw GaudiException( "Error in algorithm execute", m_alg->name(), ret );
  }

  std::string_view name() const { return m_alg->name(); }
};

// define the overloaded struct to be able to properly use std::visit to get the
// right function depending on the input type see cppreference of std::visit
template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overload( Ts... )->overload<Ts...>;

template <nodeType> // class forward declaration for the VNode typedef
class CompositeNode;

class BasicNode; // class forward declaration for the VNode typedef

using VNode =
    std::variant<BasicNode, CompositeNode<nodeType::LAZY_AND>, CompositeNode<nodeType::NONLAZY_OR>,
                 CompositeNode<nodeType::NONLAZY_AND>, CompositeNode<nodeType::LAZY_OR>, CompositeNode<nodeType::NOT>>;

// This is the BasicNode implementation, which is a wrapper around
// Gaudi::Functional::Algorithm's that shall be scheduled. It contains a pointer
// to its Gaudi::Functional::Algorithm as member, and moreover implements the
// functionality to be scheduled correctly
class BasicNode final {
public:
  std::string m_name;        // should be the same as the Gaudi::Algorithm name
  int         m_NodeID = -1; // this is the m_NodeID of the node in a list of all nodes that will be created,
                             // to correctly access a state vector
  std::vector<gsl::not_null<VNode*>> m_parents;
  std::vector<AlgWrapper>            m_RequiredAlgs; // the last element in the vector is the TopAlg itself
  MsgStream&                         m_msg;

  BasicNode( std::string const& name, MsgStream& msg ) : m_name( name ), m_msg( msg ){};

  void execute( LHCb::span<NodeState> NodeStates, LHCb::span<AlgState> AlgStates,
                std::vector<Gaudi::Accumulators::AveragingCounter<uint64_t>>& TimingCounters,
                bool const createTimingTable, EventContext& evtCtx, IAlgExecStateSvc* aess,
                SmartIF<IProperty>& appmgr ) const {
    assert( aess != nullptr );

    // first, execute the required algorithms
    try {
      for ( AlgWrapper const& requiredAlg : m_RequiredAlgs ) {
        if ( !requiredAlg.isExecuted( AlgStates ) ) {
          // if one can guarantee, that every TopAlg is a data consumer, we could omit
          // the isExecuted call for the last element of m_RequiredAlgs
          if ( createTimingTable ) {
            uint64_t const start = __rdtsc();
            requiredAlg.execute( evtCtx, AlgStates );
            TimingCounters[requiredAlg.m_executedIndex] += __rdtsc() - start;
          } else {
            requiredAlg.execute( evtCtx, AlgStates );
            // this is just so that we can use the counter to count executions
            TimingCounters[requiredAlg.m_executedIndex] += 0;
          }
        }
      }
    } catch ( ... ) {
      aess->updateEventStatus( true, evtCtx );
      m_msg << MSG::FATAL << "Event failed in Node " << m_name << endmsg;
      Gaudi::setAppReturnCode( appmgr, Gaudi::ReturnCode::AlgorithmFailure )
          .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
      return;
    }

    // the last of m_requiredAlgs is our own Algorithm, depending on which we want to set
    // executionCtr and passed flag of this node
    NodeStates[m_NodeID].executionCtr--;
    NodeStates[m_NodeID].passed = m_RequiredAlgs.back().getFilterPassed( AlgStates );

  } // end of execute

  void notifyParents( LHCb::span<NodeState> NodeStates ) const;

  bool requested( LHCb::span<NodeState const> NodeStates ) const;

}; // end of BasicNode

// This is the implementation of CompositeNodes, like the HLT Line. This gets
// BasicNodes and other CompositeNodes as children and defines a control flow
// for the children. Currently implemented are Nodes for LAZY_AND, executing
// every child until one returns FALSE, and NONLAZY_OR, which sets its state to
// TRUE when one child returns TRUE, but still executes every child.
template <nodeType nType>
class CompositeNode final {
public:
  std::string                        m_name;
  std::vector<gsl::not_null<VNode*>> m_parents;
  std::vector<std::string>           m_childrenNames;
  std::vector<gsl::not_null<VNode*>> m_children;
  int                                m_NodeID = -1;
  bool                               m_ordered; // do you care about the order of execution?

  CompositeNode( std::string const& name, std::vector<std::string> childrenNames, bool ordered = false )
      : m_name( name ), m_childrenNames( std::move( childrenNames ) ), m_ordered( ordered ) {
    assert( !m_childrenNames.empty() );
  }

  // this calls this->updateStateAndNotify on all parents of the ControlFlowNode that
  // calls this->notifyParents() and recursively notifyParents again
  void notifyParents( LHCb::span<NodeState> NodeStates ) const {
    for ( gsl::not_null<VNode*> Vparent : m_parents ) {
      std::visit( overload{[&]( auto& parent ) {
                             if ( NodeStates[parent.m_NodeID].executionCtr != 0 )
                               parent.updateStateAndNotify( m_NodeID, NodeStates );
                           },
                           []( BasicNode& ) {}},
                  *Vparent );
    }
  }

  // requesting recursion: implemented in requested(), isActive()
  // checks whether the ControlFlowNode that calls it is requested. To do that,
  // it checks whether any parent is active (parent->isActive()). The parents
  // themselves go and check again, whether they are requested themselves if
  // they are not already executed. This goes recursively until we reach reach
  // the highest ControlFlowNode (returning true) or until we reach some not
  // requested or already executed ControlFlowNode (returns false). If we went
  // all the way up to the highest ControlFlowNode, we ask: is this node active?
  // If yes, requested returns true for the second highest ControlFlowNode(s)
  // and we continue to resolve the recursion, asking each composite
  // ControlFlowNode if it is active. If any of the parents of the basic
  // ControlFlowNode, it will be executed.
  bool requested( LHCb::span<NodeState const> NodeStates ) const {
    return m_parents.empty() || std::any_of( begin( m_parents ), end( m_parents ), [&]( VNode const* Vparent ) {
             return std::visit( overload{[&]( auto const& parent ) { return parent.isActive( NodeStates ); },
                                         []( BasicNode const& ) { return false; }},
                                *Vparent );
           } );
  } // end of requested

  bool isActive( LHCb::span<NodeState const> NodeStates ) const {
    return NodeStates[m_NodeID].executionCtr != 0 && requested( NodeStates );
  }

  // returns all edges, meaning control-flow dependencies of the
  // ControlFlowNode. This is needed to schedule execution in the right order...
  std::vector<std::pair<gsl::not_null<VNode*>, gsl::not_null<VNode*>>> Edges() const {
    if ( m_children.empty() || !m_ordered ) {
      return {};
    } else {
      std::vector<std::pair<gsl::not_null<VNode*>, gsl::not_null<VNode*>>> edges;
      edges.reserve( m_children.size() - 1 );
      std::transform( std::next( begin( m_children ) ), end( m_children ), begin( m_children ),
                      std::back_inserter( edges ), []( const auto& second, const auto& first ) {
                        return std::pair{first, second};
                      } );
      return edges;
    }
  }

  // this should update the passed and executionCtr flags after each
  // loop
  void updateStateAndNotify( int senderNodeID, LHCb::span<NodeState> NodeStates ) const;

  std::string getType() const { return nodeTypeNames.at( nType ); }

}; // end of class CompositeNode

// ----------DEFINITION OF SCHEDULING UTILITIES---------------------------------------
template <typename Container, typename = std::enable_if_t<std::is_same_v<typename Container::value_type, VNode>>>
VNode* findVNodeInContainer( std::string_view name, Container& container ) {
  auto it = std::find_if( std::begin( container ), std::end( container ),
                          [name]( auto& cnode ) { return getNameOfVNode( cnode ) == name; } );
  return it != std::end( container ) ? &*it : nullptr;
}

// returns all basic nodes reachable from vnode
std::set<gsl::not_null<VNode*>> reachableBasics( gsl::not_null<VNode*> vnode );
// returns all composite nodes reachable from vnode
std::set<gsl::not_null<VNode*>> reachableComposites( gsl::not_null<VNode*> vnode );
// returns all edges reachable from vnode
std::set<std::vector<std::set<gsl::not_null<VNode*>>>>
findAllEdges( gsl::not_null<VNode const*> vnode, std::set<std::vector<gsl::not_null<VNode*>>> const custom_edges = {} );
// converts the children given as strings to a node into pointers to the right instances
void childrenNamesToPointers( std::vector<VNode>& allNodes );
// checks whether all controlflowdependencies are met for a nodeToCheck
bool CFDependenciesMet( VNode* nodeToCheck, std::set<std::vector<std::set<gsl::not_null<VNode*>>>> const& setOfEdges,
                        std::vector<gsl::not_null<VNode*>>& alreadyOrdered );
// utiliizes CFDependenciesMet to create a ordered sequence of basic nodes respecting the edge constraints
std::vector<gsl::not_null<VNode*>>
resolveDependencies( std::set<gsl::not_null<VNode*>>&                              unordered,
                     std::set<std::vector<std::set<gsl::not_null<VNode*>>>> const& setOfEdges );
// based on the child-pointers in a composite node, supply the children with parent pointers
void addParentsToAllNodes( std::set<gsl::not_null<VNode*>> const& composites );
// utility to get the name of a node variant
std::string getNameOfVNode( VNode const& node );
