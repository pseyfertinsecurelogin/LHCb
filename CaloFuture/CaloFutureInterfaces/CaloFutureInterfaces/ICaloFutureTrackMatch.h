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
// ============================================================================
#ifndef CALOFUTUREINTERFACES_ICALOFUTURETRACKMATCH_H
#define CALOFUTUREINTERFACES_ICALOFUTURETRACKMATCH_H 1
// Include files
// STD & STL
#include <functional>
#include <utility>
// GaudiKernel 
#include "GaudiKernel/IAlgTool.h"
#include "Math/Plane3D.h"
#include "Event/State.h"
namespace LHCb{
  class CaloPosition  ; // from CaloEvent package 
  class Track         ; // from TrackEvent   package
  class State         ; // from TrackEvent   package
}

/** @class ICaloTrackMatch ICaloTrackMatch.h CaloInterfaces/ICaloTrackMatch.h
 *  
 *  The generic interface for matching of 
 *  calorimeter object with tracking object 
 *
 *  The potential implementation candidates are:
 *
 *    - full matching of CaloParticle/CaloHypo object with Track 
 *      for electron/positron PID. The energy is taken into account 
 *      for matching procedure
 *    - matching of position of cluster with track for photon 
 *      selection/identification. The energy is not taken 
 *      into account in the matching procedure 
 *    - matching of track DIRECTION before magnet with 
 *      CaloCluster/CaloHypo/CaloParticle for searching for 
 *      bremmstrahlung photons. Energy is not taken 
 *      into account.
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @author Dima  Rusinov Dmitri.Roussinov@cern.ch
 *  @author Konstantin Beloous Konstantin.Beloous@cern.ch
 *  @date   30/10/2001
 */

struct  ICaloFutureTrackMatch: extend_interfaces<IAlgTool>
{
  
  struct MatchStatus
  {
   const LHCb::CaloPosition* m_cBad;//itd..
   const LHCb::Track* m_tBad;
   ROOT::Math::Plane3D m_plane;
   const LHCb::CaloPosition* m_position;
   double chi2;
   StatusCode status;
   LHCb::State m_state;
  };


  /** useful typedef for result
   *  - it is just a pair of status  and chi2 of the matching  
   */
  typedef std::pair<StatusCode,double>    MatchingPair;
  
  /** interface identification
   *  @return unique interface identifier
   */
  DeclareInterfaceID( ICaloFutureTrackMatch , 5 , 0 );
  
  /** the main matching method  
   *
   *  @param caloObj  pointer to "calorimeter" object (position)
   *  @param trObj    pointer to tracking object (track)
   *  @param chi2     returned value of chi2 of the matching
   *  @return status code for matching procedure 
   */
  virtual MatchStatus match 
  ( const LHCb::CaloPosition*   caloObj  , 
    const LHCb::Track*  trObj    ,
    const MatchStatus& ms_old     ) const = 0 ;
  
  /** The main matching method (Stl interface) 
   *  @param caloObj  pointer to "calorimeter" object (position)
   *  @param trObj    pointer to tracking object (track)
   *  @return pair of status code/chi2  for matching procedure 
   */
  virtual MatchingPair    operator() 
    ( const LHCb::CaloPosition*   caloObj  , 
      const LHCb::Track*  trObj    ) = 0 ;
  
  /** extract the TrState which was actually used for last matching
   *  @attention TrState is owned by the tool itself 
   *  The better solution could be to return the pointer 
   *  to TrStateP 
   *  @return pointer to the state actually used for last matching
   */
  virtual const LHCb::State* state   () const = 0 ;;
  
  
};
// ============================================================================
#endif // CALOFUTUREINTERFACES_ICALOFUTURETRMATCH_H
