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


#include "Event/VPLightCluster.h"
#include "SOAContainer/SOAContainer.h"
#include "Event/State.h"
#include "Event/Chi2PerDoF.h"
#include "Event/UTHit.h"
#include "Kernel/STLExtensions.h"
#include <vector>


namespace LHCb::HLT1 {


struct Track {
    const std::vector<LHCb::VPLightCluster>* veloSegment = nullptr;
    const void* utSegment = nullptr; // TODO: use proper type.
    const void* ftSegment = nullptr; // TODO: use proper type.
};


namespace details {
  template <typename T, typename Tag>
    struct tagged_type{
      explicit tagged_type(T t) : value(std::move(t)) {}
      T value;
      operator T() const { return value; }
    };
}

using TrackIndex = details::tagged_type<size_t, struct TrackIndexTag>;

// TODO:
// benchmark decisions for any of the std::vector below, consider small_vector and static_vector

// out of PrPixel
SOAFIELD_TRIVIAL( f_velohits, velohits, std::vector<LHCb::VPLightCluster> ); // to PrVeloUT **for forwarding only**
SOASKIN_TRIVIAL( s_velohits , f_velohits );
SOAFIELD_TRIVIAL( f_atbeamstate, beamstate, LHCb::State );                   // to PatPV (and IP Filter)
SOASKIN( s_atbeamstate, f_atbeamstate ) {
  SOASKIN_INHERIT_DEFAULT_METHODS( s_atbeamstate );
  auto tx() const { return this->beamstate().tx(); }
  auto ty() const { return this->beamstate().ty(); }
  auto x() const { return this->beamstate().x(); }
  auto y() const { return this->beamstate().y(); }
  auto z() const { return this->beamstate().z(); }
};
SOAFIELD_TRIVIAL( f_endofvelostate, velostate, LHCb::State );                // to PrVeloUT
SOASKIN_TRIVIAL( s_endofvelostate , f_endofvelostate );
SOAFIELD_TRIVIAL( f_chi2, chisquare, LHCb::Chi2PerDoF );                     // used???????????
SOASKIN_TRIVIAL( s_chi2 , f_chi2 );
// TODO: flags and stuff
SOASKIN( IpSelectorSkin, f_velohits, f_atbeamstate ) {
  SOASKIN_INHERIT_DEFAULT_METHODS( IpSelectorSkin );
};

SOASKIN( VeloTrack, f_velohits, f_atbeamstate, f_endofvelostate, f_chi2 ) {
  SOASKIN_INHERIT_DEFAULT_METHODS( VeloTrack );
};

// out of PrVeloUT
SOAFIELD_TRIVIAL( f_uthits, uthits, std::vector<UT::Hit> );                     // to PrForward **for forwarding only**
SOASKIN_TRIVIAL( s_uthits , f_uthits );
SOAFIELD_TRIVIAL( f_viewvelohits, velohits, LHCb::span<LHCb::VPLightCluster> ); // to PrForward **for forwarding only**
SOASKIN_TRIVIAL( s_viewvelohits , f_viewvelohits );
// SOAFIELD_TRIVIAL( f_endofvelostate, velostate, LHCb::State );                // to PrForward (will add momentum wrt. PrPixel output)
SOASKIN( VeloUTTrack, f_viewvelohits, f_uthits, f_endofvelostate ) {
  SOASKIN_INHERIT_DEFAULT_METHODS( VeloUTTrack );
};

// out of PrForward

} // namespace hlt1
