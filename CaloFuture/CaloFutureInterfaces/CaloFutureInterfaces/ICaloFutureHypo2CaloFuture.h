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
#ifndef ICALOFUTUREHYPO2CALOFUTURE_H
#define ICALOFUTUREHYPO2CALOFUTURE_H 1

// Include files
// from STL
#include <string>

#include "Event/CaloCluster.h"
#include "Event/CaloDigit.h"
#include "Event/CaloHypo.h"
#include "Kernel/CaloCellID.h"
// from Gaudi
#include "GaudiKernel/IAlgTool.h"
// forward declaration

/** @class ICaloFutureHypo2CaloFuture ICaloFutureHypo2CaloFuture.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2008-09-09
 */
namespace LHCb::Calo::Interfaces {
  struct IHypo2Calo : extend_interfaces<IAlgTool> {

    // Return the interface ID
    DeclareInterfaceID( IHypo2Calo, 2, 0 );

    virtual double energy( const CaloCluster& fromCluster, CaloCellCode::CaloIndex toCalo ) const = 0;
    virtual double energy( const CaloHypo& fromHypo, CaloCellCode::CaloIndex toCalo ) const       = 0;

    virtual int multiplicity( const CaloCluster& fromCluster, CaloCellCode::CaloIndex toCalo ) const = 0;
    virtual int multiplicity( const CaloHypo& fromHypo, CaloCellCode::CaloIndex toCalo ) const       = 0;
  };
} // namespace LHCb::Calo::Interfaces
#endif // ICALOFUTUREHYPO2CALOFUTURE_H
