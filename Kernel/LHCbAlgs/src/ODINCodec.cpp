/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "ODINCodec.h"

#include "GaudiKernel/GaudiException.h"

using namespace LHCb;
using namespace LHCb::ODINCodec;

namespace {
  inline void Assert( const bool cond, const std::string& tag, const std::string& msg ) {
    if ( UNLIKELY( !( cond ) ) ) throw GaudiException( tag, msg, StatusCode::FAILURE );
  }
  inline void DecoderAssert( const bool cond, const std::string& msg ) {
    Assert( cond, "LHCb::ODINCodec::decode", msg );
  }
} // namespace

//=============================================================================
// ODIN Bank constants
//=============================================================================
namespace {
  const size_t bank_size_v2 = 9 * sizeof( int );

  namespace PreV6 {
    enum TriggerType {
      Reserve            = 0,
      PhysicsTrigger     = 1,
      AuxilliaryTrigger  = 2,
      RandomTrigger      = 3,
      PeriodicTrigger    = 4,
      NonZSupTrigger     = 5,
      TimingTrigger      = 6,
      CalibrationTrigger = 7
    };
  }
} // namespace

ODIN LHCb::ODINCodec::decode( const RawBank& bank, const bool ignoreBankVersion ) {
  ODIN               odin;
  unsigned long long temp64{0};
  unsigned int       temp32{0};

  // Check bank type
  DecoderAssert( bank.type() == LHCb::RawBank::ODIN, "Wrong ODIN raw bank type" );
  DecoderAssert( bank.magic() == LHCb::RawBank::MagicPattern, "Magic pattern mismatch in ODIN raw bank" );

  // Validate bank version
  const unsigned int version = bank.version();
  if ( UNLIKELY( ( version > Bank::VERSION ) && !ignoreBankVersion ) ) {
    DecoderAssert( false, "Unknown ODIN bank version " + std::to_string( version ) + ", latest known is " +
                              std::to_string( Bank::VERSION ) );
  }

  // Validate bank size
  const size_t size = bank.size();
  if ( UNLIKELY( ( ( version >= 3 ) && size != Bank::SIZE ) ||
                 ( ( version < 3 ) && size < bank_size_v2 ) ) ) { // relaxed check to take into account a bug in
                                                                  // BooleInit
    DecoderAssert( false, "Wrong ODIN bank size " + std::to_string( size ) + ", expected " +
                              std::to_string( ( version >= 3 ) ? Bank::SIZE : bank_size_v2 ) );
  }

  // pointer to the data
  auto odinData = bank.range<const unsigned int>();

  // Fill the ODIN object
  odin.setVersion( version );
  odin.setRunNumber( odinData[LHCb::ODIN::Data::RunNumber] );
  odin.setOrbitNumber( odinData[LHCb::ODIN::Data::OrbitNumber] );

  temp64 = odinData[LHCb::ODIN::Data::L0EventIDHi];
  odin.setEventNumber( ( temp64 << 32 ) + odinData[LHCb::ODIN::Data::L0EventIDLo] );

  temp64 = odinData[LHCb::ODIN::Data::GPSTimeHi];
  odin.setGpsTime( ( temp64 << 32 ) + odinData[LHCb::ODIN::Data::GPSTimeLo] );

  temp32 = odinData[LHCb::ODIN::Data::Word7];
  odin.setDetectorStatus( ( temp32 & LHCb::ODIN::Word7Masks::DetectorStatusMask ) >>
                          LHCb::ODIN::Word7Bits::DetectorStatusBits );
  odin.setErrorBits( ( temp32 & LHCb::ODIN::Word7Masks::ErrorMask ) >> LHCb::ODIN::Word7Bits::ErrorBits );

  temp32 = odinData[LHCb::ODIN::Data::Word8];
  odin.setBunchId( ( temp32 & LHCb::ODIN::Word8Masks::BunchIDMask ) >> LHCb::ODIN::Word8Bits::BunchIDBits );
  if ( version <= 5 ) {
    switch ( ( temp32 & LHCb::ODIN::Word8Masks::TriggerTypeMask ) >> LHCb::ODIN::Word8Bits::TriggerTypeBits ) {
    case PreV6::Reserve:
    case PreV6::PhysicsTrigger:
      odin.setTriggerType( LHCb::ODIN::TriggerType::PhysicsTrigger );
      break;
    case PreV6::AuxilliaryTrigger:
      odin.setTriggerType( LHCb::ODIN::TriggerType::AuxiliaryTrigger );
      break;
    case PreV6::RandomTrigger:
      odin.setTriggerType( LHCb::ODIN::TriggerType::LumiTrigger );
      break;
    case PreV6::PeriodicTrigger:
      odin.setTriggerType( LHCb::ODIN::TriggerType::TechnicalTrigger );
      break;
    case PreV6::NonZSupTrigger:
      odin.setTriggerType( LHCb::ODIN::TriggerType::NonZSupTrigger );
      break;
    case PreV6::TimingTrigger:
      odin.setTriggerType( LHCb::ODIN::TriggerType::TimingTrigger );
      break;
    case PreV6::CalibrationTrigger:
      odin.setTriggerType( LHCb::ODIN::TriggerType::CalibrationTrigger );
      break;
    default:
      odin.setTriggerType( LHCb::ODIN::TriggerType::PhysicsTrigger );
      break;
    }
  } else {
    switch ( ( temp32 & LHCb::ODIN::Word8Masks::TriggerTypeMask ) >> LHCb::ODIN::Word8Bits::TriggerTypeBits ) {
    case 0:
      odin.setTriggerType( LHCb::ODIN::TriggerType::PhysicsTrigger );
      break;
    case 1:
      odin.setTriggerType( LHCb::ODIN::TriggerType::BeamGasTrigger );
      break;
    case 2:
      odin.setTriggerType( LHCb::ODIN::TriggerType::LumiTrigger );
      break;
    case 3:
      odin.setTriggerType( LHCb::ODIN::TriggerType::TechnicalTrigger );
      break;
    case 4:
      odin.setTriggerType( LHCb::ODIN::TriggerType::AuxiliaryTrigger );
      break;
    case 5:
      odin.setTriggerType( LHCb::ODIN::TriggerType::NonZSupTrigger );
      break;
    case 6:
      odin.setTriggerType( LHCb::ODIN::TriggerType::TimingTrigger );
      break;
    case 7:
      odin.setTriggerType( LHCb::ODIN::TriggerType::CalibrationTrigger );
      break;
    default:
      odin.setTriggerType( LHCb::ODIN::TriggerType::PhysicsTrigger );
      break;
    }
  }

  if ( version >= 5 ) {
    switch ( ( temp32 & LHCb::ODIN::Word8Masks::CalibrationTypeMask ) >> LHCb::ODIN::Word8Bits::CalibrationTypeBits ) {
    case 0:
      odin.setCalibrationType( LHCb::ODIN::CalibrationTypes::A );
      break;
    case 1:
      odin.setCalibrationType( LHCb::ODIN::CalibrationTypes::B );
      break;
    case 2:
      odin.setCalibrationType( LHCb::ODIN::CalibrationTypes::C );
      break;
    case 3:
      odin.setCalibrationType( LHCb::ODIN::CalibrationTypes::D );
      break;
    default:
      odin.setCalibrationType( LHCb::ODIN::CalibrationTypes::A );
      break;
    }
  } else {
    odin.setReadoutType(
        ( ( temp32 & LHCb::ODIN::Word8Masks::CalibrationTypeMask ) >> LHCb::ODIN::Word8Bits::CalibrationTypeBits ) == 0
            ? LHCb::ODIN::ReadoutTypes::ZeroSuppressed
            : LHCb::ODIN::ReadoutTypes::NonZeroSuppressed );
  }

  if ( version >= 4 ) {
    odin.setTimeAlignmentEventWindow( ( temp32 & LHCb::ODIN::Word8Masks::TAEWindowMask ) >>
                                      LHCb::ODIN::Word8Bits::TAEWindowBits );
  }

  odin.setForceBit( ( ( temp32 & LHCb::ODIN::Word8Masks::ForceMask ) >> LHCb::ODIN::Word8Bits::ForceBits ) != 0 );

  switch ( ( temp32 & LHCb::ODIN::Word8Masks::BXTypeMask ) >> LHCb::ODIN::Word8Bits::BXTypeBits ) {
  case 0:
    odin.setBunchCrossingType( LHCb::ODIN::BXTypes::NoBeam );
    break;
  case 1:
    odin.setBunchCrossingType( LHCb::ODIN::BXTypes::Beam1 );
    break;
  case 2:
    odin.setBunchCrossingType( LHCb::ODIN::BXTypes::Beam2 );
    break;
  case 3:
    odin.setBunchCrossingType( LHCb::ODIN::BXTypes::BeamCrossing );
    break;
  default:
    odin.setBunchCrossingType( LHCb::ODIN::BXTypes::NoBeam );
    break;
  }

  odin.setBunchCurrent( ( temp32 & LHCb::ODIN::Word8Masks::BunchCurrentMask ) >>
                        LHCb::ODIN::Word8Bits::BunchCurrentBits );

  if ( version < 3 ) {

    odin.setEventType( odinData[LHCb::ODIN::Data::EventType] );

  } else {

    temp32 = odinData[LHCb::ODIN::Data::EventType];
    odin.setEventType( ( temp32 & LHCb::ODIN::EventTypeMasks::EventTypeMask ) >>
                       LHCb::ODIN::EventTypeBitsEnum::EventTypeBits );
    odin.setCalibrationStep( ( temp32 & LHCb::ODIN::EventTypeMasks::CalibrationStepMask ) >>
                             LHCb::ODIN::EventTypeBitsEnum::CalibrationStepBits );

    odin.setTriggerConfigurationKey( odinData[LHCb::ODIN::Data::TriggerConfigurationKey] );
  }

  return odin;
}

Bank::array LHCb::ODINCodec::encode( const LHCb::ODIN& odin ) {
  Bank::array data;
  data[LHCb::ODIN::Data::RunNumber] = odin.runNumber();
  data[LHCb::ODIN::Data::EventType] =
      ( ( odin.eventType() << LHCb::ODIN::EventTypeBitsEnum::EventTypeBits ) &
        LHCb::ODIN::EventTypeMasks::EventTypeMask ) |
      ( ( odin.calibrationStep() << LHCb::ODIN::EventTypeBitsEnum::CalibrationStepBits ) &
        LHCb::ODIN::EventTypeMasks::CalibrationStepMask );
  data[LHCb::ODIN::Data::OrbitNumber] = odin.orbitNumber();
  data[LHCb::ODIN::Data::L0EventIDHi] = (unsigned int)( ( odin.eventNumber() >> 32 ) & 0xFFFFFFFF );
  data[LHCb::ODIN::Data::L0EventIDLo] = (unsigned int)( ( odin.eventNumber() ) & 0xFFFFFFFF );
  data[LHCb::ODIN::Data::GPSTimeHi]   = (unsigned int)( ( odin.gpsTime() >> 32 ) & 0xFFFFFFFF );
  data[LHCb::ODIN::Data::GPSTimeLo]   = (unsigned int)( ( odin.gpsTime() ) & 0xFFFFFFFF );
  data[LHCb::ODIN::Data::Word7] =
      (unsigned int)( ( ( odin.detectorStatus() << LHCb::ODIN::Word7Bits::DetectorStatusBits ) &
                        LHCb::ODIN::Word7Masks::DetectorStatusMask ) |
                      ( ( odin.errorBits() << LHCb::ODIN::Word7Bits::ErrorBits ) &
                        LHCb::ODIN::Word7Masks::ErrorMask ) );
  // This conversion is needed to be able to disentangle the enum internal (C) representation
  // from the hardware ODIN bit codes.
  int triggerType = 0;
  switch ( odin.triggerType() ) {
  case LHCb::ODIN::TriggerType::PhysicsTrigger:
    triggerType = 0;
    break;
  case LHCb::ODIN::TriggerType::BeamGasTrigger:
    triggerType = 1;
    break;
  case LHCb::ODIN::TriggerType::LumiTrigger:
    triggerType = 2;
    break;
  case LHCb::ODIN::TriggerType::TechnicalTrigger:
    triggerType = 3;
    break;
  case LHCb::ODIN::TriggerType::AuxiliaryTrigger:
    triggerType = 4;
    break;
  case LHCb::ODIN::TriggerType::NonZSupTrigger:
    triggerType = 5;
    break;
  case LHCb::ODIN::TriggerType::TimingTrigger:
    triggerType = 6;
    break;
  case LHCb::ODIN::TriggerType::CalibrationTrigger:
    triggerType = 7;
    break;
  }
  data[LHCb::ODIN::Data::Word8] =
      ( ( odin.bunchId() << LHCb::ODIN::Word8Bits::BunchIDBits ) & LHCb::ODIN::Word8Masks::BunchIDMask ) |
      ( ( odin.timeAlignmentEventWindow() << LHCb::ODIN::Word8Bits::TAEWindowBits ) &
        LHCb::ODIN::Word8Masks::TAEWindowMask ) |
      ( ( triggerType << LHCb::ODIN::Word8Bits::TriggerTypeBits ) & LHCb::ODIN::Word8Masks::TriggerTypeMask ) |
      ( ( odin.calibrationType() << LHCb::ODIN::Word8Bits::CalibrationTypeBits ) &
        LHCb::ODIN::Word8Masks::CalibrationTypeMask ) |
      ( ( odin.forceBit() << LHCb::ODIN::Word8Bits::ForceBits ) & LHCb::ODIN::Word8Masks::ForceMask ) |
      ( ( odin.bunchCrossingType() << LHCb::ODIN::Word8Bits::BXTypeBits ) & LHCb::ODIN::Word8Masks::BXTypeMask ) |
      ( ( odin.bunchCurrent() << LHCb::ODIN::Word8Bits::BunchCurrentBits ) & LHCb::ODIN::Word8Masks::BunchCurrentMask );
  data[LHCb::ODIN::Data::TriggerConfigurationKey] = odin.triggerConfigurationKey();
  return data;
}
