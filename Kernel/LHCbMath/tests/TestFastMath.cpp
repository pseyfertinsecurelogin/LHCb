/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include <utility>
#include <type_traits>
#include <iostream>
#include <array>
#include <random>
#include <cmath>
#include <limits>
#include <chrono>

#include "LHCbMath/FastMaths.h"
#include "vdt/sqrt.h"

using namespace LHCb::Math;

/**
 * Remove some stupid warnings
 */
#ifdef __INTEL_COMPILER
// both inline and noinline
#pragma warning disable 2196
#endif

#ifdef __SSE2__
#define RDTSC
#endif


/******************************************************************************/
/* globals                                                                    */
/******************************************************************************/
static long long repeat = 1e6;
static long long tries  = 4;
static int failed = 0;
static int warning = 0;
static int total  = 0;


/******************************************************************************/
/* unroll                                                                     */
/******************************************************************************/
template <class F>
void static inline __attribute((always_inline)) helper_unroll(std::integral_constant<unsigned, 0>, F&&) {}
template <unsigned I, class F>
void static inline __attribute((always_inline)) helper_unroll(std::integral_constant<unsigned, I>, F&& f) {
  helper_unroll(std::integral_constant<unsigned, I-1>{}, f);
  f(I - 1);
}

template <unsigned N, class F>
void static inline __attribute((always_inline)) unroll(F&& f) {
  helper_unroll(std::integral_constant<unsigned, N>{}, f);
}


/******************************************************************************/
/* type_name                                                                  */
/******************************************************************************/
const char* helper_type_name(float *) { return "float" ; }
const char* helper_type_name(double*) { return "double"; }
#ifdef __SSE2__
const char* helper_type_name(__m128 *) { return "__m128" ; }
const char* helper_type_name(__m128d*) { return "__m128d"; }
const char* helper_type_name(Vec4f  *) { return "Vec4f"  ; }
const char* helper_type_name(Vec2d  *) { return "Vec2d"  ; }
#endif
#ifdef __AVX__
const char* helper_type_name(__m256 *) { return "__m256" ; }
const char* helper_type_name(__m256d*) { return "__m256d"; }
#if MAX_VECTOR_SIZE >= 256
const char* helper_type_name(Vec8f  *) { return "Vec8f"  ; }
const char* helper_type_name(Vec4d  *) { return "Vec4d"  ; }
#endif
#endif
#ifdef __AVX512F__
const char* helper_type_name(__m512 *) { return "__m512" ; }
const char* helper_type_name(__m512d*) { return "__m512d"; }
#if MAX_VECTOR_SIZE >= 512
const char* helper_type_name(Vec16f *) { return "Vec16f" ; }
const char* helper_type_name(Vec8d  *) { return "Vec8d"  ; }
#endif
#endif
#ifdef __ARM_NEON
const char* helper_type_name(float32x2_t*) { return "float32x2_t"; }
const char* helper_type_name(float32x4_t*) { return "float32x4_t"; }
#ifdef __aarch64__
const char* helper_type_name(float64x2_t*) { return "float64x2_t"; }
#endif
#endif
#ifdef __ALTIVEC__
const char* helper_type_name(vector float *) { return "vector float" ; }
#ifdef __VSX__
const char* helper_type_name(vector double*) { return "vector double"; }
#endif
#endif

// Vc
const char* helper_type_name(Vc::float_v *) { return "Vc::float_v" ; }
const char* helper_type_name(Vc::double_v*) { return "Vc::double_v"; }

template <class T>
const char* type_name() { return helper_type_name(static_cast<T*>(nullptr)); }


/******************************************************************************/
/* inner_type                                                                 */
/******************************************************************************/
float  helper_inner_type(float *);
double helper_inner_type(double*);
#ifdef __SSE2__
float  helper_inner_type(__m128 *);
double helper_inner_type(__m128d*);
float  helper_inner_type(Vec4f  *);
double helper_inner_type(Vec2d  *);
#endif
#ifdef __AVX__
float  helper_inner_type(__m256 *);
double helper_inner_type(__m256d*);
#if MAX_VECTOR_SIZE >= 256
float  helper_inner_type(Vec8f  *);
double helper_inner_type(Vec4d  *);
#endif
#endif
#ifdef __AVX512F__
float  helper_inner_type(__m512 *);
double helper_inner_type(__m512d*);
#if MAX_VECTOR_SIZE >= 512
float  helper_inner_type(Vec16f *);
double helper_inner_type(Vec8d  *);
#endif
#endif
#ifdef __ARM_NEON
float  helper_inner_type(float32x2_t*);
float  helper_inner_type(float32x4_t*);
#ifdef __aarch64__
double helper_inner_type(float64x2_t*);
#endif
#endif
#ifdef __ALTIVEC__
float  helper_inner_type(vector float *);
#ifdef __VSX__
double helper_inner_type(vector double*);
#endif
#endif

// Vc
template <class T, class ABI>
typename Vc::Vector<T, ABI>::value_type helper_inner_type(Vc::Vector<T, ABI>*);

template <class T>
auto inner_type() -> decltype(helper_inner_type(static_cast<T*>(nullptr)));

template <class T>
constexpr bool is_scalar() { return std::is_same<T, decltype(inner_type<T>())>::value; }


/******************************************************************************/
/* keep                                                                       */
/******************************************************************************/
template <class T>
void static inline __attribute((always_inline)) keep(T& r) noexcept {
  asm volatile ("" : "+m"(r));
}

#ifdef __x86_64__
void static inline __attribute((always_inline)) keep(float& r) noexcept {
  asm volatile ("" : "+v"(r));
}
void static inline __attribute((always_inline)) keep(double& r) noexcept {
  asm volatile ("" : "+v"(r));
}

void static inline __attribute((always_inline)) keep(__m128& r) noexcept {
  asm volatile ("" : "+v"(r));
}
void static inline __attribute((always_inline)) keep(__m128d& r) noexcept {
  asm volatile ("" : "+v"(r));
}
#endif

#ifdef __AVX__
void static inline __attribute((always_inline)) keep(__m256& r) noexcept {
  asm volatile ("" : "+v"(r));
}
void static inline __attribute((always_inline)) keep(__m256d& r) noexcept {
  asm volatile ("" : "+v"(r));
}
#endif

#ifdef __AVX512F__
void static inline __attribute((always_inline)) keep(__m512& r) noexcept {
  asm volatile ("" : "+v"(r));
}
void static inline __attribute((always_inline)) keep(__m512d& r) noexcept {
  asm volatile ("" : "+v"(r));
}
#endif

#ifdef __ARM_NEON
void static inline __attribute((always_inline)) keep(float& r) noexcept {
  asm volatile ("" : "+w"(r));
}
void static inline __attribute((always_inline)) keep(double& r) noexcept {
  asm volatile ("" : "+w"(r));
}
void static inline __attribute((always_inline)) keep(float32x2_t& r) noexcept {
  asm volatile ("" : "+w"(r));
}
void static inline __attribute((always_inline)) keep(float32x4_t& r) noexcept {
  asm volatile ("" : "+w"(r));
}
#ifdef __aarch64__
void static inline __attribute((always_inline)) keep(float64x2_t& r) noexcept {
  asm volatile ("" : "+w"(r));
}
#endif
#endif

#ifdef __ALTIVEC__
void static inline __attribute((always_inline)) keep(float& r) noexcept {
  asm volatile ("" : "+f"(r));
}
void static inline __attribute((always_inline)) keep(double& r) noexcept {
  asm volatile ("" : "+d"(r));
}
#ifndef __VSX__
void static inline __attribute((always_inline)) keep(vector float& r) noexcept {
  asm volatile ("" : "+v"(r));
}
#else
void static inline __attribute((always_inline)) keep(vector float& r) noexcept {
  asm volatile ("" : "+wa"(r));
}
void static inline __attribute((always_inline)) keep(vector double& r) noexcept {
  asm volatile ("" : "+wa"(r));
}
#endif
#endif

// Vc
template <class T, class ABI>
void static inline __attribute((always_inline)) keep(Vc::Vector<T, ABI>& r) noexcept {
  keep(r.data());
}


/******************************************************************************/
/* card                                                                       */
/******************************************************************************/
template <class T>
constexpr int card() { return sizeof(T) / sizeof(inner_type<T>()); }

#if defined(__AVX512F__)
constexpr int max_width = 64;
#elif defined(__AVX__)
constexpr int max_width = 32;
#elif defined(__SSE2__) || defined(__ARM_NEON) || defined(__ALTIVEC__)
constexpr int max_width = 16;
#else
constexpr int max_width = 1;
#endif

template <class T>
constexpr int max_card() {
  return max_width < sizeof(T) ? 1 : max_width / sizeof(T);
}


/******************************************************************************/
/* init                                                                       */
/******************************************************************************/
template <class T, class U>
void helper_init(T& t, U&& u) {
  t = std::forward<U>(u);
}

#ifdef __SSE2__
template <class U>
void helper_init(__m128& t, U&& u) {
  t = _mm_set1_ps(u);
}
template <class U>
void helper_init(__m128d& t, U&& u) {
  t = _mm_set1_pd(u);
}
#endif

#ifdef __AVX__
template <class U>
void helper_init(__m256& t, U&& u) {
  t = _mm256_set1_ps(u);
}
template <class U>
void helper_init(__m256d& t, U&& u) {
  t = _mm256_set1_pd(u);
}
#endif

#ifdef __AVX512F__
template <class U>
void helper_init(__m512& t, U&& u) {
  t = _mm512_set1_ps(u);
}
template <class U>
void helper_init(__m512d& t, U&& u) {
  t = _mm512_set1_pd(u);
}
#endif

#ifdef __ARM_NEON
template <class U>
void helper_init(float32x2_t& t, U&& u) {
  t = vdup_n_f32(u);
}
template <class U>
void helper_init(float32x4_t& t, U&& u) {
  t = vdupq_n_f32(u);
}
#ifdef __aarch64__
template <class U>
void helper_init(float64x2_t& t, U&& u) {
  t = vdupq_n_f64(u);
}
#endif
#endif

#ifdef __ALTIVEC__
template <class U>
void helper_init(vector float& t, U&& u) {
  t = {u, u, u, u};
}
#ifdef __VSX__
template <class U>
void helper_init(vector double& t, U&& u) {
  t = {u, u};
}
#endif
#endif

template <class T, class U>
T init(U&& u) {
  T t;
  helper_init(t, std::forward<U>(u));
  return t;
}


/******************************************************************************/
/* get vec                                                                    */
/******************************************************************************/
template <class T, int N>
struct helper_get_vec {
  using type = T;
};

#ifdef __SSE2__
template <> struct helper_get_vec<float,  4> { using type = __m128;  };
template <> struct helper_get_vec<double, 2> { using type = __m128d; };
#endif
#ifdef __AVX__
template <> struct helper_get_vec<float,  8> { using type = __m256;  };
template <> struct helper_get_vec<double, 4> { using type = __m256d; };
#endif
#ifdef __AVX512F__
template <> struct helper_get_vec<float, 16> { using type = __m512;  };
template <> struct helper_get_vec<double, 8> { using type = __m512d; };
#endif
#ifdef __ARM_NEON
template <> struct helper_get_vec<float,  2> { using type = float32x2_t; };
template <> struct helper_get_vec<float,  4> { using type = float32x4_t; };
#ifdef __aarch64__
template <> struct helper_get_vec<double, 2> { using type = float64x2_t; };
#endif
#endif
#ifdef __ALTIVEC__
template <> struct helper_get_vec<float,  4> { using type = vector float;  };
#ifdef __VSX__
template <> struct helper_get_vec<double, 2> { using type = vector double; };
#endif
#endif

template <class T, int N>
using get_vec = typename helper_get_vec<T, N>::type;



/******************************************************************************/
/* transform                                                                  */
/******************************************************************************/
template <class T, class Input, class Output, class F>
void helper_Transform(T*, Input in, Input end, Output out, F&& f) {
  while (in != end) {
    *out = f(*in);
    ++in;
    ++out;
  }
}

#ifdef __SSE2__
template <class Input, class Output, class F>
void helper_Transform(__m128*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    _mm_storeu_ps(&*out, f(_mm_loadu_ps(&*in)));
    in  += 4;
    out += 4;
  }
}
template <class Input, class Output, class F>
void helper_Transform(__m128d*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    _mm_storeu_pd(&*out, f(_mm_loadu_pd(&*in)));
    in  += 2;
    out += 2;
  }
}
template <class Input, class Output, class F>
void helper_Transform(Vec4f*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    f(Vec4f{}.load(&*in)).store(&*out);
    in  += 4;
    out += 4;
  }
}
template <class Input, class Output, class F>
void helper_Transform(Vec2d*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    f(Vec2d{}.load(&*in)).store(&*out);
    in  += 2;
    out += 2;
  }
}
#endif

#ifdef __AVX__
template <class Input, class Output, class F>
void helper_Transform(__m256*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    _mm256_storeu_ps(&*out, f(_mm256_loadu_ps(&*in)));
    in  += 8;
    out += 8;
  }
}
template <class Input, class Output, class F>
void helper_Transform(__m256d*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    _mm256_storeu_pd(&*out, f(_mm256_loadu_pd(&*in)));
    in  += 4;
    out += 4;
  }
}
#if MAX_VECTOR_SIZE >= 256
template <class Input, class Output, class F>
void helper_Transform(Vec8f*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    f(Vec8f{}.load(&*in)).store(&*out);
    in  += 8;
    out += 8;
  }
}
template <class Input, class Output, class F>
void helper_Transform(Vec4d*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    f(Vec4d{}.load(&*in)).store(&*out);
    in  += 4;
    out += 4;
  }
}
#endif
#endif

#ifdef __AVX512F__
template <class Input, class Output, class F>
void helper_Transform(__m512*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    _mm512_storeu_ps(&*out, f(_mm512_loadu_ps(&*in)));
    in  += 16;
    out += 16;
  }
}
template <class Input, class Output, class F>
void helper_Transform(__m512d*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    _mm512_storeu_pd(&*out, f(_mm512_loadu_pd(&*in)));
    in  += 8;
    out += 8;
  }
}
#if MAX_VECTOR_SIZE >= 512
template <class Input, class Output, class F>
void helper_Transform(Vec16f*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    f(Vec16f{}.load(&*in)).store(&*out);
    in  += 16;
    out += 16;
  }
}
template <class Input, class Output, class F>
void helper_Transform(Vec8d*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    f(Vec8d{}.load(&*in)).store(&*out);
    in  += 8;
    out += 8;
  }
}
#endif
#endif

#ifdef __ARM_NEON
template <class Input, class Output, class F>
void helper_Transform(float32x2_t*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    vst1_f32(&*out, f(vld1_f32(&*in)));
    in  += 2;
    out += 2;
  }
}
template <class Input, class Output, class F>
void helper_Transform(float32x4_t*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    vst1q_f32(&*out, f(vld1q_f32(&*in)));
    in  += 4;
    out += 4;
  }
}
#ifdef __aarch64__
template <class Input, class Output, class F>
void helper_Transform(float64x2_t*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    vst1q_f64(&*out, f(vld1q_f64(&*in)));
    in  += 2;
    out += 2;
  }
}
#endif
#endif

#ifdef __ALTIVEC__
template <class Input, class Output, class F>
void helper_Transform(vector float*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
#ifndef __VSX__
    vec_st(f(vec_ld(0, &*in)), 0, &*out);
#else
    vec_vsx_st(f(vec_vsx_ld(0, &*in)), 0, &*out);
#endif
    in  += 4;
    out += 4;
  }
}
#ifdef __VSX__
template <class Input, class Output, class F>
void helper_Transform(vector double*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    vec_vsx_st(f(vec_vsx_ld(0, &*in)), 0, &*out);
    in  += 2;
    out += 2;
  }
}
#endif
#endif

// Vc
template <class T, class ABI, class Input, class Output, class F>
void helper_Transform(Vc::Vector<T, ABI>*, Input in, Input end, Output out, F&& f) {
  while (in < end) {
    f(Vc::Vector<T, ABI>(&*in)).store(&*out);
    in  += Vc::Vector<T, ABI>::size();
    out += Vc::Vector<T, ABI>::size();
  }
}

template <class T, class Input, class Output, class F>
void Transform(Input in, Input end, Output out, F&& f) {
  return helper_Transform(static_cast<T*>(nullptr), in, end, out, std::forward<F>(f));
}


/******************************************************************************/
/* extra functions to test                                                    */
/******************************************************************************/
namespace LHCb {
  namespace Math {
    struct naive : accuracy_tag {};
    struct vdt_ : accuracy_tag {};
    struct vdt_approx : accuracy_tag {};
    namespace details {
      // To compare to what people usually write
      template <>
      struct RSQRT<naive> : RSQRT<ieee> {
        using RSQRT<ieee>::doit;
        static float doit(float x) {
          return 1.f / std::sqrt(x);
        }
        static double doit(double x) {
          return 1. / std::sqrt(x);
        }
      };

      // use same algo as vdt for fast_approx
      template <>
      struct RSQRT<vdt_approx> {
        template <class T>
        static T doit(T x) {
          T y = RSQRT<bit_trick>::doit(x);
          if (std::is_same<double, decltype(inner_type<T>())>::value) {
            y = helper_rsqrt_nr(x, y);
            y = helper_rsqrt_nr(x, y);
          }
          y = helper_rsqrt_nr(x, y);
          return y;
        }
      };

      // use same algo as vdt for fast
      template <>
      struct RSQRT<vdt_> {
        template <class T>
        static T doit(T x) {
          T y = RSQRT<vdt_approx>::doit(x);
          y = helper_rsqrt_nr(x, y);
          return y;
        }
      };
    }
  }
}


/******************************************************************************/
/* latency - throughput                                                       */
/******************************************************************************/
template <class T, class F>
double __attribute((noinline)) time(F&& f) {
  using scalar_t = decltype(inner_type<T>());
  constexpr int S = 16384; // 16K
  constexpr int N = S / sizeof(scalar_t);

  alignas(S) scalar_t in[N];
  for (int i = 0; i < N; i++) {
    in[i] = init<scalar_t>(1.23456789e6);
  }

  double dmin = std::numeric_limits<double>::infinity();

  for (long long i = 0; i < tries; i++) {
#ifdef RDTSC
    auto t0 = _rdtsc();
#else
    auto t0 = std::chrono::high_resolution_clock::now();
#endif
    for (int j = 0; j < repeat / N; j++) {
      Transform<T>(+in, +in + N, +in, f);
    }
#ifdef RDTSC
    auto t1 = _rdtsc();
#else
    auto t1 = std::chrono::high_resolution_clock::now();
#endif
    for (int j = 0; j < N; j++) {
      keep(in[j]);
    }
#ifdef RDTSC
    double d = static_cast<double>(t1 - t0) / static_cast<double>(N);
#else
    double d = static_cast<double>(std::chrono::nanoseconds(t1 - t0).count()) / static_cast<double>(N);
#endif
    dmin = dmin < d ? dmin : d;
  }
  return dmin / (repeat / N);
}

/******************************************************************************/
/* Error                                                                      */
/******************************************************************************/
// Compute the error (generic)
template <class Storage, class Compute = Storage>
struct Error {
  Storage min, mean, quad, max;

  Error() = default;
  Error(const Error&) = default;
  Error& operator=(const Error&) = default;
  ~Error() = default;

  template <class F, class E>
  explicit __attribute((noinline)) Error(F&& f, E&& error) {
    constexpr int N = 4096 / sizeof(Storage);
    std::mt19937_64 mt;
    std::uniform_real_distribution<Storage> distribution(1., 4.);

    long long n = 1ll << 25;
    alignas(64) Storage input[N], output[N];

    min =  std::numeric_limits<Storage>::infinity();
    max = -std::numeric_limits<Storage>::infinity();
    mean = 0;
    quad = 0;

    for (long long i = 0; i < n; i += N) {
      for (int j = 0; j < N; j++) {
        input[j] = distribution(mt);
      }
      Transform<Compute>(+input, +input + N, +output, f);
      for (int j = 0; j < N; j++) {
        Storage e = error(input[j], output[j]);
        min = std::min(min, e);
        max = std::max(max, e);
        mean += e;
        quad += e * e;
      }
    }

    mean /= n;
    quad  = std::sqrt(quad / n);
  }
};

// helper to perform a bitwise reinterpret cast
template <class To, class From, class U>
To bit_cast(U&& u) {
  static_assert(sizeof(From) == sizeof(To), "From and To should have the same size");
  union {
    From from;
    To to;
  } caster;
  caster.from = static_cast<From>(u);
  return caster.to;
}

// Compute error (float)
template <class Compute>
struct Error<float, Compute> {
  float min, mean, quad, max;

  Error() = default;
  Error(const Error&) = default;
  Error& operator=(const Error&) = default;
  ~Error() = default;

  template <class F, class E>
  explicit __attribute((noinline)) Error(F&& f, E&& error) {
    constexpr int N = 4096 / sizeof(float);

    long long n = 1ll << 25;
    alignas(64) float input[N], output[N];

    min =  std::numeric_limits<float>::infinity();
    max = -std::numeric_limits<float>::infinity();
    mean = 0;
    quad = 0;

    alignas(64) int iinput[N];
    iinput[0] = bit_cast<int, float>(1.f);
    for (int j = 1; j < N; j++) {
      iinput[j] = iinput[0] + j;
    }

    for (long long i = 0; i < n; i += N) {
      for (int j = 0; j < N; j++) {
        input[j] = bit_cast<float, int>(iinput[j]);
      }
      Transform<Compute>(+input, +input + N, +output, f);
      for (int j = 0; j < N; j++) {
        float e = error(input[j], output[j]);
        min = std::min(min, e);
        max = std::max(max, e);
        mean += e;
        quad += e * e;
      }
      for (int j = 0; j < N; j++) {
        iinput[j] += N;
      }
    }

    mean /= n;
    quad  = std::sqrt(quad / n);
  }
};

// convert a relative error into bits of information
double bits(double e) {
  return e == 0 ? std::numeric_limits<double>::infinity() : -std::log2(e);
}

/******************************************************************************/
/* Tag traits                                                                 */
/******************************************************************************/
template <class Tag>
struct tag_trait;

template <>
struct tag_trait<accurate> {
  static constexpr const char* name = "accurate";
  static constexpr const int max_bit_error = 2;
};
template <>
struct tag_trait<ieee> {
  static constexpr const char* name = "ieee";
  static constexpr const int max_bit_error = 1;
};
template <>
struct tag_trait<fast> {
  static constexpr const char* name = "fast";
  static constexpr const int max_bit_error = 3;
};
template <>
struct tag_trait<faster> {
  static constexpr const char* name = "faster";
  static constexpr const int max_bit_error = 10;
};
template <>
struct tag_trait<fastest> {
  static constexpr const char* name = "fastest";
  static constexpr const int max_bit_error = 1000;
};
template <>
struct tag_trait<naive> {
  static constexpr const char* name = "naive";
  static constexpr const int max_bit_error = 1;
};
template <>
struct tag_trait<vdt_> {
  static constexpr const char* name = "vdt";
  static constexpr const int max_bit_error = 10;
};
template <>
struct tag_trait<vdt_approx> {
  static constexpr const char* name = "vdt_approx";
  static constexpr const int max_bit_error = 1000;
};

/******************************************************************************/
/* CSV output                                                                 */
/******************************************************************************/

// times for a single type
template <class T, class... Tags, class F>
void csv_time(F&&) {
  constexpr int N = sizeof...(Tags);
  double times[N] = {time<T>(typename F::template fn<Tags>{})...};

  std::cout << type_name<T>();
  for (int i = 0; i < N; i++) {
    std::cout << "," << times[i];
  }
  std::cout << std::endl;
}

// accuracies for a single type
template <class Compute, class... Tags, class F>
void csv_accuracy(F&&) {
  constexpr int N = sizeof...(Tags);
  using Storage = decltype(inner_type<Compute>());
  Error<Storage, Compute> errors[N]{
    Error<Storage, Compute>{typename F::template fn<Tags>{}, typename F::error{}}...};

  std::cout << type_name<Compute>();
  for (int i = 0; i < N; i++) {
    std::cout << "," << bits(errors[i].max);
  }
  std::cout << std::endl;
}

// all times
template <template <bool> class F, class... Tags>
void csv_time() {
  constexpr int N = sizeof...(Tags);
  const char *names[N] = {tag_trait<Tags>::name...};

  std::cout << "type";
  for (int i = 0; i < N; i++) {
    std::cout << "," << names[i];
  }
  std::cout << std::endl;

  std::cout << "scalar "; csv_time<float,  Tags...>(F<true>{});
  csv_time<float,  Tags...>(F<false>{});
#ifdef __SSE2__
  csv_time<__m128,  Tags...>(F<false>{});
#endif
#ifdef __AVX__
  csv_time<__m256,  Tags...>(F<false>{});
#endif
#ifdef __AVX512F__
  csv_time<__m512,  Tags...>(F<false>{});
#endif
#ifdef __ARM_NEON
  csv_time<float32x2_t, Tags...>(F<false>{});
  csv_time<float32x4_t, Tags...>(F<false>{});
#endif
#ifdef __ALTIVEC__
  csv_time<vector float,  Tags...>(F<false>{});
#endif
  csv_time<Vc::float_v,  Tags...>(F<false>{});

  std::cout << "scalar "; csv_time<double, Tags...>(F<true>{});
  csv_time<double, Tags...>(F<false>{});
#ifdef __SSE2__
  csv_time<__m128d, Tags...>(F<false>{});
#endif
#ifdef __AVX__
  csv_time<__m256d, Tags...>(F<false>{});
#endif
#ifdef __AVX512F__
  csv_time<__m512d, Tags...>(F<false>{});
#endif
#ifdef __aarch64__
  csv_time<float64x2_t, Tags...>(F<false>{});
#endif
#ifdef __VSX__
  csv_time<vector double, Tags...>(F<false>{});
#endif
  csv_time<Vc::double_v, Tags...>(F<false>{});
}

// all accuracies
template <template <bool> class F, class... Tags>
void csv_accuracy() {
  constexpr int N = sizeof...(Tags);
  const char *names[N] = {tag_trait<Tags>::name...};

  std::cout << "type";
  for (int i = 0; i < N; i++) {
    std::cout << "," << names[i];
  }
  std::cout << std::endl;

  std::cout << "scalar "; csv_accuracy<float,  Tags...>(F<true>{});
  csv_accuracy<float,  Tags...>(F<false>{});
#ifdef __SSE2__
  csv_accuracy<__m128,  Tags...>(F<false>{});
#endif
#ifdef __AVX__
  csv_accuracy<__m256,  Tags...>(F<false>{});
#endif
#ifdef __AVX512F__
  csv_accuracy<__m512,  Tags...>(F<false>{});
#endif
#ifdef __ARM_NEON
  csv_accuracy<float32x2_t, Tags...>(F<false>{});
  csv_accuracy<float32x4_t, Tags...>(F<false>{});
#endif
#ifdef __ALTIVEC__
  csv_accuracy<vector float,  Tags...>(F<false>{});
#endif
  csv_accuracy<Vc::float_v,  Tags...>(F<false>{});

  std::cout << "scalar "; csv_accuracy<double, Tags...>(F<true>{});
  csv_accuracy<double, Tags...>(F<false>{});
#ifdef __SSE2__
  csv_accuracy<__m128d, Tags...>(F<false>{});
#endif
#ifdef __AVX__
  csv_accuracy<__m256d, Tags...>(F<false>{});
#endif
#ifdef __AVX512F__
  csv_accuracy<__m512d, Tags...>(F<false>{});
#endif
#ifdef __aarch64__
  csv_accuracy<float64x2_t, Tags...>(F<false>{});
#endif
#ifdef __VSX__
  csv_accuracy<vector double, Tags...>(F<false>{});
#endif
  csv_accuracy<Vc::double_v, Tags...>(F<false>{});
}


/******************************************************************************/
/* Checks                                                                     */
/******************************************************************************/

// Tolerances
static constexpr float quad_error_tol = 1.01; //  1%
static constexpr float  max_error_tol = 1.01; //  1%
static constexpr double      time_tol = 1.03; //  3%
static constexpr double crit_time_tol = 1.30; // 30%
static constexpr int min_bit_accuracy = 4;

template <class F, class T, class Tag>
struct time_functor {
  static double call() {
    return time<T>(typename F::template fn<Tag>{});
  }
};

// Timing
template <class T, class... Tags, class F>
void check_times(F&&) {
  constexpr int N = sizeof...(Tags);
  const char *names[N] = {tag_trait<Tags>::name...};
  std::intptr_t ids[N] = {F::template fn<Tags>::template id<T>()...};
  double (*fns[])() = {&time_functor<F, T, Tags>::call...};

  double times[N];
  std::intptr_t last_id = 0;
  for (int i = 0; i < N; i++) {
    if (ids[i] != last_id) {
      times[i] = fns[i]();
      last_id = ids[i];
    } else {
      times[i] = times[i-1];
    }
  }

  const char* best_name = names[0];
  double      best_time = times[0];

#ifdef RDTSC
  const char* unit = "c";
#else
  const char* unit = "ns";
#endif

  for (int i = 1; i < N; i++) {
    if (times[i] > best_time * time_tol) {
      std::cout << "Test failed:\t";
      std::cout << F::name << "<" << names[i]  << ">(" << type_name<T>() << ")";
      std::cout << " is slower than ";
      std::cout << F::name << "<" << best_name << ">(" << type_name<T>() << ")";
      std::cout << ":\t" << times[i] << " " << unit << " / " << best_time << " " << unit;

      if (times[i] > best_time * crit_time_tol) {
        failed++;
        std::cout << "\t(error)";
      } else {
        std::cout << "\t(warning)";
        warning++;
      }
      std::cout << std::endl;
    }
    total++;

    if (times[i] < best_time) {
      best_time = times[i];
      best_name = names[i];
    }
  }
}

template <class F, class Storage, class Compute, class Tag>
struct error_functor {
  static Error<Storage, Compute> call() {
    return Error<Storage, Compute>{typename F::template fn<Tag>{}, typename F::error{}};
  }
};

// Accuracy
template <class Compute, class... Tags, class F>
void check_accuracies(F&&) {
  constexpr int N = sizeof...(Tags);
  const char *names[N] = {tag_trait<Tags>::name...};
  std::intptr_t ids[N] = {F::template fn<Tags>::template id<Compute>()...};
  using Storage = decltype(inner_type<Compute>());

  Error<Storage, Compute> (*fns[N])() = {&error_functor<F, Storage, Compute, Tags>::call...};

  // Compute errors
  Error<Storage, Compute> errors[N];
  std::intptr_t last_id = 0;
  for (int i = 0; i < N; i++) {
    if (ids[i] != last_id) {
      errors[i] = fns[i]();
      last_id = ids[i];
    } else {
      errors[i] = errors[i-1];
    }
  }

  // accuracy requirement
  int required[N] = {(std::numeric_limits<Storage>::digits - tag_trait<Tags>::max_bit_error)...};

  // Check absolute accuracies
  for (int i = 0; i < N; i++) {
    required[i] = std::max(required[i], min_bit_accuracy);
    double b = bits(errors[i].max);

    if (b < required[i]) {
      std::cout << "Test failed:\t";
      std::cout << F::name << "<" << names[i] << ">(" << type_name<Compute>() << ")";
      std::cout << " is less accurate than expected";
      std::cout << ":\tmin accuracy " << b << " / " << required[i] << "\t(error)" << std::endl;

      failed++;
    }
    ++total;
  }

  // Check accuracy order
  const char* max_name = names[0];
  Storage max_error = errors[0].max;

  const char* max_quad_name = names[0];
  Storage max_quad_error = errors[0].quad;

  for (int i = 1; i < N; i++) {
    if (max_error > errors[i].max * max_error_tol) {
      std::cout << "Test failed:\t";
      std::cout << F::name << "<" << max_name << ">(" << type_name<Compute>() << ")";
      std::cout << " is less accurate than ";
      std::cout << F::name << "<" << names[i]  << ">(" << type_name<Compute>() << ")";
      std::cout << ":\tmin accuracy " << bits(max_error) << " / " << bits(errors[i].max)  << "\t(error)" << std::endl;

      failed++;
    }
    total++;

    if (max_quad_error > errors[i].quad * quad_error_tol) {
      std::cout << "Test failed:\t";
      std::cout << F::name << "<" << max_quad_name << ">(" << type_name<Compute>() << ")";
      std::cout << " is less accurate than ";
      std::cout << F::name << "<" << names[i]       << ">(" << type_name<Compute>() << ")";
      std::cout << ":\tmean accuracy " << bits(max_quad_error) << " / " << bits(errors[i].quad)  << "\t(error)" << std::endl;

      failed++;
    }
    total++;

    if (max_error < errors[i].max) {
      max_error = errors[i].max;
      max_name  = names[i];
    }
    if (max_quad_error < errors[i].quad) {
      max_quad_error = errors[i].quad;
      max_quad_name  = names[i];
    }
  }
}

// Both timing and accuracy
template <template <bool> class F, class T>
void check() {
  if (is_scalar<T>()) {
    check_accuracies<T, naive, accurate, ieee, fast, faster, fastest>(F<true>{});
    check_accuracies<T, naive, vdt_, vdt_approx>(F<true>{});

#ifdef __OPTIMIZE__
    check_times<T, naive, ieee, accurate, fast, /*faster,*/ fastest>(F<true>{});
    //check_times<T, vdt_approx, fastest>(F<true>{});
    //check_times<T, vdt_, faster>(F<true>{});
#endif
  }

  check_accuracies<T, naive, accurate, ieee, fast, faster, fastest>(F<false>{});
  check_accuracies<T, naive, vdt_, vdt_approx>(F<false>{});

#ifdef __OPTIMIZE__
  check_times<T, naive, ieee, accurate, fast, faster, fastest>(F<false>{});
  check_times<T, vdt_approx, fastest>(F<false>{});
  check_times<T, vdt_, faster>(F<false>{});
#endif
}

// All checks for a single function
template <template <bool> class F>
void check_all() {
#ifdef VCL_NAMESPACE
  using namespace VCL_NAMESPACE
#endif
  check<F, float >();
  check<F, double>();

#ifdef __SSE2__
  check<F, __m128 >();
  check<F, __m128d>();
  check<F, Vec4f>();
  check<F, Vec2d>();
#endif
#ifdef __AVX__
  check<F, __m256 >();
  check<F, __m256d>();
#if MAX_VECTOR_SIZE >= 256
  check<F, Vec8f>();
  check<F, Vec4d>();
#endif
#endif
#ifdef __AVX512F__
  check<F, __m512 >();
  check<F, __m512d>();
#if MAX_VECTOR_SIZE >= 512
  check<F, Vec16f>();
  check<F, Vec8d>();
#endif
#endif
#ifdef __ARM_NEON
  check<F, float32x2_t>();
  check<F, float32x4_t>();
#ifdef __aarch64__
  check<F, float64x2_t>();
#endif
#endif
#ifdef __ALTIVEC__
  check<F, vector float >();
#ifdef __VSX__
  check<F, vector double>();
#endif
#endif

  // Vc
  check<F, Vc::float_v >();
  check<F, Vc::double_v>();
}

/******************************************************************************/
/* Functions to test                                                          */
/******************************************************************************/

template <bool force = true>
struct RSQRT {
  static constexpr const char* name = force ? "scalar rsqrt" : "rsqrt";
  template <class Tag>
  struct fn {
    template <class T>
    inline T __attribute((always_inline)) operator()(T x) const {
      if (force) keep(x);
      T y = LHCb::Math::rsqrt<Tag>(x);
      if (force) keep(y);
      return y;
    }

    template <class T>
    static std::intptr_t id() {
      constexpr int c = card<T>();
      using vec = get_vec<decltype(inner_type<T>()), c == 1 ? max_card<T>() : c>;
#if !defined(__AVX__) || defined(__AVX2__)
      if (std::is_same<T, float>::value && std::is_same<Tag, LHCb::Math::fastest>::value) {
        return RSQRT<force>::template fn<LHCb::Math::vdt_approx>::template id<float>();
      }
#endif
#if defined(__FMA__) || defined(__AVX512F__)
      if (std::is_same<T, float>::value && std::is_same<Tag, LHCb::Math::faster>::value) {
        return RSQRT<force>::template fn<LHCb::Math::vdt_>::template id<float>();
      }
#endif
#if defined(__SSE2__) && !defined(__AVX__)
      if (std::is_same<T, double>::value && std::is_same<Tag, LHCb::Math::faster>::value) {
        return RSQRT<force>::template fn<LHCb::Math::fast>::template id<double>();
      }
#endif
      return reinterpret_cast<std::intptr_t>(static_cast<vec (*)(vec)>(&LHCb::Math::details::RSQRT<Tag>::doit));
    }
  };

  struct error {
    template <class T>
    T operator()(T input, T output) const {
      T square = output * output;
      keep(square);
      return std::abs(T(1) - (input * square)) * T(0.5);
    }
  };
};

/******************************************************************************/
/* main                                                                       */
/******************************************************************************/
int main(int argc, char *argv[]) {
#ifdef RDTSC
  repeat = 16384;
  tries  = 1e5;
#else
  repeat = 1e6;
  tries  = 1e3;
#endif
  failed = 0;
  total  = 0;

  if (argc > 1 && std::string(argv[1]) == "csv") {
    std::cout << "times" << std::endl;
    csv_time<RSQRT, naive, ieee, accurate, fast, faster, fastest, vdt_, vdt_approx>();
    std::cout << "accuracies" << std::endl;
    csv_accuracy<RSQRT, naive, ieee, accurate, fast, faster, fastest, vdt_, vdt_approx>();
  } else {
    check_all<RSQRT>();
  }

  if (failed > 0 || warning > 0) {
    std::cout << "\nFAILED: " << failed + warning << "/" << total;
    if (warning > 0) {
      std::cout << "\t(warnings: " << warning << "/" << failed+warning << ")";
    }
    std::cout << std::endl;
    if (failed > 0) return 1;
    int max_warning = 2;
    if (warning > max_warning) {
      std::cout << "FAILURE: Too many warnings ( > " << max_warning << " )";
      return 1;
    }
    return 0;
  }
  std::cout << "All tests passed" << std::endl;
  return 0;
}
