// $Id: ITsaStubFind.h,v 1.1 2006-12-06 14:35:19 mneedham Exp $
#ifndef _ITsaStubFind_H
#define _ITsaStubFind_H

/** @class ITsaStep TsaKernel/ITsaStep
 *
 *  Interface to data svc
 *
 *  @author M.Needham
 *  @date   12/11/2006
 */

#include "GaudiKernel/IAlgTool.h"

static const InterfaceID IID_ITsaStubFind( "ITsaStubFind", 0, 0 );

#include <vector>

class SeedStub;
class SeedHit;

class ITsaStubFind: virtual public IAlgTool {
public:

  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_ITsaStubFind ; }

  virtual StatusCode execute(std::vector<SeedHit*> hits[], std::vector<SeedHit*> sHits[], 
                             std::vector<SeedStub*> stubs[]  ) = 0;

};

#endif
