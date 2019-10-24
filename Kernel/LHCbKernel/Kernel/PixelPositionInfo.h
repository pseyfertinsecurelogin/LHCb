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
#ifndef KERNEL_PIXELPOSITIONINFO_H
#define KERNEL_PIXELPOSITIONINFO_H 1

#include "Kernel/VPChannelID.h"

namespace LHCb {

  /** @struct PixelPositionInfo PixelPositionInfo.h Kernel/PixelPositionInfo.h
   *
   *  Adds 2D position information to a channel identifier
   *
   *  @author Victor Coco
   *  @date   2010-02-01
   */

  template <class ChannelType>
  struct PixelPositionInfo {
    double x;
    double y;
    double dx;
    double dy;
  };

  using VPPositionInfo = PixelPositionInfo<VPChannelID>;

} // namespace LHCb

#endif // KERNEL_PIXELPOSITIONINFO_H
