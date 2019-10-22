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
#include "Event/RichSummaryPhoton.h"
#include "Event/RichSummaryRadSegment.h"
#include "Event/Track.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SmartRef.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_RichSummaryTrack = 12451;

  // Namespace for locations in TDS
  namespace RichSummaryTrackLocation {
    inline const std::string Default = "Rec/Rich/RichSummaryTracks";
    inline const std::string HLT     = "Rec/Rich/RichHLTSummaryTracks";
    inline const std::string Offline = "Rec/Rich/RichOfflineSummaryTracks";
  } // namespace RichSummaryTrackLocation

  /** @class RichSummaryTrack RichSummaryTrack.h
   *
   * RICH reconstruction summary object representing a single track
   *
   * @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *
   */

  class RichSummaryTrack final : public KeyedObject<int> {
  public:
    /// typedef for std::vector of RichSummaryTrack
    using Vector      = std::vector<RichSummaryTrack*>;
    using ConstVector = std::vector<const RichSummaryTrack*>;

    /// typedef for KeyedContainer of RichSummaryTrack
    typedef KeyedContainer<RichSummaryTrack, Containers::HashMap> Container;

    /// Default Constructor
    RichSummaryTrack() = default;

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Vector of radiator segments for this track
    const LHCb::RichSummaryRadSegment::Vector& radSegments() const;

    /// Update  Vector of radiator segments for this track
    void setRadSegments( const LHCb::RichSummaryRadSegment::Vector& value );

    /// Retrieve (const)  Associated reconstructed Track
    const LHCb::Track* track() const;

    /// Update  Associated reconstructed Track
    void setTrack( const SmartRef<LHCb::Track>& value );

    /// Update (pointer)  Associated reconstructed Track
    void setTrack( const LHCb::Track* value );

    friend std::ostream& operator<<( std::ostream& str, const RichSummaryTrack& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    LHCb::RichSummaryRadSegment::Vector m_radSegments; ///< Vector of radiator segments for this track
    SmartRef<LHCb::Track>               m_track;       ///< Associated reconstructed Track

  }; // class RichSummaryTrack

  /// Definition of Keyed Container for RichSummaryTrack
  typedef KeyedContainer<RichSummaryTrack, Containers::HashMap> RichSummaryTracks;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::RichSummaryTrack::clID() const { return LHCb::RichSummaryTrack::classID(); }

inline const CLID& LHCb::RichSummaryTrack::classID() { return CLID_RichSummaryTrack; }

inline std::ostream& LHCb::RichSummaryTrack::fillStream( std::ostream& s ) const {
  s << "{ "
    << "radSegments :	" << m_radSegments << std::endl
    << " }";
  return s;
}

inline const LHCb::RichSummaryRadSegment::Vector& LHCb::RichSummaryTrack::radSegments() const { return m_radSegments; }

inline void LHCb::RichSummaryTrack::setRadSegments( const LHCb::RichSummaryRadSegment::Vector& value ) {
  m_radSegments = value;
}

inline const LHCb::Track* LHCb::RichSummaryTrack::track() const { return m_track; }

inline void LHCb::RichSummaryTrack::setTrack( const SmartRef<LHCb::Track>& value ) { m_track = value; }

inline void LHCb::RichSummaryTrack::setTrack( const LHCb::Track* value ) { m_track = value; }
