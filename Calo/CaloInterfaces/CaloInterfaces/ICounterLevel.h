#ifndef ICOUNTERLEVEL_H 
#define ICOUNTERLEVEL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

static const InterfaceID IID_ICounterLevel ( "ICounterLevel", 1, 0 );

/** @class ICounterLevel ICounterLevel.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2016-08-13
 */
class ICounterLevel : virtual public IAlgTool {
public: 

  // Return the interface ID
  static const InterfaceID& interfaceID() { return IID_ICounterLevel; }

  virtual StatusCode initialize()=0;
  virtual StatusCode finalize()=0;
  virtual bool isQuiet()const = 0;
  virtual bool isVerbose()const = 0;
  virtual bool isLevel(int l)const=0;
  virtual int  level()const=0;

protected:

private:

};
#endif // ICOUNTERLEVEL_H
