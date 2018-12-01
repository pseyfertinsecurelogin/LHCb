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
#ifndef KERNEL_ICHECKTOOL_H
#define KERNEL_ICHECKTOOL_H 1

// Include files

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

/** @class ICheckTool ICheckTool.h
 *  Interface class for generic checking tools
 *
 *  @author Marco Cattaneo
 *  @date   2003-02-24
 */

struct ICheckTool : extend_interfaces<IAlgTool> {
  /// access to interface id
  DeclareInterfaceID( ICheckTool, 1, 0 );

  /// Check the data
  virtual StatusCode check()=0;

};
#endif // KERNEL_ICHECKTOOL_H
