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
#ifndef ITRACK2CALOFUTURE_H
#define ITRACK2CALOFUTURE_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
// from LHCb
#include "CaloDet/DeCalorimeter.h"
#include "Event/CaloPosition.h"
#include "Event/State.h"
#include "Event/Track.h"
#include "Kernel/CaloCellID.h"
#include "Kernel/TrackDefaultParticles.h"

// Forward declarations
namespace LHCb {
  class CaloHypo;
  class CaloCluster;
} // namespace LHCb

/** @class ITrack2CaloFuture ITrack2CaloFuture.h Kernel/ITrack2CaloFuture.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2007-06-25
 */
struct ITrack2CaloFuture : extend_interfaces<IAlgTool> {
  // Return the interface ID
  DeclareInterfaceID( ITrack2CaloFuture, 3, 0 );

  virtual bool match( const LHCb::Track* track, std::string det = DeCalorimeterLocation::Ecal,
                      CaloPlane::Plane plane = CaloPlane::ShowerMax, double delta = 0.,
                      LHCb::Tr::PID pid = LHCb::Tr::PID::Pion() ) = 0;

  virtual LHCb::State      caloState()  = 0;
  virtual LHCb::CaloCellID caloCellID() = 0;
  virtual bool             isValid()    = 0;
  //
  virtual LHCb::State        closestState( LHCb::CaloHypo* hypo, LHCb::Tr::PID pid = LHCb::Tr::PID::Pion() )       = 0;
  virtual LHCb::State        closestState( LHCb::CaloCluster* cluster, LHCb::Tr::PID pid = LHCb::Tr::PID::Pion() ) = 0;
  virtual LHCb::State        closestState( LHCb::CaloPosition calopos, LHCb::Tr::PID pid = LHCb::Tr::PID::Pion() ) = 0;
  virtual LHCb::State        closestState( LHCb::CaloCellID cellID, LHCb::Tr::PID pid = LHCb::Tr::PID::Pion() )    = 0;
  virtual const LHCb::Track* track()                                                                               = 0;
};
#endif // ITRACK2CALOFUTURE_H
