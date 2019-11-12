/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once
#include <array>
#include <string_view>
#include <tuple>
#include <vector>

namespace LHCb {
  namespace detail {
    /** constexpr std::array concatenation
     *
     *  Adapted from https://cern.ch/go/98kd
     */
    template <typename... Arrs>
    constexpr auto array_concat( Arrs const&... arrs ) {
      std::size_t                                                                                     cur{0};
      std::array<std::common_type_t<typename Arrs::value_type...>, ( std::tuple_size_v<Arrs> + ... )> result;
      (
          [&]( auto const& arr ) {
            for ( auto const& elem : arr ) { result[cur++] = elem; }
          }( arrs ),
          ... );
      return result;
    }
  } // namespace detail

  /** Wrapper around std::array<std::string_view, N> that supports constexpr
   *  concatenation.
   */
  template <std::size_t N>
  class string_array {
    std::array<std::string_view, N> m_strings;

    template <std::size_t N1, std::size_t N2>
    friend constexpr string_array<N1 + N2> operator+( string_array<N1> const&, string_array<N2> const& );

  public:
    template <typename... Args>
    constexpr string_array( Args&&... args ) : m_strings{std::forward<Args>( args )...} {}
    constexpr auto begin() const { return m_strings.begin(); }
    constexpr auto end() const { return m_strings.end(); }
    constexpr auto size() const { return m_strings.size(); }
  };

  template <class... U>
  string_array( U... )->string_array<sizeof...( U )>;

  template <std::size_t N1, std::size_t N2>
  constexpr string_array<N1 + N2> operator+( string_array<N1> const& lhs, string_array<N2> const& rhs ) {
    return detail::array_concat( lhs.m_strings, rhs.m_strings );
  }

  template <std::size_t N>
  constexpr auto operator+( string_array<N> const& lhs, std::string_view rhs ) {
    return lhs + string_array{rhs};
  }

  template <std::size_t N>
  constexpr auto operator+( std::string_view lhs, string_array<N> const& rhs ) {
    return string_array{lhs} + rhs;
  }

  /** Helper that can be specialised by different event model types to
   *  advertise which headers must be included to use them.
   */
  template <typename T, typename U = void>
  struct header_map {};

  /** Shorthand for using LHCb::header_map<T>::value
   */
  template <typename T>
  inline constexpr auto header_map_v = header_map<T>::value;
} // namespace LHCb

/** Register the given header as being that defining the given key type.
 *  This must be called at global scope.
 */
#define REGISTER_HEADER( KeyType, Header )                                                                             \
  template <>                                                                                                          \
  struct LHCb::header_map<KeyType> {                                                                                   \
    constexpr static string_array value{Header};                                                                       \
  }

/** Provide some generic definitions for STL types. Generally these definitions
 *  can live in the relevant headers.
 */
template <typename T, typename A>
struct LHCb::header_map<std::vector<T, A>> {
  constexpr static auto value = LHCb::header_map_v<T> + "<vector>";
};

/** Specialise for fundamental types as those don't need headers. This means
 *  that missing specialisations for user-defined types will trigger compile
 *  errors.
 */
template <typename T>
struct LHCb::header_map<T, std::enable_if_t<std::is_fundamental_v<T>>> {
  constexpr static string_array value{};
};
