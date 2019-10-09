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
    typedef std::vector<AlgStatus> AlgStatusVector;

    /// Copy constructor. Creates a new ProcStatus with the same information
    ProcStatus( const LHCb::ProcStatus& proc ) : DataObject(), m_aborted( proc.aborted() ), m_algs( proc.algs() ) {}

    /// Default Constructor
    ProcStatus() : m_aborted( false ), m_algs() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Add an entry for an algorithm, with sub-system, type and abort code
    void addAlgorithmStatus( const std::string& algName, const std::string& subsystem, const std::string& reason,
                             const int status, const bool eventAborted );

    /// Checks if the given sub-system triggered an abort
    bool subSystemAbort( const std::string& subsystem ) const;

    /// Print this ProcStatus in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Indicate if the processing should be aborted
    bool aborted() const;

    /// Update  Indicate if the processing should be aborted
    void setAborted( bool value );

    /// Retrieve const  List of algorithms with status
    const std::vector<std::pair<std::string, int>>& algs() const;

    /// Update  List of algorithms with status
    void setAlgs( const std::vector<std::pair<std::string, int>>& value );

    friend std::ostream& operator<<( std::ostream& str, const ProcStatus& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Add an entry for an algorithm
    void addAlgorithmStatus( const std::string& name, const int status );

    bool                                     m_aborted; ///< Indicate if the processing should be aborted
    std::vector<std::pair<std::string, int>> m_algs;    ///< List of algorithms with status

  }; // class ProcStatus

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::ProcStatus::clID() const { return LHCb::ProcStatus::classID(); }

inline const CLID& LHCb::ProcStatus::classID() { return CLID_ProcStatus; }

inline bool LHCb::ProcStatus::aborted() const { return m_aborted; }

inline void LHCb::ProcStatus::setAborted( bool value ) { m_aborted = value; }

inline const std::vector<std::pair<std::string, int>>& LHCb::ProcStatus::algs() const { return m_algs; }

inline void LHCb::ProcStatus::setAlgs( const std::vector<std::pair<std::string, int>>& value ) { m_algs = value; }
