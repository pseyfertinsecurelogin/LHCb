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
#include "GaudiKernel/SmartRef.h"
#include "Kernel/STChannelID.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MCSTDeposit = 9001;

  // Namespace for locations in TDS
  namespace MCSTDepositLocation {
    inline const std::string TTDeposits = "MC/TT/Deposits";
    inline const std::string ITDeposits = "MC/IT/Deposits";
  } // namespace MCSTDepositLocation

  /** @class MCSTDeposit MCSTDeposit.h
   *
   * This class turns represents charge deposited on a strip by a single MCHit
   *
   * @author Matthew Needham
   *
   */

  class MCSTDeposit : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of MCSTDeposit
    typedef KeyedContainer<MCSTDeposit, Containers::HashMap> Container;

    /// special constructor
    MCSTDeposit( double charge, const LHCb::STChannelID& aChan, LHCb::MCHit* aHit )
        : m_depositedCharge( charge ), m_channelID( aChan ), m_mcHit( aHit ) {}

    /// Default Constructor
    MCSTDeposit() = default;

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  charge deposited on strip
    double depositedCharge() const;

    /// Retrieve const  channelID
    const LHCb::STChannelID& channelID() const;

    /// Retrieve (const)  pointer to MCHit
    const LHCb::MCHit* mcHit() const;

    /// Update  pointer to MCHit
    void setMcHit( const SmartRef<LHCb::MCHit>& value );

    /// Update (pointer)  pointer to MCHit
    void setMcHit( const LHCb::MCHit* value );

    friend std::ostream& operator<<( std::ostream& str, const MCSTDeposit& obj ) { return obj.fillStream( str ); }

  private:
    double                m_depositedCharge{0.0}; ///< charge deposited on strip
    LHCb::STChannelID     m_channelID;            ///< channelID
    SmartRef<LHCb::MCHit> m_mcHit;                ///< pointer to MCHit

  }; // class MCSTDeposit

  /// Definition of Keyed Container for MCSTDeposit
  typedef KeyedContainer<MCSTDeposit, Containers::HashMap> MCSTDeposits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCSTDeposit::clID() const { return LHCb::MCSTDeposit::classID(); }

inline const CLID& LHCb::MCSTDeposit::classID() { return CLID_MCSTDeposit; }

inline std::ostream& LHCb::MCSTDeposit::fillStream( std::ostream& s ) const {
  s << "{ "
    << "depositedCharge :	" << (float)m_depositedCharge << std::endl
    << "channelID :	" << m_channelID << std::endl
    << " }";
  return s;
}

inline double LHCb::MCSTDeposit::depositedCharge() const { return m_depositedCharge; }

inline const LHCb::STChannelID& LHCb::MCSTDeposit::channelID() const { return m_channelID; }

inline const LHCb::MCHit* LHCb::MCSTDeposit::mcHit() const { return m_mcHit; }

inline void LHCb::MCSTDeposit::setMcHit( const SmartRef<LHCb::MCHit>& value ) { m_mcHit = value; }

inline void LHCb::MCSTDeposit::setMcHit( const LHCb::MCHit* value ) { m_mcHit = value; }
