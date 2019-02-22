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
#include "Math/CholeskyDecomp.h"
#include <algorithm>
#include <utility>

// Math Definitions
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/Vector3DTypes.h"

// local
#include "TrajPoca.h"

DECLARE_COMPONENT( TrajPoca )

namespace {

  inline bool restrictToRange( double& l, const LHCb::Trajectory<double>& t ) {
    const auto minmax = std::minmax( {t.beginRange(), t.endRange()} );
    const auto oldl   = std::exchange( l, std::clamp( l, minmax.first, minmax.second ) );
    return oldl != l;
  }

  struct cache_t {
    Gaudi::XYZPoint  p1, p2;
    Gaudi::XYZVector dp1dmu1, dp2dmu2;
    Gaudi::XYZVector d2p1dmu12, d2p2dmu22;
  };

  enum class step_status_t { ok = 0, parallel, nearly_parallel, beyond_maxdist };

  const char* status_msg( step_status_t status ) {
    switch ( status ) {
    case step_status_t::parallel:
      return "The Trajectories are parallel.";
    case step_status_t::nearly_parallel:
      return "The Trajectories are very nearly parallel.";
    case step_status_t::beyond_maxdist:
      return "Stepped further than MaxDist.";
    case step_status_t::ok:
      return "OK, nothing to report";
    }
    __builtin_unreachable();
  }
} // namespace

//=============================================================================
//
//=============================================================================
step_status_t stepTowardPoca( const LHCb::Trajectory<double>& traj1, double& mu1,
                              ITrajPoca::RestrictRange restrictRange1, const LHCb::Trajectory<double>& traj2,
                              double& mu2, ITrajPoca::RestrictRange restrictRange2, double tolerance, cache_t& cache,
                              double maxExtrapTolerance, double maxDist ) {
  // a bunch of ugly, unitialized member variables
  traj1.expansion( mu1, cache.p1, cache.dp1dmu1, cache.d2p1dmu12 );
  traj2.expansion( mu2, cache.p2, cache.dp2dmu2, cache.d2p2dmu22 );
  const Gaudi::XYZVector d( cache.p1 - cache.p2 );
  // if the distance between points is below 1e-4 mm, we consider the
  // minimisation to be converged
  if ( UNLIKELY( d.mag2() < std::pow( 1e-4 * Gaudi::Units::mm, 2 ) ) ) { return step_status_t::ok; }
  std::array<double, 3> mat = {
      // keep all terms up to order mu1, mu2, mu1 * mu2
      cache.dp1dmu1.mag2() + d.Dot( cache.d2p1dmu12 ),
      -cache.dp2dmu2.Dot( cache.dp1dmu1 ),
      cache.dp2dmu2.mag2() - d.Dot( cache.d2p2dmu22 ),
  };
  ROOT::Math::CholeskyDecomp<double, 2> decomp( &mat[0] );
  if ( UNLIKELY( !decomp ) ) {
    // singular, or not pos. def; try again neglecting curvature
    mat[0] = cache.dp1dmu1.mag2(), mat[2] = cache.dp2dmu2.mag2();
    decomp = ROOT::Math::CholeskyDecomp<double, 2>( &mat[0] );
    if ( UNLIKELY( !decomp ) ) {
      // singular, or not pos. def; give up
      return step_status_t::parallel;
    }
  }
  {
    // check product of eigenvalues of mat; if too small, trajectories are very
    // nearly parallel
    decltype( mat ) lmat;
    if ( UNLIKELY( !decomp.getL( &lmat[0] ) ) || UNLIKELY( lmat[0] * lmat[2] < 1e-8 ) )
      return step_status_t::nearly_parallel;
  }
  const std::array<double, 2> rhs = {-d.Dot( cache.dp1dmu1 ), d.Dot( cache.dp2dmu2 )};
  std::array<double, 2>       dmu = rhs;
  decomp.Solve( dmu );

  int pathDir1 = ( dmu[0] > 0 ) ? 1 : -1;
  int pathDir2 = ( dmu[1] > 0 ) ? 1 : -1;

  // Don't try going further than worst parabolic approximation will
  // allow. The tolerance is set by 'deltadoca', the expected
  // improvement in the doca. We bound this by tolerance from below
  // and maxExtrapTolerance from above.
  double                  deltadoca       = std::sqrt( std::abs( rhs[0] * dmu[0] ) +
                                std::abs( rhs[1] * dmu[1] ) ); // std::abs only because of machine precision issues.
  double                  extraptolerance = std::min( std::max( deltadoca, tolerance ), maxExtrapTolerance );
  static constexpr double smudge          = 1.01; // Factor to push just over border of piecewise traj (essential!)
  double                  distToErr1      = smudge * traj1.distTo2ndError( mu1, extraptolerance, pathDir1 );
  double                  distToErr2      = smudge * traj2.distTo2ndError( mu2, extraptolerance, pathDir2 );

  // Factor to push just over border of piecewise traj (essential!)
  if ( UNLIKELY( 0 < distToErr1 && distToErr1 < std::abs( dmu[0] ) ) ) {
    // choose solution for which dmu[0] steps just over border
    dmu[0] = distToErr1 * pathDir1;
    // now recalculate dmu[1], given dmu[0]:
    dmu[1] = ( rhs[1] - dmu[0] * mat[1] ) / mat[2];
  }

  if ( UNLIKELY( 0 < distToErr2 && distToErr2 < std::abs( dmu[0] ) ) ) {
    // choose solution for which dmu[1] steps just over border
    dmu[1] = distToErr2 * pathDir2;
    // now recalculate dmu[0], given dmu[1]:
    dmu[0] = ( rhs[0] - dmu[1] * mat[1] ) / mat[0];
    // if still not okay,
    if ( UNLIKELY( 0 < distToErr1 && distToErr1 < std::abs( dmu[0] ) ) ) { dmu[0] = distToErr1 * pathDir1; }
  }

  mu1 += dmu[0], mu2 += dmu[1];

  // these do not make any sense here. either we need to merge them with the lines above that restrict to the validity
  // of the expansion, or we need to move them out of here entirely.
  if ( UNLIKELY( bool( restrictRange1 ) ) ) restrictToRange( mu1, traj1 );
  if ( UNLIKELY( bool( restrictRange2 ) ) ) restrictToRange( mu2, traj2 );

  // another check for parallel trajectories
  if ( UNLIKELY( std::min( std::abs( mu1 ), std::abs( mu2 ) ) > maxDist ) ) return step_status_t::beyond_maxdist;

  return step_status_t::ok;
}

//=============================================================================
// Find mus along trajectories having a distance smaller than tolerance
//=============================================================================
StatusCode TrajPoca::minimize( const LHCb::Trajectory<double>& traj1, double& mu1,
                               ITrajPoca::RestrictRange restrictRange1, const LHCb::Trajectory<double>& traj2,
                               double& mu2, ITrajPoca::RestrictRange restrictRange2, Gaudi::XYZVector& distance,
                               double precision ) const {
  StatusCode status = StatusCode::SUCCESS;

  unsigned int maxWarnings = ( LIKELY( !msgLevel( MSG::DEBUG ) ) ? 0 : 9999 );

  double  delta2( 0 ), prevdelta2( 0 );
  int     nOscillStep( 0 );
  int     nDivergingStep( 0 );
  int     nStuck( 0 );
  bool    finished = false;
  cache_t workspace;

  for ( int istep = 0; LIKELY( istep < m_maxnTry && !finished ); ++istep ) {
    double prevflt1       = mu1;
    double prevflt2       = mu2;
    double prevprevdelta2 = std::exchange( prevdelta2, delta2 );
    auto   step_status = stepTowardPoca( traj1, mu1, restrictRange1, traj2, mu2, restrictRange2, precision, workspace,
                                       m_maxExtrapTolerance, m_maxDist );
    if ( UNLIKELY( step_status != step_status_t::ok ) ) {
      if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) { debug() << status_msg( step_status ) << endmsg; }
      status = StatusCode::FAILURE;
      break; // Parallel Trajectories in stepTowardPoca
    }

    distance        = traj1.position( mu1 ) - traj2.position( mu2 );
    delta2          = distance.Mag2();
    double step1    = mu1 - prevflt1;
    double step2    = mu2 - prevflt2;
    int    pathDir1 = ( step1 > 0. ) ? 1 : -1;
    int    pathDir2 = ( step2 > 0. ) ? 1 : -1;
    // Can we stop stepping?
    double distToErr1 = traj1.distTo1stError( prevflt1, precision, pathDir1 );
    double distToErr2 = traj2.distTo1stError( prevflt2, precision, pathDir2 );
    // converged if very small steps
    finished = std::abs( step1 ) < distToErr1 && std::abs( step2 ) < distToErr2;

    // we have to catch some problematic cases
    if ( UNLIKELY( !finished && istep > 2 && delta2 > prevdelta2 ) ) {
      // we can get stuck if a flt range is restricted
      if ( UNLIKELY( ( restrictRange1 && std::abs( step1 ) > 1.0e-10 ) ||
                     ( restrictRange2 && std::abs( step2 ) > 1e-10 ) ) ) {
        if ( UNLIKELY( ++nStuck > m_maxnStuck ) ) {
          // downgrade to a point poca
          Gaudi::XYZVector dist( 0., 0., 0. );
          restrictRange2 ? minimize( traj1, mu1, restrictRange1, traj2.position( mu2 ), dist, precision )
                         : minimize( traj2, mu2, restrictRange2, traj1.position( mu1 ), dist, precision );
          if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) { debug() << "Minimization got stuck." << endmsg; }
          status   = StatusCode::SUCCESS; // "Stuck poca"
          finished = true;
        }
      } else if ( UNLIKELY( prevdelta2 > prevprevdelta2 ) ) {
        // diverging
        if ( UNLIKELY( ++nDivergingStep > m_maxnDivergingStep ) ) {
          status = StatusCode::SUCCESS; // "Failed to converge"
          Warning( "Minimization was diverging.", status, maxWarnings ).ignore();
          finished = true;
        }
      } else {
        nDivergingStep = 0;
        // oscillating
        if ( UNLIKELY( ++nOscillStep > m_maxnOscillStep ) ) {
          // bail out of oscillation. since the previous step was
          // better, use that one.
          mu1 = prevflt1;
          mu2 = prevflt2;
          if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) { debug() << "Minimization bailed out of oscillation." << endmsg; }
          status = StatusCode::SUCCESS; // "Oscillating poca"
          Warning( "Minimization bailed out of oscillation.", status, maxWarnings ).ignore();
          finished = true;
        } else {
          // we might be oscillating, but we could also just have
          // stepped over the minimum. choose a solution `in
          // between'.
          mu1 = prevflt1 + 0.5 * step1;
          if ( restrictRange1 ) restrictToRange( mu1, traj1 );
          mu2 = prevflt2 + 0.5 * step2;
          if ( restrictRange2 ) restrictToRange( mu2, traj2 );
          distance = traj1.position( mu1 ) - traj2.position( mu2 );
          delta2   = distance.Mag2();
        }
      }
    }
  }

  if ( UNLIKELY( !finished ) ) {
    status = StatusCode::FAILURE;
    Warning( "Minimization did not converge", status, maxWarnings ).ignore();
  }

  return status;
}

//=============================================================================
//
//=============================================================================
StatusCode TrajPoca::minimize( const LHCb::Trajectory<double>& traj, double& mu, RestrictRange restrictRange,
                               const Gaudi::XYZPoint& pt, Gaudi::XYZVector& distance, double /*precision*/ ) const {
  // this does not work for non-linear Trajectories!
  mu = traj.muEstimate( pt );
  if ( restrictRange ) restrictToRange( mu, traj );
  distance = traj.position( mu ) - pt;
  return StatusCode::SUCCESS;
}
