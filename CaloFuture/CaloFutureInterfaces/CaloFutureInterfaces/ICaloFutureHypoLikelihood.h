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
#ifndef CALOFUTUREINTERFACES_ICALOFUTUREHYPOLIKELIHOOD_H 
#define CALOFUTUREINTERFACES_ICALOFUTUREHYPOLIKELIHOOD_H 1
// Include files
// STD & STL 
#include <functional>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
#include "Event/CaloHypo.h"


struct ICaloFutureHypoLikelihood: extend_interfaces<IAlgTool>
{
  
  /** static interface identification
   *  @see IInterface
   *  @see IID_ICaloFutureHypoLikelihood
   *  @return the unique interface identifier
   */
  DeclareInterfaceID( ICaloFutureHypoLikelihood , 2 , 0 );
  
  /** calorimeter hypothesis to be evaluated
   *  @see CaloFutureHypotheses 
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
#endif // CALOFUTUREINTERFACES_ICALOFUTUREHYPOLIKELIHOOD_H
