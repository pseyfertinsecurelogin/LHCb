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
#ifndef LOKI_OPERATORS_H
#define LOKI_OPERATORS_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Primitives.h"
#include "LoKi/apply.h"
#define LOKI_REQUIRES(...) std::enable_if_t<(__VA_ARGS__),bool> = true
// ============================================================================
/** @file
 *
 *  Inline operators to deal with LoKi functions and predicates
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
 *  @date 2001-01-23
 */
// ============================================================================
namespace LoKi
{

  // ============================================================================
  /** inline construction of logical 'AND'
   *
   *  @code
   *
   *  typedef LoKi::Functor<SomeType,bool>             Pred ;
   *  typedef LoKi::FunctorFromFunctor<SomeType,bool>  PfP  ;
   *
   *  void func( const Pred& A, const Pred& B )
   *  {
   *    PfP p1 = A && B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::And
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2>
  LoKi::And< details::signature_of_t<F1,F2> > operator&&( F1&& fun1, F2&& fun2 )
  {
      return { std::forward<F1>(fun1), std::forward<F2>(fun2) };
  }
  // ============================================================================
  /** inline construction of logical 'OR'
   *
   *  @code
   *
   *  typedef LoKi::Functor<SomeType,bool>             Pred ;
   *  typedef LoKi::FunctorFromFunctor<SomeType,bool>  PfP  ;
   *
   *  void func( const Pred& A, const Pred& B )
   *  {
   *    PfP p1 = A || B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Or
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2>
  LoKi::Or< details::signature_of_t<F1,F2> > operator|| ( F1&& fun1, F2&& fun2 )
  {
      return { std::forward<F1>(fun1), std::forward<F2>(fun2) };
  }
  // ============================================================================
  /** inline construction for logical negate
   *
   *  @code
   *
   *  typedef LoKi::Functor<SomeType,bool>             Pred ;
   *  typedef LoKi::FunctorFromFunctor<SomeType,bool>  PfP  ;
   *
   *  void func( const Pred& A  )
   *  {
   *    PfP p1 = !A            ; // operator form
   *  }
   *
   *  @endcode
   *
   *
   *  @see std::logical_not
   *  @see LoKi::Not
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F>
  LoKi::Not< details::signature_of_t<F> > operator!  ( F&& fun  )
  { return { std::forward<F>( fun ) }  ; }
  // ============================================================================
  /** inline construction for unary negate
   *
   *  @code
   *
   *  typedef LoKi::Functor<SomeType,double>             Func;
   *  typedef LoKi::FunctorFromFunctor<SomeType,double>  FfF  ;
   *
   *  void func( const Func& A  )
   *  {
   *    FfF p1 = -A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *
   *  @see std::negate
   *  @see LoKi::Negate
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F>
  LoKi::Negate< details::signature_of_t<F> > operator-( F&& fun )
  { return { std::forward<F>( fun ) }; }
  // ============================================================================
  /** inline construction of operator "<"
   *
   *  @code
   *
   *  typedef LoKi::Functor<SomeType,double>          Func ;
   *  typedef LoKi::FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const Func& B )
   *  {
   *    PfP f = A < B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Less
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2>
  LoKi::Less<details::signature_of_t<F1,F2>> operator<  ( F1&& fun1, F2&& fun2 )
  { return { std::forward<F1>(fun1), std::forward<F2>(fun2) } ; }
  // ============================================================================
  /** inline construction of operator "<"
   *
   *  @code
   *
   *  typedef LoKi::Functor<SomeType,double>          Func ;
   *  typedef LoKi::FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const double B  )
   *  {
   *    PfP f = A < B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Less
   *  @see LoKi::Constant
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig = details::signature_of_t<F>>
  LoKi::LessThanValue<Sig> operator< ( F&&       fun,
                                     typename LoKi::V2::Constant<Sig>::T2 val )
  { return { std::forward<F>(fun), std::move(val) } ; }
  // ============================================================================
  /** inline construction for operator "<"
   *
   *  @code
   *
   *  typedef LoKi::Functor<SomeType,double>          Func ;
   *  typedef LoKi::FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const double B  )
   *  {
   *    PfP f = B < A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Less
   *  @see LoKi::Constant
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig = details::signature_of_t<F>>
  LoKi::GreaterThanValue<Sig> operator< ( typename LoKi::V2::Constant<Sig>::T2 val,
                                        F&&       fun )
  { return { std::forward<F>(fun), val } ; }
  // ============================================================================
  /** inline construction for operator ">"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const Func& B )
   *  {
   *    PfP f = A > B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Less
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2>
  LoKi::Less<details::signature_of_t<F1,F2>> operator> ( F1&& fun1, F2&& fun2 )
  { return { std::forward<F2>(fun2), std::forward<F1>(fun1) } ; }
  // ============================================================================
  /** inline construction for operator ">"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFrinFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const double B )
   *  {
   *    PfP f = A > B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Less
   *  @see LoKi::Constant
   *  @return functor
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig = details::signature_of_t<F>>
  LoKi::GreaterThanValue<Sig> operator>  ( F&&       fun,
                                         typename V2::Constant<Sig>::T2 val )
  { return { std::forward<F>(fun), val } ; }
  // ============================================================================
  /** inline construction of operator ">"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFrinFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const double B )
   *  {
   *    PfP f = B > A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Less
   *  @see LoKi::Constant
   *  @return functor
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig = details::signature_of_t<F>>
  LoKi::LessThanValue<Sig> operator> ( typename V2::Constant<Sig>::T2 val,
                                     F&& fun  )
  { return { std::forward<F>(fun), val } ; }
  // ============================================================================
  /** inline construction of operator "=="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const Func& B )
   *  {
   *    PfP f = A == B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Equal
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2>
  LoKi::Equal<details::signature_of_t<F1,F2>> operator== ( F1&& fun1, F2&& fun2 )
  { return  { std::forward<F1>(fun1), std::forward<F2>(fun2) } ; }
  // ============================================================================
  /** inline construction of operator "=="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const double B )
   *  {
   *    PfP f = B == A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::EqualToValue
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig= details::signature_of_t<F>>
  LoKi::EqualToValue<Sig> operator==( typename V2::Constant<Sig>::T2 val, F&& fun  )
  { return { std::move(val), std::forward<F>(fun) } ; }
  // ============================================================================
  template <typename F, typename Signature = details::signature_of_t<F>>
  LoKi::EqualToValue<Signature>
  operator== ( F&& fun, typename V2::Constant<Signature>::T2 val )
  { return { std::move(val), std::forward<F>(fun) } ; }
  // ============================================================================
  template <typename F, typename Signature = details::signature_of_t<F>>
  LoKi::NotEqualToValue<Signature> operator!= ( typename  V2::Constant<Signature>::T2 val, F&& fun  )
  { return { std::move(val), std::forward<F>(fun) } ; }
  // ============================================================================
  template <typename F, typename Signature = details::signature_of_t<F>>
  LoKi::NotEqualToValue<Signature> operator!= ( F&& fun, typename V2::Constant<Signature>::T2 val )
  { return { std::move(val), std::forward<F>(fun) } ; }
  // ============================================================================
  /** inline construction of operator "<="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const Func& B )
   *  {
   *    PfP f = A <= B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::LessOrEqual
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2>
  LoKi::LessOrEqual<details::signature_of_t<F1,F2>> operator<= ( F1&&  fun1,
                                                                 F2&&  fun2 )
  { return { std::forward<F1>(fun1), std::forward<F2>(fun2) } ; }
  // ============================================================================
  /** inline construction of operator "<="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const double  B )
   *  {
   *    PfP f = A <= B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::LessOrEqual
   *  @see LoKi::Constant
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Signature = details::signature_of_t<F>>
  LoKi::LessOrEqualValue<Signature> operator<= ( F&&       fun,
                                                 typename V2::Constant<Signature>::T2 val )
  { return { std::forward<F>(fun), std::move(val) } ; }
  // ============================================================================
  /** inline construction of operator "<="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const double  B )
   *  {
   *    PfP f = B <= A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::LessOrEqual
   *  @see LoKi::Constant
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Signature = details::signature_of_t<F>>
  LoKi::GreaterOrEqualValue<Signature> operator<= ( typename V2::Constant<Signature>::T2 val,
                                                  F&& fun )
  { return { std::forward<F>(fun), std::move(val) } ; }
  // ============================================================================
  /** inline construction of operator ">="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const Func&  B )
   *  {
   *    PfP f = A >= B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::LessOrEqual
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2>
  LoKi::LessOrEqual<details::signature_of_t<F1,F2>> operator>=( F1&& fun1, F2&& fun2 )
  { return { std::forward<F2>(fun2), std::forward<F1>(fun1) } ; }
  // ============================================================================
  /** inline construction of operator ">="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const double  B )
   *  {
   *    PfP f = A >= B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::LessOrEqual
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig = details::signature_of_t<F>>
  LoKi::GreaterOrEqualValue<Sig> operator>= ( F&&       fun,
                                              typename V2::Constant<Sig>::T2 val )
  { return { std::forward<F>(fun), val } ; }
  // ============================================================================
  /** inline construction of operator ">="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const double  B )
   *  {
   *    PfP f = B >= A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::LessOrEqual
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig = details::signature_of_t<F>>
  LoKi::LessOrEqualValue<Sig> operator>= ( typename V2::Constant<Sig>::T2 val,
                                           F&&     fun )
  { return { std::forward<F>(fun), std::move(val) } ; }
  // ============================================================================
  /** inline construction of operator "!="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A, const Func& B )
   *  {
   *    PfP f = A != B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::NotEqual
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2>
  LoKi::NotEqual<details::signature_of_t<F1,F2>> operator!= ( F1&&  fun1, F2&&  fun2 )
  { return { std::forward<F1>(fun1), std::forward<F2>(fun2) } ; }
  // ============================================================================
  /** inline construction of operator "+"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>            Func ;
   *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
   *
   *  void func( const Func& A, const Func& B )
   *  {
   *    FfF f = A + B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Plus
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2>
  LoKi::Plus<details::signature_of_t<F1,F2>> operator+ ( F1&&  fun1, F2&&  fun2 )
  { return { std::forward<F1>(fun1), std::forward<F2>(fun2) } ; }
  // ============================================================================
  /** inline construction of operator "+"
   *
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>            Func ;
   *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
   *
   *  void func( const Func& A, const double B )
   *  {
   *    FfF f = A + B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Plus
   *  @see LoKi::Constant
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig = details::signature_of_t<F>>
  LoKi::SumByValue<Sig> operator+ ( F&& fun, typename V2::Constant<Sig>::T2 val )
  { return { std::forward<F>(fun), std::move(val) } ; }
  // ============================================================================
  /** inline construction of operator "+"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>            Func ;
   *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
   *
   *  void func( const Func& A, const double B )
   *  {
   *    FfF f = B + A            ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Plus
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig = details::signature_of_t<F>>
  LoKi::SumByValue<Sig> operator+ ( typename V2::Constant<Sig>::T2 val, F&& fun )
  { return { std::move(val), std::forward<F>(fun) } ; }
  // ============================================================================
  /** inline construction of operator "-"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>            Func ;
   *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
   *
   *  void func( const Func& A, const Func& B )
   *  {
   *    FfF f = A - B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Minus
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2>
  LoKi::Minus<details::signature_of_t<F1,F2>> operator- ( F1&& fun1, F2&& fun2 )
  { return { std::forward<F1>(fun1), std::forward<F2>(fun2) } ; }
  // ============================================================================
  /** inline construction of operator "-"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>            Func ;
   *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
   *
   *  void func ( const Func& A, const double B )
   *  {
   *    FfF f = A - B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Minus
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig = details::signature_of_t<F>>
  LoKi::Minus1<Sig> operator- ( F&& fun, typename V2::Constant<Sig>::T2 val )
  { return { std::forward<F>(fun), std::move(val) } ; }
  // ============================================================================
  /** inline construction of operator "-"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>            Func ;
   *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
   *
   *  void func ( const Func& A, const double B )
   *  {
   *    FfF f = B - A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Minus
   *  @see LoKi::Constant
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig = details::signature_of_t<F>>
  LoKi::Minus2<Sig> operator- ( typename V2::Constant<Sig>::T2 val, F&& fun )
  { return { std::move(val), std::forward<F>(fun) } ; }
  // ============================================================================
  /** inline construction of operator "/"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>            Func ;
   *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
   *
   *  void func ( const Func& A, const Func& B )
   *  {
   *    FfF f = A / B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Divide
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2>
  LoKi::Divide<details::signature_of_t<F1,F2>> operator/ ( F1&& fun1, F2&& fun2 )
  { return { std::forward<F1>(fun1), std::forward<F2>(fun2) } ; }
  // ============================================================================
  /** inline construction of operator "/"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>            Func ;
   *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
   *
   *  void func ( const Func& A, const double B )
   *  {
   *    FfF f = A / B              ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Divide
   *  @see LoKi::Constant
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig = details::signature_of_t<F>>
  LoKi::Divide1<Sig> operator/ ( F&& fun, typename V2::Constant<Sig>::T2 val )
  { return { std::forward<F>(fun), std::move(val) } ; }
  // ============================================================================
  /** inline construction of operator "/"
   *
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>            Func ;
   *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
   *
   *  void func ( const Func& A, const double B )
   *  {
   *    FfF f = B / A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Divide
   *  @see LoKi::Constant
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig = details::signature_of_t<F>>
  LoKi::Divide2<Sig> operator/ ( typename V2::Constant<Sig>::T2 val, F&& fun )
  { return { std::move(val), std::forward<F>(fun) } ; }
  // ============================================================================
  /** inline construction of operator "*"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>            Func ;
   *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
   *
   *  void func ( const Func& A, const Func& B  )
   *  {
   *    FfF f = A * B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Multiply
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2>
  LoKi::Multiply<details::signature_of_t<F1,F2>> operator* ( F1&& fun1, F2&& fun2 )
  { return { std::forward<F1>(fun1), std::forward<F2>(fun2) } ; }
  // ============================================================================
  /** inline construction of operator "*"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>            Func ;
   *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
   *
   *  void func ( const Func& A, const double B )
   *  {
   *    FfF f = A * B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Multiply
   *  @see LoKi::Constant
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig = details::signature_of_t<F>>
  LoKi::MultiplyByValue<Sig> operator* ( F&& fun, typename V2::Constant<Sig>::T2 val )
  { return { std::forward<F>(fun), std::move(val) } ; }
  // ============================================================================
  /** inline construction of operator "*"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>            Func ;
   *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
   *
   *  void func ( const Func& A, const double B )
   *  {
   *    FfF f = B * A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Multiply
   *  @see LoKi::Constant
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F, typename Sig = details::signature_of_t<F>>
  LoKi::MultiplyByValue<Sig> operator* ( typename V2::Constant<Sig>::T2 val, F&& fun )
  { return { std::move(val), std::forward<F>(fun) } ; }
// ============================================================================
template <typename TYPE>
LoKi::EqualToList<TYPE> operator == ( const LoKi::Functor<TYPE,double>& fun,
                                      std::vector<double>        vct )
{ return { fun, std::move(vct) } ; }
// ============================================================================
template <typename TYPE>
LoKi::EqualToList<TYPE> operator == ( const LoKi::Functor<TYPE,double>& fun,
                                      std::vector<int>           vct )
{ return { fun, std::move(vct) } ; }
// ============================================================================
template <typename TYPE>
LoKi::EqualToList<TYPE> operator == ( const LoKi::Functor<TYPE,double>& fun,
                                      std::vector<unsigned int>  vct )
{ return { fun, std::move(vct) } ; }
// ============================================================================
template <typename TYPE>
LoKi::NotEqualToList<TYPE> operator != ( const LoKi::Functor<TYPE,double>& fun,
                                         std::vector<double>        vct )
{ return { fun, std::move(vct) } ; }
// ============================================================================
template <typename TYPE>
LoKi::NotEqualToList<TYPE> operator != ( const LoKi::Functor<TYPE,double>& fun,
                                         std::vector<int>           vct )
{ return { fun, std::move(vct) } ; }
// ============================================================================
template <typename TYPE>
LoKi::NotEqualToList<TYPE> operator != ( const LoKi::Functor<TYPE,double>& fun,
                                         std::vector<unsigned int>  vct )
{ return { fun, std::move(vct) } ; }

// ============================================================================
template <typename TYPE>
LoKi::EqualToList<TYPE> operator == ( std::vector<double>        vct,
                                      const LoKi::Functor<TYPE,double>& fun )
{ return { fun, std::move(vct) } ; }
// ============================================================================
template <typename TYPE>
LoKi::EqualToList<TYPE> operator == ( std::vector<int>           vct,
                                      const LoKi::Functor<TYPE,double>& fun )
{ return { fun, std::move(vct) } ; }
// ============================================================================
template <typename TYPE>
LoKi::EqualToList<TYPE> operator == ( std::vector<unsigned int>  vct,
                                      const LoKi::Functor<TYPE,double>& fun )
{ return { fun, std::move(vct) } ; }
// ============================================================================
template <typename TYPE>
LoKi::NotEqualToList<TYPE> operator != ( std::vector<double>        vct,
                                         const LoKi::Functor<TYPE,double>& fun )
{ return { fun, std::move(vct) } ; }
// ============================================================================
template <typename TYPE>
LoKi::NotEqualToList<TYPE> operator != ( std::vector<int>&          vct,
                                         const LoKi::Functor<TYPE,double>& fun )
{ return { fun, std::move(vct) } ; }
// ============================================================================
template <typename TYPE>
LoKi::NotEqualToList<TYPE> operator != ( std::vector<unsigned int>  vct,
                                         const LoKi::Functor<TYPE,double>& fun )
{ return { fun, std::move(vct) } ; }
// ============================================================================

// ============================================================================
/** inline construction of "modulo" operator
 *
 *  @see LoKi::Modulo
 *
 *  @author Vanya BELYAEV ibelyaev@cern.ch
 *  @date 2011-03-30
 */
template <typename TYPE>
LoKi::Modulo<TYPE> operator% ( const LoKi::Functor<TYPE,double>& divident,
                               unsigned int                      divisor  )
{ return { divident, divisor } ; }
// ============================================================================
} // namespace LoKi


// ============================================================================
#include "LoKi/Value.h"
// ============================================================================
// mixed types: TYPE & void
// ============================================================================
namespace LoKi {
// less
template <typename TYPE, typename TYPE2, LOKI_REQUIRES( !std::is_void<TYPE>::value )>
LoKi::Less<details::sig_t<TYPE,TYPE2>> operator<  ( const Functor<TYPE,TYPE2>&  fun1,
                                              const Functor<void,TYPE2>&  fun2 )
{ return { fun1, LoKi::Value<TYPE,TYPE2> ( fun2 ) } ; }
//
template <typename TYPE, typename TYPE2, LOKI_REQUIRES( !std::is_void<TYPE>::value )>
LoKi::Less<details::sig_t<TYPE,TYPE2>> operator<  ( const Functor<void,TYPE2>&  fun1,
                                              const Functor<TYPE,TYPE2>&  fun2 )
{ return { Value<TYPE,TYPE2> ( fun1 ), fun2 } ; }

// less or equal
template <typename TYPE, typename TYPE2, LOKI_REQUIRES( !std::is_void<TYPE>::value )>
LoKi::LessOrEqual<details::sig_t<TYPE,TYPE2>> operator<= ( const Functor<TYPE,TYPE2>& fun1,
                                                     const Functor<void,TYPE2>& fun2 )
{ return { fun1, LoKi::Value<TYPE,TYPE2> ( fun2 ) } ; }
//
template <typename TYPE, typename TYPE2, LOKI_REQUIRES( !std::is_void<TYPE>::value )>
LoKi::LessOrEqual<details::sig_t<TYPE,TYPE2>> operator<= ( const Functor<void,TYPE2>& fun1,
                                                     const Functor<TYPE,TYPE2>& fun2 )
{ return { Value<TYPE,TYPE2> ( fun1 ), fun2 } ; }

// greater
template <typename TYPE, typename TYPE2, LOKI_REQUIRES( !std::is_void<TYPE>::value )>
LoKi::Less<details::sig_t<TYPE,TYPE2>> operator>  ( const  Functor<TYPE,TYPE2>&  fun1,
                                    const  Functor<void,TYPE2>&  fun2 )
{ return { Value<TYPE,TYPE2> ( fun2 ), fun1 } ; }
//
template <typename TYPE, typename TYPE2, LOKI_REQUIRES( !std::is_void<TYPE>::value )>
LoKi::Less<details::sig_t<TYPE,TYPE2>> operator>  ( const  Functor<void,TYPE2>&  fun1,
                                    const  Functor<TYPE,TYPE2>&  fun2 )
{ return { fun2, Value<TYPE,TYPE2> ( fun1 ) } ; }

// greater or equal
template <typename TYPE, typename TYPE2, LOKI_REQUIRES( !std::is_void<TYPE>::value )>
LoKi::LessOrEqual<details::sig_t<TYPE,TYPE2>> operator>= ( const  Functor<TYPE,TYPE2>&  fun1,
                                           const  Functor<void,TYPE2>&  fun2 )
{ return { Value<TYPE,TYPE2> ( fun2 ), fun1 } ; }
//
template <typename TYPE, typename TYPE2, LOKI_REQUIRES( !std::is_void<TYPE>::value )>
LoKi::LessOrEqual<details::sig_t<TYPE,TYPE2>> operator>= ( const  Functor<void,TYPE2>&  fun1,
                                           const  Functor<TYPE,TYPE2>&  fun2 )
{ return { fun2, Value<TYPE,TYPE2> ( fun1 ) } ; }

// equality
template <typename TYPE, typename TYPE2, LOKI_REQUIRES( !std::is_void<TYPE>::value )>
LoKi::Equal<details::sig_t<TYPE,TYPE2>> operator== ( const Functor<TYPE,TYPE2>&  fun1,
                                     const Functor<void,TYPE2>&  fun2 )
{ return { fun1, Value<TYPE,TYPE2> ( fun2 ) } ; }
//
template <typename TYPE, typename TYPE2, LOKI_REQUIRES( !std::is_void<TYPE>::value )>
LoKi::Equal<details::sig_t<TYPE,TYPE2>> operator== ( const Functor<void,TYPE2>&  fun1,
                                     const Functor<TYPE,TYPE2>&  fun2 )
{ return { Value<TYPE,TYPE2> ( fun1 ), fun2 } ; }

// non-equality
template <typename TYPE, typename TYPE2, LOKI_REQUIRES( !std::is_void<TYPE>::value )>
LoKi::NotEqual<details::sig_t<TYPE,TYPE2>> operator!= ( const Functor<TYPE,TYPE2>&  fun1,
                                        const Functor<void,TYPE2>&  fun2 )
{ return { fun1, Value<TYPE,TYPE2> ( fun2 ) } ; }
//
template <typename TYPE, typename TYPE2, LOKI_REQUIRES( !std::is_void<TYPE>::value )>
LoKi::NotEqual<details::sig_t<TYPE,TYPE2>> operator!= ( const Functor<void,TYPE2>&  fun1,
                                        const Functor<TYPE,TYPE2>&  fun2 )
{ return { Value<TYPE,TYPE2> ( fun1 ), fun2 } ; }


// ============================================================================
} // namespace LoKi


// ============================================================================
// The END
// ============================================================================
#endif // LOKI_OPERATORS_H
