#pragma once
#include "Event/Track_v1.h"

namespace LHCb{
  using Track = Event::Track; // for backwards compatibility, to be deprecated one day...
  using Tracks = Event::Tracks; // for backwards compatibility, to be deprecated one day...
  namespace TrackLocation = Event::TrackLocation;
  static const CLID CLID_Track = Event::CLID_Track;

}
