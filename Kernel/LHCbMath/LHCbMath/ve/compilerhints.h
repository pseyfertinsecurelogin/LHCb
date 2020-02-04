/** @file compilerhints.h
 *
 * @brief hints to the compiler on supported compilers
 *
 * @author Rainer Schwemmer
 * @author Daniel Campora
 * @author Manuel Schiller
 * @date 2018-02-06
 *
 * For copyright and license information, see the end of the file.
 */
#pragma once

#include <cassert>

// if somebody defined these macros before us, clean up
#if defined(NOINLINE)
#undef NOINLINE
#endif
#if defined(UNREACHABLE)
#undef UNREACHABLE
#endif
#if defined(LIKELY)
#undef LIKELY
#endif
#if defined(UNLIKELY)
#undef UNLIKELY
#endif
#if defined(ASSUME)
#undef ASSUME
#endif
#if defined(PURE)
#undef PURE
#endif

// define our own versions, depending on compiler
#if defined(__GNUC__)
#define NOINLINE __attribute__((noinline))
#define UNREACHABLE() __builtin_unreachable()
#define LIKELY(x) __builtin_expect(static_cast<bool>((x)), true)
#define UNLIKELY(x) __builtin_expect(static_cast<bool>((x)), false)
#define PURE __attribute__((pure))
#define ASSUME(x)                                                            \
    (static_cast<bool>((x)) ? static_cast<void>(0) : __builtin_unreachable())
#else
#define NOINLINE
#define UNREACHABLE() assert(false)
#define LIKELY(x) (static_cast<bool>((x)))
#define UNLIKELY(x) (static_cast<bool>((x)))
#define PURE
#define ASSUME(x) assert(x)
#endif

#include <type_traits>

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

