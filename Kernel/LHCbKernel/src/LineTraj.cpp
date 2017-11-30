// Include files

// local
#include "Kernel/LineTraj.h"
#include "GaudiKernel/SystemOfUnits.h"

std::unique_ptr<LHCb::Trajectory> LHCb::LineTraj::clone() const
{
  return std::make_unique<LHCb::LineTraj>(*this);
}


/// Constructor from a begin and an end point
LHCb::LineTraj::LineTraj( const Trajectory::Point& begPoint,
                          const Trajectory::Point& endPoint )
  : Trajectory(-(Gaudi::XYZVector(endPoint-begPoint)).r()/2.,(Gaudi::XYZVector(endPoint-begPoint)).r()/2.),
    m_dir(endPoint-begPoint),
    m_pos(begPoint+0.5*m_dir)
{
  m_dir = m_dir.Unit();
}

/// Point on the trajectory at arclength from the starting point
LHCb::Trajectory::Point LHCb::LineTraj::position( double arclength ) const
{
  return m_pos + arclength * m_dir;
}

/// First derivative of the trajectory at arclength from the starting point
LHCb::Trajectory::Vector LHCb::LineTraj::direction( double /* arclength*/ ) const
{
  return m_dir;
}

/// Second derivative of the trajectory at arclength from the starting point
LHCb::Trajectory::Vector LHCb::LineTraj::curvature( double /* arclength */ ) const
{
  return {0,0,0};
}

/// Create a parabolic approximation to the trajectory
/// at arclength from the starting point
void LHCb::LineTraj::expansion( double arclength,
                          Trajectory::Point& p,
                          Trajectory::Vector& dp,
                          Trajectory::Vector& ddp ) const
{
  ddp = {0,0,0};
  dp  = m_dir;
  p   = m_pos + arclength * m_dir;
}

/// Determine the distance in arclenghts to the
/// closest point on the trajectory to a given point
double LHCb::LineTraj::muEstimate( const Trajectory::Point& point ) const
{
  return m_dir.Dot(point-m_pos);
}

// 1st order approx OK everywhere
double LHCb::LineTraj::distTo1stError( double , double , int ) const
{
  return 10*Gaudi::Units::km;
}

// 2nd order approx OK everywhere
double LHCb::LineTraj::distTo2ndError( double , double , int ) const
{
  return 10*Gaudi::Units::km;
}
