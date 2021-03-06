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
#ifndef LOKI_MATH_H
#define LOKI_MATH_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
#include <cmath>
// ============================================================================
// LokiCore
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/Functions.h"
#include "LoKi/Power.h"
#include "LoKi/Primitives.h"
// ============================================================================
/** @file
 *
 *  Collection of math-functions for LoKi functions
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
namespace LoKi {
  // ==========================================================================
  /*  just the exponent for LoKi
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto exp( F&& fun ) {
    return V2::ComposeFunction{std::exp, std::forward<F>( fun ), "exp"};
  }
  // ==========================================================================
  /*  sine for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto sin( F&& fun ) {
    return V2::ComposeFunction{std::sin, std::forward<F>( fun ), "sin"};
  }
  // ==========================================================================
  /*  hyperbolic sine for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto sinh( F&& fun ) {
    return V2::ComposeFunction{std::sinh, std::forward<F>( fun ), "sinh"};
  }
  // ==========================================================================
  /*  cosine for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto cos( F&& fun ) {
    return V2::ComposeFunction{std::cos, std::forward<F>( fun ), "cos"};
  }
  // ==========================================================================
  /*  hyperbolic cosine for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto cosh( F&& fun ) {
    return V2::ComposeFunction{std::cosh, std::forward<F>( fun ), "cosh"};
  }
  // ==========================================================================
  /*  tan for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto tan( F&& fun ) {
    return V2::ComposeFunction{std::tan, std::forward<F>( fun ), "tan"};
  }
  // ==========================================================================
  /*  hyperbolic tangent for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto tanh( F&& fun ) {
    return V2::ComposeFunction{std::tanh, std::forward<F>( fun ), "tanh"};
  }
  // ==========================================================================
  /*  sqrt for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto sqrt( F&& fun ) {
    return V2::ComposeFunction{std::sqrt, std::forward<F>( fun ), "sqrt"};
  }
  // ==========================================================================
  /*  cbrt for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto cbrt( F&& fun ) {
    return V2::ComposeFunction{std::cbrt, std::forward<F>( fun ), "cbrt"};
  }
  // ==========================================================================
  /*  asine for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto asin( F&& fun ) {
    return V2::ComposeFunction{std::asin, std::forward<F>( fun ), "asin"};
  }
  // ==========================================================================
  /*  acos for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto acos( F&& fun ) {
    return V2::ComposeFunction{std::acos, std::forward<F>( fun ), "acos"};
  }
  // ==========================================================================
  /*  atan for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto atan( F&& fun ) {
    return V2::ComposeFunction{std::atan, std::forward<F>( fun ), "atan"};
  }
  // ==========================================================================
  /*  log for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto log( F&& fun ) {
    return V2::ComposeFunction{std::log, std::forward<F>( fun ), "log"};
  }
  // ==========================================================================
  /*  log10 for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto log10( F&& fun ) {
    return V2::ComposeFunction{std::log10, std::forward<F>( fun ), "log10"};
  }
  // ==========================================================================
  /*  abs for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto abs( F&& fun ) {
    return V2::ComposeFunction{std::fabs, std::forward<F>( fun ), "abs"};
  }
  // ==========================================================================
  /*  abs for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto fabs( F&& fun ) {
    return V2::ComposeFunction{std::fabs, std::forward<F>( fun ), "abs"};
  }
  // ==========================================================================
  /*  atan2 for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F1, typename F2, LOKI_REQUIRES( details::is_functor_v<F1>&& details::is_functor_v<F2> )>
  auto atan2( F1&& fun1, F2&& fun2 ) {
    return V2::ComposeFunction2{std::atan2, std::forward<F1>( fun1 ), std::forward<F2>( fun2 ), "atan2"};
  }
  // ==========================================================================
  /*  atan2 for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto atan2( F&& fun1, typename V2::Constant<details::signature_of_t<F>>::T2 val2 ) {
    return V2::ComposeFunction2{std::atan2, std::forward<F>( fun1 ), std::move( val2 ), "atan2"};
  }
  // ==========================================================================
  /*  atan2 for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto atan2( typename V2::Constant<details::signature_of_t<F>>::T2 val1, F&& fun2 ) {
    return V2::ComposeFunction2{std::atan2, std::move( val1 ), std::forward<F>( fun2 ), "atan2"};
  }
  // ==========================================================================
  /*  atan  for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F1, typename F2, LOKI_REQUIRES( details::is_functor_v<F1>&& details::is_functor_v<F2> )>
  auto atan( F1&& fun1, F2&& fun2 ) {
    return V2::ComposeFunction2{atan2( std::forward<F1>( fun1 ), std::forward<F2>( fun2 ) )};
  }
  // ==========================================================================
  /*  atan  for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto atan( F&& fun1, typename V2::Constant<details::signature_of_t<F>>::T2 val2 ) {
    return V2::ComposeFunction2{atan2( std::forward<F>( fun1 ), std::move( val2 ) )};
  }
  // ==========================================================================
  /*  atan  for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  auto atan( typename V2::Constant<details::signature_of_t<F>>::T2 val1, F&& fun2 ) {
    return V2::ComposeFunction2{atan2( std::move( val1 ), std::forward<F>( fun2 ) )};
  }
  // ==========================================================================
  /*  pow for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F1, typename F2, LOKI_REQUIRES( details::is_functor_v<F1>&& details::is_functor_v<F2> )>
  auto pow( F1&& fun1, F2&& fun2 ) {
    return ComposeFunction2{std::pow, std::forward<F1>( fun1 ), std::forward<F2>( fun2 ), "pow"};
  }
  // ==========================================================================
  /*  pow for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  LoKi::ComposeFunction2<details::signature_of_t<F>> pow( F&&                                                   fun1,
                                                          typename V2::Constant<details::signature_of_t<F>>::T2 val2 ) {
    return {std::pow, std::forward<F>( fun1 ), std::move( val2 ), "pow"};
  }
  // ==========================================================================
  /*  pow for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F>
  LoKi::ComposeFunction2<details::signature_of_t<F>> pow( typename V2::Constant<details::signature_of_t<F>>::T2 val1,
                                                          F&&                                                   fun2 ) {
    return {std::pow, std::move( val1 ), std::forward<F>( fun2 ), "pow"};
  }
  // ==========================================================================
  /*  minimum for LoKi functions
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2004-08-10
   */
  template <typename F1, typename F2>
  LoKi::Min<details::signature_of_t<F1, F2>> min( F1&& fun1, F2&& fun2 ) {
    return {std::forward<F1>( fun1 ), std::forward<F2>( fun2 )};
  }
  // ==========================================================================
  template <typename F>
  LoKi::Min<details::signature_of_t<F>> min( const F&                                              fun1,
                                             typename V2::Constant<details::signature_of_t<F>>::T2 val2 ) {
    return {fun1, V2::Constant<details::signature_of_t<F>>{std::move( val2 )}};
  }
  // ==========================================================================
  template <typename F>
  LoKi::Min<details::signature_of_t<F>> min( typename V2::Constant<details::signature_of_t<F>>::T2 val1,
                                             const F&                                              fun2 ) {
    return {V2::Constant<details::signature_of_t<F>>{val1}, fun2};
  }
  // ==========================================================================
  template <typename F1, typename F2, typename... F3>
  LoKi::Min<details::signature_of_t<F1, F2, F3...>> min( F1&& fun1, F2&& fun2, F3&&... fun3 ) {
    return min( min( std::forward<F1>( fun1 ), std::forward<F2>( fun2 ) ), std::forward<F3>( fun3 )... );
  }
  // ==========================================================================
  /*  maximum for LoKi functions
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2004-08-10
   */
  template <typename F1, typename F2>
  LoKi::Max<details::signature_of_t<F1, F2>> max( F1&& fun1, F2&& fun2 ) {
    return {std::forward<F1>( fun1 ), std::forward<F2>( fun2 )};
  }
  // ==========================================================================
  template <typename F>
  LoKi::Max<details::signature_of_t<F>> max( F&& fun1, typename V2::Constant<details::signature_of_t<F>>::T2 val2 ) {
    return {std::forward<F>( fun1 ), V2::Constant<details::signature_of_t<F>>{std::move( val2 )}};
  }
  // ==========================================================================
  template <typename F>
  LoKi::Max<details::signature_of_t<F>> max( typename V2::Constant<details::signature_of_t<F>>::T2 val1, F&& fun2 ) {
    return {V2::Constant<details::signature_of_t<F>>{std::move( val1 )}, std::forward<F>( fun2 )};
  }
  // ==========================================================================
  template <typename F1, typename F2, typename... F3>
  LoKi::Max<details::signature_of_t<F1, F2, F3...>> max( F1&& fun1, F2&& fun2, F3&&... fun3 ) {
    return max( max( std::forward<F1>( fun1 ), std::forward<F2>( fun2 ) ), std::forward<F3>( fun3 )... );
  }
  // ==========================================================================
  /** helpful function for creation of comparison object
   *  @param fun1 function to be applied to the first object to be compared
   *  @param fun2 function to be applied to the second object to be compared
   *  @param cmp comparison criteria for result of the function evaluation
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename F1, typename F2, typename CMP = std::less<>>
  auto compare( const F1& fun1, const F2& fun2, CMP cmp = {} ) {
    return LoKi::Compare{fun1, fun2, std::move( cmp )};
  }
  // ==========================================================================
  /** select element form the sequence with minimal value of
   *  given function.
   *  It is just an extension of the STL <tt>std::min_element</tt>
   *  standard algorithm
   *  @code
   *  SEQUENCE prts = ... ;
   *  SEQUENCE::iterator particle =
   *              select_min( prts.begin() , prts.end() , PT );
   *  @endcode
   *  Where <tt>SEQUENCE</tt> could be any container or container-like
   *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
   *  This example illustrates the selection of particle with minimal
   *  transverse momentum  from "container" <tt>prts</tt>.
   *  @see PT
   *  @see LoKi::Function
   *  @param first  'begin' iterator of the sequence
   *  @param last   'end'   iterator of the sequence
   *  @param fun    function
   *  @return iterator for the element with minimal value of function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-09-05
   */
#define LOKI_REQUIRES( ... ) std::enable_if_t<( __VA_ARGS__ ), bool> = true

  template <typename Iterator, typename F,
            LOKI_REQUIRES( std::is_convertible_v<typename std::iterator_traits<Iterator>::reference,
                                                 LoKi::details::argument_t<F>> )>
  Iterator select_min( Iterator first, Iterator last, const F& fun ) {
    return std::min_element( first, last, LoKi::compare( fun, fun, std::less{} ) );
  }
  // ==========================================================================
  /** select element form the sequence with maximal value of function
   *  It is just an extension of the STL <tt>std::min_element</tt>
   *  standard algorithm
   *  @code
   *  SEQUENCE prts = ... ;
   *  SEQUENCE::iterator particle =
   *              select_max( prts.begin() , prts.end() , CL );
   *  @endcode
   *  Where <tt>SEQUENCE</tt> could be any container or container-like
   *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
   *  This example illustrates the selection of particle with maximal
   *  convidence level from "container" <tt>prts</tt>.
   *  @see CL
   *  @see LoKi::Function
   *  @param first  'begin' iterator of the sequence
   *  @param last   'end'   iterator of the sequence
   *  @param fun    function
   *  @return iterator for the element with maximal value of function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-09-05
   */
  template <typename Iterator, typename F,
            LOKI_REQUIRES( std::is_convertible_v<typename std::iterator_traits<Iterator>::reference,
                                                 LoKi::details::argument_t<F>> )>
  Iterator select_max( Iterator first, Iterator last, const F& fun ) {
    return std::max_element( first, last, LoKi::compare( fun, fun, std::less{} ) );
  }
  // ==========================================================================
  /** select element form the sequence with minimal value of function
   *
   *  It is just an extension of the STL <tt>std::min_element</tt>
   *  standard algorithm
   *  @code
   *  SEQUENCE prts = ... ;
   *  const Particle* particle = select_min( prts , PT );
   *  @endcode
   *  Where <tt>SEQUENCE</tt> could be any container or container-like
   *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
   *  This example illustrates the selection of particle with minimal
   *  transverse momentum  from "container" <tt>prts</tt>.
   *  @see PT
   *  @see LoKi::Function
   *  @param sequence sequence/container/range
   *  @param fun      function
   *  @return element with minimal value of function
   *  @attention be careful with empty ranges/containers/sequences
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-09-05
   */
  template <typename Sequence, typename F,
            LOKI_REQUIRES( std::is_convertible_v<typename Sequence::value_type, LoKi::details::argument_t<F>> )>
  typename Sequence::value_type select_min( Sequence& sequence, F&& fun ) {
    if ( sequence.begin() == sequence.end() ) { return {}; }
    return *select_min( sequence.begin(), sequence.end(), std::forward<F>( fun ) );
  }
  // ==========================================================================
  /** select element form the sequence with maximal value of function
   *
   *  It is just an extension of the STL <tt>std::min_element</tt>
   *  standard algorithm
   *  @code
   *  SEQUENCE prts = ... ;
   *  const Particle* particle = select_max( prts , CL );
   *  @endcode
   *  Where <tt>SEQUENCE</tt> could be any container or container-like
   *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
   *  This example illustrates the selection of particle with maximal
   *  confidence level from "container" <tt>prts</tt>.
   *  @see CL
   *  @see LoKi::Function
   *  @param sequence sequence/container/range
   *  @param fun      function
   *  @return element with maximal value of function
   *  @attention be careful with empty ranges/containers/sequences
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-09-05
   */
  template <typename Sequence, typename F,
            LOKI_REQUIRES( std::is_convertible_v<typename Sequence::value_type, LoKi::details::argument_t<F>> )>
  typename Sequence::value_type select_max( Sequence& sequence, F&& fun ) {
    if ( sequence.begin() == sequence.end() ) { return {}; }
    return *select_max( sequence.begin(), sequence.end(), std::forward<F>( fun ) );
  }
  // ==========================================================================
  /** select element form the sequence with minimal value of
   *  given function
   *  It is just an extension of the STL <tt>std::min_element</tt>
   *  standard algorithm with additional conditions
   *  @code
   *  SEQUENCE prts = ... ;
   *  SEQUENCE::iterator particle =
   *              select_min( prts.begin() ,
   *                          prts.end()   ,
   *                          PT           ,
   *                          P > 10 * GeV );
   *  @endcode
   *  Where <tt>SEQUENCE</tt> could be any container or container-like
   *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
   *  This example illustrates the selection of particle with minimal
   *  transverse momentum  from "container" <tt>prts</tt>. Only
   *  particles with momentum in excess of 10 GeV are considered.
   *
   *  @see PT
   *  @see LoKi::Function
   *  @param first  'begin' iterator of the sequence
   *  @param last   'end'   iterator of the sequence
   *  @param fun    function
   *  @param cut    predicate which need to be satisfied
   *  @return iterator for the element with minimal value of function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2005-03-09
   */
  template <typename Iterator, typename Fun, typename Cut,
            LOKI_REQUIRES( std::is_convertible_v<typename std::iterator_traits<Iterator>::reference,
                                                 LoKi::details::argument_t<Fun>> ),
            LOKI_REQUIRES( std::is_convertible_v<typename std::iterator_traits<Iterator>::reference,
                                                 LoKi::details::argument_t<Cut>> )>
  Iterator select_min( Iterator first, Iterator last, const Fun& fun, const Cut& cut ) {
    first = std::find_if( first, last, std::cref( cut ) );
    if ( first == last ) { return last; }
    //
    const auto cmp = LoKi::compare( fun, fun, std::less{} );
    //
    auto result = first++;
    for ( ; first != last; ++first ) {
      if ( cut( *first ) && cmp( *first, *result ) ) { result = first; }
    }
    //
    return result;
  }
  // ==========================================================================
  /** select element form the sequence with maximal value of
   *  given function
   *
   *  It is just an extension of the STL <tt>std::max_element</tt>
   *  standard algorithm with additional conditions
   *
   *  @code
   *  SEQUENCE prts = ... ;
   *  SEQUENCE::iterator particle =
   *              select_max( prts.begin() ,
   *                          prts.end()   ,
   *                          PT           ,
   *                          P > 10 * GeV );
   *  @endcode
   *
   *  Where <tt>SEQUENCE</tt> could be any container or container-like
   *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
   *  This example illustrates the selection of particle with maximal
   *  transverse momentum  from "container" <tt>prts</tt>. Only
   *  particles with momentum in excess of 10 GeV are considered.
   *
   *  @see PT
   *  @see LoKi::Function
   *  @param first  'begin' iterator of the sequence
   *  @param last   'end'   iterator of the sequence
   *  @param fun    function
   *  @param cut    predicate which need to be satisfied
   *  @return iterator for the element with minimal value of function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2005-03-09
   */
  template <typename Iterator, typename Fun, typename Cut,
            LOKI_REQUIRES( std::is_convertible_v<typename std::iterator_traits<Iterator>::reference,
                                                 LoKi::details::argument_t<Fun>> ),
            LOKI_REQUIRES( std::is_convertible_v<typename std::iterator_traits<Iterator>::reference,
                                                 LoKi::details::argument_t<Cut>> )>
  Iterator select_max( Iterator first, Iterator last, const Fun& fun, const Cut& cut ) {
    first = std::find_if( first, last, std::cref( cut ) );
    if ( first == last ) { return last; }

    const auto cmp    = LoKi::compare( fun, fun, std::less{} );
    auto       result = first++;
    for ( ; first != last; ++first ) {
      if ( cut( *first ) && cmp( *result, *first ) ) { result = first; }
    }
    return result;
  }
  // ==========================================================================
  /** "in-range" function
   *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *   @date 2009-11-21
   */
  template <typename F, typename = details::require_result_t<F, double>>
  auto inRange( double low, F&& fun, double high ) {
    return V2::InRange{low, std::forward<F>( fun ), high};
  }
  // ==========================================================================
  /** "in-range" function
   *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *   @date 2009-11-21
   */
  template <typename F1, typename F2, typename F3, typename = details::require_result_t<F1, double>,
            typename = details::require_result_t<F2, double>, typename = details::require_result_t<F3, double>>
  auto inRange( F1&& low, F2&& fun, F3&& high ) {
    return V2::InRange2{std::forward<F1>( low ), std::forward<F2>( fun ), std::forward<F3>( high )};
  }
  // ==========================================================================
  /** "in-range" function
   *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *   @date 2009-11-21
   */
  template <typename F1, typename F2, typename = details::require_result_t<F1, double>,
            typename = details::require_result_t<F2, double>>
  auto inRange( double low, F1&& fun, F2&& high ) {
    return V2::InRange2{low, std::forward<F1>( fun ), std::forward<F2>( high )};
  }
  // ==========================================================================
  /** "in-range" function
   *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *   @date 2009-11-21
   */
  template <typename F1, typename F2, typename = details::require_result_t<F1, double>,
            typename = details::require_result_t<F2, double>>
  auto inRange( F1&& low, F2&& fun, double high ) {
    return V2::InRange2{std::forward<F1>( low ), std::forward<F2>( fun ), high};
  }
  // ==========================================================================
  /** "in-range" function
   *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *   @date 2009-11-21
   */
  template <typename F1, typename F2, typename F3, typename = details::require_result_t<F1, double>,
            typename = details::require_result_t<F2, double>, typename = details::require_result_t<F3, double>>
  auto in_range( F1&& low, F2&& fun, F3&& high ) {
    return V2::InRange2{std::forward<F1>( low ), std::forward<F2>( fun ), std::forward<F3>( high )};
  }
  // ==========================================================================
  /** "in-range" function
   *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *   @date 2009-11-21
   */
  template <typename F1, typename F2, typename = details::require_result_t<F1, double>,
            typename = details::require_result_t<F2, double>>
  auto in_range( double low, F1&& fun, F2&& high ) {
    return inRange( low, std::forward<F1>( fun ), std::forward<F2>( high ) );
  }
  // ==========================================================================
  /** "in-range" function
   *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *   @date 2009-11-21
   */
  template <typename F1, typename F2, typename = details::require_result_t<F1, double>,
            typename = details::require_result_t<F2, double>>
  auto in_range( F1&& low, F2&& fun, double high ) {
    return inRange( std::forward<F1>( low ), std::forward<F2>( fun ), high );
  }
  // ==========================================================================
  /** "scale" predicate function
   *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *   @date 2009-12-06
   */
  template <typename F1, typename F2, typename = details::require_result_t<F1, bool>,
            typename = details::require_signature<F2, void, bool>>
  auto scale( F1&& cut, F2&& sf ) {
    return V2::XScaler{std::forward<F1>( cut ), std::forward<F2>( sf )};
  }
  // ==========================================================================
  /** "round" the floating result to integer value
   *   @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *   @date 2011-04-02
   */
  template <typename F, typename = details::require_result_t<F, double>>
  auto round( F&& fun ) {
    return V2::Round<details::type1_t<F>>{std::forward<F>( fun )};
  }
  // ==========================================================================
  /** "jbit" the floating result to integer value
   *  get the jth bit of value.
   *  The action :
   *   - 1. f -> round ( f )
   *   - 2. f -> abs   ( f )
   *   - 3. Gaudi::Math::bit ( f , j )
   *  @see LoKi::JBit
   *  @see Gaudi::Math::bit
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-04-02
   */
  template <typename F, typename = details::require_result_t<F, double>>
  auto jbit( F&& fun, unsigned short j ) {
    return V2::JBit{std::forward<F>( fun ), j};
  }
  // ==========================================================================
  /** "jbits" the floating result to integer value
   *  get the (j1-j2) bits of value.
   *  The action :
   *   - 1. f -> round ( f )
   *   - 2. f -> abs   ( f )
   *   - 3. Gaudi::Math::bits ( f , j1 , j2  )
   *  @see LoKi::JBits
   *  @see Gaudi::Math::bits
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-04-02
   */
  template <typename F, typename = details::require_result_t<F, double>>
  auto jbits( F&& fun, unsigned short j1, unsigned short j2 ) {
    return V2::JBits{std::forward<F>( fun ), j1, j2};
  }

  // ==========================================================================
  /** "jdigit" the floating result to integer value
   *  get the jth decimal digit of value.
   *  The action :
   *   - 1. f -> round ( f )
   *   - 2. f -> abs   ( f )
   *   - 3. Gaudi::Math::digit ( f , j )
   *  @see LoKi::JDigit
   *  @see Gaudi::Math::digit
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2017-02-16
   */
  template <typename F, typename = details::require_result_t<F, double>>
  auto jdigit( F&& fun, unsigned short j ) {
    return V2::JDigit{std::forward<F>( fun ), j};
  }
  // ==========================================================================
  /** "jdigits" the floating result to integer value
   *  get the (j1-j2) digits of value.
   *  The action :
   *   - 1. f -> round ( f )
   *   - 2. f -> abs   ( f )
   *   - 3. Gaudi::Math::digits ( f , j1 , j2  )
   *  @see LoKi::JDigits
   *  @see Gaudi::Math::digits
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2017-02-16
   */
  template <typename F, typename = details::require_result_t<F, double>>
  auto jdigits( F&& fun, unsigned short j1, unsigned short j2 ) {
    return V2::JDigits{std::forward<F>( fun ), j1, j2};
  }
  // ==========================================================================
} //                                          end of namespace LoKi
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace Math {
    // ========================================================================
    struct Abs_t {
      template <class TYPE>
      auto operator()( TYPE i ) const {
        return std::abs( i );
      }
    };
    inline constexpr Abs_t _Abs_{};
    // ========================================================================
    template <class TYPE = void, class TYPE1, class TYPE2 = std::conditional_t<std::is_void_v<TYPE>, TYPE1, TYPE>>
    std::vector<TYPE2> _abs_( const std::vector<TYPE1>& vct ) {
      std::vector<TYPE2> res;
      res.reserve( vct.size() );
      std::transform( vct.begin(), vct.end(), std::back_inserter( res ), _Abs_ );
      return res;
    }
    // ========================================================================
    template <class TYPE1>
    std::vector<TYPE1> _abs_( std::vector<TYPE1>&& vct ) {
      std::transform( vct.begin(), vct.end(), vct.begin(), _Abs_ );
      return vct;
    }
    // ========================================================================
  } // namespace Math
  // ==========================================================================
} // namespace LoKi
// ============================================================================
namespace std {
  //
  //
  inline std::vector<unsigned int> abs( const std::vector<int>& vct ) { return LoKi::Math::_abs_<unsigned int>( vct ); }
  inline std::vector<unsigned long> abs( const std::vector<long>& vct ) {
    return LoKi::Math::_abs_<unsigned long>( vct );
  }
  inline std::vector<unsigned short> abs( const std::vector<short>& vct ) {
    return LoKi::Math::_abs_<unsigned short>( vct );
  }
  inline std::vector<float>  abs( const std::vector<float>& vct ) { return LoKi::Math::_abs_( vct ); }
  inline std::vector<double> abs( const std::vector<double>& vct ) { return LoKi::Math::_abs_( vct ); }
  //
  // just for completenes
  //
  inline const std::vector<unsigned int>&   abs( const std::vector<unsigned int>& vct ) { return vct; }
  inline const std::vector<unsigned short>& abs( const std::vector<unsigned short>& vct ) { return vct; }
  inline const std::vector<unsigned long>&  abs( const std::vector<unsigned long>& vct ) { return vct; }
  // ==========================================================================
} // namespace std
#undef LOKI_REQUIRES
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_MATH_H
