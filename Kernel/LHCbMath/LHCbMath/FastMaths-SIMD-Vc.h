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
/** @file FastMaths-SIMD-Vc.h
 *
 * Vc SIMD mplementation of various fast mathematical functions.
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date   30/11/2017
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <cmath>
#include <cstdint>
#include <limits>
#include <utility>

// Local
#include "LHCbMath/SIMDTypes.h"

namespace LHCb::Math {

  //------------------------------------------------------------------------------

  /// Hidden(ish) details
  namespace details::VC {

    // import some types
    using namespace LHCb::SIMD::VC;

    // Some SIMD constants

    static const FPF MAXNUMF( 3.4028234663852885981170418348451692544e38f );
    static const FPF HALF( 0.5 );                         ///< 1/2
    static const FPF TWO( 2.0 );                          ///< 2
    static const FPF THREEHALVES( 1.5 );                  ///< 3/2
    static const FPF TAN_PIO8F( 0.4142135623730950 );     ///< tan(pi/8)
    static const FPF PIO4F( M_PI_4 );                     ///< pi/4
    static const FPF PIO2F( M_PI_2 );                     ///< pi/2
    static const FPF PIF( M_PI );                         ///< pi
    static const FPF ONEOPIO4F( 4.0 / M_PI );             ///< 4/pi
    static const FPF THREEPIO4( 3.0f * float( M_PI_4 ) ); ///< 3*pi/4
    // For exp
    static const FPF LOG2EF( 1.44269504088896341f ); ///< log2(e)
    static const FPF C1F( 0.693359375f );
    static const FPF C2F( -2.12194440e-4f );
    static const FPF C1PC2F( C1F + C2F );
    static const FPF PX1expf( 1.9875691500E-4f );
    static const FPF PX2expf( 1.3981999507E-3f );
    static const FPF PX3expf( 8.3334519073E-3f );
    static const FPF PX4expf( 4.1665795894E-2f );
    static const FPF PX5expf( 1.6666665459E-1f );
    static const FPF PX6expf( 5.0000001201E-1f );
    static const FPF MAXLOGF( 88.72283905206835f );
    static const FPF MINLOGF( -88.0f );
    static const FPF INFF( std::numeric_limits<float>::infinity() );
    // for log
    static const FPF SQRTHF( 0.707106781186547524f );
    static const FPF PX1logf( 7.0376836292E-2f );
    static const FPF PX2logf( -1.1514610310E-1f );
    static const FPF PX3logf( 1.1676998740E-1f );
    static const FPF PX4logf( -1.2420140846E-1f );
    static const FPF PX5logf( 1.4249322787E-1f );
    static const FPF PX6logf( -1.6668057665E-1f );
    static const FPF PX7logf( 2.0000714765E-1f );
    static const FPF PX8logf( -2.4999993993E-1f );
    static const FPF PX9logf( 3.3333331174E-1f );
    static const FPF LOGF_UPPER_LIMIT( MAXNUMF );
    static const FPF LOGF_LOWER_LIMIT( 0 );
    // for sin cos tan
    static const FPF DP1F( 0.78515625 );
    static const FPF DP2F( 2.4187564849853515625e-4 );
    static const FPF DP3F( 3.77489497744594108e-8 );

    /// Converts a float to an int
    inline UInt32 float2uint32( const FPF x ) noexcept {
      const union {
        FPF    f;
        UInt32 i;
      } vp = {x};
      return vp.i;
    }

    /// Converts an int to a float
    inline FPF uint322float( const UInt32 x ) noexcept {
      const union {
        UInt32 i;
        FPF    f;
      } vp = {x};
      return vp.f;
    }

    /// floor implementation
    inline FPF fpfloor( const FPF x ) noexcept {
      // return std::floor(x); // Vc implementation
      auto ret = simd_cast<Int32>( x );
      ret -= ( float2uint32( x ) >> 31 );
      return simd_cast<FPF>( ret );
    }

    // Like frexp but vectorising and the exponent is a float.
    inline std::pair<FPF, FPF> getMantExponent( const FPF x ) noexcept {
      union {
        FPF    x;
        UInt32 n;
      } v = {x};

      const Int32 e = ( v.n >> 23 ) - Int32( 127 );

      // fractional part
      const UInt32 p05f( 0x3f000000 ); // float2uint32(0.5);
      const UInt32 AA( 0x807fffff );   // ~0x7f800000;
      v.n &= AA;
      v.n |= p05f;

      return {v.x, simd_cast<FPF>( e )};
    }

    /// polynominal function for fast_log implementation
    inline FPF get_log_poly( const FPF x ) noexcept {
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

    // For sincos

    /// Reduce to 0 to 45
    inline std::pair<FPF, Int32> reduce2quadrant( FPF x ) noexcept {

      // make argument positive
      x = abs( x );

      // integer part of x/PIO4
      auto quad = simd_cast<Int32>( ONEOPIO4F * x );

      quad = ( quad + Int32::One() ) & ( ~1 );

      const auto y = simd_cast<FPF>( quad );

      return {( ( x - y * DP1F ) - y * DP2F ) - y * DP3F, quad};
    }

    /// Sincos only for -45deg < x < 45deg
    inline void fast_sincosf_m45_45( const FPF x, FPF& s, FPF& c ) noexcept {

      // sin constants
      static const FPF sa( -1.9515295891E-4f );
      static const FPF sb( 8.3321608736E-3f );
      static const FPF sc( -1.6666654611E-1f );

      // cos constants
      static const FPF ca( 2.443315711809948E-005f );
      static const FPF cb( -1.388731625493765E-003f );
      static const FPF cc( 4.166664568298827E-002f );

      auto z = x * x;

      s = ( ( ( sa * z + sb ) * z + sc ) * z * x ) + x;
      c = ( ( ca * z + cb ) * z + cc ) * z * z - HALF * z + FPF::One();
    }

    /// Get the sign mask
    inline UInt32 getSignMask( const FPF x ) noexcept {
      union {
        FPF    y;
        UInt32 i;
      } z = {x};
      return z.i & UInt32( 0x80000000 );
    }

    /// Makes an XOR of a float and a unsigned long
    inline FPF spXORuint32( const FPF x, const UInt32 i ) {
      union {
        FPF    y;
        UInt32 i;
      } z = {x};
      z.i ^= i;
      return z.y;
    }

    /// Makes an OR of a float and a unsigned long
    inline FPF spORuint32( const FPF x, const UInt32 i ) {
      union {
        FPF    y;
        UInt32 i;
      } z = {x};
      z.i |= i;
      return z.y;
    }

  } // namespace details::VC

  //------------------------------------------------------------------------------

  /** fast asin for SIMD float type
   *  Based on VDT fast_asinf */
  inline LHCb::SIMD::VC::FPF fast_asin( LHCb::SIMD::VC::FPF x ) noexcept {
    using namespace details::VC;

    const auto sign_mask = getSignMask( x );
    const auto abs_x     = abs( x );

    const auto m = ( abs_x > HALF );

    x      = abs_x;
    auto z = x * x;
    if ( any_of( m ) ) {
      z( m ) = HALF * ( FPF::One() - abs_x );
      x( m ) = std::sqrt( z );
    }

    static const FPF a( 4.2163199048E-2f );
    static const FPF b( 2.4181311049E-2f );
    static const FPF c( 4.5470025998E-2f );
    static const FPF d( 7.4953002686E-2f );
    static const FPF e( 1.6666752422E-1f );

    z = ( ( ( ( a * z + b ) * z + c ) * z + d ) * z + e ) * z * x + x;

    z( m ) = PIO2F - ( TWO * z );

    return spORuint32( z, sign_mask );
  }

  //------------------------------------------------------------------------------

  /** fast acos for SIMD float type
   *  Based on VDT fast_acosf */
  inline LHCb::SIMD::VC::FPF fast_acos( const LHCb::SIMD::VC::FPF x ) noexcept {
    return details::VC::PIO2F - fast_asin( x );
  }

  //------------------------------------------------------------------------------

  /** fast sincos for SIMD float type
   *  Based on VDT fast_sincosf */
  inline void fast_sincos( const LHCb::SIMD::VC::FPF xx, LHCb::SIMD::VC::FPF& s, LHCb::SIMD::VC::FPF& c ) noexcept {
    using namespace details::VC;

    auto xj = reduce2quadrant( xx );

    const Int32 signS = ( xj.second & Int32( 4 ) );

    xj.second -= Int32( 2 );

    const Int32 signC = ( xj.second & Int32( 4 ) );
    const Int32 poly  = ( xj.second & Int32( 2 ) );

    fast_sincosf_m45_45( xj.first, s, c );

    // swap
    const auto swap = simd_cast<FPF::MaskType>( poly == Int32::Zero() );
    const auto tmp  = c;
    c( swap )       = s;
    s( swap )       = tmp;

    c( simd_cast<FPF::MaskType>( signC == Int32::Zero() ) ) = -c;
    s( simd_cast<FPF::MaskType>( signS != Int32::Zero() ) ) = -s;
    s( xx < FPF::Zero() )                                   = -s;
  }

  //------------------------------------------------------------------------------

  /** fast log for SIMD float type
   *  Based on VDT fast_logf */
  inline LHCb::SIMD::VC::FPF fast_log( const LHCb::SIMD::VC::FPF initial_x ) noexcept {
    using namespace details::VC;

    auto x_fe = getMantExponent( initial_x );

    auto& x  = x_fe.first;
    auto& fe = x_fe.second;

    const auto m = x > SQRTHF;
    fe( m ) += FPF::One();
    x( !m ) += x;
    x -= FPF::One();

    const auto x2 = x * x;

    auto res = get_log_poly( x );
    res *= x2 * x;

    res += FPF( -2.12194440e-4f ) * fe;
    res -= HALF * x2;

    res = x + res;

    res += FPF( 0.693359375f ) * fe;

    res( initial_x > LOGF_UPPER_LIMIT ) = INFF;
    res.setQnan( initial_x < LOGF_LOWER_LIMIT );

    return res;
  }

  //------------------------------------------------------------------------------

  /** fast exp for SIMD float type
   *  Based on VDT fast_expf */
  inline LHCb::SIMD::VC::FPF fast_exp( const LHCb::SIMD::VC::FPF initial_x ) noexcept {
    using namespace details::VC;

    FPF x = initial_x;

    // floor() truncates toward -infinity.
    FPF z = fpfloor( LOG2EF * x + HALF );

    x -= z * C1PC2F;

    const auto n = LHCb::SIMD::VC::simd_cast<Int32>( z );

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
    z += x + FPF::One();

    // multiply by power of 2
    z *= uint322float( ( n + Int32( 0x7f ) ) << 23 );

    z( initial_x > MAXLOGF ) = INFF;
    z.setZero( initial_x < MINLOGF );

    return z;
  }

  //------------------------------------------------------------------------------

  /** fast atan2 for SIMD float type
   *  Based on VDT fast_atan2f */
  inline LHCb::SIMD::VC::FPF fast_atan2( const LHCb::SIMD::VC::FPF y, const LHCb::SIMD::VC::FPF x ) noexcept {
    using namespace details::VC;

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
    if ( any_of( m2 ) ) { z( m2 ) = ( t - FPF::One() ) / ( t + FPF::One() ); }

    const auto z2 = z * z;

    static const FPF a( 8.05374449538e-2f );
    static const FPF b( -1.38776856032E-1f );
    static const FPF c( 1.99777106478E-1f );
    static const FPF d( -3.33329491539E-1f );

    auto ret = ( ( ( ( a * z2 + b ) * z2 + c ) * z2 + d ) * z2 * z + z );

    // move back in place
    ret.setZero( xx == FPF::Zero() || y == FPF::Zero() );
    ret( m2 ) += PIO4F;
    ret( m1 )              = PIO2F - ret;
    ret( x < FPF::Zero() ) = PIF - ret;
    ret( y < FPF::Zero() ) = -ret;

    return ret;
  }

  //------------------------------------------------------------------------------

  /** fast tan for SIMD float type
   *  Based on VDT fast_tanf */
  inline LHCb::SIMD::VC::FPF fast_tan( const LHCb::SIMD::VC::FPF x ) noexcept {
    using namespace details::VC;

    auto z = reduce2quadrant( x );

    const auto zz = z.first * z.first;

    static const FPF a( 9.38540185543E-3f );
    static const FPF b( 3.11992232697E-3f );
    static const FPF c( 2.44301354525E-2f );
    static const FPF d( 5.34112807005E-2f );
    static const FPF e( 1.33387994085E-1f );
    static const FPF f( 3.33331568548E-1f );

    FPF res = ( ( ( ( ( ( a * zz + b ) * zz + c ) * zz + d ) * zz + e ) * zz + f ) * zz * z.first + z.first );
    res( zz < FPF( 1.0e-14f ) ) = z.first;

    // A no branching way to say: if j&2 res = -1/res. You can!!!
    z.second &= 2;
    z.second >>= 1;
    const auto alt = simd_cast<FPF>( ( z.second ) ^ UInt32::One() );

    const auto m = x == FPF::Zero();

    res( m ) += FPF::One();

    // one coeff is one and one is 0!
    res = simd_cast<FPF>( z.second ) * ( -FPF::One() / res ) + alt * res;

    const auto sign_mask = getSignMask( x );
    auto       ret       = spXORuint32( res, sign_mask );
    ret.setZero( m );

    return ret;
  }

  //------------------------------------------------------------------------------

  /** fast inverse sqrt for SIMD float type
   *  Based on VDT fast_isqrtf */
  //[[deprecated("superseded by LHCb::Math::rsqrt<faster>")]]
  // inline LHCb::SIMD::VC::FPF fast_isqrt( const LHCb::SIMD::VC::FPF& x ) noexcept
  //{
  //  // use two iterations
  //  return details::fast_isqrtf_general<2>(x);
  //}

  /** fast approximate inverse sqrt for SIMD float type
   *  Based on VDT fast_approx_isqrtf */
  //[[deprecated("superseded by LHCb::Math::rsqrt<fastest>")]]
  // inline LHCb::SIMD::VC::FPF fast_approx_isqrt( const LHCb::SIMD::VC::FPF& x ) noexcept
  //{
  //  // use only 1 iteration
  //  return details::fast_isqrtf_general<1>(x);
  //}

  //------------------------------------------------------------------------------

  namespace Approx {

    //------------------------------------------------------------------------------

    // SIMD versions

    /// Fast log2 approximation
    inline LHCb::SIMD::VC::FPF approx_log2( const LHCb::SIMD::VC::FPF x ) noexcept {
      using namespace details::VC;
      const union {
        FPF    f;
        UInt32 i;
      } vx = {x};
      const union {
        UInt32 i;
        FPF    f;
      } mx         = {( vx.i & UInt32( 0x007FFFFF ) ) | UInt32( 0x3f000000 )};
      const auto y = simd_cast<FPF>( vx.i ) * FPF( 1.1920928955078125e-7f );
      return ( y - FPF( 124.22551499f ) - FPF( 1.498030302f ) * mx.f -
               FPF( 1.72587999f ) / ( FPF( 0.3520887068f ) + mx.f ) );
    }

    /// Fast log approximation
    inline LHCb::SIMD::VC::FPF approx_log( const LHCb::SIMD::VC::FPF x ) noexcept {
      using namespace details::VC;
      return FPF( 0.69314718f ) * approx_log2( x );
    }

    /// Very fast log approximation
    inline LHCb::SIMD::VC::FPF vapprox_log( const LHCb::SIMD::VC::FPF x ) noexcept {
      using namespace details::VC;
      const union {
        FPF    f;
        UInt32 i;
      } vx = {x};
      return ( simd_cast<FPF>( vx.i ) * FPF( 8.2629582881927490e-8f ) ) - FPF( 87.989971088f );
    }

    /// Fast pow2 approximation
    inline LHCb::SIMD::VC::FPF approx_pow2( const LHCb::SIMD::VC::FPF p ) noexcept {
      using namespace details::VC;
      auto      clipp = p;
      const FPF A( -126.0f );
      clipp( p < A ) = A;
      const auto w   = simd_cast<Int32>( clipp );
      auto       z   = clipp - simd_cast<FPF>( w );
      z( p < FPF::Zero() ) += FPF::One();
      const union {
        UInt32 i;
        FPF    f;
      } v = {simd_cast<UInt32>(
          ( 1 << 23 ) *
          ( clipp + FPF( 121.2740575f ) + FPF( 27.7280233f ) / ( FPF( 4.84252568f ) - z ) - FPF( 1.49012907f ) * z ) )};
      return v.f;
    }

    /// Fast exp approximation
    inline LHCb::SIMD::VC::FPF approx_exp( const LHCb::SIMD::VC::FPF p ) noexcept {
      using namespace details::VC;
      return approx_pow2( LOG2EF * p );
    }

    /// Very fast pow2 approximation
    inline LHCb::SIMD::VC::FPF vapprox_pow2( const LHCb::SIMD::VC::FPF p ) noexcept {
      using namespace details::VC;
      auto      clipp = p;
      const FPF A( -126.0f );
      clipp( p < A ) = A;
      const union {
        UInt32 i;
        FPF    f;
      } v = {simd_cast<UInt32>( ( 1 << 23 ) * ( clipp + FPF( 126.94269504f ) ) )};
      return v.f;
    }

    /// Very fast exp approximation
    inline LHCb::SIMD::VC::FPF vapprox_exp( const LHCb::SIMD::VC::FPF p ) noexcept {
      using namespace details::VC;
      return vapprox_pow2( LOG2EF * p );
    }

    /// very fast approximate atan2
    inline LHCb::SIMD::VC::FPF vapprox_atan2( const LHCb::SIMD::VC::FPF y, const LHCb::SIMD::VC::FPF x ) {
      using namespace details::VC;

      const auto abs_y = abs( y ) + FPF( 1e-10f );

      const auto neg_x = ( x < FPF::Zero() );

      auto r     = ( x - abs_y ) / ( x + abs_y );
      r( neg_x ) = -FPF::One() / r;

      auto angle = ( ( FPF( 0.1963f ) * r * r - FPF( 0.9817f ) ) * r );
      angle( neg_x ) += THREEPIO4;
      angle( !neg_x ) += PIO4F;

      // opposite sign if in quad III or IV
      angle( y < FPF::Zero() ) *= -FPF::One();
      return angle;
    }

    //------------------------------------------------------------------------------

  } // namespace Approx

} // namespace LHCb::Math
