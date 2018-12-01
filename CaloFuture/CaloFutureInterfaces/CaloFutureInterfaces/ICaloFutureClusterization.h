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
#ifndef CALOFUTUREINTERFACES_ICALOFUTURECLUSTERIZATION_H
#define CALOFUTUREINTERFACES_ICALOFUTURECLUSTERIZATION_H 1

// Include files
// from STL
#include <vector>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// from LHCb
#include "Kernel/CaloCellID.h"
#include "CaloKernel/CaloVector.h"
#include "Event/CaloCluster.h"
#include "Event/CaloDigit.h"

// forward declarations
class DeCalorimeter;


/** @class ICaloFutureClusterization ICaloFutureClusterization.h CaloFutureInterfaces/ICaloFutureClusterization.h
 *
 *
 *  @author Victor Egorychev
 *  @date   2008-04-03
 */
struct  ICaloFutureClusterization : extend_interfaces<IAlgTool>
{

  DeclareInterfaceID( ICaloFutureClusterization, 3, 0 );

  // ==========================================================================
  /**  The main clusterization method
   * @ param pointer to CaloDigit
   * @ param pointer to DeCalorimeter object
   * @ param vector of CaloFuture cell ID's
   * @ param number of neigbour levels around cell
   * @ return vector of pointers of CaloFuture clusters
   */
  virtual unsigned int clusterize( std::vector<LHCb::CaloCluster*>&      clusters   ,
                                   const LHCb::CaloDigits&               digits     ,
                                   const DeCalorimeter*                  detector   ,
                                   const std::vector<LHCb::CaloCellID>&  seeds  = {},
                                   const unsigned int                    level  = 0 ) const = 0 ;
};
#endif // CALOFUTUREINTERFACES_ICALOFUTURECLUSTERIZATION_H
