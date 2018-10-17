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
// @author Niklas Nolte, Katya Govorkova
#pragma once

//globals
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <stdlib.h>
#include <thread>
#include <time.h>
#include <variant>
#include <vector>

//locals
#include "CFNodeType.h"

//GaudiCore
#include "GaudiKernel/AppReturnCode.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/MsgStream.h"

struct NodeState {
  int executionCtr;
  bool passed;
  NodeState(int execounter, bool pass) : executionCtr(execounter), passed(pass) {}
};

struct AlgWrapper {
  Algorithm* m_alg;
  int m_executedIndex;

  AlgWrapper (Algorithm* algo, int index) : m_alg(algo), m_executedIndex(index) {
    assert(m_alg != nullptr);
  }

  int isExecuted (std::vector<int> const& AlgoStates) const {
    return AlgoStates[m_executedIndex];
  }

  void setIndex (int i) { m_executedIndex = i; }

  StatusCode execute(EventContext& evtCtx, std::vector<int>& AlgoStates) const {
    m_alg->whiteboard()->selectStore( evtCtx.valid() ? evtCtx.slot() : 0 ).ignore();
    AlgoStates[m_executedIndex] = 1;
    return m_alg->sysExecute( evtCtx );
  }

  bool passed () const { return m_alg->filterPassed(); }

  std::string_view name () const { return m_alg->name(); }
};

// define the overloaded struct to be able to properly use std::visit to get the
// right function depending on the input type see cppreference of std::visit
template <class... Ts> struct overload : Ts... { using Ts::operator()...; };
template <class... Ts> overload(Ts...)->overload<Ts...>;

template <nodeType> // class forward declaration for the VNode typedef
class CompositeNode;

class BasicNode; // class forward declaration for the VNode typedef

using VNode = std::variant<BasicNode, CompositeNode<nodeType::LAZY_AND>, CompositeNode<nodeType::NONLAZY_OR>,
                     CompositeNode<nodeType::NONLAZY_AND>, CompositeNode<nodeType::LAZY_OR>, CompositeNode<nodeType::NOT>>;



// This is the BasicNode implementation, which is a wrapper around
// Gaudi::Functional::Algorithm's that shall be scheduled. It contains a pointer
// to its Gaudi::Functional::Algorithm as member, and moreover implements the
// functionality to be scheduled correctly
class BasicNode final {
public:
  std::string m_name; //should be the same as the Gaudi::Algorithm name
  int m_NodeID = -1; // this is the m_NodeID of the node in a list of all nodes that will be created,
              // to correctly access a state vector
  std::vector<VNode *> m_parents;
  std::vector<AlgWrapper> m_RequiredAlgs; //the last element in the vector is the TopAlg itself
  MsgStream& m_msg;


  BasicNode( std::string const & name, MsgStream& msg )
      : m_name( name ), m_msg(msg){};

  void execute(std::vector<NodeState> &NodeStates, std::vector<int> &AlgStates, EventContext& evtCtx, IAlgExecStateSvc* aess, SmartIF<IProperty> & appmgr) const {
    assert(aess != nullptr);
    //first, execute the required algorithms
    try {
      for ( AlgWrapper const & requiredAlg: m_RequiredAlgs ) {
        if ( !requiredAlg.isExecuted( AlgStates ) ) {
        // if one can guarantee, that every TopAlg is a data consumer, we could omit
        // the isExecuted call for the last element of m_RequiredAlgs
          auto sc = requiredAlg.execute(evtCtx, AlgStates);
          if ( UNLIKELY( !sc.isSuccess() ) ) {
            aess->updateEventStatus( true, evtCtx );
            break;
          }
        }
      }
    } catch ( ... ) {
      aess->updateEventStatus( true, evtCtx );
      m_msg << MSG::FATAL << "Event failed in Node " << m_name << endmsg;
      Gaudi::setAppReturnCode(appmgr, Gaudi::ReturnCode::UnhandledException);
    }

    // the last of m_requiredAlgs is our own Algorithm, depending on which we want to set
    // executionCtr and passed flag of this node
    NodeStates[m_NodeID].executionCtr--;
    NodeStates[m_NodeID].passed = m_RequiredAlgs.back().passed();

  }//end of execute

  void notifyParents(std::vector<NodeState> &NodeStates) const;

  bool requested(std::vector<NodeState> const &NodeStates) const;

}; // end of BasicNode

// This is the implementation of CompositeNodes, like the HLT Line. This gets
// BasicNodes and other CompositeNodes as children and defines a control flow
// for the children. Currently implemented are Nodes for LAZY_AND, executing
// every child until one returns FALSE, and NONLAZY_OR, which sets its state to
// TRUE when one child returns TRUE, but still executes every child.
template <nodeType nType> class CompositeNode final {
public:
  std::string m_name;
  std::vector<VNode *> m_parents;
  std::vector<std::string> m_childrenNames;
  std::vector<VNode *> m_children;
  int m_NodeID = -1;
  bool m_ordered; //do you care about the order of execution?

  CompositeNode(std::string const &name, std::vector<std::string> childrenNames, bool ordered = false)
      : m_name(name), m_childrenNames(std::move(childrenNames)), m_ordered(ordered) {
    assert(!m_childrenNames.empty());
  }


  // this calls this->updateStateAndNotify on all parents of the ControlFlowNode that
  // calls this->notifyParents() and recursively notifyParents again
  void notifyParents(std::vector<NodeState> &NodeStates) const {
    for (VNode *Vparent : m_parents) {
      std::visit(overload{[&](auto &parent) {
                            if (NodeStates[parent.m_NodeID].executionCtr != 0)
                              parent.updateStateAndNotify(m_NodeID, NodeStates);
                          },
                          [](BasicNode &) {}},
                 *Vparent);
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
  bool requested(std::vector<NodeState> const &NodeStates) const {
    return m_parents.empty() ||
      std::any_of(begin(m_parents), end(m_parents), [&](VNode const * Vparent) {
        return std::visit(overload{[&](auto const &parent) { return parent.isActive(NodeStates); },
                                   [](BasicNode const &) { return false; }},
                   *Vparent);
      });
  } // end of requested

  bool isActive(std::vector<NodeState> const & NodeStates) const {
    return NodeStates[m_NodeID].executionCtr != 0 && requested(NodeStates);
  }

  // returns all edges, meaning control-flow dependencies of the
  // ControlFlowNode. This is needed to schedule execution in the right order...
  std::vector<std::vector<VNode *>> Edges() const {
    if (!m_ordered) {
      return {};
    } else {
      std::size_t size = m_children.size() - 1;
      std::vector<std::vector<VNode *>> edges;
      edges.reserve(size);
      for (std::size_t i = 0; i < size; ++i) {
        edges.emplace_back(std::initializer_list<VNode *>{m_children[i], m_children[i + 1]});
      }
      return edges;
    }
  }

  // this should update the passed and executionCtr flags after each
  // loop
  void updateStateAndNotify(int senderNodeID, std::vector<NodeState> &NodeStates) const;

  std::string getType() const {
    return nodeTypeNames.at(nType);
  }

}; // end of class CompositeNode

// ----------DEFINITION OF SCHEDULING UTILITIES---------------------------------------
template<template<typename> class Container>
std::optional<VNode *> findVNodeInContainer(std::string_view name, Container<VNode> & container);

//returns all basic nodes reachable from vnode
std::set<VNode *> reachableBasics(VNode *vnode);
//returns all composite nodes reachable from vnode
std::set<VNode *> reachableComposites(VNode * vnode);
//returns all edges reachable from vnode
std::set<std::vector<std::set<VNode*>>> findAllEdges(VNode const *vnode, std::set<std::vector<VNode *>> const custom_edges = {});
//converts the children given as strings to a node into pointers to the right instances
void childrenNamesToPointers(std::vector<VNode> &allNodes);
//checks whether all controlflowdependencies are met for a nodeToCheck
bool CFDependenciesMet(VNode * nodeToCheck, std::set<std::vector<std::set<VNode *>>> const &setOfEdges,
                       std::vector<VNode *> &alreadyOrdered);
//utiliizes CFDependenciesMet to create a ordered sequence of basic nodes respecting the edge constraints
std::vector<VNode *> resolveDependencies(std::set<VNode *> &unordered,
                                         std::set<std::vector<std::set<VNode *>>> const &setOfEdges);
//based on the child-pointers in a composite node, supply the children with parent pointers
void addParentsToAllNodes(std::set<VNode *> const &composites);
//utility to get the name of a node variant
std::string getNameOfVNode(VNode const &node);
