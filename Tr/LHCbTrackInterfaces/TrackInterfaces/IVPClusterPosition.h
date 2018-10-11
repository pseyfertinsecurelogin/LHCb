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
#ifndef KERNEL_IVPCLUSTERPOSITION_H
#define KERNEL_IVPCLUSTERPOSITION_H 1

// Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/Point3DTypes.h"
// Kernel/LHCbKernel
#include "Kernel/PixelPositionInfo.h"


/** @class IVPClusterPosition IVPClusterPosition.h
 *TrackInterfaces/IVPClusterPosition.h
 *
 *
 *  @author Victor Coco
 *  @date   2010-02-02
 */

namespace LHCb {
class VPLightCluster;
}

struct IVPClusterPosition : extend_interfaces<IAlgTool> {
    DeclareInterfaceID(IVPClusterPosition, 2, 0);

  /** Calculate position of a given VPCluster
   * @param cluster pointer to a VPCluster
   * @return struct containing coordinates and errors
   * The returned error estimate depends only on the pixel size.
   */
  virtual LHCb::VPPositionInfo position(const LHCb::VPLightCluster& cluster)
      const = 0;

  /** Calculate position of a given VPCluster
   * @param cluster pointer to a VPCluster
   * @param point const reference to XYZ coordinates of track intercept
   * @param tx track xz slope
   * @param ty track yz slope
   * @return struct containing coordinates and errors
   * The returned error estimate depends both on the pixel size and
   * the projected angle of a track.
   */
  virtual LHCb::VPPositionInfo position(const LHCb::VPLightCluster& cluster,
                                        const Gaudi::XYZPoint& point,
                                        const double& tx,
                                        const double& ty) const = 0;
};
#endif  // KERNEL_IVPCLUSTERPOSITION_H
