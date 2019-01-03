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
#include "Kernel/BrokenLineTrajectory.h"
#include <cassert>
#include <limits>
#include <numeric>

std::unique_ptr<LHCb::Trajectory<double>> LHCb::BrokenLineTrajectory::clone() const
{
  return std::make_unique<LHCb::BrokenLineTrajectory>(*this);
}

void
LHCb::BrokenLineTrajectory::push_back(LHCb::LineTraj<double> t)
{
  const auto& i = m_traj.emplace_back(std::move(t),endRange()).first;
  setRange(beginRange(),endRange()+(i.endRange()-i.beginRange()));
}

void
LHCb::BrokenLineTrajectory::push_front(LHCb::LineTraj<double> t)
{
  auto newBeginRange = beginRange()-(t.endRange()-t.beginRange());
  m_traj.insert( m_traj.begin(), { std::move(t),newBeginRange } );
  setRange(newBeginRange,endRange());
}

// pair of (local traj, local mu)
std::pair<const LHCb::LineTraj<double>*,double>
LHCb::BrokenLineTrajectory::loc(double mu) const
{
  assert(!m_traj.empty());
  auto i = std::prev( std::lower_bound(std::next(begin(m_traj)), end(m_traj), mu,
                                       [](const auto& t, double s) { return t.second < s; } ) );
  return {&i->first,i->first.beginRange()+(mu-i->second)};
}

LHCb::Trajectory<double>::Point
LHCb::BrokenLineTrajectory::position(double s) const
{
  return local(s,[](const LHCb::LineTraj<double>& traj, double t) { return traj.position(t); } );
}

LHCb::Trajectory<double>::Vector
LHCb::BrokenLineTrajectory::direction(double s) const
{
  return local(s,[](const LHCb::LineTraj<double>& traj, double t) { return traj.direction(t); } );
}

LHCb::Trajectory<double>::Vector
LHCb::BrokenLineTrajectory::curvature(double) const
{
  return {0,0,0};
}

void
LHCb::BrokenLineTrajectory::expansion( double s,
                                      Point& p,
                                      Vector& dp,
                                      Vector& ddp ) const
{
  local(s, [&]( const LHCb::LineTraj<double>& traj, double t ) {
      traj.expansion(t,p,dp,ddp);
  });
}

double
LHCb::BrokenLineTrajectory::muEstimate(const Point& p) const
{
  return std::accumulate( begin(m_traj), end(m_traj),
                          std::pair{ 0., std::numeric_limits<double>::max() },
                          [&p](std::pair<double,double> est, const auto& traj_mu) {
      const auto& [ loc_traj, glob_mu ] = traj_mu;
      // make sure mu isn't beyond the local traj valid range...
      double loc_mu = std::clamp( loc_traj.muEstimate(p), loc_traj.beginRange(), loc_traj.endRange() );
      double dist2 = ( loc_traj.position(loc_mu) - p ).Mag2();
      return ( dist2 < est.second ) ? std::pair{ glob_mu+(loc_mu-loc_traj.beginRange()), dist2 }
                                    : est;
  }).first;
}

double
LHCb::BrokenLineTrajectory::distTo1stError( double s,
                                            double ,
                                            int pathDirection) const
{
  auto l = loc(s);
  if ( l.first!= ( pathDirection>=0 ? &m_traj.back().first
                                    : &m_traj.front().first ) ) {
    // we're movinl amongst the internal pieces
    auto endOfLocalRange = ( pathDirection>0 ? l.first->endRange()
                                             : l.first->beginRange() ) ;
    // make sure we don't run beyond the current piece...
    return std::abs( endOfLocalRange - l.second );
  }
  return 10*Gaudi::Units::km;
}

double
LHCb::BrokenLineTrajectory::distTo2ndError( double s,
                                           double tolerance,
                                           int pathDirection) const
{
  return distTo1stError(s,tolerance,pathDirection);
}

std::ostream&
LHCb::BrokenLineTrajectory::print(std::ostream& os) const
{
  for (const auto& i : m_traj ) {
    auto pieceRange = i.first.endRange() - i.first.beginRange();
    os << "   BeginPoint[global/local] : " << i.first.beginPoint() << " [" << i.second << "/" << i.first.beginRange() << "]\n"
       << " ->  EndPoint[global/local] : " << i.first.endPoint() << " [" << i.second+pieceRange << "/" << i.first.endRange() << "]\n" << std::endl;
  }
  return os;

}
