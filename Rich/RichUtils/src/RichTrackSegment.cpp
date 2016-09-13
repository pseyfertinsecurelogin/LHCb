
//-----------------------------------------------------------------------------
/** @file RichTrackSegment.cpp
 *
 *  Implementation file for class : LHCb::RichTrackSegment
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @author  Antonis Papanestis
 *  @date    2002-06-10
 */
//-----------------------------------------------------------------------------

#ifdef __INTEL_COMPILER       // Disable ICC remark from ROOT
#pragma warning(disable:1572) // Floating-point comparisons are unreliable
#endif

// local
#include "RichUtils/RichTrackSegment.h"

void LHCb::RichTrackSegment::updateState( const Gaudi::XYZPoint & rotPnt,
                                          const Gaudi::Transform3D & trans )
{
  // Entry point
  auto toEntry = entryPoint() - rotPnt;
  // Middle point
  auto toMid   = middlePoint() - rotPnt;
  // exit point
  auto toExit  = exitPoint() - rotPnt;
  // set the states
  setStates( rotPnt + trans(toEntry), trans(entryMomentum()),
             rotPnt + trans(toMid),   trans(middleMomentum()),
             rotPnt + trans(toExit),  trans(exitMomentum())  );
}

void LHCb::RichTrackSegment::updateCachedInfo()
{
  // compute the cached rotation matrices for the angle calculations
  const auto z = bestMomentum().Unit();
  auto y = z.Cross( Gaudi::XYZVector(1,0,0) );
  y *= vdt::fast_isqrtf( y.Mag2() ); // maybe not needed ?
  const auto x = y.Cross(z);
  m_rotation2 = Gaudi::Rotation3D( x.X(), y.X(), z.X(),
                                   x.Y(), y.Y(), z.Y(),
                                   x.Z(), y.Z(), z.Z() );
  m_rotation = Gaudi::Rotation3D( m_rotation2.Inverse() );

  // Vector from entry to exit point
  const auto entryExitV ( exitPoint() - entryPoint() );

  // mag^2 for entry to exit vector
  const auto entryExitVMag2 = entryExitV.mag2();

  // update entry to middle point vector
  m_midEntryV = ( middlePoint() - entryPoint()  );

  // update middle to exit point vector
  m_exitMidV  = ( exitPoint()   - middlePoint() );

  // update factors
  m_invMidFrac1 = std::sqrt( entryExitVMag2    / m_midEntryV.mag2() );
  m_midFrac2    = std::sqrt( m_exitMidV.mag2() / entryExitVMag2     );

  // update the path length
  m_pathLength = std::sqrt(m_midEntryV.mag2()) + std::sqrt(m_exitMidV.mag2());
}

Gaudi::XYZVector LHCb::RichTrackSegment::bestMomentum( const double fractDist ) const
{
  // return the best momentum vector
  if ( zCoordAt(fractDist) < middlePoint().z() )
  {
    const auto midFrac = fractDist * m_invMidFrac1;
    return (entryMomentum()*(1-midFrac)) + (middleMomentum()*midFrac);
  }
  else
  {
    const auto midFrac = ( fractDist / m_midFrac2 ) - 1.0;
    return (middleMomentum()*(1-midFrac)) + (exitMomentum()*midFrac);
  }
}

void LHCb::RichTrackSegment::chordConstructorInit2()
{
  // the direction to use
  auto v = exitPoint()-entryPoint();
  if ( v.Mag2() > 0 )
  {
    // Update direction of entry state to chord direction
    v *= std::sqrt( entryMomentum().Mag2() / v.Mag2() );
    setEntryState( entryPoint(), v );
    // Update direction of middle state to chord direction
    v *= std::sqrt( ( (entryMomentum()+exitMomentum())*0.5 ).Mag2() / v.Mag2() );
    setMiddleState( add_points(entryPoint(),exitPoint())*0.5, v );
    // Update direction of exit state to chord direction
    v *= std::sqrt( exitMomentum().Mag2() / v.Mag2() );
    setExitState( exitPoint(), v );
  }
}

void LHCb::RichTrackSegment::chordConstructorInit3()
{
  // the direction to use
  auto v = exitPoint() - entryPoint();
  if ( v.Mag2() > 0 )
  {
    // Update direction of entry state to chord direction
    v *= std::sqrt( entryMomentum().Mag2() / v.Mag2() );
    setEntryState( entryPoint(), v );
    // Update direction of middle state to chord direction
    v *= std::sqrt( middleMomentum().Mag2() / v.Mag2() );
    setMiddleState( middlePoint(), v );
    // Update direction of exit state to chord direction
    v *= std::sqrt( exitMomentum().Mag2() / v.Mag2() );
    setExitState( exitPoint(), v );
  }
}

std::ostream & LHCb::RichTrackSegment::fillStream ( std::ostream& s ) const
{
  s << "{ " << std::endl
    << " entryPoint:\t" << entryPoint() << std::endl
    << " middlePoint:\t" << middlePoint() << std::endl
    << " exitPoint:\t" << exitPoint() << std::endl
    << " entryMomentum:\t" << entryMomentum() << std::endl
    << " middleMomentum:\t" << middleMomentum() << std::endl
    << " exitMomentum:\t" << exitMomentum() << std::endl
    << " radiator:\t" << Rich::text( radiator() ) << std::endl
    << " rich:\t" << Rich::text( rich() ) << std::endl
    << " entryErrors:\t" << entryErrors() << std::endl
    << " middleErrors:\t" << middleErrors() << std::endl
    << " exitErrors:\t" << exitErrors() << std::endl
    << " } ";
  return s;
}
