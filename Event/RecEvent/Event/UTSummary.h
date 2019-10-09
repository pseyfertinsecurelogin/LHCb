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
#include "GaudiKernel/DataObject.h"
#include <map>
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_UTSummary = 9005;

  // Namespace for locations in TDS
  namespace UTSummaryLocation {
    inline const std::string UTSummary = "Rec/UT/Summary";
  }

  /** @class UTSummary UTSummary.h
   *
   * Upstream tracker summary class
   *
   * @author Andy Beiter (based on code by Matthew Needham)
   *
   */

  class UTSummary final : public DataObject {
  public:
    /// Vector of additional information
    typedef std::map<unsigned int, double> RecoveredInfo;

    /// special constructor
    UTSummary( unsigned int nClusters, unsigned int pcn, bool pcnSynch, unsigned int nBytes, unsigned int nFull,
               unsigned int nPed, unsigned int nError, const std::vector<unsigned int>& corruptedBanks,
               const std::vector<unsigned int>& missing, const RecoveredInfo& recoveredBanks )
        : m_nClusters( nClusters )
        , m_pcn( pcn )
        , m_pcnSynch( pcnSynch )
        , m_rawBufferSize( nBytes )
        , m_nFullBanks( nFull )
        , m_nPedestalBanks( nPed )
        , m_nErrorBanks( nError )
        , m_corruptedBanks( corruptedBanks )
        , m_missingBanks( missing )
        , m_recoveredBanks( recoveredBanks ) {}

    /// Default Constructor
    UTSummary()
        : m_nClusters( 0 )
        , m_pcn( 0 )
        , m_pcnSynch()
        , m_rawBufferSize( 0 )
        , m_nFullBanks( 0 )
        , m_nPedestalBanks( 0 )
        , m_nErrorBanks( 0 )
        , m_corruptedBanks()
        , m_missingBanks()
        , m_recoveredBanks() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// data is good or not
    bool hasError() const;

    /// Print in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  number of clusters
    unsigned int nClusters() const;

    /// Retrieve const  pcn
    unsigned int pcn() const;

    /// Retrieve const  pcn agree
    bool pcnSynch() const;

    /// Retrieve const  data size of raw bank [bytes]
    unsigned int rawBufferSize() const;

    /// Retrieve const  # full banks
    unsigned int nFullBanks() const;

    /// Retrieve const  # pedestal banks
    unsigned int nPedestalBanks() const;

    /// Retrieve const  # of error banks
    unsigned int nErrorBanks() const;

    /// Retrieve const  banks with error
    const std::vector<unsigned int>& corruptedBanks() const;

    /// Retrieve const  banks missed
    const std::vector<unsigned int>& missingBanks() const;

    /// Retrieve const  banks recovered
    const RecoveredInfo& recoveredBanks() const;

    friend std::ostream& operator<<( std::ostream& str, const UTSummary& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    unsigned int              m_nClusters;      ///< number of clusters
    unsigned int              m_pcn;            ///< pcn
    bool                      m_pcnSynch;       ///< pcn agree
    unsigned int              m_rawBufferSize;  ///< data size of raw bank [bytes]
    unsigned int              m_nFullBanks;     ///< # full banks
    unsigned int              m_nPedestalBanks; ///< # pedestal banks
    unsigned int              m_nErrorBanks;    ///< # of error banks
    std::vector<unsigned int> m_corruptedBanks; ///< banks with error
    std::vector<unsigned int> m_missingBanks;   ///< banks missed
    RecoveredInfo             m_recoveredBanks; ///< banks recovered

  }; // class UTSummary

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::UTSummary::clID() const { return LHCb::UTSummary::classID(); }

inline const CLID& LHCb::UTSummary::classID() { return CLID_UTSummary; }

inline unsigned int LHCb::UTSummary::nClusters() const { return m_nClusters; }

inline unsigned int LHCb::UTSummary::pcn() const { return m_pcn; }

inline bool LHCb::UTSummary::pcnSynch() const { return m_pcnSynch; }

inline unsigned int LHCb::UTSummary::rawBufferSize() const { return m_rawBufferSize; }

inline unsigned int LHCb::UTSummary::nFullBanks() const { return m_nFullBanks; }

inline unsigned int LHCb::UTSummary::nPedestalBanks() const { return m_nPedestalBanks; }

inline unsigned int LHCb::UTSummary::nErrorBanks() const { return m_nErrorBanks; }

inline const std::vector<unsigned int>& LHCb::UTSummary::corruptedBanks() const { return m_corruptedBanks; }

inline const std::vector<unsigned int>& LHCb::UTSummary::missingBanks() const { return m_missingBanks; }

inline const LHCb::UTSummary::RecoveredInfo& LHCb::UTSummary::recoveredBanks() const { return m_recoveredBanks; }

inline bool LHCb::UTSummary::hasError() const {

  bool good = m_pcnSynch & m_corruptedBanks.empty();
  return !good;
}
