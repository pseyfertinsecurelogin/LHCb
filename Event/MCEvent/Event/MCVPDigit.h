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
#include "GaudiKernel/SmartRefVector.h"
#include "Kernel/VPChannelID.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_MCVPDigit = 4502;

  // Namespace for locations in TDS
  namespace MCVPDigitLocation {
    inline const std::string Default = "MC/VP/Digits";
  }

  /** @class MCVPDigit MCVPDigit.h
   *
   * Charge deposits in a given pixel
   *
   * @author Marcin Kucharczyk
   *
   */

  class MCVPDigit : public KeyedObject<LHCb::VPChannelID> {
  public:
    /// typedef for KeyedContainer of MCVPDigit
    typedef KeyedContainer<MCVPDigit, Containers::HashMap> Container;

    /// Default Constructor
    MCVPDigit() : m_deposits() {}

    /// Default Destructor
    virtual ~MCVPDigit() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Return channel = key
    VPChannelID channelID() const;

    /// Add MCHit and charge deposit
    void addToMcHits( const SmartRef<LHCb::MCHit>& hit, const double deposit );

    /// Retrieve const  charge deposits of contributing MCHits
    const std::vector<double>& deposits() const;

    /// Update  charge deposits of contributing MCHits
    void setDeposits( const std::vector<double>& value );

    /// Retrieve (const)  references to contributing MCHits
    const SmartRefVector<LHCb::MCHit>& mcHits() const;

    /// Update  references to contributing MCHits
    void setMcHits( const SmartRefVector<LHCb::MCHit>& value );

    /// Add to  references to contributing MCHits
    void addToMcHits( const SmartRef<LHCb::MCHit>& value );

    /// Att to (pointer)  references to contributing MCHits
    void addToMcHits( const LHCb::MCHit* value );

    /// Remove from  references to contributing MCHits
    void removeFromMcHits( const SmartRef<LHCb::MCHit>& value );

    /// Clear  references to contributing MCHits
    void clearMcHits();

    friend std::ostream& operator<<( std::ostream& str, const MCVPDigit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    std::vector<double>         m_deposits; ///< charge deposits of contributing MCHits
    SmartRefVector<LHCb::MCHit> m_mcHits;   ///< references to contributing MCHits

  }; // class MCVPDigit

  /// Definition of Keyed Container for MCVPDigit
  typedef KeyedContainer<MCVPDigit, Containers::HashMap> MCVPDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCVPDigit::clID() const { return LHCb::MCVPDigit::classID(); }

inline const CLID& LHCb::MCVPDigit::classID() { return CLID_MCVPDigit; }

inline std::ostream& LHCb::MCVPDigit::fillStream( std::ostream& s ) const {
  s << "{ "
    << "deposits :	" << m_deposits << std::endl
    << " }";
  return s;
}

inline const std::vector<double>& LHCb::MCVPDigit::deposits() const { return m_deposits; }

inline void LHCb::MCVPDigit::setDeposits( const std::vector<double>& value ) { m_deposits = value; }

inline const SmartRefVector<LHCb::MCHit>& LHCb::MCVPDigit::mcHits() const { return m_mcHits; }

inline void LHCb::MCVPDigit::setMcHits( const SmartRefVector<LHCb::MCHit>& value ) { m_mcHits = value; }

inline void LHCb::MCVPDigit::addToMcHits( const SmartRef<LHCb::MCHit>& value ) { m_mcHits.push_back( value ); }

inline void LHCb::MCVPDigit::addToMcHits( const LHCb::MCHit* value ) { m_mcHits.push_back( value ); }

inline void LHCb::MCVPDigit::removeFromMcHits( const SmartRef<LHCb::MCHit>& value ) {
  auto i = std::remove( m_mcHits.begin(), m_mcHits.end(), value );
  m_mcHits.erase( i, m_mcHits.end() );
}

inline void LHCb::MCVPDigit::clearMcHits() { m_mcHits.clear(); }

inline LHCb::VPChannelID LHCb::MCVPDigit::channelID() const { return key(); }

inline void LHCb::MCVPDigit::addToMcHits( const SmartRef<LHCb::MCHit>& hit, const double deposit ) {

  m_mcHits.push_back( hit );
  m_deposits.push_back( deposit );
}
