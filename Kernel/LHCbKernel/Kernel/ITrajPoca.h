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
#ifndef KERNEL_ITRAJPOCA_H
#define KERNEL_ITRAJPOCA_H 1

// Include files
// -------------
// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/Vector3DTypes.h"

#include "Trajectory.h"
#include "GaudiKernel/TaggedBool.h"


/** @class ITrajPoca ITrajPoca.h
 *
 *  Interface for tools calculating points of closest approach
 *  between two trajectories
 *
 *  @author Edwin Bos, Jeroen van Tilburg, Eduardo Rodrigues
 *  @date   2005-11-22
 */

struct ITrajPoca : extend_interfaces<IAlgTool> {

  DeclareInterfaceID(ITrajPoca, 3, 0 );

  using RestrictRange = Gaudi::tagged_bool<struct RestrictRange_tag>;

  /// Find points along trajectories at which the distance between the
  /// trajectories is at its minimum. The precision parameter is the desired
  /// numerical accuracy of mu1 and mu2. If the restrictrange flag is true, mu
  /// is restricted to the range of the trajectory.
  virtual StatusCode minimize( const LHCb::Trajectory<double>& traj1, double& mu1, RestrictRange rRange1,
                               const LHCb::Trajectory<double>& traj2, double& mu2, RestrictRange rRange2,
                               Gaudi::XYZVector& distance, double precision ) const = 0 ;

  /// Find point along trajectory at which the distance to point 'p'
  /// is minimum. The precision parameter is the desired numerical accuracy of
  /// the expansion parameter mu. If the restrictrange flag is true, mu is
  /// restricted to the range of the trajectory.
  virtual StatusCode minimize( const LHCb::Trajectory<double>& traj, double& mu,
                               RestrictRange  restrictRange,
                               const Gaudi::XYZPoint& pt,
                               Gaudi::XYZVector& distance,
                               double precision ) const = 0;

  /// Find points along trajectories at which the distance between the
  /// trajectories is at its minimum. The precision parameter is the desired
  /// numerical accuracy of the expansion parameters mu1 and mu2.
  StatusCode minimize( const LHCb::Trajectory<double>& traj1, double& mu1,
                       const LHCb::Trajectory<double>& traj2, double& mu2,
                       Gaudi::XYZVector& distance, double precision ) const {
    return minimize( traj1, mu1, RestrictRange{false},
                     traj2, mu2, RestrictRange{false},
                     distance, precision );
  }


  /// Find point along trajectory at which the distance to point 'p'
  /// is minimum. The precision parameter is the desired
  /// numerical accuracy of the expansion parameter mu.
  StatusCode minimize( const LHCb::Trajectory<double>& traj, double& mu,
                       const Gaudi::XYZPoint& pt,
                       Gaudi::XYZVector& distance, double precision ) const {
    return minimize( traj, mu, RestrictRange{false}, pt, distance, precision );
  }
};
#endif // KERNEL_ITRAJPOCA_H
