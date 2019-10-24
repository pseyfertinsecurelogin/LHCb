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

  /** @class FTChannelID FTChannelID.h
   *
   * Channel ID for the Fibre Tracker (LHCb Upgrade)
   *
   * @author FT software team
   *
   */

  class FTChannelID final {
  public:
    /// Explicit constructor from the geometrical location
    FTChannelID( unsigned int station, unsigned int layer, unsigned int quarter, unsigned int module, unsigned int mat,
                 unsigned int sipm, unsigned int channel );

    /// Explicit constructor from the geometrical location
    FTChannelID( unsigned int station, unsigned int layer, unsigned int quarter, unsigned int module,
                 unsigned int channelInModule );

    /// Constructor from int
    FTChannelID( unsigned int id ) : m_channelID( id ) {}

    /// Default Constructor
    FTChannelID() {}

    /// Operator overload, to cast channel ID to unsigned int.                         Used by linkers where the key
    /// (channel id) is an int
    operator unsigned int() const;

    /// Comparison equality
    bool operator==( const FTChannelID& aChannel ) const;

    /// Comparison <
    bool operator<( const FTChannelID& aChannel ) const;

    /// Comparison >
    bool operator>( const FTChannelID& aChannel ) const;

    /// Increment the channelID
    void next();

    /// Increment the channelID
    void addToChannel( const int offset );

    /// Return the SiPM number within the module (0-15)
    [[nodiscard]] unsigned int sipmInModule() const;

    /// Return the die number (0 or 1)
    [[nodiscard]] unsigned int die() const;

    /// Return true if channelID is in x-layer
    [[nodiscard]] bool isX() const;

    /// Return true if channelID is in bottom part of detector
    [[nodiscard]] bool isBottom() const;

    /// Return true if channelID is in top part of detector
    [[nodiscard]] bool isTop() const;

    /// Print this FTChannelID in a human readable way
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  FT Channel ID
    [[nodiscard]] unsigned int channelID() const;

    /// Update  FT Channel ID
    void setChannelID( unsigned int value );

    /// Retrieve Channel in the 128 channel SiPM
    [[nodiscard]] unsigned int channel() const;

    /// Retrieve ID of the SiPM in the mat
    [[nodiscard]] unsigned int sipm() const;

    /// Retrieve ID of the mat in the module
    [[nodiscard]] unsigned int mat() const;

    /// Retrieve Module id (0 - 5 or 0 - 6)
    [[nodiscard]] unsigned int module() const;

    /// Retrieve Quarter ID (0 - 3)
    [[nodiscard]] unsigned int quarter() const;

    /// Retrieve Layer id
    [[nodiscard]] unsigned int layer() const;

    /// Retrieve Station id
    [[nodiscard]] unsigned int station() const;

    /// Retrieve unique layer
    [[nodiscard]] unsigned int uniqueLayer() const;

    /// Retrieve unique quarter
    [[nodiscard]] unsigned int uniqueQuarter() const;

    /// Retrieve unique module
    [[nodiscard]] unsigned int uniqueModule() const;

    /// Retrieve unique mat
    [[nodiscard]] unsigned int uniqueMat() const;

    /// Retrieve unique SiPM
    [[nodiscard]] unsigned int uniqueSiPM() const;

    friend std::ostream& operator<<( std::ostream& str, const FTChannelID& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Offsets of bitfield channelID
    enum channelIDBits {
      channelBits = 0,
      sipmBits    = 7,
      matBits     = 9,
      moduleBits  = 11,
      quarterBits = 14,
      layerBits   = 16,
      stationBits = 18
    };

    /// Bitmasks for bitfield channelID
    enum channelIDMasks {
      channelMask       = 0x7fL,
      sipmMask          = 0x180L,
      matMask           = 0x600L,
      moduleMask        = 0x3800L,
      quarterMask       = 0xc000L,
      layerMask         = 0x30000L,
      stationMask       = 0xc0000L,
      uniqueLayerMask   = layerMask + stationMask,
      uniqueQuarterMask = quarterMask + layerMask + stationMask,
      uniqueModuleMask  = moduleMask + quarterMask + layerMask + stationMask,
      uniqueMatMask     = matMask + moduleMask + quarterMask + layerMask + stationMask,
      uniqueSiPMMask    = sipmMask + matMask + moduleMask + quarterMask + layerMask + stationMask
    };

    unsigned int m_channelID{0}; ///< FT Channel ID

  }; // class FTChannelID

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::FTChannelID::FTChannelID( unsigned int station, unsigned int layer, unsigned int quarter,
                                       unsigned int module, unsigned int mat, unsigned int sipm,
                                       unsigned int channel ) {

  m_channelID = ( station << stationBits ) + ( layer << layerBits ) + ( quarter << quarterBits ) +
                ( module << moduleBits ) + ( mat << matBits ) + ( sipm << sipmBits ) + ( channel << channelBits );
}

inline LHCb::FTChannelID::FTChannelID( unsigned int station, unsigned int layer, unsigned int quarter,
                                       unsigned int module, unsigned int channelInModule ) {

  m_channelID = ( station << stationBits ) + ( layer << layerBits ) + ( quarter << quarterBits ) +
                ( module << moduleBits ) + ( channelInModule << channelBits );
}

inline unsigned int LHCb::FTChannelID::channelID() const { return m_channelID; }

inline void LHCb::FTChannelID::setChannelID( unsigned int value ) { m_channelID = value; }

inline unsigned int LHCb::FTChannelID::channel() const {
  return (unsigned int)( ( m_channelID & channelMask ) >> channelBits );
}

inline unsigned int LHCb::FTChannelID::sipm() const { return (unsigned int)( ( m_channelID & sipmMask ) >> sipmBits ); }

inline unsigned int LHCb::FTChannelID::mat() const { return (unsigned int)( ( m_channelID & matMask ) >> matBits ); }

inline unsigned int LHCb::FTChannelID::module() const {
  return (unsigned int)( ( m_channelID & moduleMask ) >> moduleBits );
}

inline unsigned int LHCb::FTChannelID::quarter() const {
  return (unsigned int)( ( m_channelID & quarterMask ) >> quarterBits );
}

inline unsigned int LHCb::FTChannelID::layer() const {
  return (unsigned int)( ( m_channelID & layerMask ) >> layerBits );
}

inline unsigned int LHCb::FTChannelID::station() const {
  return (unsigned int)( ( m_channelID & stationMask ) >> stationBits );
}

inline unsigned int LHCb::FTChannelID::uniqueLayer() const {
  return (unsigned int)( ( m_channelID & uniqueLayerMask ) >> layerBits );
}

inline unsigned int LHCb::FTChannelID::uniqueQuarter() const {
  return (unsigned int)( ( m_channelID & uniqueQuarterMask ) >> quarterBits );
}

inline unsigned int LHCb::FTChannelID::uniqueModule() const {
  return (unsigned int)( ( m_channelID & uniqueModuleMask ) >> moduleBits );
}

inline unsigned int LHCb::FTChannelID::uniqueMat() const {
  return (unsigned int)( ( m_channelID & uniqueMatMask ) >> matBits );
}

inline unsigned int LHCb::FTChannelID::uniqueSiPM() const {
  return (unsigned int)( ( m_channelID & uniqueSiPMMask ) >> sipmBits );
}

inline LHCb::FTChannelID::operator unsigned int() const { return m_channelID; }

inline bool LHCb::FTChannelID::operator==( const FTChannelID& aChannel ) const {
  return ( this->channelID() == aChannel.channelID() );
}

inline bool LHCb::FTChannelID::operator<( const FTChannelID& aChannel ) const {
  return ( this->channelID() < aChannel.channelID() );
}

inline bool LHCb::FTChannelID::operator>( const FTChannelID& aChannel ) const {
  return ( this->channelID() > aChannel.channelID() );
}

inline void LHCb::FTChannelID::next() { ++m_channelID; }

inline void LHCb::FTChannelID::addToChannel( const int offset ) { m_channelID += offset; }

inline unsigned int LHCb::FTChannelID::sipmInModule() const {
  return ( m_channelID & ( matMask + sipmMask ) ) >> sipmBits;
}

inline unsigned int LHCb::FTChannelID::die() const { return ( m_channelID & 0x40 ) >> 6; }

inline bool LHCb::FTChannelID::isX() const { return ( layer() == 0 || layer() == 3 ); }

inline bool LHCb::FTChannelID::isBottom() const { return ( quarter() == 0 || quarter() == 1 ); }

inline bool LHCb::FTChannelID::isTop() const { return ( quarter() == 2 || quarter() == 3 ); }

inline std::ostream& LHCb::FTChannelID::fillStream( std::ostream& s ) const {

  return s << "{ FTChannelID : "
           << " channel =" << channel() << " sipm =" << sipm() << " mat =" << mat() << " module=" << module()
           << " quarter=" << quarter() << " layer=" << layer() << " station=" << station() << " }";
}
