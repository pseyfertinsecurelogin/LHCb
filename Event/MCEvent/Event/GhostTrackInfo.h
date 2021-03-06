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
#include "GaudiKernel/Map.h"
#include <ostream>
#include <utility>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class GhostTrackInfo GhostTrackInfo.h
   *
   * info on ghost tracks
   *
   * @author M. Needham
   *
   */

  class GhostTrackInfo final {
  public:
    /// map of linked particles
    typedef GaudiUtils::Map<MCParticle*, unsigned int> LinkMap;
    /// pair of MCParticle and weight
    typedef std::pair<MCParticle*, double> LinkPair;

    /// Ghost Track classification
    enum Classification {
      None                = 0,
      Real                = 1,
      Conversion          = 10,
      EM                  = 11,
      FromPhi             = 12,
      HadronicInteraction = 13,
      DecayInFlight       = 14,
      SpilloverAndNoise   = 20,
      GhostParent         = 21,
      InconsistentParts   = 22,
      Combinatoric        = 23,
      GhostVeloParent     = 24,
      GhostTParent        = 25
    };

    /// special constructor
    GhostTrackInfo( const LinkMap& lmap, const Classification& value ) : m_linkMap( lmap ), m_classification( value ) {}

    /// Default Constructor
    GhostTrackInfo() : m_linkMap(), m_classification( Classification::None ) {}

    /// number of links, ie particles or noise contributing
    unsigned int nLinked() const;

    /// fraction of hits linked to noise
    double fractionNoise() const;

    /// best linked
    LinkPair bestLink() const;

    /// Printin a human readable way
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  mapping of particles
    const LinkMap& linkMap() const;

    /// Update  mapping of particles
    void setLinkMap( const LinkMap& value );

    /// Retrieve const
    const Classification& classification() const;

    /// Update
    void setClassification( const Classification& value );

    friend std::ostream& operator<<( std::ostream& str, const GhostTrackInfo& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    LinkMap        m_linkMap;        ///< mapping of particles
    Classification m_classification; ///<

  }; // class GhostTrackInfo

  inline std::ostream& operator<<( std::ostream& s, LHCb::GhostTrackInfo::Classification e ) {
    switch ( e ) {
    case LHCb::GhostTrackInfo::None:
      return s << "None";
    case LHCb::GhostTrackInfo::Real:
      return s << "Real";
    case LHCb::GhostTrackInfo::Conversion:
      return s << "Conversion";
    case LHCb::GhostTrackInfo::EM:
      return s << "EM";
    case LHCb::GhostTrackInfo::FromPhi:
      return s << "FromPhi";
    case LHCb::GhostTrackInfo::HadronicInteraction:
      return s << "HadronicInteraction";
    case LHCb::GhostTrackInfo::DecayInFlight:
      return s << "DecayInFlight";
    case LHCb::GhostTrackInfo::SpilloverAndNoise:
      return s << "SpilloverAndNoise";
    case LHCb::GhostTrackInfo::GhostParent:
      return s << "GhostParent";
    case LHCb::GhostTrackInfo::InconsistentParts:
      return s << "InconsistentParts";
    case LHCb::GhostTrackInfo::Combinatoric:
      return s << "Combinatoric";
    case LHCb::GhostTrackInfo::GhostVeloParent:
      return s << "GhostVeloParent";
    case LHCb::GhostTrackInfo::GhostTParent:
      return s << "GhostTParent";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::GhostTrackInfo::Classification";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const LHCb::GhostTrackInfo::LinkMap& LHCb::GhostTrackInfo::linkMap() const { return m_linkMap; }

inline void LHCb::GhostTrackInfo::setLinkMap( const LinkMap& value ) { m_linkMap = value; }

inline const LHCb::GhostTrackInfo::Classification& LHCb::GhostTrackInfo::classification() const {
  return m_classification;
}

inline void LHCb::GhostTrackInfo::setClassification( const Classification& value ) { m_classification = value; }

inline unsigned int LHCb::GhostTrackInfo::nLinked() const { return m_linkMap.size(); }

inline double LHCb::GhostTrackInfo::fractionNoise() const {

  LinkMap::const_iterator iter = m_linkMap.find( 0 );
  return ( iter == m_linkMap.end() ? 0.0 : iter->second );
}
