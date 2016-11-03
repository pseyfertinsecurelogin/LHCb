// ============================================================================
#ifndef LOKI_PRESCALE_H
#define LOKI_PRESCALE_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
// ============================================================================
// DAQEvent
// ============================================================================
#include "Event/ODIN.h"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace Odin
  {
    // ========================================================================
    /** @class Prescale
     *  Simple "functor"-implementation of Deterministic Prescaler
     *  @see LoKi::Cuts::ODIN_PRESCALE
     *  @see DeterministicPrescaler
     *  The idea & code belongs to Gerhard The Great Raven
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-02-12
     */
    class Prescale : public LoKi::BasicFunctors<const LHCb::ODIN*>::Predicate
    {
    public:
      // ======================================================================
      /// constructor from the accept fraction and the seed
      Prescale ( const double       accept      ,
                 const std::string& seed   = "" ) ;
      /// MANDATORY: clone method ("virtual constructor")
      Prescale* clone() const  override;
      /// MANDATORY: the only essential method
      result_type operator() ( argument o ) const  override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
    private:
      // ======================================================================
      /// the accept fraction
      double       m_accept  ;                           // the accept fraction
      /// the "seed"
      std::string  m_seed    ;                           //          the "seed"
      /// the initial value
      unsigned int m_initial ;                           //   the initial value
      // ======================================================================
    } ;
    // ========================================================================
  } //                                              end of namepsace LoKi::Odin
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_PRESCALE_H
// ============================================================================
