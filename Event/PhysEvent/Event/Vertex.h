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
#include "Event/VertexBase.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/NamedRange.h"
#include "GaudiKernel/SharedObjectsContainer.h"
#include "GaudiKernel/SmartRefVector.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class Particle;

  // Class ID definition
  static const CLID CLID_Vertex = 802;

  // Namespace for locations in TDS
  namespace VertexLocation {
    inline const std::string User       = "Phys/User/Vertices";
    inline const std::string Production = "Phys/Prod/Vertices";
  } // namespace VertexLocation

  /** @class Vertex Vertex.h
   *
   * Physics analysis Vertex. It holds geometrical information and is part of the
   * tree structure
   *
   * @author Patrick Koppenburg
   *
   */

  class Vertex : public VertexBase {
  public:
    /// typedef for std::vector of Vertex
    typedef std::vector<Vertex*>       Vector;
    typedef std::vector<const Vertex*> ConstVector;

    /// typedef for KeyedContainer of Vertex
    typedef KeyedContainer<Vertex, Containers::HashMap> Container;

    /// User information
    typedef VertexBase::ExtraInfo ExtraInfo;
    /// The container type for shared vertices (without ownership)
    typedef SharedObjectsContainer<LHCb::Vertex> Selection;
    /// For uniform access to containers in TES (KeyedContainer,SharedContainer)
    typedef Gaudi::NamedRange_<ConstVector> Range;

    /// Describe how the vertex was made (NEED MORE)
    enum CreationMethod {
      Unknown = 0,
      Primary = LHCb::VertexBase::VertexType::Primary,
      Unconstrained,
      MassConstrained,
      LastGlobal = 10000
    };

    /// create a vertex with a selected key
    explicit Vertex( int key ) : VertexBase( key ), m_technique( Vertex::CreationMethod::Unknown ) {}

    /// Constructor from a point. Needed by some tools. @to-do Find a way to remove this implicit conversion!
    Vertex( const Gaudi::XYZPoint& point ) : VertexBase( point ), m_technique( Vertex::CreationMethod::Unknown ) {}

    /// Default Constructor
    Vertex() : m_technique( Vertex::CreationMethod::Unknown ) {}

    /// Default Destructor
    virtual ~Vertex() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Clone vertex
    Vertex* clone() const override;

    /// Is the vertex a primary?
    bool isPrimary() const override;

    /// Get outgoing particles as a LHCb::Particle::ConstVector
    std::vector<const LHCb::Particle*> outgoingParticlesVector() const;

    /// Print this Particle in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  How the vertex was made
    const CreationMethod& technique() const;

    /// Update  How the vertex was made
    void setTechnique( const CreationMethod& value );

    /// Retrieve (const)  Reference to outgoing particles
    const SmartRefVector<LHCb::Particle>& outgoingParticles() const;

    /// Update  Reference to outgoing particles
    void setOutgoingParticles( const SmartRefVector<LHCb::Particle>& value );

    /// Add to  Reference to outgoing particles
    void addToOutgoingParticles( const SmartRef<LHCb::Particle>& value );

    /// Att to (pointer)  Reference to outgoing particles
    void addToOutgoingParticles( const LHCb::Particle* value );

    /// Remove from  Reference to outgoing particles
    void removeFromOutgoingParticles( const SmartRef<LHCb::Particle>& value );

    /// Clear  Reference to outgoing particles
    void clearOutgoingParticles();

    friend std::ostream& operator<<( std::ostream& str, const Vertex& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    CreationMethod                 m_technique;         ///< How the vertex was made
    SmartRefVector<LHCb::Particle> m_outgoingParticles; ///< Reference to outgoing particles

  }; // class Vertex

  /// Definition of Keyed Container for Vertex
  typedef KeyedContainer<Vertex, Containers::HashMap> Vertices;

  inline std::ostream& operator<<( std::ostream& s, LHCb::Vertex::CreationMethod e ) {
    switch ( e ) {
    case LHCb::Vertex::Unknown:
      return s << "Unknown";
    case LHCb::Vertex::Primary:
      return s << "Primary";
    case LHCb::Vertex::Unconstrained:
      return s << "Unconstrained";
    case LHCb::Vertex::MassConstrained:
      return s << "MassConstrained";
    case LHCb::Vertex::LastGlobal:
      return s << "LastGlobal";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::Vertex::CreationMethod";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations
#include "Particle.h"

inline const CLID& LHCb::Vertex::clID() const { return LHCb::Vertex::classID(); }

inline const CLID& LHCb::Vertex::classID() { return CLID_Vertex; }

inline const LHCb::Vertex::CreationMethod& LHCb::Vertex::technique() const { return m_technique; }

inline void LHCb::Vertex::setTechnique( const CreationMethod& value ) { m_technique = value; }

inline const SmartRefVector<LHCb::Particle>& LHCb::Vertex::outgoingParticles() const { return m_outgoingParticles; }

inline void LHCb::Vertex::setOutgoingParticles( const SmartRefVector<LHCb::Particle>& value ) {
  m_outgoingParticles = value;
}

inline void LHCb::Vertex::addToOutgoingParticles( const SmartRef<LHCb::Particle>& value ) {
  m_outgoingParticles.push_back( value );
}

inline void LHCb::Vertex::addToOutgoingParticles( const LHCb::Particle* value ) {
  m_outgoingParticles.push_back( value );
}

inline void LHCb::Vertex::removeFromOutgoingParticles( const SmartRef<LHCb::Particle>& value ) {
  auto i = std::remove( m_outgoingParticles.begin(), m_outgoingParticles.end(), value );
  m_outgoingParticles.erase( i, m_outgoingParticles.end() );
}

inline void LHCb::Vertex::clearOutgoingParticles() { m_outgoingParticles.clear(); }

inline LHCb::Vertex* LHCb::Vertex::clone() const { return new Vertex( *this ); }

inline bool LHCb::Vertex::isPrimary() const { return Vertex::CreationMethod::Primary == m_technique; }

inline std::vector<const LHCb::Particle*> LHCb::Vertex::outgoingParticlesVector() const {

  std::vector<const LHCb::Particle*> out;
  for ( SmartRefVector<LHCb::Particle>::const_iterator i = m_outgoingParticles.begin(); i != m_outgoingParticles.end();
        ++i ) {
    out.push_back( *i );
  }
  return out;
}
