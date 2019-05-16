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
/** @file FastMaths.h
 *
 * Implementation of various fast mathematical functions with both
 * scalar and SIMD support.
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
#include "LHCbMath/FastMaths-SIMD-ve.h"
#include "LHCbMath/FastMaths-SIMD-Vc.h"
#include "LHCbMath/FastMaths-Scalar.h"

// namespace documentation

/** @namespace Gaudi::Math::Approx
 *
 *  Namespace for fast approximations to various math functions.
 *
 *  These implementations are less precise than those above, but consequently
 *  varying degress faster (depending on how approximate you go).
 *  Use with care, but in the right case can give good gains.
 *
 *  Based in part on the code at :-
 *
 *  http://www.machinedlearnings.com/2011/06/fast-approximate-logarithm-exponential.html
 *
 *  http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html
 *
 * https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   30/11/2017
 */

namespace LHCb::Math {

  namespace details {
    template <class T, class Tag = void>
    struct is_noexcept;
  }

  //------------------------------------------------------------------------------
  /**
   * @brief Base class to all accuracy requirement tags
   *
   * Accuracy requirement tags are used to relax the accuracy requirement of a function in order to
   * be faster. See the documentation of the function to know what are the accuracy guarantees.
   *
   * The default accuracy requirement is @ref accurate
   */
  struct accuracy_tag {};

  /**
   * @brief As accurate as efficiently possible.
   *
   * This is the default accuracy requirement.
   * It is designed to be at least as accurate as @ref ieee while being at least as fast as @ref
   * ieee.
   *
   * @note
   * Currently, results may differ between @ref accurate and @ref ieee only on KNL
   * where `_mm_rsqrt28_ps` can compute 1/sqrt(x) with all bits correct;
   * and on ARM32 where there is no Neon instruction to divide or to take a square root.
   */
  struct accurate : accuracy_tag {};

  /**
   * @brief strict IEEE754 compliance
   *
   * This accuracy requirement should be used when you want reproducible results
   * accross compilers, machines, and architectures.
   *
   * @warning
   * On ARM 32bits and PowerPC 32bits, IEEE754 cannot be achieved with SIMD instructions
   * Thus, a fallback using scalar instructions is used on these architectures
   */
  struct ieee : accuracy_tag {};

  /**
   * @brief Relaxed strict accuracy constraint for fast execution
   *
   * This accuracy requirement should give an error less than 2 bits.
   * This usually enables faster execution than @ref accurate.
   *
   * @note
   * In some cases, the @ref fast implementation is the same as the @ref accurate one.
   * In those cases, no timing or accuracy differences could be observed.
   */
  struct fast : accuracy_tag {};

  /**
   * @brief Faster execution, good accuracy
   *
   * This accuracy requirement usually gives good results.
   * It is guaranted to be at least as fast as @ref fast.
   *
   * @note
   * It might use the same implementation as @ref fast if making it faster would give too bad
   * accuracy.
   *
   * @warning
   * The full range of the double precision floating point is not covered with some implementations.
   * Use this accuracy requirement only if the input fits within the range of a single precision
   * `float` (abs in [10^-38, 10^38]). Otherwise, this is undefined behaviour.
   */
  struct faster : accuracy_tag {};

  /**
   * @brief Rough estimation, super fast
   *
   * No constraint on accuracy. It usually results in very rough estimation, but is super fast.
   *
   * @warning
   * The full range of the double precision floating point is not covered with some implementations.
   * Use this accuracy requirement only if the input fits within the range of a single precision
   * `float` (abs in [10^-38, 10^38]). Otherwise, this is undefined behaviour.
   */
  struct fastest : accuracy_tag {};

  /**
   * @brief Computes the reciprocal square root of `x`: 1/sqrt(x).
   *
   * The accuracy of the result depends on template parameter @link accuracy_tag Tag @endlink.
   *
   * Guaranted accuracy in bits:
   * Tag           | `float` | `double`
   * --------------|:-------:|:-------:
   * @ref accurate | 23      | 52
   * @ref ieee     | 23      | 52
   * @ref fast     | 22      | 51
   * @ref faster   | 17      | 44
   * @ref fastest  |  9      | 12
   *
   *
   * @tparam Tag Specifies the accuracy of the result (default: @ref accurate)
   * @tparam T   Any floating point type or SIMD type
   * @param x
   * @return 1/sqrt(x)
   *
   * @note
   * When called with scalar (either `float` or `double`),
   * the compiler is able to use a vectorized variant of the function.
   *
   * @warning
   * This function takes only normal positve numbers.
   * Calling it with 0, negatives, infinity, NaNs or subnormals is undefined behaviour
   *
   * @parblock @b Examples
   *
   * @code{.cpp}
   * // simple use
   * float y = fast_math::rsqrt(x);
   * @endcode
   *
   * @code{.cpp}
   * // simple use with accuracy requirement
   * float y = fast_math::rsqrt<fast_math::ieee>(x);
   * @endcode
   *
   * @code{.cpp}
   * // simple use with accuracy requirement and imported namespace
   * using namespace fast_math;
   * float y = rsqrt<ieee>(x);
   * @endcode
   *
   * @code{.cpp}
   * // In loop use (gets auto-vectorized nicely)
   * using namespace fast_math;
   * for (int i = 0; i < n; i++) {
   *   A[i] = rsqrt<faster>(B[i]);
   * }
   * @endcode
   *
   * @code{.cpp}
   * // use intrinsic SIMD type
   * using namespace fast_math;
   * __m128 Y = rsqrt(_mm_load_ps(p));
   * @endcode
   */
  template <class Tag = accurate, class T>
  inline T rsqrt( T x ) noexcept( details::is_noexcept<T, Tag>::rsqrt );

  /**
   * @brief Refines an approximation of 1/sqrt(x).
   *
   * Uses a Newton-Raphson iteration to increase the accuracy
   *   of an approximation of 1/sqrt(x).
   *
   * Convergence is quadratic (doubles the number of correct bits)
   *
   * @param x
   * @param y approximation of 1/sqrt(x)
   * @return 1/sqrt(x)
   */
  template <class T>
  inline T rsqrt_nr( T x, T y ) noexcept( details::is_noexcept<T>::rsqrt_nr );

} // namespace LHCb::Math

// If not compiling the implementation, load everything needed
#ifndef LHCB_MATH_RSQRT_IMPL
#  ifdef __SSE2__
#    include "vectorclass.h"
#  endif
#endif

// include rsqrt implementation
#include "LHCbMath/fast_math_rsqrt.icpp"

namespace LHCb::Math {

#ifndef LHCB_MATH_RSQRT_IMPL

  /** fast inverse sqrt for SIMD float type
   *  call rsqrt */
  inline LHCb::SIMD::VC::FPF fast_isqrt( const LHCb::SIMD::VC::FPF x ) noexcept {
    // Qualified Lookup is mandatory for Vc types (ADL would find Vc::rsqrt)
    return LHCb::Math::rsqrt<fast>( x );
  }

  /** fast approximate inverse sqrt for SIMD float type
   *  call rsqrt */
  inline LHCb::SIMD::VC::FPF fast_approx_isqrt( const LHCb::SIMD::VC::FPF x ) noexcept {
    // Qualified Lookup is mandatory for Vc types (ADL would find Vc::rsqrt)
    return LHCb::Math::rsqrt<fastest>( x );
  }

  /// Fast inverse sqrt
  inline float fast_isqrt( const float x ) noexcept { return rsqrt<fast>( x ); }
  /// Fast inverse sqrt
  inline double fast_isqrt( const double x ) noexcept { return rsqrt<fast>( x ); }

  /// Fast approximate inverse sqrt
  inline float fast_approx_isqrt( const float x ) noexcept { return rsqrt<fastest>( x ); }
  /// Fast approximate inverse sqrt
  inline double fast_approx_isqrt( const double x ) noexcept { return rsqrt<faster>( x ); }

#endif

} // namespace LHCb::Math
