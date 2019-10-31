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
#pragma once
#include "GaudiKernel/detected.h"
#include "Kernel/ArenaAllocator.h"
#include <array>
#include <functional>
#include <new>
#include <numeric>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace LHCb::Kernel {

  namespace details {
    template <std::size_t N, typename T>
    std::array<T, N> distribute( T t ) {
      std::array<T, N> a;
      a.fill( t );
      return a;
    }

    template <typename... T>
    struct always_false : std::bool_constant<false> {};

    // assume that if T defines `std::tuple_size<T>::value`, that
    // std::get<I>(T) also works...
    template <typename T>
    using has_tuplesize_t = std::void_t<decltype( std::tuple_size<T>::value )>;

    template <typename T>
    constexpr bool is_tuple_like_v = Gaudi::cpp17::is_detected_v<has_tuplesize_t, T>;

    template <typename C, typename Arg,
              typename = std::enable_if_t<std::is_constructible_v<typename C::value_type, Arg> || is_tuple_like_v<Arg>>>
    decltype( auto ) emplace_back( C& c, Arg&& arg ) {
      if constexpr ( std::is_constructible_v<typename C::value_type, Arg> ) {
        return c.emplace_back( std::forward<Arg>( arg ) );
      } else if constexpr ( details::is_tuple_like_v<Arg> ) {
        return std::apply(
            [&c]( auto&&... args ) -> decltype( auto ) {
              return c.emplace_back( std::forward<decltype( args )>( args )... );
            },
            std::forward<Arg>( arg ) );
      }
    }

  } // namespace details

  template <typename... Args>
  class MultiVector {
    constexpr static auto N = sizeof...( Args );
    static_assert( N > 1 );
    constexpr static auto alignment = std::max( {sizeof( Args )...} );
    // constexpr static auto alignment = 64;
    template <typename T>
    using Allocator = LHCb::Allocators::DynamicArenaAllocator<T, alignment>;
    using Arena     = LHCb::Allocators::DynamicArena<alignment>;
    using Data      = std::tuple<std::vector<Args, Allocator<Args>>...>;

    static constexpr std::size_t n_bytes( std::size_t n ) { return ( Arena::template align_up<Args>( n ) + ... ); }

    typename Arena::Handle m_buffer;
    Data                   m_data = std::make_from_tuple<Data>( details::distribute<N>( m_buffer.get() ) );

  public:
    MultiVector( std::size_t maxElements ) : m_buffer{Arena::create( n_bytes( maxElements ) )} {
      std::apply( [maxElements]( auto&... d ) { ( d.reserve( maxElements ), ... ); }, m_data );
    }

    template <typename... U>
    auto emplace_back( U&&... args ) {
      static_assert( sizeof...( U ) == N,
                     "emplace_back takes exactly one argument per contained vector; if you need more than one argument "
                     "to construct an element, combine them inside a call to `std::forward_as_tuple` " );
      return std::apply(
          [&args...]( auto&... d ) { // C++20: [...args = std::forward<U>(args)]( auto& ... d )
            return std::forward_as_tuple( details::emplace_back( d, std::forward<U>( args ) )... );
          },
          m_data );
    }
    void clear() {
      std::apply( []( auto&... d ) { ( d.clear(), ... ); }, m_data );
    }
    void pop_back() {
      std::apply( []( auto&... d ) { ( d.pop_back(), ... ); }, m_data );
    }

    // accessors to a row across 'all' columns
    auto front() const {
      return std::apply( []( const auto&... d ) { return std::forward_as_tuple( d.front()... ); }, m_data );
    }
    auto operator[]( int i ) const {
      return std::apply( [i]( const auto&... d ) { return std::forward_as_tuple( d[i]... ); }, m_data );
    }
    auto back() const {
      return std::apply( []( const auto&... d ) { return std::forward_as_tuple( d.back()... ); }, m_data );
    }

    // accessors to a row across  columns specified by index
    template <size_t... I>
    auto front() const {
      return std::forward_as_tuple( std::get<I>( m_data ).front()... );
    }
    template <size_t... I>
    auto get( int i ) const {
      return std::forward_as_tuple( std::get<I>( m_data )[i]... );
    }
    template <size_t... I>
    auto back() const {
      return std::forward_as_tuple( std::get<I>( m_data ).back()... );
    }

    // accessors to a row across  columns specified by type
    template <typename... T>
    auto front() const {
      return std::forward_as_tuple( std::get<std::vector<T, Allocator<T>>>( m_data ).front()... );
    }
    template <typename... T>
    auto get( int i ) const {
      return std::forward_as_tuple( std::get<std::vector<T, Allocator<T>>>( m_data )[i]... );
    }
    template <typename... T>
    auto back() const {
      return std::forward_as_tuple( std::get<std::vector<T, Allocator<T>>>( m_data ).back()... );
    }

    // accessors to entire columns
    template <size_t I>
    const auto& column() const {
      return std::get<I>( m_data );
    }
    template <typename T>
    const auto& column() const {
      return std::get<std::vector<T, Allocator<T>>>( m_data );
    }

    // container properties
    size_t capacity() const { return std::get<0>( m_data ).capacity(); }

    size_t size() const { return std::get<0>( m_data ).size(); }

    bool empty() const { return std::get<0>( m_data ).empty(); }
  };

  template <typename... Args>
  class MultiContainer {
    constexpr static auto N = sizeof...( Args );
    static_assert( N > 1 );
    constexpr static auto alignment = std::max( {sizeof( Args )...} );
    // constexpr static auto alignment = 64;
    template <typename T>
    using Allocator = LHCb::Allocators::DynamicArenaAllocator<T, alignment>;
    using Arena     = LHCb::Allocators::DynamicArena<alignment>;
    using Data      = std::tuple<std::vector<Args, Allocator<Args>>...>;

    template <typename Size_t>
    static constexpr std::size_t n_bytes( std::array<Size_t, N> n ) {
      auto sizes = std::array{Arena::template align_up<Args>( 1 )...};
      return std::inner_product( sizes.begin(), sizes.end(), n.begin(), std::size_t{0} );
    }

    typename Arena::Handle m_buffer;
    Data                   m_data = std::make_from_tuple<Data>( details::distribute<N>( m_buffer.get() ) );

  public:
    template <typename... Size_t,
              typename = std::enable_if_t<sizeof...( Size_t ) == N &&
                                          std::conjunction_v<std::is_convertible<Size_t, std::size_t>...>>>
    MultiContainer( Size_t... maxElements ) : m_buffer{Arena::create( n_bytes( std::array{maxElements...} ) )} {
      std::apply( [maxElements...]( auto&... d ) { ( d.reserve( maxElements ), ... ); }, m_data );
    }

    template <size_t I>
    auto& column() {
      return std::get<I>( m_data );
    }
    template <typename T>
    auto& column() {
      return std::get<std::vector<T, Allocator<T>>>( m_data );
    }

    template <size_t I>
    const auto& column() const {
      return std::get<I>( m_data );
    }
    template <typename T>
    const auto& column() const {
      return std::get<std::vector<T, Allocator<T>>>( m_data );
    }

    bool empty() const {
      return std::apply( []( const auto&... d ) { return ( ... && d.empty() ); }, m_data );
    }
  };
} // namespace LHCb::Kernel
