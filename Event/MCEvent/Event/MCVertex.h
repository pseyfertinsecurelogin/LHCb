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
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Point4DTypes.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class MCParticle;

  // Class ID definition
  static const CLID CLID_MCVertex = 220;

  // Namespace for locations in TDS
  namespace MCVertexLocation {
    inline const std::string Default = "MC/Vertices";
  }

  /** @class MCVertex MCVertex.h
   *
   * Monte Carlo vertex information
   *
   * @author Gloria Corti, revised by P. Koppenburg
   *
   */

  class MCVertex : public KeyedObject<int> {
  public:
    /// typedef for std::vector of MCVertex
    using Vector      = std::vector<MCVertex*>;
    using ConstVector = std::vector<const MCVertex*>;

    /// typedef for KeyedContainer of MCVertex
    using Container = KeyedContainer<MCVertex, Containers::HashMap>;

    /// Describe the physics process related to the vertex
    enum MCVertexType {
      Unknown = 0,
      ppCollision,
      DecayVertex,
      OscillatedAndDecay,
      StringFragmentation,
      HadronicInteraction = 100,
      Bremsstrahlung,
      PairProduction,
      Compton,
      DeltaRay,
      PhotoElectric,
      Annihilation,
      RICHPhotoElectric,
      Cerenkov,
      RichHpdBackScat,
      GenericInteraction,
      LHCHalo        = 200,
      MuonBackground = 300,
      MuonBackgroundFlat,
      MuonBackgroundSpillover,
      WorldLimit = 400,
      KinematicLimit
    };

    /// Copy Constructor
    MCVertex( const LHCb::MCVertex* right )
        : KeyedObject<int>() // note: key is _not_ copied...
        , m_position( right->position() )
        , m_time( right->time() )
        , m_type( right->type() )
        , m_mother( right->mother() )
        , m_products( right->products() ) {}

    /// Default Constructor
    MCVertex() = default;

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Clone this MCVertex. Returns a pointer to a new MCVertex (user must take ownership)
    MCVertex* clone() const;

    /// Clone this MCVertex including key. Returns a pointer to a new MCVertex (user must take ownership)
    MCVertex* cloneWithKey() const;

    /// position time 4-vector
    const Gaudi::XYZTPoint position4vector() const;

    /// Returns true if this vertex is a primary vertex
    bool isPrimary() const;

    /// Returns true if this vertex is a decay vertex
    bool isDecay() const;

    /// Get primary vertex up the decay tree
    const LHCb::MCVertex* primaryVertex() const;

    /// Print this MCVertex in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Position in LHCb reference system
    const Gaudi::XYZPoint& position() const;

    /// Update  Position in LHCb reference system
    MCVertex& setPosition( Gaudi::XYZPoint value );

    /// Retrieve const  Time since pp interaction
    double time() const;

    /// Update  Time since pp interaction
    MCVertex& setTime( double value );

    /// Retrieve const  How the vertex was made
    const MCVertexType& type() const;

    /// Update  How the vertex was made
    MCVertex& setType( MCVertexType value );

    /// Retrieve (const)  Pointer to parent particle that decay or otherwise end in this vertex
    const LHCb::MCParticle* mother() const;

    /// Update  Pointer to parent particle that decay or otherwise end in this vertex
    MCVertex& setMother( SmartRef<LHCb::MCParticle> value );

    /// Update (pointer)  Pointer to parent particle that decay or otherwise end in this vertex
    MCVertex& setMother( const LHCb::MCParticle* value );

    /// Retrieve (const)  Pointer to daughter particles
    const SmartRefVector<LHCb::MCParticle>& products() const;

    /// Update  Pointer to daughter particles
    MCVertex& setProducts( SmartRefVector<LHCb::MCParticle> value );

    /// Add to  Pointer to daughter particles
    MCVertex& addToProducts( SmartRef<LHCb::MCParticle> value );

    /// Att to (pointer)  Pointer to daughter particles
    MCVertex& addToProducts( const LHCb::MCParticle* value );

    /// Remove from  Pointer to daughter particles
    MCVertex& removeFromProducts( const SmartRef<LHCb::MCParticle>& value );

    /// Clear  Pointer to daughter particles
    MCVertex& clearProducts();

    friend std::ostream& operator<<( std::ostream& str, const MCVertex& obj ) { return obj.fillStream( str ); }

  private:
    Gaudi::XYZPoint            m_position{0.0, 0.0, 0.0};               ///< Position in LHCb reference system
    double                     m_time{0.0};                             ///< Time since pp interaction
    MCVertexType               m_type{MCVertex::MCVertexType::Unknown}; ///< How the vertex was made
    SmartRef<LHCb::MCParticle> m_mother; ///< Pointer to parent particle that decay or otherwise end in this vertex
    SmartRefVector<LHCb::MCParticle> m_products; ///< Pointer to daughter particles

  }; // class MCVertex

  /// Definition of Keyed Container for MCVertex
  typedef KeyedContainer<MCVertex, Containers::HashMap> MCVertices;

  inline std::ostream& operator<<( std::ostream& s, LHCb::MCVertex::MCVertexType e ) {
    switch ( e ) {
    case LHCb::MCVertex::Unknown:
      return s << "Unknown";
    case LHCb::MCVertex::ppCollision:
      return s << "ppCollision";
    case LHCb::MCVertex::DecayVertex:
      return s << "DecayVertex";
    case LHCb::MCVertex::OscillatedAndDecay:
      return s << "OscillatedAndDecay";
    case LHCb::MCVertex::StringFragmentation:
      return s << "StringFragmentation";
    case LHCb::MCVertex::HadronicInteraction:
      return s << "HadronicInteraction";
    case LHCb::MCVertex::Bremsstrahlung:
      return s << "Bremsstrahlung";
    case LHCb::MCVertex::PairProduction:
      return s << "PairProduction";
    case LHCb::MCVertex::Compton:
      return s << "Compton";
    case LHCb::MCVertex::DeltaRay:
      return s << "DeltaRay";
    case LHCb::MCVertex::PhotoElectric:
      return s << "PhotoElectric";
    case LHCb::MCVertex::Annihilation:
      return s << "Annihilation";
    case LHCb::MCVertex::RICHPhotoElectric:
      return s << "RICHPhotoElectric";
    case LHCb::MCVertex::Cerenkov:
      return s << "Cerenkov";
    case LHCb::MCVertex::RichHpdBackScat:
      return s << "RichHpdBackScat";
    case LHCb::MCVertex::GenericInteraction:
      return s << "GenericInteraction";
    case LHCb::MCVertex::LHCHalo:
      return s << "LHCHalo";
    case LHCb::MCVertex::MuonBackground:
      return s << "MuonBackground";
    case LHCb::MCVertex::MuonBackgroundFlat:
      return s << "MuonBackgroundFlat";
    case LHCb::MCVertex::MuonBackgroundSpillover:
      return s << "MuonBackgroundSpillover";
    case LHCb::MCVertex::WorldLimit:
      return s << "WorldLimit";
    case LHCb::MCVertex::KinematicLimit:
      return s << "KinematicLimit";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::MCVertex::MCVertexType";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations
#include "MCParticle.h"

inline const CLID& LHCb::MCVertex::clID() const { return LHCb::MCVertex::classID(); }

inline const CLID& LHCb::MCVertex::classID() { return CLID_MCVertex; }

inline const Gaudi::XYZPoint& LHCb::MCVertex::position() const { return m_position; }

inline LHCb::MCVertex& LHCb::MCVertex::setPosition( Gaudi::XYZPoint value ) {
  m_position = std::move( value );
  return *this;
}

inline double LHCb::MCVertex::time() const { return m_time; }

inline LHCb::MCVertex& LHCb::MCVertex::setTime( double value ) {
  m_time = value;
  return *this;
}

inline const LHCb::MCVertex::MCVertexType& LHCb::MCVertex::type() const { return m_type; }

inline LHCb::MCVertex& LHCb::MCVertex::setType( MCVertexType value ) {
  m_type = std::move( value );
  return *this;
}

inline const LHCb::MCParticle* LHCb::MCVertex::mother() const { return m_mother; }

inline LHCb::MCVertex& LHCb::MCVertex::setMother( SmartRef<LHCb::MCParticle> value ) {
  m_mother = std::move( value );
  return *this;
}

inline LHCb::MCVertex& LHCb::MCVertex::setMother( const LHCb::MCParticle* value ) {
  m_mother = value;
  return *this;
}

inline const SmartRefVector<LHCb::MCParticle>& LHCb::MCVertex::products() const { return m_products; }

inline LHCb::MCVertex& LHCb::MCVertex::setProducts( SmartRefVector<LHCb::MCParticle> value ) {
  m_products = std::move( value );
  return *this;
}

inline LHCb::MCVertex& LHCb::MCVertex::addToProducts( SmartRef<LHCb::MCParticle> value ) {
  m_products.push_back( std::move( value ) );
  return *this;
}

inline LHCb::MCVertex& LHCb::MCVertex::addToProducts( const LHCb::MCParticle* value ) {
  m_products.push_back( value );
  return *this;
}

inline LHCb::MCVertex& LHCb::MCVertex::removeFromProducts( const SmartRef<LHCb::MCParticle>& value ) {
  auto i = std::remove( m_products.begin(), m_products.end(), value );
  m_products.erase( i, m_products.end() );
  return *this;
}

inline LHCb::MCVertex& LHCb::MCVertex::clearProducts() {
  m_products.clear();
  return *this;
}

inline LHCb::MCVertex* LHCb::MCVertex::clone() const { return new LHCb::MCVertex( this ); }

inline LHCb::MCVertex* LHCb::MCVertex::cloneWithKey() const {

  auto* clone = new LHCb::MCVertex( this );
  clone->setKey( this->key() );
  return clone;
}

inline const Gaudi::XYZTPoint LHCb::MCVertex::position4vector() const {
  return Gaudi::XYZTPoint( m_position.X(), m_position.Y(), m_position.Z(), m_time );
}

inline bool LHCb::MCVertex::isPrimary() const { return m_type == MCVertex::MCVertexType::ppCollision; }

inline bool LHCb::MCVertex::isDecay() const {

  return ( ( m_type == MCVertex::MCVertexType::DecayVertex ) ||
           ( m_type == MCVertex::MCVertexType::OscillatedAndDecay ) );
}

inline const LHCb::MCVertex* LHCb::MCVertex::primaryVertex() const {

  return ( this->isPrimary() ? this : ( m_mother != 0 ? m_mother->primaryVertex() : nullptr ) );
}
