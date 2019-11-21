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

  /** @class UTChannelID UTChannelID.h
   *
   * Channel ID for class for UT
   *
   * @author A Beiter (based on code by M Needham, J. Wang)
   *
   */

  class UTChannelID final {
  public:
    /// types of sub-detector channel ID
    enum detType { typeUT = 2 };

    /// constructor with station, layer, detRegion, sector , strip,
    UTChannelID( const unsigned int iType, const unsigned int iStation, const unsigned int iLayer,
                 const unsigned int iDetRegion, const unsigned int iSector, const unsigned int iStrip );

    /// constructor with int
    explicit UTChannelID( int id ) : m_channelID( id ) {}

    /// Default Constructor
    UTChannelID() = default;

    /// cast
    operator int() const;

    /// Retrieve type
    [[nodiscard]] unsigned int type() const;

    /// test whether UT or not
    [[nodiscard]] bool isUT() const;

    /// Retrieve sector
    [[nodiscard]] unsigned int sector() const;

    /// Retrieve detRegion
    [[nodiscard]] unsigned int detRegion() const;

    /// Retrieve layer
    [[nodiscard]] unsigned int layer() const;

    /// Retrieve unique layer
    [[nodiscard]] unsigned int uniqueLayer() const;

    /// Retrieve unique detRegion
    [[nodiscard]] unsigned int uniqueDetRegion() const;

    /// Print this UTChannelID in a human readable way
    std::ostream& fillStream( std::ostream& s ) const;

    /// Print method for python NOT NEEDED + SLOW IN C++ use fillStream
    [[nodiscard]] std::string toString() const;

    /// Retrieve const  UT Channel ID
    [[nodiscard]] unsigned int channelID() const;

    /// Update  UT Channel ID
    void setChannelID( unsigned int value );

    /// Retrieve strip
    [[nodiscard]] unsigned int strip() const;

    /// Retrieve station
    [[nodiscard]] unsigned int station() const;

    /// Retrieve unique sector
    [[nodiscard]] unsigned int uniqueSector() const;

    friend std::ostream& operator<<( std::ostream& str, const UTChannelID& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Offsets of UT bitfield that are different from TT/IT
    enum channelIDBitsUT { sectorBitsUT = 10, detRegionBitsUT = 17, layerBitsUT = 19 };
    /// Bitmasks for UT bitfield that are different from TT/IT
    enum channelIDMasksUT {
      sectorMaskUT          = 0x1fc00L,
      detRegionMaskUT       = 0x60000L,
      layerMaskUT           = 0x180000L,
      stationMaskUT         = 0x600000L,
      uniqueLayerMaskUT     = layerMaskUT + stationMaskUT,
      uniqueDetRegionMaskUT = detRegionMaskUT + layerMaskUT + stationMaskUT
    };

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

    unsigned int m_channelID{0}; ///< UT Channel ID

  }; // class UTChannelID

  inline std::ostream& operator<<( std::ostream& s, LHCb::UTChannelID::detType e ) {
    switch ( e ) {
    case LHCb::UTChannelID::typeUT:
      return s << "typeUT";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::UTChannelID::detType";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::UTChannelID::UTChannelID( const unsigned int iType, const unsigned int iStation, const unsigned int iLayer,
                                       const unsigned int iDetRegion, const unsigned int iSector,
                                       const unsigned int iStrip ) {

  m_channelID = ( iType << typeBits ) + ( iStation << stationBits ) + ( iLayer << layerBitsUT ) +
                ( iDetRegion << detRegionBitsUT ) + ( iSector << sectorBitsUT ) + ( iStrip << stripBits );
}

inline unsigned int LHCb::UTChannelID::channelID() const { return m_channelID; }

inline void LHCb::UTChannelID::setChannelID( unsigned int value ) { m_channelID = value; }

inline unsigned int LHCb::UTChannelID::strip() const {
  return (unsigned int)( ( m_channelID & stripMask ) >> stripBits );
}

inline unsigned int LHCb::UTChannelID::station() const {
  return (unsigned int)( ( m_channelID & stationMask ) >> stationBits );
}

inline unsigned int LHCb::UTChannelID::uniqueSector() const {
  return (unsigned int)( ( m_channelID & uniqueSectorMask ) >> sectorBits );
}

inline LHCb::UTChannelID::operator int() const { return m_channelID; }

inline unsigned int LHCb::UTChannelID::type() const { return ( m_channelID & typeMask ) >> typeBits; }

inline bool LHCb::UTChannelID::isUT() const { return type() == LHCb::UTChannelID::detType::typeUT; }

inline unsigned int LHCb::UTChannelID::sector() const {

  return (unsigned int)( ( m_channelID & sectorMaskUT ) >> sectorBitsUT );
}

inline unsigned int LHCb::UTChannelID::detRegion() const {

  return (unsigned int)( ( m_channelID & detRegionMaskUT ) >> detRegionBitsUT );
}

inline unsigned int LHCb::UTChannelID::layer() const {

  return (unsigned int)( ( m_channelID & layerMaskUT ) >> layerBitsUT );
}

inline unsigned int LHCb::UTChannelID::uniqueLayer() const {

  return (unsigned int)( ( m_channelID & uniqueLayerMaskUT ) >> layerBitsUT );
}

inline unsigned int LHCb::UTChannelID::uniqueDetRegion() const {

  return (unsigned int)( ( m_channelID & uniqueDetRegionMaskUT ) >> detRegionBitsUT );
}
