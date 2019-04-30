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
#ifndef TrackSkin_H
#define TrackSkin_H 1

// Include files
#include "Event/Track_v2.h"
#include "SOAContainer/SOASkin.h"
#include "SOAContainer/SOAField.h"

namespace LHCb::Event {
  namespace v2 {
    SOAFIELD_TRIVIAL( TrackField, track, Track );
    SOASKIN_TRIVIAL( TrackSkin, TrackField );
  } // namespace v2
} // namespace LHCb::Event

#endif /// TrackEvent_v2_H
