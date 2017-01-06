#ifndef _IUsedLHCbID_H
#define _IUsedLHCbID_H

#include "GaudiKernel/IAlgTool.h"
#include "Kernel/LHCbID.h"


/** @class IUsedLHCbID IUsedLHCbID.h Kernel/IUsedLHCbID.h
 *
 *  interface for testing if a given id is used.
 *
 *  @author M.Needham
 *  @date   02/08/2006
 */

struct IUsedLHCbID: extend_interfaces<IAlgTool> {

  DeclareInterfaceID(IUsedLHCbID, 1, 0 );

  /** Test if the LHCbID is used
  * @param id to be test tested
  *  @return true if used
  */
  virtual bool used(const LHCb::LHCbID id) const = 0;

};

#endif
