#ifndef RELATIONS_ISCONVERTIBLE_H 
#define RELATIONS_ISCONVERTIBLE_H 1
// ============================================================================
// Include files
// ============================================================================
// ============================================================================
#include <type_traits>
// ============================================================================
namespace Relations 
{
  // ==========================================================================
  /** @class IsConvertible IsConvertible.h Relations/IsConvertible.h
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2006-06-11
   */
  template <class T, class U>
  struct IsConvertible 
  { enum { same  = false , value = std::is_convertible<T,U>::value  } ; } ;
  // ==========================================================================
  template <class T>
  struct IsConvertible<T,T> 
  { enum { same  = true , value = true } ; } ;
  // ==========================================================================
} // end of namespace Relations
// ============================================================================
// The END 
// ============================================================================
#endif // RELATIONS_ISCONVERTIBLE_H
// ============================================================================
