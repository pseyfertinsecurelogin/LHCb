#pragma once
#include "Event/State.h"
#include "Event/Track.h"
#include "Kernel/LHCbID.h"
#include "Kernel/STLExtensions.h"
// in Rec, not LHCb
// #include "PrKernel/PrVeloUTTrack.h"
#include "VeloTrack.h"
#include <boost/container/small_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <vector>

#include "SOAContainer/SOAContainer.h"

namespace LHCb::Pr
{
  // in Rec, not LHCb
  // namespace v2
  // {
  //   using VeloUTTrack  = PrVeloUTTrack;   // from PrKernel/PrVeloUTTrack.h
  //   using VeloUTTracks = PrVeloUTTracks;  // from PrKernel/PrVeloUTTrack.h
  // }
  namespace experimental
  {

    struct VeloUTTrackCore;
    using VeloUTTrackCores = std::vector<VeloUTTrackCore>;

    // mostly copied from PrKernel/PrVeloUTTrack.h
    struct VeloUTTrackCore {
      float qOverP;
      // this is a proxy!
      const VeloTrack veloTr;
    };

    using VeloUTHits  = boost::container::static_vector<LHCb::LHCbID, 8>;
    using VeloUTHitss = std::vector<VeloUTTrackCore>;

    namespace VeloUTData
    {
      SOAFIELD_TRIVIAL( f_core, core, VeloUTTrackCore );
      SOAFIELD_TRIVIAL( f_hits, hits, VeloUTHits );
      SOASKIN( VeloUTTrackSkin, f_core, f_hits )
      {
        /// comment to make clang-format happy
        SOASKIN_INHERIT_DEFAULT_METHODS( VeloUTTrackSkin );
      };
    }; // namespace VeloUTData

    typedef SOA::Container<std::vector, VeloUTData::VeloUTTrackSkin> VeloUTTracks;
    typedef typename VeloUTTracks::proxy                             VeloUTTrack;

  } // namespace experimental
} // namespace LHCb::Pr
