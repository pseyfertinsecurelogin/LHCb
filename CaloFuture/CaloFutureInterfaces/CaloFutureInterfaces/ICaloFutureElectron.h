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
#include <string_view>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
// from LHCb
#include "CaloDet/DeCalorimeter.h"
#include "Event/CaloPosition.h"
#include "Event/State.h"
#include "Event/Track.h"

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
namespace Calo::Future::Interfaces {
  struct IElectron : public extend_interfaces<IAlgTool> {

    // Return the interface ID
    DeclareInterfaceID( Calo::Future::Interfaces::IElectron, 1, 0 );

    virtual bool set( LHCb::ProtoParticle const* proto, std::string const& det = DeCalorimeterLocation::Ecal,
                      CaloPlane::Plane plane = CaloPlane::ShowerMax, double delta = 0 ) = 0;

    virtual LHCb::State           caloState() const                                                         = 0;
    virtual LHCb::State           closestState() const                                                      = 0;
    virtual double                eOverP() const                                                            = 0;
    virtual const LHCb::CaloHypo* electron() const                                                          = 0;
    virtual const LHCb::CaloHypo* bremstrahlung() const                                                     = 0;
    virtual LHCb::CaloMomentum    bremCaloFutureMomentum() const                                            = 0;
    virtual double                caloTrajectoryL( CaloPlane::Plane refPlane = CaloPlane::ShowerMax ) const = 0;
  };
} // namespace Calo::Future::Interfaces
#endif // ICALOFUTUREELECTRON_H
