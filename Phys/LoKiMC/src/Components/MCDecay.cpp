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
// ============================================================================
// Include
// ============================================================================
// Event
// ============================================================================
#include "Event/MCParticle.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/DecayBase.h"
#include "LoKi/IMCDecay.h"
#include "LoKi/MCTreeFactory.h"
#include "LoKi/TreeHelpers.h"
#include "LoKi/Trees.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @class MCDecay
   *  simple implementation of interface Decays::IMCDecay
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-05-27
   */
  class MCDecay : public LoKi::DecayBase, public virtual Decays::IMCDecay {
  protected:
    // ========================================================================
    typedef Decays::Trees::Types_<const LHCb::MCParticle*>::Invalid Invalid;
    // ========================================================================
  public:
    // ========================================================================
    /** create the decay tree from the descriptor
     *  @param descriptor (INPUT)  the decay descriptor
     *  @param tree       (OUTPUT) the decay tree
     */
    Tree tree( const std::string& decay ) const override;
    // ========================================================================
  public:
    // ========================================================================
    /// build the node form the node descriptor
    Decays::Node node( const std::string& descriptor ) const override { return LoKi::DecayBase::node( descriptor ); }
    /** constructor form the type, name and parent
     *  @param type the tool type (?)
     *  @param name the instance name
     *  @param parent the parent
     */
    MCDecay( const std::string& type,   //     the tool type
             const std::string& name,   // the instance name
             const IInterface*  parent ) //        the parent
        : LoKi::DecayBase( type, name, parent ), m_tree( Invalid() ) {
      declareInterface<Decays::IMCDecay>( this );
      declareInterface<Decays::IDecayNode>( this );
    }
    // ========================================================================
  private:
    // ========================================================================
    // the default tree
    mutable Tree m_tree; // the default tree
    // ========================================================================
  }; //                                                   end of class MCDecay
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// create the decay tree from the descriptor
// ============================================================================
Decays::IMCDecay::Tree LoKi::MCDecay::tree( const std::string& decay ) const {
  // check for the default tree
  if ( decay.empty() && m_tree.valid() ) { return m_tree; } // default tree?

  MsgStream& err = error();

  // 1) parse the the descriptor into "generic tree"
  Decays::Parsers::Tree ptree;
  StatusCode            sc = _parse( ptree, decay );
  if ( sc.isFailure() ) {
    Error( "Error from _parse('" + decay + "')", sc );
    return Invalid(); // RETURN
  }
  // 2) convert parsed tree into reasonable decay tree
  Tree tree = Invalid();
  sc        = Decays::Trees::factory( tree, ptree, err.stream() );
  if ( sc.isFailure() ) {
    err << endmsg;
    Error( "Unable to create the tree from '" + ptree.toString() + "'", sc );
    return Invalid(); // RETURN
  }
  // 3) validate the tree
  if ( !tree ) {
    sc = tree.validate( ppSvc() );
    if ( sc.isFailure() ) {
      Error( "Unable to validate the tree '" + tree.toString() + "'", sc );
      return Invalid(); // RETURN
    }
  }

  // 4) store the default tree
  if ( decay.empty() ) { m_tree = tree; } // store the default tree

  // 5) finally:
  return tree;
}
// ============================================================================
//                                                the factory for instantiation
// ============================================================================
DECLARE_COMPONENT( LoKi::MCDecay )
// ============================================================================

// ============================================================================
// The END
// ============================================================================
