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
#include "Event/MCParticle.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include "Kernel/RichRadiatorType.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class MCRichSegment;

  // Class ID definition
  static const CLID CLID_MCRichTrack = 12415;

  // Namespace for locations in TDS
  namespace MCRichTrackLocation {
    inline const std::string Default = "MC/Rich/Tracks";
  }

  /** @class MCRichTrack MCRichTrack.h
   *
   * Complimentary object to a single MCParticle with addition RICH information
   *
   * @author Christopher Jones   Christopher.Rob.Jones@cern.ch
   *
   */

  class MCRichTrack : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of MCRichTrack
    typedef KeyedContainer<MCRichTrack, Containers::HashMap> Container;

    /// Default Constructor
    MCRichTrack() {}

    /// Default Destructor
    virtual ~MCRichTrack() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Returns the MCRichSegment associated with a given radiator (null if not available)
    const LHCb::MCRichSegment* segmentInRad( const Rich::RadiatorType rad ) const;

    /// Retrieve (const)  Associatted radiator segments
    const SmartRefVector<LHCb::MCRichSegment>& mcSegments() const;

    /// Update  Associatted radiator segments
    void setMcSegments( const SmartRefVector<LHCb::MCRichSegment>& value );

    /// Add to  Associatted radiator segments
    void addToMcSegments( const SmartRef<LHCb::MCRichSegment>& value );

    /// Att to (pointer)  Associatted radiator segments
    void addToMcSegments( const LHCb::MCRichSegment* value );

    /// Remove from  Associatted radiator segments
    void removeFromMcSegments( const SmartRef<LHCb::MCRichSegment>& value );

    /// Clear  Associatted radiator segments
    void clearMcSegments();

    /// Retrieve (const)  Associated MCParticle
    const LHCb::MCParticle* mcParticle() const;

    /// Update  Associated MCParticle
    void setMcParticle( const SmartRef<LHCb::MCParticle>& value );

    /// Update (pointer)  Associated MCParticle
    void setMcParticle( const LHCb::MCParticle* value );

    friend std::ostream& operator<<( std::ostream& str, const MCRichTrack& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    SmartRefVector<LHCb::MCRichSegment> m_mcSegments; ///< Associatted radiator segments
    SmartRef<LHCb::MCParticle>          m_mcParticle; ///< Associated MCParticle

  }; // class MCRichTrack

  /// Definition of Keyed Container for MCRichTrack
  typedef KeyedContainer<MCRichTrack, Containers::HashMap> MCRichTracks;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations
#include "Event/MCRichSegment.h"

inline const CLID& LHCb::MCRichTrack::clID() const { return LHCb::MCRichTrack::classID(); }

inline const CLID& LHCb::MCRichTrack::classID() { return CLID_MCRichTrack; }

inline std::ostream& LHCb::MCRichTrack::fillStream( std::ostream& s ) const { return s; }

inline const SmartRefVector<LHCb::MCRichSegment>& LHCb::MCRichTrack::mcSegments() const { return m_mcSegments; }

inline void LHCb::MCRichTrack::setMcSegments( const SmartRefVector<LHCb::MCRichSegment>& value ) {
  m_mcSegments = value;
}

inline void LHCb::MCRichTrack::addToMcSegments( const SmartRef<LHCb::MCRichSegment>& value ) {
  m_mcSegments.push_back( value );
}

inline void LHCb::MCRichTrack::addToMcSegments( const LHCb::MCRichSegment* value ) { m_mcSegments.push_back( value ); }

inline void LHCb::MCRichTrack::removeFromMcSegments( const SmartRef<LHCb::MCRichSegment>& value ) {
  auto i = std::remove( m_mcSegments.begin(), m_mcSegments.end(), value );
  m_mcSegments.erase( i, m_mcSegments.end() );
}

inline void LHCb::MCRichTrack::clearMcSegments() { m_mcSegments.clear(); }

inline const LHCb::MCParticle* LHCb::MCRichTrack::mcParticle() const { return m_mcParticle; }

inline void LHCb::MCRichTrack::setMcParticle( const SmartRef<LHCb::MCParticle>& value ) { m_mcParticle = value; }

inline void LHCb::MCRichTrack::setMcParticle( const LHCb::MCParticle* value ) { m_mcParticle = value; }
