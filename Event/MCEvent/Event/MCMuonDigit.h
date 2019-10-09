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
#include "Event/MCMuonDigitInfo.h"
#include "Event/MCMuonHitHistory.h"
#include "Event/PackMCMuonDigitInfo.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include "GaudiKernel/SmartRefVector.h"
#include "Kernel/MuonTileID.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_MCMuonDigit = 11022;

  // Namespace for locations in TDS
  namespace MCMuonDigitLocation {
    inline const std::string MCMuonDigit = "MC/Muon/Digits";
  }

  /** @class MCMuonDigit MCMuonDigit.h
   *
   * This class permits to trace back the history of a logical channel: the
   * origin of the hit that has fired it or the reason why the digit was not
   * fired (inefficiency, time resolution, deadtime etc.). In the latter case the
   * correspondig MuonDigit does not exist.
   *
   * @author Alessia Satta
   *
   */

  class MCMuonDigit : public KeyedObject<LHCb::MuonTileID> {
  public:
    /// typedef for KeyedContainer of MCMuonDigit
    typedef KeyedContainer<MCMuonDigit, Containers::HashMap> Container;

    /// description of constructor
    MCMuonDigit( const LHCb::MuonTileID& tileID );

    /// Default Constructor
    MCMuonDigit() : m_DigitInfo(), m_firingTime( 0.0 ), m_HitsHistory() {}

    /// Default Destructor
    virtual ~MCMuonDigit() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// return digitinfo
    MCMuonDigitInfo& DigitInfo();

    /// return hithistory vector
    std::vector<LHCb::MCMuonHitHistory>& HitsHistory();

    /// Update  Packed information of the origin of the hit generating the digit,  the earliest, and the fate of the
    /// digit
    void setDigitInfo( const LHCb::MCMuonDigitInfo& value );

    /// Retrieve const  Firing Time of the hit
    double firingTime() const;

    /// Update  Firing Time of the hit
    void setFiringTime( double value );

    /// Retrieve (const)  Stored GEANT hits connected to the digit.
    const SmartRefVector<LHCb::MCHit>& mcHits() const;

    /// Update  Stored GEANT hits connected to the digit.
    void setMCHits( const SmartRefVector<LHCb::MCHit>& value );

    /// Add to  Stored GEANT hits connected to the digit.
    void addToMCHits( const SmartRef<LHCb::MCHit>& value );

    /// Att to (pointer)  Stored GEANT hits connected to the digit.
    void addToMCHits( const LHCb::MCHit* value );

    /// Remove from  Stored GEANT hits connected to the digit.
    void removeFromMCHits( const SmartRef<LHCb::MCHit>& value );

    /// Clear  Stored GEANT hits connected to the digit.
    void clearMCHits();

    friend std::ostream& operator<<( std::ostream& str, const MCMuonDigit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    LHCb::MCMuonDigitInfo m_DigitInfo; ///< Packed information of the origin of the hit generating the digit,  the
                                       ///< earliest, and the fate of the digit
    double                              m_firingTime;  ///< Firing Time of the hit
    std::vector<LHCb::MCMuonHitHistory> m_HitsHistory; ///< bit pattern that stores the history information of each hit
                                                       ///< taking part to the digit, one to one correspondence to the
                                                       ///< link to  MCMuonHit stored
    SmartRefVector<LHCb::MCHit> m_MCHits;              ///< Stored GEANT hits connected to the digit.

  }; // class MCMuonDigit

  /// Definition of Keyed Container for MCMuonDigit
  typedef KeyedContainer<MCMuonDigit, Containers::HashMap> MCMuonDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::MCMuonDigit::MCMuonDigit( const LHCb::MuonTileID& tileID )
    : m_DigitInfo(), m_firingTime( 0.0 ), m_HitsHistory() {
  setKey( tileID );
}

inline const CLID& LHCb::MCMuonDigit::clID() const { return LHCb::MCMuonDigit::classID(); }

inline const CLID& LHCb::MCMuonDigit::classID() { return CLID_MCMuonDigit; }

inline std::ostream& LHCb::MCMuonDigit::fillStream( std::ostream& s ) const {
  s << "{ "
    << "DigitInfo :	" << m_DigitInfo << std::endl
    << "firingTime :	" << (float)m_firingTime << std::endl
    << "HitsHistory :	" << m_HitsHistory << std::endl
    << " }";
  return s;
}

inline void LHCb::MCMuonDigit::setDigitInfo( const LHCb::MCMuonDigitInfo& value ) { m_DigitInfo = value; }

inline double LHCb::MCMuonDigit::firingTime() const { return m_firingTime; }

inline void LHCb::MCMuonDigit::setFiringTime( double value ) { m_firingTime = value; }

inline const SmartRefVector<LHCb::MCHit>& LHCb::MCMuonDigit::mcHits() const { return m_MCHits; }

inline void LHCb::MCMuonDigit::setMCHits( const SmartRefVector<LHCb::MCHit>& value ) { m_MCHits = value; }

inline void LHCb::MCMuonDigit::addToMCHits( const SmartRef<LHCb::MCHit>& value ) { m_MCHits.push_back( value ); }

inline void LHCb::MCMuonDigit::addToMCHits( const LHCb::MCHit* value ) { m_MCHits.push_back( value ); }

inline void LHCb::MCMuonDigit::removeFromMCHits( const SmartRef<LHCb::MCHit>& value ) {
  auto i = std::remove( m_MCHits.begin(), m_MCHits.end(), value );
  m_MCHits.erase( i, m_MCHits.end() );
}

inline void LHCb::MCMuonDigit::clearMCHits() { m_MCHits.clear(); }

inline LHCb::MCMuonDigitInfo& LHCb::MCMuonDigit::DigitInfo() { return m_DigitInfo; }

inline std::vector<LHCb::MCMuonHitHistory>& LHCb::MCMuonDigit::HitsHistory() { return m_HitsHistory; }
