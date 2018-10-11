#ifndef IFUTURECOUNTERLEVEL_H 
#define IFUTURECOUNTERLEVEL_H 1

// from Gaudi
#include "GaudiKernel/IAlgTool.h"


/** @class IFutureCounterLevel IFutureCounterLevel.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2016-08-13
 */
struct IFutureCounterLevel : extend_interfaces<IAlgTool> {

  DeclareInterfaceID( IFutureCounterLevel, 1, 0 );

  virtual bool isQuiet() const = 0;
  virtual bool isVerbose() const = 0;
  virtual bool isLevel(int l) const=0;
  virtual int  level() const=0;

};
#endif // IFUTURECOUNTERLEVEL_H
