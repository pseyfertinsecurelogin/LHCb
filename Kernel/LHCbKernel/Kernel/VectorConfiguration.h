#pragma once

#include <type_traits>
#include "VectorClass/vectorclass.h"

/**
 * @brief      Helper static methods to expose the SIMD
 *             capabilities of the machine.
 */
struct VectorConfiguration {

/**
 * @brief      Byte width of SIMD architecture.
 */
#if defined(STATIC_VECTOR_BYTEWIDTH)
  static_assert(
    STATIC_VECTOR_BYTEWIDTH >= 8
    && STATIC_VECTOR_BYTEWIDTH <= 64
    && STATIC_VECTOR_BYTEWIDTH && ((STATIC_VECTOR_BYTEWIDTH & (STATIC_VECTOR_BYTEWIDTH - 1)) == 0),
    "Configured vector configuration must be a power of two, between 8 and 64");

  static constexpr std::size_t bytewidth() { return STATIC_VECTOR_BYTEWIDTH; }
#elif defined(__AVX512F__)
  static constexpr std::size_t bytewidth() { return 64; }
#elif defined(__AVX__)
  static constexpr std::size_t bytewidth() { return 32; }
#elif defined(__SSE__)
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
  template<class PRECISION>
  static constexpr std::size_t width() {
    return VectorConfiguration::bytewidth() / sizeof(PRECISION);
  }
};

namespace LHCb {
namespace Vector {

// Vectype selector
template<class T, size_t W> struct Vectype {};
template<class T> struct Vectype<T, 1> { using type = T; using booltype = bool; };
#if defined(__AVX512F__)
template<> struct Vectype<double, 8> { using type = Vec8d; using booltype = Vec8db; };
template<> struct Vectype<float, 16> { using type = Vec16f; using booltype = Vec16fb; };
template<> struct Vectype<int, 16> { using type = Vec16i; using booltype = Vec16ib; };
template<> struct Vectype<long int, 8> { using type = Vec8q; using booltype = Vec8qb; };
template<> struct Vectype<unsigned, 16> { using type = Vec16ui; using booltype = Vec16ib; };
#endif
#if defined(__AVX__)
template<> struct Vectype<double, 4> { using type = Vec4d; using booltype = Vec4db; };
template<> struct Vectype<float, 8> { using type = Vec8f; using booltype = Vec8fb; };
template<> struct Vectype<int, 8> { using type = Vec8i; using booltype = Vec8ib; };
template<> struct Vectype<long int, 4> { using type = Vec4q; using booltype = Vec4qb; };
template<> struct Vectype<unsigned, 8> { using type = Vec8ui; using booltype = Vec8ib; };
#endif
#if defined(__SSE__)
template<> struct Vectype<double, 2> { using type = Vec2d; using booltype = Vec2db; };
template<> struct Vectype<float, 4> { using type = Vec4f; using booltype = Vec4fb; };
template<> struct Vectype<int, 4> { using type = Vec4i; using booltype = Vec4ib; };
template<> struct Vectype<long int, 2> { using type = Vec2q; using booltype = Vec2qb; };
template<> struct Vectype<unsigned, 4> { using type = Vec4ui; using booltype = Vec4ib; };
#endif

}
}
