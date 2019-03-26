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
#ifndef LHCbKernel_PiecewiseTrajectory_H
#define LHCbKernel_PiecewiseTrajectory_H 1

// Includes
#include "Kernel/Trajectory.h"
#include <deque>
#include <memory>
#include <type_traits>
#include <utility>

// from Gaudi

namespace LHCb {

  /** @class PiecewiseTrajectory PiecewiseTrajectory.h
   *
   *
   * @author Gerhard Raven
   * @date   20/04/2007
   *
   */
  class PiecewiseTrajectory : public Trajectory<double> {
  public:
    template <typename ITER>
    PiecewiseTrajectory( ITER begin, ITER end ) : Trajectory( 0, 0 ) {
      while ( begin != end ) {
        append( *begin );
        ++begin;
      }
    }

    PiecewiseTrajectory() : Trajectory( 0, 0 ) {}

    PiecewiseTrajectory( const PiecewiseTrajectory& rhs );

    // clone thyself...
    std::unique_ptr<Trajectory<double>> clone() const override;

    /// Point on the trajectory at mu
    Point position( double mu ) const override;

    /// First derivative of the trajectory at mu
    Vector direction( double mu ) const override;

    /// Second derivative of the trajectory at mu
    Vector curvature( double mu ) const override;

    /// Create a parabolic approximation to the trajectory
    /// at mu
    void expansion( double arclength, Point& p, Vector& dp, Vector& ddp ) const override;
    /// Determine the distance in arclenghts to the
    /// closest point on the trajectory to a given point
    double muEstimate( const Point& ) const override;

    /// Number of arclengths until deviation of the trajectory from the expansion
    /// reaches the given tolerance.
    double distTo1stError( double arclength, double tolerance, int pathDirection = +1 ) const override;

    /// Number of arclengths until deviation of the trajectory from the expansion
    /// reaches the given tolerance.
    double distTo2ndError( double arclength, double tolerance, int pathDirection = +1 ) const override;

    /// Distance, along the Trajectory, between position(mu1) and
    /// position(mu2). .
    using Trajectory<double>::arclength;
    double arclength( double mu1, double mu2 ) const override { return mu2 - mu1; }

    // functions specific to a PieceWiseTrajectory
    // note: we _will_ assume ownership of the passed Trajectory!
    void         append( std::unique_ptr<Trajectory<double>> );
    void         prepend( std::unique_ptr<Trajectory<double>> );
    unsigned int numberOfPieces() const { return m_traj.size(); }

    std::ostream& print( std::ostream& ) const;

  private:
    //                   trajectory                  global mu for start of Traj
    std::deque<std::pair<std::unique_ptr<Trajectory<double>>, double>> m_traj;

    // global -> local mapping
    std::pair<const Trajectory<double>*, double> loc( double mu ) const;

    // generic forwarding to local trajectories
    template <typename FUN>
    decltype( auto ) local( double mu, FUN fun ) const {
      auto j = loc( mu );
      return fun( j.first, j.second );
    }

    typedef double ( LHCb::Trajectory<double>::*distFun )( double, double, int ) const;

    double distToError( double s, double tolerance, int pathDirection, distFun fun ) const;
  };

} // namespace LHCb
#endif
