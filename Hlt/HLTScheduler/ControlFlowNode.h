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

  bool requested(std::vector<NodeState> &NodeStates) const;

}; // end of BasicNode

// This is the implementation of CompositeNodes, like the HLT Line. This gets
// BasicNodes and other CompositeNodes as children and defines a control flow
// for the children. Currently implemented are Nodes for LAZY_AND, executing
// every child until one returns FALSE, and NONLAZY_OR, which sets its state to
// TRUE when one child returns TRUE, but still executes every child.
template <nodeType> class CompositeNode final {
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
      std::visit(overload{[this, &NodeStates](auto &parent) {
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
  bool requested(std::vector<NodeState> &NodeStates) const {
    return m_parents.empty() ||
      std::any_of(begin(m_parents), end(m_parents), [&NodeStates](VNode const * Vparent) {
        return std::visit(overload{[&NodeStates](auto const &parent) { return parent.isActive(NodeStates); },
                                   [](BasicNode const &) { return false; }},
                   *Vparent);
      });
  } // end of requested

  bool isActive(std::vector<NodeState> &NodeStates) const {
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

  std::string getType() const;

}; // end of class CompositeNode

  template<nodeType Type>
  std::string CompositeNode<Type>::getType() const{
    return nodeTypeNames.at(Type);
  }

// implements the updateState for the LAZY_AND CompositeNode type: If a child
// did not select anything (did not pass), the LAZY_AND node sets its own
// executed and passed flag and is then considered inactive, not requesting any
// more children. If an executed child returns TRUE, a counter is decremented,
// to be sure to finish execution after every child is executed.
template <>
void CompositeNode<nodeType::LAZY_AND>::updateStateAndNotify(int senderNodeID,
                                                         std::vector<NodeState> &NodeStates) const {
  if (!NodeStates[senderNodeID].passed) {
    NodeStates[m_NodeID].executionCtr = 0;
    NodeStates[m_NodeID].passed = false;
    notifyParents(NodeStates);
  } else {
    NodeStates[m_NodeID].executionCtr--;
    if (NodeStates[m_NodeID].executionCtr == 0) {
      // NodeStates[m_NodeID].passed = true;  //its true by default, thats why we dont need it here
      notifyParents(NodeStates);
    }
  }
}

template <>
void CompositeNode<nodeType::LAZY_OR>::updateStateAndNotify(int senderNodeID,
                                                        std::vector<NodeState> &NodeStates) const {
  if (NodeStates[senderNodeID].passed) {
    NodeStates[m_NodeID].executionCtr = 0;
    NodeStates[m_NodeID].passed = true;
    notifyParents(NodeStates);
  } else {
    NodeStates[m_NodeID].executionCtr--;
    if (NodeStates[m_NodeID].executionCtr == 0) {
      NodeStates[m_NodeID].passed = false;
      notifyParents(NodeStates);
    }
  }
}

// implements the updateState for the NONLAZY_OR CompositeNode type: requests
// all children, sets executed state as soon as every child ran and if one child
// passed, this passes as well
template <>
void CompositeNode<nodeType::NONLAZY_OR>::updateStateAndNotify(int, std::vector<NodeState> &NodeStates) const {
  NodeStates[m_NodeID].executionCtr--;
  if (NodeStates[m_NodeID].executionCtr == 0) {
    NodeStates[m_NodeID].passed = std::any_of(begin(m_children), end(m_children), [&] (VNode* vchild) {
      return std::visit([&](auto &child) { return NodeStates[child.m_NodeID].passed; }, *vchild);
    });
    notifyParents(NodeStates);
  }
}

template <>
void CompositeNode<nodeType::NONLAZY_AND>::updateStateAndNotify(int, std::vector<NodeState> &NodeStates) const {
  NodeStates[m_NodeID].executionCtr--;
  if (NodeStates[m_NodeID].executionCtr == 0) {
    NodeStates[m_NodeID].passed = std::all_of(begin(m_children), end(m_children), [&] (VNode* vchild) {
      return std::visit([&](auto &child) { return NodeStates[child.m_NodeID].passed; }, *vchild);
    });
    notifyParents(NodeStates);
  }
}

template <>
void CompositeNode<nodeType::NOT>::updateStateAndNotify(int, std::vector<NodeState> &NodeStates) const {
  NodeStates[m_NodeID].executionCtr--;
  NodeStates[m_NodeID].passed = !std::visit([&NodeStates](auto &child) {
                                 return NodeStates[child.m_NodeID].passed; }, *m_children.front());
  notifyParents(NodeStates);
}


// just the same as CompositeNode::requested()
bool BasicNode::requested(std::vector<NodeState> &NodeStates) const {
  return m_parents.empty() ||
    std::any_of(begin(m_parents), end(m_parents), [&NodeStates](VNode const * Vparent) {
      return std::visit(overload{[&NodeStates](auto const &parent) { return parent.isActive(NodeStates); },
                                 [](BasicNode const &) { return false; }},
                 *Vparent);
    });
} // end of requested


// just the same as CompositeNode::notifyParents
void BasicNode::notifyParents(std::vector<NodeState> &NodeStates) const {
  for (VNode *Vparent : m_parents) {
    std::visit(overload{[this, &NodeStates](auto &parent) {
                          if (NodeStates[parent.m_NodeID].executionCtr != 0)
                            parent.updateStateAndNotify(m_NodeID, NodeStates);
                        },
                        [](BasicNode &) {}},
               *Vparent);
  }
}

// ----------DEFINITION OF FUNCTIONS FOR SCHEDULING---------------------------------------
template<template<typename> class Container>
std::optional<VNode*> findVNodeInContainer(std::string_view name, Container<VNode> & container) {
  auto it = std::find_if(std::begin(container), std::end(container), [name](auto &cnode) {
        return getNameOfVNode(cnode) == name ;
      });
  if ( it != std::end(container) ) { return (&*it); }
  else { return {}; }
}


void childrenNamesToPointers(std::vector<VNode> &allNodes) {
  for (VNode &vnode : allNodes) {
    std::visit(overload{[&allNodes](auto &node) {
                          for (std::string_view name : node.m_childrenNames) {
                            auto OptPtr = findVNodeInContainer(name, allNodes);
                            if (OptPtr) { node.m_children.push_back(OptPtr.value());
                            } else {
                              throw GaudiException( "MissConfiguration in Children Names", __func__,
                                                    StatusCode::FAILURE );
                            }
                          }
                        },
                        [](BasicNode &) {}},
               vnode);
  }
}

namespace {
  void appendBasics(std::set<VNode*>& children, VNode *vnode){
    assert(vnode != nullptr);
    std::visit(overload{ [&] (auto & node) {
                           for (VNode *child : node.m_children)
                             appendBasics(children,child);
                         },
                         [&] (BasicNode &) {
                           children.emplace(vnode);
                         }
               }, *vnode );
  }

  void appendComposites(std::set<VNode*>& children, VNode *vnode){
    assert(vnode != nullptr);
    std::visit(overload{ [&] (auto & node) {
                           children.emplace(vnode);
                           for (VNode *child : node.m_children)
                             appendComposites(children,child);
                         },
                         [&] (BasicNode &) {}
               }, *vnode );
  }
}

// function to get all basic grandchildren and children from a CompositeNode
std::set<VNode *> reachableBasics(VNode *vnode) {
  assert(vnode != nullptr);
  std::set<VNode *> children;
  appendBasics(children, vnode);
  return children;
}

std::set<VNode *> reachableComposites(VNode *vnode) {
  assert(vnode != nullptr);
  std::set<VNode *> composites;
  appendComposites(composites, vnode);
  return composites;
}

// this function shall be called to get a set of edges of structure vector(
// VNode from , VNode to ) and and unwrap those edges to form
// vector(set(BasicNode froms ...) , set(BasicNode tos ...) )

std::set<std::vector<std::set<VNode *>>> findAllEdges(VNode *variantNode, std::set<std::vector<VNode *>> custom_edges = {}) {
  assert(variantNode != nullptr);

  std::set<std::vector<std::set<VNode *>>> allEdges;

  std::function<void(VNode *)> _findNodeEdges;

  _findNodeEdges = [&allEdges, &_findNodeEdges](VNode *current_variantNode) {
    std::visit(overload{[&allEdges, &_findNodeEdges](auto &node) {
                          for (auto &edge : node.Edges()) {
                            allEdges.emplace(std::vector<std::set<VNode *>>{reachableBasics(edge[0]),
                                                                            reachableBasics(edge[1])});
                          }
                          for (VNode *child : node.m_children)
                            _findNodeEdges(child);
                        },
                        [](BasicNode &) {}},
               *current_variantNode);
  };

  _findNodeEdges(variantNode);

  for (std::vector<VNode *> const & edge : custom_edges ) {
    allEdges.emplace(std::vector<std::set<VNode *>>{reachableBasics(edge[0]),
                                                    reachableBasics(edge[1])});
  }

  return allEdges;
}

// works the following way: iterate over the setOfEdges, which is the complete
// set of unwrapped edges. For each time I find the node I want to
// check the dependencies of in the right hand side of an edge, make sure
// that all corresponding left hand sided nodes are already in the
// alreadyOrdered vector. I know, unreadable AF
bool CFDependenciesMet(VNode *nodeToCheck, std::set<std::vector<std::set<VNode *>>> &setOfEdges,
                       std::vector<VNode *> &alreadyOrdered) {
  assert(nodeToCheck != nullptr);
  return std::all_of(begin(setOfEdges), end(setOfEdges),
                     [nodeToCheck, &alreadyOrdered](std::vector<std::set<VNode *>> const &unwrappedEdge) {
                       if (unwrappedEdge[1].find(nodeToCheck) == end(unwrappedEdge[1]))
                         return true;
                       else {
                         return std::all_of(begin(unwrappedEdge[0]), end(unwrappedEdge[0]),
                                            [&alreadyOrdered](VNode *node) {
                                              return std::find(begin(alreadyOrdered),
                                                               end(alreadyOrdered),
                                                               node) != end(alreadyOrdered);
                                            });
                       }
                     });
}

// this should resolve the CF and DD dependencies and return a ordered vector which meets
// all dependencies. Pick from unordered, append to ordered and erase from unordered when dependencies met.
std::vector<VNode *> resolveDependencies(std::set<VNode *> &unordered,
                                         std::set<std::vector<std::set<VNode *>>> &setOfEdges) {
  std::vector<VNode *> ordered;
  ordered.reserve(unordered.size());
  // check for each loop over unordered, whether at least one node was put into ordered,
  // otherwise it is an infinite loop, which is bad

  for(bool infiniteLoop = true; !unordered.empty();) {
    for (VNode *vnode : unordered) {
      if (CFDependenciesMet(vnode, setOfEdges, ordered)) {
        infiniteLoop = false;
        ordered.emplace_back(vnode);
        unordered.erase(vnode);
      }
    }
    if( infiniteLoop ) {
      throw GaudiException( "Dependency circle in control flow, review your configuration", __func__, StatusCode::FAILURE );
    } else {
      infiniteLoop = true; //reset for the next loop, to check again
    }
  }
  return ordered;
}

// fill the parents member of all nodes that are interconnected.
// you can give a list of composite nodes, and all their children's parents-member will
// be filled.
void addParentsToAllNodes(std::set<VNode *> &composites) {
  auto get_children = [](VNode * n) { return std::visit(overload{
                                               [] (auto& node) { return node.m_children; },
                                               [] (BasicNode&) { return std::vector<VNode*>{}; }
                                             }, *n);
                                    };
  for (VNode *composite : composites) {
    for (VNode *node : get_children(composite)) {
      std::visit([&composite](auto &toAppendTo) { return toAppendTo.m_parents.emplace_back(composite); },
                 *node);
    }
  }
}

inline std::string getNameOfVNode(VNode const &node) {
  return std::visit([](auto &node) { return node.m_name; }, node);
}

