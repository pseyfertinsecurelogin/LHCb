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

  /** @class HCCellID HCCellID.h
   *
   * This class identifies a single PMT in the HC
   *
   * @author Victor Coco
   *
   */

  class HCCellID final {
  public:
    /// Constructor with crate and channel
    HCCellID( unsigned int crate, unsigned int channel );

    /// Constructor with cellID
    HCCellID( int id ) : m_cellID( id ) {}

    /// Default Constructor
    HCCellID() : m_cellID( 0 ) {}

    /// Cast
    operator unsigned int() const;

    /// Special serializer to ASCII stream
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  HC Cell ID
    unsigned int cellID() const;

    /// Update  HC Cell ID
    void setCellID( unsigned int value );

    /// Retrieve channel number
    unsigned int channel() const;

    /// Update channel number
    void setChannel( unsigned int value );

    /// Retrieve crate number
    unsigned int crate() const;

    /// Update crate number
    void setCrate( unsigned int value );

    friend std::ostream& operator<<( std::ostream& str, const HCCellID& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Offsets of bitfield cellID
    enum cellIDBits { channelBits = 0, crateBits = 6 };

    /// Bitmasks for bitfield cellID
    enum cellIDMasks { channelMask = 0x3fL, crateMask = 0x7c0L };

    unsigned int m_cellID; ///< HC Cell ID

  }; // class HCCellID

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::HCCellID::HCCellID( unsigned int crate, unsigned int channel ) {

  m_cellID = ( crate << crateBits ) | channel;
}

inline unsigned int LHCb::HCCellID::cellID() const { return m_cellID; }

inline void LHCb::HCCellID::setCellID( unsigned int value ) { m_cellID = value; }

inline unsigned int LHCb::HCCellID::channel() const {
  return (unsigned int)( ( m_cellID & channelMask ) >> channelBits );
}

inline void LHCb::HCCellID::setChannel( unsigned int value ) {
  unsigned int val = (unsigned int)value;
  m_cellID &= ~channelMask;
  m_cellID |= ( ( ( (unsigned int)val ) << channelBits ) & channelMask );
}

inline unsigned int LHCb::HCCellID::crate() const { return (unsigned int)( ( m_cellID & crateMask ) >> crateBits ); }

inline void LHCb::HCCellID::setCrate( unsigned int value ) {
  unsigned int val = (unsigned int)value;
  m_cellID &= ~crateMask;
  m_cellID |= ( ( ( (unsigned int)val ) << crateBits ) & crateMask );
}

inline LHCb::HCCellID::operator unsigned int() const { return m_cellID; }
