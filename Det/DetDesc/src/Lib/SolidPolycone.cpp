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
#include "DetDesc/SolidPolycone.h"
#include "boost/container/static_vector.hpp"

// GaudiKernel
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SystemOfUnits.h"
// DetDesc
#include "DetDesc/SolidCons.h"
#include "DetDesc/SolidException.h"
#include "DetDesc/SolidTicks.h"
#include "DetDesc/SolidTubs.h"
// LHCbMath
#include "LHCbMath/FastMaths.h"

//=============================================================================
/** @file SolidPolycone.cpp
 *
 * Implementation file for class : SolidPolycone
 *
 * @author Vanya Belyaev Ivan.Belyaev@itep.ru
 * @date 24/10/2001
 *
 */
//=============================================================================

//=============================================================================
/** Standard constructor
 *  @param Name          the name of the polycone
 *  @param Params        vector of "triplets" (z,(rMax,rMin))
 *  @param StartPhiAngle the azimuthal angle phi at which polycone "begins"
 *  @param DeltaPhiAngle the opening angle
 */
//=============================================================================
SolidPolycone::SolidPolycone( const std::string&             Name,          //
                              const SolidPolycone::Triplets& Params,        //
                              const double                   StartPhiAngle, //
                              const double                   DeltaPhiAngle )
    : SolidBase( Name ), m_triplets( Params ), m_startPhiAngle( StartPhiAngle ), m_deltaPhiAngle( DeltaPhiAngle ) {
  /// check for parameters
  if ( number() < 2 ) { throw SolidException( "SolidPolycone:: wrong number of sections!" ); }
  /// check for Phi
  if ( 0.0 * Gaudi::Units::degree > startPhiAngle() ) {
    throw SolidException( "SolidPolycone ::StartPhiAngle < 0 degree!" );
  }
  if ( 0.0 * Gaudi::Units::degree > deltaPhiAngle() ) {
    throw SolidException( "SolidPolycone ::DeltaPhiAngle < 0 degree!" );
  }
  if ( 360.0 * Gaudi::Units::degree < startPhiAngle() + deltaPhiAngle() ) {
    throw SolidException( "SolidPolycone ::StartPhiAngle+DeltaPhiAngle>2pi" );
  }
  /// sort the triplets
  std::sort( m_triplets.begin(), m_triplets.end() );
  ///
  for ( unsigned int i = 0; i < number(); ++i ) {
    if ( RMin( i ) < 0 ) { throw SolidException( "SolidPolycone ::rMin < 0 !" ); }
    if ( !( RMin( i ) < RMax( i ) ) ) { throw SolidException( "SolidPolycone :: !(rMin < rMax) !" ); }
  }
  // set bounding parameters
  setBP();
  ///
  checkTickContainerCapacity();
  createCover();
}

// ============================================================================
/// set parameters for bounding solids (box, sphere and cylinder)
// ============================================================================
void SolidPolycone::setBP() {
  setZMin( m_triplets.front().first );
  setZMax( m_triplets.front().first );
  setRhoMax( m_triplets.front().second.second );
  setRMax( sqrt( rhoMax() * rhoMax() + zMax() * zMax() ) );

  double rhoMin = 1.e+23;

  for ( const auto& triplet : m_triplets ) {
    const double z = triplet.first;
    setZMin( z < zMin() ? z : zMin() );
    setZMax( z > zMax() ? z : zMax() );

    if ( rhoMin > triplet.second.first ) { rhoMin = triplet.second.first; }

    if ( rhoMin > triplet.second.second ) { rhoMin = triplet.second.second; }

    if ( rhoMax() < triplet.second.first ) { setRhoMax( triplet.second.first ); }

    if ( rhoMax() < triplet.second.second ) { setRhoMax( triplet.second.second ); }

    const double rmax = sqrt( rhoMax() * rhoMax() + z * z );
    setRMax( rmax > rMax() ? rmax : rMax() );
  }

  const double phi1 = startPhiAngle();
  const double phi2 = startPhiAngle() + deltaPhiAngle();

  const auto sphi1 = sin( phi1 );
  const auto cphi1 = cos( phi1 );
  const auto sphi2 = sin( phi2 );
  const auto cphi2 = cos( phi2 );

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

// ============================================================================
/** check for the given point (local frame)
 *  @param point point (in local reference system)
 *  @return true ifpoint is inside the solid
 */
// ============================================================================
bool SolidPolycone::isInside( const Gaudi::XYZPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
bool SolidPolycone::isInside( const Gaudi::Polar3DPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
bool SolidPolycone::isInside( const Gaudi::RhoZPhiPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
template <class aPoint>
bool SolidPolycone::isInsideImpl( const aPoint& point ) const {
  /// check z
  if ( point.z() < triplets().front().first || point.z() > triplets().back().first ) { return false; }

  /// check phi
  if ( !insidePhi( point.phi() ) ) { return false; }

  /// check radius
  auto it = std::find_if( begin(), end(), [&]( const Triplet& i ) { return i.first >= point.z(); } );

  /// outside!
  if ( begin() == it || end() == it ) { return false; }

  const unsigned int i2    = it - begin();
  const unsigned int i1    = i2 - 1;
  const auto         zfrac = ( point.z() - z( i1 ) ) / ( z( i2 ) - z( i1 ) );
  const auto         rmax  = ( 1 - zfrac ) * RMax( i1 ) + zfrac * RMax( i2 );
  const auto         rmin  = ( 1 - zfrac ) * RMin( i1 ) + zfrac * RMin( i2 );

  // const auto rho = std::sqrt( point.perp2() );
  // return !( rho > rmax || rho < rmin );
  const auto rho2 = point.perp2();
  return !( rho2 > ( rmax * rmax ) || rho2 < ( rmin * rmin ) );
}

// ============================================================================/
/** -# retrieve the pointer to "simplified" solid - "cover"
 *  -# implementation of ISolid abstract interface
 *  @see ISolid
 *  @return pointer to "simplified" solid - "cover"
 */
// ============================================================================
void SolidPolycone::createCover() {
  if ( 0.0 != startPhiAngle() || 360 * Gaudi::Units::degree != deltaPhiAngle() ) {
    m_cover = std::make_unique<SolidPolycone>( "Cover for " + name(), triplets() );
  } else {
    auto rmxmx = RMin( 0 );
    auto rmnmn = RMax( 0 );
    auto zmxmx = std::abs( z( 0 ) );
    for ( unsigned int i = 1; i < number(); ++i ) {
      if ( RMax( i ) > rmxmx ) { rmxmx = RMax( i ); }
      if ( RMin( i ) < rmnmn ) { rmnmn = RMin( i ); }
      if ( std::abs( z( i ) ) > zmxmx ) { zmxmx = std::abs( z( i ) ); }
    }
    m_cover = std::make_unique<SolidTubs>( "Cover for " + name(), zmxmx, rmxmx, rmnmn );
  }
}

// ============================================================================
/** - printout to STD/STL stream
 *  - implementation  of ISolid abstract interface
 *  - reimplementation of SolidBase::printOut( std::ostream& )
 *  @see SolidBase
 *  @see ISolid
 *  @param  os STD/STL stream
 *  @return reference to the stream
 */
// ============================================================================
std::ostream& SolidPolycone::printOut( std::ostream& os ) const {
  /// serialize the base class
  SolidBase::printOut( os );
  /// serialize the members
  os << "["
     << " number of planes: " << number();
  for ( unsigned int i = 0; i < number(); ++i ) {
    os << "(Z[mm]=" << z( i ) / Gaudi::Units::millimeter << ",Rmax[mm]=" << RMax( i ) / Gaudi::Units::millimeter
       << ",Rmin[mm]=" << RMax( i ) / Gaudi::Units::millimeter << ")";
  }
  ///
  if ( 0 * Gaudi::Units::degree != startPhiAngle() || 360 * Gaudi::Units::degree != deltaPhiAngle() ) {
    os << " startPhiAngle[deg]" << startPhiAngle() / Gaudi::Units::degree;
    os << " deltaPhiAngle[deg]" << startPhiAngle() / Gaudi::Units::degree;
  }
  ///
  return os;
  ///
}

// ============================================================================
/** printout to Gaudi  stream
 *  @param os Gaudi stream
 *  @return reference to the stream
 */
// ============================================================================
MsgStream& SolidPolycone::printOut( MsgStream& os ) const {
  /// serialize the base class
  SolidBase::printOut( os );
  /// serialize the members
  os << "["
     << " number of planes: " << number();
  for ( unsigned int i = 0; i < number(); ++i ) {
    os << "(Z[mm]=" << z( i ) / Gaudi::Units::millimeter << ",Rmax[mm]=" << RMax( i ) / Gaudi::Units::millimeter
       << ",Rmin[mm]=" << RMax( i ) / Gaudi::Units::millimeter << ")";
  }
  ///
  if ( 0 * Gaudi::Units::degree != startPhiAngle() || 360 * Gaudi::Units::degree != deltaPhiAngle() ) {
    os << " startPhiAngle[deg]" << startPhiAngle() / Gaudi::Units::degree;
    os << " deltaPhiAngle[deg]" << startPhiAngle() / Gaudi::Units::degree;
  }
  ///
  return os;
  ///
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
 *  @param Point initial point for the line
 *  @param Vector vector along the line
 *  @param ticks output container of "Ticks"
 *  @return the number of intersection points
 */
// ============================================================================
unsigned int SolidPolycone::intersectionTicks( const Gaudi::XYZPoint&  Point,  //
                                               const Gaudi::XYZVector& Vector, //
                                               ISolid::Ticks&          ticks ) const {
  return intersectionTicksImpl( Point, Vector, ticks );
}
// ============================================================================
unsigned int SolidPolycone::intersectionTicks( const Gaudi::Polar3DPoint&  Point,  //
                                               const Gaudi::Polar3DVector& Vector, //
                                               ISolid::Ticks&              ticks ) const {
  return intersectionTicksImpl( Point, Vector, ticks );
}
// ============================================================================
unsigned int SolidPolycone::intersectionTicks( const Gaudi::RhoZPhiPoint&  Point,  //
                                               const Gaudi::RhoZPhiVector& Vector, //
                                               ISolid::Ticks&              ticks ) const {
  return intersectionTicksImpl( Point, Vector, ticks );
}
// ============================================================================
template <class aPoint, class aVector>
unsigned int SolidPolycone::intersectionTicksImpl( const aPoint&  Point,  //
                                                   const aVector& Vector, //
                                                   ISolid::Ticks& ticks ) const {
  using namespace LHCb::Math;

  // check tick capacity
  checkTickContainerCapacity();

  /// clear the container
  ticks.clear();

  /// line with null direction vector is not able to intersect any solid!
  if ( Vector.mag2() <= 0 ) { return 0; }

  // bail out if we don't cross the envelope
  if ( !crossBCylinder( Point, Vector ) ) { return 0; }

  // intersect with first z-planes
  ISolid::Ticks tmpticks;
  tmpticks.clear();
  if ( SolidTicks::LineIntersectsTheZ( Point, Vector, z( 0 ), std::back_inserter( tmpticks ) ) ) {
    const auto tick = tmpticks.front();
    const auto x    = Point.x() + tick * Vector.x();
    const auto y    = Point.y() + tick * Vector.y();
    const auto r    = std::sqrt( x * x + y * y );
    if ( RMin( 0 ) <= r && r <= RMax( 0 ) && //
         ( noPhiGap() || insidePhi( fast_atan2( y, x ) ) ) ) {
      ticks.push_back( tick );
    }
  }

  // intersect with last z-plane
  tmpticks.clear();
  if ( SolidTicks::LineIntersectsTheZ( Point, Vector, z( number() - 1 ), std::back_inserter( tmpticks ) ) ) {
    const auto tick = tmpticks.front();
    const auto x    = Point.x() + tick * Vector.x();
    const auto y    = Point.y() + tick * Vector.y();
    const auto r    = std::sqrt( x * x + y * y );
    if ( RMin( number() - 1 ) <= r && r <= RMax( number() - 1 ) && //
         ( noPhiGap() || insidePhi( fast_atan2( y, x ) ) ) ) {
      ticks.push_back( tick );
    }
  }

  if ( !noPhiGap() ) {
    // intersect with phi planes
    tmpticks.clear();
    SolidTicks::LineIntersectsThePhi( Point, Vector, startPhiAngle(), std::back_inserter( tmpticks ) );
    // if( deltaPhiAngle() != M_PI )
    SolidTicks::LineIntersectsThePhi( Point, Vector, startPhiAngle() + deltaPhiAngle(),
                                      std::back_inserter( tmpticks ) );

    // check that we are anywhere inside this cylinder
    for ( const auto& tick : tmpticks ) {
      const auto thisz = Point.z() + tick * Vector.z();
      if ( z( 0 ) <= thisz && thisz <= z( number() - 1 ) ) {
        const auto i = index( thisz );
        assert( i < number() - 1 );
        const auto zfrac = ( thisz - z( i ) ) / ( z( i + 1 ) - z( i ) );
        const auto x     = Point.x() + tick * Vector.x();
        const auto y     = Point.y() + tick * Vector.y();
        const auto r     = std::sqrt( x * x + y * y );
        if ( r >= ( ( 1 - zfrac ) * RMin( i ) + zfrac * RMin( i + 1 ) ) &&
             r <= ( ( 1 - zfrac ) * RMax( i ) + zfrac * RMax( i + 1 ) ) )
          ticks.push_back( tick );
      }
    }
  }

  for ( unsigned int i = 1; i < number(); ++i )
    if ( z( i ) > z( i - 1 ) ) {
      // intersect with the cones
      tmpticks.clear();
      // intersect with outer conical surface
      SolidTicks::LineIntersectsTheCone( Point, Vector, RMax( i - 1 ), RMax( i ), z( i - 1 ), z( i ),
                                         std::back_inserter( tmpticks ) );
      // intersect with inner conical surface
      if ( ( 0 < RMin( i - 1 ) ) || ( 0 < RMin( i ) ) )
        SolidTicks::LineIntersectsTheCone( Point, Vector, RMin( i - 1 ), RMin( i ), z( i - 1 ), z( i ),
                                           std::back_inserter( tmpticks ) );
      // check that we are in the right z and phi range
      for ( const auto& tick : tmpticks ) {
        const auto thisz = Point.z() + tick * Vector.z();
        if ( z( i - 1 ) <= thisz && thisz <= z( i ) &&
             ( noPhiGap() || insidePhi( fast_atan2( Point.y() + tick * Vector.y(), Point.x() + tick * Vector.x() ) ) ) )
          ticks.push_back( tick );
      }
    } else {
      // double z-plane: intersect with this z-plane
      tmpticks.clear();
      if ( SolidTicks::LineIntersectsTheZ( Point, Vector, z( i ), std::back_inserter( tmpticks ) ) ) {
        const auto tick = tmpticks.front();
        const auto x    = Point.x() + tick * Vector.x();
        const auto y    = Point.y() + tick * Vector.y();
        const auto r    = sqrt( x * x + y * y );
        if ( noPhiGap() || insidePhi( fast_atan2( y, x ) ) ) {
          // take an excusive-OR
          bool insideFirst  = RMin( i - 1 ) <= r && r <= RMax( i - 1 );
          bool insideSecond = RMin( i ) <= r && r <= RMax( i );
          if ( ( insideFirst && !insideSecond ) || ( insideSecond && !insideFirst ) ) ticks.push_back( tick );
        }
      }
    }

  // sort the ticks and solve eventual problems
  std::sort( ticks.begin(), ticks.end() );
  return SolidTicks::RemoveAdjacentTicksFast( ticks, Point, Vector, *this );
}

//=============================================================================
/** static function to generate triplets for a cone */
SolidPolycone::Triplets SolidPolycone::makeTriplets( const double ZHalfLength,       //
                                                     const double OuterRadiusMinusZ, //
                                                     const double OuterRadiusPlusZ,  //
                                                     const double InnerRadiusMinusZ, //
                                                     const double InnerRadiusPlusZ ) {
  return {{-ZHalfLength, std::pair{InnerRadiusMinusZ, OuterRadiusMinusZ}},
          {ZHalfLength, std::pair{InnerRadiusPlusZ, OuterRadiusPlusZ}}};
}

//=============================================================================
// The end
//=============================================================================
