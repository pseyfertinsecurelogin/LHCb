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
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "Kernel/FTChannelID.h"

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MCFTDeposit = 14001;

  // Namespace for locations in TDS
  namespace MCFTDepositLocation {
    inline const std::string Default = "MC/FT/Deposits";
  }

  /** @class MCFTDeposit MCFTDeposit.h
   *
   * Number of photons deposited in each FTChannelID by the crossing MCHits
   *
   * @author Jeroen van Tilburg
   *
   */

  class MCFTDeposit final : public ContainedObject {
  public:
    /// typedef for ObjectVector of MCFTDeposit
    typedef ObjectVector<MCFTDeposit> Container;

    /// Constructor
    MCFTDeposit( const LHCb::FTChannelID& id, const LHCb::MCHit* aHit, int nPhotons, double time, bool isReflected )
        : m_channelID( id ), m_mcHit( aHit ), m_nPhotons( nPhotons ), m_time( time ), m_isReflected( isReflected ) {}

    /// Default Constructor
    MCFTDeposit() : m_channelID(), m_mcHit( 0 ), m_nPhotons( 0 ), m_time( 0.0 ), m_isReflected() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Comparison for sorting by channel ID
    static bool lowerByChannelID( MCFTDeposit* d1, MCFTDeposit* d2 );

    /// Retrieve const  The FTChannelID of this deposit
    const LHCb::FTChannelID& channelID() const;

    /// Retrieve const  Pointer to MCHit
    const LHCb::MCHit* mcHit() const;

    /// Retrieve const  Number of photons
    int nPhotons() const;

    /// Retrieve const  Time of the deposit
    double time() const;

    /// Retrieve const  isReflected flag
    bool isReflected() const;

  protected:
  private:
    const LHCb::FTChannelID m_channelID;   ///< The FTChannelID of this deposit
    const LHCb::MCHit*      m_mcHit;       ///< Pointer to MCHit
    int                     m_nPhotons;    ///< Number of photons
    double                  m_time;        ///< Time of the deposit
    bool                    m_isReflected; ///< isReflected flag

  }; // class MCFTDeposit

  /// Definition of vector container type for MCFTDeposit
  typedef ObjectVector<MCFTDeposit> MCFTDeposits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCFTDeposit::clID() const { return LHCb::MCFTDeposit::classID(); }

inline const CLID& LHCb::MCFTDeposit::classID() { return CLID_MCFTDeposit; }

inline const LHCb::FTChannelID& LHCb::MCFTDeposit::channelID() const { return m_channelID; }

inline const LHCb::MCHit* LHCb::MCFTDeposit::mcHit() const { return m_mcHit; }

inline int LHCb::MCFTDeposit::nPhotons() const { return m_nPhotons; }

inline double LHCb::MCFTDeposit::time() const { return m_time; }

inline bool LHCb::MCFTDeposit::isReflected() const { return m_isReflected; }

inline bool LHCb::MCFTDeposit::lowerByChannelID( MCFTDeposit* d1, MCFTDeposit* d2 ) {
  return d1->channelID() < d2->channelID();
}
