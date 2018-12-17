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

#include "Kernel/VPChannelID.h"
#include "GaudiKernel/ClassID.h"

#include <vector>


namespace LHCb
{

  // Namespace for locations in TDS.
  namespace VPClusterLocation
  {
    inline const std::string Light = "Raw/VP/LightClusters";
  }


  struct VPLightCluster
  {
    VPLightCluster() = default;
    VPLightCluster(float x, float y, float z, float errx, float erry, unsigned id)
      : x(x), y(y), z(z), errx(errx), erry(erry), id(id) {}

    float x; ///< Global x coordinate.
    float y; ///< Global y coordinate.
    float z; ///< Global z coordinate.
    float errx; ///< Global x measurement error.
    float erry; ///< Global y measurement error.
    VPChannelID id; ///< ChannelID of the cluster.

    /// Used for MC linking. Return the CLID of VPCluster for backwards compatibility.
    /// TODO: Remove this from light clusters.
    static inline CLID classID() { return 4504; }
  };

  // TODO: This is unnecessary to define here, remove it.
  typedef std::vector<VPLightCluster> VPLightClusters;

} // namespace LHCb;
