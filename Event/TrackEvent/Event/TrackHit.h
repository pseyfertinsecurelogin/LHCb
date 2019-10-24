/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

#pragma once

#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include <limits>
#include <utility>

namespace LHCb {

  struct TrackHit {
    TrackHit() = default;
    TrackHit( Gaudi::XYZPointF beginPoint, Gaudi::XYZPointF endPoint, float errorx, float errory, int layer )
        : beginPoint( std::move( beginPoint ) )
        , endPoint( std::move( endPoint ) )
        , errorx( errorx )
        , errory( errory )
        , layer( layer ) {}

    Gaudi::XYZPointF beginPoint = {std::numeric_limits<float>::signaling_NaN(),
                                   std::numeric_limits<float>::signaling_NaN(),
                                   std::numeric_limits<float>::signaling_NaN()};
    Gaudi::XYZPointF endPoint   = {std::numeric_limits<float>::signaling_NaN(),
                                 std::numeric_limits<float>::signaling_NaN(),
                                 std::numeric_limits<float>::signaling_NaN()};
    float            errorx     = std::numeric_limits<float>::signaling_NaN();
    float            errory     = std::numeric_limits<float>::signaling_NaN();
    int              layer      = std::numeric_limits<int>::max();
  };

} // namespace LHCb
