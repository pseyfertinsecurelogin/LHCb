#include "ODINCodecBaseTool.h"

//=============================================================================
// IMPLEMENTATION of ODINCodecBaseTool
//=============================================================================

#include <sstream>
#include <memory>
#include <algorithm>
// from LHCb
#include "Event/ODIN.h"
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "DAQKernel/DecoderToolBase.h"

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
ODINCodecBaseTool::ODINCodecBaseTool( const std::string& type,
                                      const std::string& name,
                                      const IInterface* parent )
  : base_class( type, name , parent )
{
  declareProperty("Force", m_force = false,
                  "If already present, override the destination object.");
  declareProperty("IgnoreUnknownBankVersion", m_ignoreBankVersion = false,
                  "Do not stop in case of unknown bank version number, assuming"
                  " it is binary compatible with the latest known version.");
}
//=============================================================================
// ODIN Bank constants
//=============================================================================
namespace {
  const unsigned int bank_version = 6;
  const size_t bank_size    = 10 * sizeof(int);
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
//=============================================================================
// Decode
//=============================================================================
LHCb::ODIN* ODINCodecBaseTool::i_decode(const LHCb::RawBank* bank, LHCb::ODIN* odin)
{
  unsigned long long temp64{0};
  unsigned int       temp32{0};

  // Check the passed pointers
  Assert( bank != nullptr, "Called without a RawBank object (pointer NULL)");

  // ensure that the new object is deleted in case of failure
  std::unique_ptr<LHCb::ODIN> ptr;
  if ( !odin )
  {
    odin = new LHCb::ODIN();
    ptr.reset(odin);
  }

  // Check bank type
  Assert(bank->type() == LHCb::RawBank::ODIN, "Wrong ODIN raw bank type");
  Assert(bank->magic() == LHCb::RawBank::MagicPattern, "Magic pattern mismatch in ODIN raw bank");

  // Validate bank version
  const unsigned int version = bank->version();
  if ((version > bank_version) && ! m_ignoreBankVersion) {
    std::ostringstream msg;
    msg << "Unknown ODIN bank version " << version << ", latest known is " << bank_version;
    //Exception(msg.str()); // throw
    Error(msg.str()).ignore();
    return nullptr;
  }
  // Validate bank size
  const size_t size = bank->size();
  if ( ((version >= 3) && size != bank_size) ||
       ((version < 3) && size < bank_size_v2) ) { // relaxed check to take into account a bug in BooleInit
    std::ostringstream msg;
    msg << "Wrong ODIN bank size " << size << ", expected ";
    if (version >= 3) { msg << bank_size; }
    else { msg << bank_size_v2; }
    //Exception(msg.str()); // throw
    Error(msg.str()).ignore();
    return nullptr;
  }

  // pointer to the data
  const unsigned int* odinData = bank->data();

  // Fill the ODIN object
  odin->setVersion(version);
  odin->setRunNumber(odinData[LHCb::ODIN::Data::RunNumber]);
  odin->setOrbitNumber(odinData[LHCb::ODIN::Data::OrbitNumber]);

  temp64 = odinData[LHCb::ODIN::Data::L0EventIDHi];
  odin->setEventNumber((temp64 << 32) + odinData[LHCb::ODIN::Data::L0EventIDLo]);

  temp64 = odinData[LHCb::ODIN::Data::GPSTimeHi];
  odin->setGpsTime ((temp64 << 32) + odinData[LHCb::ODIN::Data::GPSTimeLo]);

  temp32 = odinData[LHCb::ODIN::Data::Word7];
  odin->setDetectorStatus( (temp32 & LHCb::ODIN::DetectorStatusMask) >> LHCb::ODIN::Word7Bits::DetectorStatusBits );
  odin->setErrorBits( (temp32 & LHCb::ODIN::ErrorMask) >> LHCb::ODIN::Word7Bits::ErrorBits );

  temp32 = odinData[LHCb::ODIN::Data::Word8];
  odin->setBunchId( (temp32 & LHCb::ODIN::Word8Masks::BunchIDMask) >> LHCb::ODIN::BunchIDBits );
  if (version <= 5) {
    switch ((temp32 & LHCb::ODIN::Word8Masks::TriggerTypeMask) >> LHCb::ODIN::TriggerTypeBits) {
    case PreV6::Reserve            :
    case PreV6::PhysicsTrigger     : odin->setTriggerType(LHCb::ODIN::TriggerType::PhysicsTrigger);     break;
    case PreV6::AuxilliaryTrigger  : odin->setTriggerType(LHCb::ODIN::TriggerType::AuxiliaryTrigger);   break;
    case PreV6::RandomTrigger      : odin->setTriggerType(LHCb::ODIN::TriggerType::LumiTrigger);        break;
    case PreV6::PeriodicTrigger    : odin->setTriggerType(LHCb::ODIN::TriggerType::TechnicalTrigger);   break;
    case PreV6::NonZSupTrigger     : odin->setTriggerType(LHCb::ODIN::TriggerType::NonZSupTrigger);     break;
    case PreV6::TimingTrigger      : odin->setTriggerType(LHCb::ODIN::TriggerType::TimingTrigger);      break;
    case PreV6::CalibrationTrigger : odin->setTriggerType(LHCb::ODIN::TriggerType::CalibrationTrigger); break;
    default                        : odin->setTriggerType(LHCb::ODIN::TriggerType::PhysicsTrigger);     break;
    }
  } else {
    switch ((temp32 & LHCb::ODIN::Word8Masks::TriggerTypeMask) >> LHCb::ODIN::TriggerTypeBits) {
    case 0 : odin->setTriggerType(LHCb::ODIN::TriggerType::PhysicsTrigger);     break;
    case 1 : odin->setTriggerType(LHCb::ODIN::TriggerType::BeamGasTrigger);     break;
    case 2 : odin->setTriggerType(LHCb::ODIN::TriggerType::LumiTrigger);        break;
    case 3 : odin->setTriggerType(LHCb::ODIN::TriggerType::TechnicalTrigger);   break;
    case 4 : odin->setTriggerType(LHCb::ODIN::TriggerType::AuxiliaryTrigger);   break;
    case 5 : odin->setTriggerType(LHCb::ODIN::TriggerType::NonZSupTrigger);     break;
    case 6 : odin->setTriggerType(LHCb::ODIN::TriggerType::TimingTrigger);      break;
    case 7 : odin->setTriggerType(LHCb::ODIN::TriggerType::CalibrationTrigger); break;
    default: odin->setTriggerType(LHCb::ODIN::TriggerType::PhysicsTrigger);     break;
    }
  }

  if (version >= 5) {
    switch ( (temp32 & LHCb::ODIN::Word8Masks::CalibrationTypeMask) >> LHCb::ODIN::CalibrationTypeBits ) {
    case 0  : odin->setCalibrationType(LHCb::ODIN::CalibrationTypes::A); break;
    case 1  : odin->setCalibrationType(LHCb::ODIN::CalibrationTypes::B); break;
    case 2  : odin->setCalibrationType(LHCb::ODIN::CalibrationTypes::C); break;
    case 3  : odin->setCalibrationType(LHCb::ODIN::CalibrationTypes::D); break;
    default : odin->setCalibrationType(LHCb::ODIN::CalibrationTypes::A); break;
    }
  } else {
    odin->setReadoutType( ((temp32 & LHCb::ODIN::Word8Masks::CalibrationTypeMask) >> LHCb::ODIN::CalibrationTypeBits ) == 0 ?
                          LHCb::ODIN::ReadoutTypes::ZeroSuppressed : LHCb::ODIN::NonZeroSuppressed );
  }

  if (version >= 4) {
    odin->setTimeAlignmentEventWindow( (temp32 & LHCb::ODIN::Word8Masks::TAEWindowMask) >> LHCb::ODIN::TAEWindowBits );
  }

  odin->setForceBit( ((temp32 & LHCb::ODIN::Word8Masks::ForceMask) >> LHCb::ODIN::ForceBits ) != 0 );

  switch ( (temp32 & LHCb::ODIN::Word8Masks::BXTypeMask) >> LHCb::ODIN::BXTypeBits ) {
  case 0  : odin->setBunchCrossingType(LHCb::ODIN::BXTypes::NoBeam); break;
  case 1  : odin->setBunchCrossingType(LHCb::ODIN::BXTypes::Beam1); break;
  case 2  : odin->setBunchCrossingType(LHCb::ODIN::BXTypes::Beam2); break;
  case 3  : odin->setBunchCrossingType(LHCb::ODIN::BXTypes::BeamCrossing); break;
  default : odin->setBunchCrossingType(LHCb::ODIN::BXTypes::NoBeam); break;
  }

  odin->setBunchCurrent( (temp32 & LHCb::ODIN::Word8Masks::BunchCurrentMask) >> LHCb::ODIN::BunchCurrentBits );

  if (version < 3) {

    odin->setEventType(odinData[LHCb::ODIN::Data::EventType]);

  } else {

    temp32 = odinData[LHCb::ODIN::Data::EventType];
    odin->setEventType((temp32 & LHCb::ODIN::EventTypeMasks::EventTypeMask) >> LHCb::ODIN::EventTypeBits);
    odin->setCalibrationStep((temp32 & LHCb::ODIN::EventTypeMasks::CalibrationStepMask) >> LHCb::ODIN::CalibrationStepBits);

    odin->setTriggerConfigurationKey(odinData[LHCb::ODIN::Data::TriggerConfigurationKey]);

  }

  ptr.release(); // pass the ownership to the caller
  return odin;
}
//=============================================================================
// Encode
//=============================================================================
LHCb::RawBank* ODINCodecBaseTool::i_encode(const LHCb::ODIN *odin) {
  // Check the passed pointer
  Assert((odin!=NULL), "Called without an ODIN object (pointer NULL)");

  const unsigned int version = odin->version();
  if (version && (version != bank_version)) {
    std::ostringstream msg;
    msg << "Trying to convert ODIN object of version " << version
        << " to bank version " << bank_version;
    Warning(msg.str()).ignore();
  }
  // Create the new bank
  // Note that we cannot delete it, so better that there is no failure after
  // this line.
  LHCb::RawBank* bank = LHCb::RawEvent::createBank(0,LHCb::RawBank::ODIN, bank_version, bank_size);
  unsigned int *data = bank->data();

  data[LHCb::ODIN::Data::RunNumber] = odin->runNumber();

  data[LHCb::ODIN::EventType] = ((odin->eventType() << LHCb::ODIN::EventTypeBits) & LHCb::ODIN::EventTypeMasks::EventTypeMask) |
    ((odin->calibrationStep() << LHCb::ODIN::CalibrationStepBits) & LHCb::ODIN::EventTypeMasks::CalibrationStepMask);

  data[LHCb::ODIN::Data::OrbitNumber] = odin->orbitNumber();
  data[LHCb::ODIN::Data::L0EventIDHi] = (unsigned int) ((odin->eventNumber() >> 32) & 0xFFFFFFFF );
  data[LHCb::ODIN::Data::L0EventIDLo] = (unsigned int) ((odin->eventNumber()) & 0xFFFFFFFF );
  data[LHCb::ODIN::Data::GPSTimeHi]   = (unsigned int) ((odin->gpsTime() >> 32) & 0xFFFFFFFF );
  data[LHCb::ODIN::Data::GPSTimeLo]   = (unsigned int) ((odin->gpsTime()) & 0xFFFFFFFF );

  data[LHCb::ODIN::Word7] = (unsigned int) ( ((odin->detectorStatus() << LHCb::ODIN::Word7Bits::DetectorStatusBits) & LHCb::ODIN::DetectorStatusMask) |
                                             ((odin->errorBits() << LHCb::ODIN::Word7Bits::ErrorBits) & LHCb::ODIN::ErrorMask) );

  // This conversion is needed to be able to disentangle the enum internal (C) representation
  // from the hardware ODIN bit codes.
  int triggerType = 0;
  switch (odin->triggerType()) {
  case LHCb::ODIN::TriggerType::PhysicsTrigger     : triggerType = 0; break;
  case LHCb::ODIN::TriggerType::BeamGasTrigger     : triggerType = 1; break;
  case LHCb::ODIN::TriggerType::LumiTrigger        : triggerType = 2; break;
  case LHCb::ODIN::TriggerType::TechnicalTrigger   : triggerType = 3; break;
  case LHCb::ODIN::TriggerType::AuxiliaryTrigger   : triggerType = 4; break;
  case LHCb::ODIN::TriggerType::NonZSupTrigger     : triggerType = 5; break;
  case LHCb::ODIN::TriggerType::TimingTrigger      : triggerType = 6; break;
  case LHCb::ODIN::TriggerType::CalibrationTrigger : triggerType = 7; break;
  }

  data[LHCb::ODIN::Word8] = ((odin->bunchId() << LHCb::ODIN::BunchIDBits)  & LHCb::ODIN::Word8Masks::BunchIDMask) |
       ((odin->timeAlignmentEventWindow() << LHCb::ODIN::TAEWindowBits)    & LHCb::ODIN::Word8Masks::TAEWindowMask) |
       ((triggerType                      << LHCb::ODIN::TriggerTypeBits)  & LHCb::ODIN::Word8Masks::TriggerTypeMask) |
       ((odin->calibrationType()          << LHCb::ODIN::CalibrationTypeBits) & LHCb::ODIN::Word8Masks::CalibrationTypeMask) |
       ((odin->forceBit()                 << LHCb::ODIN::ForceBits)        & LHCb::ODIN::Word8Masks::ForceMask) |
       ((odin->bunchCrossingType()        << LHCb::ODIN::BXTypeBits)       & LHCb::ODIN::Word8Masks::BXTypeMask) |
       ((odin->bunchCurrent()             << LHCb::ODIN::BunchCurrentBits) & LHCb::ODIN::Word8Masks::BunchCurrentMask);

  data[LHCb::ODIN::Data::TriggerConfigurationKey] = odin->triggerConfigurationKey();

  return bank; // pass the ownership to the caller
}
//=============================================================================

