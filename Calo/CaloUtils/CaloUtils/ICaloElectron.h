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
#ifndef ICALOELECTRON_H
#define ICALOELECTRON_H 1

// Include files
// from STL
#include <string>

#include "CaloInterfaces/IPart2Calo.h"
// from LHCb
#include "CaloUtils/CaloMomentum.h"

// Forward declarations
namespace LHCb {
  class Particle;
  class ProtoParticle;
} // namespace LHCb

/** @class ICaloElectron ICaloElectron.h Kernel/ICaloElectron.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2006-11-30
 */
struct ICaloElectron : public extend_interfaces<IPart2Calo> {

  // Return the interface ID
  DeclareInterfaceID( ICaloElectron, 4, 0 );

  virtual bool               set( const LHCb::Particle* particle, std::string det = DeCalorimeterLocation::Ecal,
                                  CaloPlane::Plane plane = CaloPlane::ShowerMax, double delta = 0 ) = 0;
  virtual bool               set( const LHCb::ProtoParticle* proto, std::string det = DeCalorimeterLocation::Ecal,
                                  CaloPlane::Plane plane = CaloPlane::ShowerMax, double delta = 0 ) = 0;
  virtual double             eOverP()                                                               = 0;
  virtual double             ecalE()                                                                = 0;
  virtual LHCb::CaloHypo*    electron()                                                             = 0;
  virtual LHCb::CaloHypo*    bremstrahlung()                                                        = 0;
  virtual LHCb::CaloMomentum bremCaloMomentum()                                                     = 0;

  using ITrack2Calo::closestState;
  virtual LHCb::State closestState( std::string toWhat = "hypo" ) = 0;

  virtual double caloTrajectoryZ( CaloPlane::Plane refPlane = CaloPlane::ShowerMax, std::string toWhat = "hypo" ) = 0;
  virtual double caloTrajectoryL( CaloPlane::Plane refPlane = CaloPlane::ShowerMax, std::string toWhat = "hypo" ) = 0;
};
#endif // ICALOELECTRON_H
