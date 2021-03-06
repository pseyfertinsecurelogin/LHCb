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
#ifndef KERNEL_COUNTITERATOR_H
#define KERNEL_COUNTITERATOR_H 1
// ============================================================================
// STD & STL
// ============================================================================
#include <iterator>
// ============================================================================
/** @class CountIterator CountIterator.h Kernel/CountIterator.h
 *  Trivial "count"-iterator: output iterator that only counts
 *
 *  @author Vanya Belyaev
 *  @date   2015-03-17
 */
template <typename TYPE>
class count_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void> {
public:
  // ========================================================================
  /// Constructor
  count_iterator() = default;
  //
  count_iterator& operator=( const TYPE& /* value */ ) {
    ++m_count;
    return *this;
  }
  //
  count_iterator& operator*() { return *this; }
  count_iterator& operator++() { return *this; }
  count_iterator& operator++( int ) { return *this; }
  //
  // ========================================================================
public:
  // ==========================================================================
  /// the the counter
  [[nodiscard]] unsigned long count() const { return m_count; }
  // ==========================================================================
private:
  ///
  unsigned long m_count{0};
  // ==========================================================================
};
// ============================================================================
// The END
// ============================================================================
#endif // KERNEL_COUNTITERATOR_H
