#pragma once

#include <type_traits>

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
