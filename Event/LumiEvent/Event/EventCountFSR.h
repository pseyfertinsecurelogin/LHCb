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
#include "GaudiKernel/VectorMap.h"
#include <algorithm>
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_EventCountFSR = 13503;

  // Namespace for locations in TDS
  namespace EventCountFSRLocation {
    inline const std::string Default = "/FileRecords/EventCountFSR";
  }

  /** @class EventCountFSR EventCountFSR.h
   *
   * Accounting class for processed events in FSR
   *
   * @author Rob Lambert
   *
   */

  class EventCountFSR final : public DataObject {
  public:
    /// FSR status, is the accounting OK
    enum StatusFlag {
      Unknown    = -3, // other enum possibilities
      UNRELIABLE = -2, // The counting in this FSR is known to be suspect
      UNCHECKED  = -1, // The counting needs to be checked
      ERROR      = 0,  // The counting in all lower FSRs doesn't compute
      VERIFIED   = 1   // The counting in all lower FSRs has been verified OK
    };

    /// Default Constructor
    EventCountFSR() : m_output( 0 ), m_input( 0 ), m_statusFlag( LHCb::EventCountFSR::StatusFlag::UNCHECKED ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// conversion of string to enum for type StatusFlag
    static LHCb::EventCountFSR::StatusFlag StatusFlagToType( const std::string& aName );

    /// conversion to string for enum type StatusFlag
    static const std::string& StatusFlagToString( int aEnum );

    /// readable printout
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Number of events this output file should contain
    long long output() const;

    /// Update  Number of events this output file should contain
    void setOutput( long long value );

    /// Retrieve const  Number of events read from all input files
    long long input() const;

    /// Update  Number of events read from all input files
    void setInput( long long value );

    /// Retrieve const  Is the FSR reliable? This also allows FSR counting to be explicitly verified in production
    const LHCb::EventCountFSR::StatusFlag& statusFlag() const;

    /// Update  Is the FSR reliable? This also allows FSR counting to be explicitly verified in production
    void setStatusFlag( const LHCb::EventCountFSR::StatusFlag& value );

    friend std::ostream& operator<<( std::ostream& str, const EventCountFSR& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    long long                       m_output;     ///< Number of events this output file should contain
    long long                       m_input;      ///< Number of events read from all input files
    LHCb::EventCountFSR::StatusFlag m_statusFlag; ///< Is the FSR reliable? This also allows FSR counting to be
                                                  ///< explicitly verified in production

    static const GaudiUtils::VectorMap<std::string, StatusFlag>& s_StatusFlagTypMap();

  }; // class EventCountFSR

  inline std::ostream& operator<<( std::ostream& s, LHCb::EventCountFSR::StatusFlag e ) {
    switch ( e ) {
    case LHCb::EventCountFSR::Unknown:
      return s << "Unknown";
    case LHCb::EventCountFSR::UNRELIABLE:
      return s << "UNRELIABLE";
    case LHCb::EventCountFSR::UNCHECKED:
      return s << "UNCHECKED";
    case LHCb::EventCountFSR::ERROR:
      return s << "ERROR";
    case LHCb::EventCountFSR::VERIFIED:
      return s << "VERIFIED";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::EventCountFSR::StatusFlag";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::EventCountFSR::clID() const { return LHCb::EventCountFSR::classID(); }

inline const CLID& LHCb::EventCountFSR::classID() { return CLID_EventCountFSR; }

inline const GaudiUtils::VectorMap<std::string, LHCb::EventCountFSR::StatusFlag>&
LHCb::EventCountFSR::s_StatusFlagTypMap() {
  static const GaudiUtils::VectorMap<std::string, StatusFlag> m = {{"Unknown", Unknown},
                                                                   {"UNRELIABLE", UNRELIABLE},
                                                                   {"UNCHECKED", UNCHECKED},
                                                                   {"ERROR", ERROR},
                                                                   {"VERIFIED", VERIFIED}};
  return m;
}

inline LHCb::EventCountFSR::StatusFlag LHCb::EventCountFSR::StatusFlagToType( const std::string& aName ) {
  auto iter = s_StatusFlagTypMap().find( aName );
  return iter != s_StatusFlagTypMap().end() ? iter->second : Unknown;
}

inline const std::string& LHCb::EventCountFSR::StatusFlagToString( int aEnum ) {
  static const std::string s_Unknown = "Unknown";
  auto                     iter      = std::find_if( s_StatusFlagTypMap().begin(), s_StatusFlagTypMap().end(),
                            [&]( const std::pair<const std::string, StatusFlag>& i ) { return i.second == aEnum; } );
  return iter != s_StatusFlagTypMap().end() ? iter->first : s_Unknown;
}

inline long long LHCb::EventCountFSR::output() const { return m_output; }

inline void LHCb::EventCountFSR::setOutput( long long value ) { m_output = value; }

inline long long LHCb::EventCountFSR::input() const { return m_input; }

inline void LHCb::EventCountFSR::setInput( long long value ) { m_input = value; }

inline const LHCb::EventCountFSR::StatusFlag& LHCb::EventCountFSR::statusFlag() const { return m_statusFlag; }

inline void LHCb::EventCountFSR::setStatusFlag( const LHCb::EventCountFSR::StatusFlag& value ) { m_statusFlag = value; }

inline std::ostream& LHCb::EventCountFSR::fillStream( std::ostream& s ) const {

  std::ostream& ss = const_cast<std::ostream&>( s );
  ss << "{ "
     << " input : " << m_input << " output : " << m_output << " statusFlag : " << m_statusFlag << " }";

  return ss;
}
