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
// ===========================================================================
// STD & STL
#include "boost/container/static_vector.hpp"
#include <algorithm>
// Units
#include "GaudiKernel/SystemOfUnits.h"
// Geometry Definitions
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
/// DetDesc
#include "DetDesc/DetDesc.h"
#include "DetDesc/SolidBox.h"
#include "DetDesc/SolidException.h"
#include "DetDesc/SolidSphere.h"
#include "DetDesc/SolidTicks.h"

// ============================================================================
/** @file SolidSphere.cpp
 *
 *  implementation of class SolidSphere
 *
 *  @author Vanya Belyaev  Ivan.Belyaev@itep.ru
 *  @date xx/xx/xx
 */
// ============================================================================

// ============================================================================
/** constructor
 *  @param name             name of sphere segment
 *  @param OuterRadius      outer radius of sphere segement
 *  @param InsideRadius     inner  radius of sphere segement
 *  @param StartPhiAngle    start phi angle
 *  @param DeltaPhiAngle    delta phi angle
 *  @param StartThetaAngle  start theta angle
 *  @param DeltaThetaAngle  delta theta angle
 *  @param CoverModel       covering model
 */
// ============================================================================
SolidSphere::SolidSphere( const std::string& name,            //
                          const double       OuterRadius,     //
                          const double       InsideRadius,    //
                          const double       StartPhiAngle,   //
                          const double       DeltaPhiAngle,   //
                          const double       StartThetaAngle, //
                          const double       DeltaThetaAngle, //
                          const int          CoverModel )
    : SolidBase( name )
    , m_sphere_startPhiAngle( StartPhiAngle )
    , m_sphere_deltaPhiAngle( DeltaPhiAngle )
    , m_sphere_startThetaAngle( StartThetaAngle )
    , m_sphere_deltaThetaAngle( DeltaThetaAngle )
    , m_sphere_coverModel( CoverModel )
    , m_sphere_outerR( OuterRadius )
    , m_sphere_insideR( InsideRadius ) {
  if ( 0 >= OuterRadius ) { throw SolidException( "SolidSphere::OuterRadius  is not positive!" ); }
  if ( 0 > InsideRadius ) { throw SolidException( "SolidSphere::InsideRadius is negative!    " ); }
  if ( InsideRadius >= OuterRadius ) { throw SolidException( "SolidSphere::InsideRadius>=OuterRadius    " ); }
  ///
  m_sphere_outerR2  = OuterRadius * OuterRadius;
  m_sphere_insideR2 = InsideRadius * InsideRadius;
  ///
  if ( -180.0 * Gaudi::Units::degree > StartPhiAngle ) {
    throw SolidException( "SolidSphere::StartPhiAngle < -180 degree !" );
  }
  if ( 360.0 * Gaudi::Units::degree < StartPhiAngle ) {
    throw SolidException( "SolidSphere::StartPhiAngle >  360 degree !" );
  }
  if ( 0.0 * Gaudi::Units::degree > DeltaPhiAngle ) {
    throw SolidException( "SolidSphere::DeltaPhiAngle <    0 degree !" );
  }
  if ( 360.0 * Gaudi::Units::degree < StartPhiAngle + DeltaPhiAngle ) {
    throw SolidException( "SolidSphere::StartPhiAngle+DeltaPhiAngle>2pi" );
  }
  if ( 0.0 * Gaudi::Units::degree > StartThetaAngle ) {
    throw SolidException( "SolidSphere::StartThetaAngle < 0 degree !" );
  }
  if ( 180.0 * Gaudi::Units::degree < StartThetaAngle ) {
    throw SolidException( "SolidSphere::StartThetaAngle >  180 degree !" );
  }
  if ( 0.0 * Gaudi::Units::degree > DeltaThetaAngle ) {
    throw SolidException( "SolidSphere::DeltaThetaAngle <    0 degree !" );
  }
  if ( 180.0 * Gaudi::Units::degree < StartThetaAngle + DeltaThetaAngle ) {
    throw SolidException( "SolidSphere::StartThetaAngle+DeltaThetaAngle>pi" );
  }
  ///
  m_noPhiGap = true;
  if ( 0 * Gaudi::Units::degree != startPhiAngle() ) { m_noPhiGap = false; }
  if ( 360 * Gaudi::Units::degree != deltaPhiAngle() ) { m_noPhiGap = false; }
  m_noThetaGap = true;
  if ( 0 * Gaudi::Units::degree != startThetaAngle() ) { m_noThetaGap = false; }
  if ( 180 * Gaudi::Units::degree != deltaThetaAngle() ) { m_noThetaGap = false; }
  /// set bounding parameters
  setBP();
  createCover();
}

// ============================================================================
/// set parameters for bounding solids (box, sphere and cylinder)
// ============================================================================
void SolidSphere::setBP() {
  // set bounding parameters of SolidBase class
  setRMax( outerRadius() );

  const auto theta1 = startThetaAngle();
  const auto theta2 = startThetaAngle() + deltaThetaAngle();
  const auto phi1   = startPhiAngle();
  const auto phi2   = startPhiAngle() + deltaPhiAngle();

  const auto ctheta1 = cos( theta1 );
  const auto ctheta2 = cos( theta2 );
  const auto stheta1 = sin( theta1 );
  const auto stheta2 = sin( theta2 );
  const auto cphi1   = cos( phi1 );
  const auto cphi2   = cos( phi2 );
  const auto sphi1   = sin( phi1 );
  const auto sphi2   = sin( phi2 );

  { // evaluate ZMin and ZMax
    auto minmax = std::minmax(
        {outerRadius() * ctheta1, insideRadius() * ctheta1, outerRadius() * ctheta2, insideRadius() * ctheta2} );
    //
    setZMax( minmax.second );
    setZMin( minmax.first );
  };

  double rhoMin = 0.0;
  { // evaluate rho max
    boost::container::static_vector<double, 5> values;

    // regular case
    values.push_back( rMax() * stheta1 );
    values.push_back( rMax() * stheta2 );

    // needed for evaluation of RhoMin
    values.push_back( insideRadius() * stheta1 );
    values.push_back( insideRadius() * stheta2 );

    // special cases
    if ( theta1 <= 90 * Gaudi::Units::degree && 90 * Gaudi::Units::degree <= theta2 ) { values.push_back( rMax() ); }

    auto minmax = std::minmax_element( values.begin(), values.end() );
    setRhoMax( *minmax.second );
    rhoMin = ( *minmax.first );
  };

  { // evaluate xmin & xmax
    boost::container::static_vector<double, 12> values;

    // regular cases
    values.push_back( rhoMax() * cphi1 );
    values.push_back( rhoMax() * cphi2 );
    values.push_back( rhoMin * cphi1 );
    values.push_back( rhoMin * cphi2 );

    // special cases
    if ( phi1 <= 0 * Gaudi::Units::degree && 0 * Gaudi::Units::degree <= phi2 ) {
      values.push_back( rhoMax() );
      values.push_back( rhoMin );
    }
    if ( phi1 <= 360 * Gaudi::Units::degree && 360 * Gaudi::Units::degree <= phi2 ) {
      values.push_back( rhoMax() );
      values.push_back( rhoMin );
    }

    // special cases
    if ( phi1 <= 180 * Gaudi::Units::degree && 180 * Gaudi::Units::degree <= phi2 ) {
      values.push_back( -rhoMax() );
      values.push_back( -rhoMin );
    }
    if ( phi1 <= -180 * Gaudi::Units::degree && -180 * Gaudi::Units::degree <= phi2 ) {
      values.push_back( -rhoMax() );
      values.push_back( -rhoMin );
    }

    auto minmax = std::minmax_element( values.begin(), values.end() );
    setXMax( *minmax.second );
    setXMin( *minmax.first );
  }

  { // evaluate ymin & ymax
    boost::container::static_vector<double, 10> values;

    // regular cases
    values.push_back( rhoMax() * sphi1 );
    values.push_back( rhoMax() * sphi2 );
    values.push_back( rhoMin * sphi1 );
    values.push_back( rhoMin * sphi2 );

    // special cases
    if ( phi1 <= 90 * Gaudi::Units::degree && 90 * Gaudi::Units::degree <= phi2 ) {
      values.push_back( rhoMax() );
      values.push_back( rhoMin );
    }

    // special cases
    if ( phi1 <= -90 * Gaudi::Units::degree && -90 * Gaudi::Units::degree <= phi2 ) {
      values.push_back( -rhoMax() );
      values.push_back( -rhoMin );
    }
    if ( phi1 <= 270 * Gaudi::Units::degree && 270 * Gaudi::Units::degree <= phi2 ) {
      values.push_back( -rhoMax() );
      values.push_back( -rhoMin );
    }

    auto minmax = std::minmax_element( values.begin(), values.end() );
    setYMax( *minmax.second );
    setYMin( *minmax.first );
  }

  ///
  checkBP();
}

// ===========================================================================
/** - check for the given 3D-point.
 *    Point coordinated are in the local reference
 *    frame of the solid.
 *  - implementation of ISolid absstract interface
 *  @see ISolid
 *  @param point point (in local reference system of the solid)
 *  @return true if the point is inside the solid
 */
// ===========================================================================
bool SolidSphere::isInside( const Gaudi::XYZPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
bool SolidSphere::isInside( const Gaudi::Polar3DPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
bool SolidSphere::isInside( const Gaudi::RhoZPhiPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================

// ===========================================================================
/** -# retrieve the pointer to "simplified" solid - "cover"
 *  -# implementation of ISolid abstract interface
 *    The simplification scheme:
 *  - for Model == 0
 *     -# The cover for the general sphere segment is the clove
 *         (remove gap in theta)
 *     -# The cover for the clove is the sphere
 *         (remove gap in phi)
 *     -# The cover for the sphere is the ball
 *         (set the inner radius to zero)
 *     -# The cover for the ball is the box
 *  - alternative model
 *     -# The cover for general sphere segment is
 *          the segment with inner radius equal to zero
 *     -# The cover for the general sphere segment with
 *        inner radius equal to zero is the
 *        the sphere segemnt with no phi gap
 *     -# The cover for the general sphere segment with
 *         inner radius equal to zero and nophi gap
 *         is the ball
 *     -# the cover for the ball is the box
 *  @see ISolid
 *  @see SolidSphere::m_sphere_coverModel
 *  @return pointer to "simplified" solid - "cover"
 */
// ===========================================================================
void SolidSphere::createCover() {
  if ( 0 == m_sphere_coverModel ) {
    /** cover for sphere segment
     *  is the segment with no gap in Theta
     */
    if ( 0.0 * Gaudi::Units::degree != startThetaAngle() || 180.0 * Gaudi::Units::degree != deltaThetaAngle() ) {
      m_cover = std::make_unique<SolidSphere>( "Cover for " + name(), outerRadius(), insideRadius(), startPhiAngle(),
                                               deltaPhiAngle() );
    } else if ( 0.0 * Gaudi::Units::degree != startPhiAngle() || 360.0 * Gaudi::Units::degree != deltaPhiAngle() ) {
      /** cover for sphere segment with no gap in Theta is
       *  the spehere segment with no gap in Phi */
      m_cover = std::make_unique<SolidSphere>( "Cover for " + name(), outerRadius(), insideRadius() );
    } else if ( 0.0 != insideRadius() ) {
      /** cover for sphere segment with no gap in Phi
       *  is the sphere with null inside radius
       */
      m_cover = std::make_unique<SolidSphere>( "Cover for " + name(), outerRadius() );
    } else {
      /** cover for sphere with null inside radius
       *  is the box
       */
      m_cover = std::make_unique<SolidBox>( "Cover for " + name(), outerRadius(), outerRadius(), outerRadius() );
    }
  } else {
    /** cover for sphere with not null inside radius is
     *  the sphere with null inside radius
     */
    if ( 0.0 != insideRadius() ) {
      m_cover =
          std::make_unique<SolidSphere>( "Cover for " + name(), outerRadius(), 0.0 * Gaudi::Units::mm, startPhiAngle(),
                                         deltaPhiAngle(), startThetaAngle(), deltaThetaAngle(), m_sphere_coverModel );
    } else if ( 0.0 * Gaudi::Units::degree != startPhiAngle() || 360.0 * Gaudi::Units::degree != deltaPhiAngle() ) {
      /** cover for sphere with null inside radius
       *  is the sphere segment with no gap in phi
       */
      m_cover = std::make_unique<SolidSphere>( "Cover for " + name(), outerRadius(), insideRadius(),
                                               0.0 * Gaudi::Units::degree, 360.0 * Gaudi::Units::degree,
                                               startThetaAngle(), deltaThetaAngle(), m_sphere_coverModel );
    } else if ( 0.0 * Gaudi::Units::degree != startThetaAngle() || 180.0 * Gaudi::Units::degree != deltaThetaAngle() ) {
      /** cover for sphere segment with no gap in phi is
       *  the full sphere - ball
       */
      m_cover = std::make_unique<SolidSphere>( "Cover for " + name(), outerRadius(), insideRadius(), startPhiAngle(),
                                               deltaPhiAngle(), 0.0 * Gaudi::Units::degree,
                                               180.0 * Gaudi::Units::degree, m_sphere_coverModel );
    } else {
      //    cover for full sphere is box
      m_cover = std::make_unique<SolidBox>( "Cover for " + name(), outerRadius(), outerRadius(), outerRadius() );
    }
  }
}

// ============================================================================
/** -# calculate the intersection points("ticks") of the solid objects
 *    with given line.
 *  - Line is parametrized with parameter \a t :
 *     \f$ \vec{x}(t) = \vec{p} + t \times \vec{v} \f$
 *      - \f$ \vec{p} \f$ is a point on the line
 *      - \f$ \vec{v} \f$ is a vector along the line
 *  - \a tick is just a value of parameter \a t, at which the
 *    intersection of the solid and the line occurs
 *  - both  \a Point  (\f$\vec{p}\f$) and \a Vector
 *    (\f$\vec{v}\f$) are defined in local reference system
 *   of the solid
 *  -# implementation of ISolid abstract interface
 *  @see ISolid
 *  @param Point initial point for the line
 *  @param Vector vector along the line
 *  @param ticks output container of "Ticks"
 *  @return the number of intersection points
 */
// ============================================================================
unsigned int SolidSphere::intersectionTicks( const Gaudi::XYZPoint&  Point,  //
                                             const Gaudi::XYZVector& Vector, //
                                             ISolid::Ticks&          ticks   //
                                             ) const {
  return intersectionTicksImpl( Point, Vector, ticks );
}
// ============================================================================
unsigned int SolidSphere::intersectionTicks( const Gaudi::Polar3DPoint&  Point,  //
                                             const Gaudi::Polar3DVector& Vector, //
                                             ISolid::Ticks&              ticks   //
                                             ) const {
  return intersectionTicksImpl( Point, Vector, ticks );
}
// ============================================================================
unsigned int SolidSphere::intersectionTicks( const Gaudi::RhoZPhiPoint&  Point,  //
                                             const Gaudi::RhoZPhiVector& Vector, //
                                             ISolid::Ticks&              ticks   //
                                             ) const {
  return intersectionTicksImpl( Point, Vector, ticks );
}
// ============================================================================
template <class aPoint, class aVector>
unsigned int SolidSphere::intersectionTicksImpl( const aPoint&  Point,  //
                                                 const aVector& Vector, //
                                                 ISolid::Ticks& ticks   //
                                                 ) const {
  unsigned int rc = 0;
  ticks.clear();
  /// line with null direction vector in not able to intersect something
  if ( Vector.mag2() > 0 ) {
    ///  try to intersect with sphere outer radius
    if ( 0 != SolidTicks::LineIntersectsTheSphere( Point, Vector, outerRadius(), std::back_inserter( ticks ) ) ) {
      /// check for intersection with inner radius
      if ( insideRadius() > 0 ) {
        SolidTicks::LineIntersectsTheSphere( Point, Vector, insideRadius(), std::back_inserter( ticks ) );
      }
      // check for phi angle
      if ( 0 * Gaudi::Units::degree != startPhiAngle() || //
           360 * Gaudi::Units::degree != deltaPhiAngle() ) {
        SolidTicks::LineIntersectsThePhi( Point, Vector, startPhiAngle(), std::back_inserter( ticks ) );
        SolidTicks::LineIntersectsThePhi( Point, Vector, endPhiAngle(), std::back_inserter( ticks ) );
      }
      /// check for theta angle
      if ( 0 * Gaudi::Units::degree != startThetaAngle() || //
           180 * Gaudi::Units::degree != deltaThetaAngle() ) {
        SolidTicks::LineIntersectsTheTheta( Point, Vector, startThetaAngle(), std::back_inserter( ticks ) );
        SolidTicks::LineIntersectsTheTheta( Point, Vector, endThetaAngle(), std::back_inserter( ticks ) );
      }
      /// sort and remove adjancent and some EXTRA ticks
      rc = SolidTicks::RemoveAdjacentTicks( ticks, Point, Vector, *this );
    }
  }
  return rc;
}

// ============================================================================
/** - printout to STD/STL stream
 *  - implementation of ISolid abstract interface
 *  - reimplementation of SolidBase::printOut(std::ostream&)
 *  @see ISolid
 *  @see SolidBase
 *  @param os STD/STL stream
 *  @return reference to the stream
 */
// ============================================================================
std::ostream& SolidSphere::printOut( std::ostream& os ) const {
  /// serialize the base class
  SolidBase::printOut( os );
  os << "outerRadius[mm]" << DetDesc::print( outerRadius() / Gaudi::Units::mm );
  if ( 0 != insideR2() ) { os << "innerRadius[mm]" << DetDesc::print( insideRadius() / Gaudi::Units::mm ); }
  if ( 0 * Gaudi::Units::degree != startThetaAngle() || 180 * Gaudi::Units::degree != deltaThetaAngle() ) {
    os << "startThetaAngle[deg]" << DetDesc::print( startThetaAngle() / Gaudi::Units::degree );
    os << "deltaThetaAngle[deg]" << DetDesc::print( deltaThetaAngle() / Gaudi::Units::degree );
  }
  if ( 0 * Gaudi::Units::degree != startPhiAngle() || 360 * Gaudi::Units::degree != deltaPhiAngle() ) {
    os << "startPhiAngle[deg]" << DetDesc::print( startPhiAngle() / Gaudi::Units::degree );
    os << "deltaPhiAngle[deg]" << DetDesc::print( deltaPhiAngle() / Gaudi::Units::degree );
  }
  ///
  return os << std::endl;
}

// ============================================================================
/** - printout to Gaudi  stream
 *  - implementation of ISolid abstract interface
 *  - reimplementation of SolidBase::printOut(MsgStream&)
 *  @see ISolid
 *  @see SolidBase
 *  @param os Gaudi stream
 *  @return reference to the stream
 */
// ============================================================================
MsgStream& SolidSphere::printOut( MsgStream& os ) const {
  /// serialize the base class
  SolidBase::printOut( os );
  os << "outerRadius[mm]" << DetDesc::print( outerRadius() / Gaudi::Units::mm );
  if ( 0 != insideR2() ) { os << "innerRadius[mm]" << DetDesc::print( insideRadius() / Gaudi::Units::mm ); }
  if ( 0 * Gaudi::Units::degree != startThetaAngle() || 180 * Gaudi::Units::degree != deltaThetaAngle() ) {
    os << "startThetaAngle[deg]" << DetDesc::print( startThetaAngle() / Gaudi::Units::degree );
    os << "deltaThetaAngle[deg]" << DetDesc::print( deltaThetaAngle() / Gaudi::Units::degree );
  }
  if ( 0 * Gaudi::Units::degree != startPhiAngle() || 360 * Gaudi::Units::degree != deltaPhiAngle() ) {
    os << "startPhiAngle[deg]" << DetDesc::print( startPhiAngle() / Gaudi::Units::degree );
    os << "deltaPhiAngle[deg]" << DetDesc::print( deltaPhiAngle() / Gaudi::Units::degree );
  }
  ///
  return os << endmsg;
}
// ============================================================================
