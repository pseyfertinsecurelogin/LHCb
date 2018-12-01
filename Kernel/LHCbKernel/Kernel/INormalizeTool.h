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
#ifndef KERNEL_INORMALIZETOOL_H
#define KERNEL_INORMALIZETOOL_H 1

// Include files
#include "GaudiKernel/IAlgTool.h"


/** @class INormalizeTool INormalizeTool.h Kernel/INormalizeTool.h
 *  Interface class for generic normalisation tool
 *
 *  @author Marco Cattaneo
 *  @date   2004-07-14
 */
struct INormalizeTool : extend_interfaces<IAlgTool> {

  /// Retrieve interface ID
  DeclareInterfaceID(INormalizeTool, 2, 0);

  /// Return a normalization value
  virtual double normalize() = 0;
};
#endif // KERNEL_INORMALIZETOOL_H
