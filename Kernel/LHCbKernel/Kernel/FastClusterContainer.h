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
// ============================================================================
#ifndef GAUDIROOT_FASTCLUSTERCONTAINER_H
#define GAUDIROOT_FASTCLUSTERCONTAINER_H
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <vector>
// ============================================================================
// Gaudi
// ============================================================================
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/ObjectContainerBase.h"
// ============================================================================
/** @class FastClusterContainer FastClusterContainer.h <dir>/FastClusterContainer.h
 *
 * Fast containers can only be used for containees not having a virtual table.
 * Though the container behaves like a vector and allows the access of objects
 * of type VISIBLE, the internal representation is of type INTERNAL.
 * This avoids unnecessary calls to the constructor of the VISIBLE provided of
 * course that the INTERNAL type maps ideally to a primitive.
 * The size of the INTERNAL and the VISIBLE type must be identical.
 *
 * @author  M.Frank
 * @version 1.0
 *
 */
template <typename VISIBLE, typename INTERNAL>
class FastClusterContainer final {
  // ==========================================================================
  /// static compile-time assertion
  static_assert( sizeof( VISIBLE ) == sizeof( INTERNAL ), "Cannot remap data of difference size!" );
  // static_assert( std::is_trivially_destructible<VISIBLE>::value, "visible type must be trivially destructable");
  static_assert( !std::has_virtual_destructor<VISIBLE>::value, "visible type must not have a virtual destructor" );
  // ==========================================================================
  using VD = typename std::vector<VISIBLE>;

public:
  using value_type             = typename VD::value_type;
  using iterator               = typename VD::iterator;
  using const_iterator         = typename VD::const_iterator;
  using reverse_iterator       = typename VD::reverse_iterator;
  using const_reverse_iterator = typename VD::const_reverse_iterator;
  using size_type              = typename VD::size_type;
  using difference_type        = typename VD::difference_type;
  using pointer                = typename VD::pointer;
  using const_pointer          = typename VD::const_pointer;
  using reference              = typename VD::reference;
  using const_reference        = typename VD::const_reference;
  using chan_type              = typename VISIBLE::chan_type;

private:
  /// Data holder
  std::vector<INTERNAL> m_data;

  inline std::vector<VISIBLE>& ext() {
    // type punning
    union {
      std::vector<INTERNAL>* i;
      std::vector<VISIBLE>*  v;
    };
    i = &m_data;
    return *v;
  }
  inline const std::vector<VISIBLE>& ext() const {
    // type punning
    union {
      const std::vector<INTERNAL>* i;
      const std::vector<VISIBLE>*  v;
    };
    i = &m_data;
    return *v;
  }

public:
  /// Standard constructor
  FastClusterContainer() = default;

  /// return iterator for beginning of mutable sequence
  iterator begin() { return ext().begin(); }
  /// return iterator for end of mutable sequence
  iterator end() { return ext().end(); }
  /// return iterator for beginning of nonmutable sequence
  const_iterator begin() const { return ext().begin(); }
  /// return iterator for end of nonmutable sequence
  const_iterator end() const { return ext().end(); }
  /// return iterator for beginning of reversed mutable sequence
  reverse_iterator rbegin() { return ext().rbegin(); }
  /// return iterator for beginning of reversed nonmutable sequence
  const_reverse_iterator rbegin() const { return ext().rbegin(); }
  /// return iterator for end of reversed mutable sequence
  reverse_iterator rend() { return ext().rend(); }
  /// return iterator for end of reversed nonmutable sequence
  const_reverse_iterator rend() const { return ext().rend(); }
  /// subscript mutable sequence
  reference operator[]( size_type i ) { return ext().operator[]( i ); }
  /// subscript nonmutable sequence
  const_reference operator[]( size_type i ) const { return ext().operator[]( i ); }
  /// subscript mutable sequence with checking
  reference at( size_type i ) { return ext().at( i ); }
  /// subscript nonmutable sequence with checking
  const_reference at( size_type i ) const { return ext().at( i ); }
  /// test if sequence is empty
  [[nodiscard]] bool empty() const { return m_data.empty(); }
  /// return first element of mutable sequence
  reference front() { return ext().front(); }
  /// return first element of nonmutable sequence
  const_reference front() const { return ext().front(); }
  /// return last element of mutable sequence
  reference back() { return ext().back(); }
  /// return last element of nonmutable sequence
  const_reference back() const { return ext().back(); }
  /// insert element at end
  void push_back( const VISIBLE& val ) { ext().push_back( val ); }
  /// emplace element at end
  template <typename... Args>
  void emplace_back( Args&&... args ) {
    ext().emplace_back( std::forward<Args>( args )... );
  }
  /// erase element at end
  void pop_back() { m_data.pop_back(); }

  /// Return current length of allocated storage
  size_type size() const { return m_data.size(); }
  /// Return current length of allocated storage
  size_type capacity() const { return m_data.capacity(); }
  /// Clear container. Note: No drestructors may be called !
  void clear() { return m_data.clear(); }
  /// Reserve container space
  void reserve( size_type len ) { return m_data.reserve( len ); }
  /// Resize container entries
  void resize( size_type len ) { return m_data.resize( len ); }
  /// Resize container entries
  void resize( size_type len, const VISIBLE& def ) { return m_data.resize( len, *(const INTERNAL*)&def ); }

  /// find method
  template <class findPolicy>
  const_iterator find( const typename findPolicy::comp_type& value ) const {
    auto i = std::equal_range( begin(), end(), value, findPolicy() );
    return ( i.first != i.second ) ? i.first : end();
  }

  /// object method, adding a direct access by key using a customised lower_bound (assumes list sorted)
  const value_type* object( const chan_type& id ) const {
    const_iterator it = find<typename VISIBLE::findPolicy>( id );
    return it != end() ? &( *it ) : nullptr;
  }
};
// ============================================================================
// The END
// ============================================================================
#endif // GAUDIROOT_FASTCLUSTERCONTAINER_H
