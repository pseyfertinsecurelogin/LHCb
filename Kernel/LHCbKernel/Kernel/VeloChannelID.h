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

  /** @class VeloChannelID VeloChannelID.h
   *
   * This class describes a single channel of LHCb VELO Fully qualified
   * constructor: sets strip and sensor numbers and sensor type
   *
   * @author Chris Parkes, modified by David Hutchcroft
   *
   */

  class VeloChannelID final {
  public:
    /// Enumerates the 3 sensor types
    enum sensorType { Null = 0, RType = 1, PhiType = 2, PileUpType = 3 };

    /// constructor with sensor, strip, and sensor type
    VeloChannelID( unsigned int sensor, unsigned int strip, const VeloChannelID::sensorType& type );

    /// constructor with sensor and strip number only (for backwards compatibility)
    VeloChannelID( unsigned int sensor, unsigned int strip );

    /// constructor with channelID
    VeloChannelID( int id ) : m_channelID( id ) {}

    /// Default Constructor
    VeloChannelID() : m_channelID( 0 ) {}

    /// Set sensor type bits
    void setType( const VeloChannelID::sensorType& type );

    /// Get sensor type
    VeloChannelID::sensorType type() const;

    /// Is the channel in a pile up sensor
    bool isPileUp() const;

    /// Is the channel in an R type sensor
    bool isRType() const;

    /// Is the channel in a phi type sensor
    bool isPhiType() const;

    /// cast
    operator long() const;

    /// special serializer to ASCII stream
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  VELO Channel ID
    unsigned int channelID() const;

    /// Update  VELO Channel ID
    void setChannelID( unsigned int value );

    /// Retrieve Strip number
    unsigned int strip() const;

    /// Update Strip number
    void setStrip( unsigned int value );

    /// Retrieve Sensor number
    unsigned int sensor() const;

    /// Update Sensor number
    void setSensor( unsigned int value );

    friend std::ostream& operator<<( std::ostream& str, const VeloChannelID& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Offsets of bitfield channelID
    enum channelIDBits { stripBits = 0, sensorBits = 11, typeBits = 19 };

    /// Bitmasks for bitfield channelID
    enum channelIDMasks { stripMask = 0x7ffL, sensorMask = 0x7f800L, typeMask = 0x180000L };

    unsigned int m_channelID; ///< VELO Channel ID

  }; // class VeloChannelID

  inline std::ostream& operator<<( std::ostream& s, LHCb::VeloChannelID::sensorType e ) {
    switch ( e ) {
    case LHCb::VeloChannelID::Null:
      return s << "Null";
    case LHCb::VeloChannelID::RType:
      return s << "RType";
    case LHCb::VeloChannelID::PhiType:
      return s << "PhiType";
    case LHCb::VeloChannelID::PileUpType:
      return s << "PileUpType";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::VeloChannelID::sensorType";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::VeloChannelID::VeloChannelID( unsigned int sensor, unsigned int strip,
                                           const VeloChannelID::sensorType& type ) {

  m_channelID = ( sensor << sensorBits ) | ( type << typeBits ) | strip;
}

inline LHCb::VeloChannelID::VeloChannelID( unsigned int sensor, unsigned int strip ) : m_channelID( 0 ) {

  setSensor( sensor );
  setStrip( strip );
  if ( 64 > sensor )
    setType( VeloChannelID::sensorType::RType );
  else if ( 128 > sensor )
    setType( VeloChannelID::sensorType::PhiType );
  else
    setType( VeloChannelID::sensorType::PileUpType );
}

inline unsigned int LHCb::VeloChannelID::channelID() const { return m_channelID; }

inline void LHCb::VeloChannelID::setChannelID( unsigned int value ) { m_channelID = value; }

inline unsigned int LHCb::VeloChannelID::strip() const {
  return (unsigned int)( ( m_channelID & stripMask ) >> stripBits );
}

inline void LHCb::VeloChannelID::setStrip( unsigned int value ) {
  unsigned int val = (unsigned int)value;
  m_channelID &= ~stripMask;
  m_channelID |= ( ( ( (unsigned int)val ) << stripBits ) & stripMask );
}

inline unsigned int LHCb::VeloChannelID::sensor() const {
  return (unsigned int)( ( m_channelID & sensorMask ) >> sensorBits );
}

inline void LHCb::VeloChannelID::setSensor( unsigned int value ) {
  unsigned int val = (unsigned int)value;
  m_channelID &= ~sensorMask;
  m_channelID |= ( ( ( (unsigned int)val ) << sensorBits ) & sensorMask );
}

inline void LHCb::VeloChannelID::setType( const VeloChannelID::sensorType& type ) {

  m_channelID &= ~typeMask;
  m_channelID |= ( (unsigned long)( type ) << typeBits ) & typeMask;
}

inline LHCb::VeloChannelID::sensorType LHCb::VeloChannelID::type() const {

  return sensorType( ( m_channelID & typeMask ) >> typeBits );
}

inline bool LHCb::VeloChannelID::isPileUp() const { return ( VeloChannelID::sensorType::PileUpType == type() ); }

inline bool LHCb::VeloChannelID::isRType() const { return ( VeloChannelID::sensorType::RType == type() ); }

inline bool LHCb::VeloChannelID::isPhiType() const { return ( VeloChannelID::sensorType::PhiType == type() ); }

inline LHCb::VeloChannelID::operator long() const { return m_channelID; }
