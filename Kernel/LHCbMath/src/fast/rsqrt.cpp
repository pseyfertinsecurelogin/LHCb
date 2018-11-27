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
#define LHCB_MATH_RSQRT_IMPL
#include "LHCbMath/FastMaths.h"

// clang does not support simd variant
// gcc on ARM or PPC does not support simd variant
#if !defined(__clang__) && (defined(__i386__) || defined(__x86_64__))

/**
 * Mangle vector variant helper
 */
#ifdef __INTEL_COMPILER
#define MANGLE_SSE    "x"
#define MANGLE_AVX    "y"
#define MANGLE_AVX2   "Y"
#define MANGLE_MIC    "z"
#define MANGLE_AVX512 "Z"
#else
#define MANGLE_SSE    "b"
#define MANGLE_AVX    "c"
#define MANGLE_AVX2   "d"
#define MANGLE_AVX512 "e"
#endif

// Get the mangled name of the SIMD variant
#define VEC(abi, vl, name, fp) "_ZGV" MANGLE_##abi "N" #vl "v_" LHCB_MATH_RSQRT_MANGLED_##name #fp

#if defined(__SSE2__)
namespace {
#if __INTEL_COMPILER > 1600 || __GNUC__ > 5
  /**
   * There is no efficient legit way to cast a single float into a SIMD register.
   *
   * On x86, float registers are the same as the SIMD registers.
   * The idea is to put the float into a register,
   *   and get the SIMD var from the same register.
   *
   * No instruction is required.
   */
  inline __m128 _mm_castf32_ss(float x) noexcept {
    __m128 v;
    asm("" : "=v"(v) : "0"(x));
    return v;
  }
  inline __m128d _mm_castf64_sd(double x) noexcept {
    __m128d v;
    asm("" : "=v"(v) : "0"(x));
    return v;
  }
#else
  // Fallback with less efficient code.
  inline __m128 _mm_castf32_ss(float x) noexcept {
    return _mm_set_ss(x);
  }
  inline __m128d _mm_castf64_sd(double x) noexcept {
    return _mm_set_sd(x);
  }
#endif

  /**
   * Helper to call the fastest RSQRT
   * They are mandatory in order to force inlining
   */
  inline float helper_scalar_rsqrt(float x) noexcept {
    __m128 v = _mm_castf32_ss(x);
#if defined(__AVX512ER__)
    v = _mm_rsqrt28_ss(v, v);
#elif defined(__AVX512F__)
    v = _mm_rsqrt14_ss(v, v);
#else
    v = _mm_rsqrt_ss(v);
#endif
    return _mm_cvtss_f32(v);
  }

  inline double helper_scalar_rsqrt(double x) noexcept {
#ifdef __AVX512F__
    __m128d v = _mm_castf64_sd(x);
#ifdef __AVX512ER__
    v = _mm_rsqrt28_sd(v, v);
#else   // !__AVX512ER__
    v = _mm_rsqrt14_sd(v, v);
#endif  // !__AVX512ER__
    return _mm_cvtsd_f64(v);
#else   // !__AVX512F__
    return helper_scalar_rsqrt((float)x);
#endif  // !__AVX512F__
  }
}
#endif  // __SSE2__

namespace LHCb {
  namespace Math {
    namespace details {

#if defined(__SSE2__)

      /**
       * Whenever possible, we use symbol aliases to reduce the number of duplicated functions
       * while keeping all functions fast (no dummy jump)
       */


      /**
       * Scalar F32
       */
#ifdef __AVX512ER__
      float __attribute((const, alias(LHCB_MATH_RSQRT_MANGLED_FASTEST "f"))) accurate_rsqrt(float x) noexcept;
#else
      float __attribute((const, alias(LHCB_MATH_RSQRT_MANGLED_IEEE    "f"))) accurate_rsqrt(float x) noexcept;
#endif
      float __attribute((const)) ieee_rsqrt(float x) noexcept {
        __m128 v = _mm_castf32_ss(x);
        v = _mm_div_ss(_mm_set_ss(1.f), _mm_sqrt_ss(v));
        return _mm_cvtss_f32(v);
      }
#ifdef __AVX512ER__
      float __attribute((const, alias(LHCB_MATH_RSQRT_MANGLED_FASTEST "f"))) fast_rsqrt(float x) noexcept;
#else
      float __attribute((const)) fast_rsqrt(float x) noexcept {
        float y = helper_scalar_rsqrt(x);
        y = rsqrt_nr(x, y);
        return y;
      }
#endif
      float __attribute((const, alias(LHCB_MATH_RSQRT_MANGLED_FAST "f"))) faster_rsqrt(float x) noexcept;
      float __attribute((const)) fastest_rsqrt(float x) noexcept {
        return helper_scalar_rsqrt(x);
      }

      /**
       * Scalar F64
       */
      double __attribute((const, alias(LHCB_MATH_RSQRT_MANGLED_IEEE "d"))) accurate_rsqrt(double x) noexcept;
      double __attribute((const)) ieee_rsqrt(double x) noexcept {
        __m128d v = _mm_castf64_sd(x);
        v = _mm_div_sd(_mm_set_sd(1.), _mm_sqrt_sd(v, v));
        return _mm_cvtsd_f64(v);
      }
#ifdef __AVX512F__
      double __attribute((const)) fast_rsqrt(double x) noexcept {
        double y = helper_scalar_rsqrt(x);
#ifndef __AVX512ER__
        y = rsqrt_nr(x, y);
#endif  // !__AVX512ER__
        y = rsqrt_nr(x, y);
        return y;
      }
#else   // !__AVX512F__
      double __attribute((const, alias(LHCB_MATH_RSQRT_MANGLED_IEEE "d"))) fast_rsqrt(double x) noexcept;
#endif  // !__AVX512F__
      double __attribute((const)) faster_rsqrt(double x) noexcept {
        double y = helper_scalar_rsqrt(x);
#ifndef __AVX512ER__
        y = rsqrt_nr(x, y);
#endif  // !__AVX512ER__
        y = rsqrt_nr(x, y);
        return y;
      }
      double __attribute((const)) fastest_rsqrt(double x) noexcept {
        return helper_scalar_rsqrt(x);
      }

      /**
       * For the SIMD variants, we just inline the functions taking SIMD variables
       */


      /**
       * SIMD128 F32
       */
      __m128 __attribute((const)) accurate_rsqrt(__m128 x) noexcept asm(VEC(SSE, 4, ACCURATE, f));
      __m128 __attribute((const, flatten)) accurate_rsqrt(__m128 x) noexcept {
        return rsqrt<accurate>(x);
      }
      __m128 __attribute((const)) ieee_rsqrt(__m128 x) noexcept asm(VEC(SSE, 4, IEEE, f));
      __m128 __attribute((const, flatten)) ieee_rsqrt(__m128 x) noexcept {
        return rsqrt<ieee>(x);
      }
      __m128 __attribute((const)) fast_rsqrt(__m128 x) noexcept asm(VEC(SSE, 4, FAST, f));
      __m128 __attribute((const, flatten)) fast_rsqrt(__m128 x) noexcept {
        return rsqrt<fast>(x);
      }
      __m128 __attribute((const)) faster_rsqrt(__m128 x) noexcept asm(VEC(SSE, 4, FASTER, f));
      __m128 __attribute((const, flatten)) faster_rsqrt(__m128 x) noexcept {
        return rsqrt<faster>(x);
      }
      __m128 __attribute((const)) fastest_rsqrt(__m128 x) noexcept asm(VEC(SSE, 4, FASTEST, f));
      __m128 __attribute((const, flatten)) fastest_rsqrt(__m128 x) noexcept {
        return rsqrt<fastest>(x);
      }

      /**
       * SIMD128 F64
       */
      __m128d __attribute((const)) accurate_rsqrt(__m128d x) noexcept asm(VEC(SSE, 2, ACCURATE, d));
      __m128d __attribute((const, flatten)) accurate_rsqrt(__m128d x) noexcept {
        return rsqrt<accurate>(x);
      }
      __m128d __attribute((const)) ieee_rsqrt(__m128d x) noexcept asm(VEC(SSE, 2, IEEE, d));
      __m128d __attribute((const, flatten)) ieee_rsqrt(__m128d x) noexcept {
        return rsqrt<ieee>(x);
      }
      __m128d __attribute((const)) fast_rsqrt(__m128d x) noexcept asm(VEC(SSE, 2, FAST, d));
      __m128d __attribute((const, flatten)) fast_rsqrt(__m128d x) noexcept {
        return rsqrt<fast>(x);
      }
      __m128d __attribute((const)) faster_rsqrt(__m128d x) noexcept asm(VEC(SSE, 2, FASTER, d));
      __m128d __attribute((const, flatten)) faster_rsqrt(__m128d x) noexcept {
        return rsqrt<faster>(x);
      }
      __m128d __attribute((const)) fastest_rsqrt(__m128d x) noexcept asm(VEC(SSE, 2, FASTEST, d));
      __m128d __attribute((const, flatten)) fastest_rsqrt(__m128d x) noexcept {
        return rsqrt<fastest>(x);
      }

#ifdef __AVX__
      /**
       * AVX2 variant is exactly the same as AVX, so it is defined as an alias of the AVX one
       */

      /**
       * SIMD256 F32
       */
      __m256 __attribute((const, alias(VEC(AVX, 8, ACCURATE, f)))) accurate_rsqrt2(__m256 x) noexcept asm(VEC(AVX2, 8, ACCURATE, f));
      __m256 __attribute((const)) accurate_rsqrt(__m256 x) noexcept asm(VEC(AVX, 8, ACCURATE, f));
      __m256 __attribute((const, flatten)) accurate_rsqrt(__m256 x) noexcept {
        return rsqrt<accurate>(x);
      }
      __m256 __attribute((const, alias(VEC(AVX, 8, IEEE, f)))) ieee_rsqrt2(__m256 x) noexcept asm(VEC(AVX2, 8, IEEE, f));
      __m256 __attribute((const)) ieee_rsqrt(__m256 x) noexcept asm(VEC(AVX, 8, IEEE, f));
      __m256 __attribute((const, flatten)) ieee_rsqrt(__m256 x) noexcept {
        return rsqrt<ieee>(x);
      }
      __m256 __attribute((const, alias(VEC(AVX, 8, FAST, f)))) fast_rsqrt2(__m256 x) noexcept asm(VEC(AVX2, 8, FAST, f));
      __m256 __attribute((const)) fast_rsqrt(__m256 x) noexcept asm(VEC(AVX, 8, FAST, f));
      __m256 __attribute((const, flatten)) fast_rsqrt(__m256 x) noexcept {
        return rsqrt<fast>(x);
      }
      __m256 __attribute((const, alias(VEC(AVX, 8, FASTER, f)))) faster_rsqrt2(__m256 x) noexcept asm(VEC(AVX2, 8, FASTER, f));
      __m256 __attribute((const)) faster_rsqrt(__m256 x) noexcept asm(VEC(AVX, 8, FASTER, f));
      __m256 __attribute((const, flatten)) faster_rsqrt(__m256 x) noexcept {
        return rsqrt<faster>(x);
      }
      __m256 __attribute((const, alias(VEC(AVX, 8, FASTEST, f)))) fastest_rsqrt2(__m256 x) noexcept asm(VEC(AVX2, 8, FASTEST, f));
      __m256 __attribute((const)) fastest_rsqrt(__m256 x) noexcept asm(VEC(AVX, 8, FASTEST, f));
      __m256 __attribute((const, flatten)) fastest_rsqrt(__m256 x) noexcept {
        return rsqrt<fastest>(x);
      }

      /**
       * SIMD256 F64
       */
      __m256d __attribute((const, alias(VEC(AVX, 4, ACCURATE, d)))) accurate_rsqrt2(__m256d x) noexcept asm(VEC(AVX2, 4, ACCURATE, d));
      __m256d __attribute((const)) accurate_rsqrt(__m256d x) noexcept asm(VEC(AVX, 4, ACCURATE, d));
      __m256d __attribute((const, flatten)) accurate_rsqrt(__m256d x) noexcept {
        return rsqrt<accurate>(x);
      }
      __m256d __attribute((const, alias(VEC(AVX, 4, IEEE, d)))) ieee_rsqrt2(__m256d x) noexcept asm(VEC(AVX2, 4, IEEE, d));
      __m256d __attribute((const)) ieee_rsqrt(__m256d x) noexcept asm(VEC(AVX, 4, IEEE, d));
      __m256d __attribute((const, flatten)) ieee_rsqrt(__m256d x) noexcept {
        return rsqrt<ieee>(x);
      }
      __m256d __attribute((const, alias(VEC(AVX, 4, FAST, d)))) fast_rsqrt2(__m256d x) noexcept asm(VEC(AVX2, 4, FAST, d));
      __m256d __attribute((const)) fast_rsqrt(__m256d x) noexcept asm(VEC(AVX, 4, FAST, d));
      __m256d __attribute((const, flatten)) fast_rsqrt(__m256d x) noexcept {
        return rsqrt<fast>(x);
      }
      __m256d __attribute((const, alias(VEC(AVX, 4, FASTER, d)))) faster_rsqrt2(__m256d x) noexcept asm(VEC(AVX2, 4, FASTER, d));
      __m256d __attribute((const)) faster_rsqrt(__m256d x) noexcept asm(VEC(AVX, 4, FASTER, d));
      __m256d __attribute((const, flatten)) faster_rsqrt(__m256d x) noexcept {
        return rsqrt<faster>(x);
      }
      __m256d __attribute((const, alias(VEC(AVX, 4, FASTEST, d)))) fastest_rsqrt2(__m256d x) noexcept asm(VEC(AVX2, 4, FASTEST, d));
      __m256d __attribute((const)) fastest_rsqrt(__m256d x) noexcept asm(VEC(AVX, 4, FASTEST, d));
      __m256d __attribute((const, flatten)) fastest_rsqrt(__m256d x) noexcept {
        return rsqrt<fastest>(x);
      }
#endif  // __AVX__
#ifdef __AVX512F__
      /**
       * SIMD512 F32
       */
      __m512 __attribute((const)) accurate_rsqrt(__m512 x) noexcept asm(VEC(AVX512, 16, ACCURATE, f));
      __m512 __attribute((const, flatten)) accurate_rsqrt(__m512 x) noexcept {
        return rsqrt<accurate>(x);
      }
      __m512 __attribute((const)) ieee_rsqrt(__m512 x) noexcept asm(VEC(AVX512, 16, IEEE, f));
      __m512 __attribute((const, flatten)) ieee_rsqrt(__m512 x) noexcept {
        return rsqrt<ieee>(x);
      }
      __m512 __attribute((const)) fast_rsqrt(__m512 x) noexcept asm(VEC(AVX512, 16, FAST, f));
      __m512 __attribute((const, flatten)) fast_rsqrt(__m512 x) noexcept {
        return rsqrt<fast>(x);
      }
      __m512 __attribute((const)) faster_rsqrt(__m512 x) noexcept asm(VEC(AVX512, 16, FASTER, f));
      __m512 __attribute((const, flatten)) faster_rsqrt(__m512 x) noexcept {
        return rsqrt<faster>(x);
      }
      __m512 __attribute((const)) fastest_rsqrt(__m512 x) noexcept asm(VEC(AVX512, 16, FASTEST, f));
      __m512 __attribute((const, flatten)) fastest_rsqrt(__m512 x) noexcept {
        return rsqrt<fastest>(x);
      }

      /**
       * SIMD512 F64
       */
      __m512d __attribute((const)) accurate_rsqrt(__m512d x) noexcept asm(VEC(AVX512, 8, ACCURATE, d));
      __m512d __attribute((const, flatten)) accurate_rsqrt(__m512d x) noexcept {
        return rsqrt<accurate>(x);
      }
      __m512d __attribute((const)) ieee_rsqrt(__m512d x) noexcept asm(VEC(AVX512, 8, IEEE, d));
      __m512d __attribute((const, flatten)) ieee_rsqrt(__m512d x) noexcept {
        return rsqrt<ieee>(x);
      }
      __m512d __attribute((const)) fast_rsqrt(__m512d x) noexcept asm(VEC(AVX512, 8, FAST, d));
      __m512d __attribute((const, flatten)) fast_rsqrt(__m512d x) noexcept {
        return rsqrt<fast>(x);
      }
      __m512d __attribute((const)) faster_rsqrt(__m512d x) noexcept asm(VEC(AVX512, 8, FASTER, d));
      __m512d __attribute((const, flatten)) faster_rsqrt(__m512d x) noexcept {
        return rsqrt<faster>(x);
      }
      __m512d __attribute((const)) fastest_rsqrt(__m512d x) noexcept asm(VEC(AVX512, 8, FASTEST, d));
      __m512d __attribute((const, flatten)) fastest_rsqrt(__m512d x) noexcept {
        return rsqrt<fastest>(x);
      }
#endif  // __AVX512F__

#else   // !__SSE2__  (Fallback)
#define LHCB_MATH_RSQRT_SIMD __attribute((const, simd("notinbranch")))

      /**
       * Scalar F32
       */
      LHCB_MATH_RSQRT_SIMD float accurate_rsqrt(float x) noexcept {
        return 1.f / __builtin_sqrtf(x);
      }
      LHCB_MATH_RSQRT_SIMD float ieee_rsqrt(float x) noexcept {
        return 1.f / __builtin_sqrtf(x);
      }
      LHCB_MATH_RSQRT_SIMD float __attribute((optimize("fast-math"))) fast_rsqrt(float x) noexcept {
        return 1.f / __builtin_sqrtf(x);
      }
      LHCB_MATH_RSQRT_SIMD float __attribute((optimize("fast-math"))) faster_rsqrt(float x) noexcept {
        return 1.f / __builtin_sqrtf(x);
      }
      LHCB_MATH_RSQRT_SIMD float __attribute((optimize("fast-math"))) fastest_rsqrt(float x) noexcept {
        return 1.f / __builtin_sqrtf(x);
      }

      /**
       * Scalar F64
       */
      LHCB_MATH_RSQRT_SIMD double accurate_rsqrt(double x) noexcept {
        return 1. / __builtin_sqrt(x);
      }
      LHCB_MATH_RSQRT_SIMD double ieee_rsqrt(double x) noexcept {
        return 1. / __builtin_sqrt(x);
      }
      LHCB_MATH_RSQRT_SIMD double __attribute((optimize("fast-math"))) fast_rsqrt(double x) noexcept {
        return 1. / __builtin_sqrt(x);
      }
      LHCB_MATH_RSQRT_SIMD double __attribute((optimize("fast-math"))) faster_rsqrt(double x) noexcept {
        return 1. / __builtin_sqrt(x);
      }
      LHCB_MATH_RSQRT_SIMD double __attribute((optimize("fast-math"))) fastest_rsqrt(double x) noexcept {
        return 1. / __builtin_sqrt(x);
      }

#endif  // !__SSE2__

    } // namespace details
  }  // namespace Math
}  // namespace LHCb

#endif  // !__clang__ && (__i386__ || __x86_64__)
