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
#include "Kernel/CaloCellID.h"
#include "Kernel/FTChannelID.h"
#include "Kernel/HCCellID.h"
#include "Kernel/MuonTileID.h"
#include "Kernel/OTChannelID.h"
#include "Kernel/RichSmartID.h"
#include "Kernel/STChannelID.h"
#include "Kernel/UTChannelID.h"
#include "Kernel/VPChannelID.h"
#include "Kernel/VeloChannelID.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class LHCbID LHCbID.h
   *
   * LHCb wide channel identifier
   *
   * @author Marco Cattaneo
   *
   */

  class LHCbID final {
  public:
    /// types of sub-detector channel ID
    enum channelIDtype { Velo = 1, TT, IT, OT, Rich, Calo, Muon, VP, FT = 10, UT, HC };

    /// Constructor from unsigned int
    explicit LHCbID( unsigned int theID ) : m_lhcbID( theID ) {}

    /// Constructor from VeloChannelID
    LHCbID( const VeloChannelID& chanID );

    /// Constructor from VPChannelID, VP corresponding to pixel solution for upgrade
    LHCbID( const VPChannelID& chanID );

    /// Constructor from STChannelID
    LHCbID( const STChannelID& chanID );

    /// Constructor from UTChannelID
    LHCbID( const UTChannelID& chanID );

    /// Constructor from OTChannelID
    LHCbID( const OTChannelID& chanID );

    /// Constructor from RichSmartID
    LHCbID( const RichSmartID& chanID );

    /// Constructor from CaloCellID
    LHCbID( const CaloCellID& chanID );

    /// Constructor from MuonTileID
    LHCbID( const MuonTileID& chanID );

    /// Constructor from FTChannelID
    LHCbID( const FTChannelID& chanID );

    /// Constructor from HCCellID, HC standing for Herschel
    LHCbID( const HCCellID& cellID );

    /// Default Constructor
    LHCbID() : m_lhcbID( 0 ) {}

    /// comparison equality
    bool operator==( const LHCbID& chanID ) const;

    /// comparison ordering
    bool operator<( const LHCbID& chanID ) const;

    /// return true if this is a Velo identifier
    bool isVelo() const;

    /// return true if this is a Velo R identifier
    bool isVeloR() const;

    /// return true if this is a Velo Phi identifier
    bool isVeloPhi() const;

    /// return true if this is a Velo Pile up identifier
    bool isVeloPileUp() const;

    /// return the VeloChannelID
    VeloChannelID veloID() const;

    /// return true if this is a VP identifier
    bool isVP() const;

    /// return the VPChannelID
    VPChannelID vpID() const;

    /// return true if this is a TT Silicon Tracker identifier
    bool isTT() const;

    /// return true if this is a UT Silicon Tracker identifier
    bool isUT() const;

    /// return the UTChannelID
    UTChannelID utID() const;

    /// return true if this is a IT Silicon Tracker identifier
    bool isIT() const;

    /// return true if this is a Silicon Tracker identifier (i.e. TT, IT or UT)
    bool isST() const;

    /// return the STChannelID
    STChannelID stID() const;

    /// return true if this is a Outer Tracker identifier
    bool isOT() const;

    /// return the OTChannelID
    OTChannelID otID() const;

    /// return true if this is a Rich identifier
    bool isRich() const;

    /// return the richSmartID
    RichSmartID richID() const;

    /// return true if this is a Calo identifier
    bool isCalo() const;

    /// return the CaloCellID
    CaloCellID caloID() const;

    /// return true if this is a Muon identifier
    bool isMuon() const;

    /// return the MuonTileID
    MuonTileID muonID() const;

    /// return true if this is a Fibre Tracker identifier
    bool isFT() const;

    /// return the FTChannelID
    FTChannelID ftID() const;

    /// return true if this is a HC identifier
    bool isHC() const;

    /// return the HCCellID
    HCCellID hcID() const;

    /// Check the LHCbID sub-detector channel ID type identifier
    bool checkDetectorType( unsigned int channelIDType ) const;

    /// General ID: returns detector ID = internal unsigned int
    unsigned int channelID() const;

    /// Print this LHCbID in a human readable way
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  the internal representation
    unsigned int lhcbID() const;

    /// Update the ID bits (to recreate the channelID)
    void setID( unsigned int value );

    /// Retrieve the LHCb detector type bits
    unsigned int detectorType() const;

    /// Update the LHCb detector type bits
    void setDetectorType( unsigned int value );

    friend std::ostream& operator<<( std::ostream& str, const LHCbID& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Checks if a given bit is set
    bool isBitOn( const unsigned int pos ) const;

    /// Sets the given bit to the given value
    void setBit( const unsigned int pos, const unsigned int value );

    /// Offsets of bitfield lhcbID
    enum lhcbIDBits { IDBits = 0, detectorTypeBits = 28 };

    /// Bitmasks for bitfield lhcbID
    enum lhcbIDMasks { IDMask = 0xfffffffL, detectorTypeMask = 0xf0000000L };

    unsigned int m_lhcbID; ///< the internal representation

  }; // class LHCbID

  inline std::ostream& operator<<( std::ostream& s, LHCb::LHCbID::channelIDtype e ) {
    switch ( e ) {
    case LHCb::LHCbID::Velo:
      return s << "Velo";
    case LHCb::LHCbID::TT:
      return s << "TT";
    case LHCb::LHCbID::IT:
      return s << "IT";
    case LHCb::LHCbID::OT:
      return s << "OT";
    case LHCb::LHCbID::Rich:
      return s << "Rich";
    case LHCb::LHCbID::Calo:
      return s << "Calo";
    case LHCb::LHCbID::Muon:
      return s << "Muon";
    case LHCb::LHCbID::VP:
      return s << "VP";
    case LHCb::LHCbID::FT:
      return s << "FT";
    case LHCb::LHCbID::UT:
      return s << "UT";
    case LHCb::LHCbID::HC:
      return s << "HC";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::LHCbID::channelIDtype";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::LHCbID::LHCbID( const VeloChannelID& chanID ) {

  m_lhcbID = ( channelIDtype::Velo << detectorTypeBits ) + chanID;
}

inline LHCb::LHCbID::LHCbID( const VPChannelID& chanID ) {

  m_lhcbID = ( channelIDtype::VP << detectorTypeBits ) + chanID;
}

inline LHCb::LHCbID::LHCbID( const STChannelID& chanID ) {

  unsigned int stType = channelIDtype::TT;
  if ( chanID.isIT() ) stType = channelIDtype::IT;
  m_lhcbID = ( stType << detectorTypeBits ) + chanID;
}

inline LHCb::LHCbID::LHCbID( const UTChannelID& chanID ) {

  unsigned int utType = channelIDtype::UT;
  m_lhcbID            = ( utType << detectorTypeBits ) + chanID;
}

inline LHCb::LHCbID::LHCbID( const OTChannelID& chanID ) {

  m_lhcbID = ( channelIDtype::OT << detectorTypeBits ) + chanID;
}

inline LHCb::LHCbID::LHCbID( const RichSmartID& chanID ) : m_lhcbID( 0 ) {

  // Set the type to be RICH
  setDetectorType( channelIDtype::Rich );
  // Save the data bits
  setID( chanID.dataBitsOnly() );
  // Save the MaPMT/HPD flag in bit 27
  setBit( 27, chanID.idType() );
  // Set the validity bits
  setBit( 26, chanID.pixelDataAreValid() );
}

inline LHCb::LHCbID::LHCbID( const CaloCellID& chanID ) : m_lhcbID( 0 ) {

  setDetectorType( channelIDtype::Calo );
  setID( chanID.all() );
}

inline LHCb::LHCbID::LHCbID( const MuonTileID& chanID ) : m_lhcbID( 0 ) {

  setDetectorType( channelIDtype::Muon );
  setID( int( chanID ) );
}

inline LHCb::LHCbID::LHCbID( const FTChannelID& chanID ) {

  m_lhcbID = ( (unsigned int)channelIDtype::FT << detectorTypeBits ) + chanID;
}

inline LHCb::LHCbID::LHCbID( const HCCellID& cellID ) { m_lhcbID = ( channelIDtype::HC << detectorTypeBits ) + cellID; }

inline unsigned int LHCb::LHCbID::lhcbID() const { return m_lhcbID; }

inline void LHCb::LHCbID::setID( unsigned int value ) {
  unsigned int val = (unsigned int)value;
  m_lhcbID &= ~IDMask;
  m_lhcbID |= ( ( ( (unsigned int)val ) << IDBits ) & IDMask );
}

inline unsigned int LHCb::LHCbID::detectorType() const {
  return (unsigned int)( ( m_lhcbID & detectorTypeMask ) >> detectorTypeBits );
}

inline void LHCb::LHCbID::setDetectorType( unsigned int value ) {
  unsigned int val = (unsigned int)value;
  m_lhcbID &= ~detectorTypeMask;
  m_lhcbID |= ( ( ( (unsigned int)val ) << detectorTypeBits ) & detectorTypeMask );
}

inline bool LHCb::LHCbID::operator==( const LHCbID& chanID ) const { return ( this->lhcbID() == chanID.lhcbID() ); }

inline bool LHCb::LHCbID::operator<( const LHCbID& chanID ) const { return ( m_lhcbID < chanID.m_lhcbID ); }

inline bool LHCb::LHCbID::isVelo() const { return channelIDtype::Velo == detectorType(); }

inline bool LHCb::LHCbID::isVeloR() const { return isVelo() && veloID().isRType(); }

inline bool LHCb::LHCbID::isVeloPhi() const { return isVelo() && veloID().isPhiType(); }

inline bool LHCb::LHCbID::isVeloPileUp() const { return isVelo() && veloID().isPileUp(); }

inline LHCb::VeloChannelID LHCb::LHCbID::veloID() const {

  return ( isVelo() ? LHCb::VeloChannelID( m_lhcbID & IDMask ) : LHCb::VeloChannelID( 0xF0000000 ) );
}

inline bool LHCb::LHCbID::isVP() const { return channelIDtype::VP == detectorType(); }

inline LHCb::VPChannelID LHCb::LHCbID::vpID() const {

  return LHCb::VPChannelID( isVP() ? m_lhcbID & IDMask : 0xF0000000 );
}

inline bool LHCb::LHCbID::isTT() const { return channelIDtype::TT == detectorType(); }

inline bool LHCb::LHCbID::isUT() const { return channelIDtype::UT == detectorType(); }

inline LHCb::UTChannelID LHCb::LHCbID::utID() const {

  return ( isUT() ? LHCb::UTChannelID( m_lhcbID & IDMask ) : LHCb::UTChannelID( 0xF0000000 ) );
}

inline bool LHCb::LHCbID::isIT() const { return channelIDtype::IT == detectorType(); }

inline bool LHCb::LHCbID::isST() const { return isTT() || isIT(); }

inline LHCb::STChannelID LHCb::LHCbID::stID() const {

  return LHCb::STChannelID( isST() ? m_lhcbID & IDMask : 0xF0000000 );
}

inline bool LHCb::LHCbID::isOT() const { return ( channelIDtype::OT == detectorType() ); }

inline LHCb::OTChannelID LHCb::LHCbID::otID() const {

  return LHCb::OTChannelID( isOT() ? m_lhcbID & IDMask : 0xF0000000 );
}

inline bool LHCb::LHCbID::isRich() const { return channelIDtype::Rich == detectorType(); }

inline LHCb::RichSmartID LHCb::LHCbID::richID() const {

  // Create the RichSMartID data bits
  LHCb::RichSmartID::KeyType data( isRich() ? ( m_lhcbID & IDMask ) : 0 );
  // Create a temporary RichSmartID
  LHCb::RichSmartID tmpid( data );
  // Retrieve the MaPMT/HPD flag
  if ( isRich() ) { tmpid.setIDType( (LHCb::RichSmartID::IDType)isBitOn( 27 ) ); }
  // Object to return, with RICH and panel fields set
  LHCb::RichSmartID id( tmpid.rich(), tmpid.panel(), tmpid.pdNumInCol(), tmpid.pdCol(), tmpid.idType() );
  // Set pixels fields
  if ( isBitOn( 26 ) ) {
    id.setPixelRow( tmpid.pixelRow() );
    id.setPixelCol( tmpid.pixelCol() );
    if ( tmpid.idType() == LHCb::RichSmartID::HPDID ) { id.setPixelSubRow( tmpid.pixelSubRow() ); }
  }
  // return
  return id;
}

inline bool LHCb::LHCbID::isCalo() const { return channelIDtype::Calo == detectorType(); }

inline LHCb::CaloCellID LHCb::LHCbID::caloID() const {

  return isCalo() ? CaloCellID( m_lhcbID & IDMask ) : CaloCellID( 0xF0000000 );
}

inline bool LHCb::LHCbID::isMuon() const { return channelIDtype::Muon == detectorType(); }

inline LHCb::MuonTileID LHCb::LHCbID::muonID() const {

  return isMuon() ? MuonTileID( m_lhcbID & IDMask ) : MuonTileID( 0xF0000000 );
}

inline bool LHCb::LHCbID::isFT() const { return channelIDtype::FT == detectorType(); }

inline LHCb::FTChannelID LHCb::LHCbID::ftID() const {

  return ( isFT() ? LHCb::FTChannelID( m_lhcbID & IDMask ) : LHCb::FTChannelID( 0xF0000000 ) );
}

inline bool LHCb::LHCbID::isHC() const { return channelIDtype::HC == detectorType(); }

inline LHCb::HCCellID LHCb::LHCbID::hcID() const {

  return ( isHC() ? LHCb::HCCellID( m_lhcbID & IDMask ) : LHCb::HCCellID( 0xF0000000 ) );
}

inline bool LHCb::LHCbID::checkDetectorType( unsigned int channelIDType ) const {

  return ( channelIDType == detectorType() );
}

inline unsigned int LHCb::LHCbID::channelID() const {

  switch ( detectorType() ) {
  case channelIDtype::VP:
    return vpID().channelID();
  case channelIDtype::Velo:
    return veloID().channelID();
  case channelIDtype::UT:
    return utID().channelID();
  case channelIDtype::TT: // C++17 [[fall-through]]
  case channelIDtype::IT:
    return stID().channelID();
  case channelIDtype::OT:
    return otID().channelID();
  case channelIDtype::Rich:
    return richID().key();
  case channelIDtype::Calo: // C++17 [[ fall-through ]]
  case channelIDtype::Muon:
    return m_lhcbID & IDMask;
  case channelIDtype::FT:
    return ftID().channelID();
  case channelIDtype::HC:
    return hcID().cellID();
  default:
    return 0;
  }
}

inline bool LHCb::LHCbID::isBitOn( const unsigned int pos ) const { return ( 0 != ( lhcbID() & ( 1 << pos ) ) ); }

inline void LHCb::LHCbID::setBit( const unsigned int pos, const unsigned int value ) { m_lhcbID |= value << pos; }
