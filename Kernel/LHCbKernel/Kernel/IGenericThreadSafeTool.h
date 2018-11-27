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

// Include files
#include "GaudiAlg/IGenericTool.h"

/** General purpose interface class for tools that "do something"
 *  in a thread safe way.
 *
 *  @author Marco Clemencic
 *  @date   2016-12-09
 */
class GAUDI_API IGenericThreadSafeTool: virtual public IGenericTool {
public:
  /// InterfaceID
  DeclareInterfaceID(IGenericThreadSafeTool, 1, 0);

  /// Do the action (thread safe).
  virtual void execute() const = 0;

  /// Allow use of IGenericThreadSafeTool as IGenericTool.
  void execute() override final {
    const_cast<const IGenericThreadSafeTool*>(this)->execute();
  }
};
