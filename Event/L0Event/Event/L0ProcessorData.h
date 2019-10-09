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
#include "Event/L0DUBase.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include <ostream>
#include <string>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Namespace for locations in TDS
  namespace L0ProcessorDataLocation {
    inline const std::string Calo     = "Trig/L0/L0DUCaloData";
    inline const std::string Muon     = "Trig/L0/L0DUMuonData";
    inline const std::string PileUp   = "Trig/L0/L0DUPusData";
    inline const std::string HC       = "Trig/L0/L0DUHCData";
    inline const std::string L0Calo   = "Trig/L0/L0DUL0CaloData";
    inline const std::string L0Muon   = "Trig/L0/L0DUL0MuonData";
    inline const std::string L0PileUp = "Trig/L0/L0DUL0PusData";
    inline const std::string L0HC     = "Trig/L0/L0DUL0HCData";
    inline const std::string L0DU     = "Trig/L0/L0DUData";
  } // namespace L0ProcessorDataLocation

  /** @class L0ProcessorData L0ProcessorData.h
   *
   * This class represents the L0 processors data sent to L0DU
   *
   * @author Olivier Deschamps
   *
   */

  class L0ProcessorData final : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of L0ProcessorData
    typedef KeyedContainer<L0ProcessorData, Containers::HashMap> Container;

    /// Special constructor from 32 bit word
    L0ProcessorData( const L0DUBase::Fiber::Type& fiber, unsigned int word ) : Base( fiber ), m_wordmap{{0, word}} {}

    /// Special constructor from  16bit msb + 16bit lsb
    L0ProcessorData( const L0DUBase::Fiber::Type& fiber, unsigned int msb, unsigned int lsb )
        : Base( fiber ), m_wordmap{{0, lsb | ( msb << 16 )}} {}

    /// Default constructor
    L0ProcessorData() : Base(), m_wordmap{{0, 0x10000}} {}

    /// Copy Constructor
    L0ProcessorData( const L0ProcessorData& rh );

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// get the 32 bit word
    unsigned int word( int bx = 0 );

    /// set the 32 bit word
    void setWord( unsigned int word, int bx = 0 );

    /// get 16 bits lsb of L0 word
    unsigned int lsb( int bx = 0 );

    /// get  16 bits msb of L0 word
    unsigned int msb( int bx = 0 );

    /// update L0 word from MSB+LSB for a given bx
    void setWord( unsigned int msb, unsigned int lsb, int bx );

    /// check of data is defined for a given BX
    bool hasData( int bx = 0 );

    /// return a vector of available BXs
    std::vector<int> bxList();

    /// Retrieve const  32 bit L0 word produced by the processor (BX mapped)
    const std::map<int, int>& wordmap() const;

    /// Update  32 bit L0 word produced by the processor (BX mapped)
    void setWordmap( const std::map<int, int>& value );

    friend std::ostream& operator<<( std::ostream& str, const L0ProcessorData& obj ) { return obj.fillStream( str ); }

  protected:
    /// Shortcut for own base class
    typedef KeyedObject<int> Base;

  private:
    std::map<int, int> m_wordmap; ///< 32 bit L0 word produced by the processor (BX mapped)

  }; // class L0ProcessorData

  /// Definition of Keyed Container for L0ProcessorData
  typedef KeyedContainer<L0ProcessorData, Containers::HashMap> L0ProcessorDatas;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::L0ProcessorData::L0ProcessorData( const LHCb::L0ProcessorData& rh )
    : KeyedObject<int>(), m_wordmap( rh.m_wordmap ) {}

inline std::ostream& LHCb::L0ProcessorData::fillStream( std::ostream& s ) const {
  s << "{ "
    << "wordmap :	" << m_wordmap << std::endl
    << " }";
  return s;
}

inline const std::map<int, int>& LHCb::L0ProcessorData::wordmap() const { return m_wordmap; }

inline void LHCb::L0ProcessorData::setWordmap( const std::map<int, int>& value ) { m_wordmap = value; }

inline unsigned int LHCb::L0ProcessorData::word( int bx ) {

  if ( m_wordmap.find( bx ) == m_wordmap.end() ) m_wordmap[bx] = 0x10000;
  return (unsigned int)m_wordmap[bx];
}

inline void LHCb::L0ProcessorData::setWord( unsigned int word, int bx ) { m_wordmap[bx] = word; }

inline unsigned int LHCb::L0ProcessorData::lsb( int bx ) {

  if ( m_wordmap.find( bx ) == m_wordmap.end() ) m_wordmap[bx] = 0x10000;
  return (unsigned int)( m_wordmap[bx] & 0xFFFF );
}

inline unsigned int LHCb::L0ProcessorData::msb( int bx ) {

  if ( m_wordmap.find( bx ) == m_wordmap.end() ) m_wordmap[bx] = 0x10000;
  return (unsigned int)( ( m_wordmap[bx] >> 16 ) & 0xFFFF );
}

inline void LHCb::L0ProcessorData::setWord( unsigned int msb, unsigned int lsb, int bx ) {

  if ( m_wordmap.find( bx ) == m_wordmap.end() ) m_wordmap[bx] = 0x10000;
  m_wordmap[bx] = lsb | ( msb << 16 );
}

inline bool LHCb::L0ProcessorData::hasData( int bx ) { return ( m_wordmap.find( bx ) != m_wordmap.end() ); }

inline std::vector<int> LHCb::L0ProcessorData::bxList() {

  std::vector<int> bxs;
  for ( std::map<int, int>::iterator ibx = m_wordmap.begin(); m_wordmap.end() != ibx; ibx++ ) {
    bxs.push_back( ibx->first );
  }
  return bxs;
}
