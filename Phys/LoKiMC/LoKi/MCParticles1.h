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
#ifndef LOKI_MCPARTICLES1_H
#define LOKI_MCPARTICLES1_H 1
// ============================================================================
// Include files
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/iNode.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/iTree.h"
#include "LoKi/MCTypes.h"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace MCParticles
  {
    // ========================================================================
    /** @class DecNode
     *  simple predicate, which acts on the particleID of the particle
     *  @see LHCb::ParticleID
     *  @see Decays::iNode
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-12-17
     */
    class GAUDI_API DecNode
      : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate
    {
    public:
      // ======================================================================
      /// constructor from the actual node
      DecNode ( const Decays::iNode& node ) ;
      /// constructor from the actual node
      DecNode ( const std::string&   node ) ;
      /// MANDATORY: clone method ("virtual constructor")
      DecNode* clone() const override { return new DecNode ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() ( const LHCb::MCParticle* p ) const  override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    public:
      // ======================================================================
      /// get the decay node
      const Decays::iNode& node() const { return m_node.node () ; }
      /// valid node ?
      bool valid() const { return m_node.valid() ; }
      // validate the node
      StatusCode validate ( const LHCb::IParticlePropertySvc* svc ) const
      { return m_node.validate ( svc ) ; }
      // ======================================================================
    protected:
      // ======================================================================
      void getNode () const ;
      // ======================================================================
    private:
      // ======================================================================
      /// the decay node itself
      mutable Decays::Node m_node   ;                 // the decay node itself
      std::string          m_string ;
      // ======================================================================
    };
    // ========================================================================
    /** @class DecTree
     *  simple predicate, which acts on the decay structuire/tree for the particle
     *  @see Decays::iTree
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-12-17
     */
    class GAUDI_API DecTree
      : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate
    {
    public:
      // ======================================================================
      /// the actual type of tree (interface
      typedef Decays::iTree_<const LHCb::MCParticle*> iTree ; // the tree
      // ======================================================================
    private:
      // ======================================================================
      /// the actual type of tree (assignable)
      typedef Decays::Tree_<const LHCb::MCParticle*>   Tree ; // the tree
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the actual node
      DecTree ( const iTree&       node ) ;
      /// constructor from the actual node
      DecTree ( const std::string& node ) ;
      /// MANDATORY: clone method ("virtual constructor")
      DecTree* clone() const override { return new DecTree ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() ( const LHCb::MCParticle* p ) const  override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    public:
      // ======================================================================
      /// get the decay node
      const iTree& tree() const { return m_tree.tree () ; }
      // ======================================================================
      /// valid tree ?
      bool valid () const { return m_tree.valid() ; }
      // validate the teh tree
      StatusCode validate ( const LHCb::IParticlePropertySvc* svc ) const
      { return m_tree.validate ( svc ) ; }
      // reset the collection
      void reset() const { tree().reset() ; }
      // ======================================================================
    protected:
      // ======================================================================
      void getTree () const ;
      // ======================================================================
    private:
      // ======================================================================
      /// the decay tree itself
      mutable Tree m_tree   ;                         // the decay tree itself
      std::string  m_string ;
      // ======================================================================
    };
    // ========================================================================
  } //                                       end of namespace LoKi::MCParticles
  // ==========================================================================
  namespace Cuts
  {
    // ========================================================================
    /** @typedef MCDECNODE
     *  the trivial predicate whcih acts on ParticleID
     *
     *
     *  @code
     *
     *  // construct some node:
     *  Decays::Node node = Decays::Nodes::Lepton() && Decays::Nodes::Positive() ;
     *
     *  const LHCb::MCParticle* p = .... ;
     *
     *  // create the predicate:
     *  MCCut good = MCDECNODE( node  ) ;
     *
     *  // use the predicate:
     *  const ok = good ( p ) ;
     *
     *  @endcode
     *
     *  @see LHCb::ParticleID
     *  @see LHCb::ParticleID
     *  @see Decays::iNode
     *  @see Decays::Node
     *  @see Decays::Nodes
     *  @author Vanya BELYAEV Ivane.Belyaev@nikhef.nl
     *  @date 2008-12-17
     */
    using MCDECNODE = LoKi::MCParticles::DecNode                              ;
    // ========================================================================
    /** @typedef MCDECTREE
     *  the trivial predicate which acts on decay structure
     *
     *  @code
     *
     *  // construct some tree
     *  Decays::Tree_<const LHCb::MCParticle*> tree =  ... ;
     *
     *  const LHCb::MCParticle* p = .... ;
     *
     *  // create the predicate:
     *  MCCut good = MCDECTREE ( tree  ) ;
     *
     *  // use the predicate:
     *  const ok = good ( p ) ;
     *
     *  @endcode
     *
     *  @see LHCb::ParticleID
     *  @see Decays::iTree_
     *  @see Decays::Tree_
     *  @see Decays::Trees
     *  @author Vanya BELYAEV Ivane.Belyaev@nikhef.nl
     *  @date 2008-12-17
     */
    using MCDECTREE = LoKi::MCParticles::DecTree                              ;
    // ========================================================================
  } // end of namespace LoKi::Cuts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_MCPARTICLES1_H
