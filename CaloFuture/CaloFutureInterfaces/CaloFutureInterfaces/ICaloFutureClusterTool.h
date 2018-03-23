// ============================================================================
#ifndef CALOFUTUREINTERFACES_ICALOFUTURECLUSTERTOOL_H 
#define CALOFUTUREINTERFACES_ICALOFUTURECLUSTERTOOL_H 1
// Include files
// STD & STL 
#include <functional>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
// forward declarations
namespace LHCb{
  class     CaloCluster                ;
}

/** @class ICaloFutureClusterTool ICaloFutureClusterTool.h 
 *           CaloFutureInterfaces/ICaloFutureClusterTool.h
 *  
 *  The generic interface for "CalorimeterFuture tools" , which deals with 
 *  CaloCluster objects, the potential candidates are: 
 *
 *    - cluster parameters calculation  for whole cluster 
 *    - cluster parameters calculations for maximum 4x4 submatrix 
 *    - cluster parameters calculations from 3x3 submatrix 
 *    - cluster parameters calculations from "swiss-cross" sub-cluster 
 *   
 *  @author Ivan Belyaev
 *  @date   30/10/2001
 */

struct ICaloFutureClusterTool: extend_interfaces<IAlgTool>
{
  
  DeclareInterfaceID( ICaloFutureClusterTool , 3 , 0 );
  
  /** The main processing method 
   *  @param cluster pointer to CaloCluster object to be processed
   *  @return status code 
   */  
  virtual StatusCode process    ( LHCb::CaloCluster* cluster)const =0;
  
  /** The main processing method (functor interface) 
   *  @param cluster pointer to CaloCluster object to be processed
   *  @return status code 
   */  
  virtual StatusCode operator() ( LHCb::CaloCluster* cluster)const =0;

};

// ============================================================================
#endif // CALOFUTUREINTERFACES_ICALOFUTURECLUSTERTOOL_H
// ============================================================================