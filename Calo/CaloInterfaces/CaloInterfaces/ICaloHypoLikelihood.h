#ifndef CALOINTERFACES_ICALOHYPOLIKELIHOOD_H 
#define CALOINTERFACES_ICALOHYPOLIKELIHOOD_H 1
// Include files
// STD & STL 
#include <functional>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
#include "Event/CaloHypo.h"


struct ICaloHypoLikelihood: extend_interfaces<IAlgTool>
{
  
  /** static interface identification
   *  @see IInterface
   *  @see IID_ICaloHypoLikelihood
   *  @return the unique interface identifier
   */
  DeclareInterfaceID( ICaloHypoLikelihood , 2 , 0 );
  
  /** calorimeter hypothesis to be evaluated
   *  @see CaloHypotheses 
   *  @return calorimeter hypothesis to be evaluated 
   */
  virtual const  LHCb::CaloHypo::Hypothesis& hypothesis() const = 0 ;
  
  /** evaluate the likelihood of the cluster for the given hypotheses
   *  @param  hypo pointer to cluster, to be evaluated (const!)
   *  @return likelyhood of the hypothesis 
   */
  virtual double likelihood ( const LHCb::CaloHypo* hypo ) const = 0 ;
  
  /** evaluate the likelihood of the cluster for the given hypotheses
   *  (functor interface)
   *  @param  cluster pointer to cluster, to be evaluated (const!)
   *  @return likelyhood of the hypothesis 
   */
  virtual double operator() ( const LHCb::CaloHypo* cluster ) const = 0 ;
  
  
};

// ============================================================================
#endif // CALOINTERFACES_ICALOHYPOLIKELIHOOD_H
// ============================================================================



