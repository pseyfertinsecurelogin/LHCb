#ifndef IGENFSROUTSVC_H 
#define IGENFSROUTSVC_H 1

// Include files
#include "GaudiKernel/IService.h"

/** @class IGenFSROutSvc IGenFSROutSvc.h
 *  
 *
 *  @author Davide Fazzini
 *  @date   2018-07-16
 */

class IGenFSROutSvc : public extend_interfaces<IService> 
{ 
public:

  /// Return the interface ID
  DeclareInterfaceID(IGenFSROutSvc, 2, 0);
  
  /// Print the genFSR content in diffetent format (.xml, .html, .json)
  /// Return 0 if there is no genFSR stored in TES
  virtual void printFSR() = 0;

};
#endif // IGENFSROUTSVC_H
