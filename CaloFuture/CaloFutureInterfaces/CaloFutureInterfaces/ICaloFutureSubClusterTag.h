#ifndef CALOFUTUREINTERFACES_ICALOFUTURESUBCLUSTERTAG_H 
#define CALOFUTUREINTERFACES_ICALOFUTURESUBCLUSTERTAG_H 1
// Include files
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
// CaloFutureInterafces 
#include "CaloFutureInterfaces/ICaloFutureClusterTool.h"
#include "Event/CaloDigitStatus.h"

/** @class ICaloFutureSubClusterTag ICaloFutureSubClusterTag.h
 *  
 *
 *  @author Ivan Belyaev
 *  @date   01/04/2002
 */

struct ICaloFutureSubClusterTag : extend_interfaces<ICaloFutureClusterTool>
{
  /** static interface identification
   *  @see IInterface 
   *  @return unique interface identifier
   */
  DeclareInterfaceID(ICaloFutureSubClusterTag , 3 , 0 );
  
  /** The main method 
   *  @param cluster pointer to ClaoCluster object to be selected/tagged 
   *  @return status code 
   */
  virtual StatusCode tag(LHCb::CaloCluster* cluster)const = 0;
  
  virtual void setMask(const LHCb::CaloDigitStatus::Status mask)const=0;
  virtual unsigned int mask()const =0;

  /** The main method 
   *  @param cluster pointer to ClaoCluster object to be untagged 
   *  @return status code 
   */
  virtual StatusCode untag ( LHCb::CaloCluster* cluster ) const = 0 ;
  
};

// ============================================================================
#endif // CALOFUTUREINTERFACES_ICALOFUTURESUBCLUSTERTAG_H
// ============================================================================
