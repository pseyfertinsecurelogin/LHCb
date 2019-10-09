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
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_TimeSpanFSR = 13504;

  // Namespace for locations in TDS
  namespace TimeSpanFSRLocation {
    inline const std::string Default = "/FileRecords/TimeSpanFSR";
  }

  /** @class TimeSpanFSR TimeSpanFSR.h
   *
   * Class to store time span of an input file in FSR
   *
   * @author Jaap Panman
   *
   */

  class TimeSpanFSR final : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of TimeSpanFSR
    typedef KeyedContainer<TimeSpanFSR, Containers::HashMap> Container;

    /// Default Constructor
    TimeSpanFSR() : m_earliest( 0 ), m_latest( 0 ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Assignment operator
    TimeSpanFSR& operator=( const LHCb::TimeSpanFSR& rhs );

    /// add a time and store the earliest/latest times
    void addTime( unsigned long long present );

    /// Addition operator
    TimeSpanFSR& operator+=( const unsigned long long rhs );

    /// add two FSRs and store the earliest/latest times
    TimeSpanFSR& operator+=( const LHCb::TimeSpanFSR& rhs );

    /// add two FSRs and store the earliest/latest times
    TimeSpanFSR& operator+=( const LHCb::TimeSpanFSR* rhs );

    /// readable printout
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Earliest time found on a file
    unsigned long long earliest() const;

    /// Update  Earliest time found on a file
    void setEarliest( unsigned long long value );

    /// Retrieve const  Latest time found on a file
    unsigned long long latest() const;

    /// Update  Latest time found on a file
    void setLatest( unsigned long long value );

    friend std::ostream& operator<<( std::ostream& str, const TimeSpanFSR& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    unsigned long long m_earliest; ///< Earliest time found on a file
    unsigned long long m_latest;   ///< Latest time found on a file

  }; // class TimeSpanFSR

  /// Definition of Keyed Container for TimeSpanFSR
  typedef KeyedContainer<TimeSpanFSR, Containers::HashMap> TimeSpanFSRs;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::TimeSpanFSR::clID() const { return LHCb::TimeSpanFSR::classID(); }

inline const CLID& LHCb::TimeSpanFSR::classID() { return CLID_TimeSpanFSR; }

inline unsigned long long LHCb::TimeSpanFSR::earliest() const { return m_earliest; }

inline void LHCb::TimeSpanFSR::setEarliest( unsigned long long value ) { m_earliest = value; }

inline unsigned long long LHCb::TimeSpanFSR::latest() const { return m_latest; }

inline void LHCb::TimeSpanFSR::setLatest( unsigned long long value ) { m_latest = value; }

inline LHCb::TimeSpanFSR& LHCb::TimeSpanFSR::operator=( const LHCb::TimeSpanFSR& rhs ) {

  if ( this != &rhs ) {
    m_earliest = rhs.m_earliest;
    m_latest   = rhs.m_latest;
  }

  return *this;
}

inline void LHCb::TimeSpanFSR::addTime( unsigned long long present ) {

  if ( m_earliest > 0 ) {
    m_earliest = std::min( present, m_earliest );
  } else {
    m_earliest = present;
  }
  m_latest = std::max( present, m_latest );
}

inline LHCb::TimeSpanFSR& LHCb::TimeSpanFSR::operator+=( const unsigned long long rhs ) {

  addTime( rhs );
  return *this;
}

inline LHCb::TimeSpanFSR& LHCb::TimeSpanFSR::operator+=( const LHCb::TimeSpanFSR& rhs ) {

  if ( rhs.m_earliest > 0 ) addTime( rhs.m_earliest );
  addTime( rhs.m_latest );
  return *this;
}

inline LHCb::TimeSpanFSR& LHCb::TimeSpanFSR::operator+=( const LHCb::TimeSpanFSR* rhs ) { return *this += *rhs; }

inline std::ostream& LHCb::TimeSpanFSR::fillStream( std::ostream& s ) const {

  std::ostream& ss = const_cast<std::ostream&>( s );
  ss << "{ "
     << " earliest : " << m_earliest << " - latest : " << m_latest << " }";

  return ss;
}
