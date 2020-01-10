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
#ifdef NDEBUG
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  define GSL_UNENFORCED_ON_CONTRACT_VIOLATION
#endif
#include "ControlFlowNode.h"
#ifdef NDEBUG
#  pragma GCC diagnostic pop
#endif

// implements the updateState for the LAZY_AND CompositeNode type: If a child
// did not select anything (did not pass), the LAZY_AND node sets its own
// executed and passed flag and is then considered inactive, not requesting any
// more children. If an executed child returns TRUE, a counter is decremented,
// to be sure to finish execution after every child is executed.
template <>
void CompositeNode<nodeType::LAZY_AND>::updateStateAndNotify( int                   senderNodeID,
                                                              LHCb::span<NodeState> NodeStates ) const {
  if ( !NodeStates[senderNodeID].passed ) {
    NodeStates[m_NodeID].executionCtr = 0;
    NodeStates[m_NodeID].passed       = false;
    notifyParents( NodeStates );
  } else {
    NodeStates[m_NodeID].executionCtr--;
    if ( NodeStates[m_NodeID].executionCtr == 0 ) {
      // NodeStates[m_NodeID].passed = true;  //its true by default, thats why we dont need it here
      notifyParents( NodeStates );
    }
  }
}

template <>
void CompositeNode<nodeType::LAZY_OR>::updateStateAndNotify( int                   senderNodeID,
                                                             LHCb::span<NodeState> NodeStates ) const {
  if ( NodeStates[senderNodeID].passed ) {
    NodeStates[m_NodeID].executionCtr = 0;
    NodeStates[m_NodeID].passed       = true;
    notifyParents( NodeStates );
  } else {
    NodeStates[m_NodeID].executionCtr--;
    if ( NodeStates[m_NodeID].executionCtr == 0 ) {
      NodeStates[m_NodeID].passed = false;
      notifyParents( NodeStates );
    }
  }
}

// implements the updateState for the NONLAZY_OR CompositeNode type: requests
// all children, sets executed state as soon as every child ran and if one child
// passed, this passes as well
template <>
void CompositeNode<nodeType::NONLAZY_OR>::updateStateAndNotify( int, LHCb::span<NodeState> NodeStates ) const {
  NodeStates[m_NodeID].executionCtr--;
  if ( NodeStates[m_NodeID].executionCtr == 0 ) {
    NodeStates[m_NodeID].passed = std::any_of( begin( m_children ), end( m_children ), [&]( VNode const* vchild ) {
      return std::visit( [&]( auto const& child ) { return NodeStates[child.m_NodeID].passed; }, *vchild );
    } );
    notifyParents( NodeStates );
  }
}

template <>
void CompositeNode<nodeType::NONLAZY_AND>::updateStateAndNotify( int, LHCb::span<NodeState> NodeStates ) const {
  NodeStates[m_NodeID].executionCtr--;
  if ( NodeStates[m_NodeID].executionCtr == 0 ) {
    NodeStates[m_NodeID].passed = std::all_of( begin( m_children ), end( m_children ), [&]( VNode const* vchild ) {
      return std::visit( [&]( auto const& child ) { return NodeStates[child.m_NodeID].passed; }, *vchild );
    } );
    notifyParents( NodeStates );
  }
}

template <>
void CompositeNode<nodeType::NOT>::updateStateAndNotify( int, LHCb::span<NodeState> NodeStates ) const {
  NodeStates[m_NodeID].executionCtr--;
  NodeStates[m_NodeID].passed =
      !std::visit( [&]( auto const& child ) { return NodeStates[child.m_NodeID].passed; }, *m_children.front() );
  notifyParents( NodeStates );
}

// just the same as CompositeNode::requested()
bool BasicNode::requested( LHCb::span<NodeState const> NodeStates ) const {
  return m_parents.empty() || std::any_of( begin( m_parents ), end( m_parents ), [&]( VNode const* Vparent ) {
           return std::visit( overload{[&]( auto const& parent ) { return parent.isActive( NodeStates ); },
                                       []( BasicNode const& ) { return false; }},
                              *Vparent );
         } );
} // end of requested

// just the same as CompositeNode::notifyParents
void BasicNode::notifyParents( LHCb::span<NodeState> NodeStates ) const {
  for ( VNode* Vparent : m_parents ) {
    std::visit( overload{[&]( auto& parent ) {
                           if ( NodeStates[parent.m_NodeID].executionCtr != 0 )
                             parent.updateStateAndNotify( m_NodeID, NodeStates );
                         },
                         []( BasicNode& ) {}},
                *Vparent );
  }
}

// ----------DEFINITION OF FUNCTIONS FOR SCHEDULING---------------------------------------

void childrenNamesToPointers( std::vector<VNode>& allNodes ) {
  for ( VNode& vnode : allNodes ) {
    std::visit( overload{[&]( auto& node ) {
                           for ( std::string_view name : node.m_childrenNames ) {
                             auto ptr = findVNodeInContainer( name, allNodes );
                             if ( UNLIKELY( !ptr ) ) {
                               throw GaudiException( "MissConfiguration in Children Names", __func__,
                                                     StatusCode::FAILURE );
                             }
                             node.m_children.emplace_back( ptr );
                           }
                         },
                         []( BasicNode& ) {}},
                vnode );
  }
}

namespace { // helper functions for reachableBasics, reachableComposites and findAllEdges
  void appendBasics( std::set<gsl::not_null<VNode*>>& children, gsl::not_null<VNode*> vnode ) {
    std::visit( overload{[&]( auto const& node ) {
                           for ( gsl::not_null<VNode*> child : node.m_children ) appendBasics( children, child );
                         },
                         [&]( BasicNode const& ) { children.emplace( vnode ); }},
                *vnode );
  }

  void appendComposites( std::set<gsl::not_null<VNode*>>& children, gsl::not_null<VNode*> vnode ) {
    std::visit( overload{[&]( auto const& node ) {
                           children.emplace( vnode );
                           for ( gsl::not_null<VNode*> child : node.m_children ) appendComposites( children, child );
                         },
                         [&]( BasicNode const& ) {}},
                *vnode );
  }

  void appendEdges( std::set<std::vector<std::set<gsl::not_null<VNode*>>>>& allEdges,
                    gsl::not_null<VNode const*>                             vnode ) {
    std::visit(
        overload{[&]( auto const& node ) {
                   for ( auto const& edge : node.Edges() ) {
                     allEdges.emplace( std::vector{reachableBasics( edge.first ), reachableBasics( edge.second )} );
                   }
                   for ( auto const& child : node.m_children ) appendEdges( allEdges, child );
                 },
                 []( BasicNode const& ) {}},
        *vnode );
  }
} // namespace

// function to get all basic grandchildren and children from a CompositeNode
std::set<gsl::not_null<VNode*>> reachableBasics( gsl::not_null<VNode*> vnode ) {
  std::set<gsl::not_null<VNode*>> children;
  appendBasics( children, vnode );
  return children;
}
// same for composite
std::set<gsl::not_null<VNode*>> reachableComposites( gsl::not_null<VNode*> vnode ) {
  std::set<gsl::not_null<VNode*>> composites;
  appendComposites( composites, vnode );
  return composites;
}

// this function shall be called to get a set of edges of structure vector(
// VNode from , VNode to ) and and unwrap those edges to form
// vector(set(BasicNode froms ...) , set(BasicNode tos ...) )
std::set<std::vector<std::set<gsl::not_null<VNode*>>>>
findAllEdges( gsl::not_null<VNode const*> vnode, std::set<std::vector<gsl::not_null<VNode*>>> const custom_edges ) {

  std::set<std::vector<std::set<gsl::not_null<VNode*>>>> allEdges;

  appendEdges( allEdges, vnode );

  for ( auto const& edge : custom_edges ) {
    allEdges.emplace( std::vector{reachableBasics( edge[0] ), reachableBasics( edge[1] )} );
  }

  return allEdges;
}

// works the following way: iterate over the setOfEdges, which is the complete
// set of unwrapped edges. For each time I find the node I want to
// check the dependencies of in the right hand side of an edge, make sure
// that all corresponding left hand sided nodes are already in the
// alreadyOrdered vector. I know, unreadable AF
bool CFDependenciesMet( gsl::not_null<VNode*>                                         nodeToCheck,
                        std::set<std::vector<std::set<gsl::not_null<VNode*>>>> const& setOfEdges,
                        std::vector<gsl::not_null<VNode*>>&                           alreadyOrdered ) {
  return std::all_of( begin( setOfEdges ), end( setOfEdges ), [&]( auto const& unwrappedEdge ) {
    if ( unwrappedEdge[1].find( nodeToCheck ) == end( unwrappedEdge[1] ) )
      return true;
    else {
      return std::all_of( begin( unwrappedEdge[0] ), end( unwrappedEdge[0] ), [&]( auto const& node ) {
        return std::find( begin( alreadyOrdered ), end( alreadyOrdered ), node ) != end( alreadyOrdered );
      } );
    }
  } );
}

// this should resolve the CF and DD dependencies and return a ordered vector which meets
// all dependencies. Pick from unordered, append to ordered and erase from unordered when dependencies met.
std::vector<gsl::not_null<VNode*>>
resolveDependencies( std::set<gsl::not_null<VNode*>>&                              unordered,
                     std::set<std::vector<std::set<gsl::not_null<VNode*>>>> const& setOfEdges ) {
  std::vector<gsl::not_null<VNode*>> ordered;
  ordered.reserve( unordered.size() );
  // check for each loop over unordered, whether at least one node was put into ordered,
  // otherwise it is an infinite loop, which is bad

  for ( bool infiniteLoop = true; !unordered.empty(); ) {
    for ( auto& vnode : unordered ) {
      if ( CFDependenciesMet( vnode, setOfEdges, ordered ) ) {
        infiniteLoop = false;
        ordered.emplace_back( vnode );
        unordered.erase( vnode );
      }
    }
    if ( infiniteLoop ) {
      throw GaudiException( "Dependency circle in control flow, review your configuration", __func__,
                            StatusCode::FAILURE );
    } else {
      infiniteLoop = true; // reset for the next loop, to check again
    }
  }
  return ordered;
}

// fill the parents member of all nodes that are interconnected.
// you can give a list of composite nodes, and all their children's parents-member will
// be filled.
void addParentsToAllNodes( std::set<gsl::not_null<VNode*>> const& composites ) {
  auto get_children = []( VNode const* n ) {
    return std::visit( overload{[]( auto const& node ) { return node.m_children; },
                                []( BasicNode const& ) { return std::vector<gsl::not_null<VNode*>>{}; }},
                       *n );
  };
  for ( gsl::not_null<VNode*> composite : composites ) {
    for ( gsl::not_null<VNode*> node : get_children( composite ) ) {
      std::visit( [&]( auto& toAppendTo ) { return toAppendTo.m_parents.emplace_back( composite ); }, *node );
    }
  }
}

std::string getNameOfVNode( VNode const& node ) {
  return std::visit( []( auto const& node ) { return node.m_name; }, node );
}
