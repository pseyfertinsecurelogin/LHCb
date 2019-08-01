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

#pragma once

// STL
#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>

// local
#include "LHCbMath/FastMaths.h"
#include "LHCbMath/SIMDTypes.h"
#include "LHCbMath/bit_cast.h"

namespace LHCb::Math {

  /// Precision type
  enum class PrecisionMode { Relative, Absolute };

  /// Truncate the given value to a given number of decimal places
  template <PrecisionMode MODE,      ///< Precision mode (relative or absolute)
            std::size_t   PRECISION, ///< Number of d.p. to truncate to
            typename TYPE            ///< variable type
            >
  inline std::enable_if_t<std::is_floating_point_v<TYPE> || LHCb::SIMD::is_SIMD_v<TYPE>, TYPE>
  truncate( const TYPE x ) noexcept {

    if constexpr ( PrecisionMode::Absolute == MODE ) {
      // absolute precision truncation based on decimal places

      if constexpr ( std::is_floating_point_v<TYPE> ) {

        // int types
        using INT  = std::conditional_t<std::is_same_v<TYPE, float>, std::int32_t, std::int64_t>;
        using UINT = std::conditional_t<std::is_same_v<TYPE, float>, std::uint32_t, std::uint64_t>;

        // sanity check on required precision level
        static_assert( PRECISION <= std::log10( std::numeric_limits<UINT>::max() ) );

        // scale factors for the given number of DP
        constexpr TYPE dp     = std::pow( UINT( 10u ), PRECISION );
        constexpr TYPE inv_dp = 1.0f / TYPE( dp );

        // cast and return
        return static_cast<TYPE>( static_cast<INT>( x * dp ) ) * inv_dp;

      } else if constexpr ( LHCb::SIMD::is_SIMD_v<TYPE> ) {

        //  Get the (scalar) float type
        using FP = typename TYPE::value_type;

        // int types
        using INT  = std::conditional_t<std::is_same_v<FP, float>, std::int32_t, std::int64_t>;
        using UINT = std::conditional_t<std::is_same_v<FP, float>, std::uint32_t, std::uint64_t>;

        // sanity check on required precision level
        static_assert( PRECISION <= std::log10( std::numeric_limits<UINT>::max() ) );

        // scale factors for the given number of DP
        constexpr FP dp     = std::pow( UINT( 10u ), PRECISION );
        constexpr FP inv_dp = 1.0f / FP( dp );

        // cast and return
        using namespace LHCb::SIMD;
        return lb_cast<TYPE>( lb_cast<LHCb::SIMD::INT<INT>>( x * TYPE( dp ) ) ) * TYPE( inv_dp );
      }

    } else if constexpr ( PrecisionMode::Relative == MODE ) {
      // relative precision truncation based on mantissa bits

      if constexpr ( std::is_floating_point_v<TYPE> ) {

        // int type
        using UINT = std::conditional_t<std::is_same_v<TYPE, float>, std::uint32_t, std::uint64_t>;

        // number of mantissa bits for given type
        constexpr UINT mantissa_bits = ( std::is_same_v<TYPE, float> ? 24u : 53u );

        // sanity check on required precision level
        static_assert( mantissa_bits > PRECISION );

        // mantissa mask
        constexpr UINT mask = ( ~UINT( 0x0u ) << ( mantissa_bits - PRECISION ) );

        // return
        return bit_cast<TYPE>( bit_cast<UINT>( x ) & mask );

      } else if constexpr ( LHCb::SIMD::is_SIMD_v<TYPE> ) {

        //  Get the (scalar) float type
        using FP = typename TYPE::value_type;

        // int type
        using UINT = std::conditional_t<std::is_same_v<FP, float>, std::uint32_t, std::uint64_t>;

        // number of mantissa bits for given type
        constexpr UINT mantissa_bits = ( std::is_same_v<FP, float> ? 24u : 53u );

        // sanity check on required precision level
        static_assert( mantissa_bits > PRECISION );

        // mantissa mask
        constexpr UINT mask = ( ~UINT( 0x0u ) << ( mantissa_bits - PRECISION ) );

        // return
        using namespace LHCb::Math::impl;
        return union_cast<TYPE>( union_cast<LHCb::SIMD::INT<UINT>>( x ) & LHCb::SIMD::INT<UINT>( mask ) );
      }
    }
  }

} // namespace LHCb::Math
