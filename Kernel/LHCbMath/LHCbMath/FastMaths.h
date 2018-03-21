
//-----------------------------------------------------------------------------
/** @file FastMaths.h
 *
 * Implementation of various fast mathematical functions with SIMD support.
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date   30/11/2017
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <cmath>
#include <limits>
#include <utility>
#include <cstdint>

// Local
#include "LHCbMath/SIMDTypes.h"

// VDT
#include "vdt/exp.h"
#include "vdt/log.h"
#include "vdt/atan2.h"
#include "vdt/sincos.h"
#include "vdt/sqrt.h"
#include "vdt/asin.h"
#include "vdt/tan.h"

namespace LHCb
{
  namespace Math
  {

    //------------------------------------------------------------------------------

    /// Hidden(ish) details
    namespace details
    {
      // Some SIMD constants
      const LHCb::SIMD::FPF MAXNUMF  ( 3.4028234663852885981170418348451692544e38f );
      // numbers
      const LHCb::SIMD::FPF HALF        ( 0.5 );                ///< 1/2
      const LHCb::SIMD::FPF TWO         ( 2.0 );                ///< 2
      const LHCb::SIMD::FPF THREEHALVES ( 1.5 );                ///< 3/2
      const LHCb::SIMD::FPF TAN_PIO8F   ( 0.4142135623730950 ); ///< tan(pi/8)
      const LHCb::SIMD::FPF PIO4F       ( M_PI_4 );             ///< pi/4
      const LHCb::SIMD::FPF PIO2F       ( M_PI_2 );             ///< pi/2
      const LHCb::SIMD::FPF PIF         ( M_PI   );             ///< pi
      const LHCb::SIMD::FPF ONEOPIO4F   ( 4.0/M_PI );           ///< 4/pi
      // For exp
      const LHCb::SIMD::FPF LOG2EF   (  1.44269504088896341f ); ///< log2(e)
      const LHCb::SIMD::FPF C1F      (  0.693359375f       );
      const LHCb::SIMD::FPF C2F      ( -2.12194440e-4f     );
      const LHCb::SIMD::FPF C1PC2F   (  C1F + C2F          );
      const LHCb::SIMD::FPF PX1expf  (  1.9875691500E-4f   );
      const LHCb::SIMD::FPF PX2expf  (  1.3981999507E-3f   );
      const LHCb::SIMD::FPF PX3expf  (  8.3334519073E-3f   );
      const LHCb::SIMD::FPF PX4expf  (  4.1665795894E-2f   );
      const LHCb::SIMD::FPF PX5expf  (  1.6666665459E-1f   );
      const LHCb::SIMD::FPF PX6expf  (  5.0000001201E-1f   );
      const LHCb::SIMD::FPF MAXLOGF  (  88.72283905206835f );
      const LHCb::SIMD::FPF MINLOGF  ( -88.0f              );
      const LHCb::SIMD::FPF INFF     ( std::numeric_limits<float>::infinity() );
      // for log
      const LHCb::SIMD::FPF SQRTHF   (  0.707106781186547524f );
      const LHCb::SIMD::FPF PX1logf  (  7.0376836292E-2f );
      const LHCb::SIMD::FPF PX2logf  ( -1.1514610310E-1f );
      const LHCb::SIMD::FPF PX3logf  (  1.1676998740E-1f );
      const LHCb::SIMD::FPF PX4logf  ( -1.2420140846E-1f );
      const LHCb::SIMD::FPF PX5logf  (  1.4249322787E-1f );
      const LHCb::SIMD::FPF PX6logf  ( -1.6668057665E-1f );
      const LHCb::SIMD::FPF PX7logf  (  2.0000714765E-1f );
      const LHCb::SIMD::FPF PX8logf  ( -2.4999993993E-1f );
      const LHCb::SIMD::FPF PX9logf  (  3.3333331174E-1f );
      const LHCb::SIMD::FPF LOGF_UPPER_LIMIT ( MAXNUMF );
      const LHCb::SIMD::FPF LOGF_LOWER_LIMIT ( 0 );
      // for sin cos tan
      const LHCb::SIMD::FPF DP1F     ( 0.78515625               );
      const LHCb::SIMD::FPF DP2F     ( 2.4187564849853515625e-4 );
      const LHCb::SIMD::FPF DP3F     ( 3.77489497744594108e-8   );
      
      /// Converts a float to an int
      inline LHCb::SIMD::UInt32 float2uint32( const LHCb::SIMD::FPF& x ) noexcept
      {
        using namespace LHCb::SIMD;
        const union { FPF f; UInt32 i; } vp = { x };
        return vp.i;
      }

      /// Converts an int to a float
      inline LHCb::SIMD::FPF uint322float( const LHCb::SIMD::UInt32& x ) noexcept
      {
        using namespace LHCb::SIMD;
        const union { UInt32 i; FPF f; } vp = { x };
        return vp.f;
      }

      /// floor implementation
      inline LHCb::SIMD::FPF fpfloor( const LHCb::SIMD::FPF& x ) noexcept
      {
        using namespace LHCb::SIMD;
        //return std::floor(x); // Vc implementation
        auto ret = simd_cast<Int32>(x);
        ret -= ( float2uint32(x) >> 31 );
        return simd_cast<FPF>(ret);
      }

      // Like frexp but vectorising and the exponent is a float.
      inline std::pair<LHCb::SIMD::FPF,LHCb::SIMD::FPF> 
      getMantExponent( const LHCb::SIMD::FPF& x ) noexcept
      {
        using namespace LHCb::SIMD;

        union { FPF x; UInt32 n; } v = { x };

        const Int32 e = ( v.n >> 23 ) - Int32(127);

        // fractional part
        const UInt32 p05f( 0x3f000000 ); // float2uint32(0.5);
        const UInt32 AA  ( 0x807fffff ); // ~0x7f800000;
        v.n &= AA;
        v.n |= p05f;

        return { v.x, simd_cast<FPF>(e) };
      }

      /// polynominal function for fast_log implementation
      inline LHCb::SIMD::FPF get_log_poly( const LHCb::SIMD::FPF& x ) noexcept
      {
        auto y = x*PX1logf;
        y += PX2logf;  y *= x;
        y += PX3logf;  y *= x;
        y += PX4logf;  y *= x;
        y += PX5logf;  y *= x;
        y += PX6logf;  y *= x;
        y += PX7logf;  y *= x;
        y += PX8logf;  y *= x;
        y += PX9logf;
        return y;
      }

      // For sincos

      /// Reduce to 0 to 45
      inline std::pair<LHCb::SIMD::FPF,LHCb::SIMD::Int32>
      reduce2quadrant( LHCb::SIMD::FPF x ) noexcept
      {
        using namespace LHCb::SIMD;

        // make argument positive
        x = abs(x);

        // integer part of x/PIO4
        auto quad = simd_cast<Int32>( ONEOPIO4F * x );

        quad = ( quad + Int32::One() ) & (~1);

        const auto y = simd_cast<FPF>(quad);

        return { ( ( x - y * DP1F ) - y * DP2F ) - y * DP3F , quad };
      }

      /// Sincos only for -45deg < x < 45deg
      inline void fast_sincosf_m45_45( const LHCb::SIMD::FPF& x, 
                                       LHCb::SIMD::FPF& s, 
                                       LHCb::SIMD::FPF& c ) noexcept
      {
        using namespace LHCb::SIMD;
        auto z = x * x;
        s = (((   FPF(-1.9515295891E-4f) * z
                + FPF(8.3321608736E-3f)) * z
                - FPF(1.6666654611E-1f)) * z * x )
          + x;
        c = ((    FPF(2.443315711809948E-005f) * z
                - FPF(1.388731625493765E-003f)) * z
                + FPF(4.166664568298827E-002f)) * z * z
          - details::HALF * z + FPF::One();
      }

      /// Sqrt implmentation from Quake3
      //template < std::size_t ITERATIONS >
      //[[deprecated("superseded by LHCb::Math::rsqrt")]]
      //inline LHCb::SIMD::FPF fast_isqrtf_general( const LHCb::SIMD::FPF& x ) noexcept
      //{
      //  using namespace LHCb::SIMD;
      //  const FPF x2 = x * HALF;
      //  union { FPF y; UInt32 i; } z = { x };
      //  const UInt32 magic ( 0x5f3759df );
      //  z.i = magic - ( z.i >> 1 );
      //  for ( std::size_t j = 0; j < ITERATIONS; ++j )
      //  {
      //    z.y *= ( THREEHALVES - ( x2 * z.y * z.y ) );
      //  }
      //  return z.y;
      //}

      /// Get the sign mask
      inline LHCb::SIMD::UInt32 getSignMask( const LHCb::SIMD::FPF& x ) noexcept
      {
        using namespace LHCb::SIMD;
        union { FPF y; UInt32 i; } z = { x };
        return z.i & UInt32(0x80000000);
      }

      /// Makes an XOR of a float and a unsigned long
      inline LHCb::SIMD::FPF spXORuint32( const LHCb::SIMD::FPF& x, 
                                          const LHCb::SIMD::UInt32& i )
      {
        using namespace LHCb::SIMD;
        union { FPF y; UInt32 i; } z = { x };
        z.i ^= i;
        return z.y;
      }

      /// Makes an OR of a float and a unsigned long
      inline LHCb::SIMD::FPF spORuint32( const LHCb::SIMD::FPF& x, 
                                         const LHCb::SIMD::UInt32& i )
      {
        using namespace LHCb::SIMD;
        union { FPF y; UInt32 i; } z = { x };
        z.i |= i;
        return z.y;
      }

    } // details

    //------------------------------------------------------------------------------

    /** fast asin for SIMD float type
     *  Based on VDT fast_asinf */
    inline LHCb::SIMD::FPF fast_asin( LHCb::SIMD::FPF x ) noexcept
    {
      using namespace LHCb::SIMD;

      const auto sign_mask = details::getSignMask(x);
      const auto a         = abs(x);
      
      const auto m = ( a > details::HALF );

      x      = a;
      auto z = x * x;
      if ( any_of(m) )
      {
        z(m) = details::HALF * ( FPF::One() - a );
        x(m) = std::sqrt(z);
      }

      z = ((((   FPF(4.2163199048E-2f)  * z
               + FPF(2.4181311049E-2f)) * z
               + FPF(4.5470025998E-2f)) * z
               + FPF(7.4953002686E-2f)) * z
               + FPF(1.6666752422E-1f)) * z * x 
               + x;

      z(m) = details::PIO2F - ( details::TWO * z );

      return details::spORuint32(z,sign_mask);
    }

    //------------------------------------------------------------------------------

    /** fast acos for SIMD float type
     *  Based on VDT fast_acosf */
    inline LHCb::SIMD::FPF fast_acos( const LHCb::SIMD::FPF& x ) noexcept
    {
      return details::PIO2F - fast_asin(x);
    }

    //------------------------------------------------------------------------------

    /** fast sincos for SIMD float type
     *  Based on VDT fast_sincosf */
    inline void fast_sincos( const LHCb::SIMD::FPF& xx, 
                             LHCb::SIMD::FPF& s, 
                             LHCb::SIMD::FPF& c ) noexcept
    {
      using namespace LHCb::SIMD;

      auto xj = details::reduce2quadrant(xx);

      const Int32 signS = ( xj.second & Int32(4) );

      xj.second -= Int32(2);

      const Int32 signC = ( xj.second & Int32(4) );
      const Int32 poly  = ( xj.second & Int32(2) );

      details::fast_sincosf_m45_45( xj.first, s, c );

      // swap
      const auto swap = simd_cast<FPF::MaskType>( poly == Int32::Zero() );
      const auto tmp = c;
      c(swap) = s;
      s(swap) = tmp;

      c( simd_cast<FPF::MaskType>( signC == Int32::Zero() ) ) = -c;
      s( simd_cast<FPF::MaskType>( signS != Int32::Zero() ) ) = -s;
      s( xx < FPF::Zero()                                   ) = -s;
    }

    //------------------------------------------------------------------------------

    /** fast log for SIMD float type
     *  Based on VDT fast_logf */
    inline LHCb::SIMD::FPF fast_log( const LHCb::SIMD::FPF& initial_x ) noexcept
    {
      using namespace LHCb::SIMD;

      auto x_fe = details::getMantExponent( initial_x );

      auto & x  = x_fe.first;
      auto & fe = x_fe.second;

      const auto m = x > details::SQRTHF;
      fe(m) += FPF::One();
      x(!m) += x;
      x     -= FPF::One();

      const auto x2 = x*x;

      auto res = details::get_log_poly(x);
      res *= x2 * x;

      res += FPF(-2.12194440e-4f) * fe;
      res -= details::HALF * x2;

      res = x + res;

      res += FPF(0.693359375f) * fe;

      res        ( initial_x > details::LOGF_UPPER_LIMIT ) = details::INFF;
      res.setQnan( initial_x < details::LOGF_LOWER_LIMIT );

      return res;
    }

    //------------------------------------------------------------------------------

    /** fast exp for SIMD float type
     *  Based on VDT fast_expf */
    inline LHCb::SIMD::FPF fast_exp( const LHCb::SIMD::FPF& initial_x ) noexcept
    {
      using namespace LHCb::SIMD;

      FPF x = initial_x;

      // floor() truncates toward -infinity.
      FPF z = details::fpfloor( details::LOG2EF * x + details::HALF );

      x -= z * details::C1PC2F;

      const auto n = LHCb::SIMD::simd_cast<Int32>( z );

      z  = details::PX1expf * x;
      z += details::PX2expf;  z *= x;
      z += details::PX3expf;  z *= x;
      z += details::PX4expf;  z *= x;
      z += details::PX5expf;  z *= x;
      z += details::PX6expf;  z *= x*x;
      z += x + FPF::One();

      // multiply by power of 2
      z *= details::uint322float( ( n + Int32(0x7f) ) << 23 );

      z        ( initial_x > details::MAXLOGF ) = details::INFF;
      z.setZero( initial_x < details::MINLOGF );

      return z;
    }

    //------------------------------------------------------------------------------

    /** fast atan2 for SIMD float type
     *  Based on VDT fast_atan2f */
    inline LHCb::SIMD::FPF fast_atan2( const LHCb::SIMD::FPF& y, 
                                       const LHCb::SIMD::FPF& x ) noexcept
    {
      using namespace LHCb::SIMD;

      // move in first octant
      auto xx = abs(x);
      auto yy = abs(y);
      auto tmp ( FPF::Zero() );
      auto m = ( yy > xx );
      if ( any_of(m) )
      {
        tmp(m) = yy;
        yy(m)  = xx;
        xx(m)  = tmp;
        tmp(m) = FPF::One();
      }

      const auto t = yy / xx;
      auto       z = t;
      m            = t > details::TAN_PIO8F;
      if ( any_of(m) ) { z(m) = ( t - FPF::One() ) / ( t + FPF::One() ); }

      const auto z2 = z * z;

      auto ret = ((((   FPF(8.05374449538e-2f)  * z2
                      - FPF(1.38776856032E-1f)) * z2
                      + FPF(1.99777106478E-1f)) * z2
                      - FPF(3.33329491539E-1f)) * z2 * z
                      + z );

      // move back in place
      ret.setZero( xx == FPF::Zero() ||
                   y  == FPF::Zero() );
      ret( m                  ) += details::PIO4F;
      ret( tmp != FPF::Zero() )  = details::PIO2F - ret;
      ret( x    < FPF::Zero() )  = details::PIF   - ret;
      ret( y    < FPF::Zero() )  =                - ret;

      return ret;
    }

    //------------------------------------------------------------------------------

    /** fast tan for SIMD float type
     *  Based on VDT fast_tanf */
    inline LHCb::SIMD::FPF fast_tan( const LHCb::SIMD::FPF& x ) noexcept
    {
      using namespace LHCb::SIMD;

      auto z = details::reduce2quadrant(x);

      const auto zz = z.first * z.first;

      FPF res = ( (((((   FPF(9.38540185543E-3f)  * zz
                        + FPF(3.11992232697E-3f)) * zz
                        + FPF(2.44301354525E-2f)) * zz
                        + FPF(5.34112807005E-2f)) * zz
                        + FPF(1.33387994085E-1f)) * zz
                        + FPF(3.33331568548E-1f)) * zz * z.first
                  + z.first );
      res( zz < FPF(1.0e-14f) ) = z.first;

      // A no branching way to say: if j&2 res = -1/res. You can!!!
      z.second  &= 2;
      z.second >>= 1;
      const auto alt = simd_cast<FPF>( (z.second) ^ UInt32::One() );

      const auto m = x == FPF::Zero();

      res(m) += FPF::One();

      // one coeff is one and one is 0!
      res = simd_cast<FPF>(z.second) * (-FPF::One()/res) + alt * res; 

      const auto sign_mask = details::getSignMask(x);
      auto a = details::spXORuint32( res, sign_mask );
      a.setZero(m);

      return a;
    }

    //------------------------------------------------------------------------------

    /** fast inverse sqrt for SIMD float type
     *  Based on VDT fast_isqrtf */
    //[[deprecated("superseded by LHCb::Math::rsqrt<faster>")]]
    //inline LHCb::SIMD::FPF fast_isqrt( const LHCb::SIMD::FPF& x ) noexcept
    //{
    //  // use two iterations
    //  return details::fast_isqrtf_general<2>(x);
    //}

    /** fast approximate inverse sqrt for SIMD float type
     *  Based on VDT fast_approx_isqrtf */
    //[[deprecated("superseded by LHCb::Math::rsqrt<fastest>")]]
    //inline LHCb::SIMD::FPF fast_approx_isqrt( const LHCb::SIMD::FPF& x ) noexcept
    //{
    //  // use only 1 iteration
    //  return details::fast_isqrtf_general<1>(x);
    //}

    //------------------------------------------------------------------------------

    // Shortcuts to VDT for scalars. 
    // For consistency across all scalar and SIMD types

    /// Fast sin and cos
    inline void fast_sincos( const float  x, float&  s, float&  c ) noexcept { vdt::fast_sincosf(x,s,c); }
    /// Fast sin and cos
    inline void fast_sincos( const double x, double& s, double& c ) noexcept { vdt::fast_sincos(x,s,c);  }

    /// Fast log
    inline float  fast_log( const float  x ) noexcept { return vdt::fast_logf(x); }
    /// Fast log
    inline double fast_log( const double x ) noexcept { return vdt::fast_log(x); }

    /// Fast exp
    inline float  fast_exp( const float  x ) noexcept { return vdt::fast_expf(x); }
    /// Fast exp
    inline double fast_exp( const double x ) noexcept { return vdt::fast_exp(x); }

    /// Fast inverse sqrt
    //[[deprecated("superseded by LHCb::Math::rsqrt<faster>")]]
    //inline float  fast_isqrt( const float  x ) noexcept { return vdt::fast_isqrtf(x); }
    /// Fast inverse sqrt
    //[[deprecated("superseded by LHCb::Math::rsqrt<fast>")]]
    //inline double fast_isqrt( const double x ) noexcept { return vdt::fast_isqrt(x); }

    /// Fast approximate inverse sqrt
    //[[deprecated("superseded by LHCb::Math::rsqrt<fastest>")]]
    //inline float  fast_approx_isqrt( const float  x ) noexcept { return vdt::fast_approx_isqrtf(x); }
    /// Fast approximate inverse sqrt
    //[[deprecated("superseded by LHCb::Math::rsqrt<faster>")]]
    //inline double fast_approx_isqrt( const double x ) noexcept { return vdt::fast_approx_isqrt(x); }

    /// Fast tan
    inline float  fast_tan( const float  x ) noexcept { return vdt::fast_tanf(x); }
    /// Fast tan
    inline double fast_tan( const double x ) noexcept { return vdt::fast_tan(x); }

    /// Fast atan2
    inline float  fast_atan2( const float  y, const float  x ) noexcept { return vdt::fast_atan2f(y,x); }
    /// Fast atan2
    inline double fast_atan2( const double y, const double x ) noexcept { return vdt::fast_atan2(y,x); }

    /// Fast acos
    inline float fast_acos( const float  x ) noexcept { return vdt::fast_acosf(x); }
    /// Fast acos
    inline float fast_acos( const double x ) noexcept { return vdt::fast_acos(x); }

    /// Fast asin
    inline float fast_asin( const float  x ) noexcept { return vdt::fast_asinf(x); }
    /// Fast asin
    inline float fast_asin( const double x ) noexcept { return vdt::fast_asin(x); }

    //------------------------------------------------------------------------------

    /** @namespace Gaudi::Math::Approx
     *
     *  Namespace for fast approximations to various math functions.
     *
     *  These implementations are less precise than those above, but consequently
     *  varying degress faster (depending on how approximate you go). 
     *  Use with care, but in the right case can give good gains.
     *
     *  Based in part on the code at
     *
     *  http://www.machinedlearnings.com/2011/06/fast-approximate-logarithm-exponential.html
     *
     *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
     *  @date   30/11/2017
     */
    namespace Approx
    {

      //------------------------------------------------------------------------------

      // SIMD versions

      /// Fast log2 approximation 
      inline LHCb::SIMD::FPF approx_log2( const LHCb::SIMD::FPF & x ) noexcept
      {
        using namespace LHCb::SIMD;
        const union { FPF f; UInt32 i; } vx = { x };
        const union { UInt32 i; FPF f; } mx = { ( vx.i & UInt32(0x007FFFFF) ) | UInt32(0x3f000000) };
        const auto y = simd_cast<FPF>(vx.i) * FPF(1.1920928955078125e-7f);
        return ( y - FPF(124.22551499f)
                 - FPF(1.498030302f) * mx.f 
                 - FPF(1.72587999f) / ( FPF(0.3520887068f) + mx.f ) );
      }

      /// Fast log approximation 
      inline LHCb::SIMD::FPF approx_log( const LHCb::SIMD::FPF & x ) noexcept
      {
        using namespace LHCb::SIMD;
        return FPF(0.69314718f) * approx_log2( x );
      }
      
      /// Very fast log approximation 
      inline LHCb::SIMD::FPF vapprox_log( const LHCb::SIMD::FPF & x ) noexcept
      {
        using namespace LHCb::SIMD;
        const union { FPF f; UInt32 i; } vx = { x };
        return ( simd_cast<FPF>(vx.i) * FPF(8.2629582881927490e-8f) ) - FPF(87.989971088f);
      }

      /// Fast pow2 approximation
      inline LHCb::SIMD::FPF approx_pow2( const LHCb::SIMD::FPF & p ) noexcept
      {
        using namespace LHCb::SIMD;
        auto clipp = p;
        const FPF A( -126.0f ); 
        clipp( p < A ) = A;
        const auto w = simd_cast<Int32>(clipp);
        auto z = clipp - simd_cast<FPF>(w);
        z( p < FPF::Zero() ) += FPF::One();
        const union { UInt32 i; FPF f; } v = {
          simd_cast<UInt32>( ( 1 << 23 ) * ( clipp + FPF(121.2740575f) + FPF(27.7280233f) /
                                             ( FPF(4.84252568f) - z ) - FPF(1.49012907f) * z ) ) 
        };
        return v.f;
      }

      /// Fast exp approximation
      inline LHCb::SIMD::FPF approx_exp( const LHCb::SIMD::FPF & p ) noexcept
      {
        using namespace LHCb::SIMD;
        return approx_pow2( FPF(1.442695040f) * p );
      }

      /// Very fast pow2 approximation
      inline LHCb::SIMD::FPF vapprox_pow2( const LHCb::SIMD::FPF & p ) noexcept
      {
        using namespace LHCb::SIMD;
        auto clipp = p;
        const FPF A( -126.0f ); 
        clipp( p < A ) = A;
        const union { UInt32 i; FPF f; } v = {
          simd_cast<UInt32>( ( 1 << 23 ) * ( clipp + FPF(126.94269504f) ) ) 
        };
        return v.f;
      }

      /// Very fast exp approximation
      inline LHCb::SIMD::FPF vapprox_exp( const LHCb::SIMD::FPF & p ) noexcept
      {
        using namespace LHCb::SIMD;
        return vapprox_pow2( FPF(1.442695040f) * p );
      }
 
      //------------------------------------------------------------------------------

      // Scalar versions

      /// Fast log2 approximation 
      inline float approx_log2( const float x ) noexcept
      {
        const union { float f; std::uint32_t i; } vx = { x };
        const union { std::uint32_t i; float f; } mx = { ( vx.i & 0x007FFFFF ) | 0x3f000000 };
        const auto y = float(vx.i) * 1.1920928955078125e-7f;
        return ( y - 124.22551499f
                 - 1.498030302f * mx.f 
                 - 1.72587999f / ( 0.3520887068f + mx.f ) );
      }

      /// Fast log approximation 
      inline float approx_log( const float x ) noexcept
      {
        return 0.69314718f * approx_log2( x );
      }

      /// Very Fast log approximation 
      inline float vapprox_log( const float x ) noexcept
      {
        const union { float f; std::uint32_t i; } vx = { x };
        return ( float(vx.i) * 8.2629582881927490e-8f ) - 87.989971088f;
      }

      inline float approx_pow2 ( const float p )
      {
        const float offset   = ( p < 0 ? 1.0f : 0.0f );
        const float clipp    = ( p < -126 ? -126.0f : p );
        const std::int32_t w = clipp;
        const float        z = clipp - w + offset;
        const union { std::uint32_t i; float f; } v = { 
          static_cast<std::uint32_t>( (1 << 23) * ( clipp + 121.2740575f + 27.7280233f / 
                                                   ( 4.84252568f - z ) - 1.49012907f * z ) )
        };
        return v.f;
      }

      /// Fast exp approximation
      inline float approx_exp( const float p ) noexcept
      {
        return approx_pow2( 1.442695040f * p );
      }

      /// Very fast pow2 approximation
      inline float vapprox_pow2( const float p ) noexcept
      {
        const float clipp = ( p < -126 ? -126.0f : p );
        const union { uint32_t i; float f; } v = { 
          static_cast<std::uint32_t>( (1 << 23) * (clipp + 126.94269504f) )
        };
        return v.f;
      }

      /// Very fast exp approximation
      inline float vapprox_exp( const float p ) noexcept
      {
        return vapprox_pow2( 1.442695040f * p );
      }

      //------------------------------------------------------------------------------
      
    }

    namespace details
    {
      template <class T, class Tag = void>
      struct is_noexcept;
    }

    //------------------------------------------------------------------------------
    /**
     * @brief Base class to all accuracy requirement tags
     *
     * Accuracy requirement tags are used to relax the accuracy requirement of a function in order to be faster.
     * See the documentation of the function to know what are the accuracy guarantees.
     *
     * The default accuracy requirement is @ref accurate
     */
    struct accuracy_tag {};

    /**
     * @brief As accurate as efficiently possible.
     *
     * This is the default accuracy requirement.
     * It is designed to be at least as accurate as @ref ieee while being at least as fast as @ref ieee.
     *
     * @note
     * Currently, results may differ between @ref accurate and @ref ieee only on KNL
     * where `_mm_rsqrt28_ps` can compute 1/sqrt(x) with all bits correct;
     * and on ARM32 where there is no Neon instruction to divide or to take a square root.
     */
    struct accurate : accuracy_tag {};

    /**
     * @brief strict IEEE754 compliance
     *
     * This accuracy requirement should be used when you want reproducible results
     * accross compilers, machines, and architectures.
     *
     * @warning
     * On ARM 32bits and PowerPC 32bits, IEEE754 cannot be achieved with SIMD instructions
     * Thus, a fallback using scalar instructions is used on these architectures
     */
    struct ieee : accuracy_tag {};

    /**
     * @brief Relaxed strict accuracy constraint for fast execution
     *
     * This accuracy requirement should give an error less than 2 bits.
     * This usually enables faster execution than @ref accurate.
     *
     * @note
     * In some cases, the @ref fast implementation is the same as the @ref accurate one.
     * In those cases, no timing or accuracy differences could be observed.
     */
    struct fast : accuracy_tag {};

    /**
     * @brief Faster execution, good accuracy
     *
     * This accuracy requirement usually gives good results.
     * It is guaranted to be at least as fast as @ref fast.
     *
     * @note
     * It might use the same implementation as @ref fast if making it faster would give too bad accuracy.
     *
     * @warning
     * The full range of the double precision floating point is not covered with some implementations.
     * Use this accuracy requirement only if the input fits within the range of a single precision `float` (abs in [10^-38, 10^38]).
     * Otherwise, this is undefined behaviour.
     */
    struct faster : accuracy_tag {};

    /**
     * @brief Rough estimation, super fast
     *
     * No constraint on accuracy. It usually results in very rough estimation, but is super fast.
     *
     * @warning
     * The full range of the double precision floating point is not covered with some implementations.
     * Use this accuracy requirement only if the input fits within the range of a single precision `float` (abs in [10^-38, 10^38]).
     * Otherwise, this is undefined behaviour.
     */
    struct fastest : accuracy_tag {};

    /**
     * @brief Computes the reciprocal square root of `x`: 1/sqrt(x).
     *
     * The accuracy of the result depends on template parameter @link accuracy_tag Tag @endlink.
     *
     * Guaranted accuracy in bits:
     * Tag           | `float` | `double`
     * --------------|:-------:|:-------:
     * @ref accurate | 23      | 52
     * @ref ieee     | 23      | 52
     * @ref fast     | 22      | 51
     * @ref faster   | 17      | 44
     * @ref fastest  |  9      | 12
     *
     *
     * @tparam Tag Specifies the accuracy of the result (default: @ref accurate)
     * @tparam T   Any floating point type or SIMD type
     * @param x
     * @return 1/sqrt(x)
     *
     * @note
     * When called with scalar (either `float` or `double`),
     * the compiler is able to use a vectorized variant of the function.
     *
     * @warning
     * This function takes only normal positve numbers.
     * Calling it with 0, negatives, infinity, NaNs or subnormals is undefined behaviour
     *
     * @parblock @b Examples
     *
     * @code{.cpp}
     * // simple use
     * float y = fast_math::rsqrt(x);
     * @endcode
     *
     * @code{.cpp}
     * // simple use with accuracy requirement
     * float y = fast_math::rsqrt<fast_math::ieee>(x);
     * @endcode
     *
     * @code{.cpp}
     * // simple use with accuracy requirement and imported namespace
     * using namespace fast_math;
     * float y = rsqrt<ieee>(x);
     * @endcode
     *
     * @code{.cpp}
     * // In loop use (gets auto-vectorized nicely) 
     * using namespace fast_math;
     * for (int i = 0; i < n; i++) {
     *   A[i] = rsqrt<faster>(B[i]);
     * }
     * @endcode
     *
     * @code{.cpp}
     * // use intrinsic SIMD type
     * using namespace fast_math;
     * __m128 Y = rsqrt(_mm_load_ps(p));
     * @endcode
     */
    template <class Tag = accurate, class T>
    inline T rsqrt(T x) noexcept(details::is_noexcept<T, Tag>::rsqrt);

    /**
     * @brief Refines an approximation of 1/sqrt(x).
     *
     * Uses a Newton-Raphson iteration to increase the accuracy
     *   of an approximation of 1/sqrt(x).
     *
     * Convergence is quadratic (doubles the number of correct bits)
     * 
     * @param x
     * @param y approximation of 1/sqrt(x)
     * @return 1/sqrt(x)
     */
    template <class T>
    inline T rsqrt_nr(T x, T y) noexcept(details::is_noexcept<T>::rsqrt_nr);
  }
}

// If not compiling the implementation, load everything needed
#ifndef LHCB_MATH_RSQRT_IMPL
#ifdef __SSE2__
#include "VectorClass/vectorclass.h"
#endif
#endif

// include rsqrt implementation
#include "LHCbMath/fast_math_rsqrt.h"

namespace LHCb
{
  namespace Math
  {
#ifndef LHCB_MATH_RSQRT_IMPL
    /** fast inverse sqrt for SIMD float type
     *  call rsqrt */
    inline LHCb::SIMD::FPF fast_isqrt( const LHCb::SIMD::FPF& x ) noexcept
    {
      // Qualified Lookup is mandatory for Vc types (ADL would find Vc::rsqrt)
      return LHCb::Math::rsqrt<fast>(x);
    }

    /** fast approximate inverse sqrt for SIMD float type
     *  call rsqrt */
    inline LHCb::SIMD::FPF fast_approx_isqrt( const LHCb::SIMD::FPF& x ) noexcept
    {
      // Qualified Lookup is mandatory for Vc types (ADL would find Vc::rsqrt)
      return LHCb::Math::rsqrt<fastest>(x);
    }

    /// Fast inverse sqrt
    inline float  fast_isqrt( const float  x ) noexcept { return rsqrt<fast>(x); }
    /// Fast inverse sqrt
    inline double fast_isqrt( const double x ) noexcept { return rsqrt<fast>(x); }

    /// Fast approximate inverse sqrt
    inline float  fast_approx_isqrt( const float  x ) noexcept { return rsqrt<fastest>(x); }
    /// Fast approximate inverse sqrt
    inline double fast_approx_isqrt( const double x ) noexcept { return rsqrt<faster>(x); }
#endif
  }
}
