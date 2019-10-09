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

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_L0MuonInfo = 6009;

  // Namespace for locations in TDS
  namespace L0MuonInfoLocation {
    inline const std::string Default = "Trig/L0/MuonInfo";
  }

  /** @class L0MuonInfo L0MuonInfo.h
   *
   * L0Muon information * Contains info on the L0Muon trigger.
   *
   * @author J. Cogan
   *
   */

  class L0MuonInfo final : public DataObject {
  public:
    /// Constructor
    L0MuonInfo( int L0EventNumber, int L0_B_Id, int status )
        : m_L0EventNumber( L0EventNumber ), m_L0_B_Id( L0_B_Id ), m_status( status ) {}

    /// Default Constructor
    L0MuonInfo() : m_L0EventNumber( 0 ), m_L0_B_Id( 0 ), m_status( 0 ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Reference L0EventNumber used in the error detection
    int L0EventNumber() const;

    /// Update  Reference L0EventNumber used in the error detection
    void setL0EventNumber( int value );

    /// Retrieve const  Reference L0_B_Id used in the error detection
    int L0_B_Id() const;

    /// Update  Reference L0_B_Id used in the error detection
    void setL0_B_Id( int value );

    /// Retrieve const  Global status for the L0Muon
    int status() const;

    /// Update  Global status for the L0Muon
    void setStatus( int value );

    friend std::ostream& operator<<( std::ostream& str, const L0MuonInfo& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    int m_L0EventNumber; ///< Reference L0EventNumber used in the error detection
    int m_L0_B_Id;       ///< Reference L0_B_Id used in the error detection
    int m_status;        ///< Global status for the L0Muon

  }; // class L0MuonInfo

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::L0MuonInfo::clID() const { return LHCb::L0MuonInfo::classID(); }

inline const CLID& LHCb::L0MuonInfo::classID() { return CLID_L0MuonInfo; }

inline std::ostream& LHCb::L0MuonInfo::fillStream( std::ostream& s ) const {
  s << "{ "
    << "L0EventNumber :	" << m_L0EventNumber << std::endl
    << "L0_B_Id :	" << m_L0_B_Id << std::endl
    << "status :	" << m_status << std::endl
    << " }";
  return s;
}

inline int LHCb::L0MuonInfo::L0EventNumber() const { return m_L0EventNumber; }

inline void LHCb::L0MuonInfo::setL0EventNumber( int value ) { m_L0EventNumber = value; }

inline int LHCb::L0MuonInfo::L0_B_Id() const { return m_L0_B_Id; }

inline void LHCb::L0MuonInfo::setL0_B_Id( int value ) { m_L0_B_Id = value; }

inline int LHCb::L0MuonInfo::status() const { return m_status; }

inline void LHCb::L0MuonInfo::setStatus( int value ) { m_status = value; }
