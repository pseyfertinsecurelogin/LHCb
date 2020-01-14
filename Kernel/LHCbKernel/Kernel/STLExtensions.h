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

// This header contains methods that are likely to become parts of the STL
// at some point in the future, but currently are not available.

#include <array>
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

// add a macro state assumptions
#if defined( ASSUME )
#  undef ASSUME
#endif
#ifdef NDEBUG
#  define ASSUME( COND ) static_cast<void>( ( COND ) ? void( 0 ) : __builtin_unreachable() )
#else
#  include <cassert>
#  define ASSUME( COND ) assert( COND )
#endif

#ifdef NDEBUG
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  define GSL_UNENFORCED_ON_CONTRACT_VIOLATION
#endif
#include <cstddef>
// cstddef must be defined _before_ gsl/span to make gsl::span use std::byte
// instead of gsl::byte...
#include "gsl/span"
#ifdef NDEBUG
#  pragma GCC diagnostic pop
#endif

namespace LHCb {

  namespace cxx {

    // FIXME: C++20: replace with std::bind_front
    template <typename F, typename... BoundArgs>
    auto bind_front( F&& f, BoundArgs&&... boundArgs ) {
      return [f         = std::forward<F>( f ),
              boundArgs = std::tuple{std::forward<BoundArgs>( boundArgs )...}]( auto&&... args ) -> decltype( auto ) {
        return std::apply(
            f, std::tuple_cat( boundArgs, std::forward_as_tuple( std::forward<decltype( args )>( args )... ) ) );
      };
    }

  } // namespace cxx

  // TODO: when we use a more recent version of range-v3, switch to its version of span
  using gsl::span;

  // TODO: remove with a more recent version of GSL:
  //
  template <class ElementType>
  auto make_span( ElementType* ptr, typename span<ElementType>::index_type count ) {
    return span<ElementType>( ptr, count );
  }

  template <class ElementType>
  auto make_span( ElementType* firstElem, ElementType* lastElem ) {
    return span<ElementType>( firstElem, lastElem );
  }

  template <class ElementType, std::size_t N>
  auto make_span( ElementType ( &arr )[N] ) {
    return span<ElementType, N>( arr );
  }

  template <class ElementType, std::size_t N>
  auto make_span( std::array<ElementType, N>& arr ) {
    return span<ElementType, N>( arr );
  }

  template <class ElementType, std::size_t N>
  auto make_span( const std::array<ElementType, N>& arr ) {
    return span<const ElementType, N>( arr );
  }

  template <class Container>
  auto make_span( Container& cont ) {
    return span<typename Container::value_type>( cont );
  }

  template <class Container>
  auto make_span( const Container& cont ) {
    return span<const typename Container::value_type>( cont );
  }

  template <class Ptr>
  auto make_span( Ptr& cont, std::ptrdiff_t count ) {
    return span<typename Ptr::element_type>( cont, count );
  }

  namespace details_se {
    template <typename Iterator>
    struct is_span_iterator : std::false_type {};

    template <typename ValueType, bool isConst>
    struct is_span_iterator<gsl::details::span_iterator<ValueType, isConst>> : std::true_type {};

    template <typename Iterator>
    constexpr bool is_span_iterator_v = is_span_iterator<Iterator>::value;

    // see https://en.cppreference.com/w/cpp/named_req/ContiguousIterator
    // 1) array::iterator is typically a pointer, so no need to support it here
    // 2) string_view::iterator: why would ever want to turn that into a span?
    // 3) valarray: if you really need it, then feel free to add it
    //
    // or it is a pair of iterators into a span...
    template <typename Iterator>
    constexpr bool isContiguous() {
      using Value = typename std::iterator_traits<Iterator>::value_type;
      return ((std::is_same_v<typename std::vector<Value>::iterator, Iterator> ||
               std::is_same_v<typename std::vector<Value>::const_iterator, Iterator>)&&!std::is_same_v<bool, Value>) ||
             is_span_iterator_v<Iterator>;
    }
  } // namespace details_se

  template <typename Iterator, typename = std::enable_if_t<details_se::isContiguous<Iterator>()>>
  auto make_span( Iterator firstElem, Iterator lastElem ) {
    // avoid forming a reference to *firstElem, as [firstElem,lastElem) may be an empty range,
    // in which case *firstElem would be an invalid expression, and eg. std::addressof(*firstElem)
    // would imply UB.
    // C++20 FIXME: use std::to_address(firstElem)
    return make_span( firstElem.operator->(), std::distance( firstElem, lastElem ) );
  }

  namespace range {

    namespace details {
      template <typename A, typename B>
      using disable_if_same_or_derived = std::enable_if_t<!std::is_base_of_v<A, std::remove_reference_t<B>>>;
    }

    template <typename T>
    class single final {
      T m_data{};

    public:
      using value_type      = T;
      using pointer         = T*;
      using const_pointer   = std::add_const_t<T>*;
      using reference       = T&;
      using const_reference = std::add_const_t<T>&;

      template <typename U = T, typename = details::disable_if_same_or_derived<single, U>,
                typename = std::enable_if_t<std::is_constructible_v<T, U&&>>>
      constexpr single( U&& u ) : m_data( std::forward<U>( u ) ) {}

      template <typename... Args, typename = std::enable_if_t<std::is_constructible_v<T, Args&&...>>>
      constexpr explicit single( std::in_place_t, Args&&... args ) : m_data( std::forward<Args>( args )... ) {}

      constexpr pointer begin() noexcept { return std::addressof( m_data ); }
      constexpr pointer end() noexcept { return std::addressof( m_data ) + 1; }
      constexpr pointer data() noexcept { return begin(); }

      constexpr const_pointer begin() const noexcept { return std::addressof( m_data ); }
      constexpr const_pointer end() const noexcept { return std::addressof( m_data ) + 1; }
      constexpr const_pointer data() const noexcept { return begin(); }

      constexpr const_pointer cbegin() const noexcept { return begin(); }
      constexpr const_pointer cend() const noexcept { return end(); }

      [[nodiscard]] constexpr std::size_t size() const noexcept { return 1; }
      [[nodiscard]] constexpr bool        empty() const noexcept { return false; }

      constexpr const_reference value() const& noexcept { return m_data; }
      constexpr reference       value() & noexcept { return m_data; }
      constexpr T&&             value() && noexcept { return std::move( m_data ); }

      constexpr const_reference front() const noexcept { return m_data; }
      constexpr const_reference back() const noexcept { return m_data; }
      constexpr reference       front() noexcept { return m_data; }
      constexpr reference       back() noexcept { return m_data; }
    };

    template <typename T>
    single( T )->single<T>;

  } // namespace range

  namespace details_se {
    template <typename T, std::size_t... Is, typename... Args>
    std::array<T, sizeof...( Is )> make_object_array( std::index_sequence<Is...>, Args&&... args ) {
      return {std::conditional_t<Is, T, T>{std::forward<Args>( args )...}...};
    }
  } // namespace details_se

  /** Construct std::array<T, N>, explicitly forwarding the given arguments to the constructor of each T.
   */
  template <typename T, std::size_t N, typename... Args>
  std::array<T, N> make_object_array( Args&&... args ) {
    return details_se::make_object_array<T>( std::make_index_sequence<N>{}, std::forward<Args>( args )... );
  }
} // namespace LHCb

// gsl::span has no `front` or `back`
// whereas range::span does -- so until we get a version of range::v3 which
// includes range::span, we provide some free standing functions to provide
// `front` and `back`...
namespace gsl {
  template <typename T, std::ptrdiff_t N>
  decltype( auto ) front( span<T, N> s ) {
    return *s.begin();
  }

  template <typename T, std::ptrdiff_t N>
  decltype( auto ) back( span<T, N> s ) {
    return *s.rbegin();
  }
} // namespace gsl
