/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "GaudiKernel/SystemOfUnits.h"

namespace LHCb {
  /// Constructor from a begin and an end point
  template<typename FTYPE>
  inline
  LineTraj<FTYPE>::LineTraj( const typename Trajectory<FTYPE>::Point& beg,
                             const typename Trajectory<FTYPE>::Point& end )
    : m_dir(end-beg),
      m_pos(beg+0.5*m_dir)
  {
    // Trajectory(-(Vector(endPoint-begPoint)).r()/2.,
    //            (Vector(endPoint-begPoint)).r()/2.),
    // can we use r() with VectorClass ? it seems to use std::sqrt
    // temporarly use a raw r() impl. to be sure we use VectorClass sqrt when we manage with
    // VectorClass types
    Vector p = end-beg;
    FTYPE val = sqrt(p.Mag2())/2.;
    this->setRange(-1.*val, val);
    // m_range.first  = -1.*val;
    // m_range.second = val;
    m_dir = m_dir.Unit();
  }

  /// Point on the trajectory at arclength from the starting point
  template<typename FTYPE>
  inline typename Trajectory<FTYPE>::Point
  LineTraj<FTYPE>::position( FTYPE arclength ) const
  {
    return m_pos + arclength * m_dir;
  }

  /// First derivative of the trajectory at arclength from the starting point
  template<typename FTYPE>
  inline typename Trajectory<FTYPE>::Vector
  LineTraj<FTYPE>::direction( FTYPE /* arclength*/ ) const
  {
    return m_dir;
  }

  /// Second derivative of the trajectory at arclength from the starting point
  template<typename FTYPE>
  inline typename Trajectory<FTYPE>::Vector
  LineTraj<FTYPE>::curvature( FTYPE /* arclength */ ) const
  {
    return {0,0,0};
  }

  /// Create a parabolic approximation to the trajectory
  /// at arclength from the starting point
  template<typename FTYPE>
  inline void
  LineTraj<FTYPE>::expansion( FTYPE arclength,
                              typename Trajectory<FTYPE>::Point& p,
                              typename Trajectory<FTYPE>::Vector& dp,
                              typename Trajectory<FTYPE>::Vector& ddp ) const
  {
    ddp = {0,0,0};
    dp  = m_dir;
    p   = m_pos + arclength * m_dir;
  }

  /// Determine the distance in arclenghts to the
  /// closest point on the trajectory to a given point
  template<typename FTYPE>
  inline FTYPE
  LineTraj<FTYPE>::muEstimate( const typename Trajectory<FTYPE>::Point& point ) const
  {
    return m_dir.Dot(point-m_pos);
  }

  // 1st order approx OK everywhere
  template<typename FTYPE>
  inline FTYPE
  LineTraj<FTYPE>::distTo1stError( FTYPE , FTYPE , int ) const
  {
    return 10*Gaudi::Units::km;
  }

  // 2nd order approx OK everywhere
  template<typename FTYPE>
  inline FTYPE
  LineTraj<FTYPE>::distTo2ndError( FTYPE , FTYPE , int ) const
  {
    return 10*Gaudi::Units::km;
  }
}
