#ifndef IFORCEDBDECAYTOOL_H 
#define IFORCEDBDECAYTOOL_H 1

#include <string>
#include "GaudiKernel/IAlgTool.h"

namespace LHCb {
  class MCParticle;
}


/** @class IForcedBDecayTool IForcedBDecayTool.h 
 *  
 *  v1.2
 *  @author Marco Musy (Milano)
 *  @date   2007-02-06
 */
struct IForcedBDecayTool : extend_interfaces<IAlgTool> {
  DeclareInterfaceID(IForcedBDecayTool, 2 , 0); 
  virtual const LHCb::MCParticle*  forcedB( void )=0;
};
#endif // IFORCEDBDECAYTOOL_H
