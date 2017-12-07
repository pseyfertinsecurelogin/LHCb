
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
      const LHCb::SIMD::FPF HALF     ( 0.5    );               ///< 1/2
      const LHCb::SIMD::FPF THREEHALVES ( 1.5 );               ///< 3/2
      const LHCb::SIMD::FPF TAN_PIO8F( 0.4142135623730950 );   ///< tan(pi/8)
      const LHCb::SIMD::FPF PIO4F    ( M_PI_4 );               ///< pi/4
      const LHCb::SIMD::FPF PIO2F    ( M_PI_2 );               ///< pi/2
      const LHCb::SIMD::FPF PIF      ( M_PI   );               ///< pi
      const LHCb::SIMD::FPF ONEOPIO4F( 4.0/M_PI );             ///< 4/pi
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
      // for sin cos
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
      template < std::size_t ITERATIONS >
      inline LHCb::SIMD::FPF fast_isqrtf_general( const LHCb::SIMD::FPF& x ) noexcept
      {
        using namespace LHCb::SIMD;
        const FPF x2 = x * HALF;
        union { FPF y; UInt32 i; } z = { x };
        const UInt32 magic ( 0x5f3759df );
        z.i = magic - ( z.i >> 1 );
        for ( std::size_t j = 0; j < ITERATIONS; ++j )
        {
          z.y *= ( THREEHALVES - ( x2 * z.y * z.y ) );
        }
        return z.y;
      }

    } // details

    //------------------------------------------------------------------------------

    /** fast inverse sqrt for SIMD float type
     *  Based on VDT fast_isqrtf */
    inline LHCb::SIMD::FPF fast_isqrt( const LHCb::SIMD::FPF& x ) noexcept
    {
      // use two iterations
      return details::fast_isqrtf_general<2>(x);
    }

    /** fast approximate inverse sqrt for SIMD float type
     *  Based on VDT fast_approx_isqrtf */
    inline LHCb::SIMD::FPF fast_approx_isqrt( const LHCb::SIMD::FPF& x ) noexcept
    {
      // use only 1 iteration
      return details::fast_isqrtf_general<1>(x);
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
    inline float  fast_isqrt( const float  x ) noexcept { return vdt::fast_isqrtf(x); }
    /// Fast inverse sqrt
    inline double fast_isqrt( const double x ) noexcept { return vdt::fast_isqrt(x); }

    /// Fast approximate inverse sqrt
    inline float  fast_approx_isqrt( const float  x ) noexcept { return vdt::fast_approx_isqrtf(x); }
    /// Fast approximate inverse sqrt
    inline double fast_approx_isqrt( const double x ) noexcept { return vdt::fast_approx_isqrt(x); }

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
     using namespace LHCb::SIMD; inline float fast_asin( const float  x ) noexcept { return vdt::fast_asinf(x); }
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

    //------------------------------------------------------------------------------

  }
}
