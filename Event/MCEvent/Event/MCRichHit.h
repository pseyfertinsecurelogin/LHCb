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
#include "Event/MCParticle.h"
#include "Event/MCRichDigitHistoryCode.h"
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SmartRef.h"
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichRadiatorType.h"
#include "Kernel/RichSmartID.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MCRichHit = 12413;

  // Namespace for locations in TDS
  namespace MCRichHitLocation {
    inline const std::string Default = "MC/Rich/Hits";
  }

  /** @class MCRichHit MCRichHit.h
   *
   * MC Rich HPD detector hit
   *
   * @author Chris Jones    Christopher.Rob.Jones@cern.ch
   *
   */

  class MCRichHit : public ContainedObject {
  public:
    /// typedef for ObjectVector of MCRichHit
    using Container = ObjectVector<MCRichHit>;

    /// Default Constructor
    MCRichHit() = default;

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// The RICH detector
    Rich::DetectorType rich() const;

    /// set the RICH type
    void setRich( const Rich::DetectorType value );

    /// If Cherenkov radiation, the radiator medium
    Rich::RadiatorType radiator() const;

    /// set the radiator type
    void setRadiator( const Rich::RadiatorType value );

    /// Returns true if RICH information is valid
    bool richInfoValid() const;

    /// Returns true if radiator information is valid
    bool radiatorInfoValid() const;

    /// Returns true if this hit is CK radiation from charged tracks traversing the gas quartz windows
    bool gasQuartzCK() const;

    /// Returns true if this hit is CK radiation from charged tracks traversing the HPD quartz windows
    bool hpdQuartzCK() const;

    /// Returns true if this hit is CK radiation from charged tracks traversing the nitrogen gas
    bool nitrogenCK() const;

    /// Returns true if this hit is CK radiation from charged tracks traversing the aerogel filter
    bool aeroFilterCK() const;

    /// Returns true if this hit is Cherenkov radiation from the main radiators
    bool isRICHRadiator() const;

    /// Returns true if this hit is from an optical reflection within the HPD
    bool hpdReflection() const;

    /// Returns true if this hit is unscattered and unreflected Cherenkov radiation from the main radiators
    bool isSignal() const;

    /// Returns true if this hit is a background hit
    bool isBackground() const;

    /// Returns an MCRichDigitHistoryCode object for this hit
    MCRichDigitHistoryCode mcRichDigitHistoryCode() const;

    /// Print this MCRichDigit in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Entry point to the HPD silicon wafer (mm)
    const Gaudi::XYZPoint& entry() const;

    /// Update  Entry point to the HPD silicon wafer (mm)
    void setEntry( const Gaudi::XYZPoint& value );

    /// Retrieve const  Energy deposited in the HPD silicon wafer (MeV)
    double energy() const;

    /// Update  Energy deposited in the HPD silicon wafer (MeV)
    void setEnergy( double value );

    /// Retrieve const  Time of flight (ns)
    double timeOfFlight() const;

    /// Update  Time of flight (ns)
    void setTimeOfFlight( double value );

    /// Retrieve const  Sensitive detector ID number. Uniquely identifies the hit HPD pixel using the RichSmartID
    /// channel encoding
    const LHCb::RichSmartID& sensDetID() const;

    /// Update  Sensitive detector ID number. Uniquely identifies the hit HPD pixel using the RichSmartID channel
    /// encoding
    void setSensDetID( const LHCb::RichSmartID& value );

    /// Retrieve const  Bit packed field containing RICH specific information
    unsigned int historyCode() const;

    /// Update  Bit packed field containing RICH specific information
    void setHistoryCode( unsigned int value );

    /// Retrieve If an aerogel hit, store the aerogel Full tile ID number. ID is adjusted from the G4 number to run from
    /// zero
    unsigned int aerogelTileID() const;

    /// Update If an aerogel hit, store the aerogel Full tile ID number. ID is adjusted from the G4 number to run from
    /// zero
    void setAerogelTileID( unsigned int value );

    /// Retrieve Did the photon undergo Rayleigh scattering
    bool scatteredPhoton() const;

    /// Update Did the photon undergo Rayleigh scattering
    void setScatteredPhoton( bool value );

    /// Retrieve Hit from a charged track hitting the HPD entrance window
    bool chargedTrack() const;

    /// Update Hit from a charged track hitting the HPD entrance window
    void setChargedTrack( bool value );

    /// Retrieve Hit from an electron backscattered at the HPD silicon chip
    bool hpdSiBackscatter() const;

    /// Update Hit from an electron backscattered at the HPD silicon chip
    void setHpdSiBackscatter( bool value );

    /// Retrieve Hit from a photon reflected by the QW/PC interface in the HPD
    bool hpdReflQWPC() const;

    /// Update Hit from a photon reflected by the QW/PC interface in the HPD
    void setHpdReflQWPC( bool value );

    /// Retrieve Hit from a photon reflected by the Chromium in the HPD
    bool hpdReflChr() const;

    /// Update Hit from a photon reflected by the Chromium in the HPD
    void setHpdReflChr( bool value );

    /// Retrieve Hit from a photon reflected by the Air/QW interface in the HPD
    bool hpdReflAirQW() const;

    /// Update Hit from a photon reflected by the Air/QW interface in the HPD
    void setHpdReflAirQW( bool value );

    /// Retrieve Hit from a photon reflected by the Air/PC interface in the HPD
    bool hpdReflAirPC() const;

    /// Update Hit from a photon reflected by the Air/PC interface in the HPD
    void setHpdReflAirPC( bool value );

    /// Retrieve Hit from a photon reflected by the silicon in the HPD
    bool hpdReflSi() const;

    /// Update Hit from a photon reflected by the silicon in the HPD
    void setHpdReflSi( bool value );

    /// Retrieve Hit from a photon reflected by the kovar in the HPD
    bool hpdReflKovar() const;

    /// Update Hit from a photon reflected by the kovar in the HPD
    void setHpdReflKovar( bool value );

    /// Retrieve Hit from a photon reflected by the kapton in the HPD
    bool hpdReflKapton() const;

    /// Update Hit from a photon reflected by the kapton in the HPD
    void setHpdReflKapton( bool value );

    /// Retrieve Hit from a photon reflected by the PC/QW interface in the HPD
    bool hpdReflPCQW() const;

    /// Update Hit from a photon reflected by the PC/QW interface in the HPD
    void setHpdReflPCQW( bool value );

    /// Retrieve Hit from a photon created via scintillation in the radiator
    bool radScintillation() const;

    /// Update Hit from a photon created via scintillation in the radiator
    void setRadScintillation( bool value );

    /// Retrieve If an Aerogel SubTile hit, store the SubTile ID Number in the corresponding Full  tile
    unsigned int AerogelSubTileID() const;

    /// Update If an Aerogel SubTile hit, store the SubTile ID Number in the corresponding Full  tile
    void setAerogelSubTileID( unsigned int value );

    /// Retrieve If  a Hit has gone through a Lens in front of PMT this bit is set
    bool pmtLensFlag() const;

    /// Update If  a Hit has gone through a Lens in front of PMT this bit is set
    void setPmtLensFlag( bool value );

    /// Retrieve (const)  Pointer to the MCParticle causing the hit (Normally the MCParticle from which the Cherenkov
    /// photon was radiated, but also can be an MCParticle that directly hits the HPD entrance window
    const LHCb::MCParticle* mcParticle() const;

    /// Update  Pointer to the MCParticle causing the hit (Normally the MCParticle from which the Cherenkov photon was
    /// radiated, but also can be an MCParticle that directly hits the HPD entrance window
    void setMCParticle( const SmartRef<LHCb::MCParticle>& value );

    /// Update (pointer)  Pointer to the MCParticle causing the hit (Normally the MCParticle from which the Cherenkov
    /// photon was radiated, but also can be an MCParticle that directly hits the HPD entrance window
    void setMCParticle( const LHCb::MCParticle* value );

    friend std::ostream& operator<<( std::ostream& str, const MCRichHit& obj ) { return obj.fillStream( str ); }

  private:
    /// Offsets of bitfield historyCode
    enum historyCodeBits {
      richBits             = 0,
      radiatorBits         = 2,
      aerogelTileIDBits    = 7,
      scatteredPhotonBits  = 12,
      chargedTrackBits     = 13,
      hpdSiBackscatterBits = 14,
      hpdReflQWPCBits      = 15,
      hpdReflChrBits       = 16,
      hpdReflAirQWBits     = 17,
      hpdReflAirPCBits     = 18,
      hpdReflSiBits        = 19,
      hpdReflKovarBits     = 20,
      hpdReflKaptonBits    = 21,
      hpdReflPCQWBits      = 22,
      radScintillationBits = 23,
      AerogelSubTileIDBits = 24,
      pmtLensFlagBits      = 30
    };

    /// Bitmasks for bitfield historyCode
    enum historyCodeMasks {
      richMask             = 0x3L,
      radiatorMask         = 0x7cL,
      aerogelTileIDMask    = 0xf80L,
      scatteredPhotonMask  = 0x1000L,
      chargedTrackMask     = 0x2000L,
      hpdSiBackscatterMask = 0x4000L,
      hpdReflQWPCMask      = 0x8000L,
      hpdReflChrMask       = 0x10000L,
      hpdReflAirQWMask     = 0x20000L,
      hpdReflAirPCMask     = 0x40000L,
      hpdReflSiMask        = 0x80000L,
      hpdReflKovarMask     = 0x100000L,
      hpdReflKaptonMask    = 0x200000L,
      hpdReflPCQWMask      = 0x400000L,
      radScintillationMask = 0x800000L,
      AerogelSubTileIDMask = 0x3f000000L,
      pmtLensFlagMask      = 0x40000000L
    };

    Gaudi::XYZPoint   m_entry;             ///< Entry point to the HPD silicon wafer (mm)
    double            m_energy{0.0};       ///< Energy deposited in the HPD silicon wafer (MeV)
    double            m_timeOfFlight{0.0}; ///< Time of flight (ns)
    LHCb::RichSmartID m_sensDetID; ///< Sensitive detector ID number. Uniquely identifies the hit HPD pixel using the
                                   ///< RichSmartID channel encoding
    unsigned int               m_historyCode{0}; ///< Bit packed field containing RICH specific information
    SmartRef<LHCb::MCParticle> m_MCParticle;     ///< Pointer to the MCParticle causing the hit (Normally the MCParticle
                                                 ///< from which the Cherenkov photon was radiated, but also can be an
                                                 ///< MCParticle that directly hits the HPD entrance window

  }; // class MCRichHit

  /// Definition of vector container type for MCRichHit
  using MCRichHits = ObjectVector<MCRichHit>;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCRichHit::clID() const { return LHCb::MCRichHit::classID(); }

inline const CLID& LHCb::MCRichHit::classID() { return CLID_MCRichHit; }

inline const Gaudi::XYZPoint& LHCb::MCRichHit::entry() const { return m_entry; }

inline void LHCb::MCRichHit::setEntry( const Gaudi::XYZPoint& value ) { m_entry = value; }

inline double LHCb::MCRichHit::energy() const { return m_energy; }

inline void LHCb::MCRichHit::setEnergy( double value ) { m_energy = value; }

inline double LHCb::MCRichHit::timeOfFlight() const { return m_timeOfFlight; }

inline void LHCb::MCRichHit::setTimeOfFlight( double value ) { m_timeOfFlight = value; }

inline const LHCb::RichSmartID& LHCb::MCRichHit::sensDetID() const { return m_sensDetID; }

inline void LHCb::MCRichHit::setSensDetID( const LHCb::RichSmartID& value ) { m_sensDetID = value; }

inline unsigned int LHCb::MCRichHit::historyCode() const { return m_historyCode; }

inline void LHCb::MCRichHit::setHistoryCode( unsigned int value ) { m_historyCode = value; }

inline unsigned int LHCb::MCRichHit::aerogelTileID() const {
  return (unsigned int)( ( m_historyCode & aerogelTileIDMask ) >> aerogelTileIDBits );
}

inline void LHCb::MCRichHit::setAerogelTileID( unsigned int value ) {
  m_historyCode &= ~aerogelTileIDMask;
  m_historyCode |= ( ( value << aerogelTileIDBits ) & aerogelTileIDMask );
}

inline bool LHCb::MCRichHit::scatteredPhoton() const {
  return 0 != ( ( m_historyCode & scatteredPhotonMask ) >> scatteredPhotonBits );
}

inline void LHCb::MCRichHit::setScatteredPhoton( bool value ) {
  m_historyCode &= ~scatteredPhotonMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << scatteredPhotonBits ) & scatteredPhotonMask );
}

inline bool LHCb::MCRichHit::chargedTrack() const {
  return 0 != ( ( m_historyCode & chargedTrackMask ) >> chargedTrackBits );
}

inline void LHCb::MCRichHit::setChargedTrack( bool value ) {
  m_historyCode &= ~chargedTrackMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << chargedTrackBits ) & chargedTrackMask );
}

inline bool LHCb::MCRichHit::hpdSiBackscatter() const {
  return 0 != ( ( m_historyCode & hpdSiBackscatterMask ) >> hpdSiBackscatterBits );
}

inline void LHCb::MCRichHit::setHpdSiBackscatter( bool value ) {
  m_historyCode &= ~hpdSiBackscatterMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdSiBackscatterBits ) & hpdSiBackscatterMask );
}

inline bool LHCb::MCRichHit::hpdReflQWPC() const {
  return 0 != ( ( m_historyCode & hpdReflQWPCMask ) >> hpdReflQWPCBits );
}

inline void LHCb::MCRichHit::setHpdReflQWPC( bool value ) {
  m_historyCode &= ~hpdReflQWPCMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflQWPCBits ) & hpdReflQWPCMask );
}

inline bool LHCb::MCRichHit::hpdReflChr() const {
  return 0 != ( ( m_historyCode & hpdReflChrMask ) >> hpdReflChrBits );
}

inline void LHCb::MCRichHit::setHpdReflChr( bool value ) {
  m_historyCode &= ~hpdReflChrMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflChrBits ) & hpdReflChrMask );
}

inline bool LHCb::MCRichHit::hpdReflAirQW() const {
  return 0 != ( ( m_historyCode & hpdReflAirQWMask ) >> hpdReflAirQWBits );
}

inline void LHCb::MCRichHit::setHpdReflAirQW( bool value ) {
  m_historyCode &= ~hpdReflAirQWMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflAirQWBits ) & hpdReflAirQWMask );
}

inline bool LHCb::MCRichHit::hpdReflAirPC() const {
  return 0 != ( ( m_historyCode & hpdReflAirPCMask ) >> hpdReflAirPCBits );
}

inline void LHCb::MCRichHit::setHpdReflAirPC( bool value ) {
  m_historyCode &= ~hpdReflAirPCMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflAirPCBits ) & hpdReflAirPCMask );
}

inline bool LHCb::MCRichHit::hpdReflSi() const { return 0 != ( ( m_historyCode & hpdReflSiMask ) >> hpdReflSiBits ); }

inline void LHCb::MCRichHit::setHpdReflSi( bool value ) {
  m_historyCode &= ~hpdReflSiMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflSiBits ) & hpdReflSiMask );
}

inline bool LHCb::MCRichHit::hpdReflKovar() const {
  return 0 != ( ( m_historyCode & hpdReflKovarMask ) >> hpdReflKovarBits );
}

inline void LHCb::MCRichHit::setHpdReflKovar( bool value ) {
  m_historyCode &= ~hpdReflKovarMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflKovarBits ) & hpdReflKovarMask );
}

inline bool LHCb::MCRichHit::hpdReflKapton() const {
  return 0 != ( ( m_historyCode & hpdReflKaptonMask ) >> hpdReflKaptonBits );
}

inline void LHCb::MCRichHit::setHpdReflKapton( bool value ) {
  m_historyCode &= ~hpdReflKaptonMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflKaptonBits ) & hpdReflKaptonMask );
}

inline bool LHCb::MCRichHit::hpdReflPCQW() const {
  return 0 != ( ( m_historyCode & hpdReflPCQWMask ) >> hpdReflPCQWBits );
}

inline void LHCb::MCRichHit::setHpdReflPCQW( bool value ) {
  m_historyCode &= ~hpdReflPCQWMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflPCQWBits ) & hpdReflPCQWMask );
}

inline bool LHCb::MCRichHit::radScintillation() const {
  return 0 != ( ( m_historyCode & radScintillationMask ) >> radScintillationBits );
}

inline void LHCb::MCRichHit::setRadScintillation( bool value ) {
  m_historyCode &= ~radScintillationMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << radScintillationBits ) & radScintillationMask );
}

inline unsigned int LHCb::MCRichHit::AerogelSubTileID() const {
  return (unsigned int)( ( m_historyCode & AerogelSubTileIDMask ) >> AerogelSubTileIDBits );
}

inline void LHCb::MCRichHit::setAerogelSubTileID( unsigned int value ) {
  m_historyCode &= ~AerogelSubTileIDMask;
  m_historyCode |= ( ( value << AerogelSubTileIDBits ) & AerogelSubTileIDMask );
}

inline bool LHCb::MCRichHit::pmtLensFlag() const {
  return 0 != ( ( m_historyCode & pmtLensFlagMask ) >> pmtLensFlagBits );
}

inline void LHCb::MCRichHit::setPmtLensFlag( bool value ) {
  m_historyCode &= ~pmtLensFlagMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << pmtLensFlagBits ) & pmtLensFlagMask );
}

inline const LHCb::MCParticle* LHCb::MCRichHit::mcParticle() const { return m_MCParticle; }

inline void LHCb::MCRichHit::setMCParticle( const SmartRef<LHCb::MCParticle>& value ) { m_MCParticle = value; }

inline void LHCb::MCRichHit::setMCParticle( const LHCb::MCParticle* value ) { m_MCParticle = value; }

inline Rich::DetectorType LHCb::MCRichHit::rich() const {

  // Shift by -1 to convert packed representation to Rich::DetectorType
  return ( Rich::DetectorType )( ( ( m_historyCode & richMask ) >> richBits ) - 1 );
}

inline void LHCb::MCRichHit::setRich( const Rich::DetectorType value ) {

  // Shift bit-packed representation by +1 to start numbering from 0
  const unsigned int val = (unsigned int)value + 1;
  m_historyCode &= ~richMask;
  m_historyCode |= ( ( ( (unsigned int)val ) << richBits ) & richMask );
}

inline Rich::RadiatorType LHCb::MCRichHit::radiator() const {

  // Shift by -1 to convert packed representation to Rich::RadiatorType
  return ( Rich::RadiatorType )( ( ( m_historyCode & radiatorMask ) >> radiatorBits ) - 1 );
}

inline void LHCb::MCRichHit::setRadiator( const Rich::RadiatorType value ) {

  // Shift bit-packed representation by +1 to start numbering from 0
  const unsigned int val = (unsigned int)value + 1;
  m_historyCode &= ~radiatorMask;
  m_historyCode |= ( ( ( (unsigned int)val ) << radiatorBits ) & radiatorMask );
}

inline bool LHCb::MCRichHit::richInfoValid() const { return rich() != Rich::InvalidDetector; }

inline bool LHCb::MCRichHit::radiatorInfoValid() const { return radiator() != Rich::InvalidRadiator; }

inline bool LHCb::MCRichHit::gasQuartzCK() const { return radiator() == Rich::GasQuartzWin; }

inline bool LHCb::MCRichHit::hpdQuartzCK() const { return radiator() == Rich::HPDQuartzWin; }

inline bool LHCb::MCRichHit::nitrogenCK() const { return radiator() == Rich::Nitrogen; }

inline bool LHCb::MCRichHit::aeroFilterCK() const { return radiator() == Rich::AerogelFilter; }

inline bool LHCb::MCRichHit::isRICHRadiator() const {

  return ( radiator() == Rich::Aerogel || radiator() == Rich::C4F10 || radiator() == Rich::CF4 );
}

inline bool LHCb::MCRichHit::hpdReflection() const {

  return ( hpdReflQWPC() || hpdReflChr() || hpdReflAirQW() || hpdReflAirPC() || hpdReflSi() || hpdReflKovar() ||
           hpdReflKapton() || hpdReflPCQW() );
}

inline bool LHCb::MCRichHit::isSignal() const { return ( !isBackground() ); }

inline bool LHCb::MCRichHit::isBackground() const {

  return ( !isRICHRadiator() ||
           ( scatteredPhoton() || hpdReflection() || hpdSiBackscatter() || chargedTrack() || gasQuartzCK() ||
             hpdQuartzCK() || nitrogenCK() || aeroFilterCK() || radScintillation() ) );
}
