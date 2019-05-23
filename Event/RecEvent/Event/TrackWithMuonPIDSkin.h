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
#include "Event/MuonPID_v2.h"
#include "Event/TrackSkin.h"
#include "SOAContainer/SOAField.h"
#include "SOAContainer/SOASkin.h"

namespace LHCb::Event::v2 {
  // Define a type for zipping together track and muonID objects
  SOASKIN_TRIVIAL( TrackWithMuonID, TrackField, MuonPIDField );
} // namespace LHCb::Event::v2
