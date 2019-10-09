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
#include "Event/Track.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SmartRef.h"
#include "Kernel/RichParticleIDType.h"
#include "Kernel/RichRadiatorType.h"
#include "gsl/gsl_sf_erf.h"
#include <ostream>
#include <utility>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_RichPID = 12402;

  // Namespace for locations in TDS
  namespace RichPIDLocation {
    inline const std::string Default       = "Rec/Rich/PIDs";
    inline const std::string Offline       = "Rec/Rich/PIDs";
    inline const std::string OfflineGlobal = "Rec/Rich/GlobalPIDs";
    inline const std::string OfflineLocal  = "Rec/Rich/LocalPIDs";
    inline const std::string HLT           = "Rec/Rich/HltPIDs";
    inline const std::string HLTGlobal     = "Rec/Rich/HltGlobalPIDs";
    inline const std::string HLTLocal      = "Rec/Rich/HltLocalPIDs";
  } // namespace RichPIDLocation

  /** @class RichPID RichPID.h
   *
   * RICH particle identification information for a given track object
   *
   * @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *
   */

  class RichPID : public KeyedObject<int> {
  public:
    /// typedef for std::vector of RichPID
    typedef std::vector<RichPID*>       Vector;
    typedef std::vector<const RichPID*> ConstVector;

    /// typedef for KeyedContainer of RichPID
    typedef KeyedContainer<RichPID, Containers::HashMap> Container;

    /// Constructor from data fields
    RichPID( const Rich::ParticleIDType pid, const LHCb::Track* track, const std::vector<float>& dllValues );

    /// Copy constructor. Creates a new RichPID object with the same pid information
    RichPID( const LHCb::RichPID& pid )
        : KeyedObject<int>()
        , m_pidResultCode( pid.m_pidResultCode )
        , m_particleLLValues( pid.m_particleLLValues )
        , m_track( pid.m_track ) {}

    /// Default Constructor
    RichPID() : m_pidResultCode( 0 ), m_particleLLValues( Rich::NParticleTypes, 0 ) {}

    /// Default Destructor
    virtual ~RichPID() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Returns the raw gaussian probability value for a given particle species
    float particleRawProb( const Rich::ParticleIDType type ) const;

    /// Returns the normalised gaussian probability value for a given particle species
    float particleNormProb( const Rich::ParticleIDType type ) const;

    /// Returns the delta LL value for a given hypothesis
    float particleDeltaLL( const Rich::ParticleIDType type ) const;

    /// Sets the particle delta LL value for the given hypothesis
    void setParticleDeltaLL( const Rich::ParticleIDType type, const float deltaLL );

    /// Sets the particle delta LL value for the given hypothesis
    void setParticleLLValues( std::vector<float>&& value );

    /// Boolean method to check if the given radiator was used to create this PID result (i.e. if the associated track
    /// was found to traverse the radiator in a manner that would have produced detectable Cherenkov photons
    bool traversedRadiator( const Rich::RadiatorType radiator ) const;

    /// Verify if a given hypothesis was above threshold and the associated track present in any radiator
    bool isAboveThreshold( const Rich::ParticleIDType type ) const;

    /// Set a given hypothesis above threshold and the associated track present in any radiator
    void setAboveThreshold( const Rich::ParticleIDType type, const bool flag );

    /// Returns the signed sigma separation beween 2 particle hypotheses (first relative to last)
    float nSigmaSeparation( const Rich::ParticleIDType firstPart, const Rich::ParticleIDType lastPart ) const;

    /// Returns true if the given mass hypothesis is within the given number of sigmas separation from the best PID type
    bool isConsistentNSigma( const Rich::ParticleIDType type, const float nsigma ) const;

    /// Textual representation of PID type
    std::string pidType() const;

    /// The best Particle ID
    Rich::ParticleIDType bestParticleID() const;

    /// set the best particle ID
    void setBestParticleID( const Rich::ParticleIDType type );

    /// Print this RichPID data object in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Bit-packed information (Best particle ID and History) for the RichPID result
    unsigned int pidResultCode() const;

    /// Update  Bit-packed information (Best particle ID and History) for the RichPID result
    void setPidResultCode( unsigned int value );

    /// Retrieve Information from aerogel was used to form this PID result
    bool usedAerogel() const;

    /// Update Information from aerogel was used to form this PID result
    void setUsedAerogel( bool value );

    /// Retrieve Information from Rich1 gas was used to form this PID result
    bool usedRich1Gas() const;

    /// Update Information from Rich1 gas was used to form this PID result
    void setUsedRich1Gas( bool value );

    /// Retrieve Information from Rich2 gas was used to form this PID result
    bool usedRich2Gas() const;

    /// Update Information from Rich2 gas was used to form this PID result
    void setUsedRich2Gas( bool value );

    /// Retrieve The electron hypothesis is above threshold in at least one active radiator
    bool electronHypoAboveThres() const;

    /// Update The electron hypothesis is above threshold in at least one active radiator
    void setElectronHypoAboveThres( bool value );

    /// Retrieve The muon hypothesis is above threshold in at least one active radiator
    bool muonHypoAboveThres() const;

    /// Update The muon hypothesis is above threshold in at least one active radiator
    void setMuonHypoAboveThres( bool value );

    /// Retrieve The pion hypothesis is above threshold in at least one active radiator
    bool pionHypoAboveThres() const;

    /// Update The pion hypothesis is above threshold in at least one active radiator
    void setPionHypoAboveThres( bool value );

    /// Retrieve The kaon hypothesis is above threshold in at least one active radiator
    bool kaonHypoAboveThres() const;

    /// Update The kaon hypothesis is above threshold in at least one active radiator
    void setKaonHypoAboveThres( bool value );

    /// Retrieve The proton hypothesis is above threshold in at least one active radiator
    bool protonHypoAboveThres() const;

    /// Update The proton hypothesis is above threshold in at least one active radiator
    void setProtonHypoAboveThres( bool value );

    /// Retrieve RICH Offline Global PID result
    bool offlineGlobal() const;

    /// Update RICH Offline Global PID result
    void setOfflineGlobal( bool value );

    /// Retrieve RICH Offline Local PID result
    bool offlineLocal() const;

    /// Update RICH Offline Local PID result
    void setOfflineLocal( bool value );

    /// Retrieve RICH Offline Ring Refit PID result
    bool ringRefit() const;

    /// Update RICH Offline Ring Refit PID result
    void setRingRefit( bool value );

    /// Retrieve RICH HLT Local PID result
    bool hltLocal() const;

    /// Update RICH HLT Local PID result
    void setHltLocal( bool value );

    /// Retrieve RICH HLT Global PID result
    bool hltGlobal() const;

    /// Update RICH HLT Global PID result
    void setHltGlobal( bool value );

    /// Retrieve The deuteron hypothesis is above threshold in at least one active radiator
    bool deuteronHypoAboveThres() const;

    /// Update The deuteron hypothesis is above threshold in at least one active radiator
    void setDeuteronHypoAboveThres( bool value );

    /// Retrieve const  Vector of particle hypothesis log likelihood values
    const std::vector<float>& particleLLValues() const;

    /// Update  Vector of particle hypothesis log likelihood values
    void setParticleLLValues( const std::vector<float>& value );

    /// Retrieve (const)  Associated reconstructed Track
    const LHCb::Track* track() const;

    /// Update  Associated reconstructed Track
    void setTrack( const SmartRef<LHCb::Track>& value );

    /// Update (pointer)  Associated reconstructed Track
    void setTrack( const LHCb::Track* value );

    friend std::ostream& operator<<( std::ostream& str, const RichPID& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Offsets of bitfield pidResultCode
    enum pidResultCodeBits {
      packedBestParticleIDBits   = 0,
      usedAerogelBits            = 4,
      usedRich1GasBits           = 5,
      usedRich2GasBits           = 6,
      electronHypoAboveThresBits = 7,
      muonHypoAboveThresBits     = 8,
      pionHypoAboveThresBits     = 9,
      kaonHypoAboveThresBits     = 10,
      protonHypoAboveThresBits   = 11,
      offlineGlobalBits          = 12,
      offlineLocalBits           = 13,
      ringRefitBits              = 14,
      hltLocalBits               = 15,
      hltGlobalBits              = 16,
      deuteronHypoAboveThresBits = 17
    };

    /// Bitmasks for bitfield pidResultCode
    enum pidResultCodeMasks {
      packedBestParticleIDMask   = 0xfL,
      usedAerogelMask            = 0x10L,
      usedRich1GasMask           = 0x20L,
      usedRich2GasMask           = 0x40L,
      electronHypoAboveThresMask = 0x80L,
      muonHypoAboveThresMask     = 0x100L,
      pionHypoAboveThresMask     = 0x200L,
      kaonHypoAboveThresMask     = 0x400L,
      protonHypoAboveThresMask   = 0x800L,
      offlineGlobalMask          = 0x1000L,
      offlineLocalMask           = 0x2000L,
      ringRefitMask              = 0x4000L,
      hltLocalMask               = 0x8000L,
      hltGlobalMask              = 0x10000L,
      deuteronHypoAboveThresMask = 0x20000L
    };

    unsigned int m_pidResultCode; ///< Bit-packed information (Best particle ID and History) for the RichPID result
    std::vector<float>    m_particleLLValues; ///< Vector of particle hypothesis log likelihood values
    SmartRef<LHCb::Track> m_track;            ///< Associated reconstructed Track

  }; // class RichPID

  /// Definition of Keyed Container for RichPID
  typedef KeyedContainer<RichPID, Containers::HashMap> RichPIDs;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::RichPID::RichPID( const Rich::ParticleIDType pid, const LHCb::Track* track,
                               const std::vector<float>& dllValues )
    : m_pidResultCode( 0 ), m_particleLLValues( dllValues ), m_track( track ) {

  setBestParticleID( pid );
}

inline const CLID& LHCb::RichPID::clID() const { return LHCb::RichPID::classID(); }

inline const CLID& LHCb::RichPID::classID() { return CLID_RichPID; }

inline unsigned int LHCb::RichPID::pidResultCode() const { return m_pidResultCode; }

inline void LHCb::RichPID::setPidResultCode( unsigned int value ) { m_pidResultCode = value; }

inline bool LHCb::RichPID::usedAerogel() const {
  return 0 != ( ( m_pidResultCode & usedAerogelMask ) >> usedAerogelBits );
}

inline void LHCb::RichPID::setUsedAerogel( bool value ) {
  unsigned int val = (unsigned int)value;
  m_pidResultCode &= ~usedAerogelMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << usedAerogelBits ) & usedAerogelMask );
}

inline bool LHCb::RichPID::usedRich1Gas() const {
  return 0 != ( ( m_pidResultCode & usedRich1GasMask ) >> usedRich1GasBits );
}

inline void LHCb::RichPID::setUsedRich1Gas( bool value ) {
  unsigned int val = (unsigned int)value;
  m_pidResultCode &= ~usedRich1GasMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << usedRich1GasBits ) & usedRich1GasMask );
}

inline bool LHCb::RichPID::usedRich2Gas() const {
  return 0 != ( ( m_pidResultCode & usedRich2GasMask ) >> usedRich2GasBits );
}

inline void LHCb::RichPID::setUsedRich2Gas( bool value ) {
  unsigned int val = (unsigned int)value;
  m_pidResultCode &= ~usedRich2GasMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << usedRich2GasBits ) & usedRich2GasMask );
}

inline bool LHCb::RichPID::electronHypoAboveThres() const {
  return 0 != ( ( m_pidResultCode & electronHypoAboveThresMask ) >> electronHypoAboveThresBits );
}

inline void LHCb::RichPID::setElectronHypoAboveThres( bool value ) {
  unsigned int val = (unsigned int)value;
  m_pidResultCode &= ~electronHypoAboveThresMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << electronHypoAboveThresBits ) & electronHypoAboveThresMask );
}

inline bool LHCb::RichPID::muonHypoAboveThres() const {
  return 0 != ( ( m_pidResultCode & muonHypoAboveThresMask ) >> muonHypoAboveThresBits );
}

inline void LHCb::RichPID::setMuonHypoAboveThres( bool value ) {
  unsigned int val = (unsigned int)value;
  m_pidResultCode &= ~muonHypoAboveThresMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << muonHypoAboveThresBits ) & muonHypoAboveThresMask );
}

inline bool LHCb::RichPID::pionHypoAboveThres() const {
  return 0 != ( ( m_pidResultCode & pionHypoAboveThresMask ) >> pionHypoAboveThresBits );
}

inline void LHCb::RichPID::setPionHypoAboveThres( bool value ) {
  unsigned int val = (unsigned int)value;
  m_pidResultCode &= ~pionHypoAboveThresMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << pionHypoAboveThresBits ) & pionHypoAboveThresMask );
}

inline bool LHCb::RichPID::kaonHypoAboveThres() const {
  return 0 != ( ( m_pidResultCode & kaonHypoAboveThresMask ) >> kaonHypoAboveThresBits );
}

inline void LHCb::RichPID::setKaonHypoAboveThres( bool value ) {
  unsigned int val = (unsigned int)value;
  m_pidResultCode &= ~kaonHypoAboveThresMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << kaonHypoAboveThresBits ) & kaonHypoAboveThresMask );
}

inline bool LHCb::RichPID::protonHypoAboveThres() const {
  return 0 != ( ( m_pidResultCode & protonHypoAboveThresMask ) >> protonHypoAboveThresBits );
}

inline void LHCb::RichPID::setProtonHypoAboveThres( bool value ) {
  unsigned int val = (unsigned int)value;
  m_pidResultCode &= ~protonHypoAboveThresMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << protonHypoAboveThresBits ) & protonHypoAboveThresMask );
}

inline bool LHCb::RichPID::offlineGlobal() const {
  return 0 != ( ( m_pidResultCode & offlineGlobalMask ) >> offlineGlobalBits );
}

inline void LHCb::RichPID::setOfflineGlobal( bool value ) {
  unsigned int val = (unsigned int)value;
  m_pidResultCode &= ~offlineGlobalMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << offlineGlobalBits ) & offlineGlobalMask );
}

inline bool LHCb::RichPID::offlineLocal() const {
  return 0 != ( ( m_pidResultCode & offlineLocalMask ) >> offlineLocalBits );
}

inline void LHCb::RichPID::setOfflineLocal( bool value ) {
  unsigned int val = (unsigned int)value;
  m_pidResultCode &= ~offlineLocalMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << offlineLocalBits ) & offlineLocalMask );
}

inline bool LHCb::RichPID::ringRefit() const { return 0 != ( ( m_pidResultCode & ringRefitMask ) >> ringRefitBits ); }

inline void LHCb::RichPID::setRingRefit( bool value ) {
  unsigned int val = (unsigned int)value;
  m_pidResultCode &= ~ringRefitMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << ringRefitBits ) & ringRefitMask );
}

inline bool LHCb::RichPID::hltLocal() const { return 0 != ( ( m_pidResultCode & hltLocalMask ) >> hltLocalBits ); }

inline void LHCb::RichPID::setHltLocal( bool value ) {
  unsigned int val = (unsigned int)value;
  m_pidResultCode &= ~hltLocalMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << hltLocalBits ) & hltLocalMask );
}

inline bool LHCb::RichPID::hltGlobal() const { return 0 != ( ( m_pidResultCode & hltGlobalMask ) >> hltGlobalBits ); }

inline void LHCb::RichPID::setHltGlobal( bool value ) {
  unsigned int val = (unsigned int)value;
  m_pidResultCode &= ~hltGlobalMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << hltGlobalBits ) & hltGlobalMask );
}

inline bool LHCb::RichPID::deuteronHypoAboveThres() const {
  return 0 != ( ( m_pidResultCode & deuteronHypoAboveThresMask ) >> deuteronHypoAboveThresBits );
}

inline void LHCb::RichPID::setDeuteronHypoAboveThres( bool value ) {
  unsigned int val = (unsigned int)value;
  m_pidResultCode &= ~deuteronHypoAboveThresMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << deuteronHypoAboveThresBits ) & deuteronHypoAboveThresMask );
}

inline const std::vector<float>& LHCb::RichPID::particleLLValues() const { return m_particleLLValues; }

inline void LHCb::RichPID::setParticleLLValues( const std::vector<float>& value ) { m_particleLLValues = value; }

inline const LHCb::Track* LHCb::RichPID::track() const { return m_track; }

inline void LHCb::RichPID::setTrack( const SmartRef<LHCb::Track>& value ) { m_track = value; }

inline void LHCb::RichPID::setTrack( const LHCb::Track* value ) { m_track = value; }

inline float LHCb::RichPID::particleRawProb( const Rich::ParticleIDType type ) const {

  const auto dll = particleDeltaLL( type );
  return (float)( dll > 0 ? 1.0 - (float)gsl_sf_erf( std::sqrt( dll ) ) : 1.0 );
}

inline float LHCb::RichPID::particleNormProb( const Rich::ParticleIDType type ) const {

  float norm = 0;
  for ( const auto pid : Rich::Particles() ) { norm += particleRawProb( pid ); }
  return ( norm > 0 ? particleRawProb( type ) / norm : 0 );
}

inline float LHCb::RichPID::particleDeltaLL( const Rich::ParticleIDType type ) const {

  return m_particleLLValues[type];
}

inline void LHCb::RichPID::setParticleDeltaLL( const Rich::ParticleIDType type, const float deltaLL ) {

  m_particleLLValues[type] = deltaLL;
}

inline void LHCb::RichPID::setParticleLLValues( std::vector<float>&& value ) {

  m_particleLLValues = std::forward<std::vector<float>>( value );
}

inline float LHCb::RichPID::nSigmaSeparation( const Rich::ParticleIDType firstPart,
                                              const Rich::ParticleIDType lastPart ) const {

  const auto dLL = m_particleLLValues[lastPart] - m_particleLLValues[firstPart];
  return (float)( std::sqrt( fabs( 2.0f * dLL ) ) * ( dLL > 0 ? 1.0f : -1.0f ) );
}

inline bool LHCb::RichPID::isConsistentNSigma( const Rich::ParticleIDType type, const float nsigma ) const {

  return ( nSigmaSeparation( bestParticleID(), type ) > nsigma );
}

inline Rich::ParticleIDType LHCb::RichPID::bestParticleID() const {

  // Shift by -1 to convert packed representation to Rich::ParticleIDType
  return ( Rich::ParticleIDType )( ( ( m_pidResultCode & packedBestParticleIDMask ) >> packedBestParticleIDBits ) - 1 );
}

inline void LHCb::RichPID::setBestParticleID( const Rich::ParticleIDType type ) {

  // Shift bit-packed representation by +1 to start numbering from 0
  const auto val = (unsigned int)type + 1;
  m_pidResultCode &= ~packedBestParticleIDMask;
  m_pidResultCode |= ( ( ( (unsigned int)val ) << packedBestParticleIDBits ) & packedBestParticleIDMask );
}
