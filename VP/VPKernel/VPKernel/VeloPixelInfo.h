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
#ifndef VELOPIXELINFO_H
#define VELOPIXELINFO_H 1

// Include files

/** Constant information of the detector used to size the arrays
 *
 *
 *  @author Renato Quagliani
 *  @date   2018-02-21
 *
 *  moved from REC/Pr/PrKernel/PrKernel to LHCB on 20180710
 */
namespace VeloInfo {
  enum Numbers {
    NModules     = 52,
    NOffsets     = 53, // NModules + 1
    NChipCorners = 6,
  };
}
#endif // VELOPIXELINFO_H
