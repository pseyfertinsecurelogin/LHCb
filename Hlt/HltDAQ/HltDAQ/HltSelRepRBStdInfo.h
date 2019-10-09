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
#include <ostream>
#include <utility>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class HltSelRepRBStdInfo HltSelRepRBStdInfo.h
   *
   * Manages the RawBank::HltSelReports Std Info sub-bank. The first word
   * contains number of objects stored (less significant short) and allocated
   * size of the sub-bank in words (more significant short). The rest consists of
   * two sectors: number of words per object (8-bits), and numerical words
   * (32-bits). Numerical info must be passed as unsigned int (even if it is a
   * float) to allow for possible commpression. The one may be padded to start a
   * new sector at a full word. Info can only be read and stored sequencially.
   *
   * @author Tomasz Skwarnicki
   *
   */

  class HltSelRepRBStdInfo {
  public:
    /// Used in input/output
    typedef std::vector<unsigned int> StdInfo;

    /// takes pointer to the memory location of the subbank body as input
    HltSelRepRBStdInfo( unsigned int* pBankBody );

    /// Default Constructor
    HltSelRepRBStdInfo() : m_location( 0 ), m_iterator( 0 ), m_iteratorInfo( 0 ), m_floatLoc( 0 ) {}

    /// Default Destructor
    virtual ~HltSelRepRBStdInfo() {}

    /// length of std info for object specified by index
    unsigned int sizeInfo( unsigned int iObj ) const;

    /// initialize existing complete bank for reading
    bool initialize();

    /// Number of objects in the sub-banks
    unsigned int numberOfObj() const;

    /// actual (computed) size in words
    unsigned int size() const;

    /// replace allocated size by actual size before saving (no write access after that - deleteBank)
    void saveSize() const;

    /// throw exception if no Bank or index invalid
    void validateIndex( unsigned int iObj ) const;

    /// reports bank integrity level 0=OK
    unsigned int integrityCode() const;

    /// gets substructure from current iterator position, advances iterator to next position
    StdInfo next();

    /// reset iterator to the initial position
    void rewind();

    /// Add StdInfo its content to current iterator position. The later is advanced. Bank must be properly initialized
    /// with size info.
    void push_back( const StdInfo& stdInfo );

    /// delete the bank from program memory
    void deleteBank();

    /// create a new bank to hold nObj and max of nAllInfo info words, initialize for writing (does not delete old bank
    /// if any)
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

    /// Retrieve const  location of the first float word inside the bank
    unsigned int floatLoc() const;

    friend std::ostream& operator<<( std::ostream& str, const HltSelRepRBStdInfo& obj ) {
      return obj.fillStream( str );
    }

  protected:
  private:
    /// length of the subbank in long words
    unsigned int sizeStored() const;

    /// length of the subbank in long words
    unsigned int sizeAllocated() const;

    /// throw exception on access to non-allocated bank
    void noBank() const;

    unsigned int* m_location;     ///< Location of the subbank body in the program memory
    unsigned int  m_iterator;     ///< internal iterator - object count iterator
    unsigned int  m_iteratorInfo; ///< internal iterator - info count iterator
    unsigned int  m_floatLoc;     ///< location of the first float word inside the bank

  }; // class HltSelRepRBStdInfo

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::HltSelRepRBStdInfo::HltSelRepRBStdInfo( unsigned int* pBankBody )
    : m_location( pBankBody ), m_iterator( 0 ), m_iteratorInfo( 0 ), m_floatLoc( 0 ) {

  if ( m_location ) initialize();
}

inline const unsigned int* LHCb::HltSelRepRBStdInfo::location() const { return m_location; }

inline void LHCb::HltSelRepRBStdInfo::setLocation( unsigned int* value ) { m_location = value; }

inline unsigned int LHCb::HltSelRepRBStdInfo::iterator() const { return m_iterator; }

inline unsigned int LHCb::HltSelRepRBStdInfo::iteratorInfo() const { return m_iteratorInfo; }

inline unsigned int LHCb::HltSelRepRBStdInfo::floatLoc() const { return m_floatLoc; }

inline unsigned int LHCb::HltSelRepRBStdInfo::sizeInfo( unsigned int iObj ) const {

  noBank();
  validateIndex( iObj );
  auto bits = 8 * ( iObj % 4 );
  return ( m_location[1 + ( iObj / 4 )] >> bits ) & 0xFFu;
}

inline bool LHCb::HltSelRepRBStdInfo::initialize() {

  if ( m_floatLoc == 0 ) {
    m_floatLoc = 1 + ( 3 + numberOfObj() ) / 4;
    rewind();
  }
  return true;
}

inline unsigned int LHCb::HltSelRepRBStdInfo::numberOfObj() const {

  noBank();
  return (unsigned int)( m_location[0] & 0xFFFFu );
}

inline unsigned int LHCb::HltSelRepRBStdInfo::size() const {

  auto         nObj = numberOfObj();
  unsigned int len  = 1 + ( 3 + nObj ) / 4;
  for ( unsigned int i = 0; i != nObj; ++i ) {
    auto bits = 8 * ( i % 4 );
    len += ( m_location[1 + ( i / 4 )] >> bits ) & 0xFFu;
  }
  return len;
}

inline void LHCb::HltSelRepRBStdInfo::saveSize() const {

  noBank();
  unsigned int s = size(); // size() looks at allocated size so calculate it before we mess up with allocated
  m_location[0] &= 0xFFFFu;
  m_location[0] |= ( std::min( s, 0xFFFFu ) << 16 );
}

inline void LHCb::HltSelRepRBStdInfo::validateIndex( unsigned int iObj ) const {

  noBank();
  if ( iObj >= numberOfObj() )
    throw GaudiException( "sub-bank index out of range in HltSelRepRawBank", "HltSelRepRawBank", StatusCode::FAILURE );
}

inline unsigned int LHCb::HltSelRepRBStdInfo::integrityCode() const {

  if ( !m_location ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kNoBank );
  // empty bank is OK: if( !numberOfObj() )return ((unsigned int)HltSelRepRBEnums::IntegrityCodes::kEmptyBank);
  auto s = sizeAllocated();
  if ( s == 0 ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kZeroAllocatedSize );

  unsigned int iteraInfo = 0;
  for ( unsigned int itera = 0; itera != numberOfObj(); ) {

    unsigned int iWord = 1 + itera / 4;
    if ( iWord >= s ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kPtrOutOfRange );

    unsigned int nInfo = sizeInfo( itera );
    ++itera;

    for ( unsigned int i = 0; i != nInfo; ++i ) {

      unsigned int iWord2 = m_floatLoc + iteraInfo;
      if ( iWord2 >= s ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kPtrOutOfRange );

      ++iteraInfo;
    }
  }
  return HltSelRepRBEnums::IntegrityCodes::kBankIsOK;
}

inline LHCb::HltSelRepRBStdInfo::StdInfo LHCb::HltSelRepRBStdInfo::next() {

  noBank();
  if ( !numberOfObj() )
    throw GaudiException( "Iterating in empty bank in HltSelRepRBStdInfo", "HltSelRepRBStdInfo", StatusCode::FAILURE );
  if ( !m_floatLoc ) initialize();

  unsigned int s = sizeAllocated();

  unsigned int nInfo = sizeInfo( m_iterator );
  ++m_iterator;

  StdInfo stdInfo;

  for ( unsigned int i = 0; i != nInfo; ++i ) {

    unsigned int iWord2 = m_floatLoc + m_iteratorInfo;
    if ( iWord2 >= s )
      throw GaudiException( "Iterating beyond extend of the bank in HltSelRepRBStdInfo", "HltSelRepRBStdInfo",
                            StatusCode::FAILURE );

    ++m_iteratorInfo;

    stdInfo.push_back( m_location[iWord2] );
  }
  return stdInfo;
}

inline void LHCb::HltSelRepRBStdInfo::rewind() {

  m_iterator     = 0;
  m_iteratorInfo = 0;
}

inline void LHCb::HltSelRepRBStdInfo::push_back( const StdInfo& stdInfo ) {

  auto s = sizeStored();
  if ( s == 0 )
    throw GaudiException( "Pushing into empty bank in HltSelRepRBStdInfo", "HltSelRepRBStdInfo", StatusCode::FAILURE );
  if ( m_floatLoc == 0 )
    throw GaudiException( "Pushing into uninitialized bank in HltSelRepRBStdInfo", "HltSelRepRBStdInfo",
                          StatusCode::FAILURE );

  unsigned int nInfo = std::min( stdInfo.size(), 255ul ); // can't store more than this; non-essential output anyway

  unsigned int iObj  = numberOfObj();
  unsigned int iWord = 1 + ( iObj / 4 );
  if ( iWord >= m_floatLoc )
    throw GaudiException( "Iterating beyond extend of count table in HltSelRepRBStdInfo", "HltSelRepRBStdInfo",
                          StatusCode::FAILURE );
  m_location[0] = ( m_location[0] & ~0xFFFFL ) | ( iObj + 1 );

  unsigned int iPart = iObj % 4;
  unsigned int bits  = iPart * 8;
  unsigned int mask  = 0xFFL << bits;
  m_location[iWord]  = ( ( m_location[iWord] & ~mask ) | ( nInfo << bits ) );

  ++m_iterator;

  for ( unsigned int i = 0; i != nInfo; ++i ) {

    unsigned int iWord2 = m_floatLoc + m_iteratorInfo;
    if ( iWord2 >= s && s != 0xFFFFu ) // FIXME: skip check in case of overflow... in order to avoid breaking strict
                                       // backwards compatibility...
      throw GaudiException( "Iterating beyond extend of the bank in HltSelRepRBStdInfo", "HltSelRepRBStdInfo",
                            StatusCode::FAILURE );

    ++m_iteratorInfo;

    m_location[iWord2] = stdInfo[i];
  }
}

inline void LHCb::HltSelRepRBStdInfo::deleteBank() {

  delete[] m_location;
  m_location = nullptr;
}

inline bool LHCb::HltSelRepRBStdInfo::initialize( unsigned int nObj, unsigned int nAllInfo ) {

  unsigned int len = 1 + ( 3 + nObj ) / 4 + nAllInfo;
  // if( len  > 0xFFFFL )return false;
  m_location    = new unsigned int[len];
  m_location[0] = ( std::min( len, 0xFFFFu ) << 16 );
  // Initialize the potential padding bytes for reproducibility
  std::fill_n( m_location + 1, len - 1, 0u );
  m_floatLoc = 1 + ( 3 + nObj ) / 4;
  rewind();
  return true;
}

inline std::ostream& LHCb::HltSelRepRBStdInfo::fillStream( std::ostream& s ) const {

  unsigned int ic = integrityCode();
  s << " HltSelRepRBStdInfo : { "
    << " nObj " << numberOfObj() << " size Allocated " << sizeAllocated() << " Actual " << size() << " integrity code "
    << ic << ( " " + HltSelRepRBEnums::IntegrityCodesToString( ic ) ) << std::endl;
  if ( 0 == ic ) {

    unsigned int iteraInfo = 0;
    for ( unsigned int itera = 0; itera != numberOfObj(); ) {

      unsigned int nInfo = sizeInfo( itera );
      ++itera;

      s << " " << itera << " nInfo " << nInfo << " { ";

      for ( unsigned int i = 0; i != nInfo; ++i ) {

        unsigned int iWord2 = m_floatLoc + iteraInfo;

        ++iteraInfo;

        // assume uncompressed info
        union IntFloat {
          unsigned int mInt;
          float        mFloat;
        };
        IntFloat a;
        a.mInt         = m_location[iWord2];
        float infFloat = a.mFloat;

        s << infFloat << " ";
      }
      s << " } " << std::endl;
    }
  }
  s << " }" << std::endl;
  return s;
}

inline unsigned int LHCb::HltSelRepRBStdInfo::sizeStored() const {

  noBank();
  return ( m_location[0] >> 16 ) & 0xFFFFu;
}

inline unsigned int LHCb::HltSelRepRBStdInfo::sizeAllocated() const {

  auto s = sizeStored();
  return s != 0xFFFFu ? s : size(); // in case of 'overflow' of the stored size, return the _computed_ size from the
                                    // content instead
}

inline void LHCb::HltSelRepRBStdInfo::noBank() const {

  if ( !m_location )
    throw GaudiException( "Accessing non-allocated bank in HltSelRepRBStdInfo", "HltSelRepRBStdInfo",
                          StatusCode::FAILURE );
}
