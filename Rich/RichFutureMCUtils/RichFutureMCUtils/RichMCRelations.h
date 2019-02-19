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

// STL
#include <string>

// Event model
#include "Event/MCParticle.h"
#include "Event/Track.h"

// Relations
#include "Relations/IRelationWeighted2D.h"
#include "Relations/RelationWeighted2D.h"

namespace Rich::Future::MC::Relations {

  /// Type for Track <-> MCparticle relations
  using TkToMCPRels = LHCb::RelationWeighted2D<LHCb::Track*, LHCb::MCParticle*, double>;

  /// Default TES location for Track <-> MCparticle relations
  inline const std::string TrackToMCParticles = "MC/Rich/TrackToMCParticleRelations";

} // namespace Rich::Future::MC::Relations
