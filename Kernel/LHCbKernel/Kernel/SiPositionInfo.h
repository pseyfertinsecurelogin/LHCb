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
#ifndef KERNEL_SIPOSITIONINFO_H
#define KERNEL_SIPOSITIONINFO_H 1

// Include files

namespace LHCb {

  /** @struct SiPositionInfo SiPositionInfo.h Kernel/SiPositionInfo.h
   *
   *  Adds position information to a channel identifier
   *
   *  @author Tomasz Szumlak
   *  @date   2006-02-27
   */

  template <class ChannelType>
  struct SiPositionInfo {
    ChannelType  strip;
    double       fractionalPosition;
    double       fractionalError;
    unsigned int clusterSize;
  };
} // namespace LHCb

#endif // KERNEL_SIPOSITIONINFO_H
