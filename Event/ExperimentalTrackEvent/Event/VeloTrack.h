#pragma once
#include "Event/State.h"
#include "Event/Track.h"
#include "Kernel/LHCbID.h"
#include "Kernel/STLExtensions.h"
#include <boost/container/small_vector.hpp>
#include <vector>

#include "SOAContainer/SOAContainer.h"

namespace LHCb::Pr
{
  namespace experimental
  {
    struct placeholder {
      // what do we want to use these days?
      // LHCbID?
      // Cluster?
      // Hit?
      // Measurement?
    };
    using VeloHit = placeholder;
    // FIXME: small / static / std / VALUE
    using VeloHits = boost::container::small_vector<VeloHit, 10>;

    namespace VeloData {
      SOAFIELD_TRIVIAL( f_pvstate, pvstate,  LHCb::State );
      SOAFIELD_TRIVIAL( f_prstate, prstate,  LHCb::State );
      SOAFIELD_TRIVIAL( f_vhits,   velohits, VeloHits );
      SOASKIN( VeloTrackSkin, f_pvstate, f_prstate, f_vhits ) {
        SOASKIN_INHERIT_DEFAULT_METHODS( VeloTrackSkin );
        VeloTrackSkin(const LHCb::Track t) {
          // FIXME optimise me
          this->pvstate() = *t.states().front();
          this->prstate() = *t.states().back();
          //this->velohits() = ...
        }
      };
      SOASKIN( VeloBackwardTrackSkin, f_pvstate, f_vhits ) {
        SOASKIN_INHERIT_DEFAULT_METHODS( VeloBackwardTrackSkin );
      };
    }
    typedef SOA::Container<std::vector, VeloData::VeloTrackSkin> VeloTracks;
    typedef typename VeloTracks::proxy                           VeloTrack;
    typedef SOA::Container<std::vector, VeloData::VeloBackwardTrackSkin> VeloBackwardTracks;
    typedef typename VeloBackwardTracks::proxy                           VeloBackwardTrack;
  } // namespace experimental
} // namespace LHCb::Pr
