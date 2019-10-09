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
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SmartRefVector.h"
#include "Kernel/STChannelID.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class MCSTDeposit;

  // Class ID definition
  static const CLID CLID_MCSTDigit = 9002;

  // Namespace for locations in TDS
  namespace MCSTDigitLocation {
    inline const std::string TTDigits = "MC/TT/Digits";
    inline const std::string ITDigits = "MC/IT/Digits";
  } // namespace MCSTDigitLocation

  /** @class MCSTDigit MCSTDigit.h
   *
   * This class turns represents total charge deposited on strip in an event
   *
   * @author Matthew Needham
   *
   */

  class MCSTDigit : public KeyedObject<LHCb::STChannelID> {
  public:
    /// typedef for KeyedContainer of MCSTDigit
    typedef KeyedContainer<MCSTDigit, Containers::HashMap> Container;

    /// Default Constructor
    MCSTDigit() {}

    /// Default Destructor
    virtual ~MCSTDigit() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// channel = key
    STChannelID channelID() const;

    /// Retrieve (const)  vector of MCSTDeposits
    const SmartRefVector<LHCb::MCSTDeposit>& mcDeposit() const;

    /// Update  vector of MCSTDeposits
    void setMcDeposit( const SmartRefVector<LHCb::MCSTDeposit>& value );

    /// Add to  vector of MCSTDeposits
    void addToMcDeposit( const SmartRef<LHCb::MCSTDeposit>& value );

    /// Att to (pointer)  vector of MCSTDeposits
    void addToMcDeposit( const LHCb::MCSTDeposit* value );

    /// Remove from  vector of MCSTDeposits
    void removeFromMcDeposit( const SmartRef<LHCb::MCSTDeposit>& value );

    /// Clear  vector of MCSTDeposits
    void clearMcDeposit();

    friend std::ostream& operator<<( std::ostream& str, const MCSTDigit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    SmartRefVector<LHCb::MCSTDeposit> m_mcDeposit; ///< vector of MCSTDeposits

  }; // class MCSTDigit

  /// Definition of Keyed Container for MCSTDigit
  typedef KeyedContainer<MCSTDigit, Containers::HashMap> MCSTDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations
#include "MCSTDeposit.h"

inline const CLID& LHCb::MCSTDigit::clID() const { return LHCb::MCSTDigit::classID(); }

inline const CLID& LHCb::MCSTDigit::classID() { return CLID_MCSTDigit; }

inline std::ostream& LHCb::MCSTDigit::fillStream( std::ostream& s ) const { return s; }

inline const SmartRefVector<LHCb::MCSTDeposit>& LHCb::MCSTDigit::mcDeposit() const { return m_mcDeposit; }

inline void LHCb::MCSTDigit::setMcDeposit( const SmartRefVector<LHCb::MCSTDeposit>& value ) { m_mcDeposit = value; }

inline void LHCb::MCSTDigit::addToMcDeposit( const SmartRef<LHCb::MCSTDeposit>& value ) {
  m_mcDeposit.push_back( value );
}

inline void LHCb::MCSTDigit::addToMcDeposit( const LHCb::MCSTDeposit* value ) { m_mcDeposit.push_back( value ); }

inline void LHCb::MCSTDigit::removeFromMcDeposit( const SmartRef<LHCb::MCSTDeposit>& value ) {
  auto i = std::remove( m_mcDeposit.begin(), m_mcDeposit.end(), value );
  m_mcDeposit.erase( i, m_mcDeposit.end() );
}

inline void LHCb::MCSTDigit::clearMcDeposit() { m_mcDeposit.clear(); }

inline LHCb::STChannelID LHCb::MCSTDigit::channelID() const { return key(); }
