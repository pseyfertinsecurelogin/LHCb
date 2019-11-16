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
#include <cmath>
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
    using Vector      = std::vector<RichPID*>;
    using ConstVector = std::vector<const RichPID*>;

    /// typedef for KeyedContainer of RichPID
    typedef KeyedContainer<RichPID, Containers::HashMap> Container;

    /// Constructor from data fields
    RichPID( Rich::ParticleIDType pid, const LHCb::Track* track, std::vector<float> dllValues );

    /// Copy constructor. Creates a new RichPID object with the same pid information
    RichPID( const LHCb::RichPID& pid )
        : KeyedObject<int>()
        , m_pidResultCode( pid.m_pidResultCode )
        , m_particleLLValues( pid.m_particleLLValues )
        , m_track( pid.m_track ) {}

    /// Default Constructor
    RichPID() : m_particleLLValues( Rich::NParticleTypes, 0 ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override { return LHCb::RichPID::classID(); }
    static const CLID& classID() { return CLID_RichPID; }

    /// Returns the raw gaussian probability value for a given particle species
    float particleRawProb( Rich::ParticleIDType type ) const;

    /// Returns the normalised gaussian probability value for a given particle species
    float particleNormProb( Rich::ParticleIDType type ) const;

    /// Returns the delta LL value for a given hypothesis
    float particleDeltaLL( Rich::ParticleIDType type ) const { return m_particleLLValues[type]; }

    /// Sets the particle delta LL value for the given hypothesis
    void setParticleDeltaLL( Rich::ParticleIDType type, float deltaLL ) { m_particleLLValues[type] = deltaLL; }

    /// Sets the particle delta LL value for the given hypothesis
    void setParticleLLValues( std::vector<float>&& value ) { m_particleLLValues = std::move( value ); }

    /// Boolean method to check if the given radiator was used to create this PID result (i.e. if the associated track
    /// was found to traverse the radiator in a manner that would have produced detectable Cherenkov photons
    bool traversedRadiator( Rich::RadiatorType radiator ) const;

    /// Verify if a given hypothesis was above threshold and the associated track present in any radiator
    bool isAboveThreshold( Rich::ParticleIDType type ) const;

    /// Set a given hypothesis above threshold and the associated track present in any radiator
    void setAboveThreshold( Rich::ParticleIDType type, bool flag );

    /// Returns the signed sigma separation beween 2 particle hypotheses (first relative to last)
    float nSigmaSeparation( Rich::ParticleIDType firstPart, Rich::ParticleIDType lastPart ) const;

    /// Returns true if the given mass hypothesis is within the given number of sigmas separation from the best PID type
    bool isConsistentNSigma( Rich::ParticleIDType type, float nsigma ) const;

    /// Textual representation of PID type
    std::string pidType() const;

    /// The best Particle ID
    Rich::ParticleIDType bestParticleID() const;

    /// set the best particle ID
    void setBestParticleID( Rich::ParticleIDType type );

    /// Print this RichPID data object in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Bit-packed information (Best particle ID and History) for the RichPID result
    unsigned int pidResultCode() const { return m_pidResultCode; }

    /// Update  Bit-packed information (Best particle ID and History) for the RichPID result
    void setPidResultCode( unsigned int value ) { m_pidResultCode = value; }

    /// Retrieve Information from aerogel was used to form this PID result
    bool usedAerogel() const { return test<usedAerogelMask>(); }

    /// Update Information from aerogel was used to form this PID result
    void setUsedAerogel( bool value ) { set<usedAerogelMask>( value ); }

    /// Retrieve Information from Rich1 gas was used to form this PID result
    bool usedRich1Gas() const { return test<usedRich1GasMask>(); }

    /// Update Information from Rich1 gas was used to form this PID result
    void setUsedRich1Gas( bool value ) { set<usedRich1GasMask>( value ); }

    /// Retrieve Information from Rich2 gas was used to form this PID result
    bool usedRich2Gas() const { return test<usedRich2GasMask>(); }

    /// Update Information from Rich2 gas was used to form this PID result
    void setUsedRich2Gas( bool value ) { set<usedRich2GasMask>( value ); }

    /// Retrieve The electron hypothesis is above threshold in at least one active radiator
    bool electronHypoAboveThres() const { return test<electronHypoAboveThresMask>(); }

    /// Update The electron hypothesis is above threshold in at least one active radiator
    void setElectronHypoAboveThres( bool value ) { set<electronHypoAboveThresMask>( value ); }

    /// Retrieve The muon hypothesis is above threshold in at least one active radiator
    bool muonHypoAboveThres() const { return test<muonHypoAboveThresMask>(); }

    /// Update The muon hypothesis is above threshold in at least one active radiator
    void setMuonHypoAboveThres( bool value ) { set<muonHypoAboveThresMask>( value ); }

    /// Retrieve The pion hypothesis is above threshold in at least one active radiator
    bool pionHypoAboveThres() const { return test<pionHypoAboveThresMask>(); }

    /// Update The pion hypothesis is above threshold in at least one active radiator
    void setPionHypoAboveThres( bool value ) { set<pionHypoAboveThresMask>( value ); }

    /// Retrieve The kaon hypothesis is above threshold in at least one active radiator
    bool kaonHypoAboveThres() const { return test<kaonHypoAboveThresMask>(); }

    /// Update The kaon hypothesis is above threshold in at least one active radiator
    void setKaonHypoAboveThres( bool value ) { set<kaonHypoAboveThresMask>( value ); }

    /// Retrieve The proton hypothesis is above threshold in at least one active radiator
    bool protonHypoAboveThres() const { return test<protonHypoAboveThresMask>(); }

    /// Update The proton hypothesis is above threshold in at least one active radiator
    void setProtonHypoAboveThres( bool value ) { set<protonHypoAboveThresMask>( value ); }

    /// Retrieve RICH Offline Global PID result
    bool offlineGlobal() const { return test<offlineGlobalMask>(); }

    /// Update RICH Offline Global PID result
    void setOfflineGlobal( bool value ) { return set<offlineGlobalMask>( value ); }

    /// Retrieve RICH Offline Local PID result
    bool offlineLocal() const { return test<offlineLocalMask>(); }

    /// Update RICH Offline Local PID result
    void setOfflineLocal( bool value ) { set<offlineLocalMask>( value ); }

    /// Retrieve RICH Offline Ring Refit PID result
    bool ringRefit() const { return test<ringRefitMask>(); }

    /// Update RICH Offline Ring Refit PID result
    void setRingRefit( bool value ) { set<ringRefitMask>( value ); }

    /// Retrieve RICH HLT Local PID result
    bool hltLocal() const { return test<hltLocalMask>(); }

    /// Update RICH HLT Local PID result
    void setHltLocal( bool value ) { set<hltLocalMask>( value ); }

    /// Retrieve RICH HLT Global PID result
    bool hltGlobal() const { return test<hltGlobalMask>(); }

    /// Update RICH HLT Global PID result
    void setHltGlobal( bool value ) { set<hltGlobalMask>( value ); }

    /// Retrieve The deuteron hypothesis is above threshold in at least one active radiator
    bool deuteronHypoAboveThres() const { return test<deuteronHypoAboveThresMask>(); }

    /// Update The deuteron hypothesis is above threshold in at least one active radiator
    void setDeuteronHypoAboveThres( bool value ) { set<deuteronHypoAboveThresMask>( value ); }

    /// Retrieve const  Vector of particle hypothesis log likelihood values
    const std::vector<float>& particleLLValues() const { return m_particleLLValues; }

    /// Update  Vector of particle hypothesis log likelihood values
    void setParticleLLValues( const std::vector<float>& value ) { m_particleLLValues = value; }

    /// Retrieve (const)  Associated reconstructed Track
    const LHCb::Track* track() const { return m_track; }

    /// Update  Associated reconstructed Track
    void setTrack( const SmartRef<LHCb::Track>& value ) { m_track = value; }

    /// Update (pointer)  Associated reconstructed Track
    void setTrack( const LHCb::Track* value ) { m_track = value; }

    friend std::ostream& operator<<( std::ostream& str, const RichPID& obj ) { return obj.fillStream( str ); }

  private:
    template <unsigned int mask>
    void set( bool value ) {
      m_pidResultCode = ( m_pidResultCode & ~mask ) | ( -value & mask );
    }
    template <unsigned int mask>
    bool test() const {
      return ( m_pidResultCode & mask ) != 0;
    }

    /// Offsets of bitfield pidResultCode
    enum pidResultCodeBits {
      packedBestParticleIDBits = 0,
    };

    /// Bitmasks for bitfield pidResultCode
    enum pidResultCodeMasks : unsigned int {
      packedBestParticleIDMask   = 0xfU,
      usedAerogelMask            = 0x10U,
      usedRich1GasMask           = 0x20U,
      usedRich2GasMask           = 0x40U,
      electronHypoAboveThresMask = 0x80U,
      muonHypoAboveThresMask     = 0x100U,
      pionHypoAboveThresMask     = 0x200U,
      kaonHypoAboveThresMask     = 0x400U,
      protonHypoAboveThresMask   = 0x800U,
      offlineGlobalMask          = 0x1000U,
      offlineLocalMask           = 0x2000U,
      ringRefitMask              = 0x4000U,
      hltLocalMask               = 0x8000U,
      hltGlobalMask              = 0x10000U,
      deuteronHypoAboveThresMask = 0x20000U
    };

    unsigned int m_pidResultCode{0}; ///< Bit-packed information (Best particle ID and History) for the RichPID result
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

inline LHCb::RichPID::RichPID( const Rich::ParticleIDType pid, const LHCb::Track* track, std::vector<float> dllValues )
    : m_pidResultCode( 0 ), m_particleLLValues( std::move( dllValues ) ), m_track( track ) {

  setBestParticleID( pid );
}

inline float LHCb::RichPID::particleRawProb( const Rich::ParticleIDType type ) const {

  const auto dll = particleDeltaLL( type );
  return dll > 0 ? 1.0F - (float)gsl_sf_erf( std::sqrt( dll ) ) : 1.0F;
}

inline float LHCb::RichPID::particleNormProb( const Rich::ParticleIDType type ) const {

  float norm = 0;
  for ( const auto pid : Rich::Particles() ) { norm += particleRawProb( pid ); }
  return ( norm > 0 ? particleRawProb( type ) / norm : 0 );
}

inline float LHCb::RichPID::nSigmaSeparation( const Rich::ParticleIDType firstPart,
                                              const Rich::ParticleIDType lastPart ) const {

  const auto dLL = m_particleLLValues[lastPart] - m_particleLLValues[firstPart];
  return std::sqrt( std::abs( 2.0F * dLL ) ) * ( dLL > 0 ? 1.0F : -1.0F );
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
