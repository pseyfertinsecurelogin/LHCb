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
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SerializeSTL.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichRadiatorType.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class MCRichTrack;
  class MCRichHit;
  class MCRichOpticalPhoton;
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_MCRichSegment = 12414;

  // Namespace for locations in TDS
  namespace MCRichSegmentLocation {
    inline const std::string Default = "MC/Rich/Segments";
  }

  /** @class MCRichSegment MCRichSegment.h
   *
   * The MC trajectory through a given radiator medium
   *
   * @author Christopher Jones    Christopher.Rob.Jones@cern.ch
   *
   */

  class MCRichSegment : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of MCRichSegment
    typedef KeyedContainer<MCRichSegment, Containers::HashMap> Container;

    /// Default Constructor
    MCRichSegment() : m_historyCode( 0 ), m_trajectoryPoints(), m_trajectoryMomenta() {}

    /// Default Destructor
    virtual ~MCRichSegment() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Returns the entry point to the radiator medium
    const Gaudi::XYZPoint& entryPoint() const;

    /// Returns the exit point from the radiator medium
    const Gaudi::XYZPoint& exitPoint() const;

    /// Returns the best position estimate for a given fractional position along the trajectory through the radiator
    /// medium
    Gaudi::XYZPoint bestPoint( const double fraction ) const;

    /// Returns the best momentum estimate for a given fractional position along the trajectory through the radiator
    /// medium
    Gaudi::XYZVector bestMomentum( const double fraction ) const;

    /// Returns the momentum vector at the entry point to the radiator medium
    const Gaudi::XYZVector& entryMomentum() const;

    /// Returns the momentum vector at the exit point to the radiator medium
    const Gaudi::XYZVector& exitMomentum() const;

    /// Returns the average segment momentum vector based on the given number of samples equally spread along the
    /// trajectory
    Gaudi::XYZVector averageMomentum( const unsigned int nsamples ) const;

    ///  Returns the path length of the trajectory through the radiator
    double pathLength() const;

    /// Add given point to vector of trajectory points
    void addToTrajectoryPoints( const Gaudi::XYZPoint& point );

    /// Add given Momentum vector to vector of trajectory points
    void addToTrajectoryMomenta( const Gaudi::XYZVector& momentum );

    /// The RICH detector
    Rich::DetectorType rich() const;

    /// set the RICH type
    void setRich( const Rich::DetectorType value );

    /// If Cherenkov radiation, the radiator medium
    Rich::RadiatorType radiator() const;

    /// set the radiator type
    void setRadiator( const Rich::RadiatorType value );

    /// Retrieve const  Bit packed field containing detector information
    unsigned int historyCode() const;

    /// Update  Bit packed field containing detector information
    void setHistoryCode( unsigned int value );

    /// Retrieve const  Vector of trajectory points through the radiator for this segment
    const std::vector<Gaudi::XYZPoint>& trajectoryPoints() const;

    /// Update  Vector of trajectory points through the radiator for this segment
    void setTrajectoryPoints( const std::vector<Gaudi::XYZPoint>& value );

    /// Retrieve const  The momentum vector of the particle for each trajectory step
    const std::vector<Gaudi::XYZVector>& trajectoryMomenta() const;

    /// Update  The momentum vector of the particle for each trajectory step
    void setTrajectoryMomenta( const std::vector<Gaudi::XYZVector>& value );

    /// Retrieve (const)  Associated MCParticle
    const LHCb::MCParticle* mcParticle() const;

    /// Update  Associated MCParticle
    void setMcParticle( const SmartRef<LHCb::MCParticle>& value );

    /// Update (pointer)  Associated MCParticle
    void setMcParticle( const LHCb::MCParticle* value );

    /// Retrieve (const)  Associated MCRichTrack
    const LHCb::MCRichTrack* mcRichTrack() const;

    /// Update  Associated MCRichTrack
    void setMCRichTrack( const SmartRef<LHCb::MCRichTrack>& value );

    /// Update (pointer)  Associated MCRichTrack
    void setMCRichTrack( const LHCb::MCRichTrack* value );

    /// Retrieve (const)  Associated MCRichOpticalPhotons
    const SmartRefVector<LHCb::MCRichOpticalPhoton>& mcRichOpticalPhotons() const;

    /// Update  Associated MCRichOpticalPhotons
    void setMCRichOpticalPhotons( const SmartRefVector<LHCb::MCRichOpticalPhoton>& value );

    /// Add to  Associated MCRichOpticalPhotons
    void addToMCRichOpticalPhotons( const SmartRef<LHCb::MCRichOpticalPhoton>& value );

    /// Att to (pointer)  Associated MCRichOpticalPhotons
    void addToMCRichOpticalPhotons( const LHCb::MCRichOpticalPhoton* value );

    /// Remove from  Associated MCRichOpticalPhotons
    void removeFromMCRichOpticalPhotons( const SmartRef<LHCb::MCRichOpticalPhoton>& value );

    /// Clear  Associated MCRichOpticalPhotons
    void clearMCRichOpticalPhotons();

    /// Retrieve (const)  Associated MCRichHits
    const SmartRefVector<LHCb::MCRichHit>& mcRichHits() const;

    /// Update  Associated MCRichHits
    void setMCRichHits( const SmartRefVector<LHCb::MCRichHit>& value );

    /// Add to  Associated MCRichHits
    void addToMCRichHits( const SmartRef<LHCb::MCRichHit>& value );

    /// Att to (pointer)  Associated MCRichHits
    void addToMCRichHits( const LHCb::MCRichHit* value );

    /// Remove from  Associated MCRichHits
    void removeFromMCRichHits( const SmartRef<LHCb::MCRichHit>& value );

    /// Clear  Associated MCRichHits
    void clearMCRichHits();

    friend std::ostream& operator<<( std::ostream& str, const MCRichSegment& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Offsets of bitfield historyCode
    enum historyCodeBits { richBits = 0, radiatorBits = 2 };

    /// Bitmasks for bitfield historyCode
    enum historyCodeMasks { richMask = 0x3L, radiatorMask = 0x7cL };

    unsigned int                 m_historyCode;        ///< Bit packed field containing detector information
    std::vector<Gaudi::XYZPoint> m_trajectoryPoints;   ///< Vector of trajectory points through the radiator for this
                                                       ///< segment
    std::vector<Gaudi::XYZVector> m_trajectoryMomenta; ///< The momentum vector of the particle for each trajectory step
    SmartRef<LHCb::MCParticle>    m_mcParticle;        ///< Associated MCParticle
    SmartRef<LHCb::MCRichTrack>   m_MCRichTrack;       ///< Associated MCRichTrack
    SmartRefVector<LHCb::MCRichOpticalPhoton> m_MCRichOpticalPhotons; ///< Associated MCRichOpticalPhotons
    SmartRefVector<LHCb::MCRichHit>           m_MCRichHits;           ///< Associated MCRichHits

  }; // class MCRichSegment

  /// Definition of Keyed Container for MCRichSegment
  typedef KeyedContainer<MCRichSegment, Containers::HashMap> MCRichSegments;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations
#include "Event/MCRichHit.h"
#include "Event/MCRichOpticalPhoton.h"
#include "Event/MCRichTrack.h"

inline const CLID& LHCb::MCRichSegment::clID() const { return LHCb::MCRichSegment::classID(); }

inline const CLID& LHCb::MCRichSegment::classID() { return CLID_MCRichSegment; }

inline std::ostream& LHCb::MCRichSegment::fillStream( std::ostream& s ) const {
  s << "{ "
    << "historyCode :	" << m_historyCode << std::endl
    << "trajectoryPoints :	" << m_trajectoryPoints << std::endl
    << "trajectoryMomenta :	" << m_trajectoryMomenta << std::endl
    << " }";
  return s;
}

inline unsigned int LHCb::MCRichSegment::historyCode() const { return m_historyCode; }

inline void LHCb::MCRichSegment::setHistoryCode( unsigned int value ) { m_historyCode = value; }

inline const std::vector<Gaudi::XYZPoint>& LHCb::MCRichSegment::trajectoryPoints() const { return m_trajectoryPoints; }

inline void LHCb::MCRichSegment::setTrajectoryPoints( const std::vector<Gaudi::XYZPoint>& value ) {
  m_trajectoryPoints = value;
}

inline const std::vector<Gaudi::XYZVector>& LHCb::MCRichSegment::trajectoryMomenta() const {
  return m_trajectoryMomenta;
}

inline void LHCb::MCRichSegment::setTrajectoryMomenta( const std::vector<Gaudi::XYZVector>& value ) {
  m_trajectoryMomenta = value;
}

inline const LHCb::MCParticle* LHCb::MCRichSegment::mcParticle() const { return m_mcParticle; }

inline void LHCb::MCRichSegment::setMcParticle( const SmartRef<LHCb::MCParticle>& value ) { m_mcParticle = value; }

inline void LHCb::MCRichSegment::setMcParticle( const LHCb::MCParticle* value ) { m_mcParticle = value; }

inline const LHCb::MCRichTrack* LHCb::MCRichSegment::mcRichTrack() const { return m_MCRichTrack; }

inline void LHCb::MCRichSegment::setMCRichTrack( const SmartRef<LHCb::MCRichTrack>& value ) { m_MCRichTrack = value; }

inline void LHCb::MCRichSegment::setMCRichTrack( const LHCb::MCRichTrack* value ) { m_MCRichTrack = value; }

inline const SmartRefVector<LHCb::MCRichOpticalPhoton>& LHCb::MCRichSegment::mcRichOpticalPhotons() const {
  return m_MCRichOpticalPhotons;
}

inline void LHCb::MCRichSegment::setMCRichOpticalPhotons( const SmartRefVector<LHCb::MCRichOpticalPhoton>& value ) {
  m_MCRichOpticalPhotons = value;
}

inline void LHCb::MCRichSegment::addToMCRichOpticalPhotons( const SmartRef<LHCb::MCRichOpticalPhoton>& value ) {
  m_MCRichOpticalPhotons.push_back( value );
}

inline void LHCb::MCRichSegment::addToMCRichOpticalPhotons( const LHCb::MCRichOpticalPhoton* value ) {
  m_MCRichOpticalPhotons.push_back( value );
}

inline void LHCb::MCRichSegment::removeFromMCRichOpticalPhotons( const SmartRef<LHCb::MCRichOpticalPhoton>& value ) {
  auto i = std::remove( m_MCRichOpticalPhotons.begin(), m_MCRichOpticalPhotons.end(), value );
  m_MCRichOpticalPhotons.erase( i, m_MCRichOpticalPhotons.end() );
}

inline void LHCb::MCRichSegment::clearMCRichOpticalPhotons() { m_MCRichOpticalPhotons.clear(); }

inline const SmartRefVector<LHCb::MCRichHit>& LHCb::MCRichSegment::mcRichHits() const { return m_MCRichHits; }

inline void LHCb::MCRichSegment::setMCRichHits( const SmartRefVector<LHCb::MCRichHit>& value ) { m_MCRichHits = value; }

inline void LHCb::MCRichSegment::addToMCRichHits( const SmartRef<LHCb::MCRichHit>& value ) {
  m_MCRichHits.push_back( value );
}

inline void LHCb::MCRichSegment::addToMCRichHits( const LHCb::MCRichHit* value ) { m_MCRichHits.push_back( value ); }

inline void LHCb::MCRichSegment::removeFromMCRichHits( const SmartRef<LHCb::MCRichHit>& value ) {
  auto i = std::remove( m_MCRichHits.begin(), m_MCRichHits.end(), value );
  m_MCRichHits.erase( i, m_MCRichHits.end() );
}

inline void LHCb::MCRichSegment::clearMCRichHits() { m_MCRichHits.clear(); }

inline const Gaudi::XYZPoint& LHCb::MCRichSegment::entryPoint() const { return m_trajectoryPoints.front(); }

inline const Gaudi::XYZPoint& LHCb::MCRichSegment::exitPoint() const { return m_trajectoryPoints.back(); }

inline const Gaudi::XYZVector& LHCb::MCRichSegment::entryMomentum() const { return m_trajectoryMomenta.front(); }

inline const Gaudi::XYZVector& LHCb::MCRichSegment::exitMomentum() const { return m_trajectoryMomenta.back(); }

inline double LHCb::MCRichSegment::pathLength() const { return sqrt( ( exitPoint() - entryPoint() ).mag2() ); }

inline void LHCb::MCRichSegment::addToTrajectoryPoints( const Gaudi::XYZPoint& point ) {

  m_trajectoryPoints.push_back( point );
}

inline void LHCb::MCRichSegment::addToTrajectoryMomenta( const Gaudi::XYZVector& momentum ) {

  m_trajectoryMomenta.push_back( momentum );
}

inline Rich::DetectorType LHCb::MCRichSegment::rich() const {

  // Shift by -1 to convert packed representation to Rich::DetectorType
  return ( Rich::DetectorType )( ( ( m_historyCode & richMask ) >> richBits ) - 1 );
}

inline void LHCb::MCRichSegment::setRich( const Rich::DetectorType value ) {

  // Shift bit-packed representation by +1 to start numbering from 0
  const unsigned int val = (unsigned int)value + 1;
  m_historyCode &= ~richMask;
  m_historyCode |= ( ( ( (unsigned int)val ) << richBits ) & richMask );
}

inline Rich::RadiatorType LHCb::MCRichSegment::radiator() const {

  // Shift by -1 to convert packed representation to Rich::RadiatorType
  return ( Rich::RadiatorType )( ( ( m_historyCode & radiatorMask ) >> radiatorBits ) - 1 );
}

inline void LHCb::MCRichSegment::setRadiator( const Rich::RadiatorType value ) {

  // Shift bit-packed representation by +1 to start numbering from 0
  const unsigned int val = (unsigned int)value + 1;
  m_historyCode &= ~radiatorMask;
  m_historyCode |= ( ( ( (unsigned int)val ) << radiatorBits ) & radiatorMask );
}
