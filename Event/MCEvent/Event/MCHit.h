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
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/Vector3DTypes.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class MCParticle;

  // Class ID definition
  static const CLID CLID_MCHit = 230;

  // Namespace for locations in TDS
  namespace MCHitLocation {
    inline const std::string Velo   = "MC/Velo/Hits";
    inline const std::string PuVeto = "MC/PuVeto/Hits";
    inline const std::string VP     = "MC/VP/Hits";
    inline const std::string TT     = "MC/TT/Hits";
    inline const std::string UT     = "MC/UT/Hits";
    inline const std::string IT     = "MC/IT/Hits";
    inline const std::string OT     = "MC/OT/Hits";
    inline const std::string Muon   = "MC/Muon/Hits";
    inline const std::string FT     = "MC/FT/Hits";
    inline const std::string SL     = "MC/SL/Hits";
    inline const std::string HC     = "MC/HC/Hits";
    inline const std::string Bcm    = "MC/Bcm/Hits";
    inline const std::string Bls    = "MC/Bls/Hits";
  } // namespace MCHitLocation

  /** @class MCHit MCHit.h
   *
   * GEANT tracking hit
   *
   * @author Marco Cattaneo, revised by G.Corti
   *
   */

  class MCHit : public ContainedObject {
  public:
    /// typedef for ObjectVector of MCHit
    using Container = ObjectVector<MCHit>;

    /// Default Constructor
    MCHit() = default;

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Exit point in LHCb reference system (mm)
    const Gaudi::XYZPoint exit() const;

    /// Distance between exit and entry points
    double pathLength() const;

    /// dxdz slope between exit and entry points
    double dxdz() const;

    /// dydz slope between exit and entry points
    double dydz() const;

    /// point half way between exit and entry points
    Gaudi::XYZPoint midPoint() const;

    /// Retrieve const  Sensitive detector identifier, its meaning is different for each sub-detector
    int sensDetID() const;

    /// Update  Sensitive detector identifier, its meaning is different for each sub-detector
    MCHit& setSensDetID( int value );

    /// Retrieve const  Entry point in LHCb reference system (mm)
    const Gaudi::XYZPoint& entry() const;

    /// Update  Entry point in LHCb reference system (mm)
    MCHit& setEntry( Gaudi::XYZPoint value );

    /// Retrieve const  Difference between exit and entry points (mm)
    const Gaudi::XYZVector& displacement() const;

    /// Update  Difference between exit and entry points (mm)
    MCHit& setDisplacement( Gaudi::XYZVector value );

    /// Retrieve const  Energy deposited in step (MeV)
    double energy() const;

    /// Update  Energy deposited in step (MeV)
    MCHit& setEnergy( double value );

    /// Retrieve const  Time (ns) relative to the origin collision
    double time() const;

    /// Update  Time (ns) relative to the origin collision
    MCHit& setTime( double value );

    /// Retrieve const  |P| of particle at the entry point (MeV)
    double p() const;

    /// Update  |P| of particle at the entry point (MeV)
    MCHit& setP( double value );

    /// Retrieve (const)  Pointer to MCParticle causing the hit
    const LHCb::MCParticle* mcParticle() const;

    /// Update  Pointer to MCParticle causing the hit
    MCHit& setMCParticle( SmartRef<LHCb::MCParticle> value );

    /// Update (pointer)  Pointer to MCParticle causing the hit
    MCHit& setMCParticle( const LHCb::MCParticle* value );

    friend std::ostream& operator<<( std::ostream& str, const MCHit& obj ) { return obj.fillStream( str ); }

  private:
    int              m_sensDetID{-1}; ///< Sensitive detector identifier, its meaning is different for each sub-detector
    Gaudi::XYZPoint  m_entry{0, 0, 0};        ///< Entry point in LHCb reference system (mm)
    Gaudi::XYZVector m_displacement{0, 0, 0}; ///< Difference between exit and entry points (mm)
    double           m_energy{0.0};           ///< Energy deposited in step (MeV)
    double           m_time{0.0};             ///< Time (ns) relative to the origin collision
    double           m_p{0.0};                ///< |P| of particle at the entry point (MeV)
    SmartRef<LHCb::MCParticle> m_MCParticle;  ///< Pointer to MCParticle causing the hit

  }; // class MCHit

  /// Definition of vector container type for MCHit
  using MCHits = ObjectVector<MCHit>;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations
#include "MCParticle.h"

inline const CLID& LHCb::MCHit::clID() const { return LHCb::MCHit::classID(); }

inline const CLID& LHCb::MCHit::classID() { return CLID_MCHit; }

inline std::ostream& LHCb::MCHit::fillStream( std::ostream& s ) const {
  s << "{ "
    << "sensDetID :	" << m_sensDetID << std::endl
    << "entry :	" << m_entry << std::endl
    << "displacement :	" << m_displacement << std::endl
    << "energy :	" << (float)m_energy << std::endl
    << "time :	" << (float)m_time << std::endl
    << "p :	" << (float)m_p << std::endl
    << " }";
  return s;
}

inline int LHCb::MCHit::sensDetID() const { return m_sensDetID; }

inline LHCb::MCHit& LHCb::MCHit::setSensDetID( int value ) {
  m_sensDetID = value;
  return *this;
}

inline const Gaudi::XYZPoint& LHCb::MCHit::entry() const { return m_entry; }

inline LHCb::MCHit& LHCb::MCHit::setEntry( Gaudi::XYZPoint value ) {
  m_entry = std::move( value );
  return *this;
}

inline const Gaudi::XYZVector& LHCb::MCHit::displacement() const { return m_displacement; }

inline LHCb::MCHit& LHCb::MCHit::setDisplacement( Gaudi::XYZVector value ) {
  m_displacement = std::move( value );
  return *this;
}

inline double LHCb::MCHit::energy() const { return m_energy; }

inline LHCb::MCHit& LHCb::MCHit::setEnergy( double value ) {
  m_energy = value;
  return *this;
}

inline double LHCb::MCHit::time() const { return m_time; }

inline LHCb::MCHit& LHCb::MCHit::setTime( double value ) {
  m_time = value;
  return *this;
}

inline double LHCb::MCHit::p() const { return m_p; }

inline LHCb::MCHit& LHCb::MCHit::setP( double value ) {
  m_p = value;
  return *this;
}

inline const LHCb::MCParticle* LHCb::MCHit::mcParticle() const { return m_MCParticle; }

inline LHCb::MCHit& LHCb::MCHit::setMCParticle( SmartRef<LHCb::MCParticle> value ) {
  m_MCParticle = std::move( value );
  return *this;
}

inline LHCb::MCHit& LHCb::MCHit::setMCParticle( const LHCb::MCParticle* value ) {
  m_MCParticle = value;
  return *this;
}

inline const Gaudi::XYZPoint LHCb::MCHit::exit() const { return ( this->entry() + this->displacement() ); }

inline double LHCb::MCHit::pathLength() const { return ( sqrt( this->displacement().mag2() ) ); }

inline double LHCb::MCHit::dxdz() const {

  double dxdz = 9999.;
  if ( 1.e-4 * Gaudi::Units::mm < fabs( this->displacement().z() ) ) {
    dxdz = this->displacement().x() / this->displacement().z();
  }
  return dxdz;
}

inline double LHCb::MCHit::dydz() const {

  double dydz = 9999.;
  if ( 1.e-4 * Gaudi::Units::mm < fabs( this->displacement().z() ) ) {
    dydz = this->displacement().y() / this->displacement().z();
  }
  return dydz;
}

inline Gaudi::XYZPoint LHCb::MCHit::midPoint() const { return ( this->entry() + 0.5 * this->displacement() ); }
