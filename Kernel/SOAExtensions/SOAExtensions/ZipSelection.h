/*
 * (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration
 *
 * This software is distributed under the terms of the GNU General Public
 * Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 *
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */

// mostly copy and paste from Pr::Selection

#ifndef ZIP_SELECTION
#define ZIP_SELECTION 1
#include "ZipTraits.h"
#include "ZipUtils.h"
#include <algorithm>
#include <boost/type_index.hpp>
#include <cassert>
#include <cstddef>
#include <exception> // IWYU pragma: keep
#include <functional>
#include <iterator>
#include <limits>
#include <numeric>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
// IWYU pragma: no_include <bits/exception.h>
#include <cstdint> // IWYU pragma: keep
// IWYU pragma: no_include <bits/stdint-uintn.h>
#include "GaudiKernel/GaudiException.h"
#include "SOAExtensions/ZipContainer.h" // IWYU pragma: keep
#include <gsl/pointers>

namespace Zipping {

  namespace details {
    // Tag class to allow optimisation when we know the selection is a dummy.
    struct alwaysTrue {};
    struct alwaysFalse {};

    template <typename T>
    inline auto const typename_v = boost::typeindex::type_id_with_cvr<T>().pretty_name();
  } // namespace details

  /** @class ExportedSelection ZipSelection.h
   *
   * Exchange format for selections.
   *
   *  @tparam IndexSize The type used to represent indices into the underlying
   *  contiguous storage. Defaults to uint16_t, meaning by default you can only
   *  select 65536 objects...
   */
  template <typename IndexSize = uint16_t>
  struct ExportedSelection final {
    // usings and types
    using index_vector = typename std::vector<IndexSize>;

    // data members
    ZipFamilyNumber m_zipIdentifier;
    index_vector    m_indices{};

    // An ExportedSelection must always refer to an existing container family. Thus default construction is disabled.
    // For convenience, construction can be done with a ZipFamilyNumber or a Container.
    ExportedSelection() = delete;

    // alwaysFalse is not default'ed here to make the default explicit at call side.
    // no True option available in this option
    ExportedSelection( ZipFamilyNumber id, Zipping::details::alwaysFalse /*only for type, value unused*/ )
        : m_zipIdentifier( id ) {}

    template <typename CONTAINER,
              typename = typename std::enable_if_t<has_semantic_zip<std::decay_t<CONTAINER>>::value>>
    ExportedSelection( const CONTAINER& container, Zipping::details::alwaysFalse /*only for type, value unused*/ )
        : m_zipIdentifier{container.zipIdentifier()} {}

    template <typename CONTAINER,
              typename = typename std::enable_if_t<has_semantic_zip<std::decay_t<CONTAINER>>::value>>
    ExportedSelection( const CONTAINER& container, Zipping::details::alwaysTrue /*only for type, value unused*/ )
        : m_zipIdentifier{container.zipIdentifier()}, m_indices( container.size(), IndexSize{} ) {
      std::iota( m_indices.begin(), m_indices.end(), 0 );
    }

    // copy/move constructors, kept here to let the compiler complain should they ever get disabled accidentially
    ExportedSelection( const ExportedSelection& other ) = default;
    ExportedSelection( ExportedSelection&& other ) noexcept( std::is_nothrow_move_constructible<index_vector>::value ) =
        default;
    ~ExportedSelection()       = default;
    ExportedSelection& operator=( const ExportedSelection& ) = default;
    ExportedSelection&
    operator=( ExportedSelection&& ) noexcept( std::is_nothrow_move_constructible<index_vector>::value ) = default;

    bool operator==( const ExportedSelection& other ) const {
      return m_zipIdentifier == other.m_zipIdentifier && m_indices == other.m_indices;
    }

    // direct data construction
    // NB: INDEX_VECTOR may be const
    template <typename INDEX_VECTOR,
              typename = typename std::enable_if_t<std::is_same_v<std::decay_t<INDEX_VECTOR>, index_vector>>>
    ExportedSelection( INDEX_VECTOR&& indices, const ZipFamilyNumber id )
        : m_zipIdentifier( id ), m_indices( std::forward<INDEX_VECTOR>( indices ) ) {}

    ZipFamilyNumber zipIdentifier() const { return m_zipIdentifier; }

    std::size_t size() const { return m_indices.size(); }
    bool        empty() const { return m_indices.empty(); }

    friend ExportedSelection set_intersection( ExportedSelection const& s1, ExportedSelection const& s2 ) {
#ifndef DNDEBUG
      if ( s1.zipIdentifier() != s2.zipIdentifier() ) {
        throw GaudiException( "Performing set intersection on different container families.",
                              details::typename_v<ExportedSelection<>>, StatusCode::FAILURE );
      }
#endif
      ExportedSelection<> retval{s1.zipIdentifier(), Zipping::details::alwaysFalse{}};
      retval.m_indices.reserve( std::min( s1.size(), s2.size() ) );
      // Use std::set_intersection to combine s1.m_indices and s2.m_indices into a new index container
      std::set_intersection( s1.m_indices.begin(), s1.m_indices.end(), s2.m_indices.begin(), s2.m_indices.end(),
                             std::back_inserter( retval.m_indices ) );
      return retval;
    }

    friend ExportedSelection set_union( ExportedSelection const& s1, ExportedSelection const& s2 ) {
#ifndef DNDEBUG
      if ( s1.zipIdentifier() != s2.zipIdentifier() ) {
        throw GaudiException( "Performing set union on different container families.",
                              details::typename_v<ExportedSelection<>>, StatusCode::FAILURE );
      }
#endif
      if ( s1.empty() ) { return s2; }
      if ( s2.empty() ) { return s1; }

      ExportedSelection<> retval{s1.zipIdentifier(), Zipping::details::alwaysFalse{}};
      std::size_t         est_csize = std::max( s1.m_indices.back(), s2.m_indices.back() ) + 1;
      retval.m_indices.reserve( std::min( est_csize, s1.size() + s2.size() ) );
      // Use std::set_union to combine s1.m_indices and s2.m_indices into a new index container
      std::set_union( s1.m_indices.begin(), s1.m_indices.end(), s2.m_indices.begin(), s2.m_indices.end(),
                      std::back_inserter( retval.m_indices ) );
      return retval;
    }

    friend ExportedSelection set_difference( ExportedSelection const& s1, ExportedSelection const& s2 ) {
#ifndef DNDEBUG
      if ( s1.zipIdentifier() != s2.zipIdentifier() ) {
        throw GaudiException( "Performing set difference on different container families.",
                              details::typename_v<ExportedSelection<>>, StatusCode::FAILURE );
      }
#endif
      if ( s2.empty() ) { return s1; }

      ExportedSelection<> retval{s1.zipIdentifier(), Zipping::details::alwaysFalse{}};
      retval.m_indices.reserve( s1.size() );
      // Use std::set_difference to combine s1.m_indices and s2.m_indices into a new index container
      std::set_difference( s1.m_indices.begin(), s1.m_indices.end(), s2.m_indices.begin(), s2.m_indices.end(),
                           std::back_inserter( retval.m_indices ) );
      return retval;
    }

    friend ExportedSelection set_symmetric_difference( ExportedSelection const& s1, ExportedSelection const& s2 ) {
#ifndef DNDEBUG
      if ( s1.zipIdentifier() != s2.zipIdentifier() ) {
        throw GaudiException( "Performing set symmetric difference on different container families.",
                              details::typename_v<ExportedSelection<>>, StatusCode::FAILURE );
      }
#endif
      if ( s1.empty() ) { return s2; }
      if ( s2.empty() ) { return s1; }

      ExportedSelection<> retval{s1.zipIdentifier(), Zipping::details::alwaysFalse{}};
      std::size_t         est_csize = std::max( s1.m_indices.back(), s2.m_indices.back() ) + 1;
      retval.m_indices.reserve( std::min( est_csize, s1.size() + s2.size() ) );
      // Use std::set_symmetric_difference to combine s1.m_indices and s2.m_indices into a new index container
      std::set_symmetric_difference( s1.m_indices.begin(), s1.m_indices.end(), s2.m_indices.begin(), s2.m_indices.end(),
                                     std::back_inserter( retval.m_indices ) );
      return retval;
    }

    friend bool includes( ExportedSelection const& s1, ExportedSelection const& s2 ) {
#ifndef DNDEBUG
      if ( s1.zipIdentifier() != s2.zipIdentifier() ) {
        throw GaudiException( "Performing set includes on different container families.",
                              details::typename_v<ExportedSelection<>>, StatusCode::FAILURE );
      }
#endif
      return std::includes( s1.m_indices.begin(), s1.m_indices.end(), s2.m_indices.begin(), s2.m_indices.end() );
    }
  };

  /** @class SelectionView
   *
   * Allows to access selected element of a ZipContainer ("storage container"
   * in the following). Not intended as exchange format on TES, exchange via
   * `export_selection` and constructor. The backend behaviour is a container
   * of indices through which container access (selection[42],
   * selection.begin(), selection.begin()+5) is redirected.
   *
   * Does not own either the "data" nor the "selection".
   *
   * Should get created from an ExportedSelection, by means of makeSelection
   * below as initial creation.
   *
   *  @tparam CONTAINER The "selected" container (most likely an SOA::View)
   *  @tparam IndexSize The type used to represent indices into the underlying
   *  contiguous storage. Defaults to uint16_t, meaning by default you can only
   *  select 65536 objects...
   */

  template <typename CONTAINER, typename IndexSize = uint16_t,
            typename = typename std::enable_if_t<has_semantic_zip<std::decay_t<CONTAINER>>::value>>
  struct SelectionView {
    // TODO in optimised build we could reduce sizeof(SelectionView) by 40% by
    // using a single pointer in container_t and
    //      using an index_vector type that imposes size()==capacity()

    // usings and types
    using proxy_type = typename std::decay_t<CONTAINER>::proxy; // make it easier to write generic code
                                                                // that can handle both containers and
                                                                // SelectionViews
    using container_t  = std::decay_t<CONTAINER>;
    using index_vector = typename std::vector<IndexSize>;

    // data members
    gsl::not_null<const container_t*>  m_container;
    gsl::not_null<const index_vector*> m_indices;

    // Custom iterator class for looping through the index vector but
    // dereferencing to values in the actual container
    struct const_iterator {
      using index_iter_type   = typename index_vector::const_iterator;
      using value_type        = typename CONTAINER::proxy;
      using difference_type   = typename index_iter_type::difference_type;
      using iterator_category = std::random_access_iterator_tag;
      // FIXME: provide these to enable STL algorithms
      using pointer   = const value_type*; // for iterator_traits
      using reference = const value_type&; // for iterator_traits

      gsl::not_null<const container_t*> m_container;
      index_iter_type                   m_iter;

      value_type const operator*() const { return ( *m_container )[*m_iter]; }

      friend bool operator==( const_iterator const& lhs, const_iterator const& rhs ) {
        return lhs.m_container == rhs.m_container && lhs.m_iter == rhs.m_iter;
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

    ExportedSelection<IndexSize> export_selection() {
      return ExportedSelection<IndexSize>( *m_indices, m_container->zipIdentifier() );
    }

    // take pointer to avoid construction from temporary
    SelectionView( const container_t* container, const ExportedSelection<IndexSize>& selection )
        : m_container{container}, m_indices{&selection.m_indices} {
#ifndef NDEBUG
      if ( selection.zipIdentifier() != container->zipIdentifier() ) {
        throw GaudiException( "Building SelectionView from an ExportedSelection of a different container family.",
                              details::typename_v<SelectionView>, StatusCode::FAILURE );
      }
#endif
    }

    const_iterator begin() const { return {m_container, m_indices->begin()}; }
    const_iterator cbegin() const { return {m_container, m_indices->cbegin()}; }

    const_iterator end() const { return {m_container, m_indices->end()}; }
    const_iterator cend() const { return {m_container, m_indices->cend()}; }

    proxy_type const back() const { return ( *m_container )[m_indices->back()]; }
    proxy_type const front() const { return ( *m_container )[m_indices->front()]; }

    std::size_t size() const { return m_indices->size(); }

    bool empty() const { return m_indices->empty(); }

    proxy_type const operator[]( std::size_t i ) const {
      assert( i < size() );
      return ( *m_container )[( *m_indices )[i]];
    }

    friend bool operator==( SelectionView const& lhs, SelectionView const& rhs ) {
      return lhs.m_container == rhs.m_container && ( *lhs.m_indices ) == ( *rhs.m_indices );
    }

    friend bool operator!=( SelectionView const& lhs, SelectionView const& rhs ) { return !( lhs == rhs ); }

    /**
     * Refine a selection.
     *
     * @param predicate: the selection criterion, typically a lambda or function pointer.
     *                    FIXME: at the moment this does not invoke
     *                    std::invoke, thus not allowing SOA-Proxy methods due
     *                    to corner cases in proxy forwarding that turned out
     *                    problematic with templated proxy constructors that
     *                    forward to underlying data constructors.
     * @param reserveCapacity: anticipated number of selected objects. If no
     *                         "correct" guess is available, prefer to guess too
     *                         high rather than too low.
     * @returns A new Exported selection. I.e. the existing SelectionView is not modified.
     */
    template <typename Predicate>
    [[nodiscard]] ExportedSelection<IndexSize> select( Predicate&& predicate, int reserveCapacity = -1 ) const {
      ExportedSelection<IndexSize> retval( m_container->zipIdentifier(), details::alwaysFalse{} );
      retval.m_indices.reserve( reserveCapacity < 0 ? m_indices->size() : reserveCapacity );
      std::copy_if( m_indices->begin(), m_indices->end(), std::back_inserter( retval.m_indices ),
                    [&]( auto i ) { return predicate( ( *m_container )[i] ); } );
      return retval;
    }
  };

  template <typename CONTAINER, typename Predicate = details::alwaysTrue, typename IndexSize = uint16_t>
  ExportedSelection<IndexSize> makeSelection( const CONTAINER* container, Predicate&& predicate = {},
                                              int reserveCapacity = -1 ) {
    using container_t = std::decay_t<CONTAINER>;
    if ( container->size() >= typename container_t::size_type( std::numeric_limits<IndexSize>::max() ) ) {
      throw GaudiException{"Index overflow: " + std::to_string( container->size() - 1 ) + " > " +
                               std::to_string( std::numeric_limits<IndexSize>::max() ) +
                               details::typename_v<SelectionView<container_t>>,
                           details::typename_v<SelectionView<container_t>>, StatusCode::FAILURE};
    }
    ExportedSelection<IndexSize> retval( container->zipIdentifier(), details::alwaysFalse{} );

    if constexpr ( std::is_same_v<Predicate, details::alwaysTrue> ) {
      retval.m_indices.resize( container->size() );
      std::iota( retval.m_indices.begin(), retval.m_indices.end(), 0 );
    } else if constexpr ( std::is_same_v<Predicate, details::alwaysFalse> ) {
      if ( reserveCapacity >= 0 ) { retval.m_indices.reserve( reserveCapacity ); }
    } else {
      retval.m_indices.reserve( reserveCapacity < 0 ? container->size() : reserveCapacity );
      auto offset = 0;
      for ( auto const i : *container ) {
        if ( std::invoke( predicate, i ) ) { retval.m_indices.push_back( offset ); }
        ++offset;
      }
    }
    return retval;
  }

  // Template parameter deduction guides
  template <typename T, typename... Args>
  SelectionView( T*, Args... )->SelectionView<T>;

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
} // namespace Zipping

#endif
