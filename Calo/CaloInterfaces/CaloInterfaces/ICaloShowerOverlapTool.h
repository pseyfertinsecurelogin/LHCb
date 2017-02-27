#ifndef ICALOSHOWEROVERLAPTOOL_H 
#define ICALOSHOWEROVERLAPTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

namespace LHCb{
  class CaloCluster;
}


/** @class ICaloShowerOverlapTool ICaloShowerOverlapTool.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2014-06-03
 */
struct ICaloShowerOverlapTool : extend_interfaces<IAlgTool> 
{
  // Return the interface ID
  DeclareInterfaceID(ICaloShowerOverlapTool, 2, 0 );

  virtual StatusCode setProfile(std::string)=0;
  virtual void process(const LHCb::CaloCluster* c1,const LHCb::CaloCluster* c2,
                       int spd=0,int niter=5,bool propagateInitialWeights=false)=0;
  
};
#endif // ICALOSHOWEROVERLAPTOOL_H
