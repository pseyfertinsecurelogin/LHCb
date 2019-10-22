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
#include <ostream>
#include <string>
#include <utility>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_ProcStatus = 5001;

  // Namespace for locations in TDS
  namespace ProcStatusLocation {
    inline const std::string Default = "Rec/Status";
  }

  /** @class ProcStatus ProcStatus.h
   *
   * Record the status of the Brunel processing
   *
   * @author Olivier Callot
   *
   */

  class ProcStatus final : public DataObject {
  public:
    /// Status for a given algorithm
    typedef std::pair<std::string, int> AlgStatus;
    /// Vector of AlgStatus objects
    using AlgStatusVector = std::vector<AlgStatus>;

    // Retrieve class definition structure
    static const CLID&        classID() { return CLID_ProcStatus; }
    [[nodiscard]] const CLID& clID() const override { return LHCb::ProcStatus::classID(); }

    /// Checks if the given sub-system triggered an abort
    [[nodiscard]] bool subSystemAbort( const std::string& subsystem ) const;

    /// Retrieve const  Indicate if the processing should be aborted
    [[nodiscard]] bool aborted() const { return m_aborted; }

    /// Retrieve const  List of algorithms with status
    [[nodiscard]] const std::vector<std::pair<std::string, int>>& algs() const { return m_algs; };

    /// Add an entry for an algorithm, with sub-system, type and abort code
    void addAlgorithmStatus( const std::string& algName, const std::string& subsystem, const std::string& reason,
                             int status, bool eventAborted );

    /// Update  Indicate if the processing should be aborted
    void setAborted( bool value ) { m_aborted = value; }

    /// Update  List of algorithms with status
    void setAlgs( std::vector<std::pair<std::string, int>> value ) { m_algs = std::move( value ); }

    /// Print this ProcStatus in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    friend std::ostream& operator<<( std::ostream& str, const ProcStatus& obj ) { return obj.fillStream( str ); }

  private:
    /// Add an entry for an algorithm
    void addAlgorithmStatus( const std::string& name, int status );

    bool                                     m_aborted{false}; ///< Indicate if the processing should be aborted
    std::vector<std::pair<std::string, int>> m_algs;           ///< List of algorithms with status

  }; // class ProcStatus

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------
