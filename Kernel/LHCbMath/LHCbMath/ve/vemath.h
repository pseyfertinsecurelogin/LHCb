/* @file include/vemath.h
 *
 * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
 */
#pragma once

#include "ve.h"

#include <cmath>
#include <limits>
#include <utility>

namespace ve {
    /// the absolute value of a vector of floating point quantities
    template <typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_floating_point<typename V::value_type>::value>
    abs(const V& v) noexcept
    {
        using mask_t = typename V::mask_type::value_type;
        return V(typename V::from_mask(),
                 v.as_mask() & ((~mask_t(0)) >> mask_t(1)));
    }
    /// the absolute value of a vector of signed integer quantities
    template <typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V,
            std::is_integral<typename V::value_type>::value &&
                    std::is_signed<typename V::value_type>::value>
    abs(const V& v) noexcept
    {
        return sel(v < static_cast<typename V::value_type>(0), -v, v);
    }
    /// the absolute value of a vector of unsigned integer quantities (noop)
    template <typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V,
            std::is_integral<typename V::value_type>::value &&
                    std::is_unsigned<typename V::value_type>::value>
    abs(const V& v) noexcept
    {
        return v;
    }

    /// copy sign from second to first argument (floating point)
    template <typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_floating_point<typename V::value_type>::value>
    copysign(const V& v, const V& w) noexcept
    {
        using mask_t = typename V::mask_type::value_type;
        constexpr mask_t mask = (~mask_t(0)) >> mask_t(1);
        return V(typename V::from_mask(), (v & mask) | (w & ~mask));
    }
    /// copy sign from second to first argument (signed integer)
    template <typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V,
            std::is_integral<typename V::value_type>::value &&
                    std::is_signed<typename V::value_type>::value>
    copysign(const V& v, const V& w) noexcept
    {
        return sel(w < static_cast<typename V::value_type>(0), -abs(v),
                   abs(v));
    }
    /// copy sign from second to first argument (unsigned integer, noop)
    template <typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V,
            std::is_integral<typename V::value_type>::value &&
                    std::is_unsigned<typename V::value_type>::value>
    copysign(const V& v, const V& /* unused */) noexcept
    {
        return v;
    }

    /* Here are a couple of notes on how the inplementation of square, cubic
     * and fifth roots work internally:
     *
     * Most of the routines here are based on what is known as the "fast
     * inverse square root trick". The trick exploits that the bit pattern of
     * a floating point number, when interpreted as an integer, is (up to a
     * couple of tweaks) a good approximation of its logarithm. One thus
     * proceeds as follows:
     *
     * - reinterpret float/double as int32_t/int64_t
     * - add/subtract multiple of integer to/from magic constant
     * - reinterpret int32_t/int64_t as float/double
     * - run a couple of iterations of Newton's method to refine the guess
     *
     * The magic number can be calculated as
     *
     * (1 - p) * M * (B - 0.0450465)
     *
     * where M = 2^23 for float and M = 2^52 for double, and B = 127 for float
     * and B = 1023 for double, and p is the power to which a number should be
     * raised (p in [-1, 1]).  To compute a cube root in float, the magic
     * number is 2/3 * 2^23 * (127 - 0.0450465) = 0x2a517d47; the first guess
     * of the cube root is thus obtained as the float one gets from the bit
     * pattern of 0x2a517d47 + reinterpret_cast<int32_t>(x) / 3.
     *
     * By refining with a couple of iterations of Newton's method, this crude
     * approximation can be improved to the level that the floating point type
     * will allow.
     */

    /** @brief square root for vectors of floats
     *
     * By default, the routine differs from the true answer by 8 ULP or less.
     * The computation accuracy can be traded for speed by increasing FAST
     * from its default value of 0. At FAST = 3, the algorithm starts with a
     * percent level guess, and each decrease in FAST adds one round of root
     * polishing with a step of Newton's method, roughly doubling the number
     * of correct digits.
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2019-04-17
     */
    template <unsigned FAST = 0, typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_same<typename V::value_type, float>::value>
    sqrt(const V& x) noexcept
    {
        const V xa = abs(x);
        V r(typename V::from_mask(), 0x1fbd1df5 + (xa.as_mask() >> 1));
        if (FAST < 3) r -= (r - xa / r) / 2;
        if (FAST < 2) r -= (r - xa / r) / 2;
        if (FAST < 1) r -= (r - xa / r) / 2;
        return sel(x >= 0, r, V(std::numeric_limits<float>::quiet_NaN()));
    }
    /** @brief square root for vectors of doubles
     *
     * By default, the routine differs from the true answer by 8 ULP or less.
     * The computation accuracy can be traded for speed by increasing FAST
     * from its default value of 0. At FAST = 4, the algorithm starts with a
     * percent level guess, and each decrease in FAST adds one round of root
     * polishing with a step of Newton's method, roughly doubling the number
     * of correct digits.
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2019-04-17
     */
    template <unsigned FAST = 0, typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_same<typename V::value_type, double>::value>
    sqrt(const V& x) noexcept
    {
        const V xa = abs(x);
        V r(typename V::from_mask(), 0x1ff7a3bea91d9b1b + (xa.as_mask() >> 1));
        if (FAST < 4) r -= (r - xa / r) / 2;
        if (FAST < 3) r -= (r - xa / r) / 2;
        if (FAST < 2) r -= (r - xa / r) / 2;
        if (FAST < 1) r -= (r - xa / r) / 2;
        return sel(x >= 0, r, V(std::numeric_limits<double>::quiet_NaN()));
    }

    /** @brief inverse square root for vectors of floats
     *
     * By default, the routine differs from the true answer by 8 ULP or less.
     * The computation accuracy can be traded for speed by increasing FAST
     * from its default value of 0. At FAST = 3, the algorithm starts with a
     * percent level guess, and each decrease in FAST adds one round of root
     * polishing with a step of Newton's method, roughly doubling the number
     * of correct digits.
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2019-04-17
     */
    template <unsigned FAST = 0, typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_same<typename V::value_type, float>::value>
    invsqrt(const V& x) noexcept
    {
        const V xa = abs(x);
        V r(typename V::from_mask(), 0x5f3759df - (xa.as_mask() >> 1));
        if (FAST < 3) r += (r - xa * (r * r * r)) / 2;
        if (FAST < 2) r += (r - xa * (r * r * r)) / 2;
        if (FAST < 1) r += (r - xa * (r * r * r)) / 2;
        return sel(x >= 0, r, V(std::numeric_limits<float>::quiet_NaN()));
    }
    /** @brief inverse square root for vectors of doubles
     *
     * By default, the routine differs from the true answer by 8 ULP or less.
     * The computation accuracy can be traded for speed by increasing FAST
     * from its default value of 0. At FAST = 4, the algorithm starts with a
     * percent level guess, and each decrease in FAST adds one round of root
     * polishing with a step of Newton's method, roughly doubling the number
     * of correct digits.
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2019-04-17
     */
    template <unsigned FAST = 0, typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_same<typename V::value_type, double>::value>
    invsqrt(const V& x) noexcept
    {
        const V xa = abs(x);
        V r(typename V::from_mask(), 0x5fe6eb3bfb58d152 - (xa.as_mask() >> 1));
        if (FAST < 4) r += (r - xa * (r * r * r)) / 2;
        if (FAST < 3) r += (r - xa * (r * r * r)) / 2;
        if (FAST < 2) r += (r - xa * (r * r * r)) / 2;
        if (FAST < 1) r += (r - xa * (r * r * r)) / 2;
        return sel(x >= 0, r, V(std::numeric_limits<double>::quiet_NaN()));
    }

    /** @brief cubic root for vectors of floats
     *
     * By default, the routine differs from the true answer by 8 ULP or less.
     * The computation accuracy can be traded for speed by increasing FAST
     * from its default value of 0. At FAST = 3, the algorithm starts with a
     * percent level guess, and each decrease in FAST adds one round of root
     * polishing with a step of Newton's method, roughly doubling the number
     * of correct digits.
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2019-04-17
     */
    template <unsigned FAST = 0, typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_same<typename V::value_type, float>::value>
    cbrt(const V& x) noexcept
    {
        const V xa = abs(x);
        V r(typename V::from_mask(), 0x2a517d47 + xa.as_mask() / 3);
        if (FAST < 3) r -= (r - xa / (r * r)) / 3;
        if (FAST < 2) r -= (r - xa / (r * r)) / 3;
        if (FAST < 1) r -= (r - xa / (r * r)) / 3;
        return copysign(r, x);
    }
    /** @brief cubic root for vectors of doubles
     *
     * By default, the routine differs from the true answer by 8 ULP or less.
     * The computation accuracy can be traded for speed by increasing FAST
     * from its default value of 0. At FAST = 4, the algorithm starts with a
     * percent level guess, and each decrease in FAST adds one round of root
     * polishing with a step of Newton's method, roughly doubling the number
     * of correct digits.
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2019-04-17
     */
    template <unsigned FAST = 0, typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_same<typename V::value_type, double>::value>
    cbrt(const V& x) noexcept
    {
        const V xa = abs(x);
        V r(typename V::from_mask(), 0x2a9f84fe36d22425 + xa.as_mask() / 3);
        if (FAST < 4) r -= (r - xa / (r * r)) / 3;
        if (FAST < 3) r -= (r - xa / (r * r)) / 3;
        if (FAST < 2) r -= (r - xa / (r * r)) / 3;
        if (FAST < 1) r -= (r - xa / (r * r)) / 3;
        return copysign(r, x);
    }

    /** @brief inverse cubic root for vectors of floats
     *
     * By default, the routine differs from the true answer by 8 ULP or less.
     * The computation accuracy can be traded for speed by increasing FAST
     * from its default value of 0. At FAST = 3, the algorithm starts with a
     * percent level guess, and each decrease in FAST adds one round of root
     * polishing with a step of Newton's method, roughly doubling the number
     * of correct digits.
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2019-04-17
     */
    template <unsigned FAST = 0, typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_same<typename V::value_type, float>::value>
    invcbrt(const V& x) noexcept
    {
        const V xa = abs(x);
        V r(typename V::from_mask(), 0x54a2fa8e - xa.as_mask() / 3);
        if (FAST < 3) r += (r - xa * ((r * r) * (r * r))) / 3;
        if (FAST < 2) r += (r - xa * ((r * r) * (r * r))) / 3;
        if (FAST < 1) r += (r - xa * ((r * r) * (r * r))) / 3;
        return copysign(r, x);
    }
    /** @brief inverse cubic root for vectors of doubles
     *
     * By default, the routine differs from the true answer by 8 ULP or less.
     * The computation accuracy can be traded for speed by increasing FAST
     * from its default value of 0. At FAST = 4, the algorithm starts with a
     * percent level guess, and each decrease in FAST adds one round of root
     * polishing with a step of Newton's method, roughly doubling the number
     * of correct digits.
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2019-04-17
     */
    template <unsigned FAST = 0, typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_same<typename V::value_type, double>::value>
    invcbrt(const V& x) noexcept
    {
        const V xa = abs(x);
        V r(typename V::from_mask(), 0x553f09fc6da44849 - xa.as_mask() / 3);
        if (FAST < 4) r += (r - xa * ((r * r) * (r * r))) / 3;
        if (FAST < 3) r += (r - xa * ((r * r) * (r * r))) / 3;
        if (FAST < 2) r += (r - xa * ((r * r) * (r * r))) / 3;
        if (FAST < 1) r += (r - xa * ((r * r) * (r * r))) / 3;
        return copysign(r, x);
    }

    /** @brief fifth root for vectors of float
     *
     * By default, the routine differs from the true answer by 8 ULP or less.
     * The computation accuracy can be traded for speed by increasing FAST
     * from its default value of 0. At FAST = 3, the algorithm starts with a
     * percent level guess, and each decrease in FAST adds one round of root
     * polishing with a step of Newton's method, roughly doubling the number
     * of correct digits.
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2019-04-17
     */
    template <unsigned FAST = 0, typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_same<typename V::value_type, float>::value>
    fifthroot(const V& x) noexcept
    {
        const V xa = abs(x);
        V r(typename V::from_mask(), 0x32c82fee + xa.as_mask() / 5);
        if (FAST < 3) r -= (r - xa / ((r * r) * (r * r))) / 5;
        if (FAST < 2) r -= (r - xa / ((r * r) * (r * r))) / 5;
        if (FAST < 1) r -= (r - xa / ((r * r) * (r * r))) / 5;
        return copysign(r, x);
    }
    /** @brief fifth root for vectors of doubles
     *
     * By default, the routine differs from the true answer by 8 ULP or less.
     * The computation accuracy can be traded for speed by increasing FAST
     * from its default value of 0. At FAST = 4, the algorithm starts with a
     * percent level guess, and each decrease in FAST adds one round of root
     * polishing with a step of Newton's method, roughly doubling the number
     * of correct digits.
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2019-04-17
     */
    template <unsigned FAST = 0, typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_same<typename V::value_type, double>::value>
    fifthroot(const V& x) noexcept
    {
        const V xa = abs(x);
        V r(typename V::from_mask(), 0x3325d2caa82f5e92 + xa.as_mask() / 5);
        if (FAST < 4) r -= (r - xa / ((r * r) * (r * r))) / 5;
        if (FAST < 3) r -= (r - xa / ((r * r) * (r * r))) / 5;
        if (FAST < 2) r -= (r - xa / ((r * r) * (r * r))) / 5;
        if (FAST < 1) r -= (r - xa / ((r * r) * (r * r))) / 5;
        return copysign(r, x);
    }

    /** @brief inverse fifth root for vectors of floats
     *
     * By default, the routine differs from the true answer by 8 ULP or less.
     * The computation accuracy can be traded for speed by increasing FAST
     * from its default value of 0. At FAST = 3, the algorithm starts with a
     * percent level guess, and each decrease in FAST adds one round of root
     * polishing with a step of Newton's method, roughly doubling the number
     * of correct digits.
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2019-04-17
     */
    template <unsigned FAST = 0, typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_same<typename V::value_type, float>::value>
    invfifthroot(const V& x) noexcept
    {
        const V xa = abs(x);
        V r(typename V::from_mask(), 0x4c2c47e6 - xa.as_mask() / 5);
        if (FAST < 3) r += (r - xa * ((r * r) * (r * r) * (r * r))) / 5;
        if (FAST < 2) r += (r - xa * ((r * r) * (r * r) * (r * r))) / 5;
        if (FAST < 1) r += (r - xa * ((r * r) * (r * r) * (r * r))) / 5;
        return copysign(r, x);
    }
    /** @brief inverse fifth root for vectors of doubles
     *
     * By default, the routine differs from the true answer by 8 ULP or less.
     * The computation accuracy can be traded for speed by increasing FAST
     * from its default value of 0. At FAST = 4, the algorithm starts with a
     * percent level guess, and each decrease in FAST adds one round of root
     * polishing with a step of Newton's method, roughly doubling the number
     * of correct digits.
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2019-04-17
     */
    template <unsigned FAST = 0, typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_same<typename V::value_type, double>::value>
    invfifthroot(const V& x) noexcept
    {
        const V xa = abs(x);
        V r(typename V::from_mask(), 0x4cb8bc2ffc470ddb - xa.as_mask() / 5);
        if (FAST < 4) r += (r - xa * ((r * r) * (r * r) * (r * r))) / 5;
        if (FAST < 3) r += (r - xa * ((r * r) * (r * r) * (r * r))) / 5;
        if (FAST < 2) r += (r - xa * ((r * r) * (r * r) * (r * r))) / 5;
        if (FAST < 1) r += (r - xa * ((r * r) * (r * r) * (r * r))) / 5;
        return copysign(r, x);
    }

    /// FIXME: document, write unit tests
    template <typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_same<typename V::value_type, float>::value>
    hypot_fast(const V& x, const V& y) noexcept
    {
        const V xa = abs(x), ya = abs(y);
        return 0.96043387010341996525f * max(xa, ya) +
               0.39782473475931601382f * min(xa, ya);
    }
    template <typename V>
    constexpr inline detail::enable_if_vector_t<
            V, V, std::is_same<typename V::value_type, double>::value>
    hypot_fast(const V& x, const V& y) noexcept
    {
        const V xa = abs(x), ya = abs(y);
        return 0.96043387010341996525 * max(xa, ya) +
               0.39782473475931601382 * min(xa, ya);
    }

} // namespace ve

/* Copyright (C) CERN for the benefit of the LHCb collaboration
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */

// vim: sw=4:tw=78:ft=cpp:et
