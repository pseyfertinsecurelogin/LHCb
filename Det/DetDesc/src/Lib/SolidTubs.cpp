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
// ============================================================================
// Units
#include "GaudiKernel/SystemOfUnits.h"
#include "boost/container/static_vector.hpp"

// DetDesc includes
#include "DetDesc/DetDesc.h"
#include "DetDesc/SolidBox.h"
#include "DetDesc/SolidException.h"
#include "DetDesc/SolidTicks.h"
#include "DetDesc/SolidTubs.h"

// LHCbMaths
#include "LHCbMath/FastMaths.h"

// STL
#include <memory>

// ============================================================================
/** @file SolidTubs.cpp
 *
 *  implementation of class SolidTubs
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date xx/xx/xx
 */
// ============================================================================

// ============================================================================
/** constructor
 *  @param name           name of tube segment
 *  @param ZHalfLength    half-length of the tube segment
 *  @param OuterRadius    outer radius of tube segment
 *  @param InnerRadius    inner radius of tube segment
 *  @param StartPhiAngle  start phi angle
 *  @param DeltaPhiAngle  delta phi angle
 *  @param CoverModel     covering model
 *  @exception SolidException wrong parameter range
 */
// ============================================================================
SolidTubs::SolidTubs( const std::string& name,          //
                      const double       ZHalfLength,   //
                      const double       OuterRadius,   //
                      const double       InnerRadius,   //
                      const double       StartPhiAngle, //
                      const double       DeltaPhiAngle, //
                      const int          CoverModel )
    : SolidBase( name )
    , m_tubs_zHalfLength( ZHalfLength )
    , m_tubs_outerRadius( OuterRadius )
    , m_tubs_outerRadius_Sq( OuterRadius * OuterRadius )
    , m_tubs_innerRadius( InnerRadius )
    , m_tubs_innerRadius_Sq( InnerRadius * InnerRadius )
    , m_tubs_startPhiAngle( StartPhiAngle )
    , m_tubs_deltaPhiAngle( DeltaPhiAngle )
    , m_tubs_coverModel( CoverModel )
    , m_noPhiGap( true ) {
  if ( 0 >= ZHalfLength ) { throw SolidException( "SolidTubs::ZHalfLength is not positive!" ); }
  if ( 0 >= OuterRadius ) { throw SolidException( "SolidTubs::OuterRadius is not positive!" ); }
  if ( 0 > InnerRadius ) { throw SolidException( "SolidTubs::InnerRadius is negative    !" ); }
  if ( InnerRadius >= OuterRadius ) { throw SolidException( "SolidTubs::InnerRadius >= OuterRadius !" ); }
  if ( -180.0 * Gaudi::Units::degree > StartPhiAngle ) {
    throw SolidException( "SolidTubs::StartPhiAngle is < -180 degree! " );
  }
  if ( 360.0 * Gaudi::Units::degree < StartPhiAngle ) {
    throw SolidException( "SolidTubs::StartPhiAngle is >  360 degree! " );
  }
  if ( 0.0 * Gaudi::Units::degree > DeltaPhiAngle ) {
    throw SolidException( "SolidTubs::DeltaPhiAngle is <    0 degree! " );
  }
  if ( 360.0 * Gaudi::Units::degree < DeltaPhiAngle ) {
    throw SolidException( "SolidTubs::DeltaPhiAngle is >  360 degree! " );
  }
  if ( 360.0 * Gaudi::Units::degree < StartPhiAngle + DeltaPhiAngle ) {
    throw SolidException( "SolidTubs::StartPhiAngle+DeltaPhiAngle >2pi" );
  }
  //
  m_noPhiGap = true;
  if ( 0 * Gaudi::Units::degree != startPhiAngle() ) { m_noPhiGap = false; }
  if ( 360 * Gaudi::Units::degree != deltaPhiAngle() ) { m_noPhiGap = false; }
  // set bounding parameters
  setBP();
  //
  createCover();
}
// ============================================================================

// ============================================================================
/// set parameters for bounding solids (box, sphere and cylinder)
// ============================================================================
void SolidTubs::setBP() {
  // set bounding paramters of SolidBase class
  setZMin( -zHalfLength() );
  setZMax( zHalfLength() );
  setRhoMax( outerRadius() );
  setRMax( sqrt( zMax() * zMax() + rhoMax() * rhoMax() ) );

  const double phi1  = startPhiAngle();
  const double phi2  = startPhiAngle() + deltaPhiAngle();
  const auto   sphi1 = sin( phi1 );
  const auto   cphi1 = cos( phi1 );
  const auto   sphi2 = sin( phi2 );
  const auto   cphi2 = cos( phi2 );

  const double rhoMin = innerRadius();

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

  //
  checkBP();
}
// ============================================================================

// ============================================================================
/** constructor
 *  @param name name of tube segment
 */
// ============================================================================
SolidTubs::SolidTubs( const std::string& name ) : SolidBase( name ) { createCover(); }

// ============================================================================
/** - check for the given 3D-point.
 *    Point coordinated are in the local reference
 *    frame of the solid.
 *  - implementation of ISolid absstract interface
 *  @see ISolid
 *  @param point point (in local reference system of the solid)
 *  @return true if the point is inside the solid
 */
// ============================================================================
bool SolidTubs::isInside( const Gaudi::XYZPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
bool SolidTubs::isInside( const Gaudi::Polar3DPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
bool SolidTubs::isInside( const Gaudi::RhoZPhiPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================

// ============================================================================
void SolidTubs::createCover() {
  if ( 0 == m_tubs_coverModel ) {
    if ( 0.0 * Gaudi::Units::degree != startPhiAngle() || //
         360.0 * Gaudi::Units::degree != deltaPhiAngle() ) {
      m_cover = std::make_unique<SolidTubs>( "Cover for " + name(), zHalfLength(), outerRadius(), innerRadius() );
    } else if ( 0.0 != innerRadius() ) {
      m_cover = std::make_unique<SolidTubs>( "Cover for " + name(), zHalfLength(), outerRadius() );
    } else {
      m_cover = std::make_unique<SolidBox>( "Cover for " + name(), outerRadius(), outerRadius(), zHalfLength() );
    }
  } else {
    if ( 0.0 != innerRadius() ) {
      m_cover =
          std::make_unique<SolidTubs>( "Cover for " + name(), zHalfLength(), outerRadius(), 0.0 * Gaudi::Units::mm,
                                       startPhiAngle(), deltaPhiAngle(), m_tubs_coverModel );
    } else if ( 0.0 * Gaudi::Units::degree != startPhiAngle() || 360.0 * Gaudi::Units::degree != deltaPhiAngle() ) {
      m_cover =
          std::make_unique<SolidTubs>( "Cover for " + name(), zHalfLength(), outerRadius(), innerRadius(),
                                       0.0 * Gaudi::Units::degree, 360.0 * Gaudi::Units::degree, m_tubs_coverModel );
    } else {
      m_cover = std::make_unique<SolidBox>( "Cover for " + name(), outerRadius(), outerRadius(), zHalfLength() );
    }
  }
}

// ============================================================================
/** - calculate the intersection points("ticks") of the solid objects
 *    with given line.
 *  -# Line is parametrized with parameter \a t :
 *     \f$ \vec{x}(t) = \vec{p} + t \times \vec{v} \f$
 *      - \f$ \vec{p} \f$ is a point on the line
 *      - \f$ \vec{v} \f$ is a vector along the line
 *  -# \a tick is just a value of parameter \a t, at which the
 *    intersection of the solid and the line occurs
 *  -# both  \a Point  (\f$\vec{p}\f$) and \a Vector
 *    (\f$\vec{v}\f$) are defined in local reference system
 *   of the solid
 *  - implementation of ISolid abstract interface
 *  @see ISolid
 *  @param Point  initial point for the line
 *  @param Vector vector along the line
 *  @param ticks  output container of "Ticks"
 *  @return the number of intersection points
 */
// ============================================================================
template <class aPoint, class aVector>
unsigned int SolidTubs::intersectionTicksImpl( const aPoint&  Point,  //
                                               const aVector& Vector, //
                                               ISolid::Ticks& ticks   //
                                               ) const {

  using namespace LHCb::Math;

  /// clear the container
  ticks.clear();

  /// line with null direction vector is not able to intersect any solid.
  if ( UNLIKELY( Vector.mag2() == 0 ) ) { return 0; } ///< RETURN!

  // need to optimize these
  // if( !crossBSphere  ( point , vect ) )   { return 0 ; }
  if ( UNLIKELY( !crossBCylinder( Point, Vector ) ) ) { return 0; }

  // first the cylinders
  ISolid::Ticks tmpticks;
  // tmpticks.reserve(2);
  SolidTicks::LineIntersectsTheCylinder( Point, Vector, outerRadius(), std::back_inserter( tmpticks ) );
  if ( innerRadius() > 0 ) {
    SolidTicks::LineIntersectsTheCylinder( Point, Vector, innerRadius(), std::back_inserter( tmpticks ) );
  }

  // current number of ticks, + some for below...
  // ticks.reserve( tmpticks.size() + 2 );

  // check that ticks are actually inside z-range and phi-range
  for ( const auto& tick : tmpticks ) {
    if ( fabs( Point.z() + tick * Vector.z() ) <= zHalfLength() &&
         ( noPhiGap() || insidePhi( fast_atan2( Point.y() + tick * Vector.y(), //
                                                Point.x() + tick * Vector.x() ) ) ) ) {
      ticks.push_back( tick );
    }
  }

  // find intersection points("ticks") with z-planes
  tmpticks.clear();
  SolidTicks::LineIntersectsTheZ( Point, Vector, -1.0 * zHalfLength(), std::back_inserter( tmpticks ) );
  SolidTicks::LineIntersectsTheZ( Point, Vector, zHalfLength(), std::back_inserter( tmpticks ) );

  // check that ticks are actually inside radial range and phi
  for ( const auto& tick : tmpticks ) {
    const auto x   = Point.x() + ( tick * Vector.x() );
    const auto y   = Point.y() + ( tick * Vector.y() );
    const auto rSq = ( ( x * x ) + ( y * y ) );
    if ( innerRadiusSq() <= rSq && //
         rSq <= outerRadiusSq() && //
         ( noPhiGap() || insidePhi( fast_atan2( y, x ) ) ) ) {
      ticks.push_back( tick );
    }
  }

  if ( UNLIKELY( !noPhiGap() ) ) {
    tmpticks.clear();
    SolidTicks::LineIntersectsThePhi( Point, Vector, startPhiAngle(), std::back_inserter( tmpticks ) );
    // if( deltaPhiAngle() != M_PI )
    SolidTicks::LineIntersectsThePhi( Point, Vector, startPhiAngle() + deltaPhiAngle(),
                                      std::back_inserter( tmpticks ) );

    // check that we are anywhere inside this cylinder
    for ( const auto& tick : tmpticks ) {
      if ( fabs( Point.z() + tick * Vector.z() ) < zHalfLength() ) {
        const auto x   = Point.x() + ( tick * Vector.x() );
        const auto y   = Point.y() + ( tick * Vector.y() );
        const auto rSq = ( ( x * x ) + ( y * y ) );
        if ( innerRadiusSq() <= rSq && //
             rSq <= outerRadiusSq() ) {
          ticks.push_back( tick );
        }
      }
    }
  }

  std::sort( ticks.begin(), ticks.end() );

  return SolidTicks::RemoveAdjacentTicksFast( ticks, Point, Vector, *this );
}
// ============================================================================
unsigned int SolidTubs::intersectionTicks( const Gaudi::XYZPoint&  Point,  //
                                           const Gaudi::XYZVector& Vector, //
                                           ISolid::Ticks&          ticks   //
                                           ) const {
  return intersectionTicksImpl( Point, Vector, ticks );
}
// ============================================================================
unsigned int SolidTubs::intersectionTicks( const Gaudi::Polar3DPoint&  Point,  //
                                           const Gaudi::Polar3DVector& Vector, //
                                           ISolid::Ticks&              ticks   //
                                           ) const {
  return intersectionTicksImpl( Point, Vector, ticks );
}
// ============================================================================
unsigned int SolidTubs::intersectionTicks( const Gaudi::RhoZPhiPoint&  Point,  //
                                           const Gaudi::RhoZPhiVector& Vector, //
                                           ISolid::Ticks&              ticks   //
                                           ) const {
  return intersectionTicksImpl( Point, Vector, ticks );
}

// ============================================================================
/** - printout to STD/STL stream
 *  - implementation  of ISolid abstract interface
 *  - reimplementation of SolidBase::printOut( std::ostream& )
 *  @see SolidBase
 *  @see ISolid
 *  @param os STD/STL stream
 *  @return reference to the stream
 */
// ============================================================================
std::ostream& SolidTubs::printOut( std::ostream& os ) const {
  /// serialize the base
  SolidBase::printOut( os );
  os << "["
     << " sizeZ[mm]=" << DetDesc::print( zLength() / Gaudi::Units::mm )
     << " outerRadius[mm]=" << DetDesc::print( outerRadius() / Gaudi::Units::mm );
  if ( 0 != innerRadius() ) { os << " innerRadius[mm]=" << DetDesc::print( innerRadius() / Gaudi::Units::mm ); }
  if ( 0.0 * Gaudi::Units::degree != startPhiAngle() || 360.0 * Gaudi::Units::degree != deltaPhiAngle() ) {
    os << " startPhiAngle[deg]=" << DetDesc::print( startPhiAngle() / Gaudi::Units::degree )
       << " deltaPhiAngle[deg]=" << DetDesc::print( deltaPhiAngle() / Gaudi::Units::degree );
  };
  return os << "]" << std::endl;
}

// ============================================================================
/** - printout to Gaudi MsgStream stream
 *  - implementation  of ISolid abstract interface
 *  - reimplementation of SolidBase::printOut( MsgStream& )
 *  @see SolidBase
 *  @see ISolid
 *  @param os Gaudi MsgStream  stream
 *  @return reference to the stream
 */
// ============================================================================
MsgStream& SolidTubs::printOut( MsgStream& os ) const {
  /// serialize the base
  SolidBase::printOut( os );
  os << "["
     << " sizeZ[mm]=" << DetDesc::print( zLength() / Gaudi::Units::mm )
     << " outerRadius[mm]=" << DetDesc::print( outerRadius() / Gaudi::Units::mm );
  if ( 0 != innerRadius() ) { os << " innerRadius[mm]=" << DetDesc::print( innerRadius() / Gaudi::Units::mm ); }
  if ( 0.0 * Gaudi::Units::degree != startPhiAngle() || 360.0 * Gaudi::Units::degree != deltaPhiAngle() ) {
    os << " startPhiAngle[deg]=" << DetDesc::print( startPhiAngle() / Gaudi::Units::degree )
       << " deltaPhiAngle[deg]=" << DetDesc::print( deltaPhiAngle() / Gaudi::Units::degree );
  };
  return os << "]" << endmsg;
}

// ============================================================================
/** calculate the intersection points("ticks") of the solid objects
 *  with given line.
 *  - Line is parametrized with parameter \a t :
 *     \f$ \vec{x}(t) = \vec{p} + t \times \vec{v} \f$
 *      - \f$ \vec{p} \f$ is a point on the line
 *      - \f$ \vec{v} \f$ is a vector along the line
 *  - \a tick is just a value of parameter \a t, at which the
 *    intersection of the solid and the line occurs
 *  - both  \a Point  (\f$\vec{p}\f$) and \a Vector
 *    (\f$\vec{v}\f$) are defined in local reference system
 *   of the solid
 *  Only intersection ticks within the range
 *   \a tickMin and \a tickMax are taken into account.
 *  @see ISolid::intersectionTicks()
 *  @param Point initial point for the line
 *  @param Vector vector along the line
 *  @param tickMin minimum value of Tick
 *  @param tickMax maximu value of Tick
 *  @param ticks output container of "Ticks"
 *  @return the number of intersection points
 */
// ============================================================================
template <class aPoint, class aVector>
inline unsigned int __attribute__( ( always_inline ) )         //
SolidTubs::intersectionTicksImpl( const aPoint&       Point,   //
                                  const aVector&      Vector,  //
                                  const ISolid::Tick& tickMin, //
                                  const ISolid::Tick& tickMax, //
                                  ISolid::Ticks&      ticks ) const {
  // don't call down to SolidBase because it performs more complicated
  // tick operations than we need
  unsigned int ret = 0;
  if ( UNLIKELY( isOutBBox( Point, Vector, tickMin, tickMax ) ) ) {
    ticks.clear();
  } else {
    SolidTubs::intersectionTicksImpl( Point, Vector, ticks );
    ret = SolidTicks::adjustToTickRange( ticks, tickMin, tickMax );
  }
  return ret;
}
// ============================================================================
unsigned int SolidTubs::intersectionTicks( const Gaudi::XYZPoint&  Point,   //
                                           const Gaudi::XYZVector& Vector,  //
                                           const ISolid::Tick&     tickMin, //
                                           const ISolid::Tick&     tickMax, //
                                           ISolid::Ticks&          ticks    //
                                           ) const {
  return intersectionTicksImpl( Point, Vector, tickMin, tickMax, ticks );
}
// ============================================================================
unsigned int SolidTubs::intersectionTicks( const Gaudi::Polar3DPoint&  Point,   //
                                           const Gaudi::Polar3DVector& Vector,  //
                                           const ISolid::Tick&         tickMin, //
                                           const ISolid::Tick&         tickMax, //
                                           ISolid::Ticks&              ticks    //
                                           ) const {
  return intersectionTicksImpl( Point, Vector, tickMin, tickMax, ticks );
}
// ============================================================================
unsigned int SolidTubs::intersectionTicks( const Gaudi::RhoZPhiPoint&  Point,   //
                                           const Gaudi::RhoZPhiVector& Vector,  //
                                           const ISolid::Tick&         tickMin, //
                                           const ISolid::Tick&         tickMax, //
                                           ISolid::Ticks&              ticks    //
                                           ) const {
  return intersectionTicksImpl( Point, Vector, tickMin, tickMax, ticks );
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
