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
#include "Event/RawBank.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/Time.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_ODIN = 1005;

  // Namespace for locations in TDS
  namespace ODINLocation {
    inline const std::string Default = "DAQ/ODIN";
  }

  /** @class ODIN ODIN.h
   *
   * Class for the decoding of the ODIN RawBank.
   *
   * @author Marco Clemencic
   *
   */

  class ODIN final : public DataObject {
  public:
    /// Fields in the ODIN bank
    enum Data {
      RunNumber = 0,
      EventType,
      OrbitNumber,
      L0EventIDHi,
      L0EventIDLo,
      GPSTimeHi,
      GPSTimeLo,
      Word7,
      Word8,
      TriggerConfigurationKey
    };
    ///
    enum EventTypeBitsEnum { EventTypeBits = 0, CalibrationStepBits = 16 };
    ///
    enum EventTypeMasks { EventTypeMask = 0x0000FFFF, CalibrationStepMask = 0xFFFF0000, FlaggingModeMask = 0x00008000 };
    /// Meaning of the EventType bits
    enum EventTypes {
      VeloOpen            = 0x0001,
      Physics             = 0x0002,
      NoBias              = 0x0004,
      Lumi                = 0x0008,
      Beam1Gas            = 0x0010,
      Beam2Gas            = 0x0020,
      et_bit_06           = 0x0040,
      et_bit_07           = 0x0080,
      et_bit_08           = 0x0100,
      et_bit_09           = 0x0200,
      TriggerMaskPhysics  = 0x0400,
      TriggerMaskNoBias   = 0x0800,
      TriggerMaskBeam1Gas = 0x1000,
      TriggerMaskBeam2Gas = 0x2000,
      SequencerTrigger    = 0x4000,
      HltFlaggingMode     = 0x8000
    };
    ///
    enum Word7Bits { DetectorStatusBits = 0, ErrorBits = 24 };
    ///
    enum ErrorCodeMasks { SynchError = 0x1, SynchErrorForced = 0x2 };
    ///
    enum Word7Masks { DetectorStatusMask = 0x00FFFFFF, ErrorMask = 0xFF000000 };
    ///
    enum Word8Bits {
      BunchIDBits         = 0,
      TAEWindowBits       = 12,
      TriggerTypeBits     = 16,
      CalibrationTypeBits = 19,
      ForceBits           = 21,
      BXTypeBits          = 22,
      BunchCurrentBits    = 24
    };
    ///
    enum Word8Masks {
      BunchIDMask         = 0x00000FFF,
      TAEWindowMask       = 0x00007000,
      TriggerTypeMask     = 0x00070000,
      CalibrationTypeMask = 0x00180000,
      ForceMask           = 0x00200000,
      BXTypeMask          = 0x00C00000,
      BunchCurrentMask    = 0xFF000000
    };
    /// Provided for backward compatibility
    enum Word8Bits_v4 { ReadoutTypeBits = 19 };
    /// Provided for backward compatibility
    enum Word8Masks_v4 { ReadoutTypeMask = 0x00180000 };
    ///
    enum ReadoutTypes { ZeroSuppressed = 0, NonZeroSuppressed = 1 };
    ///
    enum CalibrationTypes { A = 0, B = 1, C = 2, D = 3 };
    ///
    enum BXTypes { NoBeam = 0, Beam1 = 1, Beam2 = 2, BeamCrossing = 3 };
    /// Type of trigger broadcasted by ODIN
    enum TriggerType {
      PhysicsTrigger,
      BeamGasTrigger,
      LumiTrigger,
      TechnicalTrigger,
      AuxiliaryTrigger,
      NonZSupTrigger,
      TimingTrigger,
      CalibrationTrigger
    };

    /// Default Constructor
    ODIN()
        : m_runNumber( 0 )
        , m_eventType( 0 )
        , m_orbitNumber( 0 )
        , m_eventNumber( 0 )
        , m_gpsTime( 0 )
        , m_detectorStatus( 0 )
        , m_errorBits( 0 )
        , m_bunchId( 0 )
        , m_triggerType( TriggerType::PhysicsTrigger )
        , m_readoutType( ReadoutTypes::ZeroSuppressed )
        , m_forceBit( false )
        , m_bunchCrossingType( BXTypes::NoBeam )
        , m_bunchCurrent( 0 )
        , m_version( 0 )
        , m_calibrationStep( 0 )
        , m_triggerConfigurationKey( 0 )
        , m_timeAlignmentEventWindow( 0 )
        , m_calibrationType( CalibrationTypes::A ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Return the event time (converted from the GPS time)
    Gaudi::Time eventTime() const;

    /// Set the event time (converted to GPS time)
    void setEventTime( const Gaudi::Time& time );

    ///
    bool isFlagging() const;

    /// Retrieve const  Run number
    unsigned int runNumber() const;

    /// Update  Run number
    void setRunNumber( unsigned int value );

    /// Retrieve const  Event type
    unsigned int eventType() const;

    /// Update  Event type
    void setEventType( unsigned int value );

    /// Retrieve const  Orbit ID
    unsigned int orbitNumber() const;

    /// Update  Orbit ID
    void setOrbitNumber( unsigned int value );

    /// Retrieve const  L0 Event ID
    unsigned long long eventNumber() const;

    /// Update  L0 Event ID
    void setEventNumber( unsigned long long value );

    /// Retrieve const  GPS Time (microseconds)
    unsigned long long gpsTime() const;

    /// Update  GPS Time (microseconds)
    void setGpsTime( unsigned long long value );

    /// Retrieve const  Detector Status
    unsigned long long detectorStatus() const;

    /// Update  Detector Status
    void setDetectorStatus( unsigned long long value );

    /// Retrieve const  Error Bits
    unsigned int errorBits() const;

    /// Update  Error Bits
    void setErrorBits( unsigned int value );

    /// Retrieve const  Bunch ID
    unsigned int bunchId() const;

    /// Update  Bunch ID
    void setBunchId( unsigned int value );

    /// Retrieve const  Trigger Type @see enum LHCb::ODIN::TriggerType
    const TriggerType& triggerType() const;

    /// Update  Trigger Type @see enum LHCb::ODIN::TriggerType
    void setTriggerType( const TriggerType& value );

    /// Retrieve const  Readout Type (@see enum LHCb::ODIN::ReadoutTypes). Meaningful only if bank version < 5.
    const ReadoutTypes& readoutType() const;

    /// Update  Readout Type (@see enum LHCb::ODIN::ReadoutTypes). Meaningful only if bank version < 5.
    void setReadoutType( const ReadoutTypes& value );

    /// Retrieve const  Force Bit
    bool forceBit() const;

    /// Update  Force Bit
    void setForceBit( bool value );

    /// Retrieve const  Bunch Crossing Type (BXType, @see enum LHCb::ODIN::BXTypes)
    const BXTypes& bunchCrossingType() const;

    /// Update  Bunch Crossing Type (BXType, @see enum LHCb::ODIN::BXTypes)
    void setBunchCrossingType( const BXTypes& value );

    /// Retrieve const  Bunch Current
    unsigned int bunchCurrent() const;

    /// Update  Bunch Current
    void setBunchCurrent( unsigned int value );

    /// Retrieve const  Version of the ODIN bank
    unsigned int version() const;

    /// Update  Version of the ODIN bank
    void setVersion( unsigned int value );

    /// Retrieve const  Calibration Step Number
    unsigned int calibrationStep() const;

    /// Update  Calibration Step Number
    void setCalibrationStep( unsigned int value );

    /// Retrieve const  Requested Trigger Configuration Key. The key actually used is in
    /// LHCb::HltDecReports::configuredTCK
    unsigned int triggerConfigurationKey() const;

    /// Update  Requested Trigger Configuration Key. The key actually used is in LHCb::HltDecReports::configuredTCK
    void setTriggerConfigurationKey( unsigned int value );

    /// Retrieve const  TAE (Time Alignment Event) window size
    unsigned int timeAlignmentEventWindow() const;

    /// Update  TAE (Time Alignment Event) window size
    void setTimeAlignmentEventWindow( unsigned int value );

    /// Retrieve const  Calibration Type (@see enum LHCb::ODIN::CalibrationTypes). Meaningful only if bank version >= 5.
    const CalibrationTypes& calibrationType() const;

    /// Update  Calibration Type (@see enum LHCb::ODIN::CalibrationTypes). Meaningful only if bank version >= 5.
    void setCalibrationType( const CalibrationTypes& value );

    friend std::ostream& operator<<( std::ostream& str, const ODIN& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    unsigned int       m_runNumber;      ///< Run number
    unsigned int       m_eventType;      ///< Event type
    unsigned int       m_orbitNumber;    ///< Orbit ID
    unsigned long long m_eventNumber;    ///< L0 Event ID
    unsigned long long m_gpsTime;        ///< GPS Time (microseconds)
    unsigned long long m_detectorStatus; ///< Detector Status
    unsigned int       m_errorBits;      ///< Error Bits
    unsigned int       m_bunchId;        ///< Bunch ID
    TriggerType        m_triggerType;    ///< Trigger Type @see enum LHCb::ODIN::TriggerType
    ReadoutTypes m_readoutType; ///< Readout Type (@see enum LHCb::ODIN::ReadoutTypes). Meaningful only if bank version
                                ///< < 5.
    bool         m_forceBit;    ///< Force Bit
    BXTypes      m_bunchCrossingType;            ///< Bunch Crossing Type (BXType, @see enum LHCb::ODIN::BXTypes)
    unsigned int m_bunchCurrent;                 ///< Bunch Current
    unsigned int m_version;                      ///< Version of the ODIN bank
    unsigned int m_calibrationStep;              ///< Calibration Step Number
    unsigned int m_triggerConfigurationKey;      ///< Requested Trigger Configuration Key. The key actually used is in
                                                 ///< LHCb::HltDecReports::configuredTCK
    unsigned int     m_timeAlignmentEventWindow; ///< TAE (Time Alignment Event) window size
    CalibrationTypes m_calibrationType; ///< Calibration Type (@see enum LHCb::ODIN::CalibrationTypes). Meaningful only
                                        ///< if bank version >= 5.

  }; // class ODIN

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::Data e ) {
    switch ( e ) {
    case LHCb::ODIN::RunNumber:
      return s << "RunNumber";
    case LHCb::ODIN::EventType:
      return s << "EventType";
    case LHCb::ODIN::OrbitNumber:
      return s << "OrbitNumber";
    case LHCb::ODIN::L0EventIDHi:
      return s << "L0EventIDHi";
    case LHCb::ODIN::L0EventIDLo:
      return s << "L0EventIDLo";
    case LHCb::ODIN::GPSTimeHi:
      return s << "GPSTimeHi";
    case LHCb::ODIN::GPSTimeLo:
      return s << "GPSTimeLo";
    case LHCb::ODIN::Word7:
      return s << "Word7";
    case LHCb::ODIN::Word8:
      return s << "Word8";
    case LHCb::ODIN::TriggerConfigurationKey:
      return s << "TriggerConfigurationKey";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::Data";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::EventTypeBitsEnum e ) {
    switch ( e ) {
    case LHCb::ODIN::EventTypeBits:
      return s << "EventTypeBits";
    case LHCb::ODIN::CalibrationStepBits:
      return s << "CalibrationStepBits";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::EventTypeBitsEnum";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::EventTypeMasks e ) {
    switch ( e ) {
    case LHCb::ODIN::EventTypeMask:
      return s << "EventTypeMask";
    case LHCb::ODIN::CalibrationStepMask:
      return s << "CalibrationStepMask";
    case LHCb::ODIN::FlaggingModeMask:
      return s << "FlaggingModeMask";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::EventTypeMasks";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::EventTypes e ) {
    switch ( e ) {
    case LHCb::ODIN::VeloOpen:
      return s << "VeloOpen";
    case LHCb::ODIN::Physics:
      return s << "Physics";
    case LHCb::ODIN::NoBias:
      return s << "NoBias";
    case LHCb::ODIN::Lumi:
      return s << "Lumi";
    case LHCb::ODIN::Beam1Gas:
      return s << "Beam1Gas";
    case LHCb::ODIN::Beam2Gas:
      return s << "Beam2Gas";
    case LHCb::ODIN::et_bit_06:
      return s << "et_bit_06";
    case LHCb::ODIN::et_bit_07:
      return s << "et_bit_07";
    case LHCb::ODIN::et_bit_08:
      return s << "et_bit_08";
    case LHCb::ODIN::et_bit_09:
      return s << "et_bit_09";
    case LHCb::ODIN::TriggerMaskPhysics:
      return s << "TriggerMaskPhysics";
    case LHCb::ODIN::TriggerMaskNoBias:
      return s << "TriggerMaskNoBias";
    case LHCb::ODIN::TriggerMaskBeam1Gas:
      return s << "TriggerMaskBeam1Gas";
    case LHCb::ODIN::TriggerMaskBeam2Gas:
      return s << "TriggerMaskBeam2Gas";
    case LHCb::ODIN::SequencerTrigger:
      return s << "SequencerTrigger";
    case LHCb::ODIN::HltFlaggingMode:
      return s << "HltFlaggingMode";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::EventTypes";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::Word7Bits e ) {
    switch ( e ) {
    case LHCb::ODIN::DetectorStatusBits:
      return s << "DetectorStatusBits";
    case LHCb::ODIN::ErrorBits:
      return s << "ErrorBits";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::Word7Bits";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::ErrorCodeMasks e ) {
    switch ( e ) {
    case LHCb::ODIN::SynchError:
      return s << "SynchError";
    case LHCb::ODIN::SynchErrorForced:
      return s << "SynchErrorForced";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::ErrorCodeMasks";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::Word7Masks e ) {
    switch ( e ) {
    case LHCb::ODIN::DetectorStatusMask:
      return s << "DetectorStatusMask";
    case LHCb::ODIN::ErrorMask:
      return s << "ErrorMask";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::Word7Masks";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::Word8Bits e ) {
    switch ( e ) {
    case LHCb::ODIN::BunchIDBits:
      return s << "BunchIDBits";
    case LHCb::ODIN::TAEWindowBits:
      return s << "TAEWindowBits";
    case LHCb::ODIN::TriggerTypeBits:
      return s << "TriggerTypeBits";
    case LHCb::ODIN::CalibrationTypeBits:
      return s << "CalibrationTypeBits";
    case LHCb::ODIN::ForceBits:
      return s << "ForceBits";
    case LHCb::ODIN::BXTypeBits:
      return s << "BXTypeBits";
    case LHCb::ODIN::BunchCurrentBits:
      return s << "BunchCurrentBits";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::Word8Bits";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::Word8Masks e ) {
    switch ( e ) {
    case LHCb::ODIN::BunchIDMask:
      return s << "BunchIDMask";
    case LHCb::ODIN::TAEWindowMask:
      return s << "TAEWindowMask";
    case LHCb::ODIN::TriggerTypeMask:
      return s << "TriggerTypeMask";
    case LHCb::ODIN::CalibrationTypeMask:
      return s << "CalibrationTypeMask";
    case LHCb::ODIN::ForceMask:
      return s << "ForceMask";
    case LHCb::ODIN::BXTypeMask:
      return s << "BXTypeMask";
    case LHCb::ODIN::BunchCurrentMask:
      return s << "BunchCurrentMask";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::Word8Masks";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::Word8Bits_v4 e ) {
    switch ( e ) {
    case LHCb::ODIN::ReadoutTypeBits:
      return s << "ReadoutTypeBits";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::Word8Bits_v4";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::Word8Masks_v4 e ) {
    switch ( e ) {
    case LHCb::ODIN::ReadoutTypeMask:
      return s << "ReadoutTypeMask";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::Word8Masks_v4";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::ReadoutTypes e ) {
    switch ( e ) {
    case LHCb::ODIN::ZeroSuppressed:
      return s << "ZeroSuppressed";
    case LHCb::ODIN::NonZeroSuppressed:
      return s << "NonZeroSuppressed";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::ReadoutTypes";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::CalibrationTypes e ) {
    switch ( e ) {
    case LHCb::ODIN::A:
      return s << "A";
    case LHCb::ODIN::B:
      return s << "B";
    case LHCb::ODIN::C:
      return s << "C";
    case LHCb::ODIN::D:
      return s << "D";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::CalibrationTypes";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::BXTypes e ) {
    switch ( e ) {
    case LHCb::ODIN::NoBeam:
      return s << "NoBeam";
    case LHCb::ODIN::Beam1:
      return s << "Beam1";
    case LHCb::ODIN::Beam2:
      return s << "Beam2";
    case LHCb::ODIN::BeamCrossing:
      return s << "BeamCrossing";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::BXTypes";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ODIN::TriggerType e ) {
    switch ( e ) {
    case LHCb::ODIN::PhysicsTrigger:
      return s << "PhysicsTrigger";
    case LHCb::ODIN::BeamGasTrigger:
      return s << "BeamGasTrigger";
    case LHCb::ODIN::LumiTrigger:
      return s << "LumiTrigger";
    case LHCb::ODIN::TechnicalTrigger:
      return s << "TechnicalTrigger";
    case LHCb::ODIN::AuxiliaryTrigger:
      return s << "AuxiliaryTrigger";
    case LHCb::ODIN::NonZSupTrigger:
      return s << "NonZSupTrigger";
    case LHCb::ODIN::TimingTrigger:
      return s << "TimingTrigger";
    case LHCb::ODIN::CalibrationTrigger:
      return s << "CalibrationTrigger";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ODIN::TriggerType";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::ODIN::clID() const { return LHCb::ODIN::classID(); }

inline const CLID& LHCb::ODIN::classID() { return CLID_ODIN; }

inline std::ostream& LHCb::ODIN::fillStream( std::ostream& s ) const {
  char l_forceBit = ( m_forceBit ) ? 'T' : 'F';
  s << "{ "
    << "runNumber :	" << m_runNumber << std::endl
    << "eventType :	" << m_eventType << std::endl
    << "orbitNumber :	" << m_orbitNumber << std::endl
    << "eventNumber :	" << m_eventNumber << std::endl
    << "gpsTime :	" << m_gpsTime << std::endl
    << "detectorStatus :	" << m_detectorStatus << std::endl
    << "errorBits :	" << m_errorBits << std::endl
    << "bunchId :	" << m_bunchId << std::endl
    << "triggerType :	" << m_triggerType << std::endl
    << "readoutType :	" << m_readoutType << std::endl
    << "forceBit :	" << l_forceBit << std::endl
    << "bunchCrossingType :	" << m_bunchCrossingType << std::endl
    << "bunchCurrent :	" << m_bunchCurrent << std::endl
    << "version :	" << m_version << std::endl
    << "calibrationStep :	" << m_calibrationStep << std::endl
    << "triggerConfigurationKey :	" << m_triggerConfigurationKey << std::endl
    << "timeAlignmentEventWindow :	" << m_timeAlignmentEventWindow << std::endl
    << "calibrationType :	" << m_calibrationType << std::endl
    << " }";
  return s;
}

inline unsigned int LHCb::ODIN::runNumber() const { return m_runNumber; }

inline void LHCb::ODIN::setRunNumber( unsigned int value ) { m_runNumber = value; }

inline unsigned int LHCb::ODIN::eventType() const { return m_eventType; }

inline void LHCb::ODIN::setEventType( unsigned int value ) { m_eventType = value; }

inline unsigned int LHCb::ODIN::orbitNumber() const { return m_orbitNumber; }

inline void LHCb::ODIN::setOrbitNumber( unsigned int value ) { m_orbitNumber = value; }

inline unsigned long long LHCb::ODIN::eventNumber() const { return m_eventNumber; }

inline void LHCb::ODIN::setEventNumber( unsigned long long value ) { m_eventNumber = value; }

inline unsigned long long LHCb::ODIN::gpsTime() const { return m_gpsTime; }

inline void LHCb::ODIN::setGpsTime( unsigned long long value ) { m_gpsTime = value; }

inline unsigned long long LHCb::ODIN::detectorStatus() const { return m_detectorStatus; }

inline void LHCb::ODIN::setDetectorStatus( unsigned long long value ) { m_detectorStatus = value; }

inline unsigned int LHCb::ODIN::errorBits() const { return m_errorBits; }

inline void LHCb::ODIN::setErrorBits( unsigned int value ) { m_errorBits = value; }

inline unsigned int LHCb::ODIN::bunchId() const { return m_bunchId; }

inline void LHCb::ODIN::setBunchId( unsigned int value ) { m_bunchId = value; }

inline const LHCb::ODIN::TriggerType& LHCb::ODIN::triggerType() const { return m_triggerType; }

inline void LHCb::ODIN::setTriggerType( const TriggerType& value ) { m_triggerType = value; }

inline const LHCb::ODIN::ReadoutTypes& LHCb::ODIN::readoutType() const { return m_readoutType; }

inline void LHCb::ODIN::setReadoutType( const ReadoutTypes& value ) { m_readoutType = value; }

inline bool LHCb::ODIN::forceBit() const { return m_forceBit; }

inline void LHCb::ODIN::setForceBit( bool value ) { m_forceBit = value; }

inline const LHCb::ODIN::BXTypes& LHCb::ODIN::bunchCrossingType() const { return m_bunchCrossingType; }

inline void LHCb::ODIN::setBunchCrossingType( const BXTypes& value ) { m_bunchCrossingType = value; }

inline unsigned int LHCb::ODIN::bunchCurrent() const { return m_bunchCurrent; }

inline void LHCb::ODIN::setBunchCurrent( unsigned int value ) { m_bunchCurrent = value; }

inline unsigned int LHCb::ODIN::version() const { return m_version; }

inline void LHCb::ODIN::setVersion( unsigned int value ) { m_version = value; }

inline unsigned int LHCb::ODIN::calibrationStep() const { return m_calibrationStep; }

inline void LHCb::ODIN::setCalibrationStep( unsigned int value ) { m_calibrationStep = value; }

inline unsigned int LHCb::ODIN::triggerConfigurationKey() const { return m_triggerConfigurationKey; }

inline void LHCb::ODIN::setTriggerConfigurationKey( unsigned int value ) { m_triggerConfigurationKey = value; }

inline unsigned int LHCb::ODIN::timeAlignmentEventWindow() const { return m_timeAlignmentEventWindow; }

inline void LHCb::ODIN::setTimeAlignmentEventWindow( unsigned int value ) { m_timeAlignmentEventWindow = value; }

inline const LHCb::ODIN::CalibrationTypes& LHCb::ODIN::calibrationType() const { return m_calibrationType; }

inline void LHCb::ODIN::setCalibrationType( const CalibrationTypes& value ) { m_calibrationType = value; }

inline Gaudi::Time LHCb::ODIN::eventTime() const {

  if ( gpsTime() == 0xD0DD0D0000000000ULL )
    return 0;
  else
    return gpsTime() * 1000;
}

inline void LHCb::ODIN::setEventTime( const Gaudi::Time& time ) { setGpsTime( time.ns() / 1000 ); }

inline bool LHCb::ODIN::isFlagging() const { return eventType() & EventTypeMasks::FlaggingModeMask; }
