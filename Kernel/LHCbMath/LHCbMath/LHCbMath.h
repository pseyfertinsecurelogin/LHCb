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
/** @file
 *
 *  Collection of math related functions for general use in LHCb
 *
 *  @author Juan PALACIOS
 *  @date   2005-11-21
 */
// ============================================================================
#ifndef LHCBMATH_LHCBMATH_H
#define LHCBMATH_LHCBMATH_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <cmath>
#include <algorithm>
#include <vector>
#include <array>
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/Lomont.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/call_traits.hpp"
#include "boost/integer_traits.hpp"
#include "boost/static_assert.hpp"
#include "boost/numeric/conversion/converter.hpp"
// ============================================================================
namespace LHCb
{
  // ==========================================================================
  /** @namespace LHCb::Math
   *  Collection of math related functions for general use in LHCb
   */
  namespace Math
  {
    // ========================================================================
    /// Parameters for numerical calculations (M.Needham)
    static const double hiTolerance    = 1e-40;
    static const double lowTolerance   = 1e-20;
    static const double looseTolerance = 1e-5;
    static const double     sqrt_12 = 3.4641016151377546; // sqrt(12.)
    static const double inv_sqrt_12 = 0.2886751345948129; // 1./sqrt(12.)
    // ========================================================================
    /** @var mULPS_float
     *  "tolerance" parameter for "Lomont"-compare of floating point numbers.
     *  It corresponds to relative ("Knuth/GLS") tolerance of about ~6*10^-6
     *  for values in excess of 10^-37.
     *
     *  @see LHCb::Math::lomont_compare_float
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-01-02
     */
    const unsigned short mULPS_float = 100 ;
    // ========================================================================
    /** @var mULPS_float_low
     *  "Low-tolerance" parameter for "Lomont"-compare of floating point numbers.
     *  It corresponds to relative ("Knuth/GLS") tolerance of about ~6*10^-5
     *  for values in excess of 10^-37.
     *
     *  @see LHCb::Math::lomont_compare_float
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-01-02
     */
    const unsigned short mULPS_float_low = 1000 ;
    // =========================================================================
    /** @var mULPS_double
     *  "tolerance" parameter for "Lomont"-compare of floating point numbers.
     *  It corresponds to relative ("Knuth/GLS") tolerance of about ~6*10^-13
     *  for values in excess of 10^-304.
     *  @see LHCb::Math::lomont_compare_double
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-01-02
     */
    const unsigned int mULPS_double = 1000 ;
    // ========================================================================
    /** @struct abs_less
     *  comparison by absolute value
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-08-17
     */
    template <class TYPE = void>
    struct abs_less final
    {
      template <typename V1, typename V2>
      bool operator() ( V1&& v1 , V2&& v2 ) const
      {
        return std::less{}( std::fabs( std::forward<V1>(v1) ),
                            std::fabs( std::forward<V2>(v2) ) ) ;
      }
    } ;
    // ========================================================================
    /** @struct abs_greater
     *  comparison by absolute value
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-08-17
     */
    template <class TYPE = void>
    struct abs_greater final
    {
      template <typename V1, typename V2>
      bool operator() ( V1&& v1 , V2&& v2 ) const
      {
        return std::greater{}( std::abs(std::forward<V1>( v1 ) ),
                               std::abs(std::forward<V1>( v2 ) ) );
      }
    } ;
    // ========================================================================
    /** return "min_by_abs"
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-08-17
     */
    template <class TYPE>
    inline TYPE absMin ( TYPE v1 , TYPE v2 )
    { return std::min ( std::fabs ( v1 ) , std::fabs ( v2 ) ) ; }
    // ========================================================================
    /** return  "max_by_abs"
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-08-17
     */
    template <class TYPE>
    inline TYPE absMax ( TYPE v1 , TYPE v2 )
    { return std::max ( std::fabs ( v1 ) , std::fabs ( v2 ) ) ; }
    // ========================================================================
    /** compare two double numbers with relative precision 'epsilon'
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
    GAUDI_API
    bool knuth_equal_to_double
    ( const double value1           ,
      const double value2           ,
      const double epsilon = 1.0e-6 ) ;
    // ========================================================================
    /** compare two double numbers with precision 'mULPS'
     *  @param value1 (INPUT) the first value
     *  @param value2 (INPUT) the second value
     *  @param mULPS  (INPUT) the precision
     *  @see LHCb::Math::lomont_compare_double
     *  @see LHCb::Math::mULPS_double
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-27
     */
    inline bool equal_to_double
    ( const double value1                      ,
      const double value2                      ,
      const unsigned int mULPS = mULPS_double  )
    { return LHCb::Math::lomont_compare_double ( value1 , value2 , mULPS ) ; }
    // ========================================================================
    /** @struct Equal_To
     *  helper structure for comparison of floating values
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-27
     */
    template <class TYPE>
    struct Equal_To final
    {
      // ======================================================================
      typedef typename boost::call_traits<const TYPE>::param_type T ;
      // ======================================================================
      /// comparison
      inline bool operator() ( T v1 , T v2 ) const
      {
        std::equal_to<TYPE> cmp ;
        return cmp ( v1 , v2 ) ;
      }
      // ======================================================================
    } ;
    // ========================================================================
    /// partial specialization for const-types
    template <class TYPE>
    struct Equal_To<const TYPE>: public Equal_To<TYPE> {} ;
    // ========================================================================
    /// partial specialization for references
    template <class TYPE>
    struct Equal_To<TYPE&>     : public Equal_To<TYPE> {} ;
    // ========================================================================
    /** explicit specialization for doubles
     *  @see LHCb::Math::mULPS_double
     */
    template <>
    struct Equal_To<double>
    {
    public:
      // ======================================================================
      /// constructor
      Equal_To ( const unsigned int eps = mULPS_double ) : m_cmp ( eps ) {}
      /// comparison:
      inline bool operator() ( const double v1 , const double v2 ) const
      { return m_cmp ( v1 , v2 ) ; }
      // ======================================================================
    private :
      // ======================================================================
      Equal_To ( const double /* eps */ ) ;
      // ======================================================================
    private :
      // ======================================================================
      /// evaluator
      LHCb::Math::Lomont<double> m_cmp ;                      // the evalautor
      // ======================================================================
    };
    // ========================================================================
    /** explicit specialization for long doubles
     *  @see LHCb::Math::mULPS_double
     */
    template <>
    struct Equal_To<long double>
    {
    public:
      // ======================================================================
      /// constructor
      Equal_To ( const unsigned int eps = mULPS_double ) : m_cmp ( eps ) {}
      /// comparison:
      inline bool operator()
      ( const long double v1 ,
        const long double v2 ) const
      {
using namespace std;
#ifdef __INTEL_COMPILER         // Disable ICC remark
  #pragma warning(disable:2259) //  non-pointer conversion may lose significant bits
  #pragma warning(push)
#endif
        return  m_cmp ( static_cast<double> ( v1 ) ,
                        static_cast<double> ( v2 ) ) ;
#ifdef __INTEL_COMPILER         // End disable ICC remark
  #pragma warning(pop)
#endif
      }
      // ======================================================================
    private :
      // ======================================================================
      /// constructor
      Equal_To ( const long double /* eps */ ) ;
      // ======================================================================
    private :
      // ======================================================================
      /// the evaluator
      Equal_To<double> m_cmp ;                                 // the evaluator
      // ======================================================================
    };
    // ========================================================================
    /** explicit specialization for floats
     *  @see LHCb::Math::mULPS_float
     *  @see LHCb::Math::Lomont
     *  @see LHCb::Math::Lomont<float>
     */
    template <>
    struct Equal_To<float>
    {
    public:
      // ======================================================================
      /** constructor
       *  @see LHCb::Math::mULPS_float
       */
      Equal_To ( const unsigned short eps =  mULPS_float ) : m_cmp ( eps ) {}
      /// comparison:
      inline bool operator() ( const float v1 , const float v2 ) const
      { return m_cmp( v1 , v2 ) ; }
      // ======================================================================
    private:
      // ======================================================================
      /// constructor
      Equal_To ( const float /* eps */ ) ;
      // ======================================================================
    private :
      // ======================================================================
      /// the evaluator
      LHCb::Math::Lomont<float> m_cmp ;                       // the evaluator
      // ======================================================================
    } ;
    // ========================================================================
    /** specialisation for vectors
     *  @see LHCb::Math::mULPS_double
     *  @see LHCb::Math::Lomont
     *  @see LHCb::Math::Lomont<double>
     */
    template <>
    struct Equal_To<std::vector<double> >
    {
    public:
      // ======================================================================
      /** constructor
       *  @see LHCb::Math::mULPS_double
       */
      Equal_To ( const unsigned int eps  = mULPS_double ) : m_cmp ( eps ) {}
      // ======================================================================
      /// comparison:
      inline bool operator() ( const std::vector<double>& v1 ,
                               const std::vector<double>& v2 ) const
      {
        return &v1 == &v2 ||
               std::equal ( v1.begin () , v1.end () , v2.begin (), v2.end() , m_cmp ) ;
      }
      /// comparison:
      inline bool operator() ( const std::vector<double>& v1 ,
                               const std::vector<float>&  v2 ) const
      {
        return std::equal ( v1.begin () , v1.end () , v2.begin () , v2.end(), m_cmp ) ;
      }
      /// comparison:
      inline bool operator() ( const std::vector<double>& v1 ,
                               const std::vector<int>&    v2 ) const
      {
        return std::equal ( v1.begin () , v1.end () , v2.begin () , v2.end(), m_cmp ) ;
      }
      /// comparison:
      inline bool operator() ( const std::vector<double>&       v1 ,
                               const std::vector<unsigned int>& v2 ) const
      {
        return std::equal ( v1.begin () , v1.end () , v2.begin () , v2.end(), m_cmp ) ;
      }
      /// comparison:
      inline bool operator() ( const std::vector<float>&  v1 ,
                               const std::vector<double>& v2 ) const
      {
        return std::equal ( v1.begin () , v1.end () , v2.begin () , v2.end(), m_cmp ) ;
      }
      /// comparison:
      inline bool operator() ( const std::vector<int>&    v1 ,
                               const std::vector<double>& v2 ) const
      {
        return std::equal ( v1.begin () , v1.end () , v2.begin () , v2.end(), m_cmp ) ;
      }
      /// comparison:
      inline bool operator() ( const std::vector<unsigned int>& v1 ,
                               const std::vector<double>&       v2 ) const
      {
        return std::equal ( v1.begin () , v1.end () , v2.begin () , v2.end(), m_cmp ) ;
      }
      // ======================================================================
    private:
      // ======================================================================
      /// the evaluator
      Equal_To<double> m_cmp ;                                 // the evaluator
      // ======================================================================
    } ;
    // ========================================================================
    template <class TYPE> struct    Zero ;
    template <class TYPE> struct NonZero ;
    // ========================================================================
    /** @struct Zero
     *  helper structure for comparison of floating values
     *  @author Vanya BELYAEV Ivan.Belyaev@iep.ru
     *  @date 2007-11-27
     */
    template <class TYPE>
    struct Zero
    {
      // ======================================================================
      typedef typename boost::call_traits<const TYPE>::param_type T ;
      /// comparison
      inline bool operator() ( T v ) const { return m_cmp ( v , 0 ) ; }
      // ======================================================================
    private:
      // ======================================================================
      // the comparizon criteria
      Equal_To<TYPE> m_cmp ;
      // ======================================================================
    } ;
    // ========================================================================
    template <>
    struct Zero<double>
    {
      // ======================================================================
      /// comparison
      inline bool operator() ( const double  v ) const
      { return !v || m_cmp ( v , 0 ) ; }
      // ======================================================================
    private:
      // ======================================================================
      // the comparizon criteria
      Equal_To<double> m_cmp ;
      // ======================================================================
    } ;
    // ========================================================================
    template <>
    struct Zero<float>
    {
      // ======================================================================
      /// comparison
      inline bool operator() ( const float  v ) const
      { return !v || m_cmp ( v , 0 ) ; }
      // ======================================================================
    private:
      // ======================================================================
      // the comparizon criteria
      Equal_To<float> m_cmp ;
      // ======================================================================
    } ;
    // ========================================================================
    /// partial specialization for const-types
    template <class TYPE>
    struct Zero<const TYPE>    : public Zero<TYPE>     {} ;
    // ========================================================================
    /// partial specialization for references
    template <class TYPE>
    struct Zero<TYPE&>         : public Zero<TYPE>     {} ;
    // ========================================================================
    /** @struct NotZero
     *  helper structure for comparison of floating values
     *  @author Vanya BELYAEV Ivan.Belyaev@iep.ru
     *  @date 2007-11-27
     */
    template <class TYPE>
    struct NotZero
    {
      // ======================================================================
      typedef typename boost::call_traits<const TYPE>::param_type T ;
      /// comparison
      inline bool operator() ( T v ) const { return !m_zero ( v ) ; }
      // ======================================================================
    private:
      // ======================================================================
      // the comparison criteria
      Zero<TYPE> m_zero ;
      // ======================================================================
    } ;
    /// partial specialization for const-types
    template <class TYPE>
    struct NotZero<const TYPE> : public NotZero<TYPE>  {} ;
    // ========================================================================
    /// partial specialization for references
    template <class TYPE>
    struct NotZero<TYPE&>      : public NotZero<TYPE>  {} ;
    // ========================================================================
    /** specialisation for vectors
     *  @see Gaudi::Math::Zero
     *  @see Gaudi::Math::Equal_To
     *  @see LHCb::Math::Lomont<float>
     */
    template < class TYPE>
    struct Zero< std::vector<TYPE> >
    {
    public:
      // ======================================================================
      inline bool operator () ( const std::vector<TYPE>& v ) const
      {
        /// empty vector or all elements are zeros
        return v.empty() || ( v.end() == std::find_if ( v.begin() , v.end  () , m_nz ) ) ;
      }
      // ======================================================================
    private :
      // ======================================================================
      // comparison criteria for elements
      NotZero<TYPE> m_nz ;
      // ======================================================================
    } ;
    // ========================================================================
    /// Is value sufficiently  small ?
    template <class TYPE>
    struct Small
    {
      // ======================================================================
      typedef  TYPE   Inner ;
      // ======================================================================
      // constructor with threshold
      Small ( const TYPE& a ) : m_a ( std::abs ( a ) ) {}
      // the opnly one important method
      inline bool operator() ( const TYPE& a ) const
      { return std::abs ( a ) <= m_a ; }
      // ======================================================================
    private :
      // ======================================================================
      /// default constructor is disabled
      Small () ;  // default constructor is disabled
      // ======================================================================
    private :
      // ======================================================================
      TYPE m_a ;
      // ======================================================================
    } ;
    // ========================================================================
    /** specialization for vectors
     *  vector is small, if empty or all elements are small
     */
    template <class TYPE>
    struct Small<std::vector<TYPE> >
    {
      // ======================================================================
      typedef TYPE Inner ;
      // ======================================================================
      // constructor with threshold
      Small ( const typename Small<TYPE>::Inner & a ) : m_cmp ( a ) {}
      // the only one important method
      inline bool operator() ( const std::vector<TYPE>& v ) const
      {
        return
          v.empty() || v.end() == std::find_if
          ( v.begin() , v.end() , std::not1 ( m_cmp ) ) ;
      }
      // ======================================================================
    private :
      // ======================================================================
      /// default constructor is disabled
      Small () ;  // default constructor is disabled
      // ======================================================================
    private :
      // ======================================================================
      Small<TYPE> m_cmp ;
      // ======================================================================
    } ;
    // ========================================================================
    /** @struct LessOrEqual
     *  check if two values ar less or equal (numerically)
     *  @see Gaudi::Math::Equal_To
     */
    template <class TYPE>
    struct LessOrEqual final
    {
      // ======================================================================
      /// the only one method:  o1 <= o2 or o1 \approx p2
      inline bool operator () ( const TYPE& o1 , const TYPE& o2 ) const
      { return m_leq ( o1 , o2 ) || m_equal ( o1 , o2 ) ; }
      // ======================================================================
    private:
      // ======================================================================
      std::less_equal<>          m_leq   ; // ordering criteria
      LHCb::Math::Equal_To<TYPE> m_equal ; // equality criteria
      // ======================================================================
    } ;
    // ========================================================================
    /** @struct GreaterOrEqual
     *  check if two values are greater or equal (numerically)
     *  @see Gaudi::Math::Equal_To
     */
    template <class TYPE>
    struct GreaterOrEqual final
    {
      // ======================================================================
      /// the only one method:  o1 >= o2 or o1 \approx = o2
      inline bool operator () ( const TYPE& o1 , const TYPE& o2 ) const
      { return m_geq ( o1 , o2 ) || m_equal ( o1 , o2 ) ; }
      // ======================================================================
    private:
      // ======================================================================
      std::greater_equal<TYPE>   m_geq   ; // ordering criteria
      LHCb::Math::Equal_To<TYPE> m_equal ; // equality criteria
      // ======================================================================
    } ;
    // ========================================================================
    /** @struct  NumLess
     *  "Numerically less"
     *  useful structure for sorting
     *  @see Gaudi::Math::Equal_To
     */
    template <class TYPE>
    struct NumLess final
    {
      // ======================================================================
      inline bool operator () ( const TYPE& o1 , const TYPE& o2 ) const
      { return m_less ( o1 , o2 ) && !m_equal ( o1 , o2 ) ; }
      // ======================================================================
    private:
      // ======================================================================
      /// comparion criteria for objects
      std::less<>                m_less  ; // comparion criteria for objects
      /// equality criteria for  objects
      LHCb::Math::Equal_To<TYPE> m_equal ; // equality criteria for objects
      // ======================================================================
    } ;
    // ========================================================================
    /** @struct MuchSmaller
     *  Is a value of "a" tiny with respect to b ?
     *  -  if b is numerical zero, a is numerical zero also
     *  - otherwise (a+b) is numerically equal to b
     */
    template <class TYPE>
    struct MuchSmaller
    {
    public:
      // ======================================================================
      /** Is a value of "a" tiny with respect to b ?
       *  - if b is numerically zero, a is also zero
       *  - otherwise (a+b) is numerically equal to b
       */
      bool operator () ( const TYPE a ,  const TYPE b )  const
      { return m_zero ( b ) ? m_zero ( a ) : m_equal ( a + b , b ) ; }
      // ======================================================================
    private :
      // ======================================================================
      /// zero ?
      Zero    <TYPE> m_zero  {} ; // zero ?
      Equal_To<TYPE> m_equal {} ; // equality ?
      // ======================================================================
    } ;
    // ========================================================================
    /** @struct Tiny
     *  Is a value of "a" tiny with respect to b ?
     *  -  if b is numerical zero, a is numerical zero also
     *  - otherwise (a+b) is numerically equal to b
     */
    template <class TYPE>
    struct Tiny
    {
    public:
      // ======================================================================
      // constructor
      Tiny ( TYPE  b ) : m_b (  b ) {} ;
      Tiny () = delete ; //  no   default constructor
      // ======================================================================
    public:
      // ======================================================================
      /// Is a value of "a" tiny with respect to b ?
      bool operator () ( const TYPE a )  const { return m_smaller ( a , m_b ) ; }
      // ======================================================================
    private :
      // ======================================================================
      // the reference value
      TYPE              m_b       ;
      MuchSmaller<TYPE> m_smaller ;
      // ======================================================================
    } ;
    // ========================================================================
    /** round to nearest integer, rounds half integers to nearest even integer
     *  It is just a simple wrapper around boost::numeric::converter
     *  @author Vanya BELYAEV Ivan.BElyaev
     */
    inline long round ( const double x )
    {
#ifdef __INTEL_COMPILER         // Disable ICC remark from Boost
  #pragma warning(disable:1572) // Floating-point equality and inequality comparisons are unreliable
  #pragma warning(push)
#endif
      typedef boost::numeric::RoundEven<double> Rounder ;
      typedef boost::numeric::make_converter_from
        <double,
        boost::numeric::silent_overflow_handler,
        Rounder>::to<long>::type Converter ;
      return Converter::convert ( x ) ;
#ifdef __INTEL_COMPILER         // Re-enable ICC remark 1572
  #pragma warning(pop)
#endif
    }
    // ========================================================================
    /** round to nearest integer, rounds half integers to nearest even integer
     *  It is just a simple wrapper around boost::numeric::converter
     *  @author Vanya BELYAEV Ivan.BElyaev
     */
    inline long round ( const float  x )
    {
      typedef boost::numeric::RoundEven<float> Rounder ;
      typedef boost::numeric::make_converter_from
        <float,
        boost::numeric::silent_overflow_handler,
        Rounder>::to<long>::type Converter ;
      return Converter::convert ( x ) ;
    }
    // ========================================================================
    /** get mantissa and exponent
     *  similar to std::frexp, but radix=10)
     *  @param x  INPUT  value
     *  @param e  UPDATE exponent
     *  @return  mantissa     (0.1<=m<1)
     *  @author Vanya BELYAEV Ivan.Belyaev
     *  @date 2015-07-21
     */
    GAUDI_API
    double frexp10 ( const double x , long& e ) ;
    // ========================================================================
    /** get mantissa and exponent
     *  similar to std::frexp, but radix=10)
     *  @param x  INPUT  value
     *  @param e  UPDATE exponent
     *  @return  mantissa    (0.1<=m<1)
     *  @author Vanya BELYAEV Ivan.Belyaev
     *  @date 2015-07-21
     */
    GAUDI_API
    float frexp10 ( const float x , long& e ) ;
    // ========================================================================
    /** get mantissa and exponent
     *  similar to std::frexp, but radix=10)
     *  @param x  INPUT  value
     *  @return   pair of mantissa (0.1<=m<1) and exponent
     *  @author Vanya BELYAEV Ivan.Belyaev
     *  @date 2015-07-21
     */
    GAUDI_API
    std::pair<double,int>
    frexp10 ( const double x ) ;
    // ========================================================================
    /** get mantissa and binary exponent
     *  similar to std::frexp
     *  @param x  INPUT  value
     *  @return   pair of mantissa (0.5<=m<1) and (binary) exponent
     *  @author Vanya BELYAEV Ivan.Belyaev
     *  @date 2015-07-21
     */
    GAUDI_API
    std::pair<double,int>
    frexp2  ( const double x ) ;
    // ========================================================================
    /** round to N-significant digits
     *  @param x  INPUT  input value
     *  @param n  INPUT  number of significnat digits
     *  @return rounded value
     *  @author Vanya BELYAEV Ivan.Belyaev
     *  @date 2015-07-21
     */
    GAUDI_API
    double round_N ( const double x , const unsigned short n ) ;
    // ========================================================================
    /** round to N-significant digits
     *  @param x  INPUT  input value
     *  @param n  INPUT  number of significnat digits
     *  @return rounded value
     *  @author Vanya BELYAEV Ivan.Belyaev
     *  @date 2015-07-21
     */
    GAUDI_API
    float round_N ( const float x , const unsigned short n ) ;
    // ========================================================================
    /** is the value actually long ?
     *  @author Vanya BELYAEV Ivan.Belyaev
     *  @date 2011-07-18
     */
    GAUDI_API
    bool islong ( const double x ) ;
    // ========================================================================
    /** is the value actually long ?
     *  @author Vanya BELYAEV Ivan.Belyaev
     *  @date 2011-07-18
     */
    GAUDI_API
    bool islong ( const float  x ) ;
    // ========================================================================
    /** is the value actually int ?
     *  @author Vanya BELYAEV Ivan.Belyaev
     *  @date 2011-07-18
     */
    GAUDI_API
    bool isint  ( const double x ) ;
    // ========================================================================
    /** is the value actually int ?
     *  @author Vanya BELYAEV Ivan.Belyaev
     *  @date 2011-07-18
     */
    GAUDI_API
    bool isint  ( const float  x ) ;
    // ========================================================================
    /** check if the double value is actually equal to the integer value
     *  @param val value to be compared with the integer
     *  @param ref the reference integer number
     *  @param mULPS the precision
     *  @see Gaudi::Math::lomont_compare_double
     *  @see LHCb::Math::mULPS_double
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-17
     */
    GAUDI_API
    bool equal_to_int
    ( const double       val                  ,
      const int          ref                  ,
      const unsigned int mULPS = mULPS_double ) ;
    // ========================================================================
    /** check if the double value is actually equal to the integer value
     *  @param ref the reference integer  number
     *  @param val value to be compared with the integer
     *  @param mULPS the precision
     *  @see Gaudi::Math::lomont_compare_double
     *  @see LHCb::Math::mULPS_double
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-17
     */
    inline bool equal_to_int
    ( const int          ref                  ,
      const double       val                  ,
      const unsigned int mULPS = mULPS_double )
    {
      return equal_to_int ( val , ref , mULPS ) ;
    }
    // ========================================================================
    /** check if the double value is actually equal to the unsigned integer value
     *  @param val value to be compared with the unsigned integer
     *  @param ref the reference unsigned integer number
     *  @param mULPS the precision
     *  @see Gaudi::Math::lomont_compare_double
     *  @see LHCb::Math::mULPS_double
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-17
     */
    GAUDI_API
    bool equal_to_uint
    ( const double       val                  ,
      const unsigned int ref                  ,
      const unsigned int mULPS = mULPS_double ) ;
    // ========================================================================
    /** check if the double value is actually equal to the integer value
     *  @param val value to be compared with the unsigned integer
     *  @param ref the reference unsigned integer number
     *  @param mULPS the precision
     *  @see Gaudi::Math::lomont_compare_double
     *  @see LHCb::Math::mULPS_double
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-17
     */
    inline bool equal_to_uint
    ( const unsigned int ref                  ,
      const double       val                  ,
      const unsigned int mULPS = mULPS_double )
    {
      return equal_to_uint ( val , ref , mULPS ) ;
    }
    // ========================================================================
    /// simple scaling of elements of non-constant sequence
    template <class ITERATOR, typename SCALAR>
    void scale ( ITERATOR first  ,
                 ITERATOR last   ,
                 SCALAR   factor )
    { for ( ; first != last ; ++first ) { (*first) *= factor ; } }
    // ========================================================================
    /// shift all elements of non-constant sequence
    template <class ITERATOR, typename SCALAR>
    void shift ( ITERATOR first  ,
                 ITERATOR last   ,
                 SCALAR   factor )
    { for ( ; first != last ; ++first ) { (*first) += factor ; } }
    // ========================================================================
    /// scale all elements of vector
    template <class TYPE , typename SCALAR>
    void scale ( std::vector<TYPE>& vct , SCALAR factor )
    { scale    ( vct.begin() , vct.end () , factor ) ; }
    // ========================================================================
    /// shift all elements of vector
    template <class TYPE , typename SCALAR>
    void shift ( std::vector<TYPE>& vct , SCALAR factor )
    { shift    ( vct.begin() , vct.end () , factor ) ; }
    // ========================================================================
    template <class ITERATOR>
    void negate ( ITERATOR first , ITERATOR last )
    { for ( ; first != last ; ++first ) { (*first) = -(*first) ; } }
    // ========================================================================
    template <class TYPE>
    void negate ( std::vector<TYPE>& vct )
    { negate ( vct.begin() , vct.end() ) ; }
    // ========================================================================
    /// signed sqrt
    inline double signed_sqrt ( const double value )
    {
      return
        0 < value ? std::sqrt ( value ) : -std::sqrt( std::abs ( value ) ) ;
    }
    // ========================================================================
    /// simple scaling of exponents for all elements of non-constant sequence
    template <class ITERATOR>
    void scale_exp2 ( ITERATOR    first ,
                      ITERATOR    last  ,
                      const int   iexp  )
    {
      if ( 0 != iexp )
      { for ( ; first != last ; ++first ) { (*first) = std::ldexp ( *first , iexp ) ; } }
    }
    // ========================================================================
    /// scale all elements of vector
    template <class TYPE>
    inline void scale_exp2 ( std::vector<TYPE>& vct , const int iexp  )
    { scale_exp2 ( vct.begin() , vct.end () , iexp ) ; }
    // ========================================================================
    /** scale all elements of vector  by 2**s
     *  @attention new vectro is returned!
     */
    template <class TYPE>
    inline
    std::vector<TYPE>
    ldexp ( std::vector<TYPE> vct , const short iexp )
    {
      if ( 0 != iexp ) { scale_exp2 ( vct , iexp ) ; }
      return vct ;
    }
    // ========================================================================
    /** Calculate p-norm for the vector
     *  \f$ |v|_{p} \equiv = \left( \sum_i  \left| v_i\right|^{p} \right)^{1/p}\f$
     *  Few special cases:
     *  - p==1        : sum of absolute values
     *  - p==infinity : the maximal absolute value
     *  @param begin begin-itetator for the sequnce of coefficients
     *  @param end   end-iterator for the sequnce of coefficients
     *  @param pinv  (1/p)
     *  @return p-norm of the vector
     */
    template <class ITERATOR>
    long double p_norm
    ( ITERATOR      begin ,
      ITERATOR      end   ,
      const double  pinv  )  //  1/p
    {
      /// check i/p
      const double ip = pinv < 0 ? 0 : pinv > 1 ? 1  : pinv ;
      ///
      long  double r  = 0 ;
      /// few "easy" cases:  treat explicitely
      /// 1) (p==1)        : sum of absolute values
      if      ( 1 == ip )
      {
        for ( ; begin != end ; ++begin )
        { const long double c = *begin ; r += std::abs ( c ) ; }
        return r ;                                                     // RETURN
      }
      /// 2) (p==infinity) : maximal  absolute value
      else if ( 0 == ip )    // p = infinity
      {
        for ( ; begin != end ; ++begin )
        {
          const long double c = *begin ;
          r = std::max ( r , std::abs(c) ) ;
        }
        return r ;                                                      // RETURN
      }
      /// 3) (p==2) : frequent case
      else if ( 0.5 == ip )  // p = 2 : frequent case
      {
        for ( ; begin != end ; ++begin )
        { const long double c  = *begin ; r += c * c ; }
        return std::sqrt ( r ) ;                                        // RETURN
      }
      /// 4) not very large integer
      else if (  ( 0.05 < ip ) && LHCb::Math::isint ( 1/ip ) )
      {
        const unsigned short p = LHCb::Math::round ( 1/ip ) ;
        for ( ; begin != end ; ++begin )
        {
          const long double c = *begin ;
          r += std::pow ( std::abs ( c ) , p ) ;
        }
        return std::pow ( r , ip ) ;                                    // RETURN
      }
      /// 5) generic case
      const double p = 1/ip ;
      for ( ; begin != end ; ++begin )
      {
        const long  double c = *begin ;
        r += std::pow ( std::abs ( c )  , p ) ;
      }
      return std::pow ( r , ip ) ;
    }
    // ========================================================================
    /** Calculate p-norm for the vector
     *  \f$ |v|_{p} \equiv = \left( \sum_i  \left| v_i\right|^{p} \right)^{1/p}\f$
     *  @param vct  the vector
     *  @param pinv  (1/p)
     *  @return p-norm of the vector
     */
    template <class TYPE>
    long double p_norm
    ( const std::vector<TYPE>& vct  ,
      const double             pinv )  //  1/p
    { return p_norm (  vct.begin() , vct.end() , pinv ) ; }
    // ========================================================================
    /** sign of the number
     *  @see https://stackoverflow.com/a/4609795
     */
    template <typename T>
    inline constexpr signed char signum ( T x , std::false_type /* is_signed */ )
    { return T(0)< x; }
    template <typename T>
    inline constexpr signed char signum ( T x , std::true_type  /* is_signed */ )
    { return ( T(0) < x ) - ( x < T(0) ); }
    template <typename T>
    inline constexpr signed char signum ( T x )
    { return signum ( x , std::is_signed<T>() ) ; }
    // ========================================================================
    /** number of (strickt) sign-variations in the sequence
     *  @param first begin-iterator for the input sequence
     *  @param last  end-iterator for the  input sequence
     *  @return number if strickt sign varination
     */
    template <class ITERATOR, class ZERO>
    unsigned int sign_changes
    ( ITERATOR first ,
      ITERATOR last  ,
      ZERO     zero  )
    {
      while ( first != last && zero ( *first ) ) { ++first ; }
      //
      if ( first == last ) { return 0 ; }         //   RETURN
      //
      signed   char si = signum ( *first ) ;
      unsigned int  nc = 0 ;
      for ( ITERATOR j = first + 1 ; j != last ; ++j )
      {
        if ( zero ( *j ) )  { continue ;  }         // CONTINUE
        const signed char sj  = signum ( *j ) ;
        if ( 0 <= si * sj ) { continue ;  }         // CONTINUE
        nc +=1  ;
        si = sj ;
      }
      return nc ;
    }
    // ========================================================================
  } //                                              end of namespace LHCb::Math
  // ==========================================================================
} //                                                      end of namespace LHCb
// ============================================================================
namespace Gaudi
{
  // ==========================================================================
  namespace Math
  {
    // ========================================================================
    using LHCb::Math::scale ;
    using LHCb::Math::shift ;
    // ========================================================================
  } //                                             end of namespace Gaudi::Math
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
namespace Gaudi
{
  // ==========================================================================
  namespace Math
  {
    // ========================================================================
    /** make dot-multiplication of two sequences using std::fma
     *  \f$ r = \sum_i  x_i y_i \f$
     *  @param begin  (INPUT) start of the first sequence
     *  @param end    (INPUT) end   of the first sequence
     *  @param begin2 (INPUT) start iterator of the second sequence
     *  @return   "dot" product of two sequences
     *
     *  @see http://en.cppreference.com/w/cpp/numeric/math/fma
     *  "...the function std::fma evaluates faster
     *   (in addition to being more precise) than the expression x*y+z for
     *   float, double, and long double arguments, respectively. "
     */
    template <class ITERATOR1, class ITERATOR2>
    inline double dot_fma
    ( ITERATOR1 begin  ,
      ITERATOR1 end    ,
      ITERATOR2 begin2 )
    {
      long double dot = 0 ;
      for ( ; begin != end ; ++begin, ++begin2 )
      { dot = std::fma ( *begin , *begin2 , dot ) ; }
      return dot  ;
    }
    // ========================================================================
    /** make dot-multiplication of two sequences using std::fma
     *  \f$ r = \sum_i  x_i y_i \f$
     *  @param x     (INPUT) the first sequence
     *  @param begin (INPUT) start iterator of the second sequence
     *  @return   "dot" product of two sequences
     */
    template <unsigned int N, class TYPE, class ITERATOR>
    inline double dot_fma
    ( const std::array<TYPE,N>& x     ,
      ITERATOR                  begin )
    { return dot_fma ( x.begin() , x.end() , begin ) ; }
    // ========================================================================
    /** make dot-multiplication of two sequences using std::fma
     *  \f$ r = \sum_i  x_i y_i \f$
     *  @param x     (INPUT) the first sequence
     *  @param begin (INPUT) start iterator of the second sequence
     *  @return   "dot" product of two sequences
     */
    template <unsigned int N, class TYPE1, class TYPE2>
    inline double dot_fma
    ( const std::array<TYPE1,N>& x ,
      const std::array<TYPE2,N>& y )
    { return dot_fma ( x.begin() , x.end() , y.begin() ) ; }
    // ========================================================================
    /** Kahan summation
     *  @see https://en.wikipedia.org/wiki/Kahan_summation_algorithm
     *  \f$ r = \sum_i x_i \f$
     *  @code
     *  // pseudocode
     *  function KahanSum(input)
     *    var sum = 0.0
     *    var c = 0.0
     *    // A running compensation for lost low-order bits.
     *    for i = 1 to input.length do
     *       var y = input[i] - c
     *    // So far, so good: c is zero.
     *       var t = sum + y
     *    // Alas, sum is big, y small, so low-order digits of y are lost.
     *       c = (t - sum) - y
     *    // (t - sum) cancels the high-order part of y; subtracting y recovers negative (low part of y)
     *       sum = t
     *    // Algebraically, c should always be zero. Beware overly-aggressive optimizing compilers!
     *    next i
     *    // Next time around, the lost low part will be added to y in a fresh attempt.
     *  return sum
     *  @endcode
     *  @param begin (INPUT) begin-iterator for the input data
     *  @param end   (INPUT) end-iterator for the input data
     */
    template <class ITERATOR>
    inline double sum_kahan
    ( ITERATOR begin ,
      ITERATOR end   )
    {
      long double sum = 0 ;
      long double c   = 0 ;
      for ( ; begin != end ; ++begin )
      {
        volatile const long double y = (*begin) - c ;
        volatile const long double t = sum      + y ;
        c        = ( t - sum ) - y ;
        sum      =   t             ;
      }
      return sum ;
    }
    // ========================================================================
    /** make dot-multiplication of two sequences based on Kahan summation
     *  \f$ r = \sum_i  x_i y_i \f$
     *  @param xbegin (INPUT) begin-iterator for the first sequence
     *  @param xend   (INPUT) end-iterator for the first sequence
     *  @param ybegin (INPUT) begin-iterator for the second sequence
     */
    template <class ITERATOR1, class ITERATOR2>
    inline double dot_kahan
    ( ITERATOR1 xbegin ,
      ITERATOR1 xend   ,
      ITERATOR2 ybegin )
    {
      long double sum = 0 ;
      long double c   = 0 ;
      for ( ; xbegin != xend ; ++xbegin , ++ybegin )
      {
        const          long double v = (*xbegin ) * (*ybegin) ;
        volatile const long double y = v   - c ;
        volatile const long double t = sum + y ;
        c   = ( t - sum ) - y ;
        sum =   t             ;
      }
      return sum ;
    }
    // ========================================================================
    /** make dot-multiplication of two sequences using Kahan summation
     *  \f$ r = \sum_i  x_i y_i \f$
     *  @param x     (INPUT) the first sequence
     *  @param begin (INPUT) start iterator of the second sequence
     *  @return   "dot" product of two sequences
     */
    template <unsigned int N, class TYPE, class ITERATOR>
    inline double dot_kahan
    ( const std::array<TYPE,N>& x     ,
      ITERATOR                  begin )
    { return dot_kahan ( x.begin() , x.end() , begin ) ; }
    // ========================================================================
    /** make dot-multiplication of two sequences using Kahan summation
     *  \f$ r = \sum_i  x_i y_i \f$
     *  @param x (INPUT) the first sequence
     *  @param y (INPUT) the second sequence
     *  @return   "dot" product of two sequences
     */
    template <unsigned int N, class TYPE1, class TYPE2>
    inline double dot_kahan
    ( const std::array<TYPE1,N>& x ,
      const std::array<TYPE2,N>& y )
    { return dor_kahan ( x.begin() , x.end() , y.begin() ) ; }
    // ========================================================================
    /** make dot-multiplication of two sequences using Kahan summation
     *  \f$ r = \sum_i  x_i y_i \f$
     *  @param x (INPUT) the first sequence
     *  @param y (INPUT) begin-iterator for the second sequence
     *  @return   "dot" product of two sequences
     */
    template <class TYPE,unsigned int N, class ITERATOR>
    inline double dot_kahan
    ( TYPE(&x)[N] ,
      ITERATOR y  ) { return dot_kahan ( x , x + N , y ) ; }
   // ========================================================================
    /** make dot-multiplication of two sequences using Kahan summation
     *  \f$ r = \sum_i  x_i y_i \f$
     *  @param x (INPUT) the first sequence
     *  @param y (INPUT) the second sequence
     *  @return   "dot" product of two sequences
     */
    template <class TYPE1, class TYPE2, unsigned int N>
    inline double dot_kahan
    ( TYPE1(&x)[N] ,
      TYPE2(&y)[N] ) { return dot_kahan ( x , x + N , y ) ; }
    // ========================================================================
    /** make dot-multiplication of two sequences using Kahan summation
     *  \f$ r = \sum_i  x_i y_i \f$
     *  @param N (INPUT) length of the sequences
     *  @param x (INPUT) the first sequence
     *  @param y (INPUT) the second sequence
     *  @return   "dot" product of two sequences
     */
    inline double dot_kahan
    ( const unsigned int N ,
      const double*      x ,
      const double*      y ) { return dot_kahan ( x , x + N , y ) ; }
    // ========================================================================
  } //                                             end of namespace Gaudi::Math
  // ==========================================================================
}
// ============================================================================
// The END
// ============================================================================
#endif // LHCBMATH_LHCBMATH_H
// ============================================================================
