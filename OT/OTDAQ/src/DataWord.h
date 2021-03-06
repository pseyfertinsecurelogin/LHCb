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

#ifndef OTDAQ_DataWord_H
#define OTDAQ_DataWord_H 1

// Include files
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class DataWord DataWord.h
   *
   * The Data Word is a word of 32 bits that containes the data of 2 hits of the
   * OT for the OTDAQ data format
   *
   * @author Jacopo Nardulli
   * created Wed Nov 14 17:03:07 2007
   *
   */

  class DataWord final {
  public:
    /// constructor with word, firstOtis, firstChannel, firstTime, null, nextOtis, nextChannel, nextTime,
    DataWord( int iWord, int ifirstOtis, int ifirstChannel, int ifirstTime, int iNull, int inextOtis, int inextChannel,
              int inextTime );

    /// constructor with DataWord
    DataWord( int id ) : m_dataWord( id ) {}

    /// Default Constructor
    DataWord() : m_dataWord( 0 ) {}

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const;

    /// Return an unsigned int from a DataWord
    unsigned int returnInt( const DataWord& dataWord );

    /// Retrieve const  Data Word ID
    unsigned int dataWord() const;

    /// Update  Data Word ID
    void setDataWord( unsigned int value );

    /// Retrieve nextTdcTime
    unsigned int nextTime() const;

    /// Retrieve nextChannelID
    unsigned int nextChannel() const;

    /// Retrieve nextOtisID
    unsigned int nextOtis() const;

    /// Retrieve Null
    bool null() const;

    /// Retrieve firstTdcTime
    unsigned int firstTime() const;

    /// Retrieve firstChannelIF
    unsigned int firstChannel() const;

    /// Retrieve firstOtisID
    unsigned int firstOtis() const;

    /// Retrieve Word
    bool word() const;

  protected:
  private:
    /// Offsets of bitfield dataWord
    enum dataWordBits {
      nextTimeBits     = 0,
      nextChannelBits  = 8,
      nextOtisBits     = 13,
      nullBits         = 15,
      firstTimeBits    = 16,
      firstChannelBits = 24,
      firstOtisBits    = 29,
      wordBits         = 31
    };

    /// Bitmasks for bitfield dataWord
    enum dataWordMasks {
      nextTimeMask     = 0xFFL,
      nextChannelMask  = 0x1F00L,
      nextOtisMask     = 0x6000L,
      nullMask         = 0x8000L,
      firstTimeMask    = 0xFF0000L,
      firstChannelMask = 0x1F000000L,
      firstOtisMask    = 0x60000000L,
      wordMask         = 0x80000000L
    };

    unsigned int m_dataWord; ///< Data Word ID

  }; // class DataWord

  inline std::ostream& operator<<( std::ostream& str, const DataWord& obj ) { return obj.fillStream( str ); }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::DataWord::DataWord( int iWord, int ifirstOtis, int ifirstChannel, int ifirstTime, int iNull, int inextOtis,
                                 int inextChannel, int inextTime ) {

  m_dataWord = ( iWord << wordBits ) + ( ifirstOtis << firstOtisBits ) + ( ifirstChannel << firstChannelBits ) +
               ( ifirstTime << firstTimeBits ) + ( iNull << nullBits ) + ( inextOtis << nextOtisBits ) +
               ( inextChannel << nextChannelBits ) + ( inextTime << nextTimeBits );
}

inline std::ostream& LHCb::DataWord::fillStream( std::ostream& s ) const {
  s << "{ "
    << "dataWord :	" << m_dataWord << std::endl
    << " }";
  return s;
}

inline unsigned int LHCb::DataWord::dataWord() const { return m_dataWord; }

inline void LHCb::DataWord::setDataWord( unsigned int value ) { m_dataWord = value; }

inline unsigned int LHCb::DataWord::nextTime() const { return ( m_dataWord & nextTimeMask ) >> nextTimeBits; }

inline unsigned int LHCb::DataWord::nextChannel() const { return ( m_dataWord & nextChannelMask ) >> nextChannelBits; }

inline unsigned int LHCb::DataWord::nextOtis() const { return ( m_dataWord & nextOtisMask ) >> nextOtisBits; }

inline bool LHCb::DataWord::null() const { return 0 != ( ( m_dataWord & nullMask ) >> nullBits ); }

inline unsigned int LHCb::DataWord::firstTime() const { return ( m_dataWord & firstTimeMask ) >> firstTimeBits; }

inline unsigned int LHCb::DataWord::firstChannel() const {
  return ( m_dataWord & firstChannelMask ) >> firstChannelBits;
}

inline unsigned int LHCb::DataWord::firstOtis() const { return ( m_dataWord & firstOtisMask ) >> firstOtisBits; }

inline bool LHCb::DataWord::word() const { return 0 != ( ( m_dataWord & wordMask ) >> wordBits ); }

inline unsigned int LHCb::DataWord::returnInt( const DataWord& dataWord ) { return dataWord.m_dataWord >> 0; }

#endif /// OTDAQ_DataWord_H
