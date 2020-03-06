/*****************************************************************************\
* (c) Copyright 2000-2020 CERN for the benefit of the LHCb Collaboration      *
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
#include "Kernel/LHCbID.h"
#include <algorithm>
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class HltSelRepRBHits HltSelRepRBHits.h
   *
   * Manages the RawBank::HltSelReports hits sub-bank. The first short is a
   * number of hit sequences. Folllowing shorts give relative location (in words)
   * in the subbank body of the end() of each sequence. After a possible padding
   * short, LHCbIDs follow. Random read access is supported. Bank must be
   * initialized with number of sequences and hits. Then sequences must be pushed
   * back in the sequencial order.
   *
   * @author Tomasz Skwarnicki
   *
   */

  class HltSelRepRBHits {
  public:
    /// type, without const and reference, used as argument to push_back; needed to make push_back work with
    /// std::back_inserter
    typedef std::vector<unsigned int> value_type;

    /// takes pointer to the memory location of the subbank body as input
    HltSelRepRBHits( unsigned int* pBankBody ) : m_location( pBankBody ), m_length( 0 ) {}

    /// takes pointer to the memory location of the subbank body and its length as input
    HltSelRepRBHits( unsigned int* pBankBody, unsigned int len ) : m_location( pBankBody ), m_length( len ) {}

    /// Default Constructor
    HltSelRepRBHits() : m_location( 0 ), m_length( 0 ) {}

    /// Default Destructor
    virtual ~HltSelRepRBHits() {}

    /// throw exception if no Bank or index invalid
    void validateIndex( unsigned int iSeq ) const;

    /// Number of hit sequences in the sub-banks
    unsigned int numberOfSeq() const;

    /// relative location of the first hit sequence in the subbank (0=means 1st word)
    unsigned int hitsLocation() const;

    /// location of the hit suquence end() in the sub-bank body (0=means 1st word)
    unsigned int seqEnd( unsigned int iSeq ) const;

    /// location of the hit suquence begin() in the sub-bank body (0=means 1st word)
    unsigned int seqBegin( unsigned int iSeq ) const;

    /// hit sequence size given its index
    unsigned int seqSize( unsigned int iSeq ) const;

    /// size of the subbank (can only be used after all sequences are in
    unsigned int size() const;

    /// hit sequence begin() given its index
    unsigned int* sequenceBegin( unsigned int iSeq ) const;

    /// hit sequence begin() given its index
    unsigned int* sequenceEnd( unsigned int iSeq ) const;

    /// hit sequence given its index
    std::vector<LHCb::LHCbID> sequence( unsigned int iSeq ) const;

    /// validate internal pointer
    void validatePtr( unsigned int iPtr ) const;

    /// reports bank integrity level 0=OK
    unsigned int integrityCode() const;

    /// create and initilize new subbank to hold nSeq-uences and nHits (if we pointed to old bank it is not deleted)
    void initialize( unsigned int nSeq, unsigned int nHits );

    /// add hit sequence to the bank (must be initialized first)
    void push_back( const value_type& hitvec );

    /// delete the bank from program memory
    void deleteBank();

    /// intelligent printout
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  Location of the subbank body in the program memory
    const unsigned int* location() const;

    /// Update  Location of the subbank body in the program memory
    void setLocation( unsigned int* value );

    /// Retrieve const  length of the subbank (0=unknown)
    unsigned int length() const;

    /// Update  length of the subbank (0=unknown)
    void setLength( unsigned int value );

    friend std::ostream& operator<<( std::ostream& str, const HltSelRepRBHits& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// throw exception on access to non-allocated bank
    void noBank() const;

    unsigned int* m_location; ///< Location of the subbank body in the program memory
    unsigned int  m_length;   ///< length of the subbank (0=unknown)

  }; // class HltSelRepRBHits

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const unsigned int* LHCb::HltSelRepRBHits::location() const { return m_location; }

inline void LHCb::HltSelRepRBHits::setLocation( unsigned int* value ) { m_location = value; }

inline unsigned int LHCb::HltSelRepRBHits::length() const { return m_length; }

inline void LHCb::HltSelRepRBHits::setLength( unsigned int value ) { m_length = value; }

inline void LHCb::HltSelRepRBHits::validateIndex( unsigned int iSeq ) const {

  noBank();
  if ( iSeq >= numberOfSeq() )
    throw GaudiException( "suquence index out of range in HltSelRepRBHits", "HltSelRepRBHits", StatusCode::FAILURE );
}

inline unsigned int LHCb::HltSelRepRBHits::numberOfSeq() const {

  noBank();
  return (unsigned int)( m_location[0] & 0xFFFFL );
}

inline unsigned int LHCb::HltSelRepRBHits::hitsLocation() const {

  noBank();
  return ( numberOfSeq() / 2 + 1 );
}

inline unsigned int LHCb::HltSelRepRBHits::seqEnd( unsigned int iSeq ) const {

  if ( ( numberOfSeq() == 0 ) && ( iSeq == 0 ) ) return hitsLocation();
  validateIndex( iSeq );
  unsigned int iWord = ( iSeq + 1 ) / 2;
  unsigned int iPart = ( iSeq + 1 ) % 2;
  unsigned int bits  = iPart * 16;
  unsigned int mask  = 0xFFFFL << bits;
  return (unsigned int)( ( m_location[iWord] & mask ) >> bits );
}

inline unsigned int LHCb::HltSelRepRBHits::seqBegin( unsigned int iSeq ) const {

  if ( ( numberOfSeq() == 0 ) && ( iSeq == 0 ) ) return hitsLocation();
  validateIndex( iSeq );
  if ( iSeq ) return seqEnd( iSeq - 1 );
  return hitsLocation();
}

inline unsigned int LHCb::HltSelRepRBHits::seqSize( unsigned int iSeq ) const {

  return ( seqEnd( iSeq ) - seqBegin( iSeq ) );
}

inline unsigned int LHCb::HltSelRepRBHits::size() const {

  if ( numberOfSeq() ) return seqEnd( numberOfSeq() - 1 );
  return seqEnd( 0 );
}

inline unsigned int* LHCb::HltSelRepRBHits::sequenceBegin( unsigned int iSeq ) const {

  validateIndex( iSeq );
  unsigned int iBegin = seqBegin( iSeq );
  validatePtr( iBegin );
  return &( m_location[iBegin] );
}

inline unsigned int* LHCb::HltSelRepRBHits::sequenceEnd( unsigned int iSeq ) const {

  validateIndex( iSeq );
  unsigned int iEnd = seqEnd( iSeq );
  validatePtr( iEnd - 1 );
  return &( m_location[iEnd] );
}

inline std::vector<LHCb::LHCbID> LHCb::HltSelRepRBHits::sequence( unsigned int iSeq ) const {

  validateIndex( iSeq );
  std::vector<LHCbID> hitseq;
  unsigned int*       iend = sequenceEnd( iSeq );
  unsigned int*       i    = sequenceBegin( iSeq );
  hitseq.reserve( iend - i );
  for ( ; i != iend; ++i ) hitseq.emplace_back( *i );
  return hitseq;
}

inline void LHCb::HltSelRepRBHits::validatePtr( unsigned int iPtr ) const {

  if ( iPtr < hitsLocation() )
    throw GaudiException( "Internal pointer out of range in HltSelRepRBHits", "HltSelRepRBHits", StatusCode::FAILURE );
  if ( m_length && ( iPtr > m_length ) )
    throw GaudiException( "Internal pointer out of range in HltSelRepRBHits", "HltSelRepRBHits", StatusCode::FAILURE );
}

inline unsigned int LHCb::HltSelRepRBHits::integrityCode() const {

  if ( !m_location ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kNoBank );
  // empty bank if OK: if( !numberOfSeq() )return ((unsigned int)HltSelRepRBEnums::IntegrityCodes::kEmptyBank);
  for ( unsigned int iSeq = 0; iSeq != numberOfSeq(); ++iSeq ) {
    unsigned int ie1 = seqEnd( iSeq );
    if ( ie1 < hitsLocation() ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kPtrOutOfRange );
    if ( m_length && ( ie1 > ( m_length + 1 ) ) )
      return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kPtrOutOfRange );
    for ( unsigned int jSeq = iSeq + 1; jSeq != numberOfSeq(); ++jSeq ) {
      if ( ie1 > seqEnd( jSeq ) ) return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kPtrOutOfSequence );
    }
  }
  return ( (unsigned int)HltSelRepRBEnums::IntegrityCodes::kBankIsOK );
}

inline void LHCb::HltSelRepRBHits::initialize( unsigned int nSeq, unsigned int nHits ) {

  unsigned int len = nSeq / 2 + 1 + nHits;
  if ( len > 0xFFFFL )
    throw GaudiException( "requested initialize with bank too large HltSelRepRBHits", "HltSelRepRBHits",
                          StatusCode::FAILURE );
  m_location = new unsigned int[len];
  // Initialize the potential padding bytes for reproducibility
  std::fill_n( m_location, nSeq / 2 + 1, 0 );
  m_location[0] = 0L;
  // set number of sequences
  m_location[0] += nSeq;
  // set end of the first sequence
  m_location[0] |= ( seqBegin( 0 ) << 16 );
  m_length = len;
}

inline void LHCb::HltSelRepRBHits::push_back( const value_type& hitvec ) {

  for ( unsigned int iSeq = 0; iSeq != numberOfSeq(); ++iSeq ) {
    if ( 0 == seqSize( iSeq ) ) {
      // set the sequence end
      unsigned int iWord = ( iSeq + 1 ) / 2;
      unsigned int iPart = ( iSeq + 1 ) % 2;
      unsigned int bits  = iPart * 16;
      unsigned int mask  = 0xFFFFL << bits;
      unsigned int begin = seqBegin( iSeq );
      unsigned int end   = begin + hitvec.size();
      if ( m_length && ( end > ( m_length + 1 ) ) )
        throw GaudiException( "Pushing too many hits to HltSelRepRBHits", "HltSelRepRBHits", StatusCode::FAILURE );
      m_location[iWord] = ( m_location[iWord] & ~mask ) | ( end << bits );
      ++iSeq;
      if ( iSeq < numberOfSeq() ) {
        iWord             = ( iSeq + 1 ) / 2;
        iPart             = ( iSeq + 1 ) % 2;
        bits              = iPart * 16;
        mask              = 0xFFFFL << bits;
        m_location[iWord] = ( m_location[iWord] & ~mask ) | ( end << bits );
      }
      // copy hits
      std::copy( std::begin( hitvec ), std::end( hitvec ), m_location + begin );
      return;
    }
  }
  throw GaudiException( "Pushing too many hit sequences to HltSelRepRBHits", "HltSelRepRBHits", StatusCode::FAILURE );
}

inline void LHCb::HltSelRepRBHits::deleteBank() {

  delete[] m_location;
  m_location = nullptr;
}

inline std::ostream& LHCb::HltSelRepRBHits::fillStream( std::ostream& s ) const {

  unsigned int ic = integrityCode();
  s << " HltSelRepRBHits : { "
    << " nSeq " << numberOfSeq() << " size " << size() << " integrity code " << ic
    << ( " " + HltSelRepRBEnums::IntegrityCodesToString( ic ) ) << std::endl;
  if ( 0 == ic )
    for ( unsigned int iSeq = 0; iSeq != numberOfSeq(); ++iSeq ) {
      s << " seq : " << iSeq << " size " << seqSize( iSeq );
      for ( unsigned int iHit = seqBegin( iSeq ); iHit != seqEnd( iSeq ); ++iHit ) { s << " " << m_location[iHit]; }
      s << std::endl;
    }
  s << " }" << std::endl;
  return s;
}

inline void LHCb::HltSelRepRBHits::noBank() const {

  if ( !m_location )
    throw GaudiException( "Accessing non-allocated bank in HltSelRepRBHits", "HltSelRepRBHits", StatusCode::FAILURE );
}
