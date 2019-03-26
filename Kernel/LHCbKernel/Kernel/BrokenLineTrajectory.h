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
#ifndef LHCbKernel_BROKENLINETRAJECTORY
#define LHCbKernel_BROKENLINETRAJECTORY 1

// Includes
#include "Kernel/LineTraj.h"
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

// from Gaudi

namespace LHCb {

  /** @class BrokenLineTrajectory BrokenLineTrajectory.h
   *
   *
   * @author Gerhard Raven
   * @date   03/01/2019
   *
   */
  class BrokenLineTrajectory : public Trajectory<double> {
    template <typename Iter>
    static constexpr bool isForwardIterator =
        std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>;

  public:
    using value_type = LineTraj<double>; // required for std::back_inserter...
    template <typename Iter, typename = std::enable_if_t<std::is_convertible_v<
                                 typename std::iterator_traits<Iter>::value_type, LineTraj<double>>>>
    BrokenLineTrajectory( Iter begin, Iter end ) : Trajectory( 0, 0 ) {
      if constexpr ( isForwardIterator<Iter> ) { reserve( std::distance( begin, end ) ); }
      std::copy( begin, end, std::back_inserter( *this ) );
    }
    template <typename... Args,
              typename = std::enable_if_t<std::conjunction_v<std::is_convertible<Args, LineTraj<double>>...>>>
    BrokenLineTrajectory( Args&&... args ) {
      reserve( sizeof...( Args ) );
      ( push_back( std::forward<Args>( args ) ), ... );
    }

    BrokenLineTrajectory() : Trajectory( 0, 0 ) {}

    void reserve( std::size_t s ) { m_traj.reserve( s ); }

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

    // functions specific to a BrokenLineTrajectory
    void         push_front( LineTraj<double> );
    void         push_back( LineTraj<double> );
    unsigned int size() const { return m_traj.size(); }
    bool         empty() const { return m_traj.empty(); }

    std::ostream& print( std::ostream& ) const;

  private:
    //                    trajectory       global mu for start of Traj
    std::vector<std::pair<LineTraj<double>, double>> m_traj;

    // global -> local mapping
    std::pair<const LineTraj<double>*, double> loc( double mu ) const;

    // generic forwarding to local trajectories
    template <typename FUN>
    decltype( auto ) local( double mu, FUN fun ) const {
      auto j = loc( mu );
      return fun( *j.first, j.second );
    }
  };

} // namespace LHCb
#endif
