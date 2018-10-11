#ifndef ICALOFUTURERELATIONSGETTER_H 
#define ICALOFUTURERELATIONSGETTER_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

#include "Event/Track.h"
//Forward dec'
template <class FROM,class TO,class WEIGHT> class IRelationWeighted; 
template <class FROM,class TO,class WEIGHT> class IRelationWeighted2D; 
template <class FROM,class TO> class IRelation; 

namespace LHCb{ 
  class CaloHypo;
  class CaloCluster;
}

/** @class ICaloFutureRelationsGetter ICaloFutureRelationsGetter.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2013-10-04
 */
struct ICaloFutureRelationsGetter : extend_interfaces<IAlgTool>{

  // Return the interface ID
  DeclareInterfaceID(ICaloFutureRelationsGetter, 2, 0 );

  virtual IRelationWeighted2D< LHCb::Track , LHCb::CaloHypo , float >* getTrHypoTable2D(std::string location)=0;
  virtual IRelation< LHCb::CaloHypo , float >*  getHypoEvalTable(std::string location)=0;
  virtual IRelationWeighted< LHCb::CaloCluster , LHCb::Track , float >* getClusTrTable  (std::string location)=0;

};
#endif // ICALOFUTURERELATIONSGETTER_H
