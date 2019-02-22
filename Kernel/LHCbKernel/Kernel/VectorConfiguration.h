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

#include "vectorclass.h"
#include <cstddef> // std::size_t
#include <cstdint> // std::*int*_t
#include <type_traits>

/**
 * @brief      Helper static methods to expose the SIMD
 *             capabilities of the machine.
 */
struct VectorConfiguration {

/**
 * @brief      Byte width of SIMD architecture.
 */
#if defined( STATIC_VECTOR_BYTEWIDTH )
  static_assert( STATIC_VECTOR_BYTEWIDTH >= 8 && STATIC_VECTOR_BYTEWIDTH <= 64 && STATIC_VECTOR_BYTEWIDTH &&
                     ( ( STATIC_VECTOR_BYTEWIDTH & ( STATIC_VECTOR_BYTEWIDTH - 1 ) ) == 0 ),
                 "Configured vector configuration must be a power of two, between 8 and 64" );

  static constexpr std::size_t bytewidth() { return STATIC_VECTOR_BYTEWIDTH; }
#elif defined( __AVX512F__ )
  static constexpr std::size_t bytewidth() { return 64; }
#elif defined( __AVX__ )
  static constexpr std::size_t bytewidth() { return 32; }
#elif defined( __SSE__ )
  static constexpr std::size_t bytewidth() { return 16; }
#else
  static constexpr std::size_t bytewidth() { return 8; }
#endif

  /**
   * @brief      Number of elements of a given precision
   *             fitting in a vector unit.
   *
   * @tparam     PRECISION  Precision of elements.
   *
   * @return     Number of elements of a given precision
   *             fitting in a vector unit.
   */
  template <class PRECISION>
  static constexpr std::size_t width() {
    return VectorConfiguration::bytewidth() / sizeof( PRECISION );
  }
};

namespace LHCb {
  namespace SIMD {

    // Dispatch tags
    struct VCL_tag;
    struct Vc_tag;
    struct UME_tag;
    struct builtin_tag;

    // Forward declaration of Vectype
    template <class T, std::size_t W, class Tag = VCL_tag>
    struct Vectype;

    // Helper
    namespace details {
      template <class T, std::size_t W, class Tag, class Enable = void>
      struct Vectype;

      template <class T, std::size_t W, class Tag>
      struct Vectype<T, W, Tag, typename std::enable_if<sizeof( T ) == 4>::type> {
        using scalar_t                           = T;
        using float_                             = LHCb::SIMD::Vectype<float, W, Tag>;
        using int_                               = LHCb::SIMD::Vectype<std::int32_t, W, Tag>;
        using uint_                              = LHCb::SIMD::Vectype<std::uint32_t, W, Tag>;
        static constexpr std::size_t cardinality = W;
      };

      template <class T, std::size_t W, class Tag>
      struct Vectype<T, W, Tag, typename std::enable_if<sizeof( T ) == 8>::type> {
        using scalar_t                           = T;
        using float_                             = LHCb::SIMD::Vectype<double, W, Tag>;
        using int_                               = LHCb::SIMD::Vectype<std::int64_t, W, Tag>;
        using uint_                              = LHCb::SIMD::Vectype<std::uint64_t, W, Tag>;
        static constexpr std::size_t cardinality = W;
      };
    } // namespace details

    /* scalar */
    template <class T, class Tag>
    struct Vectype<T, 1, Tag> : details::Vectype<T, 1, Tag> {
      using type   = T;
      using bool_t = bool;
      // backward compatibility
      using booltype = bool_t;
    };
    // avoid ambiguities
    template <class T>
    struct Vectype<T, 1, Vc_tag> : details::Vectype<T, 1, Vc_tag> {
      using type   = T;
      using bool_t = bool;
      // backward compatibility
      using booltype = bool_t;
    };
    // avoid ambiguities
    template <class T>
    struct Vectype<T, 1, UME_tag> : details::Vectype<T, 1, UME_tag> {
      using type   = T;
      using bool_t = bool;
      // backward compatibility
      using booltype = bool_t;
    };
    // avoid ambiguities
    template <class T>
    struct Vectype<T, 1, builtin_tag> : details::Vectype<T, 1, builtin_tag> {
      using type   = T;
      using bool_t = bool;
      // backward compatibility
      using booltype = bool_t;
    };

/**
 * VCL
 */
#if defined( __SSE__ )
    template <>
    struct Vectype<std::int32_t, 4, VCL_tag> : details::Vectype<std::int32_t, 4, VCL_tag> {
      using type   = Vec4i;
      using bool_t = Vec4ib;
      // backward compatibility
      using booltype = bool_t;
    };
    template <>
    struct Vectype<std::uint32_t, 4, VCL_tag> : details::Vectype<std::uint32_t, 4, VCL_tag> {
      using type   = Vec4ui;
      using bool_t = Vec4ib;
      // backward compatibility
      using booltype = bool_t;
    };
    template <>
    struct Vectype<float, 4, VCL_tag> : details::Vectype<float, 4, VCL_tag> {
      using type   = Vec4f;
      using bool_t = Vec4fb;
      // backward compatibility
      using booltype = bool_t;
    };

    template <>
    struct Vectype<std::int64_t, 2, VCL_tag> : details::Vectype<std::int64_t, 2, VCL_tag> {
      using type   = Vec2q;
      using bool_t = Vec2qb;
      // backward compatibility
      using booltype = bool_t;
    };
    template <>
    struct Vectype<std::uint64_t, 2, VCL_tag> : details::Vectype<std::uint64_t, 2, VCL_tag> {
      using type   = Vec2uq;
      using bool_t = Vec2qb;
      // backward compatibility
      using booltype = bool_t;
    };
    template <>
    struct Vectype<double, 2, VCL_tag> : details::Vectype<double, 2, VCL_tag> {
      using type   = Vec2d;
      using bool_t = Vec2db;
      // backward compatibility
      using booltype = bool_t;
    };
#endif // __SSE__
#if defined( __AVX__ ) && MAX_VECTOR_SIZE >= 256
    template <>
    struct Vectype<std::int32_t, 8, VCL_tag> : details::Vectype<std::int32_t, 8, VCL_tag> {
      using type   = Vec8i;
      using bool_t = Vec8ib;
      // backward compatibility
      using booltype = bool_t;
    };
    template <>
    struct Vectype<std::uint32_t, 8, VCL_tag> : details::Vectype<std::uint32_t, 8, VCL_tag> {
      using type   = Vec8ui;
      using bool_t = Vec8ib;
      // backward compatibility
      using booltype = bool_t;
    };
    template <>
    struct Vectype<float, 8, VCL_tag> : details::Vectype<float, 8, VCL_tag> {
      using type   = Vec8f;
      using bool_t = Vec8fb;
      // backward compatibility
      using booltype = bool_t;
    };

    template <>
    struct Vectype<std::int64_t, 4, VCL_tag> : details::Vectype<std::int64_t, 4, VCL_tag> {
      using type   = Vec4q;
      using bool_t = Vec4qb;
      // backward compatibility
      using booltype = bool_t;
    };
    template <>
    struct Vectype<std::uint64_t, 4, VCL_tag> : details::Vectype<std::uint64_t, 4, VCL_tag> {
      using type   = Vec4uq;
      using bool_t = Vec4qb;
      // backward compatibility
      using booltype = bool_t;
    };
    template <>
    struct Vectype<double, 4, VCL_tag> : details::Vectype<double, 4, VCL_tag> {
      using type   = Vec4d;
      using bool_t = Vec4db;
      // backward compatibility
      using booltype = bool_t;
    };
#endif // __AVX__
#if defined( __AVX512F__ ) && MAX_VECTOR_SIZE >= 512
    template <>
    struct Vectype<std::int32_t, 16, VCL_tag> : details::Vectype<std::int32_t, 16, VCL_tag> {
      using type   = Vec16i;
      using bool_t = Vec16ib;
      // backward compatibility
      using booltype = bool_t;
    };
    template <>
    struct Vectype<std::uint32_t, 16, VCL_tag> : details::Vectype<std::uint32_t, 16, VCL_tag> {
      using type   = Vec16ui;
      using bool_t = Vec16ib;
      // backward compatibility
      using booltype = bool_t;
    };
    template <>
    struct Vectype<float, 16, VCL_tag> : details::Vectype<float, 16, VCL_tag> {
      using type   = Vec16f;
      using bool_t = Vec16fb;
      // backward compatibility
      using booltype = bool_t;
    };

    template <>
    struct Vectype<std::int64_t, 8, VCL_tag> : details::Vectype<std::int64_t, 8, VCL_tag> {
      using type   = Vec8q;
      using bool_t = Vec8qb;
      // backward compatibility
      using booltype = bool_t;
    };
    template <>
    struct Vectype<std::uint64_t, 8, VCL_tag> : details::Vectype<std::uint64_t, 8, VCL_tag> {
      using type   = Vec8uq;
      using bool_t = Vec8qb;
      // backward compatibility
      using booltype = bool_t;
    };
    template <>
    struct Vectype<double, 8, VCL_tag> : details::Vectype<double, 8, VCL_tag> {
      using type   = Vec8d;
      using bool_t = Vec8db;
      // backward compatibility
      using booltype = bool_t;
    };
#endif // __AVX512F__

/**
 * Vc
 */
#ifdef VC_VC_
    namespace details {
      template <std::size_t size>
      struct Vc_ABI;

#  if defined( __SSE2__ )
      template <>
      struct Vc_ABI<16> {
        using type = Vc::VectorAbi::Sse;
      };
#  endif
#  if defined( __AVX__ )
      template <>
      struct Vc_ABI<32> {
        using type = Vc::VectorAbi::Avx;
      };
#  endif
    } // namespace details

    template <class T, int W>
    struct Vectype<T, W, Vc_tag> : details::Vectype<T, W, Vc_Tag> {
      using type   = Vc::Vector<T, typename details::Vc_ABI::type>;
      using bool_t = typename type::mask_type;
      // backward compatibility
      using booltype = bool_t;
    };
#endif

/**
 * UME
 */
#ifdef UME_SIMD_H_
    template <class T, int W>
    struct Vectype<T, W, Vc_tag> : details::Vectype<T, W, Vc_Tag> {
      using type   = UME::SIMD::SIMDVec<T, W>;
      using bool_t = decltype( type{} == type{} );
      // backward compatibility
      using booltype = bool_t;
    };
#endif

    /**
     * intrin
     */
    template <class T, std::size_t W>
    struct Vectype<T, W, builtin_tag> : details::Vectype<T, W, builtin_tag> {
#ifdef __clang__
      // clang does not support __vector_size__() with arbitrary expressions
      typedef T __attribute__( ( ext_vector_type( W ) ) ) type;
#else
      // gcc ignores the attribute if "using" is used instead of typedef
      typedef T __attribute__( ( __vector_size__( W * sizeof( T ) ) ) ) type;
#endif
      using bool_t = decltype( type{} == type{} );
      // backward compatibility
      using booltype = bool_t;
    };

  } // namespace SIMD
} // namespace LHCb

// Alias for backward compatibility
namespace LHCb {
  namespace Vector {
    using LHCb::SIMD::Vectype;
  }
} // namespace LHCb
