// ============================================================================
#ifndef CALOFUTUREINTERFACES_ICALOFUTURETRACKIDEVAL_H 
#define CALOFUTUREINTERFACES_ICALOFUTURETRACKIDEVAL_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
// ============================================================================
// forward declaration
// ============================================================================
#include "Event/Track.h"
// ============================================================================
/** @class ICaloFutureTrackIdEval ICaloFutureTrackIdEval.h CaloFutureInterfaces/ICaloFutureTrackIdEval.h
 *  
 *  The abstract interface for generic track ID estimator 
 *  with calorimeters  
 *
 *  The obvious candidates for implementation
 *   - e+/e-   identification with Prs/Spd 
 *   - e+/e-   identification with Ecal  
 *   - e+/e-   identification with Hcal  
 *   - mu+/mu- identification with Hcal  
 *  
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   2002-11-10
 */
struct ICaloFutureTrackIdEval : extend_interfaces<IAlgTool>
{
  // ==========================================================================
  /** The main processing method 
   *  It evaluated the Track ID estimators using the calorimeter information  
   *  @param  track  pointer to the object to be processed
   *  @param  value  (return) the value of the estimator
   *  @return status code 
   */  
  virtual StatusCode process   
  ( const LHCb::Track* track , 
    double&              value ) const = 0 ;
  
  /** The main processing method (functor interface)
   *  It evaluated the Track ID estimators using the calorimeter information  
   *  @param  track  pointer to the object to be processed
   */  
  virtual double     operator() 
    ( const LHCb::Track* track ) const = 0 ;
  // ==========================================================================  
  /** static interface identification
   *  @see IInterface
   *  @return unique interface identifier
   */
  DeclareInterfaceID( ICaloFutureTrackIdEval , 4 , 0 );
  // ==========================================================================
};
// ============================================================================
#endif // CALOFUTUREINTERFACES_ICALOFUTURETRACKIDEVAL_H
// ============================================================================
