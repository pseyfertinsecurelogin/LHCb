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

#include "Kernel/LineTraj.h"
#include "Kernel/PiecewiseTrajectory.h"
#include <limits>
#include <memory>
#include <utility>

using Point     = LHCb::Trajectory<double>::Point;
using Direction = LHCb::Trajectory<double>::Vector;

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr bool approx_equal( T lhs, T rhs ) {
  constexpr T eps   = 1e-9;
  auto        delta = std::abs( lhs - rhs );
  return delta < eps;
}

BOOST_AUTO_TEST_CASE( test_PiecewiseTrajectory ) {
  LHCb::PiecewiseTrajectory traj{};
  traj.append( std::make_unique<LHCb::LineTraj<double>>( Point{0, 0, 0}, Point{1, 0, 0} ) );
  traj.append( std::make_unique<LHCb::LineTraj<double>>( Point{1, 0, 0}, Point{1, 1, 0} ) );
  traj.append( std::make_unique<LHCb::LineTraj<double>>( Point{1, 1, 0}, Point{1, 1, 1} ) );
  traj.prepend( std::make_unique<LHCb::LineTraj<double>>( Point{0, 0, 1}, Point{0, 0, 0} ) );

  auto points = {
      std::pair{-0.5, Point{0., 0., 0.5}}, std::pair{0.0, Point{0., 0., 0.}},  std::pair{0.5, Point{0.5, 0., 0.}},
      std::pair{0.8, Point{0.8, 0., 0.}},  std::pair{0.9, Point{0.9, 0., 0.}}, std::pair{1.0, Point{1., 0., 0.}},
      std::pair{1.1, Point{1., 0.1, 0.}},  std::pair{1.5, Point{1., 0.5, 0.}}, std::pair{1.9, Point{1., 0.9, 0.}},
      std::pair{2.0, Point{1., 1., 0.}},   std::pair{2.1, Point{1., 1., 0.1}}, std::pair{2.5, Point{1., 1., 0.5}},
      std::pair{2.9, Point{1., 1., 0.9}},  std::pair{3.0, Point{1., 1., 1.}},  std::pair{10.0, Point{1., 1., 8.}}};

  for ( auto&& [mu, pnt] : points ) {
    BOOST_CHECK( approx_equal( ( traj.position( mu ) - pnt ).R(), 0. ) );
    BOOST_CHECK( approx_equal( traj.muEstimate( pnt ), std::clamp( mu, traj.beginRange(), traj.endRange() ) ) );
  }

  auto directions = {std::pair{-0.5, Direction{0., 0., -1.}}, std::pair{0.01, Direction{1., 0., 0.}},
                     std::pair{0.5, Direction{1., 0., 0.}},   std::pair{1.5, Direction{0., 1., 0.}},
                     std::pair{2.5, Direction{0., 0., 1.}},   std::pair{100., Direction{0., 0., 1.}}};
  for ( auto&& [mu, dir] : directions ) { BOOST_CHECK( approx_equal( ( traj.direction( mu ) - dir ).R(), 0. ) ); }

  BOOST_CHECK( traj.distTo1stError( -0.5, 0, -1 ) > 1 * Gaudi::Units::km );
  BOOST_CHECK( approx_equal( traj.distTo1stError( 0.5, 0, +1 ), 0.5 ) );
  BOOST_CHECK( approx_equal( traj.distTo1stError( 1.8, 0, +1 ), 0.2 ) );
  BOOST_CHECK( approx_equal( traj.distTo1stError( 1.8, 0, -1 ), 0.8 ) );
  BOOST_CHECK( traj.distTo1stError( 2.5, 0, +1 ) > 1 * Gaudi::Units::km );
}
