// ============================================================================
#ifndef CALOFUTUREINTERFACES_ICALOFUTURESPLITTOOL_H 
#define CALOFUTUREINTERFACES_ICALOFUTURESPLITTOOL_H 1
// Include files
// STD & STL 
#include <functional>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
#include "Event/CaloHypo.h"
/** @class ICaloFutureSplitTool ICaloFutureSplitTool.h CaloFutureInterfaces/ICaloFutureSplitTool.h
 *
 *  An abstract interface for "cluster splitting tools", like 
 *     Bulos, or shower-shape fitting or iterative analysis.
 *
 *  @author Vanya Belyaev      Ivan.Belyaev@itep.ru
 *  @author Frederic Machefert machefer@in2p3.fr
 *  @date   19/03/2002
 */


namespace LHCb{
  class     CaloHypo     ;     
}

struct ICaloFutureSplitTool :  extend_interfaces<IAlgTool>
{

  // OD ALREADY DEFINED AS A KEYEDCONTAINER IN EVENT
  // useful type definition for output container  
  //namespace LHCb  {
  //  typedef std::vector<LHCb::CaloHypo*>  CaloFutureHypos;
  //}
  
  /** static interface identification
   *  @return unique interface identifier
   */
  DeclareInterfaceID(ICaloFutureSplitTool , 3 , 0 );
  
  /** The main processing method 
   *  @param  hypo   pointer to CaloFutureHypo object to be processed
   *  @param  hypos  the result of the splitting procedure 
   *  @return status code 
   */  
  virtual StatusCode 
  process ( LHCb::CaloHypo*  hypo  ,
            LHCb::CaloHypos& hypos ) const = 0 ;
  
  /** The main processing method (functor interface)
   *  @param  hypo   pointer to CaloHypo object to be processed
   *  @param  hypos  the result of the splitting procedure 
   *  @return status code 
   */  
  virtual StatusCode 
  operator() ( LHCb::CaloHypo*  hypo  ,
               LHCb::CaloHypos& hypos ) const = 0 ;
  
};

#endif // CALOFUTUREINTERFACES_ICALOFUTURESPLITTOOL_H
