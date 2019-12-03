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
#ifndef ICALOFUTUREELECTRON_H
#define ICALOFUTUREELECTRON_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
// from LHCb
#include "CaloDet/DeCalorimeter.h"
#include "Event/State.h"

// Forward declarations
namespace LHCb {
  class ProtoParticle;
  class CaloHypo;
  class CaloMomentum;
} // namespace LHCb

/** @class ICaloFutureElectron ICaloFutureElectron.h Kernel/ICaloFutureElectron.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2006-11-30
 */
namespace LHCb::Calo::Interfaces {
  struct IElectron : public extend_interfaces<IAlgTool> {

    // Return the interface ID
    DeclareInterfaceID( IElectron, 2, 0 );

    virtual std::pair<const LHCb::CaloHypo *, const LHCb::CaloHypo*> set( LHCb::ProtoParticle const& proto) const = 0;

    virtual State           caloState(ProtoParticle const &proto) const                                                         = 0;
    virtual State           closestState(ProtoParticle const &proto) const                                                      = 0;
    virtual double          eOverP(ProtoParticle const &proto) const                                                            = 0;
    virtual const CaloHypo* electron(ProtoParticle const &proto) const                                                          = 0;
    virtual const CaloHypo* bremstrahlung(ProtoParticle const &proto) const                                                     = 0;
    virtual CaloMomentum    bremCaloFutureMomentum(ProtoParticle const &proto) const                                            = 0;
    virtual double          caloTrajectoryL(ProtoParticle const &proto, CaloPlane::Plane refPlane = CaloPlane::ShowerMax ) const = 0;
  };
} // namespace LHCb::Calo::Interfaces
#endif // ICALOFUTUREELECTRON_H
