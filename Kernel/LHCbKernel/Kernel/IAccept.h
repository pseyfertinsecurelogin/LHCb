// ============================================================================
#ifndef KERNEL_IACCEPT_H
#define KERNEL_IACCEPT_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/IAlgTool.h"
// ============================================================================
/** @class IAccept Kernel/IAccept.h
 *
 *  Generic 'accept'-interface
 *
 *  @author Vanya Belyaev
 *  @date   2010-08-02
 */
struct GAUDI_API IAccept : extend_interfaces<IAlgTool>
{
  // ==========================================================================
  /// InterfaceID
  DeclareInterfaceID(IAccept, 4, 0);
  // ==========================================================================
  /// make a generic selection
  virtual bool accept () const = 0 ;
  // ==========================================================================
};
// ============================================================================
// The END
// ============================================================================
#endif // KERNEL_ISELECTOR_H
// ============================================================================
