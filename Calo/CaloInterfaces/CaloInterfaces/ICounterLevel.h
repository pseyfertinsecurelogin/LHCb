#ifndef ICOUNTERLEVEL_H 
#define ICOUNTERLEVEL_H 1

// from Gaudi
#include "GaudiKernel/IAlgTool.h"


/** @class ICounterLevel ICounterLevel.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2016-08-13
 */
struct ICounterLevel : extend_interfaces<IAlgTool> {

  DeclareInterfaceID( ICounterLevel, 1, 0 );

  virtual bool isQuiet() const = 0;
  virtual bool isVerbose() const = 0;
  virtual bool isLevel(int l) const=0;
  virtual int  level() const=0;

};
#endif // ICOUNTERLEVEL_H
