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
#include "RichFutureMCUtils/RichSmartIDMCUtils.h"
#include "RichFutureMCUtils/TrackToMCParticle.h"

// Rich Utils
#include "RichUtils/RichPixelCluster.h"
#include "RichUtils/RichTrackSegment.h"

namespace Rich::Future::Rec::MC {

  using namespace Rich::Future::MC::Relations;

  /// Utility class to provide various Rec MC utilities
  class Helper : public SmartIDUtils, public TrackToMCParticle {

  public:
    /// Contructor from the underlying MC relations
    Helper( const TkToMCPRels& rels, const LHCb::MCRichDigitSummarys& histories )
        : SmartIDUtils( histories ), TrackToMCParticle( rels ) {}

  public:
    using SmartIDUtils::mcParticles;
    using TrackToMCParticle::mcParticles;

  public:
    /// Access the common MCParticle(s) for the given track and RICH hit
    LHCb::MCParticle::ConstVector trueRecPhoton( const LHCb::Track& track, const Rich::PDPixelCluster& cluster ) const;

    /// Returns a valid pointer if the given MCParticle produced (part) of the given cluster
    const LHCb::MCParticle* trueRecPhoton( const LHCb::MCParticle& mcPart, const Rich::PDPixelCluster& cluster ) const;

    /// Returns a valid pointer if the given MCParticle produced the given channel ID
    const LHCb::MCParticle* trueRecPhoton( const LHCb::MCParticle& mcPart, const LHCb::RichSmartID id ) const;

    /// If a true Cherenkov photon, returns the parent MCParticle(s)
    LHCb::MCParticle::ConstVector trueCherenkovPhoton( const LHCb::Track& track, const Rich::RadiatorType rad,
                                                       const Rich::PDPixelCluster& cluster ) const;
  };

} // namespace Rich::Future::Rec::MC
