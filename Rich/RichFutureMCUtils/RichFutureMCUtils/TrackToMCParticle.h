/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

#pragma once

// local
#include "RichFutureMCUtils/RichMCRelations.h"

// Kernel
#include "Kernel/RichParticleIDType.h"

// Event model
#include "Event/MCParticle.h"
#include "Event/Track.h"

namespace Rich::Future::MC::Relations {

  /// Helper for Track->MCParticle relations
  class TrackToMCParticle {

  public:
    /// Constructor from a relations table
    TrackToMCParticle( const TkToMCPRels& rels ) : m_rels( rels ) {}

  public:
    /// Access the best MCParticle match for the given track
    const LHCb::MCParticle* mcParticle( const LHCb::Track& track, const double minWeight = 0.5 ) const;

    /// Access the full range of MCParticles associated to a given track
    inline decltype( auto ) mcParticleRange( const LHCb::Track& track ) const { return m_rels.relations( &track ); }

    /** Get a container of the associated MCparticles for a given track.
     *  @attention If includeNull=true, ifno associations available,
     *  a single NULL entry is returned in the vector. */
    LHCb::MCParticle::ConstVector mcParticles( const LHCb::Track& track, const bool includeNull = false,
                                               const double minWeight = 0.5 ) const;

    /// Get the MC RICH PID type for a given track
    Rich::ParticleIDType mcParticleType( const LHCb::Track& track, const double minWeight = 0.5 ) const;

    /// Get the MC RICH PID type for the given MCParticle
    Rich::ParticleIDType mcParticleType( const LHCb::MCParticle* mcP ) const;

  private:
    /// reference to the relations
    const TkToMCPRels& m_rels;
  };

} // namespace Rich::Future::MC::Relations
