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
#ifndef IPART2CALOFUTURE_H
#define IPART2CALOFUTURE_H 1

// Include files
// from STL
#include <string>

// from CaloFutureInterfaces
#include "CaloFutureInterfaces/ITrack2CaloFuture.h"

namespace LHCb {
  class Particle;
  class ProtoParticle;
} // namespace LHCb

/** @class IPart2CaloFuture IPart2CaloFuture.h Kernel/IPart2CaloFuture.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2007-06-25
 */
struct IPart2CaloFuture : extend_interfaces<ITrack2CaloFuture> {
  // Return the interface ID
  DeclareInterfaceID( IPart2CaloFuture, 2, 0 );

  using ITrack2CaloFuture::match;
  virtual bool match( const LHCb::Particle* part, std::string det = DeCalorimeterLocation::Ecal,
                      CaloPlane::Plane plane = CaloPlane::ShowerMax, double delta = 0. ) = 0;
  virtual bool match( const LHCb::ProtoParticle* proto, std::string det = DeCalorimeterLocation::Ecal,
                      CaloPlane::Plane plane = CaloPlane::ShowerMax, double delta = 0. ) = 0;

  virtual bool inAcceptance() = 0;
};
#endif // IPART2CALOFUTURE_H
