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
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class STChannelID STChannelID.h
   *
   * Channel ID for class for ST
   *
   * @author M Needham, J. Wang
   *
   */

  class STChannelID final {
  public:
    /// types of sub-detector channel ID
    enum detType { typeTT = 0, typeIT = 1 };

    /// constructor with station, layer, detRegion, sector , strip,
    STChannelID( const unsigned int iType, const unsigned int iStation, const unsigned int iLayer,
                 const unsigned int iDetRegion, const unsigned int iSector, const unsigned int iStrip );

    /// constructor with int
    explicit STChannelID( int id ) : m_channelID( id ) {}

    /// Default Constructor
    STChannelID() : m_channelID( 0 ) {}

    /// cast
    operator int() const;

    /// Retrieve type
    unsigned int type() const;

    /// test whether TT or not
    bool isTT() const;

    /// test whether IT or not
    bool isIT() const;

    /// Retrieve sector
    unsigned int sector() const;

    /// Retrieve detRegion
    unsigned int detRegion() const;

    /// Retrieve layer
    unsigned int layer() const;

    /// Retrieve unique layer
    unsigned int uniqueLayer() const;

    /// Retrieve unique detRegion
    unsigned int uniqueDetRegion() const;

    /// Print this STChannelID in a human readable way
    std::ostream& fillStream( std::ostream& s ) const;

    /// Print method for python NOT NEEDED + SLOW IN C++ use fillStream
    std::string toString() const;

    /// Retrieve const  ST Channel ID
    unsigned int channelID() const;

    /// Update  ST Channel ID
    void setChannelID( unsigned int value );

    /// Retrieve strip
    unsigned int strip() const;

    /// Retrieve station
    unsigned int station() const;

    /// Retrieve unique sector
    unsigned int uniqueSector() const;

    friend std::ostream& operator<<( std::ostream& str, const STChannelID& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Offsets of bitfield channelID
    enum channelIDBits {
      stripBits     = 0,
      sectorBits    = 10,
      detRegionBits = 15,
      layerBits     = 18,
      stationBits   = 21,
      typeBits      = 23
    };

    /// Bitmasks for bitfield channelID
    enum channelIDMasks {
      stripMask           = 0x3ffL,
      sectorMask          = 0x7c00L,
      detRegionMask       = 0x38000L,
      layerMask           = 0x1c0000L,
      stationMask         = 0x600000L,
      typeMask            = 0x1800000L,
      uniqueLayerMask     = layerMask + stationMask,
      uniqueDetRegionMask = detRegionMask + layerMask + stationMask,
      uniqueSectorMask    = sectorMask + detRegionMask + layerMask + stationMask
    };

    unsigned int m_channelID; ///< ST Channel ID

  }; // class STChannelID

  inline std::ostream& operator<<( std::ostream& s, LHCb::STChannelID::detType e ) {
    switch ( e ) {
    case LHCb::STChannelID::typeTT:
      return s << "typeTT";
    case LHCb::STChannelID::typeIT:
      return s << "typeIT";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::STChannelID::detType";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::STChannelID::STChannelID( const unsigned int iType, const unsigned int iStation, const unsigned int iLayer,
                                       const unsigned int iDetRegion, const unsigned int iSector,
                                       const unsigned int iStrip ) {

  m_channelID = ( iType << typeBits ) + ( iStation << stationBits ) + ( iLayer << layerBits ) +
                ( iDetRegion << detRegionBits ) + ( iSector << sectorBits ) + ( iStrip << stripBits );
}

inline unsigned int LHCb::STChannelID::channelID() const { return m_channelID; }

inline void LHCb::STChannelID::setChannelID( unsigned int value ) { m_channelID = value; }

inline unsigned int LHCb::STChannelID::strip() const {
  return (unsigned int)( ( m_channelID & stripMask ) >> stripBits );
}

inline unsigned int LHCb::STChannelID::station() const {
  return (unsigned int)( ( m_channelID & stationMask ) >> stationBits );
}

inline unsigned int LHCb::STChannelID::uniqueSector() const {
  return (unsigned int)( ( m_channelID & uniqueSectorMask ) >> sectorBits );
}

inline LHCb::STChannelID::operator int() const { return m_channelID; }

inline unsigned int LHCb::STChannelID::type() const { return ( m_channelID & typeMask ) >> typeBits; }

inline bool LHCb::STChannelID::isTT() const { return type() == LHCb::STChannelID::detType::typeTT; }

inline bool LHCb::STChannelID::isIT() const { return type() == LHCb::STChannelID::detType::typeIT; }

inline unsigned int LHCb::STChannelID::sector() const {

  return (unsigned int)( ( m_channelID & sectorMask ) >> sectorBits );
}

inline unsigned int LHCb::STChannelID::detRegion() const {

  return (unsigned int)( ( m_channelID & detRegionMask ) >> detRegionBits );
}

inline unsigned int LHCb::STChannelID::layer() const {

  return (unsigned int)( ( m_channelID & layerMask ) >> layerBits );
}

inline unsigned int LHCb::STChannelID::uniqueLayer() const {

  return (unsigned int)( ( m_channelID & uniqueLayerMask ) >> layerBits );
}

inline unsigned int LHCb::STChannelID::uniqueDetRegion() const {

  return (unsigned int)( ( m_channelID & uniqueDetRegionMask ) >> detRegionBits );
}
