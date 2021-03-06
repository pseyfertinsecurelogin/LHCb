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
#ifndef CALOINTERFACES_ICALOCLUSTERIZATION_H
#define CALOINTERFACES_ICALOCLUSTERIZATION_H 1

// Include files
// from STL
#include <vector>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// from LHCb
#include "CaloKernel/CaloVector.h"
#include "Event/CaloCluster.h"
#include "Event/CaloDigit.h"
#include "Kernel/CaloCellID.h"

// forward declarations
class DeCalorimeter;

/** @class ICaloClusterization ICaloClusterization.h CaloInterfaces/ICaloClusterization.h
 *
 *
 *  @author Victor Egorychev
 *  @date   2008-04-03
 */
struct ICaloClusterization : extend_interfaces<IAlgTool> {

  DeclareInterfaceID( ICaloClusterization, 3, 0 );

  // ==========================================================================
  /**  The main clusterization method
   * @ param pointer to CaloDigit
   * @ param pointer to DeCalorimeter object
   * @ param vector of Calo cell ID's
   * @ param number of neigbour levels around cell
   * @ return vector of pointers of Calo clusters
   */
  virtual unsigned int clusterize( std::vector<LHCb::CaloCluster*>& clusters, const LHCb::CaloDigits& digits,
                                   const DeCalorimeter* detector, const std::vector<LHCb::CaloCellID>& seeds = {},
                                   const unsigned int level = 0 ) const = 0;
};
#endif // CALOINTERFACES_ICALOCLUSTERIZATION_H
