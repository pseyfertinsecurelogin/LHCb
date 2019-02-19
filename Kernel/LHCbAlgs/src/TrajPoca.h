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
#ifndef TRACKTOOLS_TRAJPOCA_H
#define TRACKTOOLS_TRAJPOCA_H 1

// Include files
#include <algorithm>

// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/SystemOfUnits.h"

// from Tr/TrackInterfaces
#include "Kernel/ITrajPoca.h"

// from Kernel/LHCbKernel
#include "Kernel/Trajectory.h"

/** @class TrajPoca TrajPoca.h
 *
 *  Calculates points of closest approach
 *  between two trajectories
 *
 *  @author Steve Schaffner (babar implementation)
 *  @author Edwin Bos, Jeroen van Tilburg, Eduardo Rodrigues
 *  @date   2005-11-23
 **/

class TrajPoca : public extends<GaudiTool, ITrajPoca> {
public:
  /// Default constructor
  using extends<GaudiTool, ITrajPoca>::extends;

  /// Find points along trajectories at which the distance between the
  /// trajectories is at its minimum. The precision parameter is the desired
  /// numerical accuracy of mu1 and mu2. If the restrictrange flag is true, mu
  /// is restricted to the range of the trajectory.
  StatusCode minimize( const LHCb::Trajectory<double>& traj1, double& mu1, RestrictRange range1,
                       const LHCb::Trajectory<double>& traj2, double& mu2, RestrictRange range2,
                       Gaudi::XYZVector& distance, double precision ) const override;

  /// Find point along trajectory at which the distance to point 'p'
  /// is minimum. The precision parameter is the desired numerical accuracy of
  /// the expansion parameter mu. If the restrictrange flag is true, mu is
  /// restricted to the range of the trajectory.
  StatusCode minimize( const LHCb::Trajectory<double>& traj, double& mu, RestrictRange restrictRange,
                       const Gaudi::XYZPoint& pt, Gaudi::XYZVector& distance, double precision ) const override;

private:
  // jobOptions
  Gaudi::Property<int>    m_maxnOscillStep{this, "MaxnOscillStep", 5};
  Gaudi::Property<int>    m_maxnDivergingStep{this, "MaxnDivergingStep", 5};
  Gaudi::Property<int>    m_maxnStuck{this, "MaxnStuck", 3};
  Gaudi::Property<int>    m_maxnTry{this, "MaxnTry", 100};
  Gaudi::Property<double> m_maxDist{this, "MaxDist", 100000000};
  Gaudi::Property<double> m_maxExtrapTolerance{this, "MaxExtrapTolerance", 1 * Gaudi::Units::cm};
};

#endif // TRACKTOOLS_TRAJPOCA_H
