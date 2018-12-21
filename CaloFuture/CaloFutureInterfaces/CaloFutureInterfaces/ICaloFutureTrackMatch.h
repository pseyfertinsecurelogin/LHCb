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
#include "Event/Track.h"
#include "GaudiKernel/Plane3DTypes.h"
namespace LHCb{
  class CaloPosition  ; // from CaloEvent package 
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

 struct MatchResults 
 {
  //this the same or modyfied plane of calorimeter - can be the same like "detector_plane" or different
  Gaudi::Plane3D plane = Gaudi::Plane3D();
  //matching state - can be the same like "match_state" or different
  LHCb::State state;
  //when true this means there is no point to check another tracks for this calo object
  bool skip_this_calo = false;
  //when equal true means that the whole matching process finished with successful ( no fails )
  bool match_successful = false;
  //calculated chi2 value - if something goes bad this value is bad()
  double chi2_value = -1;
 };

  /** interface identification
   *  @return unique interface identifier
   */
  DeclareInterfaceID( ICaloFutureTrackMatch , 5 , 0 );

  /** the main matching method
   *  @see ICaloFutureTrackMatch
   *  @param calo_obj "calorimeter" object (position)
   *  @param track_obj tracking object (track)
   *  @param is_new_calo_obj when true means we use new calo_obj ( next one from calo objects list )
   *  @param detector_plane detector's plane - hase to be initialized in calling method
   *  @param match_state - matching state
   */
 virtual MatchResults match( const LHCb::CaloPosition& calo_obj, const LHCb::Track& track_obj, const bool& is_new_calo_obj, const Gaudi::Plane3D& detector_plane, const LHCb::State& match_state ) const = 0;
    
};
// ============================================================================
#endif // CALOFUTUREINTERFACES_ICALOFUTURETRMATCH_H
