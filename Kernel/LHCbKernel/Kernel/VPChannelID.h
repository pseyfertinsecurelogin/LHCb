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

  /** @class VPChannelID VPChannelID.h
   *
   * This class identifies a single pixel in the VP
   *
   * @author Victor Coco
   *
   */

  class VPChannelID final {
  public:
    /// Constructor with sensor, chip, column and row
    VPChannelID( unsigned int sensor, unsigned int chip, unsigned int col, unsigned int row );

    /// Constructor with channelID
    VPChannelID( int id ) : m_channelID( id ) {}

    /// Default Constructor
    VPChannelID() {}

    /// Cast
    operator unsigned int() const;

    /// Get sensor column number
    [[nodiscard]] unsigned int scol() const;

    /// Get module number
    [[nodiscard]] unsigned int module() const;

    /// Get station number
    [[nodiscard]] unsigned int station() const;

    /// Get side (left/right)
    [[nodiscard]] unsigned int sidepos() const;

    /// Special serializer to ASCII stream
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  VP Channel ID
    [[nodiscard]] unsigned int channelID() const;

    /// Update  VP Channel ID
    void setChannelID( unsigned int value );

    /// Retrieve pixel row
    [[nodiscard]] unsigned int row() const;

    /// Update pixel row
    void setRow( unsigned int value );

    /// Retrieve pixel column
    [[nodiscard]] unsigned int col() const;

    /// Update pixel column
    void setCol( unsigned int value );

    /// Retrieve chip number
    [[nodiscard]] unsigned int chip() const;

    /// Update chip number
    void setChip( unsigned int value );

    /// Retrieve sensor number
    [[nodiscard]] unsigned int sensor() const;

    /// Update sensor number
    void setSensor( unsigned int value );

    friend std::ostream& operator<<( std::ostream& str, const VPChannelID& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Offsets of bitfield channelID
    enum channelIDBits { rowBits = 0, colBits = 8, chipBits = 16, sensorBits = 18 };

    /// Bitmasks for bitfield channelID
    enum channelIDMasks { rowMask = 0xffL, colMask = 0xff00L, chipMask = 0x30000L, sensorMask = 0xffc0000L };

    unsigned int m_channelID{0}; ///< VP Channel ID

  }; // class VPChannelID

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::VPChannelID::VPChannelID( unsigned int sensor, unsigned int chip, unsigned int col, unsigned int row ) {

  m_channelID = ( sensor << sensorBits ) | ( chip << chipBits ) | ( col << colBits ) | row;
}

inline unsigned int LHCb::VPChannelID::channelID() const { return m_channelID; }

inline void LHCb::VPChannelID::setChannelID( unsigned int value ) { m_channelID = value; }

inline unsigned int LHCb::VPChannelID::row() const { return (unsigned int)( ( m_channelID & rowMask ) >> rowBits ); }

inline void LHCb::VPChannelID::setRow( unsigned int value ) {
  auto val = (unsigned int)value;
  m_channelID &= ~rowMask;
  m_channelID |= ( ( ( (unsigned int)val ) << rowBits ) & rowMask );
}

inline unsigned int LHCb::VPChannelID::col() const { return (unsigned int)( ( m_channelID & colMask ) >> colBits ); }

inline void LHCb::VPChannelID::setCol( unsigned int value ) {
  auto val = (unsigned int)value;
  m_channelID &= ~colMask;
  m_channelID |= ( ( ( (unsigned int)val ) << colBits ) & colMask );
}

inline unsigned int LHCb::VPChannelID::chip() const { return (unsigned int)( ( m_channelID & chipMask ) >> chipBits ); }

inline void LHCb::VPChannelID::setChip( unsigned int value ) {
  auto val = (unsigned int)value;
  m_channelID &= ~chipMask;
  m_channelID |= ( ( ( (unsigned int)val ) << chipBits ) & chipMask );
}

inline unsigned int LHCb::VPChannelID::sensor() const {
  return (unsigned int)( ( m_channelID & sensorMask ) >> sensorBits );
}

inline void LHCb::VPChannelID::setSensor( unsigned int value ) {
  auto val = (unsigned int)value;
  m_channelID &= ~sensorMask;
  m_channelID |= ( ( ( (unsigned int)val ) << sensorBits ) & sensorMask );
}

inline LHCb::VPChannelID::operator unsigned int() const { return m_channelID; }

inline unsigned int LHCb::VPChannelID::scol() const { return ( m_channelID & ( chipMask | colMask ) ) >> colBits; }

inline unsigned int LHCb::VPChannelID::module() const { return sensor() / 4; }

inline unsigned int LHCb::VPChannelID::station() const { return module() / 2; }

inline unsigned int LHCb::VPChannelID::sidepos() const { return module() % 2; }
