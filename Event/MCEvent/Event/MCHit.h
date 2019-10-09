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
    typedef ObjectVector<MCHit> Container;

    /// Default Constructor
    MCHit()
        : m_sensDetID( -1 )
        , m_entry( 0.0, 0.0, 0.0 )
        , m_displacement( 0.0, 0.0, 0.0 )
        , m_energy( 0.0 )
        , m_time( 0.0 )
        , m_p( 0.0 ) {}

    /// Default Destructor
    virtual ~MCHit() {}

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
    void setSensDetID( int value );

    /// Retrieve const  Entry point in LHCb reference system (mm)
    const Gaudi::XYZPoint& entry() const;

    /// Update  Entry point in LHCb reference system (mm)
    void setEntry( const Gaudi::XYZPoint& value );

    /// Retrieve const  Difference between exit and entry points (mm)
    const Gaudi::XYZVector& displacement() const;

    /// Update  Difference between exit and entry points (mm)
    void setDisplacement( const Gaudi::XYZVector& value );

    /// Retrieve const  Energy deposited in step (MeV)
    double energy() const;

    /// Update  Energy deposited in step (MeV)
    void setEnergy( double value );

    /// Retrieve const  Time (ns) relative to the origin collision
    double time() const;

    /// Update  Time (ns) relative to the origin collision
    void setTime( double value );

    /// Retrieve const  |P| of particle at the entry point (MeV)
    double p() const;

    /// Update  |P| of particle at the entry point (MeV)
    void setP( double value );

    /// Retrieve (const)  Pointer to MCParticle causing the hit
    const LHCb::MCParticle* mcParticle() const;

    /// Update  Pointer to MCParticle causing the hit
    void setMCParticle( const SmartRef<LHCb::MCParticle>& value );

    /// Update (pointer)  Pointer to MCParticle causing the hit
    void setMCParticle( const LHCb::MCParticle* value );

    friend std::ostream& operator<<( std::ostream& str, const MCHit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    int              m_sensDetID;    ///< Sensitive detector identifier, its meaning is different for each sub-detector
    Gaudi::XYZPoint  m_entry;        ///< Entry point in LHCb reference system (mm)
    Gaudi::XYZVector m_displacement; ///< Difference between exit and entry points (mm)
    double           m_energy;       ///< Energy deposited in step (MeV)
    double           m_time;         ///< Time (ns) relative to the origin collision
    double           m_p;            ///< |P| of particle at the entry point (MeV)
    SmartRef<LHCb::MCParticle> m_MCParticle; ///< Pointer to MCParticle causing the hit

  }; // class MCHit

  /// Definition of vector container type for MCHit
  typedef ObjectVector<MCHit> MCHits;

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

inline void LHCb::MCHit::setSensDetID( int value ) { m_sensDetID = value; }

inline const Gaudi::XYZPoint& LHCb::MCHit::entry() const { return m_entry; }

inline void LHCb::MCHit::setEntry( const Gaudi::XYZPoint& value ) { m_entry = value; }

inline const Gaudi::XYZVector& LHCb::MCHit::displacement() const { return m_displacement; }

inline void LHCb::MCHit::setDisplacement( const Gaudi::XYZVector& value ) { m_displacement = value; }

inline double LHCb::MCHit::energy() const { return m_energy; }

inline void LHCb::MCHit::setEnergy( double value ) { m_energy = value; }

inline double LHCb::MCHit::time() const { return m_time; }

inline void LHCb::MCHit::setTime( double value ) { m_time = value; }

inline double LHCb::MCHit::p() const { return m_p; }

inline void LHCb::MCHit::setP( double value ) { m_p = value; }

inline const LHCb::MCParticle* LHCb::MCHit::mcParticle() const { return m_MCParticle; }

inline void LHCb::MCHit::setMCParticle( const SmartRef<LHCb::MCParticle>& value ) { m_MCParticle = value; }

inline void LHCb::MCHit::setMCParticle( const LHCb::MCParticle* value ) { m_MCParticle = value; }

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
