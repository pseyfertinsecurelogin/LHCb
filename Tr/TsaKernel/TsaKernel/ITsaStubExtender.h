// $Id: ITsaStubExtender.h,v 1.2 2007-03-01 12:08:55 cattanem Exp $
#ifndef _ITsaStubExtender_H
#define _ITsaStubExtender_H

/** @class ITsaStubExtender ITsaStubExtender.h TsaKernel/ITsaStubExtender.h
 *
 *  Interface to data svc
 *
 *  @author M.Needham
 *  @date   12/11/2006
 */

#include "GaudiKernel/IAlgTool.h"

static const InterfaceID IID_ITsaStubExtender( "ITsaStubExtender", 0, 0 );

#include <vector>

class SeedStub;
class SeedTrack;
class SeedHit;

class ITsaStubExtender: virtual public IAlgTool {
public:

  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_ITsaStubExtender ; }

  virtual StatusCode execute(int& sect, std::vector<SeedStub*> stubs[], std::vector<SeedHit*> hits[], 
                             std::vector<SeedHit*> sHits[], std::vector<SeedTrack*>& seeds )= 0;

};

#endif
