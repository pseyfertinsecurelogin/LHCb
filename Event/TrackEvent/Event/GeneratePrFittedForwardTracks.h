/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once
#include "Event/PrFittedForwardTracks.h"

namespace LHCb::Pr::Fitted::Forward {
  /** Helper for tests and benchmarks that generates a
   *  LHCb::Pr::Fitted::Forward::Tracks container that is populated with
   *  fake-but-sane data.
   */
  Tracks generate_tracks( std::size_t nTracks );
} // namespace LHCb::Pr::Fitted::Forward