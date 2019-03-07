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
#ifndef PHYSEVENT_LCGDICT_H
#define PHYSEVENT_LCGDICT_H 1

// Additional classes to be added to automatically generated lcgdict

// begin include files
#include "Event/Particle.h"
#include "Event/RelatedInfoMap.h"
#include "Event/Vertex.h"
#include <vector>

// end include files

namespace {
  struct _Instantiations {
    // begin instantiations
    std::vector<const LHCb::Particle*>           _i1;
    std::vector<const LHCb::Particle*>::iterator _i2;
    std::vector<const LHCb::Vertex*>             _i3;
    //
    LHCb::Particle::Selection                       _i4;
    Gaudi::NamedRange_<LHCb::Particle::ConstVector> _i5;
    Gaudi::Range_<LHCb::Particle::ConstVector>      _i6;

    LHCb::RelatedInfoMap      _i7;
    LHCb::RelatedInfoMap::MAP _i8;

    // end instantiations
  };
} // namespace
#endif // PHYSEVENT_LCGDICT_H
