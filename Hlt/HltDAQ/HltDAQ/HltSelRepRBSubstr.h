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

  /** @class HltSelRepRBSubstr HltSelRepRBSubstr.h
   *
   * Manages the RawBank::HltSelReports substructure sub-bank. All info is stored
   * in 16-bit shorts (packed in 32-bit words). The first short is a number of
   * substructures stored. The second short is the allocated length of the bank
   * (in 32-bit words). This is followed by sequencially stacked substructure
   * records. First short of the record gives number of substructure pointers in
   * the most significant 15 bits. The least significant bit of this short is a
   * flag 0=lhcbid ptrs 1=object ptrs. The rest of the record are pointers i.e.
   * either hit sequence index or stored object index. The bank should be written
   * and read sequencially. Never write into into the bank with iterator not at
   * its end-of-the bank position.
   *
   * @author Tomasz Skwarnicki
   *
   */

  class HltSelRepRBSubstr {
  public:
    /// Used in input/output first:type (0=objects,1=hits) second:pointers
    typedef std::vector<unsigned short> Substrv;
    /// Used in input/output first:type (0=objects,1=hits) second:pointers
    typedef std::pair<unsigned int, Substrv> Substr;

    /// default allocation size in 32-bit words for new bank or increment in its extension
    enum DefaultAllocation { kDefaultAllocation = 500 };
    /// iterator value to access the first element
    enum InitialPositionOfIterator { kInitialPosition = 2 };

    /// takes pointer to the memory location of the subbank body as input
    HltSelRepRBSubstr( unsigned int* pBankBody )
        : m_location( pBankBody ), m_iterator( InitialPositionOfIterator::kInitialPosition ) {}

    /// Default Constructor
    HltSelRepRBSubstr() : m_location( 0 ), m_iterator( InitialPositionOfIterator::kInitialPosition ) {}

    /// Default Destructor
    virtual ~HltSelRepRBSubstr() {}

    /// Number of hit substructures in the sub-banks
    unsigned int numberOfObj() const;

    /// length of the subbank in long words (allocated)
    unsigned int allocatedSize() const;

    /// replace allocated size by actual size before saving (no write access after that - deleteBank)
    void saveSize() const;

    /// type of substructure 0=objects 1=hits
    static unsigned int hitSubstr( unsigned short inpt );

    /// length of substructure
    static unsigned int lenSubstr( unsigned short inpt );

    /// reports bank integrity level 0=OK
    unsigned int integrityCode() const;

    /// gets substructure from current iterator position, advances iterator to next position
    Substr next();

    /// reset iterator to the initial position
    void rewind();

    /// actual size of the bank in long words
    unsigned int size() const;

    /// Add substructure copying its content to current iterator position. The later is advanced.
    bool push_back( const Substr& sub );

    /// delete the bank from program memory
    void deleteBank();

    /// clear bank content without deleting
    void clear();

    /// create a new bank with given allocation len and initialize it (does not delete old bank if any)
    void initialize( unsigned int len = DefaultAllocation::kDefaultAllocation );

    /// intelligent printout
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  Location of the subbank body in the program memory
    const unsigned int* location() const;

    /// Update  Location of the subbank body in the program memory
    void setLocation( unsigned int* value );

    /// Retrieve const  internal iterator - gives current position in number of shorts
    unsigned int iterator() const;

    friend std::ostream& operator<<( std::ostream& str, const HltSelRepRBSubstr& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// throw exception on access to non-allocated bank
    void noBank() const;

    unsigned int* m_location; ///< Location of the subbank body in the program memory
    unsigned int  m_iterator; ///< internal iterator - gives current position in number of shorts

  }; // class HltSelRepRBSubstr

  inline std::ostream& operator<<( std::ostream& s, LHCb::HltSelRepRBSubstr::DefaultAllocation e ) {
    switch ( e ) {
    case LHCb::HltSelRepRBSubstr::kDefaultAllocation:
      return s << "kDefaultAllocation";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::HltSelRepRBSubstr::DefaultAllocation";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::HltSelRepRBSubstr::InitialPositionOfIterator e ) {
    switch ( e ) {
    case LHCb::HltSelRepRBSubstr::kInitialPosition:
      return s << "kInitialPosition";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::HltSelRepRBSubstr::InitialPositionOfIterator";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const unsigned int* LHCb::HltSelRepRBSubstr::location() const { return m_location; }

inline void LHCb::HltSelRepRBSubstr::setLocation( unsigned int* value ) { m_location = value; }

inline unsigned int LHCb::HltSelRepRBSubstr::iterator() const { return m_iterator; }

inline unsigned int LHCb::HltSelRepRBSubstr::numberOfObj() const {

  noBank();
  return (unsigned int)( m_location[0] & 0xFFFFL );
}

inline unsigned int LHCb::HltSelRepRBSubstr::allocatedSize() const {

  noBank();
  return (unsigned int)( ( m_location[0] & 0xFFFF0000L ) >> 16 );
}

inline void LHCb::HltSelRepRBSubstr::saveSize() const {

  noBank();
  unsigned int s = size(); // size() looks at allocated size so calculate it before we mess up with allocated
  m_location[0] &= 0xFFFFL;
  m_location[0] |= ( s << 16 );
}

inline unsigned int LHCb::HltSelRepRBSubstr::hitSubstr( unsigned short inpt ) { return (unsigned int)( inpt & 0x1L ); }

inline unsigned int LHCb::HltSelRepRBSubstr::lenSubstr( unsigned short inpt ) {

  return (unsigned int)( ( inpt & 0xFFFF ) >> 1 );
}

inline unsigned int LHCb::HltSelRepRBSubstr::integrityCode() const {

  if ( !m_location ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kNoBank );
  // empty bank is OK: if( !numberOfObj() )return ((unsigned int)HltSelRepRBEnums::IntegrityCodes::kEmptyBank);
  if ( !allocatedSize() ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kZeroAllocatedSize );
  unsigned int s     = allocatedSize();
  unsigned int itera = InitialPositionOfIterator::kInitialPosition;
  for ( unsigned int iSub = 0; iSub != numberOfObj(); ++iSub ) {
    unsigned int iWord = itera / 2;
    unsigned int iPart = itera % 2;
    if ( iWord >= s ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kPtrOutOfRange );
    unsigned short nW;
    if ( iPart ) {
      nW = (unsigned short)( ( m_location[iWord] & 0xFFFF0000L ) >> 16 );
    } else {
      nW = (unsigned short)( m_location[iWord] & 0xFFFFL );
    }
    unsigned int nL = lenSubstr( nW );
    itera += nL + 1;
  }
  unsigned int iWord = itera / 2;
  unsigned int iPart = itera % 2;
  if ( !iPart ) --iWord;
  if ( iWord >= s ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kPtrOutOfRange );
  return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kBankIsOK );
}

inline LHCb::HltSelRepRBSubstr::Substr LHCb::HltSelRepRBSubstr::next() {

  unsigned int s     = allocatedSize();
  unsigned int iWord = m_iterator / 2;
  unsigned int iPart = m_iterator % 2;
  ++m_iterator;
  if ( iWord >= s )
    throw GaudiException( "Iterating beyond extend of the bank in HltSelRepRBSubstr", "HltSelRepRBSubstr",
                          StatusCode::FAILURE );
  unsigned short nW;
  if ( iPart ) {
    nW = (unsigned short)( ( m_location[iWord] & 0xFFFF0000L ) >> 16 );
  } else {
    nW = (unsigned short)( m_location[iWord] & 0xFFFFL );
  }
  unsigned int                nL = lenSubstr( nW );
  std::vector<unsigned short> vect;
  for ( unsigned int i = 0; i != nL; ++i ) {
    iWord = m_iterator / 2;
    iPart = m_iterator % 2;
    ++m_iterator;
    if ( iWord >= s )
      throw GaudiException( "Iterating beyond extend of the bank in HltSelRepRBSubstr", "HltSelRepRBSubstr",
                            StatusCode::FAILURE );
    unsigned short n;
    if ( iPart ) {
      n = (unsigned short)( ( m_location[iWord] & 0xFFFF0000L ) >> 16 );
    } else {
      n = (unsigned short)( m_location[iWord] & 0xFFFFL );
    }
    vect.push_back( n );
  }
  return Substr( hitSubstr( nW ), vect );
}

inline void LHCb::HltSelRepRBSubstr::rewind() { m_iterator = InitialPositionOfIterator::kInitialPosition; }

inline unsigned int LHCb::HltSelRepRBSubstr::size() const {

  unsigned int s     = allocatedSize();
  unsigned int itera = InitialPositionOfIterator::kInitialPosition;
  for ( unsigned int iSub = 0; iSub != numberOfObj(); ++iSub ) {
    unsigned int iWord = itera / 2;
    unsigned int iPart = itera % 2;
    if ( iWord >= s )
      throw GaudiException( "Iterating beyond extend of the bank in HltSelRepRBSubstr", "HltSelRepRBSubstr",
                            StatusCode::FAILURE );
    unsigned short nW;
    if ( iPart ) {
      nW = (unsigned short)( ( m_location[iWord] & 0xFFFF0000L ) >> 16 );
    } else {
      nW = (unsigned short)( m_location[iWord] & 0xFFFFL );
    }
    unsigned int nL = lenSubstr( nW );
    itera += nL + 1;
  }
  unsigned int iWord = itera / 2;
  unsigned int iPart = itera % 2;
  if ( iPart ) ++iWord; // if we are in upper part of the word then first unused is the next one
  return iWord;
}

inline bool LHCb::HltSelRepRBSubstr::push_back( const Substr& sub ) {

  unsigned int nL = sub.second.size();

  // allocate new bank if neccessary
  if ( !m_location ) {
    unsigned int len = DefaultAllocation::kDefaultAllocation;
    len              = ( nL < ( len - 2 ) ) ? len : ( len + DefaultAllocation::kDefaultAllocation );
    if ( len > 0xFFFFL ) return false;
    initialize( len );
  }

  // re-locate the bank body if necessary
  unsigned int aSize = allocatedSize();
  unsigned int nSize = ( m_iterator + 1 + nL ) / 2 + 2;
  if ( nSize > aSize ) {
    unsigned int len = aSize + DefaultAllocation::kDefaultAllocation;
    len              = ( nSize < len ) ? len : ( nSize + DefaultAllocation::kDefaultAllocation );
    if ( len > 0xFFFFL ) return false;
    unsigned int* newLocation = new unsigned int[len];
    for ( unsigned int i = 0; i != aSize; ++i ) { newLocation[i] = m_location[i]; }
    delete[] m_location;
    m_location = newLocation;
    m_location[0] &= 0xFFFFL;
    m_location[0] |= ( len << 16 );
  }

  // add the substructure sequence -------------------------------------
  // increment number of substrs
  unsigned int iSub = m_location[0] & 0xFFFFL;
  m_location[0]     = ( m_location[0] & ~0xFFFFL ) | ( iSub + 1 );
  // put the count in
  unsigned int iWord = m_iterator / 2;
  unsigned int iPart = m_iterator % 2;
  ++m_iterator;
  unsigned short nW = 0;
  nW                = sub.second.size() << 1;
  if ( sub.first ) nW |= 1;
  if ( iPart ) {
    m_location[iWord] &= 0xFFFFL;
    m_location[iWord] |= ( nW << 16 );
  } else {
    m_location[iWord] = ( 0L | nW );
  }
  // copy the content
  for ( unsigned int i = 0; i != nL; ++i ) {
    iWord = m_iterator / 2;
    iPart = m_iterator % 2;
    ++m_iterator;
    unsigned short n = sub.second[i];
    if ( iPart ) {
      m_location[iWord] &= 0xFFFFL;
      m_location[iWord] |= ( n << 16 );
    } else {
      m_location[iWord] = ( 0L | n );
    }
  }
  return true;
}

inline void LHCb::HltSelRepRBSubstr::deleteBank() {

  delete[] m_location;
  m_location = nullptr;
}

inline void LHCb::HltSelRepRBSubstr::clear() {

  // number of substr set to 0
  m_location[0] &= ~0xFFFFL;
  rewind();
}

inline void LHCb::HltSelRepRBSubstr::initialize( unsigned int len ) {

  if ( len < 1 ) len = DefaultAllocation::kDefaultAllocation;
  m_location    = new unsigned int[len];
  m_location[0] = ( len << 16 );
  rewind();
}

inline std::ostream& LHCb::HltSelRepRBSubstr::fillStream( std::ostream& s ) const {

  unsigned int ic = integrityCode();
  s << " HltSelRepRBSubstr : { "
    << " nSubstr " << numberOfObj() << " size Allocated " << allocatedSize() << " Actual " << size()
    << " integrity code " << ic << ( " " + HltSelRepRBEnums::IntegrityCodesToString( ic ) ) << std::endl;
  if ( 0 == ic ) {
    unsigned int itera = ( (unsigned int)InitialPositionOfIterator::kInitialPosition );
    for ( unsigned int iSub = 0; iSub != numberOfObj(); ++iSub ) {
      unsigned int iWord = itera / 2;
      unsigned int iPart = itera % 2;
      ++itera;
      unsigned short nW;
      if ( iPart ) {
        nW = (unsigned short)( ( m_location[iWord] & 0xFFFF0000L ) >> 16 );
      } else {
        nW = (unsigned short)( m_location[iWord] & 0xFFFFL );
      }
      unsigned int nL = lenSubstr( nW );
      s << " subStr : " << iSub << " size " << nL << " hitType " << hitSubstr( nW ) << " { ";
      for ( unsigned int i = 0; i != nL; ++i ) {
        unsigned int iWord = itera / 2;
        unsigned int iPart = itera % 2;
        ++itera;
        unsigned short n;
        if ( iPart ) {
          n = (unsigned short)( ( m_location[iWord] & 0xFFFF0000L ) >> 16 );
        } else {
          n = (unsigned short)( m_location[iWord] & 0xFFFFL );
        }
        s << " " << int( n );
      }
      s << " } " << std::endl;
    }
  }
  s << " }" << std::endl;
  return s;
}

inline void LHCb::HltSelRepRBSubstr::noBank() const {

  if ( !m_location )
    throw GaudiException( "Accessing non-allocated bank in HltSelRepRBSubstr", "HltSelRepRBSubstr",
                          StatusCode::FAILURE );
}
