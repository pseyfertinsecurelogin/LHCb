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
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class HltSelRepRBObjTyp HltSelRepRBObjTyp.h
   *
   * Manages the RawBank::HltSelReports object type sub-bank. All info is stored
   * in 16-bit shorts (packed in 32-bit words). The first short is a number of
   * object types stored. The second short is the allocated length of the bank
   * (in 32-bit words). This is followed by 1 long word per object type. The more
   * significant short is the class identified (CLID). Less significant short
   * gives cumulative count of the objects stored (of all types). For convenience
   * of having the same type of iterator as other object sub-banks the iterator
   * iterates over object number and not object type number. Also push_back
   * operator puts in single object CLID. Objects must be sorted by the CLID
   * before using this insertor.
   *
   * @author Tomasz Skwarnicki
   *
   */

  class HltSelRepRBObjTyp {
  public:
    /// default allocation size in 32-bit words for new bank or increment in its extension
    enum DefaultAllocation { kDefaultAllocation = 50 };
    /// iterator value to access the first element
    enum InitialPositionOfIterator { kInitialPosition = 1 };

    /// takes pointer to the memory location of the subbank body as input
    HltSelRepRBObjTyp( unsigned int* pBankBody )
        : m_location( pBankBody ), m_iterator( InitialPositionOfIterator::kInitialPosition ), m_objiterator( 0 ) {}

    /// Default Constructor
    HltSelRepRBObjTyp()
        : m_location( 0 ), m_iterator( InitialPositionOfIterator::kInitialPosition ), m_objiterator( 0 ) {}

    /// Default Destructor
    virtual ~HltSelRepRBObjTyp() {}

    /// Number of Object Types in the sub-banks
    unsigned int numberOfObjTyp() const;

    /// Number of Objects in the sub-banks
    unsigned int numberOfObj() const;

    /// length of the subbank in long words (allocated)
    unsigned int allocatedSize() const;

    /// replace allocated size by actual size before saving (no write access after that - deleteBank)
    void saveSize() const;

    /// reports bank integrity level 0=OK
    unsigned int integrityCode() const;

    /// gets CLID from current object iterator position, advances object iterator to next position
    unsigned int next();

    /// reset iterator to the initial position
    void rewind();

    /// actual size of the bank in long words
    unsigned int size() const;

    /// Add object and its id to the bank. Must add objects of the same id sequencially.
    void push_back( unsigned int clid );

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

    /// Retrieve const  internal iterator - gives current position in number of long word
    unsigned int iterator() const;

    /// Retrieve const  internal object iterator - gives current object count (used in association with iterator)
    unsigned int objiterator() const;

    friend std::ostream& operator<<( std::ostream& str, const HltSelRepRBObjTyp& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// throw exception on access to non-allocated bank
    void noBank() const;

    unsigned int* m_location;    ///< Location of the subbank body in the program memory
    unsigned int  m_iterator;    ///< internal iterator - gives current position in number of long word
    unsigned int  m_objiterator; ///< internal object iterator - gives current object count (used in association with
                                 ///< iterator)

  }; // class HltSelRepRBObjTyp

  inline std::ostream& operator<<( std::ostream& s, LHCb::HltSelRepRBObjTyp::DefaultAllocation e ) {
    switch ( e ) {
    case LHCb::HltSelRepRBObjTyp::kDefaultAllocation:
      return s << "kDefaultAllocation";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::HltSelRepRBObjTyp::DefaultAllocation";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::HltSelRepRBObjTyp::InitialPositionOfIterator e ) {
    switch ( e ) {
    case LHCb::HltSelRepRBObjTyp::kInitialPosition:
      return s << "kInitialPosition";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::HltSelRepRBObjTyp::InitialPositionOfIterator";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const unsigned int* LHCb::HltSelRepRBObjTyp::location() const { return m_location; }

inline void LHCb::HltSelRepRBObjTyp::setLocation( unsigned int* value ) { m_location = value; }

inline unsigned int LHCb::HltSelRepRBObjTyp::iterator() const { return m_iterator; }

inline unsigned int LHCb::HltSelRepRBObjTyp::objiterator() const { return m_objiterator; }

inline unsigned int LHCb::HltSelRepRBObjTyp::numberOfObjTyp() const {

  noBank();
  return (unsigned int)( m_location[0] & 0xFFFFL );
}

inline unsigned int LHCb::HltSelRepRBObjTyp::numberOfObj() const {

  noBank();
  if ( numberOfObjTyp() ) return (unsigned int)( m_location[numberOfObjTyp()] & 0xFFFFL );
  return 0;
}

inline unsigned int LHCb::HltSelRepRBObjTyp::allocatedSize() const {

  noBank();
  return (unsigned int)( ( m_location[0] & 0xFFFF0000L ) >> 16 );
}

inline void LHCb::HltSelRepRBObjTyp::saveSize() const {

  noBank();
  unsigned int s = size(); // size() looks at allocated size so calculate it before we mess up with allocated
  m_location[0] &= 0xFFFFL;
  m_location[0] |= ( s << 16 );
}

inline unsigned int LHCb::HltSelRepRBObjTyp::integrityCode() const {

  if ( !m_location ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kNoBank );
  // empty bank is OK: if( !numberOfObjTyp() )return ((unsigned int)HltSelRepRBEnums::IntegrityCodes::kEmptyBank);
  if ( !allocatedSize() ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kZeroAllocatedSize );
  if ( numberOfObjTyp() > ( allocatedSize() - 1 ) )
    return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kPtrOutOfRange );
  return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kBankIsOK );
}

inline unsigned int LHCb::HltSelRepRBObjTyp::next() {

  if ( m_objiterator >= numberOfObj() )
    throw GaudiException( "Iterating beyond number of objects in the bank in HltSelRepRBObjTyp", "HltSelRepRBObjTyp",
                          StatusCode::FAILURE );
  unsigned int s     = size();
  unsigned int iWord = m_iterator;
  if ( iWord >= s )
    throw GaudiException( "Iterating beyond extend of the bank in HltSelRepRBObjTyp", "HltSelRepRBObjTyp",
                          StatusCode::FAILURE );
  unsigned int nObj = ( m_location[iWord] & 0xFFFFL );
  unsigned int clid = ( ( m_location[iWord] & 0xFFFF0000L ) >> 16 );
  ++m_objiterator;
  if ( m_objiterator >= nObj ) ++m_iterator;
  return clid;
}

inline void LHCb::HltSelRepRBObjTyp::rewind() {

  m_iterator    = InitialPositionOfIterator::kInitialPosition;
  m_objiterator = 0;
}

inline unsigned int LHCb::HltSelRepRBObjTyp::size() const { return ( numberOfObjTyp() + 1 ); }

inline void LHCb::HltSelRepRBObjTyp::push_back( unsigned int clid ) {

  // allocate new bank if neccessary
  if ( !m_location ) {
    unsigned int len = DefaultAllocation::kDefaultAllocation;
    initialize( len );
  }

  // re-locate the bank body if necessary
  unsigned int aSize = allocatedSize();
  unsigned int nSize = m_iterator + 1; // presume we may need to add new id
  if ( nSize > aSize ) {
    unsigned int  len         = aSize + DefaultAllocation::kDefaultAllocation;
    unsigned int* newLocation = new unsigned int[len];
    for ( unsigned int i = 0; i != aSize; ++i ) { newLocation[i] = m_location[i]; }
    delete[] m_location;
    m_location = newLocation;
    m_location[0] &= 0xFFFFL;
    m_location[0] |= ( len << 16 );
  }

  // add the object type  -------------------------------------
  unsigned int iWord   = m_iterator;
  unsigned int nObj    = ( m_location[iWord] & 0xFFFFL );
  unsigned int clidold = ( ( m_location[iWord] & 0xFFFF0000L ) >> 16 );
  if ( nObj != m_objiterator )
    throw GaudiException( "Illegal push_back in HltSelRepRBObjTyp", "HltSelRepRBObjTyp", StatusCode::FAILURE );
  ++m_objiterator;
  if ( clidold != clid ) {
    unsigned int n = ( m_location[0] & 0xFFFFL );
    if ( n ) {
      ++m_iterator;
      iWord = m_iterator;
    }
    m_location[iWord] = ( clid << 16 );
    ++n;
    m_location[0] = ( ( m_location[0] & 0xFFFF0000L ) | n );
  }
  m_location[iWord] = ( ( m_location[iWord] & 0xFFFF0000L ) | m_objiterator );
}

inline void LHCb::HltSelRepRBObjTyp::deleteBank() {

  delete[] m_location;
  m_location = nullptr;
}

inline void LHCb::HltSelRepRBObjTyp::clear() {

  // number of substr set to 0
  m_location[0] &= ~0xFFFFL;
  m_location[1] = 0;
  rewind();
}

inline void LHCb::HltSelRepRBObjTyp::initialize( unsigned int len ) {

  if ( len < 1 ) len = DefaultAllocation::kDefaultAllocation;
  m_location    = new unsigned int[len];
  m_location[0] = ( len << 16 );
  m_location[1] = 0;
  rewind();
}

inline std::ostream& LHCb::HltSelRepRBObjTyp::fillStream( std::ostream& s ) const {

  unsigned int ic = integrityCode();
  s << " HltSelRepRBObjTyp : { "
    << " nObjTyp " << numberOfObjTyp() << " nObj " << numberOfObj() << " size Allocated " << allocatedSize()
    << " Actual " << size() << " integrity code " << ic << ( " " + HltSelRepRBEnums::IntegrityCodesToString( ic ) )
    << std::endl;
  if ( 0 == ic ) {
    unsigned int iold = 0;
    for ( unsigned int iObjTyp = 0; iObjTyp != numberOfObjTyp(); ++iObjTyp ) {
      unsigned int iWord = InitialPositionOfIterator::kInitialPosition + iObjTyp;
      unsigned int nObj  = ( m_location[iWord] & 0xFFFFL );
      unsigned int clid  = ( ( m_location[iWord] & 0xFFFF0000L ) >> 16 );
      s << "  " << iObjTyp << " type " << clid << " #-of-objs " << nObj - iold << " cumulative " << nObj;
      iold = nObj;
      s << std::endl;
    }
  }
  s << " }" << std::endl;
  return s;
}

inline void LHCb::HltSelRepRBObjTyp::noBank() const {

  if ( !m_location )
    throw GaudiException( "Accessing non-allocated bank in HltSelRepRBObjTyp", "HltSelRepRBObjTyp",
                          StatusCode::FAILURE );
}
