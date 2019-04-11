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
    struct alwaysTrue_t {};
    inline constexpr alwaysTrue_t alwaysTrue = alwaysTrue_t{};
    struct alwaysFalse_t {};
    inline constexpr alwaysFalse_t alwaysFalse = alwaysFalse_t{};

    template <typename T>
    inline auto const typename_v = boost::typeindex::type_id_with_cvr<T>().pretty_name();
  } // namespace details

  /** @class ExportedSelection ZipSelection.h
   *
   * @brief Exchange format for selections.
   *
   * Should normally be created with Zipping::makeSelection and used through
   * Zipping::SelectionView. The ExportedSelection is intended to be written to
   * TES and passed between algorithms. It facilitates creating a selection on
   * one Zipping::ZipContainer and applied to another Zipping::ZipContainer.
   *
   *  @tparam IndexSize The type used to represent indices into the underlying
   *  contiguous storage. Defaults to uint16_t, meaning by default you can only
   *  select 65536 objects...
   */
  template <typename IndexSize = uint16_t>
  struct ExportedSelection final {
    // usings and types
    using index_vector = typename std::vector<IndexSize>; ///< type of indices

    // data members
    ZipFamilyNumber m_zipIdentifier; ///< Number of the container family this selection applies to
    index_vector    m_indices{};     ///< indices of selected elements

    // alwaysFalse is not default'ed here to make the default explicit at call side.
    // no True option available in this option
    /// Constructor from a container family number, defaults to zero selected elements
    ExportedSelection( ZipFamilyNumber id, Zipping::details::alwaysFalse_t /*only for type, value unused*/ )
        : m_zipIdentifier( id ) {}

    /**
     * @brief constructor from an existing container with no selection
     *
     * @tparam CONTAINER (deduced) type of Zipping::ZipContainer that is referenced
     * @param container  referenced container
     * @param            unused input (purely for type deduction and verbose call code)
     */
    template <typename CONTAINER, typename = std::enable_if_t<has_semantic_zip_v<std::decay_t<CONTAINER>>>>
    ExportedSelection( const CONTAINER& container, Zipping::details::alwaysFalse_t /*only for type, value unused*/ )
        : m_zipIdentifier{container.zipIdentifier()} {}

    /**
     * @brief constructor from an existing container selecting everything
     *
     * @tparam CONTAINER (deduced) type of Zipping::ZipContainer that is referenced
     * @param container  referenced container
     * @param            unused input (purely for type deduction and verbose call code)
     */
    template <typename CONTAINER, typename = std::enable_if_t<has_semantic_zip_v<std::decay_t<CONTAINER>>>>
    ExportedSelection( const CONTAINER& container, Zipping::details::alwaysTrue_t /*only for type, value unused*/ )
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

    /// comparison of ExportedSelection (same family and same selected indices)
    friend bool operator==( ExportedSelection const& lhs, ExportedSelection const& rhs ) {
      return lhs.m_zipIdentifier == rhs.m_zipIdentifier && lhs.m_indices == rhs.m_indices;
    }

    // direct data construction
    // NB: INDEX_VECTOR may be const
    template <typename INDEX_VECTOR,
              typename = std::enable_if_t<std::is_same_v<std::decay_t<INDEX_VECTOR>, index_vector>>>
    ExportedSelection( INDEX_VECTOR&& indices, const ZipFamilyNumber id )
        : m_zipIdentifier( id ), m_indices( std::forward<INDEX_VECTOR>( indices ) ) {}

    /**
     * @brief Semantic validation method
     *
     * Meant for library internal usage but provided publically anyway.
     *
     * @return Zipping::ZipFamilyNumber of the containers to which this application can be applied.
     */
    ZipFamilyNumber zipIdentifier() const { return m_zipIdentifier; }

    /// number of selected elements
    std::size_t size() const { return m_indices.size(); }
    /// check if zero elements are selected
    bool empty() const { return m_indices.empty(); }

    /**
     * @brief Provide a selection of all elements that are contained in both input selections
     *
     * (Set theoretical) intersection of the input selections. Performs semantic validation in debug build.
     *
     * @param s1 a Zipping::ExportedSelection
     * @param s2 a Zipping::ExportedSelection
     */
    friend ExportedSelection set_intersection( ExportedSelection const& s1, ExportedSelection const& s2 ) {
#ifndef DNDEBUG
      if ( s1.zipIdentifier() != s2.zipIdentifier() ) {
        throw GaudiException( "Performing set intersection on different container families.",
                              details::typename_v<ExportedSelection<>>, StatusCode::FAILURE );
      }
#endif
      ExportedSelection<> retval{s1.zipIdentifier(), Zipping::details::alwaysFalse};
      retval.m_indices.reserve( std::min( s1.size(), s2.size() ) );
      // Use std::set_intersection to combine s1.m_indices and s2.m_indices into a new index container
      std::set_intersection( s1.m_indices.begin(), s1.m_indices.end(), s2.m_indices.begin(), s2.m_indices.end(),
                             std::back_inserter( retval.m_indices ) );
      return retval;
    }

    /**
     * @brief Provide a selection of all elements that are contained in at least one input selections
     *
     * (Set theoretical) union of the input selections. Performs semantic validation in debug build.
     *
     * @param s1 a Zipping::ExportedSelection
     * @param s2 a Zipping::ExportedSelection
     */
    friend ExportedSelection set_union( ExportedSelection const& s1, ExportedSelection const& s2 ) {
#ifndef DNDEBUG
      if ( s1.zipIdentifier() != s2.zipIdentifier() ) {
        throw GaudiException( "Performing set union on different container families.",
                              details::typename_v<ExportedSelection<>>, StatusCode::FAILURE );
      }
#endif
      if ( s1.empty() ) { return s2; }
      if ( s2.empty() ) { return s1; }

      ExportedSelection<> retval{s1.zipIdentifier(), Zipping::details::alwaysFalse};
      std::size_t         est_csize = std::max( s1.m_indices.back(), s2.m_indices.back() ) + 1;
      retval.m_indices.reserve( std::min( est_csize, s1.size() + s2.size() ) );
      // Use std::set_union to combine s1.m_indices and s2.m_indices into a new index container
      std::set_union( s1.m_indices.begin(), s1.m_indices.end(), s2.m_indices.begin(), s2.m_indices.end(),
                      std::back_inserter( retval.m_indices ) );
      return retval;
    }

    /**
     * @brief Provide a selection of all elements that are contained in s1 but not s2
     *
     * (Set theoretical) difference of the input selections. Performs semantic validation in debug build.
     *
     * @param s1 a Zipping::ExportedSelection
     * @param s2 a Zipping::ExportedSelection
     */
    friend ExportedSelection set_difference( ExportedSelection const& s1, ExportedSelection const& s2 ) {
#ifndef DNDEBUG
      if ( s1.zipIdentifier() != s2.zipIdentifier() ) {
        throw GaudiException( "Performing set difference on different container families.",
                              details::typename_v<ExportedSelection<>>, StatusCode::FAILURE );
      }
#endif
      if ( s2.empty() ) { return s1; }

      ExportedSelection<> retval{s1.zipIdentifier(), Zipping::details::alwaysFalse};
      retval.m_indices.reserve( s1.size() );
      // Use std::set_difference to combine s1.m_indices and s2.m_indices into a new index container
      std::set_difference( s1.m_indices.begin(), s1.m_indices.end(), s2.m_indices.begin(), s2.m_indices.end(),
                           std::back_inserter( retval.m_indices ) );
      return retval;
    }

    /**
     * @brief Provide a selection of all elements that are contained in exactly one of s1 and s2
     *
     * (Set theoretical) symmetric difference of the input selections. Performs semantic validation in debug build.
     *
     * @param s1 a Zipping::ExportedSelection
     * @param s2 a Zipping::ExportedSelection
     */
    friend ExportedSelection set_symmetric_difference( ExportedSelection const& s1, ExportedSelection const& s2 ) {
#ifndef DNDEBUG
      if ( s1.zipIdentifier() != s2.zipIdentifier() ) {
        throw GaudiException( "Performing set symmetric difference on different container families.",
                              details::typename_v<ExportedSelection<>>, StatusCode::FAILURE );
      }
#endif
      if ( s1.empty() ) { return s2; }
      if ( s2.empty() ) { return s1; }

      ExportedSelection<> retval{s1.zipIdentifier(), Zipping::details::alwaysFalse};
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
            typename = std::enable_if_t<has_semantic_zip_v<std::decay_t<CONTAINER>>>>
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
    gsl::not_null<const container_t*> m_container; ///< Container from which elements are selected
    /// Container of selected indices (member of a Zipping::ExportedSelection)
    gsl::not_null<const index_vector*> m_indices;

    // Custom iterator class for looping through the index vector but
    // dereferencing to values in the actual container
    struct const_iterator {
      using index_iter_type   = typename index_vector::const_iterator;
      using value_type        = typename CONTAINER::proxy;
      using difference_type   = typename index_iter_type::difference_type;
      using iterator_category = std::random_access_iterator_tag;
      // provide these to enable STL algorithms
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

    /**
     * @brief Create an ExportedSelection for further exchange
     *
     * This creates a copy of the ExportedSelection from which the SelectionView got created.
     *
     * @return An ExportedSelection (no internal references/pointers to objects that might go out of context)
     */
    ExportedSelection<IndexSize> export_selection() {
      return ExportedSelection<IndexSize>( *m_indices, m_container->zipIdentifier() );
    }

    /**
     * @brief Turn ExportedSelection into SelectionView that grants access to container elements.
     *
     * @param container ZipContainer with the actual data
     * @param selection ExportedSelection with the elements that are to be accessed
     */
    SelectionView( const container_t* container, const ExportedSelection<IndexSize>& selection )
        : m_container{container}, m_indices{&selection.m_indices} {
#ifndef NDEBUG
      if ( selection.zipIdentifier() != container->zipIdentifier() ) {
        throw GaudiException( "Building SelectionView from an ExportedSelection of a different container family.",
                              details::typename_v<SelectionView>, StatusCode::FAILURE );
      }
#endif
    }

    /**
     * @brief iterator to the first selected element
     *
     * const in any case since SelectionView only provide a look, no touch, at the data
     */
    const_iterator begin() const { return {m_container, m_indices->begin()}; }
    /**
     * @brief const iterator to the first selected element (same as begin(), but provided for stl container conformity)
     */
    const_iterator cbegin() const { return begin(); }

    /**
     * @brief iterator to the after-last selected element
     *
     * const in any case since SelectionView only provide a look, no touch, at the data
     */
    const_iterator end() const { return {m_container, m_indices->end()}; }
    /**
     * @brief const iterator to the after-last selected element (same as end(), provided for stl container conformity)
     */
    const_iterator cend() const { return end(); }

    /**
     * @brief last selected element
     */
    proxy_type const back() const { return ( *m_container )[m_indices->back()]; }
    /**
     * @brief first selected element
     */
    proxy_type const front() const { return ( *m_container )[m_indices->front()]; }

    /**
     * @brief number of selected elements
     */
    std::size_t size() const { return m_indices->size(); }

    /**
     * @brief if the selection is empty (true: zero selected elements; false: more than zero elements)
     */
    bool empty() const { return m_indices->empty(); }

    /**
     * @brief the n-th selected element
     */
    proxy_type const operator[]( std::size_t n ) const {
      assert( n < size() );
      return ( *m_container )[( *m_indices )[n]];
    }

    /**
     * @brief Compares two selection views (requires the *same* backing container)
     *
     * The comparison falls back to equality of the selected indices and falls back to the == operator of the underlying
     * SOA::View.
     *
     * @return true if the SelectionViews are equal
     */
    friend bool operator==( SelectionView const& lhs, SelectionView const& rhs ) {
      return lhs.m_container == rhs.m_container && ( *lhs.m_indices ) == ( *rhs.m_indices );
    }

    /**
     * @brief Compares two selection views (requires the *same* backing container)
     *
     * The comparison falls back to equality of the selected indices and falls back to the == operator of the underlying
     * SOA::View.
     *
     * @return true if the SelectionViews are unequal
     */
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
      ExportedSelection<IndexSize> retval( m_container->zipIdentifier(), details::alwaysFalse );
      retval.m_indices.reserve( reserveCapacity < 0 ? m_indices->size() : reserveCapacity );
      std::copy_if( m_indices->begin(), m_indices->end(), std::back_inserter( retval.m_indices ),
                    [&]( auto i ) { return predicate( ( *m_container )[i] ); } );
      return retval;
    }
  };

  /**
   * @brief standard method to create a selection
   *
   * This creates an `ExportedSelection`. To immediately use the selection (as
   * `SelectionView`), create a SelectionView manually.
   *
   * @tparam CONTAINER  type of `Zipping::ZipContainer`from which objects will be selected (automatically detected)
   * @tparam Predicate  callable type to specify the selection (automatically detected)
   * @tparam IndexSize  variable type for indexing (default uint16_t is good enough for 65536 input objects)
   * @param container   container from which objects are selected
   * @param predicate   selection criterion, typically a lambda `[](auto obj) -> bool { ... return ...;}`
   *
   * @return ExportedSelection to select objects for which the predicate returns true.
   */
  template <typename CONTAINER, typename Predicate = details::alwaysTrue_t, typename IndexSize = uint16_t>
  ExportedSelection<IndexSize> makeSelection( const CONTAINER* container, Predicate&& predicate = {},
                                              int reserveCapacity = -1 ) {
    using container_t = std::decay_t<CONTAINER>;
    if ( container->size() >= typename container_t::size_type( std::numeric_limits<IndexSize>::max() ) ) {
      throw GaudiException{"Index overflow: " + std::to_string( container->size() - 1 ) + " > " +
                               std::to_string( std::numeric_limits<IndexSize>::max() ) +
                               details::typename_v<SelectionView<container_t>>,
                           details::typename_v<SelectionView<container_t>>, StatusCode::FAILURE};
    }
    ExportedSelection<IndexSize> retval( container->zipIdentifier(), details::alwaysFalse );

    if constexpr ( std::is_same_v<std::decay_t<Predicate>, details::alwaysTrue_t> ) {
      retval.m_indices.resize( container->size() );
      std::iota( retval.m_indices.begin(), retval.m_indices.end(), 0 );
    } else if constexpr ( std::is_same_v<std::decay_t<Predicate>, details::alwaysFalse_t> ) {
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
  bool equal_values( S const& lhs, typename S::container_t const& rhs ) {
    return std::equal( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
  }

  template <typename S, typename = requireSelection<S>>
  bool equal_values( typename S::container_t const& lhs, S const& rhs ) {
    return equal_values( rhs, lhs );
  }
} // namespace Zipping

#endif
