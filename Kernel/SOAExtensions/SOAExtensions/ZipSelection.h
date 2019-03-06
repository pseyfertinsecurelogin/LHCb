/*
 * (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration
 *
 * This software is distributed under the terms of the GNU General Public
 * Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 *
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */

// with some changes

#ifndef ZIP_SELECTION
#define ZIP_SELECTION 1
#include "ZipTraits.h"          // for is_IDed
#include <algorithm>            // for is_sorted, copy_if, includes, set_di...
#include <boost/type_index.hpp> // for type_id_with_cvr
#include <cassert>              // for assert
#include <cstddef>              // for size_t
#include <exception>            // IWYU pragma: keep
#include <functional>           // for invoke
#include <iterator>             // for back_inserter, random_access_iterato...
#include <limits>               // for numeric_limits
#include <numeric>              // for iota
#include <string>               // for string, operator+, to_string
#include <type_traits>          // for enable_if_t, false_type, true_type
#include <utility>              // for forward
#include <vector>               // for vector, allocator
// IWYU pragma: no_include <bits/exception.h>
#include <cstdint> // IWYU pragma: keep
// IWYU pragma: no_include <bits/stdint-uintn.h>
#include "GaudiKernel/GaudiException.h"
#include "SOAExtensions/SOAContainerSet.h"

namespace details {
  // Tag class to allow optimisation when we know the selection is a dummy.
  struct alwaysTrue {};
  struct alwaysFalse {};

  template <typename T>
  inline auto const typename_v = boost::typeindex::type_id_with_cvr<T>().pretty_name();

  template <typename S>
  void check_for_set_operation( S const& s1, S const& s2, const char* msg_prefix ) {
    if ( UNLIKELY( s1.m_container.data() != s2.m_container.data() ) ) {
      throw GaudiException{std::string{msg_prefix} +
                               "(SelectionView, SelectionView) was "
                               "given views to different "
                               "underlying storage." +
                               typename_v<S>,
                           typename_v<S>, StatusCode::FAILURE};
    }
    assert( std::is_sorted( s1.m_indices.begin(), s1.m_indices.end() ) );
    assert( std::is_sorted( s2.m_indices.begin(), s2.m_indices.end() ) );
  }
} // namespace details

template <typename IndexSize = uint16_t>
struct ExportedSelection {
  using index_vector = typename std::vector<IndexSize>;
  index_vector m_indices;
  ZipID        m_identifier;
  ZipID        zipIdentifier() const { return m_identifier; }
};

/** @class SelectionView
 *
 *  TODO: update docs
 *
 *  @tparam CONTAINER The "selected" container (most likely an SOA::View)
 *  @tparam IndexSize The type used to represent indices into the underlying
 * contiguous storage. Defaults to uint16_t, meaning by default you can only
 * select 65536 objects...
 */
template <typename CONTAINER, typename IndexSize = uint16_t,
          typename = typename std::enable_if_t<has_semantic_zip<std::decay_t<CONTAINER>>::value>>
struct SelectionView {
  // TODO in optimised build we could reduce sizeof(SelectionView) by 40% by
  // using a single pointer in container_t and
  //      using an index_vector type that imposes size()==capacity()
  using proxy_type = typename std::decay_t<CONTAINER>::proxy; // make it easier to write generic code
                                                // that can handle both containers and
                                                // SelectionViews
  using container_t  = std::decay_t<CONTAINER>;
  using index_vector = typename std::vector<IndexSize>;

  // Custom iterator class for looping through the index vector but
  // dereferencing to values in the actual container
  struct const_iterator {
    using index_iter_type   = typename index_vector::const_iterator;
    using value_type        = typename CONTAINER::proxy;
    using difference_type   = typename index_iter_type::difference_type;
    using iterator_category = std::random_access_iterator_tag;

    const container_t& m_container;
    index_iter_type    m_iter;

    value_type const operator*() const { return m_container[*m_iter]; }

    friend bool operator==( const_iterator const& lhs, const_iterator const& rhs ) {
      // TODO: CHECKME
      return &( lhs.m_container ) == &( rhs.m_container ) && lhs.m_iter == rhs.m_iter;
    }

    friend bool operator!=( const_iterator const& lhs, const_iterator const& rhs ) { return !( lhs == rhs ); }

    friend difference_type operator-( const_iterator const& lhs, const_iterator const& rhs ) {
      return lhs.m_iter - rhs.m_iter;
    }

    const_iterator operator+( difference_type n ) const { return {m_container, m_iter + n}; }

    const_iterator& operator-=( difference_type n ) const {
      m_iter -= n;
      return *this;
    }

    const_iterator& operator+=( difference_type n ) const {
      m_iter += n;
      return *this;
    }

    const_iterator operator-( difference_type n ) const { return {m_container, m_iter - n}; }

    const_iterator& operator++() {
      ++m_iter;
      return *this;
    }

    const_iterator& operator--() {
      --m_iter;
      return *this;
    }
  };

  const container_t& m_container;
  index_vector m_indices;

  ExportedSelection<IndexSize> export_selection() { return {m_indices, m_container.zipIdentifier()}; }

  /** Constructor creating a SelectionView from a contiguous storage
   * container.
   *
   *  By default all elements of the input span are marked as selected.
   *
   *  @param container       Contiguous storage container that this
   * selection refers to.
   *  @param predicate       Optional predicate applied to elements of the
   * input container.
   *  @param reserveCapacity Optional estimate of the number of elements
   * that will be selected by predicate, which is used to initialise the
   * vector of selected indices.
   */
  template <typename Predicate = details::alwaysTrue>
  SelectionView( const container_t& container, Predicate&& predicate = {}, int reserveCapacity = -1 )
      : m_container{container} {
    if (m_container.size() >= typename container_t::size_type(std::numeric_limits<IndexSize>::max())) {
      throw GaudiException{"Index overflow: " + std::to_string( container.size() - 1 ) + " > " +
                               std::to_string( std::numeric_limits<IndexSize>::max() ) +
                               details::typename_v<SelectionView>,
                           details::typename_v<SelectionView>, StatusCode::FAILURE};
    }

    if constexpr ( std::is_same_v<Predicate, details::alwaysTrue> ) {
      m_indices.resize( m_container.size() );
      // container_t is contiguous so we just need {0 ..
      // container.size()-1}
      std::iota( m_indices.begin(), m_indices.end(), 0 );
    } else if constexpr ( std::is_same_v<Predicate, details::alwaysFalse> ) {
      if ( reserveCapacity >= 0 ) { m_indices.reserve( reserveCapacity ); }
    } else {
      m_indices.reserve( reserveCapacity < 0 ? m_container.size() : reserveCapacity );
      auto offset = 0;
      for ( auto const i : m_container ) {
        if ( std::invoke( predicate, i ) ) { m_indices.push_back( offset ); }
        ++offset;
      }
    }
  }

  SelectionView( const container_t& container, const ExportedSelection<IndexSize>& selection )
      : m_container{container}, m_indices{selection.m_indices} {
    if ( selection.zipIdentifier() != container.zipIdentifier() ) {
      throw GaudiException( "incompatible selection!!!! ahahah write something more "
                            "meaningful!",
                            details::typename_v<SelectionView>, StatusCode::FAILURE );
    }
  }
  SelectionView( const container_t& container, ExportedSelection<IndexSize>& selection )
      : m_container{container}, m_indices{selection.m_indices} {
    if ( selection.zipIdentifier() != container.zipIdentifier() ) {
      throw GaudiException( "incompatible selection!!!! ahahah write something more "
                            "meaningful!",
                            details::typename_v<SelectionView>, StatusCode::FAILURE );
    }
  }

  /** Constructor creating a SelectionView from another SelectionView,
   * optionally applying an extra selection.
   *
   *  By default the same elements of the underlying storage are selected.
   *
   *  @param old_selection   Other SelectionView object to copy.
   *  @param predicate       Optional predicate to further filter the input
   * selection.
   *  @param reserveCapacity Optional estimate of the number of elements
   * that will be selected by predicate, which is used to initialise the
   * vector of selected indices.
   */
  template <typename Predicate>
  SelectionView( SelectionView const& old_selection, Predicate&& predicate, int reserveCapacity = -1 )
      : m_container{old_selection.m_container} {
    // It's imposed that old_selection has the same IndexSize as us, so we
    // don't need to do any overflow checking apply an additional
    // selection on the input selection
    m_indices.reserve( reserveCapacity < 0 ? old_selection.size() : reserveCapacity );
    std::copy_if( old_selection.m_indices.begin(), old_selection.m_indices.end(), std::back_inserter( m_indices ),
                  [&]( auto i ) { return predicate( m_container[i] ); } );
  }

  const_iterator begin() const { return {m_container, m_indices.begin()}; }
  const_iterator cbegin() const { return {m_container, m_indices.cbegin()}; }

  const_iterator end() const { return {m_container, m_indices.end()}; }
  const_iterator cend() const { return {m_container, m_indices.cend()}; }

  proxy_type const back() const { return m_container[m_indices.back()]; }
  proxy_type const front() const { return m_container[m_indices.front()]; }

  std::size_t size() const { return m_indices.size(); }

  bool empty() const { return m_indices.empty(); }

  proxy_type const operator[]( std::size_t i ) const {
    assert( i < size() );
    return m_container[m_indices[i]];
  }

  // TODO: CHECKME
  friend bool operator==( SelectionView const& lhs, SelectionView const& rhs ) {
    return lhs.m_container == rhs.m_container && lhs.m_indices == rhs.m_indices;
  }

  friend bool operator!=( SelectionView const& lhs, SelectionView const& rhs ) { return !( lhs == rhs ); }

  template <typename Predicate>
  SelectionView select( Predicate&& p, int reserveCapacity = -1 ) const {
    return SelectionView{*this, std::forward<Predicate>( p ), reserveCapacity};
  }

  // Set operations -- these throw if two SelectionViews that point to
  // different underlying storage are provided.
  //
  // TODO: abstract away that selections can be of different type from the
  // same containerset
  friend SelectionView set_union( SelectionView const& s1, SelectionView const& s2 ) {
    details::check_for_set_operation( s1, s2,
                                      "set_union" ); // check s1 and s2 are valid and compatible

    // Shortcuts so we can use .back() below
    if ( s1.empty() ) { return s2; }
    if ( s2.empty() ) { return s1; }

    // Create a new SelectionView object with an empty index vector with
    // an appropriate reserved capacity
    std::size_t   est_csize = std::max( s1.m_indices.back(), s2.m_indices.back() ) + 1;
    SelectionView ret{s1.m_container, details::alwaysFalse{},
                      static_cast<int>( std::min( est_csize, s1.size() + s2.size() ) )};

    // Use std::set_union to combine s1.m_indices and s2.m_indices into a
    // new index container
    std::set_union( s1.m_indices.begin(), s1.m_indices.end(), s2.m_indices.begin(), s2.m_indices.end(),
                    std::back_inserter( ret.m_indices ) );
    return ret;
  }

  friend SelectionView set_intersection( SelectionView const& s1, SelectionView const& s2 ) {
    details::check_for_set_operation( s1, s2,
                                      "set_intersection" ); // check s1 and s2 are valid and
                                                            // compatible

    // Create a new SelectionView object with an empty index vector with
    // an appropriate reserved capacity
    SelectionView ret{s1.m_container, details::alwaysFalse{}, static_cast<int>( std::min( s1.size(), s2.size() ) )};

    // Use std::set_intersection to combine s1.m_indices and s2.m_indices
    // into a new index container
    std::set_intersection( s1.m_indices.begin(), s1.m_indices.end(), s2.m_indices.begin(), s2.m_indices.end(),
                           std::back_inserter( ret.m_indices ) );
    return ret;
  }

  friend SelectionView set_difference( SelectionView const& s1, SelectionView const& s2 ) {
    details::check_for_set_operation( s1, s2,
                                      "set_difference" ); // check s1 and s2 are valid and compatible

    // Create a new SelectionView object with an empty index vector with
    // an appropriate reserved capacity
    SelectionView ret{s1.m_container, details::alwaysFalse{}, static_cast<int>( s1.size() )};

    // Use std::set_difference to combine s1.m_indices and s2.m_indices
    // into a new index container
    std::set_difference( s1.m_indices.begin(), s1.m_indices.end(), s2.m_indices.begin(), s2.m_indices.end(),
                         std::back_inserter( ret.m_indices ) );
    return ret;
  }

  friend bool includes( SelectionView const& s1, SelectionView const& s2 ) {
    details::check_for_set_operation( s1, s2,
                                      "includes" ); // check s1 and s2 are valid and compatible
    return std::includes( s1.m_indices.begin(), s1.m_indices.end(), s2.m_indices.begin(), s2.m_indices.end() );
  }

  friend SelectionView set_symmetric_difference( SelectionView const& s1, SelectionView const& s2 ) {
    details::check_for_set_operation( s1, s2,
                                      "set_symmetric_difference" ); // check s1 and s2 are valid and
                                                                    // compatible

    // Shortcuts so we can use .back() below safely
    if ( s1.empty() ) { return s2; }
    if ( s2.empty() ) { return s1; }

    // Create a new SelectionView object with an empty index vector with
    // an appropriate reserved capacity
    std::size_t   est_csize = std::max( s1.m_indices.back(), s2.m_indices.back() ) + 1;
    SelectionView ret{s1.m_container, details::alwaysFalse{},
                      static_cast<int>( std::min( est_csize, s1.size() + s2.size() ) )};

    // Use std::set_difference to combine s1.m_indices and s2.m_indices
    // into a new index container
    std::set_symmetric_difference( s1.m_indices.begin(), s1.m_indices.end(), s2.m_indices.begin(), s2.m_indices.end(),
                                   std::back_inserter( ret.m_indices ) );
    return ret;
  }
};

// Template parameter deduction guides
// template<typename T, typename... Args>
// SelectionView(T, Args...)->SelectionView<typename T::value_type>;

namespace details {
  // Helpers for requireSelection<S> below
  template <typename T>
  struct isSelectionHelper : std::false_type {};

  template <typename C, typename I>
  struct isSelectionHelper<SelectionView<C, I>> : std::true_type {};

  template <typename S>
  inline constexpr bool isSelection_v = details::isSelectionHelper<S>::value;
} // namespace details

// Helper to determine whether a type is a SelectionView
template <typename S>
using requireSelection = std::enable_if_t<details::isSelection_v<S>>;

// Value-by-value comparison, don't care whether lhs and rhs are actually
// views onto the same container.
template <typename S, typename = requireSelection<S>>
bool equal_values( S const& lhs, S const& rhs ) {
  return std::equal( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
}

template <typename S, typename = requireSelection<S>>
bool equal_values( S const& lhs, typename S::container_t rhs ) {
  return std::equal( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
}

template <typename S, typename = requireSelection<S>>
bool equal_values( typename S::container_t lhs, S const& rhs ) {
  return std::equal( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
}

#endif
