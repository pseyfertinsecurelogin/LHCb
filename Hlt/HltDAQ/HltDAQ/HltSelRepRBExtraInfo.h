/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

// Include files
#include "GaudiKernel/GaudiException.h"
#include "HltSelRepRBEnums.h"
#include <algorithm>
#include <ostream>
#include <utility>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class HltSelRepRBExtraInfo HltSelRepRBExtraInfo.h
   *
   * Manages the RawBank::HltSelReports Extra Info sub-bank. The first word
   * contains number of objects stored (less significant short) and size of the
   * sub-bank in words (more significant short). The rest consists of three
   * sectors: number per object (8-bits), info-keys (16-bits) and floats
   * (32-bits). The first two may be padded to start a new sector at a full word.
   * Info can only be read and stored sequencially.
   *
   * @author Tomasz Skwarnicki
   *
   */

  class HltSelRepRBExtraInfo {
  public:
    /// Used in input/output
    typedef std::pair<unsigned short, float> Inf;
    /// Used in input/output
    typedef std::vector<Inf> ExtraInfo;

    /// takes pointer to the memory location of the subbank body as input
    HltSelRepRBExtraInfo( unsigned int* pBankBody );

    /// Default Constructor
    HltSelRepRBExtraInfo() : m_location( 0 ), m_iterator( 0 ), m_iteratorInfo( 0 ), m_infoLoc( 0 ), m_floatLoc( 0 ) {}

    /// Default Destructor
    virtual ~HltSelRepRBExtraInfo() {}

    /// length of extra info for object specified by index
    unsigned int sizeInfo( unsigned int iObj ) const;

    /// initialize existing complete bank for reading
    bool initialize();

    /// Number of objects in the sub-banks
    unsigned int numberOfObj() const;

    /// length of the subbank in long words
    unsigned int size() const;

    /// throw exception if no Bank or index invalid
    void validateIndex( unsigned int iObj ) const;

    /// reports bank integrity level 0=OK
    unsigned int integrityCode() const;

    /// gets substructure from current iterator position, advances iterator to next position
    ExtraInfo next();

    /// reset iterator to the initial position
    void rewind();

    /// Add ExtraInfo its content to current iterator position. The later is advanced. Bank must be properly initialized
    /// with size info.
    void push_back( const ExtraInfo& extraInfo );

    /// delete the bank from program memory
    void deleteBank();

    /// create a new bank to hold nObj and nAllInfo infos, and initialize for writing (does not delete old bank if any)
    bool initialize( unsigned int nObj, unsigned int nAllInfo );

    /// intelligent printout
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  Location of the subbank body in the program memory
    const unsigned int* location() const;

    /// Update  Location of the subbank body in the program memory
    void setLocation( unsigned int* value );

    /// Retrieve const  internal iterator - object count iterator
    unsigned int iterator() const;

    /// Retrieve const  internal iterator - info count iterator
    unsigned int iteratorInfo() const;

    /// Retrieve const  location of the first info word inside the bank
    unsigned int infoLoc() const;

    /// Retrieve const  location of the first float word inside the bank
    unsigned int floatLoc() const;

    friend std::ostream& operator<<( std::ostream& str, const HltSelRepRBExtraInfo& obj ) {
      return obj.fillStream( str );
    }

  protected:
  private:
    /// throw exception on access to non-allocated bank
    void noBank() const;

    unsigned int* m_location;     ///< Location of the subbank body in the program memory
    unsigned int  m_iterator;     ///< internal iterator - object count iterator
    unsigned int  m_iteratorInfo; ///< internal iterator - info count iterator
    unsigned int  m_infoLoc;      ///< location of the first info word inside the bank
    unsigned int  m_floatLoc;     ///< location of the first float word inside the bank

  }; // class HltSelRepRBExtraInfo

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::HltSelRepRBExtraInfo::HltSelRepRBExtraInfo( unsigned int* pBankBody )
    : m_location( pBankBody ), m_iterator( 0 ), m_iteratorInfo( 0 ), m_infoLoc( 0 ), m_floatLoc( 0 ) {

  if ( m_location ) initialize();
}

inline const unsigned int* LHCb::HltSelRepRBExtraInfo::location() const { return m_location; }

inline void LHCb::HltSelRepRBExtraInfo::setLocation( unsigned int* value ) { m_location = value; }

inline unsigned int LHCb::HltSelRepRBExtraInfo::iterator() const { return m_iterator; }

inline unsigned int LHCb::HltSelRepRBExtraInfo::iteratorInfo() const { return m_iteratorInfo; }

inline unsigned int LHCb::HltSelRepRBExtraInfo::infoLoc() const { return m_infoLoc; }

inline unsigned int LHCb::HltSelRepRBExtraInfo::floatLoc() const { return m_floatLoc; }

inline unsigned int LHCb::HltSelRepRBExtraInfo::sizeInfo( unsigned int iObj ) const {

  noBank();
  validateIndex( iObj );
  unsigned int iWord = 1 + iObj / 4;
  unsigned int iPart = iObj % 4;
  unsigned int bits  = iPart * 8;
  unsigned int mask  = 0xFFL << bits;
  return (unsigned int)( ( m_location[iWord] & mask ) >> bits );
}

inline bool LHCb::HltSelRepRBExtraInfo::initialize() {

  if ( m_floatLoc ) return true;
  unsigned int nObj = numberOfObj();
  m_infoLoc         = 1;
  if ( nObj ) m_infoLoc += 1 + ( nObj - 1 ) / 4;
  unsigned int nInfo = 0;
  for ( unsigned int iObj = 0; iObj != nObj; ++iObj ) { nInfo += sizeInfo( iObj ); }
  m_floatLoc = m_infoLoc;
  if ( nInfo ) m_floatLoc += ( nInfo - 1 ) / 2 + 1;
  rewind();
  return true;
}

inline unsigned int LHCb::HltSelRepRBExtraInfo::numberOfObj() const {

  noBank();
  return (unsigned int)( m_location[0] & 0xFFFFL );
}

inline unsigned int LHCb::HltSelRepRBExtraInfo::size() const {

  noBank();
  return (unsigned int)( ( m_location[0] & 0xFFFF0000L ) >> 16 );
}

inline void LHCb::HltSelRepRBExtraInfo::validateIndex( unsigned int iObj ) const {

  noBank();
  if ( iObj >= numberOfObj() )
    throw GaudiException( "sub-bank index out of range in HltSelRepRawBank", "HltSelRepRawBank", StatusCode::FAILURE );
}

inline unsigned int LHCb::HltSelRepRBExtraInfo::integrityCode() const {

  if ( !m_location ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kNoBank );
  // empty bank is OK: if( !numberOfObj() )return ((unsigned int)HltSelRepRBEnums::IntegrityCodes::kEmptyBank);
  if ( !size() ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kZeroAllocatedSize );
  unsigned int s = size();

  unsigned int iteraInfo = 0;
  for ( unsigned int itera = 0; itera != numberOfObj(); ) {

    unsigned int iWord = 1 + itera / 4;
    if ( iWord >= s ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kPtrOutOfRange );

    unsigned int nInfo = sizeInfo( itera );
    ++itera;

    for ( unsigned int i = 0; i != nInfo; ++i ) {

      unsigned int iWord1 = m_infoLoc + ( iteraInfo / 2 );
      if ( iWord1 >= s ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kPtrOutOfRange );

      unsigned int iWord2 = m_floatLoc + iteraInfo;
      if ( iWord2 >= s ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kPtrOutOfRange );

      ++iteraInfo;
    }
  }
  return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kBankIsOK );
}

inline LHCb::HltSelRepRBExtraInfo::ExtraInfo LHCb::HltSelRepRBExtraInfo::next() {

  noBank();
  if ( !numberOfObj() )
    throw GaudiException( "Iterating in empty bank in HltSelRepRBExtraInfo", "HltSelRepRBExtraInfo",
                          StatusCode::FAILURE );
  if ( !m_floatLoc ) initialize();

  unsigned int s = size();

  unsigned int nInfo = sizeInfo( m_iterator );
  ++m_iterator;

  ExtraInfo extraInfo;

  for ( unsigned int i = 0; i != nInfo; ++i ) {

    unsigned int iWord1 = m_infoLoc + ( m_iteratorInfo / 2 );
    unsigned int iPart1 = m_iteratorInfo % 2;
    if ( iWord1 >= s )
      throw GaudiException( "Iterating beyond extend of the bank in HltSelRepRBExtraInfo", "HltSelRepRBExtraInfo",
                            StatusCode::FAILURE );

    unsigned int iWord2 = m_floatLoc + m_iteratorInfo;
    if ( iWord2 >= s )
      throw GaudiException( "Iterating beyond extend of the bank in HltSelRepRBExtraInfo", "HltSelRepRBExtraInfo",
                            StatusCode::FAILURE );

    ++m_iteratorInfo;

    unsigned int   bits   = iPart1 * 16;
    unsigned int   mask   = 0xFFFFL << bits;
    unsigned short infKey = (unsigned short)( ( m_location[iWord1] & mask ) >> bits );

    union IntFloat {
      unsigned int mInt;
      float        mFloat;
    };
    IntFloat a;
    a.mInt         = m_location[iWord2];
    float infFloat = a.mFloat;

    Inf inf( infKey, infFloat );
    extraInfo.push_back( inf );
  }
  return extraInfo;
}

inline void LHCb::HltSelRepRBExtraInfo::rewind() {

  m_iterator     = 0;
  m_iteratorInfo = 0;
}

inline void LHCb::HltSelRepRBExtraInfo::push_back( const ExtraInfo& extraInfo ) {

  noBank();
  if ( !size() )
    throw GaudiException( "Pushing into empty bank in HltSelRepRBExtraInfo", "HltSelRepRBExtraInfo",
                          StatusCode::FAILURE );
  if ( !m_floatLoc )
    throw GaudiException( "Pushing into uninitialized bank in HltSelRepRBExtraInfo", "HltSelRepRBExtraInfo",
                          StatusCode::FAILURE );

  unsigned int s = size();

  unsigned int nInfo = extraInfo.size();
  if ( nInfo > 255 ) nInfo = 255; // can't store more than this; non-essential output anyway

  unsigned int iObj  = ( m_location[0] & 0xFFFFL );
  unsigned int iWord = 1 + iObj / 4;
  if ( iWord >= s )
    throw GaudiException( "Iterating beyond extend of the bank in HltSelRepRBExtraInfo", "HltSelRepRBExtraInfo",
                          StatusCode::FAILURE );
  unsigned int iPart = iObj % 4;
  ++iObj;
  m_location[0] = ( ( m_location[0] & ~0xFFFFL ) | iObj );

  unsigned int bits = iPart * 8;
  unsigned int mask = 0xFFL << bits;
  m_location[iWord] = ( ( m_location[iWord] & ~mask ) | ( nInfo << bits ) );

  ++m_iterator;

  for ( unsigned int i = 0; i != nInfo; ++i ) {

    unsigned int iWord1 = m_infoLoc + ( m_iteratorInfo / 2 );
    unsigned int iPart1 = m_iteratorInfo % 2;
    if ( iWord1 >= s )
      throw GaudiException( "Iterating beyond extend of the bank in HltSelRepRBExtraInfo", "HltSelRepRBExtraInfo",
                            StatusCode::FAILURE );

    unsigned int iWord2 = m_floatLoc + m_iteratorInfo;
    if ( iWord2 >= s )
      throw GaudiException( "Iterating beyond extend of the bank in HltSelRepRBExtraInfo", "HltSelRepRBExtraInfo",
                            StatusCode::FAILURE );

    ++m_iteratorInfo;

    unsigned int bits   = iPart1 * 16;
    unsigned int mask   = 0xFFFFL << bits;
    unsigned int infKey = extraInfo[i].first;

    m_location[iWord1] = ( ( m_location[iWord1] & ~mask ) | ( infKey << bits ) );

    union IntFloat {
      unsigned int mInt;
      float        mFloat;
    };
    IntFloat a;
    a.mFloat           = extraInfo[i].second;
    m_location[iWord2] = a.mInt;
  }
}

inline void LHCb::HltSelRepRBExtraInfo::deleteBank() {

  delete[] m_location;
  m_location = nullptr;
}

inline bool LHCb::HltSelRepRBExtraInfo::initialize( unsigned int nObj, unsigned int nAllInfo ) {

  unsigned int len = 2;
  if ( nObj ) len += ( nObj - 1 ) / 4;
  if ( nAllInfo ) len += 1 + ( nAllInfo - 1 ) / 2 + nAllInfo;
  if ( len > 0xFFFFL ) return false;
  m_location = new unsigned int[len];
  // Initialize the potential padding bytes for reproducibility
  std::fill_n( m_location, len, 0 );
  // TODO: Make this more efficient by only initializing the padding
  // bytes. I (RM) don't know which are the padding bytes.
  m_location[0] = ( len << 16 );
  m_location[1] = 0;
  m_infoLoc     = 1;
  if ( nObj ) m_infoLoc += 1 + ( nObj - 1 ) / 4;
  m_floatLoc = m_infoLoc;
  if ( nAllInfo ) m_floatLoc += ( nAllInfo - 1 ) / 2 + 1;
  rewind();
  return true;
}

inline std::ostream& LHCb::HltSelRepRBExtraInfo::fillStream( std::ostream& s ) const {

  unsigned int ic = integrityCode();
  s << " HltSelRepRBExtraInfo : { "
    << " nObj " << numberOfObj() << " size " << size() << " integrity code " << ic
    << ( " " + HltSelRepRBEnums::IntegrityCodesToString( ic ) ) << std::endl;
  if ( 0 == ic ) {

    unsigned int iteraInfo = 0;
    for ( unsigned int itera = 0; itera != numberOfObj(); ) {

      unsigned int nInfo = sizeInfo( itera );
      ++itera;

      s << " " << itera << " nInfo " << nInfo << " { ";

      for ( unsigned int i = 0; i != nInfo; ++i ) {

        unsigned int iWord1 = m_infoLoc + ( iteraInfo / 2 );
        unsigned int iPart1 = iteraInfo % 2;

        unsigned int iWord2 = m_floatLoc + iteraInfo;

        ++iteraInfo;

        unsigned int   bits   = iPart1 * 16;
        unsigned int   mask   = 0xFFFFL << bits;
        unsigned short infKey = (unsigned short)( ( m_location[iWord1] & mask ) >> bits );

        union IntFloat {
          unsigned int mInt;
          float        mFloat;
        };
        IntFloat a;
        a.mInt         = m_location[iWord2];
        float infFloat = a.mFloat;

        s << infKey << ": " << infFloat << " ";
      }
      s << " } " << std::endl;
    }
  }
  s << " }" << std::endl;
  return s;
}

inline void LHCb::HltSelRepRBExtraInfo::noBank() const {

  if ( !m_location )
    throw GaudiException( "Accessing non-allocated bank in HltSelRepRBExtraInfo", "HltSelRepRBExtraInfo",
                          StatusCode::FAILURE );
}
