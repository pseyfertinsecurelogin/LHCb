#include "ODINCodec.h"

#include "GaudiKernel/GaudiException.h"

using namespace LHCb;
using namespace LHCb::ODINCodec;

namespace {
  inline void Assert(const bool cond, const std::string& tag, const std::string& msg) {
    if ( UNLIKELY( ! (cond) ) )
      throw GaudiException(tag, msg, StatusCode::FAILURE);
  }
  inline void DecoderAssert(const bool cond, const std::string& msg) {
    Assert(cond, "LHCb::ODINCodec::decode", msg);
  }
  inline void EncoderAssert(const bool cond, const std::string& msg) {
    Assert(cond, "LHCb::ODINCodec::encode", msg);
  }
}

//=============================================================================
// ODIN Bank constants
//=============================================================================
namespace {
  const size_t bank_size_v2 =  9 * sizeof(int);

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
}

ODIN LHCb::ODINCodec::decode( const RawBank& bank, const bool ignoreBankVersion ) {
  ODIN odin;
  unsigned long long temp64{0};
  unsigned int       temp32{0};

  // Check bank type
  DecoderAssert(bank.type() == LHCb::RawBank::ODIN, "Wrong ODIN raw bank type");
  DecoderAssert(bank.magic() == LHCb::RawBank::MagicPattern, "Magic pattern mismatch in ODIN raw bank");

  // Validate bank version
  const unsigned int version = bank.version();
  if ( UNLIKELY( (version > Bank::VERSION) && ! ignoreBankVersion ) ) {
    DecoderAssert( false,
      "Unknown ODIN bank version " + std::to_string(version) + ", latest known is " + std::to_string(Bank::VERSION) );
  }

  // Validate bank size
  const size_t size = bank.size();
  if ( UNLIKELY( ((version >= 3) && size != Bank::SIZE) ||
                 ((version < 3) && size < bank_size_v2) ) ) { // relaxed check to take into account a bug in BooleInit
    DecoderAssert( false,
                   "Wrong ODIN bank size " + std::to_string(size) + ", expected " +
                   std::to_string((version >= 3) ? Bank::SIZE : bank_size_v2) );
  }

  // pointer to the data
  const unsigned int* odinData = bank.data();

  // Fill the ODIN object
  odin.setVersion(version);
  odin.setRunNumber(odinData[LHCb::ODIN::RunNumber]);
  odin.setOrbitNumber(odinData[LHCb::ODIN::OrbitNumber]);

  temp64 = odinData[LHCb::ODIN::L0EventIDHi];
  odin.setEventNumber((temp64 << 32) + odinData[LHCb::ODIN::L0EventIDLo]);

  temp64 = odinData[LHCb::ODIN::GPSTimeHi];
  odin.setGpsTime ((temp64 << 32) + odinData[LHCb::ODIN::GPSTimeLo]);

  temp32 = odinData[LHCb::ODIN::Word7];
  odin.setDetectorStatus( (temp32 & LHCb::ODIN::DetectorStatusMask) >> LHCb::ODIN::DetectorStatusBits );
  odin.setErrorBits( (temp32 & LHCb::ODIN::ErrorMask) >> LHCb::ODIN::ErrorBits );

  temp32 = odinData[LHCb::ODIN::Word8];
  odin.setBunchId( (temp32 & LHCb::ODIN::BunchIDMask) >> LHCb::ODIN::BunchIDBits );
  if (version <= 5) {
    switch ((temp32 & LHCb::ODIN::TriggerTypeMask) >> LHCb::ODIN::TriggerTypeBits) {
    case PreV6::Reserve            :
    case PreV6::PhysicsTrigger     : odin.setTriggerType(LHCb::ODIN::PhysicsTrigger);     break;
    case PreV6::AuxilliaryTrigger  : odin.setTriggerType(LHCb::ODIN::AuxiliaryTrigger);   break;
    case PreV6::RandomTrigger      : odin.setTriggerType(LHCb::ODIN::LumiTrigger);        break;
    case PreV6::PeriodicTrigger    : odin.setTriggerType(LHCb::ODIN::TechnicalTrigger);   break;
    case PreV6::NonZSupTrigger     : odin.setTriggerType(LHCb::ODIN::NonZSupTrigger);     break;
    case PreV6::TimingTrigger      : odin.setTriggerType(LHCb::ODIN::TimingTrigger);      break;
    case PreV6::CalibrationTrigger : odin.setTriggerType(LHCb::ODIN::CalibrationTrigger); break;
    default                        : odin.setTriggerType(LHCb::ODIN::PhysicsTrigger);     break;
    }
  } else {
    switch ((temp32 & LHCb::ODIN::TriggerTypeMask) >> LHCb::ODIN::TriggerTypeBits) {
    case 0 : odin.setTriggerType(LHCb::ODIN::PhysicsTrigger);     break;
    case 1 : odin.setTriggerType(LHCb::ODIN::BeamGasTrigger);     break;
    case 2 : odin.setTriggerType(LHCb::ODIN::LumiTrigger);        break;
    case 3 : odin.setTriggerType(LHCb::ODIN::TechnicalTrigger);   break;
    case 4 : odin.setTriggerType(LHCb::ODIN::AuxiliaryTrigger);   break;
    case 5 : odin.setTriggerType(LHCb::ODIN::NonZSupTrigger);     break;
    case 6 : odin.setTriggerType(LHCb::ODIN::TimingTrigger);      break;
    case 7 : odin.setTriggerType(LHCb::ODIN::CalibrationTrigger); break;
    default: odin.setTriggerType(LHCb::ODIN::PhysicsTrigger);     break;
    }
  }

  if (version >= 5) {
    switch ( (temp32 & LHCb::ODIN::CalibrationTypeMask) >> LHCb::ODIN::CalibrationTypeBits ) {
    case 0  : odin.setCalibrationType(LHCb::ODIN::A); break;
    case 1  : odin.setCalibrationType(LHCb::ODIN::B); break;
    case 2  : odin.setCalibrationType(LHCb::ODIN::C); break;
    case 3  : odin.setCalibrationType(LHCb::ODIN::D); break;
    default : odin.setCalibrationType(LHCb::ODIN::A); break;
    }
  } else {
    odin.setReadoutType( ((temp32 & LHCb::ODIN::CalibrationTypeMask) >> LHCb::ODIN::CalibrationTypeBits ) == 0 ?
                          LHCb::ODIN::ZeroSuppressed : LHCb::ODIN::NonZeroSuppressed );
  }

  if (version >= 4) {
    odin.setTimeAlignmentEventWindow( (temp32 & LHCb::ODIN::TAEWindowMask) >> LHCb::ODIN::TAEWindowBits );
  }

  odin.setForceBit( ((temp32 & LHCb::ODIN::ForceMask) >> LHCb::ODIN::ForceBits ) != 0 );

  switch ( (temp32 & LHCb::ODIN::BXTypeMask) >> LHCb::ODIN::BXTypeBits ) {
  case 0  : odin.setBunchCrossingType(LHCb::ODIN::NoBeam); break;
  case 1  : odin.setBunchCrossingType(LHCb::ODIN::Beam1); break;
  case 2  : odin.setBunchCrossingType(LHCb::ODIN::Beam2); break;
  case 3  : odin.setBunchCrossingType(LHCb::ODIN::BeamCrossing); break;
  default : odin.setBunchCrossingType(LHCb::ODIN::NoBeam); break;
  }

  odin.setBunchCurrent( (temp32 & LHCb::ODIN::BunchCurrentMask) >> LHCb::ODIN::BunchCurrentBits );

  if (version < 3) {

    odin.setEventType(odinData[LHCb::ODIN::EventType]);

  } else {

    temp32 = odinData[LHCb::ODIN::EventType];
    odin.setEventType((temp32 & LHCb::ODIN::EventTypeMask) >> LHCb::ODIN::EventTypeBits);
    odin.setCalibrationStep((temp32 & LHCb::ODIN::CalibrationStepMask) >> LHCb::ODIN::CalibrationStepBits);

    odin.setTriggerConfigurationKey(odinData[LHCb::ODIN::TriggerConfigurationKey]);

  }

  return odin;
}


std::array<unsigned int, Bank::SIZE> LHCb::ODINCodec::encode( const LHCb::ODIN& odin ) {
  std::array<unsigned int, Bank::SIZE> data;
  data[LHCb::ODIN::RunNumber] = odin.runNumber();
  data[LHCb::ODIN::EventType] = ((odin.eventType() << LHCb::ODIN::EventTypeBits) & LHCb::ODIN::EventTypeMask) |
    ((odin.calibrationStep() << LHCb::ODIN::CalibrationStepBits) & LHCb::ODIN::CalibrationStepMask);
  data[LHCb::ODIN::OrbitNumber] = odin.orbitNumber();
  data[LHCb::ODIN::L0EventIDHi] = (unsigned int) ((odin.eventNumber() >> 32) & 0xFFFFFFFF );
  data[LHCb::ODIN::L0EventIDLo] = (unsigned int) ((odin.eventNumber()) & 0xFFFFFFFF );
  data[LHCb::ODIN::GPSTimeHi]   = (unsigned int) ((odin.gpsTime() >> 32) & 0xFFFFFFFF );
  data[LHCb::ODIN::GPSTimeLo]   = (unsigned int) ((odin.gpsTime()) & 0xFFFFFFFF );
  data[LHCb::ODIN::Word7] = (unsigned int) ( ((odin.detectorStatus() << LHCb::ODIN::DetectorStatusBits) & LHCb::ODIN::DetectorStatusMask) |
                                             ((odin.errorBits() << LHCb::ODIN::ErrorBits) & LHCb::ODIN::ErrorMask) );
  // This conversion is needed to be able to disentangle the enum internal (C) representation
  // from the hardware ODIN bit codes.
  int triggerType = 0;
  switch (odin.triggerType()) {
  case LHCb::ODIN::PhysicsTrigger     : triggerType = 0; break;
  case LHCb::ODIN::BeamGasTrigger     : triggerType = 1; break;
  case LHCb::ODIN::LumiTrigger        : triggerType = 2; break;
  case LHCb::ODIN::TechnicalTrigger   : triggerType = 3; break;
  case LHCb::ODIN::AuxiliaryTrigger   : triggerType = 4; break;
  case LHCb::ODIN::NonZSupTrigger     : triggerType = 5; break;
  case LHCb::ODIN::TimingTrigger      : triggerType = 6; break;
  case LHCb::ODIN::CalibrationTrigger : triggerType = 7; break;
  }
  data[LHCb::ODIN::Word8] = ((odin.bunchId() << LHCb::ODIN::BunchIDBits) & LHCb::ODIN::BunchIDMask) |
    ((odin.timeAlignmentEventWindow() << LHCb::ODIN::TAEWindowBits) & LHCb::ODIN::TAEWindowMask) |
    ((triggerType << LHCb::ODIN::TriggerTypeBits) & LHCb::ODIN::TriggerTypeMask) |
    ((odin.calibrationType() << LHCb::ODIN::CalibrationTypeBits) & LHCb::ODIN::CalibrationTypeMask) |
    ((odin.forceBit() << LHCb::ODIN::ForceBits) & LHCb::ODIN::ForceMask) |
    ((odin.bunchCrossingType() << LHCb::ODIN::BXTypeBits) & LHCb::ODIN::BXTypeMask) |
    ((odin.bunchCurrent() << LHCb::ODIN::BunchCurrentBits) & LHCb::ODIN::BunchCurrentMask);
  data[LHCb::ODIN::TriggerConfigurationKey] = odin.triggerConfigurationKey();
  return data;
}
