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
// ===========================================================================
#define CALOFUTUREUTILS_CLUSTERFUNCTORS_CPP 1
// ===========================================================================
// Include files
#include "CaloFutureUtils/ClusterFunctors.h"
#include "CaloDet/DeCalorimeter.h"
#include "CaloKernel/CaloException.h"
#include "Event/CaloCluster.h"
#include "Event/CaloDigit.h"

// ============================================================================
/** @file ClusterFunctors.cpp
 *
 *  Implementation of non-inline method from ClusterFunctors namespace
 *  @see ClusterFunctors
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date 04/07/2001
 */
// ============================================================================

// ============================================================================
/**  Calculate the "energy" of the cluster as a sum of
 *   energies of its digits, weighted with energy fractions
 *   @param   cl  pointer to cluster
 *   @return      "energy" of cluster
 */
// ============================================================================
double LHCb::ClusterFunctors::energy( const LHCb::CaloCluster* cl ) {
  if ( !cl || cl->entries().empty() ) { return 0; }
  return ClusterFunctors::energy( cl->entries().begin(), cl->entries().end() );
}

// ===========================================================================
/**    useful function to determine, if clusters have
 *     at least one common cell.
 *
 *     For invalid arguments return "false"
 *
 *     @param   cl1   pointer to first  cluster
 *     @param   cl2   pointer to second cluster
 *     @return "true" if clusters have at least 1 common cell
 */
// ===========================================================================
bool LHCb::ClusterFunctors::overlapped( const LHCb::CaloCluster* cl1, const LHCb::CaloCluster* cl2 ) {
  if ( !cl1 || !cl2 || cl1->entries().empty() || cl2->entries().empty() ) { return false; }
  ///
  auto p = LHCb::ClusterFunctors::commonDigit( cl1->entries().begin(), cl1->entries().end(), cl2->entries().begin(),
                                               cl2->entries().end() );
  ///
  return cl1->entries().end() != p.first;
}

// ===========================================================================
/**  Calculate the "energy", X and Y position
 *   of the cluster as a sum of
 *   energies/x/y of its digits,
 *   weighted with energy fractions
 *   @param   cl  pointer to cluster object
 *   @param   de  pointer to DeCalorimeter object
 *   @param   e   energy
 *   @param   x   x-position
 *   @param   y   y-position
 *   @return    status code
 */
// ===========================================================================
StatusCode LHCb::ClusterFunctors::calculateEXY( const LHCb::CaloCluster* cl, const DeCalorimeter* de, double& e,
                                                double& x, double& y ) {
  e = 0;
  x = 0;
  y = 0;
  if ( !cl || cl->entries().empty() ) { return StatusCode::FAILURE; }
  ///
  return ClusterFunctors::calculateEXY( cl->entries().begin(), cl->entries().end(), de, e, x, y );
}

// ===========================================================================
/** throw the exception
 *  @param message exception message
 *  @return status code (fictive)
 */
// ===========================================================================
void LHCb::ClusterFunctors::throwException( const std::string& message ) {
  throw CaloException( " ClusterFunctors::" + message );
}
