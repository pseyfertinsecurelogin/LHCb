// $Id: ValueWithError.cpp 141156 2012-06-17 12:51:56Z ibelyaev $
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <cmath>
#include <string>
#include <sstream>
#include <climits>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GAUDI_VERSION.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/Lomont.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/ToStream.h"
// ============================================================================
// local
// ============================================================================
#include "LHCbMath/ValueWithError.h"
#include "LHCbMath/Power.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/format.hpp"
#include "boost/math/special_functions/fpclassify.hpp"
// ============================================================================
/** @file
 *  Implementation file for class Gaudi::Math::ValueWithError
 *  @date 2009-06-03
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 */
// ============================================================================
#ifdef __INTEL_COMPILER                                   // Disable ICC remark
//            floating-point equality and inequality comparisons are unreliable
#pragma warning(disable:1572) 
#endif
// ============================================================================
// local namespace to hide the details
// ============================================================================
namespace
{
  // ==========================================================================
  const unsigned int _maxULPs = 10000 ;
  // ==========================================================================
  inline bool _equal ( const double value1 ,
                       const double value2 )
  { return value1 == value2 ||
      Gaudi::Math::lomont_compare_double ( value1 ,value2 , _maxULPs ) ; }
  // ==========================================================================
  // check if the double value close to zero
  inline bool _zero  ( const double value ) { return _equal ( value , 0 ) ; }
  // ==========================================================================
  // check if the double value close to one
  inline bool _one   ( const double value ) { return _equal ( value , 1 ) ; }
  // ==========================================================================
  /// helper wrapper
  inline double _pow ( const double v , const int n )
  {
    return
      0 <= n ?
      Gaudi::Math::pow ( v , (unsigned long) n ) :
      std::pow         ( v , n                 ) ;
  }
  // ==========================================================================
}
// ============================================================================
// constructor from the value and covariance
// ============================================================================
Gaudi::Math::ValueWithError::ValueWithError
( const double value      ,
  const double covariance )
  : m_value ( value      )
  , m_cov2  ( covariance )
{}
// ============================================================================
// constructor from the (value,error)-pair
// ============================================================================
Gaudi::Math::ValueWithError::ValueWithError
( const std::pair<double,double>& value )
  : m_value ( value.first )
  , m_cov2  ()
{
  setError ( value.second ) ;
}
// ============================================================================
// constructor from textual representation
// ============================================================================
Gaudi::Math::ValueWithError::ValueWithError
( const std::string& value )
  : m_value ()
  , m_cov2  ()
{
  StatusCode sc = Gaudi::Parsers::parse ( *this , value ) ;
  if ( sc.isFailure() )
  { throw GaudiException ( "Unable to parse ValueWithError: '" + value + "'" ,
                           "Gaudi::Math" , sc ) ; }
}
// ============================================================================
// set the error
// ============================================================================
void Gaudi::Math::ValueWithError::setError ( const double e )
{
  m_cov2  = e * e ;
  if ( 0 > e ) { m_cov2 = -m_cov2 ; }
}
// ============================================================================
// get the error
// ============================================================================
double Gaudi::Math::ValueWithError::error      () const
{ return 0 <= m_cov2 ? std::sqrt ( m_cov2 ) : -std::sqrt ( -m_cov2 ) ; }
// ============================================================================
// +=
// ============================================================================
Gaudi::Math::ValueWithError&
Gaudi::Math::ValueWithError::operator+=
( const Gaudi::Math::ValueWithError& right )                             // +=
{
  //
  if ( &right == this ) 
  {
    m_value *= 2  ;
    m_cov2  *= 4  ;
    return  *this ;
  }
  //
  m_value += right.m_value ;
  m_cov2  += right.m_cov2  ;
  return *this ;
}
// ============================================================================
// -=
// ============================================================================
Gaudi::Math::ValueWithError&
Gaudi::Math::ValueWithError::operator-=
( const Gaudi::Math::ValueWithError& right )                              // -=
{
  //
  if ( &right == this ) 
  {
    m_value = 0   ;
    m_cov2  = 0   ;
    return  *this ;
  }
  //
  m_value -= right.m_value ;
  m_cov2  += right.m_cov2  ;
  return *this ;
}
// ============================================================================
// *=
// ============================================================================
Gaudi::Math::ValueWithError&
Gaudi::Math::ValueWithError::operator*=
( const Gaudi::Math::ValueWithError& right )                              // *=
{
  if ( &right == this ) 
  {
    const double a = value() ;
    m_value  =     a * a ;
    m_cov2  *= 4 * a * a ;
    return  *this ;
  }
  //
  const double _a2 =       m_value *       m_value ;
  const double _b2 = right.m_value * right.m_value ;
  m_cov2  *= _b2                 ;
  m_cov2  += _a2 * right.m_cov2  ;
  m_value *=      right.m_value ;
  return *this ;
}
// ============================================================================
// /=
// ============================================================================
Gaudi::Math::ValueWithError&
Gaudi::Math::ValueWithError::operator/=
( const Gaudi::Math::ValueWithError& right )                              // /=
{
  if ( &right == this ) 
  {
    m_value  =  1 ;
    m_cov2   =  0 ;
    return  *this ;
  }
  //
  const double _a2 =       m_value *       m_value ;
  const double _b2 = right.m_value * right.m_value ;
  const double _b4 = _b2 * _b2 ;
  //
  m_cov2  /= _b2 ;
  m_cov2  += ( _a2 / _b4 ) * right.m_cov2 ;
  m_value /= right.m_value ;
  return *this ;
}
// ============================================================================
// *=
// ============================================================================
Gaudi::Math::ValueWithError&
Gaudi::Math::ValueWithError::operator*= ( const double v )                // *=
{
  m_value *= v     ;
  m_cov2  *= (v*v) ;
  return *this ;
}
// ============================================================================
// /=
// ============================================================================
Gaudi::Math::ValueWithError&
Gaudi::Math::ValueWithError::operator/= ( const double v )                // /=
{
  m_value /= v     ;
  m_cov2  /= (v*v) ;
  return *this ;
}
// ============================================================================
// +=
// ============================================================================
Gaudi::Math::ValueWithError&
Gaudi::Math::ValueWithError::operator+=( const double right )             // +=
{
  m_value += right ;
  return *this ;
}
// ============================================================================
// -=
// ============================================================================
Gaudi::Math::ValueWithError&
Gaudi::Math::ValueWithError::operator-= ( const double right )            // -=
{
  m_value -= right ;
  return *this ;
}
// ============================================================================
// unary-
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::operator-() const                        // unary-
{ return ValueWithError( -value() , cov2() ) ; }
// ============================================================================
// printout
// ============================================================================
std::ostream&
Gaudi::Math::ValueWithError::fillStream ( std::ostream& s ) const
{ return s << "( " << m_value << " +- " << error() << " )" ; }
// ============================================================================
// printout using format
// ============================================================================
std::ostream&
Gaudi::Math::ValueWithError::fillStream
( std::ostream&      s   ,
  const std::string& fmt ) const
{
  using namespace boost::io ;
  //
  boost::format printer ( fmt) ;
  printer.exceptions ( all_error_bits ^ ( too_many_args_bit | too_few_args_bit ) ) ;
  printer % value () % error () ;
  //
  return s << printer ;
}
// ============================================================================
// conversion to string
// ============================================================================
std::string Gaudi::Math::ValueWithError::toString   () const
{
  std::ostringstream s ;
  fillStream ( s ) ;
  return s.str() ;
}
// ============================================================================
// conversion to the string using format
// ============================================================================
std::string Gaudi::Math::ValueWithError::toString
( const std::string& fmt ) const
{
  std::ostringstream s ;
  fillStream ( s , fmt ) ;
  return s.str() ;
}
// ============================================================================
// evaluate the mean of a and b
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::mean
( const Gaudi::Math::ValueWithError& b ) const
{
  //
  if ( &b == this ) { return *this ; } // self-mean ???
  //
  if      ( 0 >=   cov2 () && 0 >= b.cov2 () ) 
  { return 0.5 * ( value() + b.value() ) ; }
  else if ( 0 >=   cov2 ()                   ) { return *this ; }
  else if ( 0 >= b.cov2 ()                   ) { return b     ; }
  //
  const double _cov2 = 1.0/( 1.0/cov2() + 1.0/b.cov2() ) ;
  //
  return Gaudi::Math::ValueWithError
    ( _cov2 * ( value()/cov2() + b.value()/b.cov2() ) ,  _cov2 ) ;
}
// =============================================================================
// evaluate chi2
// =============================================================================
double Gaudi::Math::ValueWithError::chi2
( const Gaudi::Math::ValueWithError& b ) const
{
  //
  if ( _equal ( value () , b.value() ) ) { return 0 ; } // RETURN
  //
  const double s_cov2 = cov2() + b.cov2() ;
  if ( 0 >= s_cov2 )                     { return -1 ; } // RETURN
  //
  const double diff = value() - b.value() ;
  return diff*diff/s_cov2 ;
}
// =============================================================================
// evaluate chi2
// =============================================================================
double Gaudi::Math::ValueWithError::chi2 ( const double b ) const
{
  //
  if ( _equal ( value() , b ) ) { return  0 ; } // RETURN
  //
  if ( 0 >= cov2 ()           ) { return -1 ; } // RETURN
  const double diff = value() - b ;
  return diff*diff/cov2() ;
}
// =============================================================================
/*  get Kullback-Liebler divergency 
 *  return the divergency for valid arguments, -1 otherwise
 */
// =============================================================================
double Gaudi::Math::ValueWithError::kullback
( const Gaudi::Math::ValueWithError& b ) const
{
  //
  if ( 0 >= cov2() || b.cov2 () >= 0 ) { return -1 ; }
  //
  const double c1 =   cov2 () ;
  const double c2 = b.cov2 () ;
  //
  return ( c1 - c2 ) * ( 1.0 / c2 - 1.0 / c1 ) + chi2 ( b ) ;  
}
// =============================================================================
// evaluate residual: signed sqrt(chi2)
// =============================================================================
double Gaudi::Math::ValueWithError::residual
( const Gaudi::Math::ValueWithError& b ) const
{
  //
  if ( _equal ( value () , b.value() ) ) { return     0 ; } // RETURN
  //
  const double s_cov2 = cov2() + b.cov2() ;
  if ( 0 >= s_cov2 )                     { return -1000 ; } // RETURN
  //
  const double diff = value() - b.value() ;
  //
  return diff / std::sqrt ( s_cov2 ) ;
}
// =============================================================================
// evaluate residual: signed sqrt(chi2)
// =============================================================================
double Gaudi::Math::ValueWithError::residual
( const double b ) const
{
  //
  if ( _equal ( value() , b ) ) { return     0 ; } // RETURN
  //
  if ( 0 >= cov2 () )           { return -1000 ; } // RETURN
  //
  const double diff = value() - b ;
  //
  return diff / error () ;
}
// ============================================================================
/*  evaluate the "fraction" \f$  \frac{a}{a+b} \f$
 *  @param  b the parameter "b" for the fraction
 *  @return a/(a+b)
 */
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::frac
( const Gaudi::Math::ValueWithError& b ) const
{
  const double r  = value() / ( value() + b.value() ) ;
  //
  const double s  = value() + b.value() ;
  const double s2 = s  * s  ;
  const double s4 = s2 * s2 ;
  const double c2 =
    std::fabs (   cov2 () ) * b.value () * b.value () +
    std::fabs ( b.cov2 () ) *   value () *   value () ;
  //
  return ValueWithError
    ( r , 0 <= cov2() && 0 <= b.cov2() ? c2/s4 : -1.0 * c2 / s4 ) ;
  //
}
// ============================================================================
/*  evaluate the "fraction" \f$  \frac{a}{a+b} \f$
 *  @param  b the parameter "b" for the fraction
 *  @return a/(a+b)
 */
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::frac ( const double  b ) const
{ return frac ( ValueWithError ( b ) ) ; }
// ============================================================================
/*  evaluate the "asymmetry" \f$  \frac{a-b}{a+b} \f$
 *  @param  b the parameter "b" for the fraction
 *  @return (a-b)/(a+b)
 */
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::asym
( const Gaudi::Math::ValueWithError& b ) const
{
  //
  const double r  = ( value() - b.value() ) / ( value() + b.value() ) ;
  //
  const double s  = value() + b.value() ;
  const double s2 = s  * s  ;
  const double s4 = s2 * s2 ;
  //
  const double c2 =
    4 * std::fabs (   cov2 () ) * b.value () * b.value () +
    4 * std::fabs ( b.cov2 () ) *   value () *   value () ;
  //
  return ValueWithError
    ( r , 0 <= cov2() && 0 <= b.cov2() ? c2/s4 : -1.0 * c2 / s4 ) ;
  //
}
// ============================================================================
/*  evaluate the "asymmetry" \f$  \frac{a-b}{a+b} \f$
 *  @param  b the parameter "b" for the fraction
 *  @return (a-b)/(a+b)
 */
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::asym ( const double  b ) const
{ return asym ( ValueWithError ( b ) ) ; }
// =============================================================================
// check for NaN
// =============================================================================
bool Gaudi::Math::ValueWithError::isnan    () const
{
  return
    boost::math::isnan    ( m_value ) ||
    boost::math::isnan    ( m_cov2  )  ;
}
// =============================================================================
// check for finiteness
// =============================================================================
bool Gaudi::Math::ValueWithError::isfinite () const
{
  return
    boost::math::isfinite ( m_value ) &&
    boost::math::isfinite ( m_cov2  )  ;
}
// =============================================================================
// check for finiteness
// =============================================================================
bool Gaudi::Math::ValueWithError::isnormal () const
{
  return
    boost::math::isnormal ( m_value ) &&
    boost::math::isnormal ( m_cov2  )  ;
}
// =============================================================================
// check for finiteness
// =============================================================================
bool Gaudi::Math::ValueWithError::isinf () const
{
  return
    boost::math::isinf ( m_value ) ||
    boost::math::isinf ( m_cov2  )  ;
}
// =============================================================================
// for easy pythonization
// =============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__add__
( const Gaudi::Math::ValueWithError& right ) const
{
  //
  if ( &right == this ) { return right * 2.0 ; }
  //
  ValueWithError tmp ( *this ) ;
  return tmp += right ;
}
// =============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__sub__
( const Gaudi::Math::ValueWithError& right ) const
{
  //
  if ( &right == this ) { return  ValueWithError(0,0) ; }
  //
  ValueWithError tmp ( *this ) ;
  return tmp -= right ;
}
// =============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__mul__
( const Gaudi::Math::ValueWithError& right ) const
{
  //
  if ( &right == this ) { return  pow ( *this , 2 ) ; }
  //
  ValueWithError tmp ( *this ) ;
  return tmp *= right ;
}
// =============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__div__
( const Gaudi::Math::ValueWithError& right ) const
{
  //
  if ( &right == this ) { return  ValueWithError ( 1 , 0 ) ; }
  //
  ValueWithError tmp ( *this ) ;
  return tmp /= right ;
}
// =============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__add__ ( const double right ) const
{
  ValueWithError tmp ( *this ) ;
  return tmp += right ;
}
// =============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__sub__ ( const double right ) const
{
  ValueWithError tmp ( *this ) ;
  return tmp -= right ;
}
// =============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__mul__ ( const double right ) const
{
  ValueWithError tmp ( *this ) ;
  return tmp *= right ;
}
// =============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__div__ ( const double right ) const
{
  ValueWithError tmp ( *this ) ;
  return tmp /= right ;
}
// =============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__rsub__ ( const double right ) const
{ return ValueWithError ( right - value() , cov2()  ) ; }
// =============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__rdiv__ ( const double right ) const
{  
  ValueWithError tmp ( right ) ;
  return tmp /= (*this) ;
}
// ============================================================================
// abs(a)
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__abs__ () const
{ return Gaudi::Math::abs ( *this ) ; }
// ============================================================================
// me**e
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__pow__  ( const int             e ) const
{ return pow ( *this , e ) ; }
// ============================================================================
// me**e
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__pow__  ( const double          e ) const
{ return pow ( *this , e ) ; }
// ============================================================================
// me**e
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__pow__
( const Gaudi::Math::ValueWithError&  e ) const
{ return pow ( *this , e ) ; }
// ============================================================================
// e**me
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__rpow__  ( const int             e ) const
{ return pow ( e , *this ) ; }
// ============================================================================
// e**me
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__rpow__ ( const double          e ) const
{ return pow ( e , *this ) ; }
// ============================================================================
// -me
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__neg__() const
{ return Gaudi::Math::ValueWithError ( -value() , cov2() ) ; }
// ============================================================================
// +me (no-effect)
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__pos__() const { return *this ; }
// ============================================================================
// exp(me)
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__exp__   () const { return exp   ( *this ) ; }
// ============================================================================
// log(me)
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__log__   () const { return log   ( *this ) ; }
// ============================================================================
// log10(me)
// ============================================================================
Gaudi::Math::ValueWithError
Gaudi::Math::ValueWithError::__log10__ () const { return log10 ( *this ) ; }
// ============================================================================


// ============================================================================
/*  evaluate abs(a)
 *  @param a (INPUT) the value
 *  @return the absolute value
 */
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::abs
( const Gaudi::Math::ValueWithError& a )
{ return ValueWithError ( std::fabs ( a.value() ) , a.cov2() ) ; }
// ============================================================================
/* evaluate the binomial efficiency for Bernulli scheme with
 *  @param n (INPUT) number of 'success'
 *  @param N (INPUT) total number
 *  @return the binomial efficiency
 */
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::binomEff
( const size_t n ,
  const size_t N )
{
  if       ( n >  N ) { return binomEff       ( N , n ) ; }
  else if  ( 0 == N ) { return ValueWithError ( 1 , 1 ) ; }
  //
  const long n1 = 0 == n ? 1 :     n ;
  const long n2 = n == N ? 1 : N - n ;
  //
  const double eff = double ( n       ) / N         ;
  const double c2  = double ( n1 * n2 ) / N / N / N ;
  //
  return Gaudi::Math::ValueWithError  ( eff , c2 ) ;
}
// ============================================================================
/*  evaluate the binomial efficiency interval using Wilson's prescription
 *  @param n (INPUT) number of 'success'
 *  @param N (INPUT) total number
 *  @return the binomial efficiency
 */
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::wilsonEff
( const size_t n ,
  const size_t N )
{
  //
  if      ( n >  N ) { return wilsonEff      ( N , n ) ; }
  else if ( 0 == N ) { return ValueWithError ( 1 , 1 ) ; }
  //
  const long n1       = 0 == n ? 1 :     n ;
  const long n2       = n == N ? 1 : N - n ;
  //
  const double p      = double ( n1 ) / N ;
  const double q      = double ( n2 ) / N ;
  //
  const double kappa  =             1 ; // "1*sigma"
  const double kappa2 = kappa * kappa ;
  //
  const double nK     = N + kappa2 ;
  const double eff    = ( n + 0.5 * kappa2 ) / nK ;
  //
  const double prefix = kappa2 * N / ( nK * nK ) ;
  const double c2     = prefix * ( q * p + 0.25 * kappa2 / N ) ;
  //
  return Gaudi::Math::ValueWithError  ( eff , c2 ) ;
}
// ============================================================================
/*  evaluate the binomial efficiency interval using Agresti-Coull's prescription
 *  @param n (INPUT) number of 'success'
 *  @param N (INPUT) total number
 *  @return the binomial efficiency
 */
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::agrestiCoullEff
( const size_t n ,
  const size_t N )
{
  //
  if      ( n >  N ) { return wilsonEff      ( N , n ) ; }
  else if ( 0 == N ) { return ValueWithError ( 1 , 1 ) ; }
  //
  const double kappa  =             1 ; // "1*sigma"
  const double kappa2 = kappa * kappa ;
  //
  const double n1 = n + 0.5 * kappa2 ;
  const double n2 = N +       kappa2 ;
  //
  const double p  = n1/n2 ;
  const double q  = 1 - p ;
  //
  const double eff = p ;
  const double c2  = kappa2 * p * q / n2 ;
  //
  return Gaudi::Math::ValueWithError  ( eff , c2 ) ;
}
// ============================================================================
/*  evaluate pow(a,b)
 *  @param a (INPUT) the base
 *  @param b (INPUT) the exponent
 *  @return the <c>a</c> rased to power <c>b</b>
 */
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::pow
( const Gaudi::Math::ValueWithError& a ,
  const int                          b )
{
  //
  if      ( 0 == b         ) { return 1 ; }          // RETURN
  else if ( 1 == b         ) { return a ; }          // RETURN
  //
  else if ( 0 >= a.cov2 () || _zero ( a.cov2() ) )
  { return _pow ( a.value() , b ) ;  }               // RETURN
  //
  const double v  =     _pow ( a.value () , b     ) ;
  const double e1 = b * _pow ( a.value () , b - 1 ) ;
  //
  return Gaudi::Math::ValueWithError ( v , e1 * e1 * a.cov2 () ) ;
  //
}
// ============================================================================
/*  evaluate pow(a,b)
 *  @param a (INPUT) the base
 *  @param b (INPUT) the exponent
 *  @return the <c>a</c> raised to power <c>b</b>
 */
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::pow
( const Gaudi::Math::ValueWithError& a ,
  const double                       b )
{
  //
  if      ( _zero ( b )    ) { return 1 ; }         // RETURN
  else if ( _one  ( b )    ) { return a ; }         // RETURN
  else if ( 0 >= a.cov2 () || _zero ( a.cov2() ) )
  { return std::pow ( a.value() , b ) ; }           // RETURN
  //
  const double v  =     std::pow ( a.value () , b     ) ;
  const double e1 = b * std::pow ( a.value () , b - 1 ) ;
  //
  return Gaudi::Math::ValueWithError ( v , e1 * e1 * a.cov2 () ) ;
}
// ============================================================================
/*  evaluate pow(a,b)
 *  @param a (INPUT) the base
 *  @param b (INPUT) the exponent
 *  @return the <c>a</c> raised to power <c>b</b>
 */
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::pow
( const int                          a ,
  const Gaudi::Math::ValueWithError& b )
{
  if      ( 0 == a && 0 < b.value()       ) { return 0 ; }    // RETURN
  else if ( 1 == a && _zero ( b.cov2 () ) ) { return 1 ; }    // RETURN
  else if ( 0 >= b.cov2() || _zero ( b.cov2() ) )
  { return std::pow ( double ( a ) , b.value() ) ; }    // RETURN
  //
  const double v  =     std::pow ( double ( a ) , b.value() ) ;
  const double e2 = v * std::log ( double ( a ) ) ;
  //
  return Gaudi::Math::ValueWithError ( v , e2 * e2 * b.cov2 () ) ;
}
// ============================================================================
/*  evaluate pow(a,b)
 *  @param a (INPUT) the base
 *  @param b (INPUT) the exponent
 *  @return the <c>a</c> raised to power <c>b</b>
 */
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::pow
( const double                       a ,
  const Gaudi::Math::ValueWithError& b )
{
  if      ( _zero ( a ) && 0 < b.value() ) { return 0 ; }    // RETURN
  else if ( 0 >= b.cov2() || _zero ( b.cov2() ) )
  { return std::pow ( a , b.value() ) ; }    // RETURN
  //
  const double v  =     std::pow ( a , b.value() ) ;
  const double e2 = v * std::log ( a ) ;
  //
  return Gaudi::Math::ValueWithError ( v , e2 * e2 * b.cov2 () ) ;
  //
}
// ============================================================================
/*  evaluate pow(a,b)
 *  @param a (INPUT) the base
 *  @param b (INPUT) the exponent
 *  @return the <c>a</c> raised to power <c>b</b>
 */
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::pow
( const Gaudi::Math::ValueWithError& a ,
  const Gaudi::Math::ValueWithError& b )
{
  //
  if ( &a == &b ) 
  {
    if      ( 0 >= a.cov2 () || _zero ( a.cov2() ) )
    { return std::pow ( a.value() , a.value() ) ; }
    //
    const double v2 = std::pow ( a.value() , a.value() ) ;
    const double v3 = std::log ( a.value() ) + 1 ;
    //
    return Gaudi::Math::ValueWithError
      ( v2 , v2 * v2 * v3 * v3 * a.cov2 () ) ;
  }
  //
  if      ( 0 >= a.cov2 () || _zero ( a.cov2() ) )
  { return pow ( a.value() , b         ) ; }
  else if ( 0 >= b.cov2 () || _zero ( b.cov2() ) )
  { return pow ( a         , b.value() ) ; }
  //
  const double v  = std::pow ( a.value () , b.value ()     ) ;
  const double v1 = std::pow ( a.value () , b.value () - 1 ) ;
  //
  const double e1 = v1 *            b.value ()   ;
  const double e2 = v  * std::log ( a.value () ) ;
  //
  return Gaudi::Math::ValueWithError
    ( v , e1 * e1 * a.cov2 () + e2 * e2 * b.cov2 () ) ;
}
// ============================================================================
/*  evaluate exp(b)
 *  @param b (INPUT) the exponent
 *  @return the <c>e</c> raised to power <c>b</b>
 *  @warning invalid and small covariances are ignored
 */
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::exp
( const Gaudi::Math::ValueWithError& b )
{
  if ( 0 >= b.cov2 () || _zero ( b.cov2() ) )
  { return std::exp ( b.value() ) ; }
  //
  const double v = std::exp ( b.value() ) ;
  return Gaudi::Math::ValueWithError ( v , v * v * b.cov2 () ) ;
}
// ============================================================================
/*  evaluate log(b)
 *  @param b (INPUT) the parameter
 *  @return logarithm
 *  @warning invalid and small covariances are ignored
 */
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::log
( const Gaudi::Math::ValueWithError& b )
{
  if ( 0 >= b.cov2 () || _zero ( b.cov2() ) ) { return std::log ( b.value() ) ; }
  //
  const double v  = std::log ( b.value() ) ;
  const double e1 = 1.0 / b.value()        ;
  return Gaudi::Math::ValueWithError ( v , e1 * e1 * b.cov2 () ) ;
}
// ============================================================================
/*  evaluate log(b)
 *  @param b (INPUT) the parameter
 *  @return logarithm
 *  @warning invalid and small covariances are ignored
 */
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::log10
( const Gaudi::Math::ValueWithError& b )
{
  if ( 0 >= b.cov2 () || _zero ( b.cov2() ) )
  { return std::log10 ( b.value() ) ; }
  //
  const double v  = std::log10 ( b.value() ) ;
  ///
  static const double a  = 1.0 / std::log ( 10.0 ) ;
  //
  const double e1 = a / b.value() ;
  //
  return Gaudi::Math::ValueWithError ( v , e1 * e1 * b.cov2 () ) ;
}
// ============================================================================
/*  simple linear interpolation 
 *  @param x  the value to evaluate the function 
 *  @param x0 the abscissa for the first  point
 *  @param y0 the function value for the first  point
 *  @param x1 the abscissa for the second point
 *  @param y1 the function value for the second point
 *  @return linear interpolation at point x
 */
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::interpolate_1D
( const double                       x  , 
  const double                       x0 ,
  const Gaudi::Math::ValueWithError& y0 , 
  const double                       x1 ,
  const Gaudi::Math::ValueWithError& y1 ) 
{
  //
  const double c0 = ( x - x1 ) / ( x0 - x1 ) ;
  const double c1 = ( x - x0 ) / ( x1 - x0 ) ;
  //
  return c0 * y0 + c1 * y1 ;
}
// ============================================================================
/*  simple (bi)linear interpolation 
 *  @param x  the x-coordiate to evaluate the function 
 *  @param y  the y-coordiate to evaluate the function 
 *  @param x0 the x-coordinate for the first  pair of points
 *  @param x1 the x-coordinate for the second pair of points
 *  @param y0 the y-coordinate for the first  pair of points
 *  @param y1 the y-coordinate for the second pair of points
 *  @param v00 the function value 
 *  @param v01 the function value 
 *  @param v10 the function value 
 *  @param v11 the function value 
 *  @return bilinear interpolation at point (x,y)
 */
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::interpolate_2D 
( const double                       x   , 
  const double                       y   , 
  const double                       x0  ,
  const double                       x1  ,
  const double                       y0  ,
  const double                       y1  ,
  const Gaudi::Math::ValueWithError& v00 , 
  const Gaudi::Math::ValueWithError& v01 , 
  const Gaudi::Math::ValueWithError& v10 , 
  const Gaudi::Math::ValueWithError& v11 ) 
{
  //
  const double c00 =  ( x - x1 ) * ( y - y1 ) / ( x0 - x1 ) / ( y0 - y1 ) ;
  const double c01 =  ( x - x1 ) * ( y - y0 ) / ( x0 - x1 ) / ( y1 - y0 ) ;
  const double c10 =  ( x - x0 ) * ( y - y1 ) / ( x1 - x0 ) / ( y0 - y1 ) ;
  const double c11 =  ( x - x0 ) * ( y - y0 ) / ( x1 - x0 ) / ( y1 - y0 ) ;
  //
  return c00 * v00 + c01 * v01 + c10 * v10 + c11 * v11  ;
} 
// ============================================================================
/*  get the sum of the vector 
 *  @param vct the vector
 *  @param ini the intial value 
 *  @return the sum over the vector 
 */
// ============================================================================
Gaudi::Math::ValueWithError 
Gaudi::Math::sum 
( const std::vector<Gaudi::Math::ValueWithError>& vct , 
  Gaudi::Math::ValueWithError                     ini ) 
{
  //
  for ( std::vector<Gaudi::Math::ValueWithError>::const_iterator iv = 
          vct.begin() ; vct.end() != iv ; ++iv ) { ini += (*iv) ; }
  //
  return ini ;
  //
}
// ============================================================================
/*  get the sum of absolute values for the vector 
 *  @param vct the vector
 *  @return the sum over the vector 
 */
// ============================================================================
Gaudi::Math::ValueWithError 
Gaudi::Math::abssum 
( const std::vector<Gaudi::Math::ValueWithError>& vct )
{
  //
  ValueWithError val ;
  for ( std::vector<Gaudi::Math::ValueWithError>::const_iterator iv = 
          vct.begin() ; vct.end() != iv ; ++iv ) 
  { val += abs (*iv) ; }
  //
  return val ;
  //
}
// ============================================================================
 
// ============================================================================
// Utiilties 
// ============================================================================
// print the vector
// ============================================================================
std::ostream& Gaudi::Utils::toStream
( const std::vector<Gaudi::Math::ValueWithError>& o , std::ostream& s ) 
{ 
  //
  Gaudi::Utils::toStream 
    ( o.begin () , o.end () , s  , "[ " , " ]" , " , " ) ;
  //
  return s ;
}
// ============================================================================
// the output operator for the vector 
// ============================================================================
std::ostream& 
Gaudi::Math::operator<<( std::ostream&                                   s , 
                         const std::vector<Gaudi::Math::ValueWithError>& v ) 
{ 
  return Gaudi::Utils::toStream ( v , s ) ; 
}
// ============================================================================

// ============================================================================
// Boost.Bind
// ============================================================================
#include "boost/bind.hpp"
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ParsersFactory.h"
// ============================================================================
namespace Gaudi
{
  // ==========================================================================
  namespace Parsers
  {
    // ========================================================================
    /** @class ValueWithErrorGrammar
     *
     *  The valid represenation of value with error are:
     *     "( 34 +- 10 )" , "( 34 , 10 )" , "34"
     *
     *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
     *  @author Alexander Mazurov  alexander.mazurov@gmail.com
     *  @date 2011-07-27
     */
    template< typename Iterator, typename Skipper>
    class ValueWithErrorGrammar:
       public qi::grammar<Iterator,Gaudi::Math::ValueWithError(), Skipper> {
     // =======================================================================
     public: // Result type
       typedef Gaudi::Math::ValueWithError ResultT ;
     // =======================================================================
     public: // Grammar is defined at the constructor:
      ValueWithErrorGrammar() :ValueWithErrorGrammar::base_type(value) {
          value = ( qi::lit("(") >> grvalue[op(qi::_val,qi::_1)]
             >> -((qi::lit("+-") | qi::lit(',') | qi::lit(';'))
                   >> grvalue[op(qi::_val,qi::_1, tag_error())])
             >> ')'
             ) | grvalue[op(qi::_val,qi::_1)] ;
        }
     // =======================================================================
     private:  // Rules and grammars:
      // Start rule should have the same template parameters as
      // qi::grammar at the class definition:
      qi::rule<Iterator, Gaudi::Math::ValueWithError(), Skipper> value;
      RealGrammar<Iterator, double, Skipper> grvalue;
     // =======================================================================
     private: // Semantic actions:
      struct tag_error{};
      struct Operations {
        // Some magic:
        template <typename A, typename B = boost::fusion::unused_type,
            typename C = boost::fusion::unused_type,
            typename D = boost::fusion::unused_type>
        struct result { typedef void type; };
        // Actions:
        // --------------------------------------------------------------------
        void operator()(Gaudi::Math::ValueWithError& val, double v) const {
          val.setValue(v);
        }
        void operator()(Gaudi::Math::ValueWithError& val, double err,
            tag_error) const {
          val.setError(err);
        }
        // --------------------------------------------------------------------
      };  // End of Operations
      // ----------------------------------------------------------------------
      ph::function<Operations> op;
    };
    // ========================================================================
    REGISTER_GRAMMAR(Gaudi::Math::ValueWithError, ValueWithErrorGrammar);
    // ========================================================================
  } //                                          end of namespace Gaudi::Parsers
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================


// =============================================================================
/*  parse the input string into the result
 *  @param result (output) the result
 *  @param input  (input) input string
 */
// =============================================================================
StatusCode Gaudi::Parsers::parse(Gaudi::Math::ValueWithError& result,
    const std::string& input) {
  return parse_(result, input);
}
// =============================================================================
/* parse the input string into the result
 *  @param result (output) the result
 *  @param input  (input) input string
 */
// =============================================================================
StatusCode Gaudi::Parsers::parse(
    std::vector<Gaudi::Math::ValueWithError>& result,
    const std::string& input) {
  return parse_(result, input);
}
// =============================================================================


// =============================================================================
// The END
// =============================================================================


