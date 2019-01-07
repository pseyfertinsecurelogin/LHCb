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
#ifndef LOKI_GENCHILDSELECTOR_H
#define LOKI_GENCHILDSELECTOR_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
// ============================================================================
// Kernel/PartProp
// ============================================================================
#include "Kernel/iNode.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/iTree.h"
#include "LoKi/IGenDecay.h"
#include "LoKi/AuxFunBase.h"
#include "LoKi/GenTypes.h"
// ============================================================================
/** @file LoKi/GenChildSelector.h
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya Belyaev  Ivan.Belyaev@cern.ch
 *  @date 2011-12-11
 *
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace GenChild
  {
    // ========================================================================
    /** @class Selector LoKi/GenChildSelector.h
     *
     *  "Child-selector" for HepMC-particles
     *
     *  This file is a part of LoKi project -
     *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
     *
     *  The package has been designed with the kind help from
     *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
     *  contributions and advices from G.Raven, J.van Tilburg,
     *  A.Golutvin, P.Koppenburg have been used in the design.
     *
     *  @author Vanya Belyaev  Ivan.Belyaev@cern.ch
     *  @date   2011-12-13
     */
    // ========================================================================
    class GAUDI_API Selector final : public LoKi::AuxFunBase
    {
    public:
      // ======================================================================
      /// the vector of selectors
      typedef std::vector<LoKi::GenChild::Selector>                    Vector ;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the indices
      Selector ( const std::vector<unsigned int>& indices          ) ;
      /// constructor from decay node
      Selector ( const Decays::iNode&             node             ) ;
      /// constructor from decay tree
      Selector ( const Decays::IGenDecay::iTree&  child            ) ;
      /// constructor from decay tree
      Selector ( const Decays::IGenDecay::Finder& child            ) ;
      /// constructor from decay desctriptor
      Selector ( const std::string&               child            ) ;
      /// constructor from decay desctriptor & factory
      Selector ( const std::string&               child              ,
                 const std::string&               factory          ) ;
      /// constructor from the cut
      Selector ( const LoKi::Types::GCuts&        cut              ) ;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
    public:
      // ======================================================================
      /** get the child according to the selector
       *  @param  head (INPUT) the head fo the decay tree
       *  @return child particle
       */
      const HepMC::GenParticle* child ( const HepMC::GenParticle* head ) const ;
      /** get the child according to the selector
       *  @param head      (INPUT) the head fo the decay tree
       *  @param daughters (UPDATE)
       *  @return number of particles
       */
      unsigned int children
      ( const HepMC::GenParticle*     head      ,
        LoKi::GenTypes::ConstVector&  daughters ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// check the validity the child-selector
      bool valid () const  ;
      /// try to validate it!
      StatusCode validate ( const LHCb::IParticlePropertySvc* svc ) const ;
      // ======================================================================
    private:
      // ======================================================================
      /// build the decay tree form the descriptor
      StatusCode buildTree ( const std::string& descriptor ,
                             const std::string& factory    ) ;
      // ======================================================================
    private:
      // ======================================================================
      /// the child selector by index
      std::vector<unsigned int> m_indices   ;    // the child selector by index
      /// rule to find the child
      Decays::IGenDecay::Finder m_finder    ;    //      rule to find the child
      /// cut set
      bool                      m_setCut    ;    //                     cut set
      /// the cut
      LoKi::Types::GCut         m_cut       ;    //                     the cut
      // ======================================================================
    };
    // ========================================================================
    /** Trivial accessor to the daughter particles for the given particle.
     *  @param  particle (const) pointer to mother particle
     *  @param  dscriptor
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2016-06-21
     */
    GAUDI_API
    const HepMC::GenParticle* child
    ( const HepMC::GenParticle*       particle   ,
      const std::string&              descriptor ) ;
    // ========================================================================
    /** Trivial accessor to the daughter particles for the given particle.
     *  @param  particle (const) pointer to mother particle
     *  @param  cuts     (INPUT) cuts to be used
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2016-06-21
     */
    GAUDI_API
    const HepMC::GenParticle* child
    ( const HepMC::GenParticle* particle   ,
      const LoKi::Types::GCuts& cuts       ) ;
    // ========================================================================
    /** accessor to certain children particles for the given particle
     *  @param  particle   (INPUT) pointer to mother particle
     *  @param  descriptor (INPUT) the descriptor
     *  @return the container of found particles
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2016-06-21
     */
    GAUDI_API
    LoKi::GenTypes::ConstVector
    children
    ( const HepMC::GenParticle*       particle   ,
      const std::string&              descriptor ) ;
    // ========================================================================
    /** accessor to certain children particles for the given particle
     *  @param  particle (INPUT) pointer to mother particle
     *  @param  cuts     (INPUT) cuts to be used
     *  @return the container of found particles
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2016-06-21
     */
    GAUDI_API
    LoKi::GenTypes::ConstVector
    children
    ( const HepMC::GenParticle* particle   ,
      const LoKi::Types::GCuts& cuts       ) ;
    // ========================================================================
  } //                                          end of namespace LoKi::GenChild
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_GENCHILDSELECTOR_H
