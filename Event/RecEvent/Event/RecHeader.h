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
#include "Event/ProcessHeader.h"
#include "GaudiKernel/SerializeSTL.h"
#include <ostream>
#include <string>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_RecHeader = 105;

  // Namespace for locations in TDS
  namespace RecHeaderLocation {
    inline const std::string Default = LHCb::ProcessHeaderLocation::Rec;
  }

  /** @class RecHeader RecHeader.h
   *
   * Header for reconstruction information
   *
   * @author M. Cattaneo
   *
   */

  class RecHeader final : public ProcessHeader {
  public:
    /// Copy constructor. Creates a new RecHeader with the same information.
    RecHeader( const LHCb::RecHeader& head )
        : ProcessHeader( head )
        , m_evtNumber( head.evtNumber() )
        , m_gpsTime( head.gpsTime() )
        , m_rawID( head.rawID() ) {}

    /// Default Constructor
    RecHeader() : m_evtNumber( 0 ), m_gpsTime( 0 ), m_rawID() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Event number (same as in ODIN)
    long long evtNumber() const;

    /// Update  Event number (same as in ODIN)
    void setEvtNumber( long long value );

    /// Retrieve const  GPS Time (same as in ODIN, microseconds)
    unsigned long long gpsTime() const;

    /// Update  GPS Time (same as in ODIN, microseconds)
    void setGpsTime( unsigned long long value );

    /// Retrieve const  UUID of the raw file to which this event belongs
    const std::string& rawID() const;

    /// Update  UUID of the raw file to which this event belongs
    void setRawID( const std::string& value );

    friend std::ostream& operator<<( std::ostream& str, const RecHeader& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    long long          m_evtNumber; ///< Event number (same as in ODIN)
    unsigned long long m_gpsTime;   ///< GPS Time (same as in ODIN, microseconds)
    std::string        m_rawID;     ///< UUID of the raw file to which this event belongs

  }; // class RecHeader

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::RecHeader::clID() const { return LHCb::RecHeader::classID(); }

inline const CLID& LHCb::RecHeader::classID() { return CLID_RecHeader; }

inline std::ostream& LHCb::RecHeader::fillStream( std::ostream& s ) const {
  ProcessHeader::fillStream( s );
  s << "{ "
    << "evtNumber :	" << m_evtNumber << std::endl
    << "gpsTime :	" << m_gpsTime << std::endl
    << "rawID :	" << m_rawID << std::endl
    << " }";
  return s;
}

inline long long LHCb::RecHeader::evtNumber() const { return m_evtNumber; }

inline void LHCb::RecHeader::setEvtNumber( long long value ) { m_evtNumber = value; }

inline unsigned long long LHCb::RecHeader::gpsTime() const { return m_gpsTime; }

inline void LHCb::RecHeader::setGpsTime( unsigned long long value ) { m_gpsTime = value; }

inline const std::string& LHCb::RecHeader::rawID() const { return m_rawID; }

inline void LHCb::RecHeader::setRawID( const std::string& value ) { m_rawID = value; }
