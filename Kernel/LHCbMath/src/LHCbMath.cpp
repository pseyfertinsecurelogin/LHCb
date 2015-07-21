// $Id$
// ============================================================================
// Include files
// ============================================================================
// STD& STL
// ============================================================================
#include <cmath>
// ============================================================================
// GSL 
// ============================================================================
#include "gsl/gsl_sys.h"
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
// ============================================================================
// Local
// ============================================================================
#include "LHCbMath/LHCbMath.h"
#include "LHCbMath/Power.h"
// ============================================================================
// Boost 
// ============================================================================
#include "boost/integer_traits.hpp"
// ============================================================================
/** @file 
 *  Implementation file for functions from namespace LHcb::Math 
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date   2007-11-27
 */
// ============================================================================
/*  compare two double numbers with the relative
 *  precision   'epsilon'
 *
 *  Essentially it is a wrapper to gsl_fcmp function 
 *  from GSL library
 *
 *  See D.E.Knuth, "Seminumerical Algorithms", section 4.2.2
 *  
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-11-27
 */
// ============================================================================
#ifdef __INTEL_COMPILER         // Disable ICC remark
#pragma warning(disable:1572) // floating-point equality and inequality comparisons are unreliable
#endif
// ============================================================================
/* compare two double numbers with relative precision 'epsilon'
 *
 *  Essentially it is a wrapper to gsl_fcmp function from GSL library
 *  See D.E.Knuth, "Seminumerical Algorithms", section 4.2.2
 *
 *  @param value1  (INPUT) the first value 
 *  @param value2  (INPUT) the second value 
 *  @param epsilon (INPUT) the (relative) precision 
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-11-27
 */
// ============================================================================
bool LHCb::Math::knuth_equal_to_double
( const double value1  , 
  const double value2  , 
  const double epsilon ) 
{
  return 
    !epsilon    ? 0 == gsl_fcmp ( value1 , value2 , 1.0e-6  ) : 
    0 < epsilon ? 0 == gsl_fcmp ( value1 , value2 , epsilon ) : 
    0 == gsl_fcmp ( value1 , value2 , -epsilon ) ;
}
// ============================================================================
namespace 
{
  // ==========================================================================
  /// check the specialization 
  static_assert ( boost::integer_traits<long> ::is_specialized     , 
                  "boost::integer_traits<long> is not specialized" ) ;
  static_assert ( boost::integer_traits<int>  ::is_specialized     , 
                  "boost::integer_traits<int>  is not specialized" ) ;
  // ==========================================================================
  const double s_MAX_L =  0.1 + boost::integer_traits<long>::const_max ;
  const double s_MIN_L = -0.1 - boost::integer_traits<long>::const_max ;
  const double s_MAX_I =  0.1 + boost::integer_traits<int>::const_max  ;
  const double s_MIN_I = -0.1 - boost::integer_traits<int>::const_max  ;
  // ==========================================================================
}
// ============================================================================
/*  is the value actually long ?
 *  @author Vanya BELYAEV Ivan.Belyaev       
 *  @date 2011-07-18
 */
// ============================================================================
bool LHCb::Math::islong ( const double x ) 
{
  return 
    x <= s_MIN_L  ? false :
    x >= s_MAX_L  ? false :
    Gaudi::Math::lomont_compare_double ( x                       , 
                                         LHCb::Math::round ( x ) , 
                                         mULPS_double            ) ;
}
// ============================================================================
/*  is the value actually long ?
 *  @author Vanya BELYAEV Ivan.Belyaev       
 *  @date 2011-07-18
 */
// ============================================================================
bool LHCb::Math::islong ( const float x ) 
{
  return 
    x <= s_MIN_L  ? false :
    x >= s_MAX_L  ? false :
    Gaudi::Math::lomont_compare_double ( x                       , 
                                         LHCb::Math::round ( x ) , 
                                         mULPS_float             ) ;
}
// ============================================================================
/*  is the value actually int ?
 *  @author Vanya BELYAEV Ivan.Belyaev       
 *  @date 2011-07-18
 */
// ============================================================================
bool LHCb::Math::isint ( const double x ) 
{
  return 
    x <= s_MIN_I  ? false :
    x >= s_MAX_I  ? false :
    Gaudi::Math::lomont_compare_double ( x                       , 
                                         LHCb::Math::round ( x ) , 
                                         mULPS_double            ) ;
}
// ============================================================================
/*  is the value actually int ?
 *  @author Vanya BELYAEV Ivan.Belyaev       
 *  @date 2011-07-18
 */
// ============================================================================
bool LHCb::Math::isint ( const float x ) 
{
  return 
    x <= s_MIN_I  ? false :
    x >= s_MAX_I  ? false :
    Gaudi::Math::lomont_compare_double ( x                       , 
                                         LHCb::Math::round ( x ) , 
                                         mULPS_float             ) ;
}
// ============================================================================
/* check if the double value is actually equal to the integer value  
 *  @param val value to be compared with the integer 
 *  @param ref the reference integer number 
 *  @param mULPS the precision 
 *  @see Gaudi::Math::lomont_compare_double 
 *  @see LHCb::Math::mULPS_double
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2008-09-17
 */        
// ============================================================================
bool LHCb::Math::equal_to_int 
( const double       val   , 
  const int          ref   , 
  const unsigned int mULPS ) 
{
  const double tmp = ref ;
  return Gaudi::Math::lomont_compare_double ( val , tmp , mULPS ) ;
}
// ============================================================================
/*  check if the double value is actually equal to the unsigned integer value  
 *  @param val value to be compared with the unsigned integer 
 *  @param ref the reference unsigned integer number 
 *  @param mULPS the precision 
 *  @see Gaudi::Math::lomont_compare_double 
 *  @see LHCb::Math::mULPS_double
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2008-09-17
 */        
// ============================================================================
bool LHCb::Math::equal_to_uint 
( const double       val   , 
  const unsigned int ref   , 
  const unsigned int mULPS ) 
{
  const double tmp = ref ;
  return Gaudi::Math::lomont_compare_double ( val , tmp , mULPS ) ;
}
// ============================================================================
/*  get mantissa and exponent 
 *  similar to std::frexp, but radix=10)
 *  @param x  INPUT  value 
 *  @param e  UPDATE exponent 
 *  @return  pair of mantissa (0.1<=m<1) and exponent 
 *  @author Vanya BELYAEV Ivan.Belyaev       
 *  @date 2015-07-21
 */
// ============================================================================
std::pair<double,int> 
LHCb::Math::frexp10 ( const double x ) 
{
  //
  if ( UNLIKELY ( 0 == x ) )  { return std::pair<double,int>( x , 0 ) ; }
  //
  long double xa = 0 < x ?  x : -x ;
  //
  int q  = (int) std::floor ( std::log10 ( xa ) ) ;
  if      ( 0 < q ) { xa /= Gaudi::Math::pow ( 10.0L , (unsigned long)           q   ) ; }
  else if ( 0 > q ) { xa *= Gaudi::Math::pow ( 10.0L , (unsigned long) std::abs( q ) ) ; }
  //
  if ( 1 <= xa ) { xa /= 10 ; ++q ; }
  //
  return 0 < x ? 
    std::pair<double,int>(  xa , q ) : 
    std::pair<double,int>( -xa , q ) ;
}
// ============================================================================
/*  get mantissa and exponent 
 *  similar to std::frexp, but radix=10)
 *  @param x  INPUT  value 
 *  @param e  UPDATE exponent 
 *  @return  mantissa  (0.1<=m<1)
 *  @author Vanya BELYAEV Ivan.Belyaev       
 *  @date 2011-07-18
 */
// ============================================================================
double LHCb::Math::frexp10 ( const double  x , long& e ) 
{
  //
  const std::pair<double,int> r = frexp10 ( x ) ;
  e    = r.second ;
  return r.first  ;
}
// ============================================================================
/*  get mantissa and exponent 
 *  similar to std::frexp, but radix=10)
 *  @param x  INPUT  value 
 *  @param e  UPDATE exponent 
 *  @return  mantissa  (0.1<=m<1)
 *  @author Vanya BELYAEV Ivan.Belyaev       
 *  @date 2011-07-18
 */
// ============================================================================
float LHCb::Math::frexp10 ( const float  x , long& e ) 
{
  const double xx = x ;
  return frexp10 ( xx , e ) ;
}
// ============================================================================
/*  round to N-significant digits 
 *  @param x  INPUT  input value 
 *  @param n  INPUT  number of significnat digits 
 *  @return rounded value 
 *  @author Vanya BELYAEV Ivan.Belyaev       
 *  @date 2015-07-21
 */
// ============================================================================
double LHCb::Math::round_N ( const double x , const unsigned short n ) 
{
  //
  if ( 0 == n ) { return 0 ; } // none of correct digits is required 
  //
  const std::pair<double,int> r = frexp10 ( x ) ;
  //
  const long    e  = r.second -  1 ;
  const double  m  = r.first  * 10 ;
  //
  const long    ni = n - 1 ;
  //
  const double  f1 = Gaudi::Math::pow ( 10.0 , ni )  ;
  const double  f2 =
    ni < e ?     Gaudi::Math::pow ( 10.0 , (unsigned long) ( e  - ni ) ) : 
    ni > e ? 1.0/Gaudi::Math::pow ( 10.0 , (unsigned long) ( ni - e  ) ) : 1 ;
//
return round ( m * f1 ) * f2 ;
}
// ============================================================================
/*  round to N-significant digits 
 *  @param x  INPUT  input value 
 *  @param n  INPUT  number of significnat digits 
 *  @return rounded value 
 *  @author Vanya BELYAEV Ivan.Belyaev       
 *  @date 2015-07-21
 */
// ============================================================================
float LHCb::Math::round_N ( const float x , const unsigned short n ) 
{
  const double xd = x ;
  return round_N ( xd , n ) ;
}

 

// ============================================================================
// The END 
// ============================================================================
