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
#include "Event/ProcessHeader.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class BeamParameters;
  class GenCollision;

  // Class ID definition
  static const CLID CLID_GenHeader = 102;

  // Namespace for locations in TDS
  namespace GenHeaderLocation {
    inline const std::string Default = "Gen/Header";
  }

  /** @class GenHeader GenHeader.h
   *
   * Header for event generator information
   *
   * @author Marco Cattaneo, revised by P. Koppenburg, then G.Corti
   *
   */

  class GenHeader final : public ProcessHeader {
  public:
    /// Constructor
    GenHeader();

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Number of collisions in bunch crossing
    unsigned int numOfCollisions() const;

    /// Retrieve instantaneous luminosity used for pileup probability
    double luminosity() const;

    /// Retrieve total cross-section used for pileup probability
    double totCrossSection() const;

    /// Retrieve filled-filled bunches crossing frequency
    double crossingFreq() const;

    /// Retrieve const  LHCb event type. Code used in BookKeeping to identify samples, as defined in <a
    /// href="http://cdsweb.cern.ch/record-restricted/855452/">LHCb-2005-034</a>
    int evType() const;

    /// Update  LHCb event type. Code used in BookKeeping to identify samples, as defined in <a
    /// href="http://cdsweb.cern.ch/record-restricted/855452/">LHCb-2005-034</a>
    void setEvType( int value );

    /// Retrieve const  Event number
    long long evtNumber() const;

    /// Update  Event number
    void setEvtNumber( long long value );

    /// Retrieve (const)  Pointer to collisions in this event
    const SmartRefVector<LHCb::GenCollision>& collisions() const;

    /// Update  Pointer to collisions in this event
    void setCollisions( const SmartRefVector<LHCb::GenCollision>& value );

    /// Add to  Pointer to collisions in this event
    void addToCollisions( const SmartRef<LHCb::GenCollision>& value );

    /// Att to (pointer)  Pointer to collisions in this event
    void addToCollisions( const LHCb::GenCollision* value );

    /// Remove from  Pointer to collisions in this event
    void removeFromCollisions( const SmartRef<LHCb::GenCollision>& value );

    /// Clear  Pointer to collisions in this event
    void clearCollisions();

    /// Retrieve (const)  Pointer to beam parameters in this event
    const LHCb::BeamParameters* beamParameters() const;

    /// Update  Pointer to beam parameters in this event
    void setBeamParameters( const SmartRef<LHCb::BeamParameters>& value );

    /// Update (pointer)  Pointer to beam parameters in this event
    void setBeamParameters( const LHCb::BeamParameters* value );

    friend std::ostream& operator<<( std::ostream& str, const GenHeader& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    double m_luminosity;         ///< Retrieve const Instantaneous luminosity used for pileUp probability (for backward
                                 ///< compatibility)
    int m_evType;                ///< LHCb event type. Code used in BookKeeping to identify samples, as defined in <a
                                 ///< href="http://cdsweb.cern.ch/record-restricted/855452/">LHCb-2005-034</a>
    long long m_evtNumber;       ///< Event number
    double    m_totCrossSection; ///< Retrieve const Total cross section used for pileUp probability (for backward
                                 ///< compatibility)
    double m_crossingFreq; ///< Retrieve const Filled-filled bunches crossing frequency (for backward compatibility)
    SmartRefVector<LHCb::GenCollision> m_collisions;     ///< Pointer to collisions in this event
    SmartRef<LHCb::BeamParameters>     m_beamParameters; ///< Pointer to beam parameters in this event

  }; // class GenHeader

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations
#include "BeamParameters.h"
#include "GenCollision.h"

inline LHCb::GenHeader::GenHeader()
    : m_luminosity( 0.0 ), m_evType( 0 ), m_evtNumber( 0 ), m_totCrossSection( 0.0 ), m_crossingFreq( 0.0 ) {

  setVersion( '2' );
}

inline const CLID& LHCb::GenHeader::clID() const { return LHCb::GenHeader::classID(); }

inline const CLID& LHCb::GenHeader::classID() { return CLID_GenHeader; }

inline std::ostream& LHCb::GenHeader::fillStream( std::ostream& s ) const {
  ProcessHeader::fillStream( s );
  s << "{ "
    << "luminosity :	" << (float)m_luminosity << std::endl
    << "evType :	" << m_evType << std::endl
    << "evtNumber :	" << m_evtNumber << std::endl
    << "totCrossSection :	" << (float)m_totCrossSection << std::endl
    << "crossingFreq :	" << (float)m_crossingFreq << std::endl
    << " }";
  return s;
}

inline int LHCb::GenHeader::evType() const { return m_evType; }

inline void LHCb::GenHeader::setEvType( int value ) { m_evType = value; }

inline long long LHCb::GenHeader::evtNumber() const { return m_evtNumber; }

inline void LHCb::GenHeader::setEvtNumber( long long value ) { m_evtNumber = value; }

inline const SmartRefVector<LHCb::GenCollision>& LHCb::GenHeader::collisions() const { return m_collisions; }

inline void LHCb::GenHeader::setCollisions( const SmartRefVector<LHCb::GenCollision>& value ) { m_collisions = value; }

inline void LHCb::GenHeader::addToCollisions( const SmartRef<LHCb::GenCollision>& value ) {
  m_collisions.push_back( value );
}

inline void LHCb::GenHeader::addToCollisions( const LHCb::GenCollision* value ) { m_collisions.push_back( value ); }

inline void LHCb::GenHeader::removeFromCollisions( const SmartRef<LHCb::GenCollision>& value ) {
  auto i = std::remove( m_collisions.begin(), m_collisions.end(), value );
  m_collisions.erase( i, m_collisions.end() );
}

inline void LHCb::GenHeader::clearCollisions() { m_collisions.clear(); }

inline const LHCb::BeamParameters* LHCb::GenHeader::beamParameters() const { return m_beamParameters; }

inline void LHCb::GenHeader::setBeamParameters( const SmartRef<LHCb::BeamParameters>& value ) {
  m_beamParameters = value;
}

inline void LHCb::GenHeader::setBeamParameters( const LHCb::BeamParameters* value ) { m_beamParameters = value; }

inline unsigned int LHCb::GenHeader::numOfCollisions() const { return collisions().size(); }

inline double LHCb::GenHeader::luminosity() const {

  return ( '2' != version() ? m_luminosity : beamParameters() ? beamParameters()->luminosity() : m_luminosity );
}

inline double LHCb::GenHeader::totCrossSection() const {

  return ( '2' != version() ? m_totCrossSection
                            : beamParameters() ? beamParameters()->totalXSec() : m_totCrossSection );
}

inline double LHCb::GenHeader::crossingFreq() const {

  return ( '2' != version() ? m_crossingFreq
                            : beamParameters() ? beamParameters()->revolutionFrequency() : m_crossingFreq );
}
