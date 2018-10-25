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
#include "Event/Track_v1.h"

namespace LHCb{
  using Track = Event::Track; // for backwards compatibility, to be deprecated one day...
  using Tracks = Event::Tracks; // for backwards compatibility, to be deprecated one day...
  namespace TrackLocation = Event::TrackLocation;
  static const CLID CLID_Track = Event::CLID_Track;

}
