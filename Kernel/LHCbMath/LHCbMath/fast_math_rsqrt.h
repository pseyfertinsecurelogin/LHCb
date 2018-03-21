#ifndef FAST_MATH_RSQRT_H
#define FAST_MATH_RSQRT_H

#include <cstdint>
#include <cmath>
#include <utility>

#ifdef __SSE2__
#include <immintrin.h>
#endif  // __SSE2__
#ifdef __ARM_NEON
#include <arm_neon.h>
#endif  // __ARM_NEON
#ifdef __ALTIVEC__
#include <altivec.h>
#endif  // __ALTIVEC

#include "LHCbMath/FastMathAccuracyTags.h"
#include "LHCbMath/SIMDTypes.h"
#include "VectorClass/vectorclass.h"

/**
 * Scalar helpers (vectorization-ready)
 */
#define LHCB_MATH_RSQRT_MANGLED_ACCURATE "_ZN4LHCb4Math7details14accurate_rsqrtE"
#define LHCB_MATH_RSQRT_MANGLED_IEEE     "_ZN4LHCb4Math7details10ieee_rsqrtE"
#define LHCB_MATH_RSQRT_MANGLED_FAST     "_ZN4LHCb4Math7details10fast_rsqrtE"
#define LHCB_MATH_RSQRT_MANGLED_FASTER   "_ZN4LHCb4Math7details12faster_rsqrtE"
#define LHCB_MATH_RSQRT_MANGLED_FASTEST  "_ZN4LHCb4Math7details13fastest_rsqrtE"

/**
 * Declare scalar functions with SIMD variants
 * OpenMP equivalent: #pragma omp declare simd
 *
 * The symbol name of a SIMD variant is the mangled name of the function
 *   prefixed with extra information
 *
 * To facilitate the definiton of such variant, we need to know
 *   the mangled name of these scalar functions
 *
 * More Information about the ABI for GCC:
 * https://sourceware.org/glibc/wiki/libmvec?action=AttachFile&do=view&target=VectorABI.txt
 *
 * More Information about the ABI for ICC:
 * https://software.intel.com/en-us/articles/vector-simd-function-abi
 */
#ifndef LHCB_MATH_RSQRT_IMPL
#ifdef __INTEL_COMPILER
#define LHCB_MATH_RSQRT_SIMD __attribute((const, vector(nomask)))
#else
#define LHCB_MATH_RSQRT_SIMD __attribute((const, simd("notinbranch")))
#endif
#else
#define LHCB_MATH_RSQRT_SIMD __attribute((const))
#endif

namespace LHCb {
  namespace Math {
    namespace details {
      // clang does not support simd variant
      // gcc on ARM or PPC does not support simd variant
#if defined(__clang__) || !(defined(__i386__) || defined(__x86_64__))
      inline float accurate_rsqrt(float x) noexcept { return 1.f / __builtin_sqrtf(x); }
      inline float     ieee_rsqrt(float x) noexcept { return 1.f / __builtin_sqrtf(x); }
      inline float     fast_rsqrt(float x) noexcept { return 1.f / __builtin_sqrtf(x); }
      inline float   faster_rsqrt(float x) noexcept { return 1.f / __builtin_sqrtf(x); }
      inline float  fastest_rsqrt(float x) noexcept { return 1.f / __builtin_sqrtf(x); }

      inline double accurate_rsqrt(double x) noexcept { return 1. / __builtin_sqrt(x); }
      inline double     ieee_rsqrt(double x) noexcept { return 1. / __builtin_sqrt(x); }
      inline double     fast_rsqrt(double x) noexcept { return 1. / __builtin_sqrt(x); }
      inline double   faster_rsqrt(double x) noexcept { return 1. / __builtin_sqrt(x); }
      inline double  fastest_rsqrt(double x) noexcept { return 1. / __builtin_sqrt(x); }
#else   // !__clang__ && (__i386__ || __x86_64__)
      // The asm directive define the symbol name used by the function
      LHCB_MATH_RSQRT_SIMD float accurate_rsqrt(float x) noexcept asm(LHCB_MATH_RSQRT_MANGLED_ACCURATE "f");
      LHCB_MATH_RSQRT_SIMD float     ieee_rsqrt(float x) noexcept asm(LHCB_MATH_RSQRT_MANGLED_IEEE     "f");
      LHCB_MATH_RSQRT_SIMD float     fast_rsqrt(float x) noexcept asm(LHCB_MATH_RSQRT_MANGLED_FAST     "f");
      LHCB_MATH_RSQRT_SIMD float   faster_rsqrt(float x) noexcept asm(LHCB_MATH_RSQRT_MANGLED_FASTER   "f");
      LHCB_MATH_RSQRT_SIMD float  fastest_rsqrt(float x) noexcept asm(LHCB_MATH_RSQRT_MANGLED_FASTEST  "f");

      LHCB_MATH_RSQRT_SIMD double accurate_rsqrt(double x) noexcept asm(LHCB_MATH_RSQRT_MANGLED_ACCURATE "d");
      LHCB_MATH_RSQRT_SIMD double     ieee_rsqrt(double x) noexcept asm(LHCB_MATH_RSQRT_MANGLED_IEEE     "d");
      LHCB_MATH_RSQRT_SIMD double     fast_rsqrt(double x) noexcept asm(LHCB_MATH_RSQRT_MANGLED_FAST     "d");
      LHCB_MATH_RSQRT_SIMD double   faster_rsqrt(double x) noexcept asm(LHCB_MATH_RSQRT_MANGLED_FASTER   "d");
      LHCB_MATH_RSQRT_SIMD double  fastest_rsqrt(double x) noexcept asm(LHCB_MATH_RSQRT_MANGLED_FASTEST  "d");
#endif  // !__clang__ && (__i386__ || __x86_64__)

#undef LHCB_MATH_RSQRT_SIMD

      /**
       * Helper to compute one single iteration of newton raphson
       */
      // Generic case
      template <class T>
      static inline T helper_rsqrt_nr(T x, T y) noexcept(noexcept(T(y * (T(1.5) + y*y*(T(-0.5) * x))))) {
        return y * (T(1.5) + y*y*(T(-0.5) * x));
      }

      // SSE
#ifdef __SSE2__
      static inline __m128 helper_rsqrt_nr(__m128 x, __m128 y) noexcept {
        __m128 v3half = _mm_set1_ps(1.5f), vhalfn = _mm_set1_ps(-0.5f);
        __m128 xhalfn = _mm_mul_ps(x, vhalfn);
#ifndef __FMA__
        return _mm_mul_ps(y, _mm_add_ps(v3half, _mm_mul_ps(y, _mm_mul_ps(y, xhalfn))));
#else  // __FMA__
        return _mm_mul_ps(y, _mm_fmadd_ps(y, _mm_mul_ps(y, xhalfn), v3half));
#endif  // __FMA__
      }
      static inline __m128d helper_rsqrt_nr(__m128d x, __m128d y) noexcept {
        __m128d v3half = _mm_set1_pd(1.5), vhalfn = _mm_set1_pd(-0.5);
        __m128d xhalfn = _mm_mul_pd(x, vhalfn);
#ifndef __FMA__
        return _mm_mul_pd(y, _mm_add_pd(v3half, _mm_mul_pd(y, _mm_mul_pd(y, xhalfn))));
#else   // __FMA__
        return _mm_mul_pd(y, _mm_fmadd_pd(y, _mm_mul_pd(y, xhalfn), v3half));
#endif  // __FMA__
      }
#endif  // __SSE2__

      // AVX
#ifdef __AVX__
      static inline __m256 helper_rsqrt_nr(__m256 x, __m256 y) noexcept {
        __m256 v3half = _mm256_set1_ps(1.5f), vhalfn = _mm256_set1_ps(-0.5f);
        __m256 xhalfn = _mm256_mul_ps(x, vhalfn);
#ifndef __FMA__
        return _mm256_mul_ps(y, _mm256_add_ps(v3half, _mm256_mul_ps(y, _mm256_mul_ps(y, xhalfn))));
#else   // __FMA__
        return _mm256_mul_ps(y, _mm256_fmadd_ps(y, _mm256_mul_ps(y, xhalfn), v3half));
#endif  // __FMA__
      }
      static inline __m256d helper_rsqrt_nr(__m256d x, __m256d y) noexcept {
        __m256d v3half = _mm256_set1_pd(1.5), vhalfn = _mm256_set1_pd(-0.5);
        __m256d xhalfn = _mm256_mul_pd(x, vhalfn);
#ifndef __FMA__
        return _mm256_mul_pd(y, _mm256_add_pd(v3half, _mm256_mul_pd(y, _mm256_mul_pd(y, xhalfn))));
#else   // __FMA__
        return _mm256_mul_pd(y, _mm256_fmadd_pd(y, _mm256_mul_pd(y, xhalfn), v3half));
#endif  // __FMA__
      }
#endif  // __AVX__

      // AVX512
#ifdef __AVX512F__
      static inline __m512 helper_rsqrt_nr(__m512 x, __m512 y) noexcept {
        __m512 v3half = _mm512_set1_ps(1.5f), vhalfn = _mm512_set1_ps(-0.5f);
        __m512 xhalfn = _mm512_mul_ps(x, vhalfn);
        return _mm512_mul_ps(y, _mm512_fmadd_ps(y, _mm512_mul_ps(y, xhalfn), v3half));
      }
      static inline __m512d helper_rsqrt_nr(__m512d x, __m512d y) noexcept {
        __m512d v3half = _mm512_set1_pd(1.5), vhalfn = _mm512_set1_pd(-0.5);
        __m512d xhalfn = _mm512_mul_pd(x, vhalfn);
        return _mm512_mul_pd(y, _mm512_fmadd_pd(y, _mm512_mul_pd(y, xhalfn), v3half));
      }
#endif  // __AVX512F__

#ifdef __ARM_NEON
      static inline float32x2_t helper_rsqrt_nr(float32x2_t x, float32x2_t y) noexcept {
        return vmul_f32(vrsqrts_f32(vmul_f32(x, y), y), y);
      }
      static inline float32x4_t helper_rsqrt_nr(float32x4_t x, float32x4_t y) noexcept {
        return vmulq_f32(vrsqrtsq_f32(vmulq_f32(x, y), y), y);
      }

#ifdef __aarch64__
      static inline float64x2_t helper_rsqrt_nr(float64x2_t x, float64x2_t y) noexcept {
        return vmulq_f64(vrsqrtsq_f64(vmulq_f64(x, y), y), y);
      }
#endif  // __aarch64__
#endif  // __ARM_NEON

#ifdef __ALTIVEC__
      static inline vector float helper_rsqrt_nr(vector float x, vector float y) noexcept {
        vector float vhalfn = {-0.5f, -0.5f, -0.5f, -0.5f};
        vector float vthreehalf = {1.5f, 1.5f, 1.5f, 1.5f};
#ifndef __VSX__
        vector float vzero = {0.f, 0.f, 0.f, 0.f};
        return vec_madd(y, vec_madd(y, vec_madd(y, vec_madd(x, vhalfn, vzero), vzero), vthreehalf), vzero);
#else   // __VSX__
        return vec_mul(y, vec_madd(y, vec_mul(y, vec_mul(x, vhalfn)), vthreehalf));
#endif  // __VSX__
      }


#ifdef __VSX__
      static inline vector double helper_rsqrt_nr(vector double x, vector double y) noexcept {
        vector double vhalfn = {-0.5, -0.5};
        vector double vthreehalf = {1.5, 1.5};
        return vec_mul(y, vec_madd(y, vec_mul(y, vec_mul(x, vhalfn)), vthreehalf));
      }
#endif  // __VSX__
#endif  // __ALTIVEC__

      /**
       * Implementation of rsqrt
       */
      template <class Tag = accurate>
      struct RSQRT;

      using std::sqrt;

      // IEEE accuracy
      template <>
      struct RSQRT<ieee> {
        // Generic case
        template <class T>
        static inline T doit(T x) noexcept(noexcept((T(T(1) / sqrt(x))))) {
          return T(1) / sqrt(x);
        }

        static inline float  doit(float  x) noexcept { return ieee_rsqrt(x); }
        static inline double doit(double x) noexcept { return ieee_rsqrt(x); }

        // SSE
#ifdef __SSE__
        static inline __m128 doit(__m128 x) noexcept {
          return _mm_div_ps(_mm_set1_ps(1.f), _mm_sqrt_ps(x));
        }
        static inline __m128d doit(__m128d x) noexcept {
          return _mm_div_pd(_mm_set1_pd(1.), _mm_sqrt_pd(x));
        }
#endif  // __SSE__

        // AVX
#ifdef __AVX__
        static inline __m256 doit(__m256 x) noexcept {
          return _mm256_div_ps(_mm256_set1_ps(1.f), _mm256_sqrt_ps(x));
        }
        static inline __m256d doit(__m256d x) noexcept {
          return _mm256_div_pd(_mm256_set1_pd(1.), _mm256_sqrt_pd(x));
        }
#endif  // __AVX__

        // AVX512
#ifdef __AVX512F__
        static inline __m512 doit(__m512 x) noexcept {
          return _mm512_div_ps(_mm512_set1_ps(1.f), _mm512_sqrt_ps(x));
        }
        static inline __m512d doit(__m512d x) noexcept {
          return _mm512_div_pd(_mm512_set1_pd(1.), _mm512_sqrt_pd(x));
        }
#endif  // __AVX512F__

#ifdef __ARM_NEON
        static inline float32x2_t doit(float32x2_t x) noexcept {
#if defined(__ARM_FEATURE_DIV) || defined(__aarch64__)
          return vdiv_f32(vdup_n_f32(1.f), vsqrt_f32(x));
#else   // !__ARM_FEATURE_DIV
          // If no Neon instruction for division, fallback to scalar to ensure IEEE compliance
          alignas(alignof(float32x2_t)) float A[2];
          vst1_f32(A, x);
          for (int i = 0; i < 2; i++) {
            A[i] = 1.f / __builtin_sqrtf(A[i]);
          }
          return vld1_f32(A);
#endif  // !__ARM_FEATURE_DIV
        }
        static inline float32x4_t doit(float32x4_t x) noexcept {
#if defined(__ARM_FEATURE_DIV) || defined(__aarch64__)
          return vdivq_f32(vdupq_n_f32(1.f), vsqrtq_f32(x));
#else   // !__ARM_FEATURE_DIV
          // If no Neon instruction for division, fallback to scalar to ensure IEEE compliance
          alignas(alignof(float32x4_t)) float A[4];
          vst1q_f32(A, x);
          for (int i = 0; i < 4; i++) {
            A[i] = 1.f / __builtin_sqrtf(A[i]);
          }
          return vld1q_f32(A);
#endif  // !__ARM_FEATURE_DIV
        }

#ifdef __aarch64__
        static inline float64x2_t doit(float64x2_t x) noexcept {
          return vdivq_f64(vdupq_n_f64(1.), vsqrtq_f64(x));
        }
#endif  // __aarch64__
#endif  // __ARM_NEON

#ifdef __ALTIVEC__
#ifndef __VSX__
        static inline vector float doit(vector float x) noexcept {
          // Altivec has no instruction for division, fallback to scalar to ensure IEEE compliance
          alignas(alignof(vector float)) float A[4];
          vec_st(x, 0, A);
          for (int i = 0; i < 4; i++) {
            A[i] = 1.f / __builtin_sqrtf(A[i]);
          }
          return vec_ld(0, A);
        }
#else   // __VSX__
        static inline vector float doit(vector float x) noexcept {
          vector float vone = {1.f, 1.f, 1.f, 1.f};
          return vec_div(vone, vec_sqrt(x));
        }
        static inline vector double doit(vector double x) noexcept {
          vector double vone = {1., 1.};
          return vec_div(vone, vec_sqrt(x));
        }
#endif  // __VSX__
#endif  // __ALTIVEC__
      };

      // bit trick helper
      struct bit_trick;

      template <>
      struct RSQRT<bit_trick> : RSQRT<ieee> {
        using RSQRT<ieee>::doit;

        /**
         * Fast reciprocal Square Root bit trick
         * For more information:
         * https://en.wikipedia.org/wiki/Fast_inverse_square_root
         * https://cs.uwaterloo.ca/~m32rober/rsqrt.pdf
         */
        static constexpr std::uint32_t magic_f32 = 0x5f3759dfu;
        static constexpr std::uint64_t magic_f64 = 0x5fe6eb50c7b537a9ull;

        static inline float doit(float x) noexcept {
          union { float f; std::uint32_t u; } caster;
          caster.f = x;
          caster.u = magic_f32 - (caster.u >> 1);
          return caster.f;
        }
        static inline double doit(double x) noexcept {
          union { double f; std::uint64_t u; } caster;
          caster.f = x;
          caster.u = magic_f64 - (caster.u >> 1);
          return caster.f;
        }

#ifdef __SSE2__
        static inline __m128 doit(__m128 x) noexcept {
          __m128i vmagic = _mm_set1_epi32(magic_f32);
          return _mm_castsi128_ps(_mm_sub_epi32(vmagic, _mm_srli_epi32(_mm_castps_si128(x), 1)));
        }
        static inline __m128d doit(__m128d x) noexcept {
          __m128i vmagic = _mm_set1_epi64x(magic_f64);
          return _mm_castsi128_pd(_mm_sub_epi64(vmagic, _mm_srli_epi64(_mm_castpd_si128(x), 1)));
        }
#endif  // __SSE2__
#ifdef __AVX__
        static inline __m256 doit(__m256 x) noexcept {
          // Integer operations on __m256i only available with AVX2
#ifdef __AVX2__
          __m256i vmagic = _mm256_set1_epi32(magic_f32);
          __m256  y = _mm256_castsi256_ps(_mm256_sub_epi32(vmagic, _mm256_srli_epi32(_mm256_castps_si256(x), 1)));
#else   // !__AVX2__
          __m128i vmagic = _mm_set1_epi32(magic_f32);
          __m128  X0 = _mm256_castps256_ps128(x);
          __m128  X1 = _mm256_extractf128_ps(x, 1);
          __m128  Y0 = _mm_castsi128_ps(_mm_sub_epi32(vmagic, _mm_srli_epi32(_mm_castps_si128(X0), 1)));
          __m128  Y1 = _mm_castsi128_ps(_mm_sub_epi32(vmagic, _mm_srli_epi32(_mm_castps_si128(X1), 1)));
          __m256  y = _mm256_insertf128_ps(_mm256_castps128_ps256(Y0), Y1, 0x1);
#endif  // !__AVX2__
          return y;
        }
        static inline __m256d doit(__m256d x) noexcept {
          // Integer operations on __m256i only available with AVX2
#ifdef __AVX2__
          __m256i vmagic = _mm256_set1_epi64x(magic_f64);
          __m256d y = _mm256_castsi256_pd(_mm256_sub_epi64(vmagic, _mm256_srli_epi64(_mm256_castpd_si256(x), 1)));
#else   // !__AVX2__
          __m128i vmagic = _mm_set1_epi64x(magic_f64);
          __m128d X0 = _mm256_castpd256_pd128(x);
          __m128d X1 = _mm256_extractf128_pd(x, 1);
          __m128d Y0 = _mm_castsi128_pd(_mm_sub_epi64(vmagic, _mm_srli_epi64(_mm_castpd_si128(X0), 1)));
          __m128d Y1 = _mm_castsi128_pd(_mm_sub_epi64(vmagic, _mm_srli_epi64(_mm_castpd_si128(X1), 1)));
          __m256d y = _mm256_insertf128_pd(_mm256_castpd128_pd256(Y0), Y1, 0x1);
#endif  // !__AVX2__
          return y;
        }
#endif  // __AVX__
#ifdef __AVX512F__
        static inline __m512 doit(__m512 x) noexcept {
          __m512i vmagic = _mm512_set1_epi32(magic_f32);
          return _mm512_castsi512_ps(_mm512_sub_epi32(vmagic, _mm512_srli_epi32(_mm512_castps_si512(x), 1)));
        }
        static inline __m512d doit(__m512d x) {
          __m512i vmagic = _mm512_set1_epi64(magic_f64);
          return _mm512_castsi512_pd(_mm512_sub_epi64(vmagic, _mm512_srli_epi64(_mm512_castpd_si512(x), 1)));
        }
#endif  // __AVX512F__
      };

      // As accurate as possible
      template <>
      struct RSQRT<accurate> : RSQRT<ieee> {
        // Usually same as IEEE, except when _mm512_rsqrt28_ps is available
        using RSQRT<ieee>::doit;

        static inline float  doit(float  x) noexcept { return accurate_rsqrt(x); }
        static inline double doit(double x) noexcept { return accurate_rsqrt(x); }

#ifdef __AVX512ER__
        static inline __m512 doit(__m512 x) noexcept { return _mm512_rsqrt28_ps(x); }
#endif  // __AVX512ER__

#if defined(__ARM_NEON) && !defined(__ARM_FEATURE_DIV) && !defined(__aarch64__)
        static inline float32x2_t doit(float32x2_t x) noexcept {
          float32x2_t y = vrsqrte_f32(x);
          y = helper_rsqrt_nr(x, y);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
        static inline float32x4_t doit(float32x4_t x) noexcept {
          float32x4_t y = vrsqrteq_f32(x);
          y = helper_rsqrt_nr(x, y);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
#endif  // __ARM_NEON && !__ARM_FEATURE_DIV && !__aarch64__
#if defined(__ALTIVEC__) && !defined(__VSX__)
        static inline vector float doit(vector float x) noexcept {
          vector float y = vec_rsqrte(x);
          y = helper_rsqrt_nr(x, y);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
#endif  // __ALTIVEC__ && !__VSX__
      };


      // When the whole range of double precision float is needed
      template <>
      struct RSQRT<fast> : RSQRT<accurate> {
        // Fallback to accurate
        using RSQRT<accurate>::doit;

        static inline float  doit(float  x) noexcept { return fast_rsqrt(x); }
        static inline double doit(double x) noexcept { return fast_rsqrt(x); }

        // SSE
#ifdef __SSE2__
        static inline __m128 doit(__m128 x) noexcept {
          __m128 y = _mm_rsqrt_ps(x);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
        // Newton-Raphson is too slow on double precision -> fallback to accurate
#endif  // __SSE2__


        // AVX
#ifdef __AVX__
        static inline __m256 doit(__m256 x) noexcept {
          __m256 y = _mm256_rsqrt_ps(x);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
        static inline __m256d doit(__m256d x) noexcept {
          __m256d y = RSQRT<bit_trick>::doit(x);
          y = helper_rsqrt_nr(x, y);
          y = helper_rsqrt_nr(x, y);
          y = helper_rsqrt_nr(x, y);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
#endif  // __AVX__


        // AVX512
#ifdef __AVX512F__
        // No need to override single precision for AVX512ER
#ifndef __AVX512ER__
        static inline __m512 doit(__m512 x) noexcept {
          __m512 y = _mm512_rsqrt14_ps(x);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
#endif  // !__AVX512ER__
        static inline __m512d doit(__m512d x) noexcept {
#ifdef __AVX512ER__
          __m512d y = _mm512_rsqrt28_pd(x);
#else   // !__AVX512ER__
          __m512d y = _mm512_rsqrt14_pd(x);
          y = helper_rsqrt_nr(x, y);
#endif  // !__AVX512ER__
          y = helper_rsqrt_nr(x, y);
          return y;
        }
#endif  // __AVX512F__

#ifdef __ARM_NEON
        static inline float32x2_t doit(float32x2_t x) noexcept {
          float32x2_t y = vrsqrte_f32(x);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
        static inline float32x4_t doit(float32x4_t x) noexcept {
          float32x4_t y = vrsqrteq_f32(x);
          y = helper_rsqrt_nr(x, y);
          return y;
        }

#ifdef __aarch64__
        static inline float64x2_t doit(float64x2_t x) noexcept {
          float64x2_t y = vrsqrteq_f64(x);
          y = helper_rsqrt_nr(x, y);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
#endif  // __aarch64__
#endif  // __ARM_NEON

#ifdef __ALTIVEC__
#ifndef __VSX__
        static inline vector float doit(vector float x) noexcept {
          vector float y = vec_rsqrte(x);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
#else   // __VSX__
        static inline vector float doit(vector float x) noexcept {
          return vec_rsqrt(x);
        }
        static inline vector double doit(vector double x) noexcept {
          return vec_rsqrt(x);
        }
#endif  // __VSX__
#endif  // __ALTIVEC__
      };


      // Less accurate (no full range on double precision
      template <>
      struct RSQRT<faster> : RSQRT<fast> {
        // Fallback to fast
        using RSQRT<fast>::doit;

#if defined(__FMA__) || defined(__AVX512F__)
        static inline float  doit(float  x) noexcept {
          float y = RSQRT<bit_trick>::doit(x);
          y = helper_rsqrt_nr(x, y);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
#else
        static inline float  doit(float  x) noexcept { return faster_rsqrt(x); }
#endif

#if !defined(__SSE2__) || defined(__AVX__)
        // On SSE, better use fast here
        static inline double doit(double x) noexcept { return faster_rsqrt(x); }
#endif

        // SSE
#ifdef __SSE2__
        static inline __m128d doit(__m128d x) noexcept {
          __m128d y = _mm_cvtps_pd(_mm_rsqrt_ps(_mm_cvtpd_ps(x)));
          y = helper_rsqrt_nr(x, y);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
#endif  // __SSE2__

        // AVX
#ifdef __AVX__
        static inline __m256d doit(__m256d x) noexcept {
          __m256d y = _mm256_cvtps_pd(_mm_rsqrt_ps(_mm256_cvtpd_ps(x)));
          y = helper_rsqrt_nr(x, y);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
#endif  // __AVX__
      };

      template <>
      struct RSQRT<fastest> : RSQRT<faster> {
        // Fallback to faster
        using RSQRT<faster>::doit;

#if !defined(__AVX__) || defined(__AVX2__)
        static inline float  doit(float  x) noexcept {
          float y = RSQRT<bit_trick>::doit(x);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
#else
        static inline float  doit(float  x) noexcept { return fastest_rsqrt(x); }
#endif
        static inline double doit(double x) noexcept { return fastest_rsqrt(x); }

        // SSE: just call _mm_rsqrt_ps
#ifdef __SSE2__
        static inline __m128 doit(__m128 x) noexcept {
          return _mm_rsqrt_ps(x);
        }
        static inline __m128d doit(__m128d x) noexcept {
          return _mm_cvtps_pd(_mm_rsqrt_ps(_mm_cvtpd_ps(x)));
        }
#endif  // __SSE2__

        // AVX: _mm256_rsqrt_ps
#ifdef __AVX__
        static inline __m256 doit(__m256 x) noexcept {
          return _mm256_rsqrt_ps(x);
        }
        static inline __m256d doit(__m256d x) noexcept {
          return _mm256_cvtps_pd(_mm_rsqrt_ps(_mm256_cvtpd_ps(x)));
        }
#endif  // __AVX__

        // AVX512: _mm512_rsqrt14/28_p*
#ifdef __AVX512F__
#ifndef __AVX512ER__
        static inline __m512 doit(__m512 x) noexcept {
          return _mm512_rsqrt14_ps(x);
        }
#endif  // !__AVX512ER__
        static inline __m512d doit(__m512d x) noexcept {
#ifndef __AVX512ER__
          return _mm512_rsqrt14_pd(x);
#else   // __AVX512ER__
          return _mm512_rsqrt28_pd(x);
#endif  // __AVX512ER__
        }
#endif  // __AVX512F__


#ifdef __ARM_NEON
        static inline float32x2_t doit(float32x2_t x) noexcept {
          return vrsqrte_f32(x);
        }
        static inline float32x4_t doit(float32x4_t x) noexcept {
          return vrsqrteq_f32(x);
        }

#ifdef __aarch64__
        static inline float64x2_t doit(float64x2_t x) noexcept {
          return vrsqrteq_f64(x);
        }
#endif  // __aarch64__
#endif  // __ARM_NEON


#ifdef __ALTIVEC__
        static inline vector float doit(vector float x) noexcept {
          return vec_rsqrte(x);
        }
#ifdef __VSX__
        static inline vector double doit(vector double x) noexcept {
          return vec_rsqrte(x);
        }
#endif  // __VSX__
#endif  // __ALTIVEC__
      };


      template <class Tag, class T>
      static inline T helper_rsqrt(T x) noexcept(noexcept(RSQRT<Tag>::doit(x))) {
        return RSQRT<Tag>::doit(x);
      }
#ifndef LHCB_MATH_RSQRT_IMPL
#ifdef VCL_NAMESPACE
      using namespace VCL_NAMESPACE;
#endif
      // VCL
#ifdef __SSE2__
      template <class Tag>
      static inline Vec4f helper_rsqrt(Vec4f x) noexcept {
        return helper_rsqrt<Tag>(static_cast<__m128>(x));
      }
      template <class Tag>
      static inline Vec2d helper_rsqrt(Vec2d x) noexcept {
        return helper_rsqrt<Tag>(static_cast<__m128d>(x));
      }
      static inline Vec4f helper_rsqrt_nr(Vec4f x, Vec4f y) noexcept {
        return helper_rsqrt_nr(static_cast<__m128>(x), static_cast<__m128>(y));
      }
      static inline Vec2d helper_rsqrt_nr(Vec2d x, Vec2d y) noexcept {
        return helper_rsqrt_nr(static_cast<__m128d>(x), static_cast<__m128d>(y));
      }
#endif  // __SSE2__
#if defined(__AVX__) && MAX_VECTOR_SIZE >= 256
      template <class Tag>
      static inline Vec8f helper_rsqrt(Vec8f x) noexcept {
        return helper_rsqrt<Tag>(static_cast<__m256>(x));
      }
      template <class Tag>
      static inline Vec4d helper_rsqrt(Vec4d x) noexcept {
        return helper_rsqrt<Tag>(static_cast<__m256d>(x));
      }
      static inline Vec8f helper_rsqrt_nr(Vec8f x, Vec8f y) noexcept {
        return helper_rsqrt_nr(static_cast<__m256>(x), static_cast<__m256>(y));
      }
      static inline Vec4d helper_rsqrt_nr(Vec4d x, Vec4d y) noexcept {
        return helper_rsqrt_nr(static_cast<__m256d>(x), static_cast<__m256d>(y));
      }
#endif  // __AVX__
#if defined(__AVX512F__) && MAX_VECTOR_SIZE >= 512
      template <class Tag>
      static inline Vec16f helper_rsqrt(Vec16f x) noexcept {
        return helper_rsqrt<Tag>(static_cast<__m512>(x));
      }
      template <class Tag>
      static inline Vec8d helper_rsqrt(Vec8d x) noexcept {
        return helper_rsqrt<Tag>(static_cast<__m512d>(x));
      }
      static inline Vec16f helper_rsqrt_nr(Vec16f x, Vec16f y) noexcept {
        return helper_rsqrt_nr(static_cast<__m512>(x), static_cast<__m512>(y));
      }
      static inline Vec8d helper_rsqrt_nr(Vec8d x, Vec8d y) noexcept {
        return helper_rsqrt_nr(static_cast<__m512d>(x), static_cast<__m512d>(y));
      }
#endif  // __AVX512F__
      // Vc
      template <class Tag, class T, class ABI>
      static inline Vc::Vector<T, ABI> helper_rsqrt(Vc::Vector<T, ABI> x) noexcept {
        x.data() = helper_rsqrt<Tag>(x.data());
        return x;
      }
      template <class T, class ABI>
      static inline Vc::Vector<T, ABI> helper_rsqrt_nr(Vc::Vector<T, ABI> x, Vc::Vector<T, ABI> y) noexcept {
        y.data() = helper_rsqrt_nr(x.data(), y.data());
        return y;
      }
#endif

      template <class T, class Tag>
      struct is_noexcept {
        static constexpr bool rsqrt    = noexcept(helper_rsqrt<Tag>(std::declval<T>()));
        static constexpr bool rsqrt_nr = noexcept(helper_rsqrt_nr(std::declval<T>(), std::declval<T>()));
      };

    } // namespace details

    /**
     * Call the implementation
     */
    template <class Tag, class T>
    inline T rsqrt(T x) noexcept(details::is_noexcept<T, Tag>::rsqrt) {
      return details::helper_rsqrt<Tag>(x);
    }

    template <class T>
    inline T rsqrt_nr(T x, T y) noexcept(details::is_noexcept<T>::rsqrt_nr) {
      return details::helper_rsqrt_nr(x, y);
    }
  }  // namespace Math
}  // namespace LHCb

// undef all macros
#ifndef LHCB_MATH_RSQRT_IMPL
#undef LHCB_MATH_RSQRT_MANGLED_ACCURATE
#undef LHCB_MATH_RSQRT_MANGLED_IEEE
#undef LHCB_MATH_RSQRT_MANGLED_FAST
#undef LHCB_MATH_RSQRT_MANGLED_FASTER
#undef LHCB_MATH_RSQRT_MANGLED_FASTEST
#endif


#endif  // !FAST_MATH_RSQRT_H
