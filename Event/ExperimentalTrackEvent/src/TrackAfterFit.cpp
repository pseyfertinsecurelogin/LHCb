#include "Event/TrackAfterFit.h"

experimental::TrackAfterFitExtension::Hits::Hits(
    const std::vector<LHCb::LHCbID> &container)
    : m_hits(container.begin(), container.end()) {}
experimental::TrackAfterFitExtension::Hits::Hits(
    const LHCb::span<const LHCb::LHCbID> container)
    : m_hits(container.begin(), container.end()) {}
