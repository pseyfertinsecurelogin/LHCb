// $Id: ITsaSeedStep.h,v 1.1 2006-12-06 14:35:19 mneedham Exp $
#ifndef _ITsaSeedStep_H
#define _ITsaSeedStep_H

/** @class ITsaStep TsaKernel/ITsaStep
 *
 *  Interface to data svc
 *
 *  @author M.Needham
 *  @date   12/11/2006
 */

#include "GaudiKernel/IAlgTool.h"

static const InterfaceID IID_ITsaSeedStep( "ITsaSeedStep", 0, 0 );

#include <vector>

class SeedTrack;
class SeedHit;

namespace LHCb {
 class State;
}

class ITsaSeedStep: virtual public IAlgTool {
public:

  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_ITsaSeedStep ; }

  virtual StatusCode execute(std::vector<SeedTrack*>& seeds, std::vector<SeedHit*> hits[6] = 0 ) = 0;

  virtual StatusCode execute(LHCb::State& hint, std::vector<SeedTrack*>& seeds, std::vector<SeedHit*> hits[6] = 0 ) = 0;

};

#endif
