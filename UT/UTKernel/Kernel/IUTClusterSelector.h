#ifndef UTKERNEL_IUTCLUSTERSELECTOR_H
#define UTKERNEL_IUTCLUSTERSELECTOR_H 1
// Include files
#include <functional>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
// forward declaration
namespace LHCb{
  class  UTCluster     ;
}

/** @class IUTClusterSelector IUTClusterSelector.h
 *
 *  An abstract interface for cluster "Selector"/"Preselector"
 *  tools, which allow a fast and efficient selection/preselection
 *  of clusters
 *
 *
 *  @author A Beiter
 *  @date   2018-09-04
 */

struct IUTClusterSelector : extend_interfaces<IAlgTool>
{

  /** static interface identification
   *  @see IInterface
   *  @see IID_IUTClusterSelector
   *  @return the unique interface identifier
   */
  DeclareInterfaceID( IUTClusterSelector , 2 , 0 );


  /** "select"/"preselect" method
   *  @param  cluster pointer to calo cluster object to be selected
   *  @return true if cluster is selected
   */
  virtual bool select     ( const LHCb::UTCluster* cluster ) const = 0 ;

  /** "select"/"preselect" method (functor interface)
   *  @param  cluster pointer to calo cluster object to be selected
   *  @return true if cluster is selected
   */
  virtual bool operator() ( const LHCb::UTCluster* cluster ) const = 0 ;

};

// ============================================================================
#endif // UT_IUTCLUSTERSELECTOR_H
// ============================================================================
