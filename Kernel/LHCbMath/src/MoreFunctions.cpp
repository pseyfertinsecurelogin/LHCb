// $Id:$ 
// ============================================================================
// Include files 
// ============================================================================
// STD&STL
// ============================================================================
#include <cmath>
#include <climits>
#include <cassert>
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/MoreFunctions.h"
#include "LHCbMath/LHCbMath.h"
#include "LHCbMath/Power.h"
// ============================================================================
// GSL 
// ============================================================================
#include "gsl/gsl_sf_hyperg.h"
// ============================================================================
// Local
// ============================================================================
#include "GSL_sentry.h"
// ============================================================================
/** @file
 *  implementation file for function from file LHCbMath/MoreFunctions.h
 *  @see LHCbMath/MoreFunctions.h
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-03-26
 *                    $Revision:$
 *  Last modification $Date:$
 *                 by $Author:$
 */  
// ============================================================================
namespace 
{
  // ==========================================================================
  static_assert ( std::numeric_limits<float>::is_specialized            , 
                  "mumeric_limits are not specialized for flota  "      ) ;
  static_assert ( std::numeric_limits<double>::is_specialized           , 
                  "mumeric_limits are not specialized for doubles"      ) ;
  static_assert ( std::numeric_limits<long double>::is_specialized      , 
                  "mumeric_limits are not specialized for long doubles" ) ;
  // ==========================================================================
  /// equality criteria for doubles
  LHCb::Math::Equal_To<double> s_equal ;       // equality criteria for doubles
  /// zero for doubles  
  LHCb::Math::Zero<double>     s_zero  ;       // zero for doubles
  /// "almost infinity" 
  const long double s_infinity = 0.9 * std::numeric_limits<double>::max () ;
  /// "relatively large value" 
  const int         s_imax     = -32   ;
  const long double s_large    = std::pow ( 2.0 , -s_imax ) ;
  // epsilon
  const long double s_epsilon  =  
    std::min ( (long double) std::numeric_limits<double>     ::epsilon () , 
               10000       * std::numeric_limits<long double>::epsilon () ) ;
  // ==========================================================================
  /// get a factorial 
  inline unsigned long long 
  _factorial_i_ ( const unsigned short N ) 
  {
    return 
      0 == N ?  1 : 
      0 == N ?  1 : 
      2 == N ?  2 : 
      3 == N ?  6 : 
      4 == N ? 24 : N * _factorial_i_ ( N - 1 ) ;
  }
  // ==========================================================================
  /// get a factorial 
  inline long double 
  _factorial_d_ ( const unsigned short N ) 
  {
    return 
      0 == N ?  1 : 
      0 == N ?  1 : 
      2 == N ?  2 : 
      3 == N ?  6 : 
      4 == N ? 24 : N * _factorial_d_ ( N - 1 ) ;
  }
  // ==========================================================================
}
// ============================================================================
namespace
{
  // ==========================================================================
  /** sum of N-terms in the exponential expansion 
   *  \f$ f = \sum_{i=0}^{N} \frac{x^k}{k!}\f$
   *  @param x  INPUT the argument 
   *  @param N  INPUT N-terms to be used 
   *  @return partial exponential sum 
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-03-26
   */
  inline long double _exp_N_ 
  ( const long double    x , 
    const unsigned short N ) 
  {
    long double r = 1 ;
    long double t = 1 ;
    for ( unsigned short n = 1 ; n <= N ; ++n ) 
    {
      t *= x ;
      t /= n ;
      r += t ;
      if ( r > s_infinity ) { return s_infinity ; }  // RETURN 
    }
    return r ;
  }
  // ==========================================================================
}
// ============================================================================
/*  sum of N-terms in the exponential expansion 
 *  \f$ f = \sum_{i=0}^{N} \frac{x^k}{k!}\f$
 *  @param x  INPUT the argument 
 *  @param N  INPUT N-terms to be used 
 *  @return partial expoenntial sum 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-03-26
 */
// ============================================================================
double Gaudi::Math::exp_N ( const double x , const unsigned short N ) 
{
  return 
    0 == N       ? 1 :
    1 == N       ? 1 + x : 
    2 == N       ? 1 + x * ( 1 + x *   0.5 ) : 
    3 == N       ? 1 + x * ( 1 + x * ( 0.5 + x / 6.0 ) ) : 
    s_zero ( x ) ? 1. : _exp_N_ ( x , N ) ;
}
// ============================================================================
namespace 
{
  // ==========================================================================
  /** calculate "relative or reduced exponent"
   *  \f$f(x) = N! ( e^{x} - \sum_{k=0}^{N-1} \frac{x^k}{k!})/x^{N} \f$ 
   *  @param x  INPUT the argument 
   *  @param N  INPUT N-terms to be used 
   *  @return the value of "reduced exponent"
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-03-26
   */
  inline long double _exp_rel_N_  
  ( const long double  x , 
    const unsigned int N ) 
  {
    //
    // 1. calculate regular part 
    //
    long double hm1 = 1  ; 
    long double km1 = 0  ;
    long double h0  = 0  ;
    long double k0  = 1  ;
    long double hp1 = h0 ;
    long double kp1 = k0 ;
    for ( unsigned long n = 2 ; n <= 100000 ; ++n ) 
    {
      const long double   an = ( 0 == n % 2 ?  x * n / 2 :  -x*( N + ( n - 1 ) / 2 ) ) ;
      const unsigned long bn = n + N ;
      //
      hp1 = bn * h0 + an * hm1 ;
      kp1 = bn * k0 + an * km1 ;  
      //
      hm1 = h0  ;
      km1 = k0  ;
      h0  = hp1 ;
      k0  = kp1 ;
      //
      if ( std::abs ( hp1 ) > s_large || std::abs ( kp1 ) > s_large ) 
      {
        //
        h0  = std::ldexp ( h0  , s_imax ) ;
        k0  = std::ldexp ( k0  , s_imax ) ;
        hm1 = std::ldexp ( hm1 , s_imax ) ;
        km1 = std::ldexp ( km1 , s_imax ) ;
      }
      // time-to-time check the convergency 
      if ( 0 == n % 5 ) 
      {
        const long double delta  =  ( hm1 / km1 ) / ( h0 / k0 ) - 1 ;
        if ( std::abs ( delta ) <= 2 * s_epsilon ) { break ; }
      }
    }
    const long double result = h0/k0 ;
    //
    // add the "irregular part"
    //
    return 1 / ( 1 - x / ( N + 1 + result )  ) ;
  }
  // ==========================================================================
}
// ============================================================================
/* "relative or reduced exponent"
 *  \f$f(x) = N! ( e^{x} - \sum_{k=0}^{N-1} \frac{x^k}{k!})/x^{N} \f$ 
 *  @param x  INPUT the argument 
 *  @param N  INPUT N-terms to be used 
 *  @return the value of "reduced exponent"
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-03-26
 */
// ============================================================================
double Gaudi::Math::exp_rel_N ( const double x , const unsigned short N ) 
{
  const long double y = x ;
  return 
    0 == N ?  std::exp ( y ) :  
    s_zero ( x ) ? 1 : 
    _exp_rel_N_ ( y , N ) ;  
}
// ============================================================================
namespace
{
  // ==========================================================================
  /** regularized incomplete gamma function 
   *  \f$ \gamma^{\ast}(a,x) = \frac{x^{-a}}{\Gamma(a)\gamma(a,x) }\f$, 
   *  where 
   *  \f$ \gamma(a,x) = \Gamma(a) - \Gamma(a,x) = \int_0^x e^{-t}t^{a-1}dt\f$, 
   *  @param a INPUT a-parameter 
   *  @param x INPUT x-argument 
   *  @return the value of regularized incomplete gamma function 
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-03-27
   */
  inline long double _gamma_star_ 
  ( const long double a , 
    const long double x )  
  {
    //
    long double t  = 1     ; 
    long double r  = t / a ;
    //
    for ( unsigned long n = 1 ; n <1000000; ++n ) 
    {
      //
      t *= -x ; 
      t /=  n ;
      //
      if ( 0 == a + n ) { break ; }   // BREAK  :-( 
      r += t / ( a + n ) ;
      //
      if      ( std::abs ( t ) <= 2 * s_epsilon ) { break ; } // BREAK 
    }
    //
    long double Ga = std::tgamma ( a ) ;
    r /= Ga ;
    //
    return 
      r < -s_infinity ? -s_infinity :
      r >  s_infinity ?  s_infinity : r ;
  }
  // ==========================================================================
  /** regularized incomplete gamma function 
   *  \f$ \gamma^{\ast}(a,x) = \frac{x^{-a}}{\Gamma(a)\gamma(a,x) }\f$, 
   *  where 
   *  \f$ \gamma(a,x) = \Gamma(a) - \Gamma(a,x) = \int_0^x e^{-t}t^{a-1}dt\f$, 
   *  @param a INPUT a-parameter 
   *  @param x INPUT x-argument 
   *  @return the value of regularized incomplete gamma function 
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-03-27
   */
  inline long double _gamma_star_2_ 
  ( const long double a , 
    const long double x )  
  {
    //
    long double t = 1 / a ; 
    long double r = t     ;
    //
    for ( unsigned long n = 1 ; n <1000000; ++n ) 
    {
      //
      if ( 0 == a + n ) { break ; }   // BREAK :-( 
      t *=   x       ;
      t /= ( a + n ) ;
      //
      r += t ;
      if ( std::abs ( t ) <= 2 * s_epsilon ) { break ; } // BREAK 
    }
    //
    return r * std::exp ( -x ) / std::tgamma ( a ) ;
    //
  }
  // ==========================================================================
}
// ============================================================================
/** regularized incomplete gamma function 
 *  \f$ \gamma^{\ast}(a,x) = \frac{x^{-a}}{\Gamma(a)\gamma(a,x) }\f$, 
 *  where 
 *  \f$ \gamma(a,x) = \Gamma(a) - \Gamma(a,x) = \int_0^x e^{-t}t^{a-1}dt\f$, 
 *  @param a INPUT a-parameter 
 *  @param x INPUT x-argument 
 *  @return the value of regularized incomplete gamma function 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-03-27
 */
// ============================================================================
double Gaudi::Math::gamma_star ( const double a , const double x ) 
{
  //
  if ( LHCb::Math::isint ( a ) || std::abs ( a - LHCb::Math::round ( a ) ) < 1.e-4 ) 
  {
    const int         n = LHCb::Math::round ( a ) ;
    const long double y = x ;
    if ( n <= 0 ) { return 1./ Gaudi::Math::pow ( y , std::abs ( n ) ) ; }
  }
  //
  // const double r3 = 
  //   0 < x && 0 < a ? gsl_sf_gamma_inc_P ( a , x ) * std::pow ( x , -a ) : -1 ;
  //
  return
    1.1 < x ? _gamma_star_2_ ( a , x ) :  _gamma_star_   ( a , x ) ;    
}
// ============================================================================
/** regularized incomplete gamma function 
 *  \f$ \gamma^{\ast}(n,x) = \frac{x^{-n}}{\Gamma(n)\gamma(n,x) }\f$, 
 *  where 
 *  \f$ \gamma(n,x) = \Gamma(n) - \Gamma(n,x) = \int_0^x e^{-t}t^{n-1}dt\f$, 
 *  @param n INPUT n-parameter 
 *  @param x INPUT x-argument 
 *  @return the value of regularized incomplete gamma function 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-03-27
 */
// ============================================================================
double Gaudi::Math::gamma_star ( const int n , const double x ) 
{
  const long double y = x ;
  return 
    n   <= 0 ? 1./Gaudi::Math::pow ( y , std::abs ( n ) ) : 
    1.1 <  x ? _gamma_star_2_ ( n ,  y ) : _gamma_star_  ( n , y ) ;  
}
// ============================================================================
/*  alpha_n 
 *  \f$\alpha_n(x) = \int_1^\inf t^n e^{-tz}dt \f$
 *  @param n INPUT n-parameter 
 *  @param x INPUT x-argument 
 *  @return the function value 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-03-27
 */
// ============================================================================
double Gaudi::Math::alpha_N 
( const unsigned short n , const double x ) 
{
  const long double z = x ;
  //
  long double result  = _factorial_d_ ( n ) ;
  result /= Gaudi::Math::pow ( z , n + 1 ) ;
  //
  return result * std::exp ( -z ) * _exp_N_ ( z , n ) ;
}
// ============================================================================
/* alpha'_n 
 *  \f$\alpha^{\prime}_n(x) = \int_0^1 t^n e^{-tx}dt \f$
 *  @param n INPUT n-parameter 
 *  @param x INPUT x-argument 
 *  @return the function value 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-03-27
 */
// ============================================================================
double Gaudi::Math::alpha_prime_N ( const unsigned short n , const double x ) 
{
  const long double z   = x     ;
  const long double np1 = n + 1 ;
  //
  return
    s_zero   (  x ) ?                         1 / np1 :
    std::exp ( -z ) * _exp_rel_N_ ( z , n + 1 ) / np1 ;
}
// ============================================================================
/*  beta_n 
 *  \f$\beta_n(x) = \int_{-1}^{+1} t^n e^{-tx}dt \f$
 *  @param n INPUT n-parameter 
 *  @param x INPUT x-argument 
 *  @return the function value 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-03-27
 */
// ============================================================================
double Gaudi::Math::beta_N ( const unsigned short n , const double x ) 
{
  return 
    0 == n % 2 ? 
    alpha_prime_N ( n , x ) + alpha_prime_N ( n , -x ) : 
    alpha_prime_N ( n , x ) - alpha_prime_N ( n , -x ) ;
}
// ============================================================================
namespace 
{
  // ==========================================================================
  /* evaluate "simple" continued fraction:
   *  \f$f(x) = a_0 + \frac{1}{ a_1 + \frac{1}{ a_2 + ...} } \f$
   */
  template <class ITERATOR>
  inline long double 
  _simple_cf_ 
  ( ITERATOR begin , 
    ITERATOR end   ) 
  {
    if ( begin == end ) { return 0 ; }
    //
    long double hm1 = 0  ; 
    long double km1 = 1  ;
    long double h0  = 1  ;
    long double k0  = 0  ;
    long double hp1 = h0 ;
    long double kp1 = k0 ;
    for ( ; begin != end ; ++begin ) 
    {
      const double a = *begin ;
      hp1 = a * h0 + hm1 ;
      kp1 = a * k0 + km1 ;  
      //
      hm1 = h0  ;
      km1 = k0  ;
      h0  = hp1 ;
      k0  = kp1 ;
      //
      if ( std::abs ( hp1 ) > s_large || std::abs ( kp1 ) > s_large ) 
      {
        h0  = std::ldexp ( h0  , s_imax ) ;
        k0  = std::ldexp ( k0  , s_imax ) ;
        hm1 = std::ldexp ( hm1 , s_imax ) ;
        km1 = std::ldexp ( km1 , s_imax ) ;
      }
    }
    //
    return h0 / k0 ;
  }
  // ==========================================================================
  /* evaluate "simple" continued fraction:
   *  \f$f(x) = \frac{b_0}{ 1 + \frac{b_1}{ 1 +  ...} } \f$
   */
  template <class ITERATOR>
  inline long double 
  _simple_cf_b_ 
  ( ITERATOR begin , 
    ITERATOR end   ) 
  {
    if ( begin == end ) { return 0 ; }
    //
    long double hm1 = 1  ; 
    long double km1 = 0  ;
    long double h0  = 0  ;
    long double k0  = 1  ;
    long double hp1 = h0 ;
    long double kp1 = k0 ;
    for ( ; begin != end ; ++begin ) 
    {
      const double b = *begin ;
      hp1 = h0 + b * hm1 ;
      kp1 = k0 + b * km1 ;  
      //
      hm1 = h0  ;
      km1 = k0  ;
      h0  = hp1 ;
      k0  = kp1 ;
      //
      if ( std::abs ( hp1 ) > s_large || std::abs ( kp1 ) > s_large ) 
      {
        h0  = std::ldexp ( h0  , s_imax ) ;
        k0  = std::ldexp ( k0  , s_imax ) ;
        hm1 = std::ldexp ( hm1 , s_imax ) ;
        km1 = std::ldexp ( km1 , s_imax ) ;
      }
    }
    //
    return h0 / k0 ;
  }
  // ==========================================================================
  /* evaluate "simple" continued fraction:
   *  \f$f(x) = \frac{a_1}{ b_1 + \frac{a_2}{ b_2 +  ...} } \f$
   */
  template <class ITERATOR1, class ITERATOR2>
  inline long double 
  _simple_cf_ 
  ( ITERATOR1 ab , 
    ITERATOR1 ae , 
    ITERATOR2 bb , 
    ITERATOR2 be ) 
  {
    if ( ab == ae || bb == be ) { return 0 ; }
    //
    long double hm1 = 1  ; 
    long double km1 = 0  ;
    long double h0  = 0  ;
    long double k0  = 1  ;
    long double hp1 = h0 ;
    long double kp1 = k0 ;
    for ( ; ab != ae && bb != be ;  ++ab, ++bb  ) 
    {
      //
      const double a = *ab ;
      const double b = *bb ;
      //
      hp1 = b * h0 + a * hm1 ;
      kp1 = b * k0 + a * km1 ;  
      //
      hm1 = h0  ;
      km1 = k0  ;
      h0  = hp1 ;
      k0  = kp1 ;
      //
      if ( std::abs ( hp1 ) > s_large || std::abs ( kp1 ) > s_large ) 
      {
        h0  = std::ldexp ( h0  , s_imax ) ;
        k0  = std::ldexp ( k0  , s_imax ) ;
        hm1 = std::ldexp ( hm1 , s_imax ) ;
        km1 = std::ldexp ( km1 , s_imax ) ;
      }
    }
    //
    return h0 / k0 ;
  }
  // ==========================================================================
}
// ============================================================================
/* evaluate "simple" continued fraction 
 *  \f$f(x) = a_0 + \frac{1}{ a_1 + \frac{1}{ a_2 + ...} } \f$
 *  @param a  INPUT  coefficients  
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-02-10
 */
// ============================================================================
double Gaudi::Math::continued_fraction_simple 
( const std::vector<double>& a ) 
{ return a.empty() ? 0 : _simple_cf_ ( a.begin() , a.end() ) ; }
// ============================================================================
/* evaluate "simple" continued fraction 
 *  \f$f(x) = \frac{b_0}{ 1 + \frac{b_1}{ 1 + ...}} \f$
 *  @param b  INPUT  coefficients  
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-02-10
 */
// ============================================================================
double Gaudi::Math::continued_fraction_b 
( const std::vector<double>& b )
{ 
  return 
    b.empty()            ? 0 :
    s_zero ( b.front() ) ? 0 : _simple_cf_b_ ( b.begin() , b.end() ) ; 
}
// ============================================================================
/* evaluate the continued fraction 
 *  \f$f(x) =  [b_0+] \frac{a_1}{ b_1 + \frac{a_2}{ b_2 + ...}} \f$
 *  @param a  INPUT  coefficients (len = N) 
 *  @param b  INPUT  coefficients (len = N or N+1)
 *  @attention shortest sequence is extended ith zeroes 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-02-10
 */
// ============================================================================
double Gaudi::Math::continued_fraction
( const std::vector<double>& a , 
  const std::vector<double>& b ) 
{
  //
  if      ( a.size()     == b.size() ) 
  { return             _simple_cf_ ( a.begin() , a.end() , b.begin()     , b.end() ) ; }
  else if ( a.size() + 1 == b.size() ) 
  { return b.front() + _simple_cf_ ( a.begin() , a.end() , b.begin() + 1 , b.end() ) ; }
  //
  return std::numeric_limits<double>::quiet_NaN();
}
// ============================================================================
/*  confluent hypergeometrical function  1F1 aka Kummer's function
 *  \f$ f(a,b,x) = \sum_i  \frac{(a,i)}{((b,i)}\frac{x^i}{i!}\$f 
 *  @param a INPUT a-parameter 
 *  @param b INPUT b-argument  (b>0)
 *  @param x argument
 *  @retutrn value of Kummer function
 */
// ============================================================================
double Gaudi::Math::kummer 
( const unsigned short a ,
  const unsigned short b , 
  const double         x ) 
{
  //
  // simple cases 
  //
  if      ( 0 == a || s_zero ( x ) ) { return 1 ; }
  else if ( a == b ) 
  {
    const long double z = x ;
    return std::abs ( x ) < 0.3 ? std::expm1 ( z ) + 1.0L : std::exp ( z ) ;
  }
  else if ( 1 == a && a < b        ) { return exp_rel_N  ( x , b - 1 ) ; }
  else if ( a + 1 == b ) 
  {
    long double gs = gamma_star ( a , -x );
    return gs * _factorial_d_ ( a ) ;
  }
  //
  // use GSL: 
  Gaudi::Math::GSL::GSL_Error_Handler sentry ;
  //
  gsl_sf_result result ;
  const int ierror  = gsl_sf_hyperg_1F1_int_e ( a  , b , x, &result) ;
  if ( ierror ) 
  {
    //
    if      ( ierror == GSL_EDOM     ) // input domain error, e.g sqrt(-1)
    { return std::numeric_limits<double>::quiet_NaN(); }
    //
    if ( ierror == GSL_ERANGE   ||    // output range error, e.g. exp(1e100)
         ierror == GSL_EINVAL   ||    // invalid argument supplied by user
         ierror == GSL_EUNDRFLW ||    // underflow
         ierror == GSL_EOVRFLW  ||    // overflow
         ierror == GSL_ELOSS    ||    // loss of accuracy
         ierror == GSL_EROUND    )    // failed because of roundoff error
    {}
    else
    {
      gsl_error ( "Error from kummer function" ,
                  __FILE__ , __LINE__ , ierror ) ;
    } 
  }
  //
  return result.val ;
}
// ============================================================================
// The END 
// ============================================================================
