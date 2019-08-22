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
#include "Event/Track_v2.h"
#include "SOAContainer/SOAField.h"
#include "SOAContainer/SOASkin.h"

namespace LHCb::Event::v2 {
  // clang-format off
  SOAFIELD( TrackField, Track, SOAFIELD_ACCESSORS( track )
            auto pt() const { return track().pt(); }
            auto pseudoRapidity() const { return track().pseudoRapidity(); }
            auto chi2PerDoF() const {return track().chi2PerDoF() ;}
            template <typename... Args>
            auto stateAt( Args&&... args ) const {
              return track().stateAt( std::forward<Args>( args )... );
            }
            decltype( auto ) closestToBeamStatePos() const {
              return track().stateAt( LHCb::State::ClosestToBeam )->position();
            }
            decltype( auto ) closestToBeamStateDir() const {
              return track().stateAt( LHCb::State::ClosestToBeam )->slopes();
            }
            );
  // clang-format on
  SOASKIN_TRIVIAL( TrackSkin, TrackField );
} // namespace LHCb::Event::v2
