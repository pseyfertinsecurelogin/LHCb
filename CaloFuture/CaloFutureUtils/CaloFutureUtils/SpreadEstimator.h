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
#ifndef CALOFUTUREUTILS_SPREADESTIMATOR_H
#define CALOFUTUREUTILS_SPREADESTIMATOR_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/StatEntity.h"
#include "GaudiKernel/StatusCode.h"
// ============================================================================
// RecEvent
// ============================================================================
#include "Event/CaloDataFunctor.h"
// ============================================================================
/// forward declarations
// ============================================================================
class DeCalorimeter; // from CaloDet package
// ============================================================================
/** @class SpreadEstimator SpreadEstimator.h CaloFutureUtils/SpreadEstimator.h
 *
 *  simple helper class for estimation of spread of
 *  CaloCluster object.
 *
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   22/11/2001
 */
class SpreadEstimator {
  // ==========================================================================
public:
  // ==========================================================================
  /** standard/default constructor
   *  @param Det pointer to calorimeter detector
   */
  SpreadEstimator( const DeCalorimeter* Det = nullptr );
  // ==========================================================================
  /** calculation of cluster spread
   *
   *  Error codes:
   *   - 221 - invalid source of detector information
   *
   *  @param  cluster  pointer to cluster object
   *  @return status code
   */
  StatusCode operator()( LHCb::CaloCluster* cluster ) const;
  // ==========================================================================
  /** calculate spread for cluster
   *  @param  cluster  pointer to cluster
   *  @return status code
   */
  StatusCode calculateSpread( LHCb::CaloCluster* cluster ) const { return ( *this )( cluster ); }
  // ==========================================================================
public:
  // ==========================================================================
  /** set new value for calorimeter
   *  @param Det pointer to calorimeter detector
   */
  void setDetector( const DeCalorimeter* Det );
  // ==========================================================================
  /** simple accessor to DeCalorimeter object
   *  @return pointer to detector
   */
  [[deprecated]] const DeCalorimeter* detector() const { return m_detector; }
  // ==========================================================================
  /// get the counter for problematic cases
  const StatEntity& invalidRatio() const { return m_ratio; }
  /// get the counter for problematic cases
  const StatEntity& invalidCells() const { return m_cells; }
  /// get the counter for problematic cases
  const StatEntity& invalidEnergy() const { return m_energy; }
  // ==========================================================================
private:
  // ==========================================================================
  /// the detector elemenet
  const DeCalorimeter* m_detector; // the detector elemenet
  // ==========================================================================
  /// counter of invalid cells
  mutable StatEntity m_cells; // counter for invalid cells
  /// counter of invalid size ratio
  mutable StatEntity m_ratio; // counter for invalid size ratio
  /// counter of invalid energy
  mutable StatEntity m_energy; // counter for invalid energy
};
// ============================================================================
// The END
// ============================================================================
#endif // CALOFUTUREUTILS_SPREADESTIMATOR_H
