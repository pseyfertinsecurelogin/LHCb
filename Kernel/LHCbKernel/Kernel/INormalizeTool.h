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
