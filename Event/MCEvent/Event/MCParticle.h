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
    typedef std::vector<MCParticle*>       Vector;
    typedef std::vector<const MCParticle*> ConstVector;

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
    MCParticle() : m_momentum( 0.0, 0.0, 0.0, -1 * Gaudi::Units::GeV ), m_particleID( 0 ), m_flags( 0 ) {}

    /// Default Destructor
    virtual ~MCParticle() {}

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
    void setMomentum( const Gaudi::LorentzVector& value );

    /// Retrieve const  Particle ID
    const LHCb::ParticleID& particleID() const;

    /// Update  Particle ID
    void setParticleID( const LHCb::ParticleID& value );

    /// Retrieve const  Bit-packed information on how this MCParticle was produced
    unsigned int flags() const;

    /// Update  Bit-packed information on how this MCParticle was produced
    void setFlags( unsigned int value );

    /// Retrieve Flag to indicate if this particle was produced as part of the signal decay tree
    bool fromSignal() const;

    /// Update Flag to indicate if this particle was produced as part of the signal decay tree
    void setFromSignal( bool value );

    /// Retrieve Reserved flags to extend fromSignal in the future (currently not used!)
    unsigned int _reservedFlags() const;

    /// Update Reserved flags to extend fromSignal in the future (currently not used!)
    void set_reservedFlags( unsigned int value );

    /// Retrieve Flag to indicate if this particle is in the ReDecay part of the event
    bool fromReDecay() const;

    /// Update Flag to indicate if this particle is in the ReDecay part of the event
    void setFromReDecay( bool value );

    /// Retrieve (const)  Pointer to origin vertex
    const LHCb::MCVertex* originVertex() const;

    /// Update  Pointer to origin vertex
    void setOriginVertex( const SmartRef<LHCb::MCVertex>& value );

    /// Update (pointer)  Pointer to origin vertex
    void setOriginVertex( const LHCb::MCVertex* value );

    /// Retrieve (const)  Vector of pointers to decay vertices
    const SmartRefVector<LHCb::MCVertex>& endVertices() const;

    /// Update  Vector of pointers to decay vertices
    void setEndVertices( const SmartRefVector<LHCb::MCVertex>& value );

    /// Add to  Vector of pointers to decay vertices
    void addToEndVertices( const SmartRef<LHCb::MCVertex>& value );

    /// Att to (pointer)  Vector of pointers to decay vertices
    void addToEndVertices( const LHCb::MCVertex* value );

    /// Remove from  Vector of pointers to decay vertices
    void removeFromEndVertices( const SmartRef<LHCb::MCVertex>& value );

    /// Clear  Vector of pointers to decay vertices
    void clearEndVertices();

    friend std::ostream& operator<<( std::ostream& str, const MCParticle& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Offsets of bitfield flags
    enum flagsBits { fromSignalBits = 0, _reservedFlagsBits = 1, fromReDecayBits = 4 };

    /// Bitmasks for bitfield flags
    enum flagsMasks { fromSignalMask = 0x1L, _reservedFlagsMask = 0xeL, fromReDecayMask = 0x10L };

    Gaudi::LorentzVector           m_momentum;     ///< 4-momentum-vector
    LHCb::ParticleID               m_particleID;   ///< Particle ID
    unsigned int                   m_flags;        ///< Bit-packed information on how this MCParticle was produced
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

inline void LHCb::MCParticle::setMomentum( const Gaudi::LorentzVector& value ) { m_momentum = value; }

inline const LHCb::ParticleID& LHCb::MCParticle::particleID() const { return m_particleID; }

inline void LHCb::MCParticle::setParticleID( const LHCb::ParticleID& value ) { m_particleID = value; }

inline unsigned int LHCb::MCParticle::flags() const { return m_flags; }

inline void LHCb::MCParticle::setFlags( unsigned int value ) { m_flags = value; }

inline bool LHCb::MCParticle::fromSignal() const { return 0 != ( ( m_flags & fromSignalMask ) >> fromSignalBits ); }

inline void LHCb::MCParticle::setFromSignal( bool value ) {
  unsigned int val = (unsigned int)value;
  m_flags &= ~fromSignalMask;
  m_flags |= ( ( ( (unsigned int)val ) << fromSignalBits ) & fromSignalMask );
}

inline unsigned int LHCb::MCParticle::_reservedFlags() const {
  return (unsigned int)( ( m_flags & _reservedFlagsMask ) >> _reservedFlagsBits );
}

inline void LHCb::MCParticle::set_reservedFlags( unsigned int value ) {
  unsigned int val = (unsigned int)value;
  m_flags &= ~_reservedFlagsMask;
  m_flags |= ( ( ( (unsigned int)val ) << _reservedFlagsBits ) & _reservedFlagsMask );
}

inline bool LHCb::MCParticle::fromReDecay() const { return 0 != ( ( m_flags & fromReDecayMask ) >> fromReDecayBits ); }

inline void LHCb::MCParticle::setFromReDecay( bool value ) {
  unsigned int val = (unsigned int)value;
  m_flags &= ~fromReDecayMask;
  m_flags |= ( ( ( (unsigned int)val ) << fromReDecayBits ) & fromReDecayMask );
}

inline const LHCb::MCVertex* LHCb::MCParticle::originVertex() const { return m_originVertex; }

inline void LHCb::MCParticle::setOriginVertex( const SmartRef<LHCb::MCVertex>& value ) { m_originVertex = value; }

inline void LHCb::MCParticle::setOriginVertex( const LHCb::MCVertex* value ) { m_originVertex = value; }

inline const SmartRefVector<LHCb::MCVertex>& LHCb::MCParticle::endVertices() const { return m_endVertices; }

inline void LHCb::MCParticle::setEndVertices( const SmartRefVector<LHCb::MCVertex>& value ) { m_endVertices = value; }

inline void LHCb::MCParticle::addToEndVertices( const SmartRef<LHCb::MCVertex>& value ) {
  m_endVertices.push_back( value );
}

inline void LHCb::MCParticle::addToEndVertices( const LHCb::MCVertex* value ) { m_endVertices.push_back( value ); }

inline void LHCb::MCParticle::removeFromEndVertices( const SmartRef<LHCb::MCVertex>& value ) {
  auto i = std::remove( m_endVertices.begin(), m_endVertices.end(), value );
  m_endVertices.erase( i, m_endVertices.end() );
}

inline void LHCb::MCParticle::clearEndVertices() { m_endVertices.clear(); }

inline LHCb::MCParticle* LHCb::MCParticle::clone() const { return new LHCb::MCParticle( *this ); }

inline LHCb::MCParticle* LHCb::MCParticle::cloneWithKey() const {

  LHCb::MCParticle* clone = new LHCb::MCParticle( *this );
  clone->setKey( this->key() );
  return clone;
}

inline double LHCb::MCParticle::virtualMass() const { return m_momentum.M(); }

inline double LHCb::MCParticle::pt() const { return m_momentum.Pt(); }

inline double LHCb::MCParticle::p() const { return m_momentum.R(); }

inline const LHCb::MCParticle* LHCb::MCParticle::mother() const {

  return ( originVertex() ? originVertex()->mother() : NULL );
}

inline double LHCb::MCParticle::beta() const { return ( m_momentum.E() > 0. ? m_momentum.R() / m_momentum.E() : 1. ); }

inline double LHCb::MCParticle::gamma() const {

  return ( m_momentum.M() > 0. ? m_momentum.E() / m_momentum.M() : -999. );
}

inline double LHCb::MCParticle::betaGamma() const {

  return ( m_momentum.M() > 0. ? m_momentum.R() / m_momentum.M() : -999. );
}

inline double LHCb::MCParticle::pseudoRapidity() const {

#ifdef __INTEL_COMPILER             // Disable ICC remark from GenVector/eta.h
#  pragma warning( disable : 1572 ) // Floating-point equality and inequality comparisons are unreliable
#  pragma warning( push )
#endif
  return m_momentum.eta();
#ifdef __INTEL_COMPILER // Re-enable ICC remark 1572
#  pragma warning( pop )
#endif
}

inline const LHCb::MCVertex* LHCb::MCParticle::primaryVertex() const {

  return ( originVertex() ? originVertex()->primaryVertex() : NULL );
}
