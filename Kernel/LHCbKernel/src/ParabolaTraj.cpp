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
// Include files

// local
#include "Kernel/ParabolaTraj.h"
#include "GaudiKernel/SystemOfUnits.h"
using namespace LHCb;

std::unique_ptr<Trajectory<double>> ParabolaTraj::clone() const
{
        return std::make_unique<ParabolaTraj>(*this);
}

#ifdef __INTEL_COMPILER         // Disable ICC remark from ROOT
  #pragma warning(disable:1572) // Floating-point equality and inequality comparisons are unreliable
#endif

/// Constructor from a (middle) point, a (unit) direction vector and a curvature
ParabolaTraj::ParabolaTraj( const Point& point,
                            const Vector& dir,
                            const Vector& curv,
                            const Range& range)
  : Trajectory(range),
    m_pos(point),
    m_dir(dir.unit()),
    m_curv(curv)
{
}

/// Point on the trajectory at arclength from the starting point
Trajectory<double>::Point ParabolaTraj::position( double arclength ) const
{
  return m_pos + arclength * (m_dir + 0.5 * arclength * m_curv);
}

/// First derivative of the trajectory at arclength from the starting point
Trajectory<double>::Vector ParabolaTraj::direction( double arclength ) const
{
  return m_dir + arclength * m_curv;
}

/// Second derivative of the trajectory at arclength from the starting point
Trajectory<double>::Vector ParabolaTraj::curvature( double /* arclength */) const
{
  return m_curv;
}

/// Create a parabolic approximation to the trajectory
/// at arclength from the starting point
void ParabolaTraj::expansion( double arclength,
                              Point& p,
                              Vector& dp,
                              Vector& ddp ) const
{
  ddp = m_curv;
  dp  = m_dir + arclength*m_curv;
  p   = m_pos + arclength* (m_dir + 0.5 * arclength * m_curv);
}

  /// Determine the arclenghts of the
  /// closest point on this trajectory to a given point
double ParabolaTraj::muEstimate( const Point& point ) const
{
  auto r = point - m_pos;
  if (m_curv.R()<0.01*m_dir.R()) { // small curvature limit: neglect curvature
     return r.Dot(m_dir);
  }
  // get vector from m_pos to point projected into plane of parabola
  auto normal = m_dir.Cross(m_curv).unit();
  r -= normal.Dot(r)*normal;
  // get normalized 'x' and 'y' coordinates of this vector by projecting onto the
  // axis. In terms of these, the parabola is parameterized as (arclen, arclen^2/2)
  // (i.e. arclen is actually the distance along the 'x' coordinate!)
  auto x = r.Dot(m_dir);
  auto y = r.Dot(m_curv);
  //  now we need to minimize the distance between (x,y) and (s,s*s/2)
  //  where s is the arclen (well, not quite, but that is what we really
  //  use in 'point(arclen)' ;-)
  //  This requires solving a 3rd order polynomial, so we assume m_curve<<1
  //  and solve a linear equation instead.
  return x/(1-y);
}

/// arclengths until deviation of the trajectory from the expansion
/// reaches the given tolerance.
double ParabolaTraj::distTo1stError( double , double tolerance , int ) const
{
  return std::sqrt(2*tolerance/m_curv.R());
}

/// 2nd order is OK everywhere...
double ParabolaTraj::distTo2ndError( double , double , int ) const
{
  return 10*Gaudi::Units::km;
}
