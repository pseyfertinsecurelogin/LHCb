/*****************************************************************************\
* (c) Copyright 2000-2020 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef LHCbKernel_ParabolaTraj_H
#define LHCbKernel_ParabolaTraj_H 1

// Include files
#include "Kernel/DifTraj.h"

namespace LHCb {

  /** @class ParabolaTraj ParabolaTraj.h Kernel/ParabolaTraj.h
   *
   * Trajectory parameterized as a parabola
   *
   * @author Edwin Bos, Jeroen van Tilburg, Eduardo Rodrigues
   * @date   09/01/2006
   */

  class ParabolaTraj : public Trajectory<double> {

  public:
    // clone thyself...
    [[nodiscard]] std::unique_ptr<Trajectory<double>> clone() const override;

    /// Constructor from a (middle) point, a (unit) direction vector,
    /// a curvature vector and the pair of begin- and endpoints.
    ParabolaTraj( Point middle, const Vector& dir, Vector curv, const Trajectory<double>::Range& range );

    /// Point on the trajectory at arclength from the starting point
    [[nodiscard]] Point position( double arclength ) const override;

    /// First derivative of the trajectory at arclength from the starting point
    [[nodiscard]] Vector direction( double arclength ) const override;

    /// Second derivative of the trajectory at arclength from the starting point
    [[nodiscard]] Vector curvature( double arclength = 0 ) const override;

    /// Create a parabolic approximation to the trajectory
    /// at arclength from the starting point
    void expansion( double arclength, Point& p, Vector& dp, Vector& ddp ) const override;

    /// Determine the distance in arclenghts to the
    /// closest point on the trajectory to a given point
    [[nodiscard]] double muEstimate( const Point& point ) const override;

    /// Number of arclengths until deviation of the trajectory from the
    /// expansion reaches the given tolerance.
    [[nodiscard]] double distTo1stError( double arclength, double tolerance, int pathDirection ) const override;

    /// Number of arclengths until deviation of the trajectory from the
    /// expansion reaches the given tolerance.
    [[nodiscard]] double distTo2ndError( double arclength, double tolerance, int pathDirection ) const override;

    /// Distance, along the Trajectory, between position(mu1) and
    /// position(mu2). Trivial because ParabolaTraj is parameterized in
    /// arclength.
    using Trajectory<double>::arclength;
    [[nodiscard]] double arclength( double mu1, double mu2 ) const override { return mu2 - mu1; }

  private:
    Point  m_pos;
    Vector m_dir;
    Vector m_curv;

  }; // class ParabolaTraj

} // namespace LHCb

#endif /// LHCbKernel_ParabolaTraj_H
