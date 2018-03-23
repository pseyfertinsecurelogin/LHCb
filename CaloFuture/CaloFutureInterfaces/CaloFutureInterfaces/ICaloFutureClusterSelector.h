// ============================================================================
#ifndef CALOFUTUREINTERFACES_ICALOFUTURECLUSTERSELECTOR_H 
#define CALOFUTUREINTERFACES_ICALOFUTURECLUSTERSELECTOR_H 1
// Include files
// STD & STL 
#include <functional>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
// forward declaration
namespace LHCb{
  class  CaloCluster     ;    
}

/** @class ICaloFutureClusterSelector ICaloFutureClusterSelector.h
 *  
 *  An abstract interface for cluster "Selector"/"Preselector" 
 *  tools, which allow a fast and efficient selection/preselection 
 *  of clusters 
 *
 *  @see CaloHypotheses
 *  @see CaloCluster
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   31/03/2002
 */

struct ICaloFutureClusterSelector : extend_interfaces<IAlgTool>
{
  
  DeclareInterfaceID( ICaloFutureClusterSelector , 2 , 0 );
  
  /** "select"/"preselect" method 
   *  @param  cluster pointer to calo cluster object to be selected 
   *  @return true if cluster is selected
   */
  virtual bool select     ( const LHCb::CaloCluster* cluster ) const = 0 ;
  
  /** "select"/"preselect" method (functor interface)
   *  @param  cluster pointer to calo cluster object to be selected 
   *  @return true if cluster is selected
   */
  virtual bool operator() ( const LHCb::CaloCluster* cluster ) const = 0 ;
  
};
  
// ============================================================================
#endif // CALOFUTUREINTERFACES_ICALOFUTURECLUSTERSELECTOR_H
// ============================================================================