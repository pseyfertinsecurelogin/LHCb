#ifndef CALOFUTUREINTERFACES_ICALOFUTUREDIGITTOOL_H
#define CALOFUTUREINTERFACES_ICALOFUTUREDIGITTOOL_H 1
// Include files
// STD & STL
#include <functional>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
namespace LHCb{
  class    CaloDigit                ;
}


/** @class ICaloFutureDigitTool ICaloFutureDigitTool.h CaloFutureInterfaces/ICaloFutureDigitTool.h
 *
 *  The generic interface for "CalorimeterFuture tools" , which deals with
 *  CaloDigit objects, the potential candidates are:
 *
 *    - digit calibration
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   30/10/2001
 */

struct ICaloFutureDigitTool: extend_interfaces<IAlgTool>
{
  /** static interface identification
   *  @return unique interface identifier
   */
  DeclareInterfaceID( ICaloFutureDigitTool , 3 , 0 );

  /** The main processing method
   *  @param digit pointer to CaloFutureCluster object to be processed
   *  @return status code
   */
  virtual StatusCode process    ( LHCb::CaloDigit* digit ) const = 0 ;

  /** The main processing method (functor interface)
   *  @param digit pointer to CaloFutureCluster object to be processed
   *  @return status code
   */
  virtual StatusCode operator() ( LHCb::CaloDigit* digit ) const = 0 ;

};

// ============================================================================
#endif // CALOFUTUREINTERFACES_ICALOFUTUREDIGITTOOL_H
// ============================================================================
