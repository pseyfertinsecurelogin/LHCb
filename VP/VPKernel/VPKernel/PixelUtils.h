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
#ifndef PRPIXELUTILS_H
#define PRPIXELUTILS_H

/** @file PrPixelUtils.h
 *
 *  Useful named constants and utilities for algorithms and
 *  tools in PrPixel.
 *
 *  @author Kurt Rinnert
 *  @date   2014-03-10
 *
 *  moved from REC/Pr/PrKernel/PrKernel to LHCB on 20180710
 */

namespace Pixel {

/// named constants for module layout
enum {
  CHIP_COLUMNS = 256,
  CHIP_0_END = CHIP_COLUMNS - 1,
  CHIP_1_START = CHIP_COLUMNS,
  CHIP_1_END = 2 * CHIP_COLUMNS - 1,
  CHIP_2_START = 2 * CHIP_COLUMNS,
};

}

#endif  // PRPIXELUTILS_H
