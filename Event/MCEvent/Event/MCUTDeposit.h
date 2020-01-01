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
#include "Kernel/UTChannelID.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MCUTDeposit = 9001;

  // Namespace for locations in TDS
  namespace MCUTDepositLocation {
    inline const std::string UTDeposits = "MC/UT/Deposits";
  }

  /** @class MCUTDeposit MCUTDeposit.h
   *
   * This class turns represents charge deposited on a strip by a single MCHit
   *
   * @author Andy Beiter (based on code by Matthew Needham)
   *
   */

  class MCUTDeposit : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of MCUTDeposit
    typedef KeyedContainer<MCUTDeposit, Containers::HashMap> Container;

    /// special constructor
    MCUTDeposit( double charge, const LHCb::UTChannelID& aChan, LHCb::MCHit* aHit )
        : m_depositedCharge( charge ), m_channelID( aChan ), m_mcHit( aHit ) {}

    /// Default Constructor
    MCUTDeposit() = default;

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  charge deposited on strip
    double depositedCharge() const;

    /// Retrieve const  channelID
    const LHCb::UTChannelID& channelID() const;

    /// Retrieve (const)  pointer to MCHit
    const LHCb::MCHit* mcHit() const;

    /// Update  pointer to MCHit
    LHCb::MCUTDeposit& setMcHit( SmartRef<LHCb::MCHit> value );

    /// Update (pointer)  pointer to MCHit
    LHCb::MCUTDeposit& setMcHit( const LHCb::MCHit* value );

    friend std::ostream& operator<<( std::ostream& str, const MCUTDeposit& obj ) { return obj.fillStream( str ); }

  private:
    double                m_depositedCharge{0.0}; ///< charge deposited on strip
    LHCb::UTChannelID     m_channelID;            ///< channelID
    SmartRef<LHCb::MCHit> m_mcHit;                ///< pointer to MCHit

  }; // class MCUTDeposit

  /// Definition of Keyed Container for MCUTDeposit
  typedef KeyedContainer<MCUTDeposit, Containers::HashMap> MCUTDeposits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCUTDeposit::clID() const { return LHCb::MCUTDeposit::classID(); }

inline const CLID& LHCb::MCUTDeposit::classID() { return CLID_MCUTDeposit; }

inline std::ostream& LHCb::MCUTDeposit::fillStream( std::ostream& s ) const {
  s << "{ "
    << "depositedCharge :	" << (float)m_depositedCharge << std::endl
    << "channelID :	" << m_channelID << std::endl
    << " }";
  return s;
}

inline double LHCb::MCUTDeposit::depositedCharge() const { return m_depositedCharge; }

inline const LHCb::UTChannelID& LHCb::MCUTDeposit::channelID() const { return m_channelID; }

inline const LHCb::MCHit* LHCb::MCUTDeposit::mcHit() const { return m_mcHit; }

inline LHCb::MCUTDeposit& LHCb::MCUTDeposit::setMcHit( SmartRef<LHCb::MCHit> value ) {
  m_mcHit = std::move( value );
  return *this;
}

inline LHCb::MCUTDeposit& LHCb::MCUTDeposit::setMcHit( const LHCb::MCHit* value ) {
  m_mcHit = value;
  return *this;
}
