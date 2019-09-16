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
#ifndef ICALOFUTUREHYPOESTIMATOR_H
#define ICALOFUTUREHYPOESTIMATOR_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "CaloFutureInterfaces/Enums.h"
#include "CaloFutureInterfaces/ICaloFutureHypo2CaloFuture.h"
#include "Event/CaloHypo.h"
#include "Event/Track.h"
#include "GaudiKernel/IAlgTool.h"

namespace LHCb::Calo::Interfaces {

  /** @class IHypoEstimator IHypoEstimator.h
   *
   *
   *  @author Olivier Deschamps
   *  @date   2010-08-18
   */
  struct IHypoEstimator : extend_interfaces<IAlgTool> {

    // Return the interface ID
    DeclareInterfaceID( IHypoEstimator, 1, 0 );

    virtual std::optional<double> data( const CaloCluster& cluster, Enum::DataType type ) const = 0;
    virtual std::optional<double> data( const CaloHypo& hypo, Enum::DataType type ) const       = 0;
    virtual StatusCode            _setProperty( const std::string&, const std::string& )        = 0;
    virtual IHypo2Calo*           hypo2Calo()                                                   = 0;
    virtual bool                  status() const                                                = 0;
    virtual const Track*          toTrack( Enum::MatchType match ) const                        = 0;
  };
} // namespace LHCb::Calo::Interfaces
#endif // ICALOFUTUREHYPOESTIMATOR_H
