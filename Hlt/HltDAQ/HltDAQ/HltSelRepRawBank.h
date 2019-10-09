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
#include "HltDAQ/HltSelRepRBEnums.h"
#include <algorithm>
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class HltSelRepRawBank HltSelRepRawBank.h
   *
   * Manages the RawBank::HltSelReports bank body and its sub-bank structure. The
   * class makes no assumptions about sub-bank substructure. However, it allows
   * only one irreplacable instance of each sub-bank type given by its id. Order
   * of sub-banks is arbitrary, but the sub-banks may be added only to the back
   * of the bank. Random access to a sub-bank is supported by either relative
   * index within the bank or by its id. The header of the bank body is fixed
   * length and consists of 10 words. The first word is the present allocated
   * bank length (in number of 32-bit words). First 3 bits of the 2nd word are
   * number of sub-banks (max 8). Consequent 3-bit chunks are sub-bank IDs (0-7).
   * Next 8 words are the first relative location in the bank body beyond given
   * sub-bank. Sub-banks immediately follow the header.
   *
   * @author Tomasz Skwarnicki
   *
   */

  class HltSelRepRawBank {
  public:
    /// positions of words in the header
    enum Header { kAllocatedSize = 0, kSubBankIDs = 1, kSubBankLocations = 2, kHeaderSize = 10 };
    /// default allocation size in 32-bit words for new bank or increment in its extension
    enum DefaultAllocation { kDefaultAllocation = 3000 };

    /// takes pointer to the memory location of the bank body as input
    HltSelRepRawBank( unsigned int* pBankBody ) : m_location( pBankBody ) {}

    /// Default Constructor
    HltSelRepRawBank() : m_location( 0 ) {}

    /// Default Destructor
    virtual ~HltSelRepRawBank() {}

    /// throw exception if no Bank or index invalid
    void validateIndex( unsigned int iBank ) const;

    /// Number of sub-banks
    unsigned int numberOfSubBanks() const;

    /// index of the sub-bank given its id (returns HltSelRepRBEnums::kUnknownID if not found)
    unsigned int indexSubBank( unsigned int idSubBank ) const;

    /// ID of the sub-bank iBank (iBank counts from 0)
    unsigned int subBankID( unsigned int iBank ) const;

    /// relative location of the sub-bank in the bank body in number of words given its id (0=means first word)
    unsigned int subBankBeginFromID( unsigned int idSubBank ) const;

    /// relative location of the sub-bank in the bank body in number of words given its id (0=means first word)
    unsigned int subBankEndFromID( unsigned int idSubBank ) const;

    /// length of the sub-bank in number of words given its index
    unsigned int subBankSize( unsigned int iBank ) const;

    /// length of the sub-bank in number of words given its id
    unsigned int subBankSizeFromID( unsigned int idSubBank ) const;

    /// pointer to the subbank given its id (0 returned if not found)
    unsigned int* subBankFromID( unsigned int idSubBank ) const;

    /// allocated bank body size
    unsigned int allocatedSize() const;

    /// replace allocated size by actual size before saving (no write access after that - deleteBank)
    void saveSize() const;

    /// size of the bank body (can be less than allocated)
    unsigned int size() const;

    /// validate internal pointer
    void validatePtr( unsigned int iPtr ) const;

    /// reports bank integrity level 0=OK
    unsigned int integrityCode() const;

    /// Add sub-bank copying its content. Cannot be used to replace sub-bank which is already in.
    void push_back( unsigned int idSubBank, const unsigned int* pSubBank, unsigned int sizeSubBank );

    /// delete the bank from program memory
    void deleteBank();

    /// clear bank content without deleting
    void clear();

    /// create a new bank with given allocation len and initialize it (does not delete old bank if any)
    void initialize( unsigned int len = DefaultAllocation::kDefaultAllocation );

    /// intelligent printout
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  Location of the bank body in the program memory
    const unsigned int* location() const;

    /// Update  Location of the bank body in the program memory
    void setLocation( unsigned int* value );

    friend std::ostream& operator<<( std::ostream& str, const HltSelRepRawBank& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// throw exception on access to non-allocated bank
    void noBank() const;

    /// relative location of the sub-bank in the bank body in number of words given its index (0=means first word)
    unsigned int subBankBegin( unsigned int iBank ) const;

    /// relative location of the end() of sub-bank in the bank body in number of words given its index (0=means first
    /// word)
    unsigned int subBankEnd( unsigned int iBank ) const;

    unsigned int* m_location; ///< Location of the bank body in the program memory

  }; // class HltSelRepRawBank

  inline std::ostream& operator<<( std::ostream& s, LHCb::HltSelRepRawBank::Header e ) {
    switch ( e ) {
    case LHCb::HltSelRepRawBank::kAllocatedSize:
      return s << "kAllocatedSize";
    case LHCb::HltSelRepRawBank::kSubBankIDs:
      return s << "kSubBankIDs";
    case LHCb::HltSelRepRawBank::kSubBankLocations:
      return s << "kSubBankLocations";
    case LHCb::HltSelRepRawBank::kHeaderSize:
      return s << "kHeaderSize";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::HltSelRepRawBank::Header";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::HltSelRepRawBank::DefaultAllocation e ) {
    switch ( e ) {
    case LHCb::HltSelRepRawBank::kDefaultAllocation:
      return s << "kDefaultAllocation";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::HltSelRepRawBank::DefaultAllocation";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const unsigned int* LHCb::HltSelRepRawBank::location() const { return m_location; }

inline void LHCb::HltSelRepRawBank::setLocation( unsigned int* value ) { m_location = value; }

inline void LHCb::HltSelRepRawBank::validateIndex( unsigned int iBank ) const {

  noBank();
  if ( iBank > HltSelRepRBEnums::SubBankIDs::kMaxBankID )
    throw GaudiException( "sub-bank index out of range in HltSelRepRawBank", "HltSelRepRawBank", StatusCode::FAILURE );
}

inline unsigned int LHCb::HltSelRepRawBank::numberOfSubBanks() const {

  noBank();
  return (unsigned int)( m_location[Header::kSubBankIDs] & 0x7L );
}

inline unsigned int LHCb::HltSelRepRawBank::indexSubBank( unsigned int idSubBank ) const {

  if ( !m_location ) return (unsigned int)( HltSelRepRBEnums::SubBankIDs::kUnknownID );
  for ( unsigned int iBank = 0; iBank != numberOfSubBanks(); ++iBank ) {
    if ( subBankID( iBank ) == idSubBank ) return iBank;
  }
  return (unsigned int)( HltSelRepRBEnums::SubBankIDs::kUnknownID );
}

inline unsigned int LHCb::HltSelRepRawBank::subBankID( unsigned int iBank ) const {

  validateIndex( iBank );
  unsigned int bits = ( iBank + 1 ) * 3;
  unsigned int mask = 0x7L << bits;
  return (unsigned int)( ( m_location[Header::kSubBankIDs] & mask ) >> bits );
}

inline unsigned int LHCb::HltSelRepRawBank::subBankBeginFromID( unsigned int idSubBank ) const {

  return subBankBegin( indexSubBank( idSubBank ) );
}

inline unsigned int LHCb::HltSelRepRawBank::subBankEndFromID( unsigned int idSubBank ) const {

  return subBankEnd( indexSubBank( idSubBank ) );
}

inline unsigned int LHCb::HltSelRepRawBank::subBankSize( unsigned int iBank ) const {

  validateIndex( iBank );
  return ( subBankEnd( iBank ) - subBankBegin( iBank ) );
}

inline unsigned int LHCb::HltSelRepRawBank::subBankSizeFromID( unsigned int idSubBank ) const {

  return subBankSize( indexSubBank( idSubBank ) );
}

inline unsigned int* LHCb::HltSelRepRawBank::subBankFromID( unsigned int idSubBank ) const {

  unsigned int loc = subBankBeginFromID( idSubBank );
  if ( !loc ) return 0;
  validatePtr( loc );
  return &( m_location[loc] );
}

inline unsigned int LHCb::HltSelRepRawBank::allocatedSize() const {

  noBank();
  return m_location[Header::kAllocatedSize];
}

inline void LHCb::HltSelRepRawBank::saveSize() const {

  noBank();
  m_location[Header::kAllocatedSize] = size();
}

inline unsigned int LHCb::HltSelRepRawBank::size() const {

  noBank();
  if ( numberOfSubBanks() ) return subBankEnd( numberOfSubBanks() - 1 );
  return Header::kHeaderSize;
}

inline void LHCb::HltSelRepRawBank::validatePtr( unsigned int iPtr ) const {

  if ( iPtr < ( (unsigned int)Header::kHeaderSize ) )
    throw GaudiException( "Internal pointer out of range in HltSelRepRawBank", "HltSelRepRawBank",
                          StatusCode::FAILURE );
  if ( iPtr > allocatedSize() )
    throw GaudiException( "Internal pointer out of range in HltSelRepRawBank", "HltSelRepRawBank",
                          StatusCode::FAILURE );
}

inline unsigned int LHCb::HltSelRepRawBank::integrityCode() const {

  if ( !m_location ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kNoBank );
  if ( !numberOfSubBanks() ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kEmptyBank );
  if ( !allocatedSize() ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kZeroAllocatedSize );
  for ( unsigned int iBank = 0; iBank != numberOfSubBanks(); ++iBank ) {
    unsigned int ie1 = subBankEnd( iBank );
    if ( ie1 < ( (unsigned int)Header::kHeaderSize ) )
      return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kPtrOutOfRange );
    if ( ie1 > ( allocatedSize() + 1 ) ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kPtrOutOfRange );
    unsigned int id1 = subBankID( iBank );
    for ( unsigned int jBank = iBank + 1; jBank != numberOfSubBanks(); ++jBank ) {
      if ( ie1 > subBankEnd( jBank ) ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kPtrOutOfSequence );
      if ( id1 == subBankID( jBank ) ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kDuplicateBankIDs );
    }
  }
  return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kBankIsOK );
}

inline void LHCb::HltSelRepRawBank::push_back( unsigned int idSubBank, const unsigned int* pSubBank,
                                               unsigned int sizeSubBank ) {

  if ( idSubBank > HltSelRepRBEnums::SubBankIDs::kMaxBankID )
    throw GaudiException( "Adding illegal sub-bank ID in HltSelRepRawBank", "HltSelRepRawBank", StatusCode::FAILURE );

  if ( indexSubBank( idSubBank ) != HltSelRepRBEnums::SubBankIDs::kUnknownID )
    throw GaudiException( "Adding sub-bank which is already there in HltSelRepRawBank", "HltSelRepRawBank",
                          StatusCode::FAILURE );

  // allocate new bank if neccessary
  if ( !m_location ) {
    unsigned int len = DefaultAllocation::kDefaultAllocation;
    len              = ( ( sizeSubBank + Header::kHeaderSize ) < len ) ? len : ( sizeSubBank + len );
    initialize( len );
  }

  // re-locate the bank body if necessary
  unsigned int aSize = allocatedSize();
  unsigned int nSize = size() + sizeSubBank;
  if ( nSize > aSize ) {
    unsigned int len          = aSize + DefaultAllocation::kDefaultAllocation;
    len                       = ( nSize < len ) ? len : ( nSize + DefaultAllocation::kDefaultAllocation );
    unsigned int* newLocation = new unsigned int[len];
    std::copy_n( m_location, size(), newLocation );
    delete[] m_location;
    m_location                         = newLocation;
    m_location[Header::kAllocatedSize] = len;
  }

  // add the bank -------------------------------------
  // increment number of banks (don't need to worry about exceeding the size since
  //  sub-bank id range takes care of it)
  unsigned int iBank              = m_location[Header::kSubBankIDs] & 0x7L;
  m_location[Header::kSubBankIDs] = ( m_location[Header::kSubBankIDs] & ~0x7L ) | ( iBank + 1 );
  // set its id
  unsigned int bits               = ( iBank + 1 ) * 3;
  unsigned int mask               = 0x7L << bits;
  m_location[Header::kSubBankIDs] = ( m_location[Header::kSubBankIDs] & ~mask ) | ( idSubBank << bits );
  // get its location
  unsigned int locBank = subBankBegin( iBank );
  // set its end()
  m_location[Header::kSubBankLocations + iBank] = locBank + sizeSubBank;
  // copy content
  std::copy_n( pSubBank, sizeSubBank, m_location + locBank );
}

inline void LHCb::HltSelRepRawBank::deleteBank() {

  delete[] m_location;
  m_location = nullptr;
}

inline void LHCb::HltSelRepRawBank::clear() {

  // Initialize the potential padding bytes for reproducibility
  std::fill_n( m_location, Header::kHeaderSize, 0 );
  m_location[Header::kSubBankIDs]                            = 0;
  m_location[Header::kSubBankLocations + numberOfSubBanks()] = Header::kHeaderSize;
}

inline void LHCb::HltSelRepRawBank::initialize( unsigned int len ) {

  if ( len < Header::kHeaderSize ) len = DefaultAllocation::kDefaultAllocation;
  m_location                         = new unsigned int[len];
  m_location[Header::kAllocatedSize] = len;
  clear();
}

inline std::ostream& LHCb::HltSelRepRawBank::fillStream( std::ostream& s ) const {

  unsigned int ic = integrityCode();
  s << " HltSelRepRawBank : { "
    << " nSubBanks " << numberOfSubBanks() << " size Allocated " << allocatedSize() << " Actual " << size()
    << " integrity code " << ic << ( " " + HltSelRepRBEnums::IntegrityCodesToString( ic ) ) << std::endl;
  if ( 0 == ic )
    for ( unsigned int iBank = 0; iBank != numberOfSubBanks(); ++iBank ) {
      s << " subBank : " << iBank << " size " << subBankSize( iBank ) << " id " << subBankID( iBank )
        << ( " " + HltSelRepRBEnums::SubBankIDsToString( subBankID( iBank ) ) ) << " begin " << subBankBegin( iBank )
        << " end " << subBankEnd( iBank ) << std::endl;
    }
  s << " }" << std::endl;
  return s;
}

inline void LHCb::HltSelRepRawBank::noBank() const {

  if ( !m_location )
    throw GaudiException( "Accessing non-allocated bank in HltSelRepRawBank", "HltSelRepRawBank", StatusCode::FAILURE );
}

inline unsigned int LHCb::HltSelRepRawBank::subBankBegin( unsigned int iBank ) const {

  validateIndex( iBank );
  if ( iBank ) {
    return m_location[Header::kSubBankLocations + iBank - 1];
  } else {
    return (unsigned int)Header::kHeaderSize;
  }
}

inline unsigned int LHCb::HltSelRepRawBank::subBankEnd( unsigned int iBank ) const {

  validateIndex( iBank );
  return m_location[Header::kSubBankLocations + iBank];
}
