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
#include "Kernel/AlignTraj.h"
#include "GaudiKernel/GenericMatrixTypes.h"

#include "Math/Rotation3D.h"

#include <cmath>

using LHCb::AlignTraj;

// tiny helpers to make assignements from Vectors to SMatrix rows less painfull...
namespace {
  template <typename T>
  class Column {
  public:
    Column( T& t, unsigned c ) : m_t( t ), m_c( c ) {}
    template <typename V>
    Column<T>& operator=( const V& v ) {
      m_t( 0, m_c ) = v.X();
      m_t( 1, m_c ) = v.Y(), m_t( 2, m_c ) = v.Z();
      return *this;
    }

  private:
    T&       m_t;
    unsigned m_c;
  };

  template <typename T>
  class Diag {
  public:
    explicit Diag( T& t, unsigned begin = 0, unsigned end = T::kRows ) : m_t( t ), m_b( begin ), m_e( end ) {}
    Diag<T>& operator=( double x ) {
      for ( unsigned i = m_b; i < m_e; ++i ) m_t( i, i ) = x;
      return *this;
    }

  private:
    T&       m_t;
    unsigned m_b, m_e;
  };

  // some helper functions for the helper classes to avoid having to explicitly specify types...
  template <typename T>
  Column<T> column( T& t, unsigned c ) {
    return Column<T>( t, c );
  }

  template <typename T>
  Diag<T> diag( T& t, unsigned begin = 0, unsigned end = T::kRows ) {
    return Diag<T>( t, begin, end );
  }
} // namespace

std::unique_ptr<LHCb::Trajectory<double>> AlignTraj::clone() const { return std::make_unique<AlignTraj>( *this ); }

AlignTraj::Parameters AlignTraj::parameters() const {
  return {m_trans.X(), m_trans.Y(), m_trans.Z(), m_rx.Angle(), m_ry.Angle(), m_rz.Angle()};
}

AlignTraj& AlignTraj::operator+=( const Parameters& delta ) {
  m_trans += Vector( delta( 0 ), delta( 1 ), delta( 2 ) );
  m_rx *= ROOT::Math::RotationX( delta( 3 ) );
  m_ry *= ROOT::Math::RotationY( delta( 4 ) );
  m_rz *= ROOT::Math::RotationZ( delta( 5 ) );
  return *this;
}

AlignTraj::Derivative AlignTraj::derivative( double arclength ) const {

  Derivative d;
  // first, the derivatives wrt m_trans:
  diag( d, 0, 3 ) = 1;
  // next, the derivatives wrt. rx, ry, and rz:
  Vector v       = position( arclength ) - m_pivot;
  column( d, 3 ) = ( m_rx * ROOT::Math::RotationX( M_PI_2 ) )( m_ry( m_rz( v ) ).SetX( 0 ) );
  column( d, 4 ) = m_rx( ( m_ry * ROOT::Math::RotationY( M_PI_2 ) )( m_rz( v ).SetY( 0 ) ) );
  column( d, 5 ) = m_rx( m_ry( ( m_rz * ROOT::Math::RotationZ( M_PI_2 ) )(
      v.SetZ( 0 ) ) ) ); // note: this changes v, unlike the previous two lines -- so this should be last...
  return d;
}

AlignTraj::Point AlignTraj::position( double s ) const {
  // rotate around pivot, then translate
  return m_pivot + rotate( m_traj->position( s ) - m_pivot ) + m_trans;
}

AlignTraj::Vector AlignTraj::direction( double s ) const { return rotate( m_traj->direction( s ) ); }

AlignTraj::Vector AlignTraj::curvature( double s ) const { return rotate( m_traj->curvature( s ) ); }

void AlignTraj::expansion( double s, Point& p, Vector& dp, Vector& ddp ) const {
  m_traj->expansion( s, p, dp, ddp );
  p   = m_pivot + m_trans + rotate( p - m_pivot );
  dp  = rotate( dp );
  ddp = rotate( ddp );
}

double AlignTraj::muEstimate( const Point& p ) const {
  // apply inverse transformation to Point, and forward...
  return m_traj->arclength( m_pivot + invRotate( p - m_pivot - m_trans ) );
}

double AlignTraj::distTo1stError( double arclength, double tolerance, int pathDirection ) const {
  return m_traj->distTo1stError( arclength, tolerance, pathDirection );
}

double AlignTraj::distTo2ndError( double arclength, double tolerance, int pathDirection ) const {
  return m_traj->distTo2ndError( arclength, tolerance, pathDirection );
}
