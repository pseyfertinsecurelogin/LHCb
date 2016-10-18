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
