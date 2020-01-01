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

  /** @class MCRichDigitHistoryCode MCRichDigitHistoryCode.h
   *
   * Bit-packed history code object for RICH data objects
   *
   * @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *
   */

  class MCRichDigitHistoryCode final {
  public:
    /// Constructor from bit-packed integer
    explicit MCRichDigitHistoryCode( const unsigned int code ) : m_historyCode( code ) {}

    /// Default Constructor
    MCRichDigitHistoryCode() = default;

    /// set the flags according the the given history code
    void setFlags( const LHCb::MCRichDigitHistoryCode history );

    /// Returns true if one of the contributing hits is Cherenkov radiation from the main radiators
    [[nodiscard]] bool isRICHRadiator() const;

    /// Returns true if one of the contributing hits is from an optical reflection within the HPD
    [[nodiscard]] bool hpdReflection() const;

    /// Returns true if at least one of the hits contributing to this digit came from a background source
    [[nodiscard]] bool hasBackground() const;

    /// Returns true if this digit is purely due to Cherekov signal from one of the main RICH radiators. I.e. has NO
    /// background contribution.
    [[nodiscard]] bool isSignal() const;

    /// Returns true if this digit is only due to background. I.e. none of the contributing hits were signal hits
    [[nodiscard]] bool isBackground() const;

    /// Print this MCRichDigitHistoryCode in a human readable way
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  Bit-packed history information
    [[nodiscard]] unsigned int historyCode() const;

    /// Update  Bit-packed history information
    void setHistoryCode( unsigned int value );

    /// Retrieve Digit is associated to a signal event MCRichHit
    [[nodiscard]] bool signalEvent() const;

    /// Update Digit is associated to a signal event MCRichHit
    void setSignalEvent( bool value );

    /// Retrieve Digit is associated to a Prev event MCRichHit
    [[nodiscard]] bool prevEvent() const;

    /// Update Digit is associated to a Prev event MCRichHit
    void setPrevEvent( bool value );

    /// Retrieve Digit is associated to a PrevPrev event MCRichHit
    [[nodiscard]] bool prevPrevEvent() const;

    /// Update Digit is associated to a PrevPrev event MCRichHit
    void setPrevPrevEvent( bool value );

    /// Retrieve Digit is associated to a Next event MCRichHit
    [[nodiscard]] bool nextEvent() const;

    /// Update Digit is associated to a Next event MCRichHit
    void setNextEvent( bool value );

    /// Retrieve Digit is associated to a NextNext event MCRichHit
    [[nodiscard]] bool nextNextEvent() const;

    /// Update Digit is associated to a NextNext event MCRichHit
    void setNextNextEvent( bool value );

    /// Retrieve One of the contributing hits came from aerogel Cherenkov radiation
    [[nodiscard]] bool aerogelHit() const;

    /// Update One of the contributing hits came from aerogel Cherenkov radiation
    void setAerogelHit( bool value );

    /// Retrieve One of the contributing hits came from C4F10 Cherenkov radiation
    [[nodiscard]] bool c4f10Hit() const;

    /// Update One of the contributing hits came from C4F10 Cherenkov radiation
    void setC4f10Hit( bool value );

    /// Retrieve One of the contributing hits came from CF4 Cherenkov radiation
    [[nodiscard]] bool cf4Hit() const;

    /// Update One of the contributing hits came from CF4 Cherenkov radiation
    void setCf4Hit( bool value );

    /// Retrieve One of the contributing hits underwent rayleigh scattering
    [[nodiscard]] bool scatteredHit() const;

    /// Update One of the contributing hits underwent rayleigh scattering
    void setScatteredHit( bool value );

    /// Retrieve One of the contributing hits came from a charged track hitting the HPD window
    [[nodiscard]] bool chargedTrack() const;

    /// Update One of the contributing hits came from a charged track hitting the HPD window
    void setChargedTrack( bool value );

    /// Retrieve One of the contributing hits came from CK radiation from charged tracks traversing the Gas quartz
    /// windows
    [[nodiscard]] bool gasQuartzCK() const;

    /// Update One of the contributing hits came from CK radiation from charged tracks traversing the Gas quartz windows
    void setGasQuartzCK( bool value );

    /// Retrieve One of the contributing hits came from CK radiation from charged tracks traversing the HPD quartz
    /// windows
    [[nodiscard]] bool hpdQuartzCK() const;

    /// Update One of the contributing hits came from CK radiation from charged tracks traversing the HPD quartz windows
    void setHpdQuartzCK( bool value );

    /// Retrieve One of the contributing hits came from CK radiation from charged tracks traversing the nitrogen gas
    [[nodiscard]] bool nitrogenCK() const;

    /// Update One of the contributing hits came from CK radiation from charged tracks traversing the nitrogen gas
    void setNitrogenCK( bool value );

    /// Retrieve One of the contributing hits came from CK radiation from the aerogel filter
    [[nodiscard]] bool aeroFilterCK() const;

    /// Update One of the contributing hits came from CK radiation from the aerogel filter
    void setAeroFilterCK( bool value );

    /// Retrieve One of the contributing hits came from a dark hit (I.e. a red photon)
    [[nodiscard]] bool darkHit() const;

    /// Update One of the contributing hits came from a dark hit (I.e. a red photon)
    void setDarkHit( bool value );

    /// Retrieve This digit was produced due to charge sharing in the HPD silicon pixel chip
    [[nodiscard]] bool chargeShareHit() const;

    /// Update This digit was produced due to charge sharing in the HPD silicon pixel chip
    void setChargeShareHit( bool value );

    /// Retrieve One of the contributing hits was pure Cherenkov (unscattered/reflected etc.) signal from one of the
    /// main RICH radiators
    [[nodiscard]] bool hasSignal() const;

    /// Update One of the contributing hits was pure Cherenkov (unscattered/reflected etc.) signal from one of the main
    /// RICH radiators
    void setHasSignal( bool value );

    /// Retrieve One of the contributing hits came from an electron backscattered at the HPD silicon chip
    [[nodiscard]] bool hpdSiBackscatter() const;

    /// Update One of the contributing hits came from an electron backscattered at the HPD silicon chip
    void setHpdSiBackscatter( bool value );

    /// Retrieve One of the contributing hits came from a photon reflected by the QW/PC interface in the HPD
    [[nodiscard]] bool hpdReflQWPC() const;

    /// Update One of the contributing hits came from a photon reflected by the QW/PC interface in the HPD
    void setHpdReflQWPC( bool value );

    /// Retrieve One of the contributing hits came from a photon reflected by the Chromium in the HPD
    [[nodiscard]] bool hpdReflChr() const;

    /// Update One of the contributing hits came from a photon reflected by the Chromium in the HPD
    void setHpdReflChr( bool value );

    /// Retrieve One of the contributing hits came from a photon reflected by the Air/QW interface in the HPD
    [[nodiscard]] bool hpdReflAirQW() const;

    /// Update One of the contributing hits came from a photon reflected by the Air/QW interface in the HPD
    void setHpdReflAirQW( bool value );

    /// Retrieve One of the contributing hits came from a photon reflected by the Air/PC interface in the HPD
    [[nodiscard]] bool hpdReflAirPC() const;

    /// Update One of the contributing hits came from a photon reflected by the Air/PC interface in the HPD
    void setHpdReflAirPC( bool value );

    /// Retrieve One of the contributing hits came from a photon reflected by the silicon in the HPD
    [[nodiscard]] bool hpdReflSi() const;

    /// Update One of the contributing hits came from a photon reflected by the silicon in the HPD
    void setHpdReflSi( bool value );

    /// Retrieve One of the contributing hits came from a photon reflected by the kovar in the HPD
    [[nodiscard]] bool hpdReflKovar() const;

    /// Update One of the contributing hits came from a photon reflected by the kovar in the HPD
    void setHpdReflKovar( bool value );

    /// Retrieve One of the contributing hits came from a photon reflected by the kapton in the HPD
    [[nodiscard]] bool hpdReflKapton() const;

    /// Update One of the contributing hits came from a photon reflected by the kapton in the HPD
    void setHpdReflKapton( bool value );

    /// Retrieve One of the contributing hits came from a photon reflected by the PC/QW interface in the HPD
    [[nodiscard]] bool hpdReflPCQW() const;

    /// Update One of the contributing hits came from a photon reflected by the PC/QW interface in the HPD
    void setHpdReflPCQW( bool value );

    /// Retrieve One of the contributing hits came from a photon created via scintillation in the radiator
    [[nodiscard]] bool radScintillation() const;

    /// Update One of the contributing hits came from a photon created via scintillation in the radiator
    void setRadScintillation( bool value );

    friend std::ostream& operator<<( std::ostream& str, const MCRichDigitHistoryCode& obj ) {
      return obj.fillStream( str );
    }

  private:
    /// Offsets of bitfield historyCode
    enum historyCodeBits {
      signalEventBits      = 0,
      prevEventBits        = 1,
      prevPrevEventBits    = 2,
      nextEventBits        = 3,
      nextNextEventBits    = 4,
      aerogelHitBits       = 5,
      c4f10HitBits         = 6,
      cf4HitBits           = 7,
      scatteredHitBits     = 8,
      chargedTrackBits     = 9,
      gasQuartzCKBits      = 10,
      hpdQuartzCKBits      = 11,
      nitrogenCKBits       = 12,
      aeroFilterCKBits     = 13,
      darkHitBits          = 14,
      chargeShareHitBits   = 15,
      hasSignalBits        = 16,
      hpdSiBackscatterBits = 17,
      hpdReflQWPCBits      = 18,
      hpdReflChrBits       = 19,
      hpdReflAirQWBits     = 20,
      hpdReflAirPCBits     = 21,
      hpdReflSiBits        = 22,
      hpdReflKovarBits     = 23,
      hpdReflKaptonBits    = 24,
      hpdReflPCQWBits      = 25,
      radScintillationBits = 26
    };

    /// Bitmasks for bitfield historyCode
    enum historyCodeMasks {
      signalEventMask      = 0x1L,
      prevEventMask        = 0x2L,
      prevPrevEventMask    = 0x4L,
      nextEventMask        = 0x8L,
      nextNextEventMask    = 0x10L,
      aerogelHitMask       = 0x20L,
      c4f10HitMask         = 0x40L,
      cf4HitMask           = 0x80L,
      scatteredHitMask     = 0x100L,
      chargedTrackMask     = 0x200L,
      gasQuartzCKMask      = 0x400L,
      hpdQuartzCKMask      = 0x800L,
      nitrogenCKMask       = 0x1000L,
      aeroFilterCKMask     = 0x2000L,
      darkHitMask          = 0x4000L,
      chargeShareHitMask   = 0x8000L,
      hasSignalMask        = 0x10000L,
      hpdSiBackscatterMask = 0x20000L,
      hpdReflQWPCMask      = 0x40000L,
      hpdReflChrMask       = 0x80000L,
      hpdReflAirQWMask     = 0x100000L,
      hpdReflAirPCMask     = 0x200000L,
      hpdReflSiMask        = 0x400000L,
      hpdReflKovarMask     = 0x800000L,
      hpdReflKaptonMask    = 0x1000000L,
      hpdReflPCQWMask      = 0x2000000L,
      radScintillationMask = 0x4000000L
    };

    unsigned int m_historyCode{0}; ///< Bit-packed history information

  }; // class MCRichDigitHistoryCode

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline unsigned int LHCb::MCRichDigitHistoryCode::historyCode() const { return m_historyCode; }

inline void LHCb::MCRichDigitHistoryCode::setHistoryCode( unsigned int value ) { m_historyCode = value; }

inline bool LHCb::MCRichDigitHistoryCode::signalEvent() const {
  return 0 != ( ( m_historyCode & signalEventMask ) >> signalEventBits );
}

inline void LHCb::MCRichDigitHistoryCode::setSignalEvent( bool value ) {
  auto val = (unsigned int)value;
  m_historyCode &= ~signalEventMask;
  m_historyCode |= ( ( ( (unsigned int)val ) << signalEventBits ) & signalEventMask );
}

inline bool LHCb::MCRichDigitHistoryCode::prevEvent() const {
  return 0 != ( ( m_historyCode & prevEventMask ) >> prevEventBits );
}

inline void LHCb::MCRichDigitHistoryCode::setPrevEvent( bool value ) {
  auto val = (unsigned int)value;
  m_historyCode &= ~prevEventMask;
  m_historyCode |= ( ( ( (unsigned int)val ) << prevEventBits ) & prevEventMask );
}

inline bool LHCb::MCRichDigitHistoryCode::prevPrevEvent() const {
  return 0 != ( ( m_historyCode & prevPrevEventMask ) >> prevPrevEventBits );
}

inline void LHCb::MCRichDigitHistoryCode::setPrevPrevEvent( bool value ) {
  auto val = (unsigned int)value;
  m_historyCode &= ~prevPrevEventMask;
  m_historyCode |= ( ( ( (unsigned int)val ) << prevPrevEventBits ) & prevPrevEventMask );
}

inline bool LHCb::MCRichDigitHistoryCode::nextEvent() const {
  return 0 != ( ( m_historyCode & nextEventMask ) >> nextEventBits );
}

inline void LHCb::MCRichDigitHistoryCode::setNextEvent( bool value ) {
  auto val = (unsigned int)value;
  m_historyCode &= ~nextEventMask;
  m_historyCode |= ( ( ( (unsigned int)val ) << nextEventBits ) & nextEventMask );
}

inline bool LHCb::MCRichDigitHistoryCode::nextNextEvent() const {
  return 0 != ( ( m_historyCode & nextNextEventMask ) >> nextNextEventBits );
}

inline void LHCb::MCRichDigitHistoryCode::setNextNextEvent( bool value ) {
  auto val = (unsigned int)value;
  m_historyCode &= ~nextNextEventMask;
  m_historyCode |= ( ( ( (unsigned int)val ) << nextNextEventBits ) & nextNextEventMask );
}

inline bool LHCb::MCRichDigitHistoryCode::aerogelHit() const {
  return 0 != ( ( m_historyCode & aerogelHitMask ) >> aerogelHitBits );
}

inline void LHCb::MCRichDigitHistoryCode::setAerogelHit( bool value ) {
  m_historyCode &= ~aerogelHitMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << aerogelHitBits ) & aerogelHitMask );
}

inline bool LHCb::MCRichDigitHistoryCode::c4f10Hit() const {
  return 0 != ( ( m_historyCode & c4f10HitMask ) >> c4f10HitBits );
}

inline void LHCb::MCRichDigitHistoryCode::setC4f10Hit( bool value ) {
  m_historyCode &= ~c4f10HitMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << c4f10HitBits ) & c4f10HitMask );
}

inline bool LHCb::MCRichDigitHistoryCode::cf4Hit() const {
  return 0 != ( ( m_historyCode & cf4HitMask ) >> cf4HitBits );
}

inline void LHCb::MCRichDigitHistoryCode::setCf4Hit( bool value ) {
  m_historyCode &= ~cf4HitMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << cf4HitBits ) & cf4HitMask );
}

inline bool LHCb::MCRichDigitHistoryCode::scatteredHit() const {
  return 0 != ( ( m_historyCode & scatteredHitMask ) >> scatteredHitBits );
}

inline void LHCb::MCRichDigitHistoryCode::setScatteredHit( bool value ) {
  m_historyCode &= ~scatteredHitMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << scatteredHitBits ) & scatteredHitMask );
}

inline bool LHCb::MCRichDigitHistoryCode::chargedTrack() const {
  return 0 != ( ( m_historyCode & chargedTrackMask ) >> chargedTrackBits );
}

inline void LHCb::MCRichDigitHistoryCode::setChargedTrack( bool value ) {
  m_historyCode &= ~chargedTrackMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << chargedTrackBits ) & chargedTrackMask );
}

inline bool LHCb::MCRichDigitHistoryCode::gasQuartzCK() const {
  return 0 != ( ( m_historyCode & gasQuartzCKMask ) >> gasQuartzCKBits );
}

inline void LHCb::MCRichDigitHistoryCode::setGasQuartzCK( bool value ) {
  m_historyCode &= ~gasQuartzCKMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << gasQuartzCKBits ) & gasQuartzCKMask );
}

inline bool LHCb::MCRichDigitHistoryCode::hpdQuartzCK() const {
  return 0 != ( ( m_historyCode & hpdQuartzCKMask ) >> hpdQuartzCKBits );
}

inline void LHCb::MCRichDigitHistoryCode::setHpdQuartzCK( bool value ) {
  m_historyCode &= ~hpdQuartzCKMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdQuartzCKBits ) & hpdQuartzCKMask );
}

inline bool LHCb::MCRichDigitHistoryCode::nitrogenCK() const {
  return 0 != ( ( m_historyCode & nitrogenCKMask ) >> nitrogenCKBits );
}

inline void LHCb::MCRichDigitHistoryCode::setNitrogenCK( bool value ) {
  m_historyCode &= ~nitrogenCKMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << nitrogenCKBits ) & nitrogenCKMask );
}

inline bool LHCb::MCRichDigitHistoryCode::aeroFilterCK() const {
  return 0 != ( ( m_historyCode & aeroFilterCKMask ) >> aeroFilterCKBits );
}

inline void LHCb::MCRichDigitHistoryCode::setAeroFilterCK( bool value ) {
  m_historyCode &= ~aeroFilterCKMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << aeroFilterCKBits ) & aeroFilterCKMask );
}

inline bool LHCb::MCRichDigitHistoryCode::darkHit() const {
  return 0 != ( ( m_historyCode & darkHitMask ) >> darkHitBits );
}

inline void LHCb::MCRichDigitHistoryCode::setDarkHit( bool value ) {
  m_historyCode &= ~darkHitMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << darkHitBits ) & darkHitMask );
}

inline bool LHCb::MCRichDigitHistoryCode::chargeShareHit() const {
  return 0 != ( ( m_historyCode & chargeShareHitMask ) >> chargeShareHitBits );
}

inline void LHCb::MCRichDigitHistoryCode::setChargeShareHit( bool value ) {
  m_historyCode &= ~chargeShareHitMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << chargeShareHitBits ) & chargeShareHitMask );
}

inline bool LHCb::MCRichDigitHistoryCode::hasSignal() const {
  return 0 != ( ( m_historyCode & hasSignalMask ) >> hasSignalBits );
}

inline void LHCb::MCRichDigitHistoryCode::setHasSignal( bool value ) {
  m_historyCode &= ~hasSignalMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hasSignalBits ) & hasSignalMask );
}

inline bool LHCb::MCRichDigitHistoryCode::hpdSiBackscatter() const {
  return 0 != ( ( m_historyCode & hpdSiBackscatterMask ) >> hpdSiBackscatterBits );
}

inline void LHCb::MCRichDigitHistoryCode::setHpdSiBackscatter( bool value ) {
  m_historyCode &= ~hpdSiBackscatterMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdSiBackscatterBits ) & hpdSiBackscatterMask );
}

inline bool LHCb::MCRichDigitHistoryCode::hpdReflQWPC() const {
  return 0 != ( ( m_historyCode & hpdReflQWPCMask ) >> hpdReflQWPCBits );
}

inline void LHCb::MCRichDigitHistoryCode::setHpdReflQWPC( bool value ) {
  m_historyCode &= ~hpdReflQWPCMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflQWPCBits ) & hpdReflQWPCMask );
}

inline bool LHCb::MCRichDigitHistoryCode::hpdReflChr() const {
  return 0 != ( ( m_historyCode & hpdReflChrMask ) >> hpdReflChrBits );
}

inline void LHCb::MCRichDigitHistoryCode::setHpdReflChr( bool value ) {
  m_historyCode &= ~hpdReflChrMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflChrBits ) & hpdReflChrMask );
}

inline bool LHCb::MCRichDigitHistoryCode::hpdReflAirQW() const {
  return 0 != ( ( m_historyCode & hpdReflAirQWMask ) >> hpdReflAirQWBits );
}

inline void LHCb::MCRichDigitHistoryCode::setHpdReflAirQW( bool value ) {
  m_historyCode &= ~hpdReflAirQWMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflAirQWBits ) & hpdReflAirQWMask );
}

inline bool LHCb::MCRichDigitHistoryCode::hpdReflAirPC() const {
  return 0 != ( ( m_historyCode & hpdReflAirPCMask ) >> hpdReflAirPCBits );
}

inline void LHCb::MCRichDigitHistoryCode::setHpdReflAirPC( bool value ) {
  m_historyCode &= ~hpdReflAirPCMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflAirPCBits ) & hpdReflAirPCMask );
}

inline bool LHCb::MCRichDigitHistoryCode::hpdReflSi() const {
  return 0 != ( ( m_historyCode & hpdReflSiMask ) >> hpdReflSiBits );
}

inline void LHCb::MCRichDigitHistoryCode::setHpdReflSi( bool value ) {
  m_historyCode &= ~hpdReflSiMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflSiBits ) & hpdReflSiMask );
}

inline bool LHCb::MCRichDigitHistoryCode::hpdReflKovar() const {
  return 0 != ( ( m_historyCode & hpdReflKovarMask ) >> hpdReflKovarBits );
}

inline void LHCb::MCRichDigitHistoryCode::setHpdReflKovar( bool value ) {
  m_historyCode &= ~hpdReflKovarMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflKovarBits ) & hpdReflKovarMask );
}

inline bool LHCb::MCRichDigitHistoryCode::hpdReflKapton() const {
  return 0 != ( ( m_historyCode & hpdReflKaptonMask ) >> hpdReflKaptonBits );
}

inline void LHCb::MCRichDigitHistoryCode::setHpdReflKapton( bool value ) {
  m_historyCode &= ~hpdReflKaptonMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflKaptonBits ) & hpdReflKaptonMask );
}

inline bool LHCb::MCRichDigitHistoryCode::hpdReflPCQW() const {
  return 0 != ( ( m_historyCode & hpdReflPCQWMask ) >> hpdReflPCQWBits );
}

inline void LHCb::MCRichDigitHistoryCode::setHpdReflPCQW( bool value ) {
  m_historyCode &= ~hpdReflPCQWMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << hpdReflPCQWBits ) & hpdReflPCQWMask );
}

inline bool LHCb::MCRichDigitHistoryCode::radScintillation() const {
  return 0 != ( ( m_historyCode & radScintillationMask ) >> radScintillationBits );
}

inline void LHCb::MCRichDigitHistoryCode::setRadScintillation( bool value ) {
  m_historyCode &= ~radScintillationMask;
  m_historyCode |= ( ( ( (unsigned int)value ) << radScintillationBits ) & radScintillationMask );
}

inline void LHCb::MCRichDigitHistoryCode::setFlags( const LHCb::MCRichDigitHistoryCode history ) {

  m_historyCode = ( m_historyCode | history.historyCode() );
}

inline bool LHCb::MCRichDigitHistoryCode::isRICHRadiator() const { return ( aerogelHit() || c4f10Hit() || cf4Hit() ); }

inline bool LHCb::MCRichDigitHistoryCode::hpdReflection() const {

  return ( hpdReflQWPC() || hpdReflChr() || hpdReflAirQW() || hpdReflAirPC() || hpdReflSi() || hpdReflKovar() ||
           hpdReflKapton() || hpdReflPCQW() );
}

inline bool LHCb::MCRichDigitHistoryCode::hasBackground() const {

  return ( scatteredHit() || hpdReflection() || hpdSiBackscatter() || chargedTrack() || gasQuartzCK() ||
           hpdQuartzCK() || nitrogenCK() || aeroFilterCK() || chargeShareHit() || darkHit() || radScintillation() );
}

inline bool LHCb::MCRichDigitHistoryCode::isSignal() const { return ( !hasBackground() ); }

inline bool LHCb::MCRichDigitHistoryCode::isBackground() const { return ( !hasSignal() ); }
