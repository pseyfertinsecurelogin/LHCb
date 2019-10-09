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
#include "Event/MCCaloHit.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SmartRefVector.h"
#include "Kernel/CaloCellID.h"
#include "Kernel/CaloCellIDKeyTraits.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MCCaloDigit = 2001;

  // Namespace for locations in TDS
  namespace MCCaloDigitLocation {
    inline const std::string Spd  = "MC/Spd/Digits";
    inline const std::string Prs  = "MC/Prs/Digits";
    inline const std::string Ecal = "MC/Ecal/Digits";
    inline const std::string Hcal = "MC/Hcal/Digits";
  } // namespace MCCaloDigitLocation

  /** @class MCCaloDigit MCCaloDigit.h
   *
   * Monte Carlo history for the given cell * * * The class represents the Monte
   * Carlo (Geant) energy * deposition in the active material ("active energy")
   * of * calorimetry cell from all Monte Carlo particles * *
   *
   * @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *
   */

  class MCCaloDigit : public KeyedObject<LHCb::CaloCellID> {
  public:
    /// typedef for KeyedContainer of MCCaloDigit
    typedef KeyedContainer<MCCaloDigit, Containers::HashMap> Container;

    /// Shortcut for references to Monte Carlo Hits
    typedef SmartRefVector<LHCb::MCCaloHit> Hits;

    /// Non-default constructor
    MCCaloDigit( const LHCb::CaloCellID& id, double e, const Hits& d ) : Base( id ), m_activeE( e ), m_hits( d ) {}

    /// Default constructor
    MCCaloDigit() : Base(), m_activeE( 0 ), m_hits() {}

    /// Copy Constructor
    MCCaloDigit( const LHCb::MCCaloDigit& right )
        : Base( right.key() ), m_activeE( right.activeE() ), m_hits( right.hits() ) {}

    /// Default Destructor
    virtual ~MCCaloDigit() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve cell identifier/key @attention alias to Base::key() method!
    const LHCb::CaloCellID& cellID() const;

    /// update cell identifier/key @attention alias to Base::setKey() method!
    void setCellID( const LHCb::CaloCellID& CellID );

    /// Add the active energy
    void addActiveE( const double value );

    /// Cloning of the object ('virtual constructor')
    virtual MCCaloDigit* clone() const;

    /// Retrieve const  Monte Carlo active energy deposition in the given cell
    double activeE() const;

    /// Update  Monte Carlo active energy deposition in the given cell
    void setActiveE( double value );

    /// Retrieve (const)  References to the Monte Carlo hits
    const SmartRefVector<LHCb::MCCaloHit>& hits() const;

    /// Update  References to the Monte Carlo hits
    void setHits( const SmartRefVector<LHCb::MCCaloHit>& value );

    /// Add to  References to the Monte Carlo hits
    void addToHits( const SmartRef<LHCb::MCCaloHit>& value );

    /// Att to (pointer)  References to the Monte Carlo hits
    void addToHits( const LHCb::MCCaloHit* value );

    /// Remove from  References to the Monte Carlo hits
    void removeFromHits( const SmartRef<LHCb::MCCaloHit>& value );

    /// Clear  References to the Monte Carlo hits
    void clearHits();

    friend std::ostream& operator<<( std::ostream& str, const MCCaloDigit& obj ) { return obj.fillStream( str ); }

  protected:
    /// Shortcut for own base class
    typedef KeyedObject<LHCb::CaloCellID> Base;

  private:
    double                          m_activeE; ///< Monte Carlo active energy deposition in the given cell
    SmartRefVector<LHCb::MCCaloHit> m_hits;    ///< References to the Monte Carlo hits

  }; // class MCCaloDigit

  /// Definition of Keyed Container for MCCaloDigit
  typedef KeyedContainer<MCCaloDigit, Containers::HashMap> MCCaloDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCCaloDigit::clID() const { return LHCb::MCCaloDigit::classID(); }

inline const CLID& LHCb::MCCaloDigit::classID() { return CLID_MCCaloDigit; }

inline std::ostream& LHCb::MCCaloDigit::fillStream( std::ostream& s ) const {
  s << "{ "
    << "activeE :	" << (float)m_activeE << std::endl
    << " }";
  return s;
}

inline double LHCb::MCCaloDigit::activeE() const { return m_activeE; }

inline void LHCb::MCCaloDigit::setActiveE( double value ) { m_activeE = value; }

inline const SmartRefVector<LHCb::MCCaloHit>& LHCb::MCCaloDigit::hits() const { return m_hits; }

inline void LHCb::MCCaloDigit::setHits( const SmartRefVector<LHCb::MCCaloHit>& value ) { m_hits = value; }

inline void LHCb::MCCaloDigit::addToHits( const SmartRef<LHCb::MCCaloHit>& value ) { m_hits.push_back( value ); }

inline void LHCb::MCCaloDigit::addToHits( const LHCb::MCCaloHit* value ) { m_hits.push_back( value ); }

inline void LHCb::MCCaloDigit::removeFromHits( const SmartRef<LHCb::MCCaloHit>& value ) {
  auto i = std::remove( m_hits.begin(), m_hits.end(), value );
  m_hits.erase( i, m_hits.end() );
}

inline void LHCb::MCCaloDigit::clearHits() { m_hits.clear(); }

inline const LHCb::CaloCellID& LHCb::MCCaloDigit::cellID() const { return key(); }

inline void LHCb::MCCaloDigit::setCellID( const LHCb::CaloCellID& CellID ) { setKey( CellID ); }

inline void LHCb::MCCaloDigit::addActiveE( const double value ) { m_activeE += value; }

inline LHCb::MCCaloDigit* LHCb::MCCaloDigit::clone() const { return new LHCb::MCCaloDigit( *this ); }
