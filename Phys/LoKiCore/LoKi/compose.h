// ============================================================================
#ifndef LOKI_COMPOSE_H 
#define LOKI_COMPOSE_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Primitives.h"
// ============================================================================
namespace LoKi 
{
  // ==========================================================================
  /** compose two functions (with rather liberal interfaces)
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-29
   */
  template <typename F1, typename F2,
            typename TYPE = typename details::LF<F1>::type1,
            typename TYPE1 = typename details::LF<F1>::type2,
            typename TYPE2 = typename details::LF<F2>::type2,
            typename TYPE3 = typename details::LF<F2>::type1 >
  inline 
  LoKi::Compose<TYPE,TYPE1,TYPE2,TYPE3>
  compose ( F1&& fun1, F2&& fun2 )
  {
    return { std::forward<F1>(fun1) , std::forward<F2>(fun2) } ;
  }
  // ==========================================================================
} // end of namespace LoKi 
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_COMPOSE_H
// ============================================================================
