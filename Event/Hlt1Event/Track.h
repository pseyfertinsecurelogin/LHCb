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


namespace hlt1 {


struct Track {
    const std::vector<LHCb::VPLightCluster>* veloSegment = nullptr;
    const void* utSegment = nullptr; // TODO: use proper type.
    const void* ftSegment = nullptr; // TODO: use proper type.
};


} // namespace hlt1





