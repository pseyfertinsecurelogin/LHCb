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
#ifndef KERNEL_ALIGNTRAJ_H
#define KERNEL_ALIGNTRAJ_H 1

#ifdef __INTEL_COMPILER             // Disable ICC remark from ROOT
#  pragma warning( disable : 1572 ) // Floating-point equality and inequality comparisons are unreliable
#endif

// Include files
#include <utility>

#include "Math/RotationX.h"
#include "Math/RotationY.h"
#include "Math/RotationZ.h"

// local
#include "Kernel/DifTraj.h"

/** @class AlignTraj AlignTraj.h Kernel/AlignTraj.h
 *
 *  AlignTraj is a DifTraj, which takes an external trajectory
 *  as input, and translates/rotates this trajectory.
 *  It provides the derivatives wrt. the translations and rotations
 *  Note: this trajectory does NOT own the external trajectory
 *        provided...
 *  Note: First we rotate around a pivot point, i.e. we take
 *        a given point, compute its difference with the pivot,
 *        and rotate this difference, and then add the pivot.
 *        Next we translate the result. Basically, think about this
 *        as first getting the orientation around the pivot right,
 *        and then moving the pivot into the right location.
 *  Note: the intended use is for small (delta) alignments wrt a
 *        baseline geometry
 *  Note: in general, it is not guaranteed that the six parameters
 *        are independent -- i.e. for straight lines, only four are...
 *
 *  @author Gerhard Raven
 *  @date   2006-11-27
 */

namespace LHCb {

  class AlignTraj : public DifTraj<6> {

  public:
    // import types from parent...
    using Parameters = DifTraj<6>::Parameters;
    using Derivative = DifTraj<6>::Derivative;
    using Point      = DifTraj<6>::Point;
    using Vector     = DifTraj<6>::Vector;

    /// Constructors
    AlignTraj( const Trajectory& traj, Point pivot )
        : DifTraj<6>( traj.range() ), m_pivot( std::move( pivot ) ), m_traj( &traj ) {}

    AlignTraj( const Trajectory& traj ) : DifTraj<6>( traj.range() ), m_pivot( 0, 0, 0 ), m_traj( &traj ) {}

    AlignTraj( const Trajectory& traj, const Parameters& p )
        : DifTraj<6>( traj.range() )
        , m_rx( p( 3 ) )
        , m_ry( p( 4 ) )
        , m_rz( p( 5 ) )
        , m_pivot( 0, 0, 0 )
        , m_traj( &traj ) {
      m_trans.SetX( p( 0 ) );
      m_trans.SetY( p( 1 ) );
      m_trans.SetZ( p( 2 ) );
    }

    AlignTraj( const Trajectory& traj, const Parameters& p, Point pivot )
        : DifTraj<6>( traj.range() )
        , m_rx( p( 3 ) )
        , m_ry( p( 4 ) )
        , m_rz( p( 5 ) )
        , m_pivot( std::move( pivot ) )
        , m_traj( &traj ) {
      m_trans.SetX( p( 0 ) );
      m_trans.SetY( p( 1 ) );
      m_trans.SetZ( p( 2 ) );
    }

    // clone thyself...
    [[nodiscard]] std::unique_ptr<Trajectory> clone() const override;

    /// Retrieve the derivative of the point at fixed arclength 'arclength'
    /// with respect to the alignment parameters
    [[nodiscard]] Derivative derivative( double arclength ) const override;

    /// Retrieve the alignment parameters
    [[nodiscard]] Parameters parameters() const override;

    /// update the parameters
    AlignTraj& operator+=( const Parameters& delta ) override;

    [[nodiscard]] Point  position( double arclength ) const override;
    [[nodiscard]] Vector direction( double arclength ) const override;
    [[nodiscard]] Vector curvature( double arclength ) const override;
    void                 expansion( double arclength, Point& p, Vector& dp, Vector& ddp ) const override;

    [[nodiscard]] double muEstimate( const Point& ) const override;

    [[nodiscard]] double distTo1stError( double arclength, double tolerance, int pathDirection ) const override;
    [[nodiscard]] double distTo2ndError( double arclength, double tolerance, int pathDirection ) const override;

    /// Distance, along the Trajectory, between position(mu1) and
    /// position(mu2). Trivial because AlignTraj is parameterized in
    /// arclength.
    using DifTraj<6>::arclength;
    [[nodiscard]] double arclength( double mu1, double mu2 ) const override { return mu2 - mu1; }

  private:
    template <typename T>
    T rotate( const T& t ) const {
      return m_rx( m_ry( m_rz( t ) ) );
    }
    template <typename T>
    T invRotate( const T& t ) const {
      return m_rz.Inverse()( m_ry.Inverse()( m_rx.Inverse()( t ) ) );
    }

    ROOT::Math::RotationX m_rx;
    ROOT::Math::RotationY m_ry;
    ROOT::Math::RotationZ m_rz;
    Vector                m_trans;
    Point                 m_pivot;
    const Trajectory*     m_traj;
  };

} // namespace LHCb

#endif // KERNEL_ALIGNTRAJ_H
