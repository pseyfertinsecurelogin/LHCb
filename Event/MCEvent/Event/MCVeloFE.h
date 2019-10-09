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
#include "Event/MCHit.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include "Kernel/VeloChannelID.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_MCVeloFE = 4002;

  // Namespace for locations in TDS
  namespace MCVeloFELocation {
    inline const std::string Default = "MC/Velo/FE";
    inline const std::string PuVeto  = "MC/Velo/PuFE";
  } // namespace MCVeloFELocation

  /** @class MCVeloFE MCVeloFE.h
   *
   * This class represents the output of the front-end chip and contains
   * information about the simulation of the silicon and FE chip response stored
   * for a single silicon/output channel. Constructed from MCHits (i.e. GEANT
   * deposits), MCVeloFE of previous events (spill over), noise (above a
   * threshold)
   *
   * @author Chris Parkes, update Tomasz Szumlak
   *
   */

  class MCVeloFE : public KeyedObject<LHCb::VeloChannelID> {
  public:
    /// typedef for KeyedContainer of MCVeloFE
    typedef KeyedContainer<MCVeloFE, Containers::HashMap> Container;

    /// Constructor with key
    MCVeloFE( const LHCb::VeloChannelID& key )
        : KeyedObject<LHCb::VeloChannelID>( key )
        , m_addedSignal( 0.0 )
        , m_addedPedestal( 0.0 )
        , m_addedNoise( 0.0 )
        , m_addedCMNoise( 0.0 ) {}

    /// Default Constructor
    MCVeloFE()
        : m_addedSignal( 0.0 ), m_addedPedestal( 0.0 ), m_addedNoise( 0.0 ), m_addedCMNoise( 0.0 ), m_MCHitsCharge() {}

    /// Default Destructor
    virtual ~MCVeloFE() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// combined sensor and strip number ID
    VeloChannelID channelID() const;

    /// Sensor Number
    long sensor() const;

    /// Strip Number
    long strip() const;

    /// add MC hit and its deposited charge
    void addToMCHits( const SmartRef<LHCb::MCHit>& hit, double chg );

    /// retrieve MCHit reference
    SmartRef<LHCb::MCHit> mcHit( long num ) const;

    /// retrieve deposited charge of MCHit
    double mcHitCharge( long num ) const;

    /// set deposited charge of MCHit
    void setMCHitCharge( long num, double chg );

    /// number of strips in cluster
    long NumberOfMCHits() const;

    /// Access total charge - in electrons
    double charge() const;

    /// Retrieve const  Added Signal in units of electrons
    double addedSignal() const;

    /// Update  Added Signal in units of electrons
    void setAddedSignal( double value );

    /// Retrieve const  pedestal added in Si/FE chip simulation in units of electrons
    double addedPedestal() const;

    /// Update  pedestal added in Si/FE chip simulation in units of electrons
    void setAddedPedestal( double value );

    /// Retrieve const  noise added in Si/FE chip simulation in units of electrons
    double addedNoise() const;

    /// Update  noise added in Si/FE chip simulation in units of electrons
    void setAddedNoise( double value );

    /// Retrieve const  common mode noise added in Si/FE chip simulation in units of electrons
    double addedCMNoise() const;

    /// Update  common mode noise added in Si/FE chip simulation in units of electrons
    void setAddedCMNoise( double value );

    /// Retrieve const  Charge of Monte Carlo hit contributed to this channel
    const std::vector<double>& mcHitsCharge() const;

    /// Update  Charge of Monte Carlo hit contributed to this channel
    void setMCHitsCharge( const std::vector<double>& value );

    /// Retrieve (const)  References to Monte Carlo hits contributing to this channel
    const SmartRefVector<LHCb::MCHit>& mcHits() const;

    /// Update  References to Monte Carlo hits contributing to this channel
    void setMCHits( const SmartRefVector<LHCb::MCHit>& value );

    /// Add to  References to Monte Carlo hits contributing to this channel
    void addToMCHits( const SmartRef<LHCb::MCHit>& value );

    /// Att to (pointer)  References to Monte Carlo hits contributing to this channel
    void addToMCHits( const LHCb::MCHit* value );

    /// Remove from  References to Monte Carlo hits contributing to this channel
    void removeFromMCHits( const SmartRef<LHCb::MCHit>& value );

    /// Clear  References to Monte Carlo hits contributing to this channel
    void clearMCHits();

    friend std::ostream& operator<<( std::ostream& str, const MCVeloFE& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    double              m_addedSignal;    ///< Added Signal in units of electrons
    double              m_addedPedestal;  ///< pedestal added in Si/FE chip simulation in units of electrons
    double              m_addedNoise;     ///< noise added in Si/FE chip simulation in units of electrons
    double              m_addedCMNoise;   ///< common mode noise added in Si/FE chip simulation in units of electrons
    std::vector<double> m_MCHitsCharge;   ///< Charge of Monte Carlo hit contributed to this channel
    SmartRefVector<LHCb::MCHit> m_MCHits; ///< References to Monte Carlo hits contributing to this channel

  }; // class MCVeloFE

  /// Definition of Keyed Container for MCVeloFE
  typedef KeyedContainer<MCVeloFE, Containers::HashMap> MCVeloFEs;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCVeloFE::clID() const { return LHCb::MCVeloFE::classID(); }

inline const CLID& LHCb::MCVeloFE::classID() { return CLID_MCVeloFE; }

inline std::ostream& LHCb::MCVeloFE::fillStream( std::ostream& s ) const {
  s << "{ "
    << "addedSignal :	" << (float)m_addedSignal << std::endl
    << "addedPedestal :	" << (float)m_addedPedestal << std::endl
    << "addedNoise :	" << (float)m_addedNoise << std::endl
    << "addedCMNoise :	" << (float)m_addedCMNoise << std::endl
    << "MCHitsCharge :	" << m_MCHitsCharge << std::endl
    << " }";
  return s;
}

inline double LHCb::MCVeloFE::addedSignal() const { return m_addedSignal; }

inline void LHCb::MCVeloFE::setAddedSignal( double value ) { m_addedSignal = value; }

inline double LHCb::MCVeloFE::addedPedestal() const { return m_addedPedestal; }

inline void LHCb::MCVeloFE::setAddedPedestal( double value ) { m_addedPedestal = value; }

inline double LHCb::MCVeloFE::addedNoise() const { return m_addedNoise; }

inline void LHCb::MCVeloFE::setAddedNoise( double value ) { m_addedNoise = value; }

inline double LHCb::MCVeloFE::addedCMNoise() const { return m_addedCMNoise; }

inline void LHCb::MCVeloFE::setAddedCMNoise( double value ) { m_addedCMNoise = value; }

inline const std::vector<double>& LHCb::MCVeloFE::mcHitsCharge() const { return m_MCHitsCharge; }

inline void LHCb::MCVeloFE::setMCHitsCharge( const std::vector<double>& value ) { m_MCHitsCharge = value; }

inline const SmartRefVector<LHCb::MCHit>& LHCb::MCVeloFE::mcHits() const { return m_MCHits; }

inline void LHCb::MCVeloFE::setMCHits( const SmartRefVector<LHCb::MCHit>& value ) { m_MCHits = value; }

inline void LHCb::MCVeloFE::addToMCHits( const SmartRef<LHCb::MCHit>& value ) { m_MCHits.push_back( value ); }

inline void LHCb::MCVeloFE::addToMCHits( const LHCb::MCHit* value ) { m_MCHits.push_back( value ); }

inline void LHCb::MCVeloFE::removeFromMCHits( const SmartRef<LHCb::MCHit>& value ) {
  auto i = std::remove( m_MCHits.begin(), m_MCHits.end(), value );
  m_MCHits.erase( i, m_MCHits.end() );
}

inline void LHCb::MCVeloFE::clearMCHits() { m_MCHits.clear(); }

inline LHCb::VeloChannelID LHCb::MCVeloFE::channelID() const { return key(); }

inline long LHCb::MCVeloFE::sensor() const { return key().sensor(); }

inline long LHCb::MCVeloFE::strip() const { return key().strip(); }

inline void LHCb::MCVeloFE::addToMCHits( const SmartRef<LHCb::MCHit>& hit, double chg ) {

  m_MCHits.push_back( hit );
  m_MCHitsCharge.push_back( chg );
  return;
}

inline SmartRef<LHCb::MCHit> LHCb::MCVeloFE::mcHit( long num ) const { return m_MCHits[num]; }

inline double LHCb::MCVeloFE::mcHitCharge( long num ) const { return m_MCHitsCharge[num]; }

inline void LHCb::MCVeloFE::setMCHitCharge( long num, double chg ) {

  m_MCHitsCharge[num] = chg;
  return;
}

inline long LHCb::MCVeloFE::NumberOfMCHits() const { return mcHits().size(); }

inline double LHCb::MCVeloFE::charge() const { return addedSignal() + addedNoise() + addedPedestal() + addedCMNoise(); }
