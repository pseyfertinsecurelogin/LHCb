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
#include "Kernel/UTChannelID.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class MCUTDeposit;

  // Class ID definition
  static const CLID CLID_MCUTDigit = 9002;

  // Namespace for locations in TDS
  namespace MCUTDigitLocation {
    inline const std::string UTDigits = "MC/UT/Digits";
  }

  /** @class MCUTDigit MCUTDigit.h
   *
   * This class turns represents total charge deposited on strip in an event
   *
   * @author Andy Beiter (based on code by Matthew Needham)
   *
   */

  class MCUTDigit : public KeyedObject<LHCb::UTChannelID> {
  public:
    /// typedef for KeyedContainer of MCUTDigit
    typedef KeyedContainer<MCUTDigit, Containers::HashMap> Container;

    /// Default Constructor
    MCUTDigit() {}

    /// Default Destructor
    virtual ~MCUTDigit() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// channel = key
    UTChannelID channelID() const;

    /// Retrieve (const)  vector of MCUTDeposits
    const SmartRefVector<LHCb::MCUTDeposit>& mcDeposit() const;

    /// Update  vector of MCUTDeposits
    void setMcDeposit( const SmartRefVector<LHCb::MCUTDeposit>& value );

    /// Add to  vector of MCUTDeposits
    void addToMcDeposit( const SmartRef<LHCb::MCUTDeposit>& value );

    /// Att to (pointer)  vector of MCUTDeposits
    void addToMcDeposit( const LHCb::MCUTDeposit* value );

    /// Remove from  vector of MCUTDeposits
    void removeFromMcDeposit( const SmartRef<LHCb::MCUTDeposit>& value );

    /// Clear  vector of MCUTDeposits
    void clearMcDeposit();

    friend std::ostream& operator<<( std::ostream& str, const MCUTDigit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    SmartRefVector<LHCb::MCUTDeposit> m_mcDeposit; ///< vector of MCUTDeposits

  }; // class MCUTDigit

  /// Definition of Keyed Container for MCUTDigit
  typedef KeyedContainer<MCUTDigit, Containers::HashMap> MCUTDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations
#include "MCUTDeposit.h"

inline const CLID& LHCb::MCUTDigit::clID() const { return LHCb::MCUTDigit::classID(); }

inline const CLID& LHCb::MCUTDigit::classID() { return CLID_MCUTDigit; }

inline std::ostream& LHCb::MCUTDigit::fillStream( std::ostream& s ) const { return s; }

inline const SmartRefVector<LHCb::MCUTDeposit>& LHCb::MCUTDigit::mcDeposit() const { return m_mcDeposit; }

inline void LHCb::MCUTDigit::setMcDeposit( const SmartRefVector<LHCb::MCUTDeposit>& value ) { m_mcDeposit = value; }

inline void LHCb::MCUTDigit::addToMcDeposit( const SmartRef<LHCb::MCUTDeposit>& value ) {
  m_mcDeposit.push_back( value );
}

inline void LHCb::MCUTDigit::addToMcDeposit( const LHCb::MCUTDeposit* value ) { m_mcDeposit.push_back( value ); }

inline void LHCb::MCUTDigit::removeFromMcDeposit( const SmartRef<LHCb::MCUTDeposit>& value ) {
  auto i = std::remove( m_mcDeposit.begin(), m_mcDeposit.end(), value );
  m_mcDeposit.erase( i, m_mcDeposit.end() );
}

inline void LHCb::MCUTDigit::clearMcDeposit() { m_mcDeposit.clear(); }

inline LHCb::UTChannelID LHCb::MCUTDigit::channelID() const { return key(); }
