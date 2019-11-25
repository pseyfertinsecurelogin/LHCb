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
#include "boost/callable_traits.hpp"
#include <array>
#include <functional>
#include <new>
#include <numeric>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#if defined( __clang__ ) && ( __clang_major__ < 9 )
#  define GF_SUPPRESS_SPURIOUS_CLANG_WARNING_BEGIN                                                                     \
    _Pragma( "clang diagnostic push" ) _Pragma( "clang diagnostic ignored \"-Wunused-lambda-capture\"" )
#  define GF_SUPPRESS_SPURIOUS_CLANG_WARNING_END _Pragma( "clang diagnostic pop" )
#else
#  define GF_SUPPRESS_SPURIOUS_CLANG_WARNING_BEGIN
#  define GF_SUPPRESS_SPURIOUS_CLANG_WARNING_END
#endif

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

    template <size_t i, typename Callable>
    using arg_t = std::tuple_element_t<i, boost::callable_traits::args_t<Callable>>;
    template <typename Callable>
    constexpr auto arity_v = std::tuple_size_v<boost::callable_traits::args_t<Callable>>;

    template <typename Fun, typename Tuple, std::size_t... iarg>
    decltype( auto ) apply_helper( Fun&& f, const Tuple& data, int index, std::index_sequence<iarg...> ) {
      return std::apply( f, data.template get<arg_t<iarg, Fun>...>( index ) );
    }

    template <typename Fun, typename Out, typename Tuple, std::size_t... iarg>
    decltype( auto ) apply_binary_helper( Fun&& f, Out&& out, const Tuple& data, int index,
                                          std::index_sequence<iarg...> ) {
      return std::apply( f, std::tuple_cat( std::forward_as_tuple( std::forward<Out>( out ) ),
                                            data.template get<arg_t<iarg, Fun>...>( index ) ) );
    }

    template <typename Fun, typename Data>
    decltype( auto ) apply( Fun&& f, const Data& data, int index ) {
      return apply_helper( std::forward<Fun>( f ), data, index, std::make_index_sequence<arity_v<Fun>>{} );
    }

    template <std::size_t... Is, typename Fun, typename Data, typename = std::enable_if_t<( sizeof...( Is ) > 0 )>>
    decltype( auto ) apply( Fun&& f, const Data& data, int index ) {
      return std::apply( std::forward<Fun>( f ), data.template get<Is...>( index ) );
    }

    template <typename Fun, typename Out, typename Data>
    decltype( auto ) apply_binary( Fun&& f, Out&& out, const Data& data, int index ) {
      return apply_binary_helper( std::forward<Fun>( f ), std::forward<Out>( out ), data, index,
                                  std::make_index_sequence<arity_v<Fun> - 1>{} );
    }

    struct multiVectorView_tag {};

  } // namespace details

  template <typename... Args>
  class MultiVector {
    constexpr static auto N = sizeof...( Args );
    static_assert( N > 1 );
    constexpr static std::size_t MinAlign  = 0;
    constexpr static auto        alignment = std::max( {MinAlign, sizeof( Args )...} );
    template <typename T>
    using Allocator = LHCb::Allocators::DynamicArenaAllocator<T, alignment>;
    using Arena     = LHCb::Allocators::DynamicArena<alignment>;
    using Data      = std::tuple<std::vector<Args, Allocator<Args>>...>;

    static constexpr std::size_t n_bytes( std::size_t n ) { return ( Arena::template align_up<Args>( n ) + ... ); }

    typename Arena::Handle m_buffer;
    Data                   m_data = std::make_from_tuple<Data>( details::distribute<N>( m_buffer.get() ) );

    template <std::size_t... Is>
    class indexView : public details::multiVectorView_tag {
      const MultiVector* parent;
      class Iterator {
        const MultiVector* parent;
        std::size_t        i;

      public:
        Iterator( const MultiVector* parent, std::size_t i ) : parent{parent}, i{i} {}
        auto      operator*() const { return parent->template get<Is...>( i ); }
        Iterator& operator++() {
          ++i;
          return *this;
        }
        friend bool operator!=( const Iterator& lhs, const Iterator& rhs ) {
          return lhs.parent != rhs.parent || lhs.i != rhs.i;
        }
      };

    public:
      indexView( const MultiVector* parent ) : parent{parent} {}
      Iterator begin() const { return {parent, 0}; }
      Iterator end() const { return {parent, parent->size()}; }
    };

    template <typename... T>
    class typeView : public details::multiVectorView_tag {
      const MultiVector* parent;
      class Iterator {
        const MultiVector* parent;
        std::size_t        i;

      public:
        using parent_t = MultiVector;
        Iterator( const MultiVector* parent, std::size_t i ) : parent{parent}, i{i} {}
        auto      operator*() const { return parent->template get<T...>( i ); }
        Iterator& operator++() {
          ++i;
          return *this;
        }
        friend bool operator!=( const Iterator& lhs, const Iterator& rhs ) {
          return lhs.parent != rhs.parent || lhs.i != rhs.i;
        }
      };

    public:
      typeView( const MultiVector* parent ) : parent{parent} {}
      Iterator begin() const { return {parent, 0}; }
      Iterator end() const { return {parent, parent->size()}; }
    };

  public:
    MultiVector( std::size_t maxElements ) : m_buffer{Arena::create( n_bytes( maxElements ) )} {
      std::apply( [maxElements]( auto&... d ) { ( d.reserve( maxElements ), ... ); }, m_data );
    }

    template <typename... U>
    auto emplace_back( U&&... args ) {
      static_assert( sizeof...( U ) == N,
                     "emplace_back takes exactly one argument per contained vector; if you need more than one argument "
                     "to construct an element, combine them inside a call to `std::forward_as_tuple` " );
      GF_SUPPRESS_SPURIOUS_CLANG_WARNING_BEGIN
      return std::apply(
          [&args...]( auto&... d ) { // C++20: [...args = std::forward<U>(args)]( auto& ... d )
            return std::forward_as_tuple( details::emplace_back( d, std::forward<U>( args ) )... );
          },
          m_data );
      GF_SUPPRESS_SPURIOUS_CLANG_WARNING_END
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
    auto operator[]( std::size_t i ) const {
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
    auto get( std::size_t i ) const {
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
    auto get( std::size_t i ) const {
      return std::forward_as_tuple( std::get<std::vector<T, Allocator<T>>>( m_data )[i]... );
    }
    template <typename... T>
    auto back() const {
      return std::forward_as_tuple( std::get<std::vector<T, Allocator<T>>>( m_data ).back()... );
    }

    // accessors to entire column(s)
    template <size_t I, size_t... Is>
    decltype( auto ) column() const {
      if constexpr ( sizeof...( Is ) != 0 ) {
        return indexView<I, Is...>{this};
      } else {
        return std::get<I>( m_data );
      }
    }
    template <typename T, typename... Ts>
    decltype( auto ) column() const {
      if constexpr ( sizeof...( Ts ) != 0 ) {
        return typeView<T, Ts...>{this};
      } else {
        return std::get<std::vector<T, Allocator<T>>>( m_data );
      }
    }

    // container properties
    size_t capacity() const { return std::get<0>( m_data ).capacity(); }

    size_t size() const { return std::get<0>( m_data ).size(); }

    bool empty() const { return std::get<0>( m_data ).empty(); }
  };

  namespace details {
    template <typename T>
    struct isMultiVector : std::false_type {};
    template <typename... Columns>
    struct isMultiVector<MultiVector<Columns...>> : std::true_type {};

  } // namespace details

  template <typename T>
  constexpr bool isMultiVector_v = details::isMultiVector<std::decay_t<T>>::value;

  template <typename T>
  constexpr bool isMultiVectorView_v = std::is_base_of_v<details::multiVectorView_tag, std::decay_t<T>>;

  // transform
  template <typename MultiVector, typename OutputIterator, typename Callable,
            typename = std::enable_if_t<isMultiVector_v<MultiVector>>>
  OutputIterator transform( MultiVector const& in, OutputIterator out, Callable&& f ) {
    const auto N = in.size();
    for ( size_t i = 0; i != N; ++i ) *out++ = details::apply( f, in, i );
    return out;
  }

  template <typename MultiVectorView, typename OutputIterator, typename Callable,
            typename = std::enable_if_t<isMultiVectorView_v<MultiVectorView>>>
  OutputIterator transform( MultiVectorView in, OutputIterator out, Callable&& f ) {
    return std::transform( in.begin(), in.end(), out, [f = std::forward<Callable>( f )]( auto&& tuple ) {
      return std::apply( f, std::forward<decltype( tuple )>( tuple ) );
    } );
  }

  // transform_reduce
  template <typename MultiVector, typename Value, typename BinaryOp, typename UnaryOp,
            typename = std::enable_if_t<isMultiVector_v<MultiVector>>>
  Value transform_reduce( MultiVector const& in, Value out, BinaryOp binaryOp, UnaryOp unaryOp ) {
    const auto N = in.size();
    for ( size_t i = 0; i != N; ++i ) {
      out = std::invoke( binaryOp, std::move( out ), details::apply( unaryOp, in, i ) );
    }
    return out;
  }

  // accumulate
  template <typename MultiVector, typename Value, typename BinaryOp,
            typename = std::enable_if_t<isMultiVector_v<MultiVector>>>
  Value accumulate( MultiVector const& in, Value out, BinaryOp&& f ) {
    const auto N = in.size();
    for ( size_t i = 0; i != N; ++i ) out = details::apply_binary( f, std::move( out ), in, i );
    return std::move( out );
  }

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
      GF_SUPPRESS_SPURIOUS_CLANG_WARNING_BEGIN
      std::apply( [maxElements...]( auto&... d ) { ( d.reserve( maxElements ), ... ); }, m_data );
      GF_SUPPRESS_SPURIOUS_CLANG_WARNING_END
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
