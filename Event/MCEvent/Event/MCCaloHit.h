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
#include "Event/MCParticle.h"
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "Kernel/CaloCellID.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MCCaloHit = 2000;

  // Namespace for locations in TDS
  namespace MCCaloHitLocation {
    inline const std::string Spd  = "MC/Spd/Hits";
    inline const std::string Prs  = "MC/Prs/Hits";
    inline const std::string Ecal = "MC/Ecal/Hits";
    inline const std::string Hcal = "MC/Hcal/Hits";
  } // namespace MCCaloHitLocation

  /** @class MCCaloHit MCCaloHit.h
   *
   * @brief Monte Carlo energy deposition from one particle in the cell * * * The
   * class represents the Monte Carlo energy deposition (GEANT) * in the active
   * material ("active energy") of * calorimetry cell from the given Monte Carlo
   * particle * at given "time" slot (whatever it means) * *
   *
   * @author Olivier Callot (Olivier.Callot@cern.ch) & Vanya Belyaev(Ivan.Belyaev@itep.ru)
   *
   */

  class MCCaloHit : public ContainedObject {
  public:
    /// typedef for ObjectVector of MCCaloHit
    typedef ObjectVector<MCCaloHit> Container;

    /// MCCaloHits -> MCCaloHitVector equivalence
    typedef ObjectVector<LHCb::MCCaloHit> MCCaloHits;
    /// Shortcut for particle class
    typedef SmartRef<LHCb::MCParticle> Particle;
    /// The type of 'Time' variable (\#of 25ns timeslot)
    typedef char Time;

    /// Non-default constructor
    MCCaloHit( int id, double e, const Time& t, const Particle& p )
        : m_activeE( e ), m_sensDetID( id ), m_time( t ), m_particle( p ) {}

    /// Non-default constructor
    MCCaloHit( const LHCb::CaloCellID& cellID, double e, const Time& t, const Particle& p )
        : m_activeE( e ), m_sensDetID( int( cellID.all() ) ), m_time( t ), m_particle( p ) {}

    /// Copy Constructor
    MCCaloHit( const LHCb::MCCaloHit& right )
        : ContainedObject()
        , m_activeE( right.activeE() )
        , m_sensDetID( right.sensDetID() )
        , m_time( right.time() )
        , m_particle( right.particle() ) {}

    /// Default Constructor
    MCCaloHit() : m_activeE( 0 ), m_sensDetID( -1 ), m_time( 0 ) {}

    /// Default Destructor
    virtual ~MCCaloHit() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Cloning of the object ('virtual constructor')
    virtual MCCaloHit* clone() const;

    /// CaloCellID from sensDetID
    CaloCellID cellID() const;

    /// sensDetID from CaloCellID
    void setCellID( const LHCb::CaloCellID& id );

    /// Retrieve const  Monte Carlo energy deposition in the active media of given cell
    double activeE() const;

    /// Update  Monte Carlo energy deposition in the active media of given cell
    void setActiveE( double value );

    /// Retrieve const  Sensitive detector identifier - the identifier of calorimeter cell
    int sensDetID() const;

    /// Update  Sensitive detector identifier - the identifier of calorimeter cell
    void setSensDetID( int value );

    /// Retrieve const  Time slot# for energy deposition(in 25ns units, 0 means 'current BX')
    const Time& time() const;

    /// Update  Time slot# for energy deposition(in 25ns units, 0 means 'current BX')
    void setTime( const Time& value );

    /// Retrieve (const)  reference to the Monte Carlo particle
    const LHCb::MCParticle* particle() const;

    /// Update  reference to the Monte Carlo particle
    void setParticle( const SmartRef<LHCb::MCParticle>& value );

    /// Update (pointer)  reference to the Monte Carlo particle
    void setParticle( const LHCb::MCParticle* value );

    friend std::ostream& operator<<( std::ostream& str, const MCCaloHit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    double                     m_activeE;   ///< Monte Carlo energy deposition in the active media of given cell
    int                        m_sensDetID; ///< Sensitive detector identifier - the identifier of calorimeter cell
    Time                       m_time;      ///< Time slot# for energy deposition(in 25ns units, 0 means 'current BX')
    SmartRef<LHCb::MCParticle> m_particle;  ///< reference to the Monte Carlo particle

  }; // class MCCaloHit

  /// Definition of vector container type for MCCaloHit
  typedef ObjectVector<MCCaloHit> MCCaloHits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCCaloHit::clID() const { return LHCb::MCCaloHit::classID(); }

inline const CLID& LHCb::MCCaloHit::classID() { return CLID_MCCaloHit; }

inline std::ostream& LHCb::MCCaloHit::fillStream( std::ostream& s ) const {
  s << "{ "
    << "activeE :	" << (float)m_activeE << std::endl
    << "sensDetID :	" << m_sensDetID << std::endl
    << "time :	" << m_time << std::endl
    << " }";
  return s;
}

inline double LHCb::MCCaloHit::activeE() const { return m_activeE; }

inline void LHCb::MCCaloHit::setActiveE( double value ) { m_activeE = value; }

inline int LHCb::MCCaloHit::sensDetID() const { return m_sensDetID; }

inline void LHCb::MCCaloHit::setSensDetID( int value ) { m_sensDetID = value; }

inline const LHCb::MCCaloHit::Time& LHCb::MCCaloHit::time() const { return m_time; }

inline void LHCb::MCCaloHit::setTime( const Time& value ) { m_time = value; }

inline const LHCb::MCParticle* LHCb::MCCaloHit::particle() const { return m_particle; }

inline void LHCb::MCCaloHit::setParticle( const SmartRef<LHCb::MCParticle>& value ) { m_particle = value; }

inline void LHCb::MCCaloHit::setParticle( const LHCb::MCParticle* value ) { m_particle = value; }

inline LHCb::MCCaloHit* LHCb::MCCaloHit::clone() const { return new LHCb::MCCaloHit( *this ); }

inline LHCb::CaloCellID LHCb::MCCaloHit::cellID() const { return CaloCellID( m_sensDetID ); }

inline void LHCb::MCCaloHit::setCellID( const LHCb::CaloCellID& id ) { m_sensDetID = int( id.all() ); }
