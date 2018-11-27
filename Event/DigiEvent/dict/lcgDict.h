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
#ifndef DIGIEVENT_LCGDICT_H 
#define DIGIEVENT_LCGDICT_H 1

// Additional classes to be added to automatically generated lcgdict

// begin include files
#include "Kernel/FastClusterContainer.h"
#include "Event/VeloLiteCluster.h"
#include "Event/STLiteCluster.h"

// end include files

namespace {
  struct _Instantiations {
    // begin instantiations
    FastClusterContainer<LHCb::VeloLiteCluster, int> _i1;
    FastClusterContainer<LHCb::STLiteCluster, int>   _i2;
    // end instantiations
  };
}

#endif // DIGIEVENT_LCGDICT_H
