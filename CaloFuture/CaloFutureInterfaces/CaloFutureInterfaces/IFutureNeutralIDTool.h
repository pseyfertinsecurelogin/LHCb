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
#ifndef IFUTURENEUTRALIDTOOL_H
#define IFUTURENEUTRALIDTOOL_H 1

// Include files
// from Gaudi
#include "Event/CaloHypo.h"
#include "GaudiKernel/IAlgTool.h"
#include "ICaloFutureHypoEstimator.h"

/** @class IFutureNeutralIDTool IFutureNeutralIDTool.h
 *  @author Mostafa HOBALLAH --
 *  @date   2013-07-25
 */

namespace LHCb::Calo::Interfaces {
  struct INeutralID : extend_interfaces<IAlgTool> {

    // Return the interface ID
    DeclareInterfaceID( INeutralID, 1, 0 );

    struct Observables {
      double clmatch = std::numeric_limits<double>::quiet_NaN();
      double prse    = std::numeric_limits<double>::quiet_NaN();
      double e19     = std::numeric_limits<double>::quiet_NaN();
      double hclecl  = std::numeric_limits<double>::quiet_NaN();
      double prse19  = std::numeric_limits<double>::quiet_NaN();
      double prse49  = std::numeric_limits<double>::quiet_NaN();
      double sprd    = std::numeric_limits<double>::quiet_NaN();
      double prse4mx = std::numeric_limits<double>::quiet_NaN();
      double prsm    = std::numeric_limits<double>::quiet_NaN();
      double spdm    = std::numeric_limits<double>::quiet_NaN();
    };

    virtual std::optional<double> isNotE( const CaloHypo& hypo, const Observables& v ) const = 0;
    virtual std::optional<double> isNotH( const CaloHypo& hypo, const Observables& v ) const = 0;

    virtual double isNotE( const Observables& v ) const = 0;
    virtual double isNotH( const Observables& v ) const = 0;
  };
} // namespace LHCb::Calo::Interfaces
#endif // IFUTURENEUTRALIDTOOL_H
