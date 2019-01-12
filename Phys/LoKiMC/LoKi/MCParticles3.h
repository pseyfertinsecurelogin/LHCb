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
#ifndef LOKI_MCPARTICLES3_H
#define LOKI_MCPARTICLES3_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/MCTypes.h"
#include "LoKi/MCParticles1.h"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace MCParticles
  {
    // ========================================================================
    /** @class FromInclusive
     *  Simple predicate that allows to bypass limitations of
     *  decay tree finder and select particles from the inclusive decays
     *
     *  Essentially it is just a shortcut for expression:
     *   "daughter-cut" & ( MCINANCESTORS ( "mother-cut" ) )
     *
     *  Or replacement for "mother -->  ^daughter ..." decay descriptor
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2017-02-07
     */
    class FromInclusive : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate
    {
    public:
      // ======================================================================
      FromInclusive
      ( const LoKi::Types::MCCuts& mother           ,
        const LoKi::Types::MCCuts& daughter         ,
        const bool                 decayOnly = true ) ;
      FromInclusive
      ( const Decays::iNode&       mother           ,
        const Decays::iNode&       daughter         ,
        const bool                 decayOnly = true ) ;
      FromInclusive
      ( const std::string&         mother           ,
        const std::string&         daughter         ,
        const bool                 decayOnly = true ) ;
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      FromInclusive* clone      ()                  const override ;
      /// MANDATORY: the only one essential method
      bool    operator   () ( const LHCb::MCParticle* p ) const override ;
      /// OPTIONAL: the nice printout
      std::ostream&  fillStream ( std::ostream& s ) const override ;
      // ======================================================================
    private:
      // ======================================================================
      /// the final cut
      LoKi::Types::MCCut m_cut ;  // the final cut
      // ======================================================================
    } ;
    // ========================================================================
  } //                                   the end of namespace LoKi::MCParticles
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace Cuts
  {
    // ========================================================================
    /** @typedef MCINCLUSIVE
     *  Simple predicate that allows to bypass limitations of
     *  decay tree finder and select particles from the inclusive decays
     *
     *  @code
     *  MCCut mccut = MCINCLUSIVE ( 'e+' , 'Beauty' )
     *  @endcode
     *
     *  Essentially it is just a shortcut for expression:
     *   "daughter-cut" & ( MCINANCESTORS ( "mother-cut" )
     *
     *  Or replacement for "mother -->  ^daughter ..." decay descriptor
     *
     *  @see LoKi::MCParticles::FromInclusive
     *  @see LoKi::Cuts::MCFROMINCLUSIVE
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2017-02-07
     */
    typedef LoKi::MCParticles::FromInclusive                      MCINCLUSIVE ;
    // ========================================================================
    /** @typedef MCFROMINCLUSIVE
     *  Simple predicate that allows to bypass limitations of
     *  decay tree finder and select particles from the inclusive decays
     *
     *  @code
     *  MCCut mccut = MCFROMINCLUSIVE ( 'e+' , 'Beauty' )
     *  @endcode
     *
     *  Essentially it is just a shortcut for expression:
     *   "daughter-cut" & ( MCINANCESTORS ( "mother-cut" ) )
     *
     *  Or replacement for "mother -->  ^daughter ..." decay descriptor
     *
     *  @see LoKi::MCParticles::FromInclusive
     *  @see LoKi::Cuts::MCINCLUSIVE
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2017-02-07
     */
    typedef LoKi::MCParticles::FromInclusive                  MCFROMINCLUSIVE ;
    // ========================================================================
  }
  // ==========================================================================
}
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_MCPARTICLES3_H
