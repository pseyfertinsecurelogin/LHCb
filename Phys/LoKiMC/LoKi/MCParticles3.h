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
      result_type    operator   () ( argument   p ) const override ;
      /// OPTIONAL: the nice printout
      std::ostream&  fillStream ( std::ostream& s ) const override ;
      // ======================================================================
    private: 
      // ======================================================================
      /// default constructor is disabled  
      FromInclusive () ; // default constructor is disabled  
      // ======================================================================      
    public:
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
// ============================================================================
