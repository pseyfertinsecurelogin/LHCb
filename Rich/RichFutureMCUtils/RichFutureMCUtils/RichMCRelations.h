
#pragma once

// STL
#include <string>

// Event model
#include "Event/MCParticle.h"
#include "Event/Track.h"

// Relations
#include "Relations/IRelationWeighted2D.h"
#include "Relations/RelationWeighted2D.h"

namespace Rich::Future::MC::Relations
{

  /// Type for Track <-> MCparticle relations
  using TkToMCPRels = LHCb::RelationWeighted2D< LHCb::Track *, LHCb::MCParticle *, double >;

  /// Default TES location for Track <-> MCparticle relations
  static const std::string TrackToMCParticles = "MC/Rich/TrackToMCParticleRelations";

} // namespace Rich::Future::MC::Relations
