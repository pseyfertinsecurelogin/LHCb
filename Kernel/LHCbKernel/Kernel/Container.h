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
#include "Kernel/ArenaAllocator.h"
#include <array>
#include <functional>
#include <new>
#include <numeric>
#include <tuple>
#include <utility>
#include <vector>
#include <type_traits>

namespace LHCb::Kernel {

  namespace details {
    template <std::size_t N, typename T>
    std::array<T, N> distribute( T t ) {
      std::array<T, N> a;
      a.fill( t );
      return a;
    }

    template<typename T, typename = void>
    struct has_tuplesize_t : std::false_type {} ;

    template<typename T>
    struct has_tuplesize_t<T, std::void_t<decltype(std::tuple_size<T>::value)>>
        : std::true_type {};

    template <typename T>
    constexpr bool is_tuple_like_v = has_tuplesize_t<T>::value;
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
    auto emplace_back( U&&... arg ) {
      static_assert( sizeof...( U ) == N,
                     "emplace_back takes exactly one argument per contained vector. For more complex in-place "
                     "construction please refer to the std::piecewise_construct overload of emplace_back" );
      // C++20 std::apply( [..arg=std::forward<U>(arg)](auto&... d) { (d.emplace_back(std::forward<U>(arg)),...); },
      // m_data);
      return std::apply(
          [&arg...]( auto&... d ) { return std::forward_as_tuple( d.emplace_back( std::forward<U>( arg ) )... ); },
          m_data );
    }

    template <typename... U,
              typename = std::enable_if_t< ( details::is_tuple_like_v<U> && ...   )  > >
    auto emplace_back( std::piecewise_construct_t, U&&... args ) {
      static_assert( sizeof...( U ) == N );
      return std::apply(
          [&args...]( auto&... d ) {
            return std::forward_as_tuple( std::apply(
                [&d]( auto&&... arg1 ) -> decltype( auto ) {
                  return d.emplace_back( std::forward<decltype( arg1 )>( arg1 )... );
                },
                args )... );
          },
          m_data );
    }

    auto operator[]( int i ) const {
      return std::apply( [i]( const auto&... d ) { return std::forward_as_tuple( d[i]... ); }, m_data );
    }

    template <size_t... I>
    auto get( int i ) const {
      return std::forward_as_tuple( std::get<I>( m_data )[i]... );
    }

    template <typename... T>
    auto get( int i ) const {
      return std::forward_as_tuple( std::get<std::vector<T, Allocator<T>>>( m_data )[i]... );
    }

    template <size_t I>
    const auto& column() const {
      return std::get<I>( m_data );
    }

    template <typename T>
    const auto& column() const {
      return std::get<std::vector<T, Allocator<T>>>( m_data );
    }

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

    template <size_t I>
    const auto& column( int i ) const {
      return std::get<I>( m_data )[i];
    }
    template <typename T>
    const auto& column( int i ) const {
      return std::get<std::vector<T, Allocator<T>>>( m_data )[i];
    }

    bool empty() const {
      return std::apply( []( const auto&... d ) { return ( ... && d.empty() ); }, m_data );
    }
  };
} // namespace LHCb::Kernel
