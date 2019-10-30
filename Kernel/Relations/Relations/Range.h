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
#ifndef RELATIONS_RANGE_H
#define RELATIONS_RANGE_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <cstddef>
#include <utility>
// ============================================================================
// Relations
// ============================================================================
#include "Relations/Bases.h"
// ============================================================================

namespace Relations {
  /** @struct Range_
   *
   *  An auxillary structure to provide a little bit
   *  better access to the components of standard
   *  @p std::pair class
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   27/01/2002
   */
  template <class ENTRIES>
  struct Range_ : Relations::BaseRange {
    // useful types
    using iterator               = typename ENTRIES::const_iterator;
    using const_iterator         = typename ENTRIES::const_iterator;
    using const_reverse_iterator = typename ENTRIES::const_reverse_iterator;
    using reverse_iterator       = typename ENTRIES::const_reverse_iterator;
    using reference              = typename ENTRIES::const_reference;
    using const_reference        = typename ENTRIES::const_reference;
    using value_type             = typename ENTRIES::value_type;
    /// default constructor
    Range_() : Relations::BaseRange(), m_begin(), m_end(){};
    /// constructor
    Range_( iterator begin, iterator end ) : m_begin( begin ), m_end( end ){};
    Range_( const ENTRIES& e ) : Range_{e.begin(), e.end()} {}
    template <class T1, class T2>
    Range_( const std::pair<T1, T2>& r ) : Range_{r.first, r.second} {}
    /// begin-iterator (    const version)
    iterator begin() const { return this->m_begin; }
    /// end-iterator   (    const version)
    iterator end() const { return this->m_end; }
    /// reverse iterator "begin"
    reverse_iterator rbegin() const { return const_reverse_iterator( end() ); }
    /// reverse iterator "end"
    reverse_iterator rend() const { return const_reverse_iterator( begin() ); }
    /// front element (applied only for 'valid' ranges)
    const_reference front() const { return *begin(); }
    /// back  element (applied only for 'valid' ranges)
    const_reference back() const { return ( *this )( size() - 1 ); }
    /// number of relations
    [[nodiscard]] size_t size() const { return end() - begin(); }
    /// empty?
    [[nodiscard]] bool empty() const { return end() == begin(); }
    /// the access to the items by index
    const_reference operator()( const size_t index ) const { return *( begin() + index ); };
    /// the access to the items by index
    const_reference operator[]( const size_t index ) const { return ( *this )( index ); };
    /// the access to the items by index
    const_reference at( const size_t index ) const { return ( *this )( index ); };

  private:
    iterator m_begin;
    iterator m_end;
  };
} // namespace Relations

// ============================================================================
// The END
// ============================================================================
#endif // RELATIONS_RANGE_H
