/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// ============================================================================
#ifndef LOKI_CALLS_H
#define LOKI_CALLS_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/SmartRef.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functions.h"
#include "LoKi/BasicFunctors.h"
// ============================================================================
/** @file LoKi/Calls.h
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 */
namespace LoKi
{
  // ==========================================================================
  namespace Dicts
  {
    // ========================================================================
    /** @class FunCalls LoKi/Calls.h
     *
     *  @attention The "generic" implementation fits nicely only
     *             to few classes, for all other cases one needs to
     *             look for template specializations
     *
     *
     *  This file is a part of LoKi project -
     *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
     *
     *  The package has been designed with the kind help from
     *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
     *  contributions and advices from G.Raven, J.van Tilburg,
     *  A.Golutvin, P.Koppenburg have been used in the design.
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-05-29
     */
    template <class TYPE>
    class FunCalls
    {
      // ======================================================================
    private:
      // ======================================================================
      typedef TYPE                                                Type ;
      typedef typename LoKi::BasicFunctors<const Type*>::Function Fun  ;
      typedef LoKi::details::result_t<Fun>                        result_type;
      // ======================================================================
    public:
      // ======================================================================
      static result_type __call__ ( const Fun& fun  , const Type*           o )
      { return fun ( o ) ; }
      static result_type __call__ ( const Fun& fun  , const SmartRef<Type>& o )
      { return fun ( o ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // __rrshift__
      static std::vector<result_type> __rrshift__( const Fun& fun, const typename Type::ConstVector& o )
      { return o >> fun  ; }
      // __rrshift__
      static std::vector<result_type> __rrshift__( const Fun& fun, const typename Type::Vector& o )
      { return o >> fun  ; }
      // __rrshift__
      static result_type __rrshift__ ( const Fun& fun  , const Type*           o )
      { return fun ( o ) ; }
      // __rrshift__
      static result_type __rrshift__ ( const Fun& fun  , const SmartRef<Type>& o )
      { return fun ( o ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // __rshift__
      template <typename F,
                typename TYPE2 = details::type2_t<F>,
                typename TYPE3 = std::enable_if_t<std::is_convertible<TYPE2,double>::value
                                                ||std::is_convertible<TYPE2,bool  >::value,
                               std::conditional_t<std::is_convertible<TYPE2,double>::value,
                                                   double, bool > >,
                typename = std::enable_if_t<std::is_convertible<details::type1_t<F>,double>::value> >
      static LoKi::FunctorFromFunctor<const Type*,TYPE3> __rshift__
      ( const Fun& fun  , F&&  o    )
      { return fun >> o  ; }
      // ======================================================================
    } ;
    // ========================================================================
    /** @class CutCalls Calls.h LoKi/Calls.h
     *
     *  @attention The "generic" implementation fits nicely only
     *             to few classes, for all other cases one needs to
     *             look for template specializations
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-05-29
     */
    template <class TYPE>
    class CutCalls
    {
    private:
      // ======================================================================
      typedef TYPE                                      Type ;
      typedef typename LoKi::Predicate_t<const Type*>   Fun  ;
      typedef LoKi::details::result_t<Fun>              result_type;
      // ======================================================================
    public:
      // ======================================================================
      static result_type __call__ ( const Fun& fun  , const Type*  o )
      { return fun ( o ) ; }
      static result_type __call__ ( const Fun& fun  , const SmartRef<Type>& o )
      { return fun ( o ) ; }
      // ======================================================================
      static typename Type::ConstVector __rrshift__
      ( const Fun& fun  , const typename Type::ConstVector& o ) { return o >> fun  ; }
      static typename Type::Vector __rrshift__
      ( const Fun& fun  , const typename Type::Vector&      o ) { return o >> fun  ; }
      static result_type  __rrshift__( const Fun& fun  , const Type* o )
      { return fun ( o ) ; }
      static result_type  __rrshift__( const Fun& fun  , const SmartRef<Type>& o )
      { return fun ( o ) ; }
      // ======================================================================
      static LoKi::Assignable_t<Fun>
      __rshift__ ( const Fun& fun, const Fun& o ) { return fun >> o  ; }
      // ======================================================================
    } ;
    // ========================================================================
  } //                                             end of namespace LoKi::Dicts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_CALLS_H
// ============================================================================
