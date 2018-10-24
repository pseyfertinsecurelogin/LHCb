/*****************************************************************************\
* (c) Copyright 2015-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
/** @file SOATypelist.h
 *
 * @author Manuel Schiller <Manuel.Schiller@cern.ch>
 * @date 2015-04-10
 * - initial related
 *
 * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
 * @date 2016-11-17
 * - rewrite to use better coding, more C++11 constructs
 */

#ifndef SOATYPELIST_H
#define SOATYPELIST_H

#include <tuple>
#include <cstdint>
#include <type_traits>

#include "c++14_compat.h"

/// namespace to encapsulate SOA stuff
namespace SOA {
    /// namespace for typelist type used by Container and related utilities
    namespace Typelist {
        namespace typelist_impl {
            /// sum up an empty index_sequence
            constexpr std::size_t sum(std::index_sequence<>) noexcept
            { return 0; }
            /// sum up an index_sequence
            template <std::size_t HEAD, std::size_t... TAIL>
            constexpr std::size_t sum(std::index_sequence<HEAD, TAIL...>) noexcept
            { return HEAD + sum(std::index_sequence<TAIL...>{}); }
            /// minimum of an empty index_sequence
            constexpr std::size_t min(std::index_sequence<>) noexcept
            { return -1; }
            /// minimum of an index_sequence
            template <std::size_t HEAD, std::size_t... TAIL>
            constexpr std::size_t min(std::index_sequence<HEAD, TAIL...>) noexcept
            {
                return HEAD <= min(std::index_sequence<TAIL...>{}) ? HEAD :
                    min(std::index_sequence<TAIL...>{});
            }
            /// return index_sequence: IDX if NEEDLE in TL at IDX, else -1
            template <typename NEEDLE, typename TL, std::size_t... IDXS>
            constexpr static std::index_sequence<(std::is_same<
                NEEDLE, typename TL::template at<IDXS>::type>::value ?
                IDXS : std::size_t(-1))...>
            _idx_if_found(std::index_sequence<IDXS...>) noexcept
            { return {}; }
        } // namespace typelist_impl

        /// a very simple type list
        template <typename... ARGS>
        struct typelist {
            /// return if typelist is empty
            constexpr static bool empty() noexcept
            { return 0 == sizeof...(ARGS); }
            /// return number of elements in typelist
            constexpr static std::size_t size() noexcept
            { return sizeof...(ARGS); }
            /// get type at index IDX
            template <std::size_t IDX>
            struct at : std::tuple_element<
                        IDX, std::tuple<ARGS...> > {};
            /// count how often T occurs in typelist
            template <typename T>
            constexpr static std::size_t count() noexcept
            {
                return typelist_impl::sum(std::index_sequence<
                        std::is_same<T, ARGS>::value...>{});
            }
            /// find index of first occurrence of T, -1 otherwise
            template <typename T>
            constexpr static std::size_t find() noexcept
            {
                return typelist_impl::min(
                        typelist_impl::_idx_if_found<T, typelist<ARGS...> >(
                            std::make_index_sequence<size()>()));
            }
            /// little helper to map over the types in the typelist
            template <template <typename ARG> class OP>
            using map_t = typelist<OP<ARGS>...>;
        };

        /// check basic properties to validate implementation
        namespace __impl_compile_time_tests {
            template <typename T> using add_const_t = const T;
            static_assert(typelist<>::empty(), "implementation error");
            static_assert(typelist<>::size() == 0, "implementation error");
            static_assert(typelist<>::find<float>() == std::size_t(-1),
                    "implementation error");
            static_assert(!typelist<int>::empty(), "implementation error");
            static_assert(typelist<int>::size() == 1, "implementation error");
            static_assert(std::is_same<typelist<int>::at<0>::type, int>::value,
                    "implementation error");
#if !defined(__clang__) && !defined(INTEL_COMPILER) && (\
        __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 8))
            static_assert(std::is_same<typelist<const int>,
                    typelist<int>::map_t<add_const_t> >::value,
                    "implementation error");
#endif
            static_assert(typelist<int>::find<int>() == 0, "implementation error");
            static_assert(typelist<int>::find<float>() == std::size_t(-1),
                    "implementation error");
            static_assert(!typelist<int, bool>::empty(), "implementation error");
            static_assert(typelist<int, bool>::size() == 2, "implementation error");
            static_assert(std::is_same<typelist<int, bool>::at<0>::type, int>::value,
                    "implementation error");
            static_assert(std::is_same<typelist<int, bool>::at<1>::type, bool>::value,
                    "implementation error");
            static_assert(typelist<int, bool>::find<int>() == 0, "implementation error");
            static_assert(typelist<int, bool>::find<bool>() == 1, "implementation error");
#if !defined(__clang__) && !defined(INTEL_COMPILER) && (\
        __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 8))
            static_assert(std::is_same<typelist<const int, const bool>,
                    typelist<int, bool>::map_t<add_const_t> >::value,
                    "implementation error");
#endif
            static_assert(0 == typelist<int, int, bool>::template count<char>(),
                    "implementation error");
            static_assert(1 == typelist<int, int, bool>::template count<bool>(),
                    "implementation error");
            static_assert(2 == typelist<int, int, bool>::template count<int>(),
                    "implementation error");
        }
    } // namespace Typelist
} // namespace SOA

#endif // SOATYPELIST_H

// vim: sw=4:tw=78:ft=cpp:et
