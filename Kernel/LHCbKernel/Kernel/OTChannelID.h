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

  /** @class OTChannelID OTChannelID.h
   *
   * Simple class that represents a straw and the tdc time of a hit
   *
   * @author Jeroen van Tilburg and Jacopo Nardulli
   *
   */

  class OTChannelID final {
  public:
    /// Number of unique sequential IDs
    enum NumUniqueIDEnum {
      NumSequentialLayer   = 12,
      NumSequentialQuarter = 48,
      NumSequentialModule  = 432,
      NumSequentialOtis    = 432 * 4,
      NumSequentialStraw   = 432 * 128
    };

    /// Explicit constructor from the geometrical location of the straw and the tdc time of the hit
    OTChannelID( unsigned int station, unsigned int layer, unsigned int quarter, unsigned int module,
                 unsigned int straw, unsigned int tdcTime = 0u );

    /// Constructor from int
    OTChannelID( unsigned int id ) : m_channelID( id ) {}

    /// Default Constructor
    OTChannelID() = default;

    /// Comparison equality
    bool operator==( const OTChannelID& aChannel ) const;

    /// Comparison <
    bool operator<( const OTChannelID& aChannel ) const;

    /// Comparison >
    bool operator>( const OTChannelID& aChannel ) const;

    /// Operator overload, to cast channel ID to unsigned int. Used by linkers where the key (channel id) is an int
    operator unsigned int() const;

    /// Returns the geometrical part of the channelID
    [[nodiscard]] unsigned int geometry() const;

    /// True if same geometric channel
    [[nodiscard]] bool sameGeometry( const OTChannelID& testChannel ) const;

    /// true if same tdc counts
    [[nodiscard]] bool sameTime( const OTChannelID& testChannel ) const;

    /// Returns a layer id in range [0,11]
    [[nodiscard]] unsigned int sequentialUniqueLayer() const;

    /// Returns a quarter id in range [0,47]
    [[nodiscard]] unsigned int sequentialUniqueQuarter() const;

    /// Returns a module id in range [0,431]
    [[nodiscard]] unsigned int sequentialUniqueModule() const;

    /// Returns an otis id in range [0,1727]
    [[nodiscard]] unsigned int sequentialUniqueOtis() const;

    /// Returns a straw id in range [0,55295]
    [[nodiscard]] unsigned int sequentialUniqueStraw() const;

    /// Print this OTChannelID in a human readable way
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  OT Channel ID of straw
    [[nodiscard]] unsigned int channelID() const;

    /// Update  OT Channel ID of straw
    void setChannelID( unsigned int value );

    /// Retrieve TDC-time of hit
    [[nodiscard]] unsigned int tdcTime() const;

    /// Update TDC-time of hit
    void setTdcTime( unsigned int value );

    /// Retrieve Straw id
    [[nodiscard]] unsigned int straw() const;

    /// Retrieve Module id
    [[nodiscard]] unsigned int module() const;

    /// Retrieve Quarter id
    [[nodiscard]] unsigned int quarter() const;

    /// Retrieve Layer id
    [[nodiscard]] unsigned int layer() const;

    /// Retrieve Station id
    [[nodiscard]] unsigned int station() const;

    /// Retrieve Unique layer id, i.e. this layer belong to this station
    [[nodiscard]] unsigned int uniqueLayer() const;

    /// Retrieve Unique quarter id, i.e. this quarter belongs to this layer and station
    [[nodiscard]] unsigned int uniqueQuarter() const;

    /// Retrieve Unique module id, i.e. this module belongs to this quarter, layer and station
    [[nodiscard]] unsigned int uniqueModule() const;

    /// Retrieve Unique straw, i.e. this straw belongs to this module, quarter, layer and station
    [[nodiscard]] unsigned int uniqueStraw() const;

    friend std::ostream& operator<<( std::ostream& str, const OTChannelID& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Offsets of bitfield channelID
    enum channelIDBits {
      tdcTimeBits = 0,
      strawBits   = 8,
      moduleBits  = 16,
      quarterBits = 20,
      layerBits   = 22,
      stationBits = 24
    };

    /// Bitmasks for bitfield channelID
    enum channelIDMasks {
      tdcTimeMask       = 0xffL,
      strawMask         = 0xff00L,
      moduleMask        = 0xf0000L,
      quarterMask       = 0x300000L,
      layerMask         = 0xc00000L,
      stationMask       = 0x3000000L,
      uniqueLayerMask   = layerMask + stationMask,
      uniqueQuarterMask = quarterMask + layerMask + stationMask,
      uniqueModuleMask  = moduleMask + quarterMask + layerMask + stationMask,
      uniqueStrawMask   = strawMask + moduleMask + quarterMask + layerMask + stationMask
    };

    unsigned int m_channelID{0}; ///< OT Channel ID of straw

  }; // class OTChannelID

  inline std::ostream& operator<<( std::ostream& s, LHCb::OTChannelID::NumUniqueIDEnum e ) {
    switch ( e ) {
    case LHCb::OTChannelID::NumSequentialLayer:
      return s << "NumSequentialLayer";
    case LHCb::OTChannelID::NumSequentialQuarter:
      return s << "NumSequentialQuarter";
    case LHCb::OTChannelID::NumSequentialModule:
      return s << "NumSequentialModule";
    case LHCb::OTChannelID::NumSequentialOtis:
      return s << "NumSequentialOtis";
    case LHCb::OTChannelID::NumSequentialStraw:
      return s << "NumSequentialStraw";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::OTChannelID::NumUniqueIDEnum";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::OTChannelID::OTChannelID( unsigned int station, unsigned int layer, unsigned int quarter,
                                       unsigned int module, unsigned int straw, unsigned int tdcTime ) {

  m_channelID = ( station << stationBits ) + ( layer << layerBits ) + ( quarter << quarterBits ) +
                ( module << moduleBits ) + ( straw << strawBits ) + ( tdcTime << tdcTimeBits );
}

inline unsigned int LHCb::OTChannelID::channelID() const { return m_channelID; }

inline void LHCb::OTChannelID::setChannelID( unsigned int value ) { m_channelID = value; }

inline unsigned int LHCb::OTChannelID::tdcTime() const {
  return (unsigned int)( ( m_channelID & tdcTimeMask ) >> tdcTimeBits );
}

inline void LHCb::OTChannelID::setTdcTime( unsigned int value ) {
  m_channelID &= ~tdcTimeMask;
  m_channelID |= ( value << tdcTimeBits ) & tdcTimeMask;
}

inline unsigned int LHCb::OTChannelID::straw() const {
  return (unsigned int)( ( m_channelID & strawMask ) >> strawBits );
}

inline unsigned int LHCb::OTChannelID::module() const {
  return (unsigned int)( ( m_channelID & moduleMask ) >> moduleBits );
}

inline unsigned int LHCb::OTChannelID::quarter() const {
  return (unsigned int)( ( m_channelID & quarterMask ) >> quarterBits );
}

inline unsigned int LHCb::OTChannelID::layer() const {
  return (unsigned int)( ( m_channelID & layerMask ) >> layerBits );
}

inline unsigned int LHCb::OTChannelID::station() const {
  return (unsigned int)( ( m_channelID & stationMask ) >> stationBits );
}

inline unsigned int LHCb::OTChannelID::uniqueLayer() const {
  return (unsigned int)( ( m_channelID & uniqueLayerMask ) >> layerBits );
}

inline unsigned int LHCb::OTChannelID::uniqueQuarter() const {
  return (unsigned int)( ( m_channelID & uniqueQuarterMask ) >> quarterBits );
}

inline unsigned int LHCb::OTChannelID::uniqueModule() const {
  return (unsigned int)( ( m_channelID & uniqueModuleMask ) >> moduleBits );
}

inline unsigned int LHCb::OTChannelID::uniqueStraw() const {
  return (unsigned int)( ( m_channelID & uniqueStrawMask ) >> strawBits );
}

inline bool LHCb::OTChannelID::operator==( const OTChannelID& aChannel ) const {
  return ( this->channelID() == aChannel.channelID() );
}

inline bool LHCb::OTChannelID::operator<( const OTChannelID& aChannel ) const {
  return ( this->channelID() < aChannel.channelID() );
}

inline bool LHCb::OTChannelID::operator>( const OTChannelID& aChannel ) const {
  return ( this->channelID() > aChannel.channelID() );
}

inline LHCb::OTChannelID::operator unsigned int() const { return m_channelID; }

inline unsigned int LHCb::OTChannelID::geometry() const { return ( m_channelID & uniqueStrawMask ); }

inline bool LHCb::OTChannelID::sameGeometry( const OTChannelID& testChannel ) const {
  return ( geometry() == testChannel.geometry() );
}

inline bool LHCb::OTChannelID::sameTime( const OTChannelID& testChannel ) const {
  return ( tdcTime() == testChannel.tdcTime() );
}

inline unsigned int LHCb::OTChannelID::sequentialUniqueLayer() const { return ( station() - 1 ) * 4 + layer(); }

inline unsigned int LHCb::OTChannelID::sequentialUniqueQuarter() const {
  return sequentialUniqueLayer() * 4 + quarter();
}

inline unsigned int LHCb::OTChannelID::sequentialUniqueModule() const {
  return sequentialUniqueQuarter() * 9 + module() - 1;
}

inline unsigned int LHCb::OTChannelID::sequentialUniqueOtis() const {
  return sequentialUniqueModule() * 4 + ( straw() - 1 ) / 32;
}

inline unsigned int LHCb::OTChannelID::sequentialUniqueStraw() const {
  return sequentialUniqueModule() * 128 + ( straw() - 1 );
}
