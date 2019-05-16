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
/** @file FastMaths-Scalar.h
 *
 * Scalar implementation of various fast mathematical functions.
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

// VDT
#include "vdt/asin.h"
#include "vdt/atan2.h"
#include "vdt/exp.h"
#include "vdt/log.h"
#include "vdt/sincos.h"
#include "vdt/sqrt.h"
#include "vdt/tan.h"

namespace LHCb::Math {

  //------------------------------------------------------------------------------

  /// Hidden(ish) details
  namespace details::SCALAR {

    static constexpr float THREEPIO4( 3.0f * float( M_PI_4 ) ); ///< 3*pi/4
    static constexpr float PIO4F( M_PI_4 );                     ///< pi/4
    static constexpr float PIO2F( M_PI_2 );                     ///< pi/2
    static constexpr float PIF( M_PI );                         ///< pi
    static constexpr float TAN_PIO8F( 0.4142135623730950 );     ///< tan(pi/8)
    static constexpr float LOG2EF( 1.44269504088896341f );      ///< log2(e)

  } // namespace details::SCALAR

  //------------------------------------------------------------------------------

  // Shortcuts to VDT for scalars.
  // For consistency across all scalar and SIMD types

  /// Fast sin and cos
  inline void fast_sincos( const float x, float& s, float& c ) noexcept { vdt::fast_sincosf( x, s, c ); }
  /// Fast sin and cos
  inline void fast_sincos( const double x, double& s, double& c ) noexcept { vdt::fast_sincos( x, s, c ); }

  /// Fast log
  inline float fast_log( const float x ) noexcept { return vdt::fast_logf( x ); }
  /// Fast log
  inline double fast_log( const double x ) noexcept { return vdt::fast_log( x ); }

  /// Fast exp
  inline float fast_exp( const float x ) noexcept { return vdt::fast_expf( x ); }
  /// Fast exp
  inline double fast_exp( const double x ) noexcept { return vdt::fast_exp( x ); }

  /// Fast inverse sqrt
  //[[deprecated("superseded by LHCb::Math::rsqrt<faster>")]]
  // inline float  fast_isqrt( const float  x ) noexcept { return vdt::fast_isqrtf(x); }
  /// Fast inverse sqrt
  //[[deprecated("superseded by LHCb::Math::rsqrt<fast>")]]
  // inline double fast_isqrt( const double x ) noexcept { return vdt::fast_isqrt(x); }

  /// Fast approximate inverse sqrt
  //[[deprecated("superseded by LHCb::Math::rsqrt<fastest>")]]
  // inline float  fast_approx_isqrt( const float  x ) noexcept { return vdt::fast_approx_isqrtf(x);
  // }
  /// Fast approximate inverse sqrt
  //[[deprecated("superseded by LHCb::Math::rsqrt<faster>")]]
  // inline double fast_approx_isqrt( const double x ) noexcept { return vdt::fast_approx_isqrt(x);
  // }

  /// Fast tan
  inline float fast_tan( const float x ) noexcept { return vdt::fast_tanf( x ); }
  /// Fast tan
  inline double fast_tan( const double x ) noexcept { return vdt::fast_tan( x ); }

  /// Fast atan2
  inline float fast_atan2( const float y, const float x ) noexcept { return vdt::fast_atan2f( y, x ); }
  /// Fast atan2
  inline double fast_atan2( const double y, const double x ) noexcept { return vdt::fast_atan2( y, x ); }

  /// Fast acos
  inline float fast_acos( const float x ) noexcept { return vdt::fast_acosf( x ); }
  /// Fast acos
  inline float fast_acos( const double x ) noexcept { return vdt::fast_acos( x ); }

  /// Fast asin
  inline float fast_asin( const float x ) noexcept { return vdt::fast_asinf( x ); }
  /// Fast asin
  inline float fast_asin( const double x ) noexcept { return vdt::fast_asin( x ); }

  /// Fast sqrt
  inline float fast_sqrt( const float x ) noexcept {

    union {
      float        x;
      std::int32_t i;
    } u = {x};

    // method 1
    u.i = ( 1 << 29 ) + ( u.i >> 1 ) - ( 1 << 22 );
    // Two Babylonian Steps
    u.x = 0.5f * ( u.x + ( x / u.x ) );
    u.x = 0.5f * ( u.x + ( x / u.x ) );
    return u.x;

    // u.i            = std::int32_t( 0x5f3759df ) - ( u.i >> 1 ); // gives initial guess y0
    // const auto xux = x * u.x;
    // return xux * ( 1.5f - ( 0.5f * xux * u.x ) ); // Newton step, repeating increases accuracy
  }

  //------------------------------------------------------------------------------
  namespace Approx {

    //------------------------------------------------------------------------------

    // Scalar versions

    /// Fast log2 approximation
    inline float approx_log2( const float x ) noexcept {
      const union {
        float         f;
        std::uint32_t i;
      } vx = {x};
      const union {
        std::uint32_t i;
        float         f;
      } mx         = {( vx.i & 0x007FFFFF ) | 0x3f000000};
      const auto y = float( vx.i ) * 1.1920928955078125e-7f;
      return ( y - 124.22551499f - 1.498030302f * mx.f - 1.72587999f / ( 0.3520887068f + mx.f ) );
    }

    /// Fast log approximation
    inline float approx_log( const float x ) noexcept { return 0.69314718f * approx_log2( x ); }

    /// Very Fast log approximation
    inline float vapprox_log( const float x ) noexcept {
      const union {
        float         f;
        std::uint32_t i;
      } vx = {x};
      return ( float( vx.i ) * 8.2629582881927490e-8f ) - 87.989971088f;
    }

    inline float approx_pow2( const float p ) {
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
    }

    /// Fast exp approximation
    inline float approx_exp( const float p ) noexcept {
      using namespace details::SCALAR;
      return approx_pow2( LOG2EF * p );
    }

    /// Very fast pow2 approximation
    inline float vapprox_pow2( const float p ) noexcept {
      const float clipp = ( p < -126 ? -126.0f : p );
      const union {
        uint32_t i;
        float    f;
      } v = {static_cast<std::uint32_t>( ( 1 << 23 ) * ( clipp + 126.94269504f ) )};
      return v.f;
    }

    /// Very fast exp approximation
    inline float vapprox_exp( const float p ) noexcept {
      using namespace details::SCALAR;
      return vapprox_pow2( LOG2EF * p );
    }

    /// very fast approximate atan2
    inline float vapprox_atan2( const float y, const float x ) {
      using namespace details::SCALAR;

      const auto abs_y = std::fabs( y ) + 1e-10f;

      const auto neg_x = ( x < 0.0f );

      const auto r = ( neg_x ? ( x + abs_y ) / ( abs_y - x ) : ( x - abs_y ) / ( x + abs_y ) );

      const auto angle = ( ( neg_x ? THREEPIO4 : PIO4F ) + ( ( 0.1963f * r * r - 0.9817f ) * r ) );

      // opposite sign if in quad III or IV
      return ( y < 0.0f ? -angle : angle );
    }

    /// Approx sqrt
    inline float approx_sqrt( const float x ) noexcept {

      union {
        float x;
        int   i;
      } u = {x};

      u.i = ( 1 << 29 ) + ( u.i >> 1 ) - ( 1 << 22 );

      // One Babylonian Step
      u.x = 0.5f * ( u.x + x / u.x );

      return u.x;
    }

    /// Very approx sqrt
    inline float vapprox_sqrt( const float x ) noexcept {

      union {
        float x;
        int   i;
      } u = {x};

      u.i = ( 1 << 29 ) + ( u.i >> 1 ) - ( 1 << 22 );

      return u.x;
    }

    //------------------------------------------------------------------------------

  } // namespace Approx

} // namespace LHCb::Math
