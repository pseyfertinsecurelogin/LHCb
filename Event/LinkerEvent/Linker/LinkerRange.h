/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef LINKER_LINKERRANGE_H 
#define LINKER_LINKERRANGE_H 1

// Include files
#include "Linker/LinkerEntry.h"
#include <vector>

/** @class LinkerRange LinkerRange.h Linker/LinkerRange.h
 *  Holds a range, a collection of LinkerEntry  
 *
 *  @author Olivier Callot
 *  @date   2005-01-19
 */

template <class SOURCE, class TARGET>
class LinkerRange final {
public: 

  typedef typename std::vector<LinkerEntry<SOURCE,TARGET> >::const_iterator iterator;
  
  /** Add a LinkerEntry inside the range
   *  @param src    pointer to the SOURCE of the entry
   *  @param tgt    pointer to the TARGET of the entry
   *  @param weight Weight of the relation
   */
  void addEntry( const SOURCE* src, const TARGET* tgt, double weight ) {
    emplace_back( src, tgt, weight );
  }

  template <typename... Args>
  void emplace_back( Args&&... args ) {
    m_entries.emplace_back( std::forward<Args>(args)... );
  }

  void push_back( const LinkerEntry<SOURCE,TARGET>& le ) {
    m_entries.push_back( le );
  }

  void push_back( LinkerEntry<SOURCE,TARGET>&& le ) {
    m_entries.push_back( std::move(le) );
  }

  /** returns an iterator to the beginning of the table of entries = range
   *  @return  iterator
   */
  iterator begin() const { return m_entries.begin(); }

  /** returns an iterator to the end of the table of entries = range
   *  @return  iterator
   */
  iterator end()   const { return m_entries.end(); }

  /** returns the size of the range, the number of entries
   *  @return  size
   */
  int size()       const { return m_entries.size(); }

  /** returns true if the range is empty
   *  @return  is range empty ?
   */
  bool empty()     const { return m_entries.empty(); }

  /** clear the range, remove all entries
   */
  void clear()           { m_entries.clear(); }

  /** returns a reference to the first element
   */
  const LinkerEntry<SOURCE,TARGET>& front() const { return m_entries.front(); }

  /** returns a reference to the last element
   */
  const LinkerEntry<SOURCE,TARGET>& back() const { return m_entries.back(); }

private:
  std::vector<LinkerEntry<SOURCE,TARGET> > m_entries;
};
#endif // LINKER_LINKERRANGE_H
