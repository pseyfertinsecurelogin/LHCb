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
