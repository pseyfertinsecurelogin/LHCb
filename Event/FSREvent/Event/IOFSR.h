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
#include "GaudiKernel/SerializeSTL.h"
#include "GaudiKernel/VectorMap.h"
#include <algorithm>
#include <map>
#include <ostream>
#include <string>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_IOFSR = 13506;

  // Namespace for locations in TDS
  namespace IOFSRLocation {
    inline const std::string Default = "/FileRecords/IOFSR";
  }

  /** @class IOFSR IOFSR.h
   *
   * Class to store IO information about a job, upgraded EventAccountFSR, needs a
   * special FSRWriter to add file-by-file
   *
   * @author Rob lambert
   *
   */

  class IOFSR final : public DataObject {
  public:
    /// typedef for std::vector of IOFSR
    typedef std::vector<IOFSR*>       Vector;
    typedef std::vector<const IOFSR*> ConstVector;

    /// File Input Events, {GUID:unsigned long long}
    typedef std::map<std::string, unsigned long long> FileEventMap;
    /// File Provenance, {GUID:<GUIDs>}
    typedef std::map<std::string, std::vector<std::string>> ProvenanceMap;

    /// FSR status, is the accounting OK
    enum StatusFlag {
      Unknown      = -6, // other enum possibilities
      FILEDOUBLED  = -5, // Some files have been multiply processed, in more than one job
      EVENTDOUBLED = -4, // Some events have been multiply processed, seen events too large
      EVENTSKIPPED = -3, // Some events have been skipped, seen events too small
      UNRELIABLE   = -2, // The counting in this FSR is known to be suspect
      ERROR        = -1, // Missing or corrupt FSRs
      UNCHECKED    = 0,  // The counting needs to be checked
      VERIFIED     = 1   // The counting in all lower FSRs has been verified OK
    };

    /// Default Constructor
    IOFSR()
        : m_eventsOutput( 0 )
        , m_eventsSeen( 0 )
        , m_parents( 0 )
        , m_provenance()
        , m_eventsWrittenTo()
        , m_eventsSeenBy()
        , m_eventsReadFrom()
        , m_FID()
        , m_statusFlag( LHCb::IOFSR::StatusFlag::UNCHECKED ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// conversion of string to enum for type StatusFlag
    static LHCb::IOFSR::StatusFlag StatusFlagToType( const std::string& aName );

    /// conversion to string for enum type StatusFlag
    static const std::string& StatusFlagToString( int aEnum );

    /// Retrieve const  Number of events this output file should contain
    unsigned long long eventsOutput() const;

    /// Update  Number of events this output file should contain
    void setEventsOutput( unsigned long long value );

    /// Retrieve const  Number of events read over to produce this file
    unsigned long long eventsSeen() const;

    /// Update  Number of events read over to produce this file
    void setEventsSeen( unsigned long long value );

    /// Retrieve const  list of parent files at this level
    const std::vector<std::string>& parents() const;

    /// Update  list of parent files at this level
    void setParents( const std::vector<std::string>& value );

    /// Retrieve const  maps of ancestor files below this level
    const ProvenanceMap& provenance() const;

    /// Update  maps of ancestor files below this level
    void setProvenance( const ProvenanceMap& value );

    /// Retrieve const  Map of input files and events written to those files
    const FileEventMap& eventsWrittenTo() const;

    /// Update  Map of input files and events written to those files
    void setEventsWrittenTo( const FileEventMap& value );

    /// Retrieve const  Map of input files and events read in creating those files
    const FileEventMap& eventsSeenBy() const;

    /// Update  Map of input files and events read in creating those files
    void setEventsSeenBy( const FileEventMap& value );

    /// Retrieve const  Map of input files and events read from those files
    const FileEventMap& eventsReadFrom() const;

    /// Update  Map of input files and events read from those files
    void setEventsReadFrom( const FileEventMap& value );

    /// Retrieve const  May be filled with this file's GUID if it is known, almost never known
    const std::string& fid() const;

    /// Update  May be filled with this file's GUID if it is known, almost never known
    void setFID( const std::string& value );

    /// Retrieve const  Is the FSR reliable? This time, OK wrt streaming. Fill during merging.
    const LHCb::IOFSR::StatusFlag& statusFlag() const;

    /// Update  Is the FSR reliable? This time, OK wrt streaming. Fill during merging.
    void setStatusFlag( const LHCb::IOFSR::StatusFlag& value );

    friend std::ostream& operator<<( std::ostream& str, const IOFSR& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    unsigned long long       m_eventsOutput;    ///< Number of events this output file should contain
    unsigned long long       m_eventsSeen;      ///< Number of events read over to produce this file
    std::vector<std::string> m_parents;         ///< list of parent files at this level
    ProvenanceMap            m_provenance;      ///< maps of ancestor files below this level
    FileEventMap             m_eventsWrittenTo; ///< Map of input files and events written to those files
    FileEventMap             m_eventsSeenBy;    ///< Map of input files and events read in creating those files
    FileEventMap             m_eventsReadFrom;  ///< Map of input files and events read from those files
    std::string              m_FID;        ///< May be filled with this file's GUID if it is known, almost never known
    LHCb::IOFSR::StatusFlag  m_statusFlag; ///< Is the FSR reliable? This time, OK wrt streaming. Fill during merging.

    static const GaudiUtils::VectorMap<std::string, StatusFlag>& s_StatusFlagTypMap();

  }; // class IOFSR

  inline std::ostream& operator<<( std::ostream& s, LHCb::IOFSR::StatusFlag e ) {
    switch ( e ) {
    case LHCb::IOFSR::Unknown:
      return s << "Unknown";
    case LHCb::IOFSR::FILEDOUBLED:
      return s << "FILEDOUBLED";
    case LHCb::IOFSR::EVENTDOUBLED:
      return s << "EVENTDOUBLED";
    case LHCb::IOFSR::EVENTSKIPPED:
      return s << "EVENTSKIPPED";
    case LHCb::IOFSR::UNRELIABLE:
      return s << "UNRELIABLE";
    case LHCb::IOFSR::ERROR:
      return s << "ERROR";
    case LHCb::IOFSR::UNCHECKED:
      return s << "UNCHECKED";
    case LHCb::IOFSR::VERIFIED:
      return s << "VERIFIED";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::IOFSR::StatusFlag";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::IOFSR::clID() const { return LHCb::IOFSR::classID(); }

inline const CLID& LHCb::IOFSR::classID() { return CLID_IOFSR; }

inline std::ostream& LHCb::IOFSR::fillStream( std::ostream& s ) const {
  s << "{ "
    << "eventsOutput :	" << m_eventsOutput << std::endl
    << "eventsSeen :	" << m_eventsSeen << std::endl
    << "parents :	" << m_parents << std::endl
    << "provenance :	" << m_provenance << std::endl
    << "eventsWrittenTo :	" << m_eventsWrittenTo << std::endl
    << "eventsSeenBy :	" << m_eventsSeenBy << std::endl
    << "eventsReadFrom :	" << m_eventsReadFrom << std::endl
    << "FID :	" << m_FID << std::endl
    << "statusFlag :	" << m_statusFlag << std::endl
    << " }";
  return s;
}

inline const GaudiUtils::VectorMap<std::string, LHCb::IOFSR::StatusFlag>& LHCb::IOFSR::s_StatusFlagTypMap() {
  static const GaudiUtils::VectorMap<std::string, StatusFlag> m = {
      {"Unknown", Unknown},           {"FILEDOUBLED", FILEDOUBLED}, {"EVENTDOUBLED", EVENTDOUBLED},
      {"EVENTSKIPPED", EVENTSKIPPED}, {"UNRELIABLE", UNRELIABLE},   {"ERROR", ERROR},
      {"UNCHECKED", UNCHECKED},       {"VERIFIED", VERIFIED}};
  return m;
}

inline LHCb::IOFSR::StatusFlag LHCb::IOFSR::StatusFlagToType( const std::string& aName ) {
  auto iter = s_StatusFlagTypMap().find( aName );
  return iter != s_StatusFlagTypMap().end() ? iter->second : Unknown;
}

inline const std::string& LHCb::IOFSR::StatusFlagToString( int aEnum ) {
  static const std::string s_Unknown = "Unknown";
  auto                     iter      = std::find_if( s_StatusFlagTypMap().begin(), s_StatusFlagTypMap().end(),
                            [&]( const std::pair<const std::string, StatusFlag>& i ) { return i.second == aEnum; } );
  return iter != s_StatusFlagTypMap().end() ? iter->first : s_Unknown;
}

inline unsigned long long LHCb::IOFSR::eventsOutput() const { return m_eventsOutput; }

inline void LHCb::IOFSR::setEventsOutput( unsigned long long value ) { m_eventsOutput = value; }

inline unsigned long long LHCb::IOFSR::eventsSeen() const { return m_eventsSeen; }

inline void LHCb::IOFSR::setEventsSeen( unsigned long long value ) { m_eventsSeen = value; }

inline const std::vector<std::string>& LHCb::IOFSR::parents() const { return m_parents; }

inline void LHCb::IOFSR::setParents( const std::vector<std::string>& value ) { m_parents = value; }

inline const LHCb::IOFSR::ProvenanceMap& LHCb::IOFSR::provenance() const { return m_provenance; }

inline void LHCb::IOFSR::setProvenance( const ProvenanceMap& value ) { m_provenance = value; }

inline const LHCb::IOFSR::FileEventMap& LHCb::IOFSR::eventsWrittenTo() const { return m_eventsWrittenTo; }

inline void LHCb::IOFSR::setEventsWrittenTo( const FileEventMap& value ) { m_eventsWrittenTo = value; }

inline const LHCb::IOFSR::FileEventMap& LHCb::IOFSR::eventsSeenBy() const { return m_eventsSeenBy; }

inline void LHCb::IOFSR::setEventsSeenBy( const FileEventMap& value ) { m_eventsSeenBy = value; }

inline const LHCb::IOFSR::FileEventMap& LHCb::IOFSR::eventsReadFrom() const { return m_eventsReadFrom; }

inline void LHCb::IOFSR::setEventsReadFrom( const FileEventMap& value ) { m_eventsReadFrom = value; }

inline const std::string& LHCb::IOFSR::fid() const { return m_FID; }

inline void LHCb::IOFSR::setFID( const std::string& value ) { m_FID = value; }

inline const LHCb::IOFSR::StatusFlag& LHCb::IOFSR::statusFlag() const { return m_statusFlag; }

inline void LHCb::IOFSR::setStatusFlag( const LHCb::IOFSR::StatusFlag& value ) { m_statusFlag = value; }
