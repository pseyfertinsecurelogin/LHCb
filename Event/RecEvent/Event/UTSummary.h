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
#include <utility>
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
               unsigned int nPed, unsigned int nError, std::vector<unsigned int> corruptedBanks,
               std::vector<unsigned int> missing, RecoveredInfo recoveredBanks )
        : m_nClusters( nClusters )
        , m_pcn( pcn )
        , m_pcnSynch( pcnSynch )
        , m_rawBufferSize( nBytes )
        , m_nFullBanks( nFull )
        , m_nPedestalBanks( nPed )
        , m_nErrorBanks( nError )
        , m_corruptedBanks( std::move( corruptedBanks ) )
        , m_missingBanks( std::move( missing ) )
        , m_recoveredBanks( std::move( recoveredBanks ) ) {}

    /// Default Constructor
    UTSummary() = default;

    // Retrieve pointer to class definition structure
    [[nodiscard]] const CLID& clID() const override { return LHCb::UTSummary::classID(); }
    static const CLID&        classID() { return CLID_UTSummary; }

    /// data is good or not
    [[nodiscard]] bool hasError() const { return !m_pcnSynch || !m_corruptedBanks.empty(); }

    /// Print in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  number of clusters
    [[nodiscard]] unsigned int nClusters() const { return m_nClusters; }

    /// Retrieve const  pcn
    [[nodiscard]] unsigned int pcn() const { return m_pcn; }

    /// Retrieve const  pcn agree
    [[nodiscard]] bool pcnSynch() const { return m_pcnSynch; }

    /// Retrieve const  data size of raw bank [bytes]
    [[nodiscard]] unsigned int rawBufferSize() const { return m_rawBufferSize; }

    /// Retrieve const  # full banks
    [[nodiscard]] unsigned int nFullBanks() const { return m_nFullBanks; }

    /// Retrieve const  # pedestal banks
    [[nodiscard]] unsigned int nPedestalBanks() const { return m_nPedestalBanks; }

    /// Retrieve const  # of error banks
    [[nodiscard]] unsigned int nErrorBanks() const { return m_nErrorBanks; }

    /// Retrieve const  banks with error
    [[nodiscard]] const std::vector<unsigned int>& corruptedBanks() const { return m_corruptedBanks; }

    /// Retrieve const  banks missed
    [[nodiscard]] const std::vector<unsigned int>& missingBanks() const { return m_missingBanks; }

    /// Retrieve const  banks recovered
    [[nodiscard]] const RecoveredInfo& recoveredBanks() const { return m_recoveredBanks; }

    friend std::ostream& operator<<( std::ostream& str, const UTSummary& obj ) { return obj.fillStream( str ); }

  private:
    unsigned int              m_nClusters{0};      ///< number of clusters
    unsigned int              m_pcn{0};            ///< pcn
    bool                      m_pcnSynch{};        ///< pcn agree
    unsigned int              m_rawBufferSize{0};  ///< data size of raw bank [bytes]
    unsigned int              m_nFullBanks{0};     ///< # full banks
    unsigned int              m_nPedestalBanks{0}; ///< # pedestal banks
    unsigned int              m_nErrorBanks{0};    ///< # of error banks
    std::vector<unsigned int> m_corruptedBanks;    ///< banks with error
    std::vector<unsigned int> m_missingBanks;      ///< banks missed
    RecoveredInfo             m_recoveredBanks;    ///< banks recovered

  }; // class UTSummary

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------
