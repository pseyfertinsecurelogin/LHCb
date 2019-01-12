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
#ifndef LOKI_POWER_H
#define LOKI_POWER_H 1
// ============================================================================
// Include files
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/Power.h"
// ============================================================================
// LOKi
// ============================================================================
#include "LoKi/Math.h"
// ============================================================================
/** @file
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
  // ==========================================================================
  /** @class Power Power.h LoKi/Power.h
   *
   *  Simple function which allows the efficient
   *  evaluation of pow(X,N) for LoKi-functions
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@lapp.in2p3.fr
   *  @date   2005-04-09
   */
  // ==========================================================================
  template <class TYPE, class TYPE2=double>
  class Power final : public LoKi::Functor<TYPE,TYPE2>
  {
  private:
    // ========================================================================
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /** Standard constructor
     *  @param fun function to be used in "pow"
     *  @param val power itself
     */
    Power ( LoKi::FunctorFromFunctor<TYPE,TYPE2> fun , int val )
      : LoKi::AuxFunBase ( std::tie ( fun , val ) )
      , m_fun ( std::move(fun) )
      , m_val ( val )
    {}
    /// MANDATORY: clone method ("virtual constructor")
    Power* clone() const override { return new Power ( *this ) ; }
    /// MANDATORY: the only one essential method
    TYPE2 operator() ( argument p ) const override
    {
      if ( 0 == m_val ) { return TYPE2(1) ; }
      // evaluate the function
      auto value = m_fun.fun ( p ) ;
      // evaluate the result
      if ( 0 <  m_val ) { return Gaudi::Math::pow( value, (unsigned long)m_val ); }
      if ( 0 == value )
      { this->Error ( " ZERO in NEGATIVE power! return -1.e+9" ) ; return -1.e+9 ; }
      //
      return Gaudi::Math::pow( 1.0/value, (unsigned long)(-m_val) );
    };
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << "pow("  << m_fun << "," << m_val << ")" ; };
    // ========================================================================
  private:
    // ========================================================================
    /// the functor
    LoKi::FunctorFromFunctor<TYPE,TYPE2>  m_fun ;                // the functor
    /// the power
    int                                   m_val ;                  // the power
    // ========================================================================
  };
  // ==========================================================================
  template <class TYPE2>
  class Power<void,TYPE2> final : public LoKi::Functor<void,TYPE2>
  {
  private:
    // ========================================================================
    /// argument type
    typedef typename LoKi::Functor<void,TYPE2>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /** Standard constructor
     *  @param fun function to be used in "pow"
     *  @param val power itself
     */
    Power ( LoKi::FunctorFromFunctor<void,TYPE2> fun , int val )
      : LoKi::AuxFunBase ( std::tie ( fun , val ) )
      , m_fun ( std::move(fun) )
      , m_val ( val )
    {}
    /// MANDATORY: clone method ("virtual constructor")
    Power* clone() const override { return new Power ( *this ) ; }
    /// MANDATORY: the only one essential method
    TYPE2 operator() (  ) const override
    {
      if ( 0 == m_val ) { return TYPE2(1) ; }
      // evaluate the function
      auto value = m_fun.fun (  ) ;
      // evaluate the result
      if ( 0 <  m_val ) { return Gaudi::Math::pow( value, (unsigned long)m_val ); }
      if ( 0 == value )
      { this->Error ( " ZERO in NEGATIVE power! return -1.e+9" ) ; return -1.e+9 ; }
      //
      return Gaudi::Math::pow( 1.0/value, (unsigned long)(-m_val) );
    }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << "pow("  << m_fun << "," << m_val << ")" ; };
    // ========================================================================
  private:
    // ========================================================================
    /// the functor
    LoKi::FunctorFromFunctor<void,TYPE2>  m_fun ;                // the functor
    /// the power
    int                                   m_val ;                  // the power
    // ========================================================================
  };
  // ==========================================================================
  /** pow for LoKi functions
   *  @see LoKi::Power
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2005-04-09
   */
  template <typename F,
            typename TYPE = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  inline LoKi::Power<TYPE,TYPE2>
  pow ( F&& fun , int val ) { return { std::forward<F>(fun) , val } ; }
  // ==========================================================================
  /** powN for LoKi functions
   *  @see LoKi::pow
   *  @see LoKi::Power
   *  @author Gerhard Raven gerhard.raven@nikhef.nl
   *  @date   2016-09-10
   */
  template <int N, typename F,
            typename TYPE = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  inline LoKi::Power<TYPE,TYPE2>
  pow( F&& fun ) { return { std::forward<F>(fun) , N } ; }

  // ==========================================================================
  /** pow2 for LoKi functions
   *  @see LoKi::pow
   *  @see LoKi::Power
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2005-04-09
   */
  // C++14: template <typename F> constexpr auto pow2<F> = pow<2,F>;
  template <typename F> auto pow2(F&& f) -> decltype( pow(f,2) )
  { return pow(std::forward<F>(f),2); }
  // ==========================================================================
  /** pow3 for LoKi functions
   *  @see LoKi::pow
   *  @see LoKi::Power
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2005-04-09
   */
  //C++14: template <typename F> constexpr auto pow3<F> = pow<3,F>;
  template <typename F> auto pow3(F&& f) -> decltype( pow(f,3) )
  { return pow(std::forward<F>(f),3); }
  // ==========================================================================
  /** pow4 for LoKi functions
   *  @see LoKi::pow
   *  @see LoKi::Power
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2005-04-09
   */
  //C++14: template <typename F> constexpr auto pow3<F> = pow<4,F>;
  template <typename F> auto pow4(F&& f) -> decltype( pow(f,4) )
  { return pow(std::forward<F>(f),4); }
  // ==========================================================================
  /** square for LoKi functions
   *  @see LoKi::pow
   *  @see LoKi::Power
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2005-04-09
   */
  //C++14: template <typename F> constexpr auto square<F> = pow<2,F>;
  template <typename F> auto square(F&& f) -> decltype( pow(f,2) )
  { return pow2(std::forward<F>(f)); }
  // ==========================================================================
} //end of namespace LoKi
// ============================================================================
#endif // LOKI_POWER_H
