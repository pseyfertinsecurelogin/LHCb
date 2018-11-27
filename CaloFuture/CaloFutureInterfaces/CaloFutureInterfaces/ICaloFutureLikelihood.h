/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef CALOFUTUREINTERFACES_ICALOFUTURELIKELIHOOD_H 
#define CALOFUTUREINTERFACES_ICALOFUTURELIKELIHOOD_H 1
// Include files
// STD & STL 
#include <functional>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
#include "Event/CaloHypo.h"
namespace LHCb{
  class  CaloCluster     ;     
}

/** @class ICaloFutureLikelihood ICaloFutureLikelihood.h CaloFutureInterfaces/ICaloFutureLikelihood.h
 *  
 *  An abstract interface for "Likelihood"-tools.
 * 
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru 
 *  @date   30/03/2002
 */
struct ICaloFutureLikelihood: extend_interfaces<IAlgTool>
{
  
  /** static interface identification
   *  @see IInterface
   *  @see IID_ICaloFutureLikelyhood
   *  @return the unique interface identifier
   */
  DeclareInterfaceID(ICaloFutureLikelihood , 2 , 0 );
  
  /** calorimeter hypothesis to be evaluated
   *  @see CaloFutureHypotheses 
   *  @return calorimeter hypothesis to be evaluated 
   */
  virtual const LHCb::CaloHypo::Hypothesis& hypothesis() const = 0 ;
  
  /** evaluate the likelyhood of the cluster for the given hypotheses
   *  @param  cluster pointer to cluster, to be evaluated (const!)
   *  @return likelyhood of the hypothesis 
   */
  virtual double likelihood ( const LHCb::CaloCluster* cluster ) const = 0 ;
  
  /** evaluate the likelyhood of the cluster for the given hypotheses
   *  (functor interface)
   *  @param  cluster pointer to cluster, to be evaluated (const!)
   *  @return likelyhood of the hypothesis 
   */
  virtual double operator() ( const LHCb::CaloCluster* cluster ) const = 0 ;
  
  
};

// ============================================================================
#endif // CALOFUTUREINTERFACES_ICALOFUTURELIKELIHOOD_H
// ============================================================================
