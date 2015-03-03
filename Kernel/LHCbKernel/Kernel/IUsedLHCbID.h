// $Id: IUsedLHCbID.h,v 1.2 2006-12-06 16:28:53 cattanem Exp $ 
#ifndef _IUsedLHCbID_H
#define _IUsedLHCbID_H
 
#include "GaudiKernel/IAlgTool.h"
 
static const InterfaceID IID_IUsedLHCbID( "IUsedLHCbID", 0, 0 );
 
/** @class IUsedLHCbID
 *
 *  interface for testing ida given id is used. 
 *
 *  @author M.Needham
 *  @date   02/08/2006
 */

namespace LHCb{
 class LHCbID;
};
  
class IUsedLHCbID: virtual public IAlgTool{
 
public:
                                                                                
  /** Retrieve interface ID */
  static const InterfaceID& interfaceID() { return IID_IUsedLHCbID ; }

  /** Test if the LHCbID is used 
  * @param id to be test tested
  *  @return true if used
  */  
  virtual bool used(const LHCb::LHCbID id) const = 0;
                                                                                
};
                                                                                
#endif
