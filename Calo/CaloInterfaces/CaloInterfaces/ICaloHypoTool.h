// ============================================================================
#ifndef CALOINTERFACES_ICALOHYPOTOOL_H 
#define CALOINTERFACES_ICALOHYPOTOOL_H 1
// Include files
// STD & STL 
#include <functional>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
namespace LHCb{
  class    CaloHypo                ;  
}
/** @class ICaloHypoTool ICaloHypoTool.h CaloInterfaces/ICaloHypoTool.h
 *
 *  The generic interface for "Calorimeter tools" , which deals with 
 *  CaloHypo objects, the potential candidates are: 
 *
 *    \li hypothesis processing 
 *    \li dispatching 
 *    \li subcomponent of CaloParticle processing  
 *   
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru 
 *  @date   30/10/2001
 */

struct ICaloHypoTool: extend_interfaces<IAlgTool>
{
  
  /** static interface identification
   *  @see IInterface
   *  @return unique interface identifier
   */
  DeclareInterfaceID( ICaloHypoTool , 3 , 0 );
  
  /** The main processing method 
   *  @param  hypo  pointer to CaloHypo object to be processed
   *  @return status code 
   */  
  virtual StatusCode process    ( LHCb::CaloHypo* hypo  ) const = 0 ;
  
  /** The main processing method (functor interface)
   *  @param  hypo  pointer to CaloHypo object to be processed
   *  @return status code 
   */  
  virtual StatusCode operator() ( LHCb::CaloHypo* hypo  ) const = 0 ;
  
};

// ============================================================================
#endif // CALOINTERFACES_ICALOCLUSTERTOOL_H
// ============================================================================
