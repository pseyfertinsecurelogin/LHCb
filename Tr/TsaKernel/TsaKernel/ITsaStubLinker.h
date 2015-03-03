// $Id: ITsaStubLinker.h,v 1.2 2007-03-01 12:08:55 cattanem Exp $
#ifndef _ITsaStubLinker_H
#define _ITsaStubLinker_H

/** @class ITsaStubLinker ITsaStubLinker.h TsaKernel/ITsaStubLinker.h
 *
 *  Interface to data svc
 *
 *  @author M.Needham
 *  @date   12/11/2006
 */

#include "GaudiKernel/IAlgTool.h"

static const InterfaceID IID_ITsaStubLinker( "ITsaStubLinker", 0, 0 );

#include <vector>

class SeedStub;
class SeedTrack;

class ITsaStubLinker: virtual public IAlgTool {
public:

  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_ITsaStubLinker ; }

  virtual StatusCode execute(  std::vector<SeedStub*> stubs[], std::vector<SeedTrack*>& seeds  ) = 0;

};

#endif
