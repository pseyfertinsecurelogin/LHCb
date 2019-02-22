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
#ifndef EVENT_SMARTBANK_H
#define EVENT_SMARTBANK_H 1

// Include files
#include <stddef.h>

/** @class LHCb::SmartBank SmartBank.h Event/SmartBank.h
 *  Convert a bank (e.g. RawBank or subpart) to iterators over short or char
 *
 *  @author Matt Needham
 *  @date   2005-10-13
 */

namespace LHCb {
  template <class TYPE>
  class SmartBank final {

  public:
    typedef const TYPE* iterator;

    /**
     *  Constructor
     * @param data     pointer to data
     * @param dataSize size in bytes
     */
    SmartBank( unsigned int* data, size_t dataSize );

    /** iterator to start of data
     * @return begin
     */
    iterator begin() const;

    /** iterator to end of data
     * @return end
     */
    iterator end() const;

  private:
    TYPE*  m_data;
    size_t m_size;
  };

  template <class TYPE>
  inline typename SmartBank<TYPE>::iterator SmartBank<TYPE>::begin() const {
    return m_data;
  }

  template <class TYPE>
  inline typename SmartBank<TYPE>::iterator SmartBank<TYPE>::end() const {
    return begin() + m_size;
  }

  template <class TYPE>
  inline SmartBank<TYPE>::SmartBank( unsigned int* data, size_t dataSize ) {

    m_data              = reinterpret_cast<TYPE*>( data );
    unsigned int dBytes = sizeof( TYPE ) / sizeof( char );
    m_size              = dataSize / dBytes;
  }
} // namespace LHCb

#endif // EVENT_SMARTBANK_H
