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
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/Vector4DTypes.h"
#include "Kernel/ParticleID.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class MCVertex;

  // Class ID definition
  static const CLID CLID_MCParticle = 210;

  // Namespace for locations in TDS
  namespace MCParticleLocation {
    inline const std::string Default = "MC/Particles";
  }

  /** @class MCParticle MCParticle.h
   *
   * The Monte Carlo particle kinematics information
   *
   * @author Gloria Corti, revised by P. Koppenburg
   *
   */

  class MCParticle : public KeyedObject<int> {
  public:
    /// typedef for std::vector of MCParticle
    using Vector      = std::vector<MCParticle*>;
    using ConstVector = std::vector<const MCParticle*>;

    /// typedef for KeyedContainer of MCParticle
    typedef KeyedContainer<MCParticle, Containers::HashMap> Container;

    /// const MCParticle keyed container
    typedef KeyedContainer<const MCParticle, Containers::HashMap> ConstContainer;

    /// Copy Constructor[Attention: the key is NOT copied!]
    MCParticle( const LHCb::MCParticle& right )
        : KeyedObject<int>()
        , m_momentum( right.m_momentum )
        , m_particleID( right.m_particleID )
        , m_flags( right.m_flags )
        , m_originVertex( right.m_originVertex )
        , m_endVertices( right.m_endVertices ) {}

    /// Default Constructor
    MCParticle() = default;

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Clone this MCParticle. Returns a pointer to a new MCParticle (user must take ownership)
    MCParticle* clone() const;

    /// Clone this MCParticle including key. Returns a pointer to a new MCParticle (user must take ownership)
    MCParticle* cloneWithKey() const;

    /// Describe if a particle has oscillated
    bool hasOscillated() const;

    /// Retrieve virtual mass, i.e. sqrt(E^2-P^2)
    double virtualMass() const;

    /// Short-cut to pt value
    double pt() const;

    /// short cut for |P|
    double p() const;

    /// Pointer to parent particle
    const LHCb::MCParticle* mother() const;

    /// Beta of the particle
    double beta() const;

    /// Gamma of the particle
    double gamma() const;

    /// Beta x Gamma of the particle
    double betaGamma() const;

    /// Pseudorapidity of the particle
    double pseudoRapidity() const;

    /// Get primary vertex up the decay tree
    const LHCb::MCVertex* primaryVertex() const;

    /// Print this MCParticle in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  4-momentum-vector
    const Gaudi::LorentzVector& momentum() const;

    /// Update  4-momentum-vector
    MCParticle& setMomentum( Gaudi::LorentzVector value );

    /// Retrieve const  Particle ID
    const LHCb::ParticleID& particleID() const;

    /// Update  Particle ID
    MCParticle& setParticleID( LHCb::ParticleID value );

    /// Retrieve const  Bit-packed information on how this MCParticle was produced
    unsigned int flags() const;

    /// Update  Bit-packed information on how this MCParticle was produced
    MCParticle& setFlags( unsigned int value );

    /// Retrieve Flag to indicate if this particle was produced as part of the signal decay tree
    bool fromSignal() const;

    /// Update Flag to indicate if this particle was produced as part of the signal decay tree
    MCParticle& setFromSignal( bool value );

    /// Retrieve Reserved flags to extend fromSignal in the future (currently not used!)
    unsigned int _reservedFlags() const;

    /// Update Reserved flags to extend fromSignal in the future (currently not used!)
    MCParticle& set_reservedFlags( unsigned int value );

    /// Retrieve Flag to indicate if this particle is in the ReDecay part of the event
    bool fromReDecay() const;

    /// Update Flag to indicate if this particle is in the ReDecay part of the event
    MCParticle& setFromReDecay( bool value );

    /// Retrieve (const)  Pointer to origin vertex
    const LHCb::MCVertex* originVertex() const;

    /// Update  Pointer to origin vertex
    MCParticle& setOriginVertex( SmartRef<LHCb::MCVertex> value );

    /// Update (pointer)  Pointer to origin vertex
    MCParticle& setOriginVertex( const LHCb::MCVertex* value );

    /// Retrieve (const)  Vector of pointers to decay vertices
    const SmartRefVector<LHCb::MCVertex>& endVertices() const;

    /// Update  Vector of pointers to decay vertices
    MCParticle& setEndVertices( SmartRefVector<LHCb::MCVertex> value );

    /// Add to  Vector of pointers to decay vertices
    MCParticle& addToEndVertices( SmartRef<LHCb::MCVertex> value );

    /// Att to (pointer)  Vector of pointers to decay vertices
    MCParticle& addToEndVertices( const LHCb::MCVertex* value );

    /// Remove from  Vector of pointers to decay vertices
    MCParticle& removeFromEndVertices( const SmartRef<LHCb::MCVertex>& value );

    /// Clear  Vector of pointers to decay vertices
    MCParticle& clearEndVertices();

    friend std::ostream& operator<<( std::ostream& str, const MCParticle& obj ) { return obj.fillStream( str ); }

  private:
    /// Offsets of bitfield flags
    enum flagsBits { fromSignalBits = 0, _reservedFlagsBits = 1, fromReDecayBits = 4 };

    /// Bitmasks for bitfield flags
    enum flagsMasks { fromSignalMask = 0x1L, _reservedFlagsMask = 0xeL, fromReDecayMask = 0x10L };

    Gaudi::LorentzVector           m_momentum{0.0, 0.0, 0.0, -1 * Gaudi::Units::GeV}; ///< 4-momentum-vector
    LHCb::ParticleID               m_particleID{0};                                   ///< Particle ID
    unsigned int                   m_flags{0};     ///< Bit-packed information on how this MCParticle was produced
    SmartRef<LHCb::MCVertex>       m_originVertex; ///< Pointer to origin vertex
    SmartRefVector<LHCb::MCVertex> m_endVertices;  ///< Vector of pointers to decay vertices

  }; // class MCParticle

  /// Definition of Keyed Container for MCParticle
  typedef KeyedContainer<MCParticle, Containers::HashMap> MCParticles;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations
#include "MCVertex.h"

inline const CLID& LHCb::MCParticle::clID() const { return LHCb::MCParticle::classID(); }

inline const CLID& LHCb::MCParticle::classID() { return CLID_MCParticle; }

inline const Gaudi::LorentzVector& LHCb::MCParticle::momentum() const { return m_momentum; }

inline LHCb::MCParticle& LHCb::MCParticle::setMomentum( Gaudi::LorentzVector value ) {
  m_momentum = std::move( value );
  return *this;
}

inline const LHCb::ParticleID& LHCb::MCParticle::particleID() const { return m_particleID; }

inline LHCb::MCParticle& LHCb::MCParticle::setParticleID( LHCb::ParticleID value ) {
  m_particleID = std::move( value );
  return *this;
}

inline unsigned int LHCb::MCParticle::flags() const { return m_flags; }

inline LHCb::MCParticle& LHCb::MCParticle::setFlags( unsigned int value ) {
  m_flags = value;
  return *this;
}

inline bool LHCb::MCParticle::fromSignal() const { return 0 != ( ( m_flags & fromSignalMask ) >> fromSignalBits ); }

inline LHCb::MCParticle& LHCb::MCParticle::setFromSignal( bool value ) {
  m_flags &= ~fromSignalMask;
  m_flags |= ( ( ( (unsigned int)value ) << fromSignalBits ) & fromSignalMask );
  return *this;
}

inline unsigned int LHCb::MCParticle::_reservedFlags() const {
  return (unsigned int)( ( m_flags & _reservedFlagsMask ) >> _reservedFlagsBits );
}

inline LHCb::MCParticle& LHCb::MCParticle::set_reservedFlags( unsigned int value ) {
  m_flags &= ~_reservedFlagsMask;
  m_flags |= ( ( value << _reservedFlagsBits ) & _reservedFlagsMask );
  return *this;
}

inline bool LHCb::MCParticle::fromReDecay() const { return 0 != ( ( m_flags & fromReDecayMask ) >> fromReDecayBits ); }

inline LHCb::MCParticle& LHCb::MCParticle::setFromReDecay( bool value ) {
  m_flags &= ~fromReDecayMask;
  m_flags |= ( ( ( (unsigned int)value ) << fromReDecayBits ) & fromReDecayMask );
  return *this;
}

inline const LHCb::MCVertex* LHCb::MCParticle::originVertex() const { return m_originVertex; }

inline LHCb::MCParticle& LHCb::MCParticle::setOriginVertex( SmartRef<LHCb::MCVertex> value ) {
  m_originVertex = std::move( value );
  return *this;
}

inline LHCb::MCParticle& LHCb::MCParticle::setOriginVertex( const LHCb::MCVertex* value ) {
  m_originVertex = value;
  return *this;
}

inline const SmartRefVector<LHCb::MCVertex>& LHCb::MCParticle::endVertices() const { return m_endVertices; }

inline LHCb::MCParticle& LHCb::MCParticle::setEndVertices( SmartRefVector<LHCb::MCVertex> value ) {
  m_endVertices = std::move( value );
  return *this;
}

inline LHCb::MCParticle& LHCb::MCParticle::addToEndVertices( SmartRef<LHCb::MCVertex> value ) {
  m_endVertices.push_back( std::move( value ) );
  return *this;
}

inline LHCb::MCParticle& LHCb::MCParticle::addToEndVertices( const LHCb::MCVertex* value ) {
  m_endVertices.push_back( value );
  return *this;
}

inline LHCb::MCParticle& LHCb::MCParticle::removeFromEndVertices( const SmartRef<LHCb::MCVertex>& value ) {
  auto i = std::remove( m_endVertices.begin(), m_endVertices.end(), value );
  m_endVertices.erase( i, m_endVertices.end() );
  return *this;
}

inline LHCb::MCParticle& LHCb::MCParticle::clearEndVertices() {
  m_endVertices.clear();
  return *this;
}

inline LHCb::MCParticle* LHCb::MCParticle::clone() const { return new LHCb::MCParticle( *this ); }

inline LHCb::MCParticle* LHCb::MCParticle::cloneWithKey() const {
  auto* clone = new LHCb::MCParticle( *this );
  clone->setKey( this->key() );
  return clone;
}

inline double LHCb::MCParticle::virtualMass() const { return m_momentum.M(); }

inline double LHCb::MCParticle::pt() const { return m_momentum.Pt(); }

inline double LHCb::MCParticle::p() const { return m_momentum.R(); }

inline const LHCb::MCParticle* LHCb::MCParticle::mother() const {
  return originVertex() ? originVertex()->mother() : nullptr;
}

inline double LHCb::MCParticle::beta() const { return ( m_momentum.E() > 0. ? m_momentum.R() / m_momentum.E() : 1. ); }

inline double LHCb::MCParticle::gamma() const { return m_momentum.M() > 0. ? m_momentum.E() / m_momentum.M() : -999.; }

inline double LHCb::MCParticle::betaGamma() const {

  return m_momentum.M() > 0. ? m_momentum.R() / m_momentum.M() : -999.;
}

inline double LHCb::MCParticle::pseudoRapidity() const { return m_momentum.eta(); }

inline const LHCb::MCVertex* LHCb::MCParticle::primaryVertex() const {

  return originVertex() ? originVertex()->primaryVertex() : nullptr;
}
