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
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

#include "Kernel/BrokenLineTrajectory.h"
#include <array>
#include <limits>
#include <utility>

using Point     = LHCb::Trajectory<double>::Point;
using Direction = LHCb::Trajectory<double>::Vector;
using Line      = LHCb::LineTraj<double>;

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr bool approx_equal( T lhs, T rhs ) {
  constexpr T eps   = 1e-9;
  auto        delta = std::abs( lhs - rhs );
  return delta < eps;
}

void test_trajectory( LHCb::BrokenLineTrajectory traj, double muOffset = 0. ) {
  BOOST_CHECK( traj.size() == 4 );
  auto points = {
      std::pair{-0.5, Point{0., 0., 0.5}}, std::pair{0.0, Point{0., 0., 0.}},  std::pair{0.5, Point{0.5, 0., 0.}},
      std::pair{0.8, Point{0.8, 0., 0.}},  std::pair{0.9, Point{0.9, 0., 0.}}, std::pair{1.0, Point{1., 0., 0.}},
      std::pair{1.1, Point{1., 0.1, 0.}},  std::pair{1.5, Point{1., 0.5, 0.}}, std::pair{1.9, Point{1., 0.9, 0.}},
      std::pair{2.0, Point{1., 1., 0.}},   std::pair{2.1, Point{1., 1., 0.1}}, std::pair{2.5, Point{1., 1., 0.5}},
      std::pair{2.9, Point{1., 1., 0.9}},  std::pair{3.0, Point{1., 1., 1.}},  std::pair{10.0, Point{1., 1., 8.}}};

  for ( auto&& [mu, pnt] : points ) {
    BOOST_CHECK( approx_equal( ( traj.position( mu - muOffset ) - pnt ).R(), 0. ) );
    BOOST_CHECK(
        approx_equal( traj.muEstimate( pnt ), std::clamp( mu - muOffset, traj.beginRange(), traj.endRange() ) ) );
  }

  auto directions = {std::pair{-0.5, Direction{0., 0., -1.}}, std::pair{0.01, Direction{1., 0., 0.}},
                     std::pair{0.5, Direction{1., 0., 0.}},   std::pair{1.5, Direction{0., 1., 0.}},
                     std::pair{2.5, Direction{0., 0., 1.}},   std::pair{100., Direction{0., 0., 1.}}};
  for ( auto&& [mu, dir] : directions ) {
    BOOST_CHECK( approx_equal( ( traj.direction( mu - muOffset ) - dir ).R(), 0. ) );
  }

  BOOST_CHECK( traj.distTo1stError( -0.5 - muOffset, 0, -1 ) > 1 * Gaudi::Units::km );
  BOOST_CHECK( approx_equal( traj.distTo1stError( 0.5 - muOffset, 0, +1 ), 0.5 ) );
  BOOST_CHECK( approx_equal( traj.distTo1stError( 1.8 - muOffset, 0, +1 ), 0.2 ) );
  BOOST_CHECK( approx_equal( traj.distTo1stError( 1.8 - muOffset, 0, -1 ), 0.8 ) );
  BOOST_CHECK( traj.distTo1stError( 2.5 - muOffset, 0, +1 ) > 1 * Gaudi::Units::km );
}

BOOST_AUTO_TEST_CASE( test_BrokenLineTrajectory ) {
  LHCb::BrokenLineTrajectory traj1;
  BOOST_CHECK( traj1.empty() );
  traj1.reserve( 4 );
  traj1.push_back( {Point{0, 0, 0}, Point{1, 0, 0}} );
  traj1.push_back( {Point{1, 0, 0}, Point{1, 1, 0}} );
  traj1.push_back( {Point{1, 1, 0}, Point{1, 1, 1}} );
  traj1.push_front( {Point{0, 0, 1}, Point{0, 0, 0}} );
  test_trajectory( traj1 );

  auto lines = std::array{Line{Point{0, 0, 1}, Point{0, 0, 0}}, Line{Point{0, 0, 0}, Point{1, 0, 0}},
                          Line{Point{1, 0, 0}, Point{1, 1, 0}}, Line{Point{1, 1, 0}, Point{1, 1, 1}}};

  test_trajectory( {begin( lines ), end( lines )}, -1.0 );

  LHCb::BrokenLineTrajectory traj2{lines[1], lines[2]};
  traj2.push_front( lines[0] );
  traj2.push_back( lines[3] );
  test_trajectory( traj2 );

  test_trajectory( {lines[0], lines[1], lines[2], lines[3]}, -1.0 );
}
