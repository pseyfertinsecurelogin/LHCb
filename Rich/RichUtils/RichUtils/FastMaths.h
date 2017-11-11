
//-----------------------------------------------------------------------------
/** @file FastMaths.h
 *
 * Implementation of vaious fast mathematical functions for the Rich
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date 02/09/2007
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <cmath>
#include <limits>
#include <utility>

// Local
#include "RichUtils/RichSIMDTypes.h"

// VDT
#include "vdt/exp.h"
#include "vdt/log.h"
#include "vdt/atan2.h"
#include "vdt/sincos.h"

namespace Rich
{

  /** @namespace Rich::SIMD::Maths
   *
   *  Namespace for RICH SIMD Math functons
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   17/10/2017
   */
  namespace Maths
  {

    //------------------------------------------------------------------------------
    
    using FPF    = Rich::SIMD::FPF;
    using Int32  = Rich::SIMD::Int32;
    using UInt32 = Rich::SIMD::UInt32;

    //------------------------------------------------------------------------------

    /// Hidden(ish) details
    namespace details
    {
      // Some SIMD constants
      const FPF MAXNUMF  ( 3.4028234663852885981170418348451692544e38f );
      // numbers
      const FPF HALF     ( 0.5    );               ///< 1/2
      const FPF TAN_PIO8F( 0.4142135623730950 );   ///< tan(pi/8)
      const FPF PIO4F    ( M_PI_4 );               ///< pi/4
      const FPF PIO2F    ( M_PI_2 );               ///< pi/2
      const FPF PIF      ( M_PI   );               ///< pi
      const FPF ONEOPIO4F( 4.0/M_PI );             ///< 4/pi
      // For exp
      const FPF LOG2EF   (  1.44269504088896341f ); ///< log2(e)
      const FPF C1F      (  0.693359375f       );
      const FPF C2F      ( -2.12194440e-4f     );
      const FPF C1PC2F   (  C1F + C2F          );
      const FPF PX1expf  (  1.9875691500E-4f   );
      const FPF PX2expf  (  1.3981999507E-3f   );
      const FPF PX3expf  (  8.3334519073E-3f   );
      const FPF PX4expf  (  4.1665795894E-2f   );
      const FPF PX5expf  (  1.6666665459E-1f   );
      const FPF PX6expf  (  5.0000001201E-1f   );
      const FPF MAXLOGF  (  88.72283905206835f );
      const FPF MINLOGF  ( -88.0f              );
      const FPF INFF     ( std::numeric_limits<float>::infinity() );
      // for log
      const FPF SQRTHF   (  0.707106781186547524f );
      const FPF PX1logf  (  7.0376836292E-2f );
      const FPF PX2logf  ( -1.1514610310E-1f );
      const FPF PX3logf  (  1.1676998740E-1f );
      const FPF PX4logf  ( -1.2420140846E-1f );
      const FPF PX5logf  (  1.4249322787E-1f );
      const FPF PX6logf  ( -1.6668057665E-1f );
      const FPF PX7logf  (  2.0000714765E-1f );
      const FPF PX8logf  ( -2.4999993993E-1f );
      const FPF PX9logf  (  3.3333331174E-1f );
      const FPF LOGF_UPPER_LIMIT ( MAXNUMF );
      const FPF LOGF_LOWER_LIMIT ( 0 );
      // for sin cos
      const FPF DP1F     ( 0.78515625               );
      const FPF DP2F     ( 2.4187564849853515625e-4 );
      const FPF DP3F     ( 3.77489497744594108e-8   );

      /// Converts a float to an int
      inline UInt32 float2uint32( const FPF& x ) noexcept
      {
        union { FPF f; UInt32 i; } vp = { x };
        return vp.i;
      }

      /// Converts an int to a float
      inline FPF uint322float( const UInt32& x ) noexcept
      {
        union { UInt32 i; FPF f; } vp = { x };
        return vp.f;
      }

      /// floor implementation
      inline FPF fpfloor( const FPF& x )
      {
        //return std::floor(x); // Vc implementation
        auto ret = Vc::simd_cast<Int32>(x);
        ret -= ( float2uint32(x) >> 31 );
        return Vc::simd_cast<FPF>(ret);
      }

      // Like frexp but vectorising and the exponent is a float.
      inline std::pair<FPF,FPF> getMantExponent( const FPF& x ) noexcept
      {
        union { FPF x; UInt32 n; } v = { x };

        const Int32 e = ( v.n >> 23 ) - Int32(127);

        // fractional part
        const UInt32 p05f( 0x3f000000 ); // float2uint32(0.5);
        const UInt32 AA  ( 0x807fffff ); // ~0x7f800000;
        v.n &= AA;
        v.n |= p05f;

        return { v.x, Vc::simd_cast<FPF>(e) };
      }

      /// polynominal function for fast_log implementation
      inline FPF get_log_poly( const FPF& x ) noexcept
      {
        auto y = x*PX1logf;
        y += PX2logf;
        y *= x;
        y += PX3logf;
        y *= x;
        y += PX4logf;
        y *= x;
        y += PX5logf;
        y *= x;
        y += PX6logf;
        y *= x;
        y += PX7logf;
        y *= x;
        y += PX8logf;
        y *= x;
        y += PX9logf;
        return y;
      }

      // For sincos

      /// Reduce to 0 to 45
      inline std::pair<FPF,Int32> reduce2quadrant( FPF x ) noexcept
      {
        // make argument positive
        x = abs(x);

        // integer part of x/PIO4
        auto quad = Vc::simd_cast<Int32>( ONEOPIO4F * x );

        quad = ( quad + Int32::One() ) & (~1);

        const auto y = Vc::simd_cast<FPF>(quad);

        return { ( ( x - y * DP1F ) - y * DP2F ) - y * DP3F , quad };
      }

      /// Sincos only for -45deg < x < 45deg
      inline void fast_sincosf_m45_45( const FPF& x, FPF& s, FPF& c ) noexcept
      {
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

    } // details

    //------------------------------------------------------------------------------

    /** fast sincos for Vc::Vector<float> type
     *  Based on VDT fast_sincosf */
    inline void fast_sincos( const FPF& xx, FPF& s, FPF& c )
    {
      auto xj = details::reduce2quadrant(xx);

      const Int32 signS = ( xj.second & Int32(4) );

      xj.second -= Int32(2);

      const Int32 signC = ( xj.second & Int32(4) );
      const Int32 poly  = ( xj.second & Int32(2) );

      details::fast_sincosf_m45_45( xj.first, s, c );

      // swap
      const auto swap = Vc::simd_cast<FPF::MaskType>( poly == Int32::Zero() );
      const auto tmp = c;
      c(swap) = s;
      s(swap) = tmp;

      c( Vc::simd_cast<FPF::MaskType>( signC == Int32::Zero() ) ) = -c;
      s( Vc::simd_cast<FPF::MaskType>( signS != Int32::Zero() ) ) = -s;
      s( xx < FPF::Zero()                                       ) = -s;
    }

    //------------------------------------------------------------------------------

    /** fast log for Vc::Vector<float> type
     *  Based on VDT fast_logf */
    inline FPF fast_log( const FPF& initial_x )
    {

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

    /** fast exp for Vc::Vector<float> type
     *  Based on VDT fast_expf */
    inline FPF fast_exp( const FPF& initial_x )
    {

      FPF x = initial_x;

      // floor() truncates toward -infinity.
      FPF z = details::fpfloor( details::LOG2EF * x + details::HALF );

      x -= z * details::C1PC2F;

      const auto n = Vc::simd_cast<Int32>( z );

      z  = details::PX1expf * x;
      z += details::PX2expf;
      z *= x;
      z += details::PX3expf;
      z *= x;
      z += details::PX4expf;
      z *= x;
      z += details::PX5expf;
      z *= x;
      z += details::PX6expf;
      z *= x*x;
      z += x + FPF::One();

      // multiply by power of 2
      z *= details::uint322float( ( n + Int32(0x7f) ) << 23 );

      z        ( initial_x > details::MAXLOGF ) = details::INFF;
      z.setZero( initial_x < details::MINLOGF );

      return z;
    }

    //------------------------------------------------------------------------------

    /** fast atan2 for Vc::Vector<float> type
     *  Based on VDT fast_atan2f */
    inline FPF fast_atan2( const FPF& y, const FPF& x )
    {

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
    inline void fast_sincos( const float  x, float&  s, float&  c ) { vdt::fast_sincosf(x,s,c); }
    /// Fast sin and cos
    inline void fast_sincos( const double x, double& s, double& c ) { vdt::fast_sincos(x,s,c);  }

    /// Fast log
    inline float  fast_log( const float  x ) { return vdt::fast_logf(x); }
    /// Fast log
    inline double fast_log( const double x ) { return vdt::fast_log(x); }

    /// Fast exp
    inline float  fast_exp( const float  x ) { return vdt::fast_expf(x); }
    /// Fast exp
    inline double fast_exp( const double x ) { return vdt::fast_exp(x); }

    /// Fast atan2
    inline float  fast_atan2( const float  y, const float  x ) { return vdt::fast_atan2f(y,x); }
    /// Fast atan2
    inline double fast_atan2( const double y, const double x ) { return vdt::fast_atan2(y,x); }

    //------------------------------------------------------------------------------

  }
}
