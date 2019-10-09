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
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/Vector4DTypes.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class MCParticle;

  // Class ID definition
  static const CLID CLID_MCSensPlaneHit = 232;

  // Namespace for locations in TDS
  namespace MCSensPlaneHitLocation {
    inline const std::string Ecal = "MC/Ecal/SensPlaneHits";
  }

  /** @class MCSensPlaneHit MCSensPlaneHit.h
   *
   * @brief Monte Carlo information from Sensitive plane * * * The class
   * represents general information of a particle entering a * plane sensitive
   * detector at the Geant level (i.e. also information not * stored as an
   * MCParticle (GEANT) * in the active material ("active energy") of *
   * calorimetry cell from the given Monte Carlo particle * at given "time" slot
   * (whatever it means) * *
   *
   * @author Vanya Belyaev, revised by V.Romanovski and G.Corti
   *
   */

  class MCSensPlaneHit : public ContainedObject {
  public:
    /// typedef for ObjectVector of MCSensPlaneHit
    typedef ObjectVector<MCSensPlaneHit> Container;

    /// Default Constructor
    MCSensPlaneHit()
        : m_position( 0, 0, -1 * Gaudi::Units::km, -100 * Gaudi::Units::second )
        , m_momentum( 0, 0, -100 * Gaudi::Units::TeV, -200 * Gaudi::Units::TeV )
        , m_type( 0 ) {}

    /// Default Destructor
    virtual ~MCSensPlaneHit() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Position of the hit in the sensitive plane
    const Gaudi::LorentzVector& position() const;

    /// Update  Position of the hit in the sensitive plane
    void setPosition( const Gaudi::LorentzVector& value );

    /// Retrieve const  Particle momentum for the hit in the sensitive plane
    const Gaudi::LorentzVector& momentum() const;

    /// Update  Particle momentum for the hit in the sensitive plane
    void setMomentum( const Gaudi::LorentzVector& value );

    /// Retrieve const  true particle Geant identifier
    int type() const;

    /// Update  true particle Geant identifier
    void setType( int value );

    /// Retrieve (const)  reference to the Monte Carlo particle
    const LHCb::MCParticle* particle() const;

    /// Update  reference to the Monte Carlo particle
    void setParticle( const SmartRef<LHCb::MCParticle>& value );

    /// Update (pointer)  reference to the Monte Carlo particle
    void setParticle( const LHCb::MCParticle* value );

    friend std::ostream& operator<<( std::ostream& str, const MCSensPlaneHit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    Gaudi::LorentzVector       m_position; ///< Position of the hit in the sensitive plane
    Gaudi::LorentzVector       m_momentum; ///< Particle momentum for the hit in the sensitive plane
    int                        m_type;     ///< true particle Geant identifier
    SmartRef<LHCb::MCParticle> m_particle; ///< reference to the Monte Carlo particle

  }; // class MCSensPlaneHit

  /// Definition of vector container type for MCSensPlaneHit
  typedef ObjectVector<MCSensPlaneHit> MCSensPlaneHits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations
#include "MCParticle.h"

inline const CLID& LHCb::MCSensPlaneHit::clID() const { return LHCb::MCSensPlaneHit::classID(); }

inline const CLID& LHCb::MCSensPlaneHit::classID() { return CLID_MCSensPlaneHit; }

inline std::ostream& LHCb::MCSensPlaneHit::fillStream( std::ostream& s ) const {
  s << "{ "
    << "position :	" << m_position << std::endl
    << "momentum :	" << m_momentum << std::endl
    << "type :	" << m_type << std::endl
    << " }";
  return s;
}

inline const Gaudi::LorentzVector& LHCb::MCSensPlaneHit::position() const { return m_position; }

inline void LHCb::MCSensPlaneHit::setPosition( const Gaudi::LorentzVector& value ) { m_position = value; }

inline const Gaudi::LorentzVector& LHCb::MCSensPlaneHit::momentum() const { return m_momentum; }

inline void LHCb::MCSensPlaneHit::setMomentum( const Gaudi::LorentzVector& value ) { m_momentum = value; }

inline int LHCb::MCSensPlaneHit::type() const { return m_type; }

inline void LHCb::MCSensPlaneHit::setType( int value ) { m_type = value; }

inline const LHCb::MCParticle* LHCb::MCSensPlaneHit::particle() const { return m_particle; }

inline void LHCb::MCSensPlaneHit::setParticle( const SmartRef<LHCb::MCParticle>& value ) { m_particle = value; }

inline void LHCb::MCSensPlaneHit::setParticle( const LHCb::MCParticle* value ) { m_particle = value; }
