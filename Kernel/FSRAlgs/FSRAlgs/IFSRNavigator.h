#ifndef IFSRNAVIGATOR_H
#define IFSRNAVIGATOR_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// forward declarations
namespace LHCb {
  class LumiFSR;
  class LumiIntegral;
}
class IRegistry;


/** @class IFSRNavigator IFSRNavigator.h
 *
 *
 *  @author Jaap Panman
 *  @date   2010-10-05
 */
class IFSRNavigator : public extend_interfaces<IAlgTool> {
public:
  DeclareInterfaceID(IFSRNavigator, 2, 0 );

  // return list of addresses found below the address
  virtual std::vector< std::string > navigate(std::string, std::string) = 0;

protected:
  virtual void explore(IRegistry* pObj, std::string tag, std::vector< std::string >& addresses) = 0;

};
#endif // IFSRNAVIGATOR_H
