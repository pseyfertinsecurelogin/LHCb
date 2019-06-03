
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

//-----------------------------------------------------------------------------
/** @file FastMaths-Core.h
 *
 * Core implementations of various fast mathematical functions.
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date   16/05/2019
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>

// VDT
#include "vdt/asin.h"
#include "vdt/atan2.h"
#include "vdt/exp.h"
#include "vdt/log.h"
#include "vdt/sincos.h"
#include "vdt/sqrt.h"
#include "vdt/tan.h"

// Local
#include "LHCbMath/SIMDTypes.h"

namespace LHCb::Math {

  namespace impl {

    /// Selects the correct unsigned int type based on floating point type...
    template <typename FP>
    using UInt32 = std::conditional_t<
        std::is_same<FP, LHCb::SIMD::VC::FPF>::value,
        LHCb::SIMD::VC::UInt32,                                                                       // VC
        std::conditional_t<std::is_same<FP, LHCb::SIMD::VE::FPF>::value, LHCb::SIMD::VE::UInt32,      // VE
                           std::conditional_t<std::is_same<FP, float>::value, std::uint32_t, void>>>; // float

    /// Selects the correct signed int type based on floating point type...
    template <typename FP>
    using Int32 = std::conditional_t<
        std::is_same<FP, LHCb::SIMD::VC::FPF>::value,
        LHCb::SIMD::VC::Int32,                                                                       // VC
        std::conditional_t<std::is_same<FP, LHCb::SIMD::VE::FPF>::value, LHCb::SIMD::VE::Int32,      // VE
                           std::conditional_t<std::is_same<FP, float>::value, std::int32_t, void>>>; // float

    /// Get the sign mask
    template <typename FP>
    inline UInt32<FP> getSignMask( const FP x ) noexcept {
      union {
        FP         y;
        UInt32<FP> i;
      } z = {x};
      return z.i & UInt32<FP>( 0x80000000 );
    }

    /// Makes an OR of a float and a unsigned long
    template <typename FP>
    inline FP spORuint32( const FP x, const UInt32<FP> i ) noexcept {
      union {
        FP         y;
        UInt32<FP> i;
      } z = {x};
      z.i |= i;
      return z.y;
    }

    /// Makes an XOR of a float and a unsigned long
    template <typename FP>
    inline FP spXORuint32( const FP x, const UInt32<FP> i ) noexcept {
      union {
        FP         y;
        UInt32<FP> i;
      } z = {x};
      z.i ^= i;
      return z.y;
    }

    /// Reduce to 0 to 45
    template <typename FP>
    inline std::pair<FP, Int32<FP>> reduce2quadrant( FP x ) noexcept {

      using namespace LHCb::SIMD;

      // constants
      const FP ONEOPIO4F( 4.0f / float( M_PI ) ); ///< 4/pi
      const FP DP1F( 0.78515625f );
      const FP DP2F( 2.4187564849853515625e-4f );
      const FP DP3F( 3.77489497744594108e-8f );

      // make argument positive
      x = abs( x );

      // integer part of x/PIO4
      auto quad = lb_cast<Int32<FP>>( ONEOPIO4F * x );

      quad = lb_cast<Int32<FP>>( ( quad + Int32<FP>::One() ) & ( ~1 ) );

      const auto y = lb_cast<FP>( quad );

      return {( ( x - y * DP1F ) - y * DP2F ) - y * DP3F, quad};
    }

    /// Sincos only for -45deg < x < 45deg
    template <typename FP>
    inline void fast_sincosf_m45_45( const FP x, FP& s, FP& c ) noexcept {

      // sin constants
      const FP sa( -1.9515295891E-4f );
      const FP sb( 8.3321608736E-3f );
      const FP sc( -1.6666654611E-1f );

      // cos constants
      const FP ca( 2.443315711809948E-005f );
      const FP cb( -1.388731625493765E-003f );
      const FP cc( 4.166664568298827E-002f );

      auto z = x * x;

      s = ( ( ( sa * z + sb ) * z + sc ) * z * x ) + x;
      c = ( ( ca * z + cb ) * z + cc ) * z * z - FP( 0.5f ) * z + FP( 1.0f );
    }

    // Like frexp but vectorising and the exponent is a float.
    template <typename FP>
    inline std::pair<FP, FP> getMantExponent( const FP x ) noexcept {

      using namespace LHCb::SIMD;

      union {
        FP         x;
        UInt32<FP> n;
      } v = {x};

      const auto e = lb_cast<Int32<FP>>( v.n >> 23 ) - Int32<FP>( 127 );

      // fractional part
      const UInt32<FP> p05f( 0x3f000000 );
      const UInt32<FP> AA( 0x807fffff ); // ~0x7f800000;
      v.n &= AA;
      v.n |= p05f;

      return {v.x, lb_cast<FP>( e )};
    }

    /// polynominal function for fast_log implementation
    template <typename FP>
    inline FP get_log_poly( const FP x ) noexcept {

      // constants...
      const FP PX1logf( 7.0376836292E-2f );
      const FP PX2logf( -1.1514610310E-1f );
      const FP PX3logf( 1.1676998740E-1f );
      const FP PX4logf( -1.2420140846E-1f );
      const FP PX5logf( 1.4249322787E-1f );
      const FP PX6logf( -1.6668057665E-1f );
      const FP PX7logf( 2.0000714765E-1f );
      const FP PX8logf( -2.4999993993E-1f );
      const FP PX9logf( 3.3333331174E-1f );

      auto y = x * PX1logf;
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

    /// Converts a float to an int
    template <typename FP>
    inline UInt32<FP> float2uint32( const FP x ) noexcept {
      const union {
        FP         f;
        UInt32<FP> i;
      } vp = {x};
      return vp.i;
    }

    /// floor implementation
    template <typename FP>
    inline FP fpfloor( const FP x ) noexcept {
      using namespace LHCb::SIMD;
      auto       ret = simd_cast<Int32<FP>>( x );
      const auto m   = ( float2uint32( x ) >> 31 );
      ret -= lb_cast<Int32<FP>>( m );
      return lb_cast<FP>( ret );
    }

    // constants.
    constexpr float INFF( std::numeric_limits<float>::infinity() );
    constexpr float MAXNUMF( 3.4028234663852885981170418348451692544e38f );
    constexpr float PIO2F( M_PI_2 );                     ///< pi/2
    constexpr float LOG2EF( 1.44269504088896341f );      ///< log2(e)
    constexpr float THREEPIO4( 3.0f * float( M_PI_4 ) ); ///< 3*pi/4
    constexpr float PIO4F( M_PI_4 );                     ///< pi/4

  } // namespace impl

  //------------------------------------------------------------------------------

  /// fast asin
  template <typename FP>
  inline FP fast_asin( FP x ) noexcept {
    // shortcuts to scalar VDT versions.
    if constexpr ( std::is_same<FP, float>::value ) {
      return vdt::fast_asinf( x );
    } else if constexpr ( std::is_same<FP, double>::value ) {
      return vdt::fast_asin( x );
    }
    // SIMD version
    else if constexpr ( !std::is_arithmetic<FP>::value ) {
      using namespace impl;

      const auto sign_mask = getSignMask( x );
      const auto abs_x     = abs( x );

      const auto m = ( abs_x > FP( 0.5f ) );

      x      = abs_x;
      auto z = x * x;
      if ( any_of( m ) ) {
        z( m ) = FP( 0.5f ) * ( FP( 1 ) - abs_x );
        x( m ) = std::sqrt( z );
      }

      const FP a( 4.2163199048E-2f );
      const FP b( 2.4181311049E-2f );
      const FP c( 4.5470025998E-2f );
      const FP d( 7.4953002686E-2f );
      const FP e( 1.6666752422E-1f );

      z = ( ( ( ( a * z + b ) * z + c ) * z + d ) * z + e ) * z * x + x;

      z( m ) = FP( PIO2F ) - ( FP( 2 ) * z );

      return spORuint32( z, sign_mask );
    }
  }

  //------------------------------------------------------------------------------

  /// fast acos
  template <typename FP>
  inline FP fast_acos( FP x ) noexcept {
    // shortcuts to scalar VDT versions.
    if constexpr ( std::is_same<FP, float>::value ) {
      return vdt::fast_acosf( x );
    } else if constexpr ( std::is_same<FP, double>::value ) {
      return vdt::fast_acos( x );
    }
    // SIMD version
    else if constexpr ( !std::is_arithmetic<FP>::value ) {
      return FP( impl::PIO2F ) - fast_asin( x );
    }
  }

  //------------------------------------------------------------------------------

  /// fast sincos
  template <typename FP>
  inline void fast_sincos( const FP xx, FP& s, FP& c ) noexcept {
    // shortcuts to scalar VDT versions.
    if constexpr ( std::is_same<FP, float>::value ) {
      vdt::fast_sincosf( xx, s, c );
    } else if constexpr ( std::is_same<FP, double>::value ) {
      vdt::fast_sincos( xx, s, c );
    }
    // SIMD version
    else if constexpr ( !std::is_arithmetic<FP>::value ) {

      using namespace impl;

      using mtype = typename FP::mask_type;

      auto xj = reduce2quadrant( xx );

      const auto signS = simd_cast<Int32<FP>>( xj.second & Int32<FP>( 4 ) );

      xj.second -= Int32<FP>( 2 );

      const auto signC = simd_cast<Int32<FP>>( xj.second & Int32<FP>( 4 ) );
      const auto poly  = simd_cast<Int32<FP>>( xj.second & Int32<FP>( 2 ) );

      fast_sincosf_m45_45( xj.first, s, c );

      // swap
      const auto swap = simd_cast<mtype>( poly == Int32<FP>::Zero() );
      if ( UNLIKELY( any_of( swap ) ) ) {
        const auto tmp = c;
        c( swap )      = s;
        s( swap )      = tmp;
      }

      c( simd_cast<mtype>( signC == Int32<FP>::Zero() ) ) = -c;
      s( simd_cast<mtype>( signS != Int32<FP>::Zero() ) ) = -s;
      s( xx < FP::Zero() )                                = -s;
    }
  }

  //------------------------------------------------------------------------------

  /// fast log
  template <typename FP>
  inline FP fast_log( const FP initial_x ) noexcept {

    // shortcuts to scalar VDT versions.
    if constexpr ( std::is_same<FP, float>::value ) {
      return vdt::fast_logf( initial_x );
    } else if constexpr ( std::is_same<FP, double>::value ) {
      return vdt::fast_log( initial_x );
    }
    // SIMD version
    else if constexpr ( !std::is_arithmetic<FP>::value ) {

      using namespace impl;

      auto x_fe = getMantExponent( initial_x );

      auto& x  = x_fe.first;
      auto& fe = x_fe.second;

      const FP   SQRTHF( 0.707106781186547524f );
      const auto m = x > SQRTHF;
      fe( m ) += FP::One();
      x( !m ) += x;
      x -= FP::One();

      const auto x2 = x * x;

      auto res = get_log_poly( x );
      res *= x2 * x;

      res += FP( -2.12194440e-4f ) * fe;
      res -= FP( 0.5f ) * x2;

      res = x + res;

      res += FP( 0.693359375f ) * fe;

      const FP LOGF_UPPER_LIMIT( MAXNUMF );
      const FP LOGF_LOWER_LIMIT( 0 );
      res( initial_x > LOGF_UPPER_LIMIT ) = FP( INFF );
      res.setQnan( initial_x < LOGF_LOWER_LIMIT );

      return res;
    }
  }

  //------------------------------------------------------------------------------

  /// fast exp
  template <typename FP>
  inline FP fast_exp( const FP initial_x ) noexcept {

    // shortcuts to scalar VDT versions.
    if constexpr ( std::is_same<FP, float>::value ) {
      return vdt::fast_expf( initial_x );
    } else if constexpr ( std::is_same<FP, double>::value ) {
      return vdt::fast_exp( initial_x );
    }
    // SIMD version
    else if constexpr ( !std::is_arithmetic<FP>::value ) {

      using namespace impl;

      FP x = initial_x;

      // floor() truncates toward -infinity.
      FP z = fpfloor( FP( LOG2EF ) * x + FP( 0.5f ) );

      const FP C1F( 0.693359375f );
      const FP C2F( -2.12194440e-4f );
      const FP C1PC2F( C1F + C2F );
      const FP PX1expf( 1.9875691500E-4f );
      const FP PX2expf( 1.3981999507E-3f );
      const FP PX3expf( 8.3334519073E-3f );
      const FP PX4expf( 4.1665795894E-2f );
      const FP PX5expf( 1.6666665459E-1f );
      const FP PX6expf( 5.0000001201E-1f );

      x -= z * C1PC2F;

      const auto n = simd_cast<Int32<FP>>( z );

      z = PX1expf * x;
      z += PX2expf;
      z *= x;
      z += PX3expf;
      z *= x;
      z += PX4expf;
      z *= x;
      z += PX5expf;
      z *= x;
      z += PX6expf;
      z *= x * x;
      z += x + FP::One();

      // multiply by power of 2
      const union {
        UInt32<FP> i;
        FP         f;
      } vp = {( ( n + Int32<FP>( 0x7f ) ) << 23 )};
      z *= vp.f;
      const FP MAXLOGF( 88.72283905206835f );
      const FP MINLOGF( -88.0f );
      z( initial_x > MAXLOGF ) = FP( INFF );
      z.setZero( initial_x < MINLOGF );

      return z;
    }
  }

  //------------------------------------------------------------------------------

  /// fast tan
  template <typename FP>
  inline FP fast_tan( const FP x ) noexcept {

    // shortcuts to scalar VDT versions.
    if constexpr ( std::is_same<FP, float>::value ) {
      return vdt::fast_tanf( x );
    } else if constexpr ( std::is_same<FP, double>::value ) {
      return vdt::fast_tan( x );
    }
    // SIMD version
    else if constexpr ( !std::is_arithmetic<FP>::value ) {

      using namespace impl;

      auto z = reduce2quadrant( x );

      const auto zz = z.first * z.first;

      const FP a( 9.38540185543E-3f );
      const FP b( 3.11992232697E-3f );
      const FP c( 2.44301354525E-2f );
      const FP d( 5.34112807005E-2f );
      const FP e( 1.33387994085E-1f );
      const FP f( 3.33331568548E-1f );

      FP res = ( ( ( ( ( ( a * zz + b ) * zz + c ) * zz + d ) * zz + e ) * zz + f ) * zz * z.first + z.first );
      res( zz < FP( 1.0e-14f ) ) = z.first;

      // A no branching way to say: if j&2 res = -1/res. You can!!!
      z.second &= 2;
      z.second >>= 1;
      const auto alt = simd_cast<FP>( ( z.second ) ^ UInt32<FP>::One() );

      const auto m = x == FP::Zero();

      res( m ) += FP::One();

      // one coeff is one and one is 0!
      res = simd_cast<FP>( z.second ) * ( -FP::One() / res ) + alt * res;

      const auto sign_mask = getSignMask( x );
      auto       ret       = spXORuint32( res, sign_mask );
      ret.setZero( m );

      return ret;
    }
  }

  //------------------------------------------------------------------------------

  /// fast atan2
  template <typename FP>
  inline FP fast_atan2( const FP y, const FP x ) noexcept {

    // shortcuts to scalar VDT versions.
    if constexpr ( std::is_same<FP, float>::value ) {
      return vdt::fast_atan2f( y, x );
    } else if constexpr ( std::is_same<FP, double>::value ) {
      return vdt::fast_atan2( y, x );
    }
    // SIMD version
    else if constexpr ( !std::is_arithmetic<FP>::value ) {

      using namespace impl;

      const FP TAN_PIO8F( 0.4142135623730950f ); ///< tan(pi/8)
      const FP PIF( float( M_PI ) );             ///< pi

      // move in first octant
      auto       xx = abs( x );
      auto       yy = abs( y );
      const auto m1 = ( yy > xx );
      if ( any_of( m1 ) ) {
        const auto tmp = yy;
        yy( m1 )       = xx;
        xx( m1 )       = tmp;
      }

      const auto t  = yy / xx;
      auto       z  = t;
      const auto m2 = t > TAN_PIO8F;
      if ( any_of( m2 ) ) { z( m2 ) = ( t - FP::One() ) / ( t + FP::One() ); }

      const auto z2 = z * z;

      const FP a( 8.05374449538e-2f );
      const FP b( -1.38776856032E-1f );
      const FP c( 1.99777106478E-1f );
      const FP d( -3.33329491539E-1f );

      auto ret = ( ( ( ( a * z2 + b ) * z2 + c ) * z2 + d ) * z2 * z + z );

      // move back in place
      const auto xZero = xx == FP::Zero();
      const auto yZero = y == FP::Zero();
      ret.setZero( xZero || yZero );
      ret( m2 ) += FP( PIO4F );
      ret( m1 )             = PIO2F - ret;
      ret( x < FP::Zero() ) = PIF - ret;
      ret( y < FP::Zero() ) = -ret;

      return ret;
    }
  }

  //------------------------------------------------------------------------------

  //------------------------------------------------------------------------------
  namespace Approx {

    //----------------------------------------------------------------------------

    /// Fast log2 approximation
    template <typename FP>
    inline FP approx_log2( const FP x ) noexcept {
      using namespace impl;
      using namespace LHCb::SIMD;
      const union {
        FP         f;
        UInt32<FP> i;
      } vx = {x};
      const union {
        UInt32<FP> i;
        FP         f;
      } mx         = {( vx.i & UInt32<FP>( 0x007FFFFF ) ) | UInt32<FP>( 0x3f000000 )};
      const auto y = lb_cast<FP>( vx.i ) * FP( 1.1920928955078125e-7f );
      return ( y - FP( 124.22551499f ) - FP( 1.498030302f ) * mx.f -
               FP( 1.72587999f ) / ( FP( 0.3520887068f ) + mx.f ) );
    }

    /// Fast log approximation
    template <typename FP>
    inline FP approx_log( const FP x ) noexcept {
      return FP( 0.69314718f ) * approx_log2( x );
    }

    /// Very fast log approximation
    template <typename FP>
    inline FP vapprox_log( const FP x ) noexcept {
      using namespace impl;
      using namespace LHCb::SIMD;
      const union {
        FP         f;
        UInt32<FP> i;
      } vx = {x};
      return ( lb_cast<FP>( vx.i ) * FP( 8.2629582881927490e-8f ) ) - FP( 87.989971088f );
    }

    /// Fast pow2 approximation
    template <typename FP>
    inline FP approx_pow2( const FP p ) noexcept {
      if constexpr ( std::is_arithmetic<FP>::value ) {
        // scalar
        const float        offset = ( p < 0 ? 1.0f : 0.0f );
        const float        clipp  = ( p < -126 ? -126.0f : p );
        const std::int32_t w      = clipp;
        const float        z      = clipp - w + offset;
        const union {
          std::uint32_t i;
          float         f;
        } v = {static_cast<std::uint32_t>(
            ( 1 << 23 ) * ( clipp + 121.2740575f + 27.7280233f / ( 4.84252568f - z ) - 1.49012907f * z ) )};
        return v.f;
      } else {
        // SIMD
        using namespace impl;
        using namespace LHCb::SIMD;
        const FP A( -126.0f );
        auto     clipp = p;
        clipp( p < A ) = A;
        const auto w   = lb_cast<Int32<FP>>( clipp );
        auto       z   = clipp - lb_cast<FP>( w );
        z( p < FP::Zero() ) += FP::One();
        const union {
          UInt32<FP> i;
          FP         f;
        } v = {lb_cast<UInt32<FP>>(
            ( 1 << 23 ) *
            ( clipp + FP( 121.2740575f ) + FP( 27.7280233f ) / ( FP( 4.84252568f ) - z ) - FP( 1.49012907f ) * z ) )};
        return v.f;
      }
    }

    /// Fast exp approximation
    template <typename FP>
    inline FP approx_exp( const FP p ) noexcept {
      return approx_pow2( FP( impl::LOG2EF ) * p );
    }

    /// Very fast pow2 approximation
    template <typename FP>
    inline FP vapprox_pow2( const FP p ) noexcept {
      using namespace impl;
      if constexpr ( std::is_arithmetic<FP>::value ) {
        // scalar
        const float clipp = ( p < -126 ? -126.0f : p );
        const union {
          UInt32<FP> i;
          FP         f;
        } v = {static_cast<UInt32<FP>>( ( 1 << 23 ) * ( clipp + 126.94269504f ) )};
        return v.f;
      } else {
        // SIMD
        using namespace LHCb::SIMD;
        auto     clipp = p;
        const FP A( -126.0f );
        clipp( p < A ) = A;
        const union {
          UInt32<FP> i;
          FP         f;
        } v = {simd_cast<UInt32<FP>>( ( 1 << 23 ) * ( clipp + FP( 126.94269504f ) ) )};
        return v.f;
      }
    }

    /// Very fast exp approximation
    template <typename FP>
    inline FP vapprox_exp( const FP p ) noexcept {
      return vapprox_pow2( FP( impl::LOG2EF ) * p );
    }

    /// very fast approximate atan2
    template <typename FP>
    inline FP vapprox_atan2( const FP y, const FP x ) {
      using namespace impl;
      if constexpr ( std::is_arithmetic<FP>::value ) {
        // scalar
        const auto abs_y = std::fabs( y ) + 1e-10f;
        const auto neg_x = ( x < 0.0f );
        const auto r     = ( neg_x ? ( x + abs_y ) / ( abs_y - x ) : ( x - abs_y ) / ( x + abs_y ) );
        const auto angle = ( ( neg_x ? THREEPIO4 : PIO4F ) + ( ( 0.1963f * r * r - 0.9817f ) * r ) );
        // opposite sign if in quad III or IV
        return ( y < 0.0f ? -angle : angle );
      } else {
        const auto abs_y = abs( y ) + FP( 1e-10f );
        const auto neg_x = ( x < FP::Zero() );
        auto       r     = ( x - abs_y ) / ( x + abs_y );
        r( neg_x )       = -FP::One() / r;
        auto angle       = ( ( FP( 0.1963f ) * r * r - FP( 0.9817f ) ) * r );
        angle( neg_x ) += FP( THREEPIO4 );
        angle( !neg_x ) += FP( PIO4F );
        // opposite sign if in quad III or IV
        angle( y < FP::Zero() ) *= -FP::One();
        // return
        return angle;
      }
    }

    /// Approx sqrt
    template <typename FP>
    inline FP approx_sqrt( const FP x ) noexcept {

      using namespace impl;

      union {
        FP        x;
        Int32<FP> i;
      } u = {x};

      u.i = ( 1 << 29 ) + ( u.i >> 1 ) - ( 1 << 22 );

      // One Babylonian Step
      u.x = FP( 0.5f ) * ( u.x + ( x / u.x ) );

      return u.x;
    }

    /// Approx sqrt
    template <typename FP>
    inline FP vapprox_sqrt( const FP x ) noexcept {

      using namespace impl;

      union {
        FP        x;
        Int32<FP> i;
      } u = {x};

      u.i = ( 1 << 29 ) + ( u.i >> 1 ) - ( 1 << 22 );

      return u.x;
    }

    //----------------------------------------------------------------------------

  } // namespace Approx

  //------------------------------------------------------------------------------

} // namespace LHCb::Math
