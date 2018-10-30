/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
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
#include "Event/CaloDigit.h"
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
   *  @param  digits calo digits from TES
   *  @return the value of the estimator
   */  
  virtual std::optional<double> process( const LHCb::Track& track , const LHCb::CaloDigits& digits) const = 0 ;
  
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
