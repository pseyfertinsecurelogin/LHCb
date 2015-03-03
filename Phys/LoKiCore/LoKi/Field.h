// $Id: Field.h 53291 2010-08-05 14:35:53Z ibelyaev $
// ============================================================================
#ifndef LOKI_FIELD_H 
#define LOKI_FIELD_H 1
// ============================================================================
// Incldue files 
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Reference.h"
#include "LoKi/ConstReference.h"
// ============================================================================
namespace LoKi 
{
  // ==========================================================================
  /** @struct Field
   *  Helper structure to define a proper data member type 
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2007-06-08
   */
  template <class TYPE>
  struct Field
  {
    // ========================================================================
    /// in general case it is a "reference"
    typedef LoKi::Reference<TYPE> Type ; // in general case it is a "reference"
    // ========================================================================
  };
  // ==========================================================================
  template <class TYPE>
  struct Field<const TYPE>
  {
    // ========================================================================
    /// for const-type it is a const-reference 
    typedef LoKi::ConstReference<TYPE> Type ; //                const-reference 
    // ========================================================================
  };
  // ==========================================================================
  template <class TYPE>
  struct Field<TYPE*>
  {
    // ========================================================================
    /// in case of pointers it is a bare pointer 
    typedef TYPE* Type ;            // in case of pointers it is a bare pointer 
    // ========================================================================
  };
  // ==========================================================================
  template <class TYPE>
  struct Field<TYPE* const>
  {
    // ========================================================================
    /// skip the constness of pointer 
    typedef typename LoKi::Field<TYPE*>::Type Type ;
    // ========================================================================
  };
  // ==========================================================================
  template <class TYPE>
  struct Field<TYPE&>
  {
    // ========================================================================
    /// skip the reference 
    typedef typename Field<TYPE>::Type Type ;             // skip the reference
    // ========================================================================
  };
  // ==========================================================================
  template <class TYPE>
  struct Field<const TYPE&>
  {
    // ========================================================================
    /// skip the reference 
    typedef typename Field<const TYPE>::Type Type ;       // skip the reference
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_FIELD_H
// ============================================================================
