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
/** STD & STL  */
#include <iostream>
#include <string>
/** DetDesc */
#include "DetDesc/Solid.h"
#include "DetDesc/SolidException.h"
#include "DetDesc/SolidUnion.h"

// ============================================================================
/** @file
 *
 *  implementation of class SolidUnion
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   xx/xx/xxxx
 */
// ============================================================================

// ============================================================================
/** constructor
 *  @param name name of the intersection
 *  @param first pointer to first/main solid
 */
// ============================================================================
SolidUnion::SolidUnion( const std::string& name, std::unique_ptr<ISolid> first )
    : SolidBase( name ), SolidBoolean( name, std::move( first ) ) {
  if ( UNLIKELY( !SolidBoolean::first() ) ) { throw SolidException( " SolidUnion:: ISolid* points to NULL!" ); }
  createCoverTop();
}
// ============================================================================

// ============================================================================
/** constructor
 *  @param name name of the solid union
 */
// ============================================================================
SolidUnion::SolidUnion( const std::string& name ) : SolidBase( name ), SolidBoolean( name ) {}
// ============================================================================

// ============================================================================
/** - check for the given 3D-point.
 *    Point coordinates are in the local reference
 *    frame of the solid.
 *  - implementation of ISolid absstract interface
 *  @see ISolid
 *  @param point point (in local reference system of the solid)
 *  @return true if the point is inside the solid
 */
// ============================================================================
bool SolidUnion::isInside( const Gaudi::XYZPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
bool SolidUnion::isInside( const Gaudi::Polar3DPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
bool SolidUnion::isInside( const Gaudi::RhoZPhiPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
template <class aPoint>
bool SolidUnion::isInsideImpl( const aPoint& point ) const {
  /// check bounding box
  if ( isOutBBox( point ) ) { return false; }
  ///  is point inside the "main" volume?
  if ( first()->isInside( point ) ) { return true; }
  /// find the first daughter in which the given point is placed
  auto c = children();
  return std::any_of( begin( c ), end( c ), Solid::isInside( point ) );
}

// ============================================================================
/** add child solid to the solid union
 *  @param solid pointer to child solid
 *  @param mtrx  pointer to transformation
 *  @return status code
 */
// ============================================================================
StatusCode SolidUnion::unite( std::unique_ptr<ISolid> solid, const Gaudi::Transform3D* mtrx ) {
  auto sc = addChild( std::move( solid ), mtrx );
  return sc.isSuccess() ? updateBP() : sc;
}

// ============================================================================
/** add child solid to the solid union
 *  @param child pointer to child solid
 *  @param position position
 *  @return status code
 */
// ============================================================================
StatusCode SolidUnion::unite( std::unique_ptr<ISolid> child, const Gaudi::XYZPoint& position,
                              const Gaudi::Rotation3D& rotation ) {
  auto sc = addChild( std::move( child ), position, rotation );
  return sc.isSuccess() ? updateBP() : sc;
}
// ============================================================================

// ============================================================================
/** create the cover box
 */
// ============================================================================
void SolidUnion::createCoverTop() {
  const double x = std::max( std::abs( xMin() ), std::abs( xMax() ) );
  const double y = std::max( std::abs( yMin() ), std::abs( yMax() ) );
  const double z = std::max( std::abs( zMin() ), std::abs( zMax() ) );
  m_coverTop     = std::make_unique<SolidBox>( "CoverTop for " + name(), x, y, z );
}
// ============================================================================

// ============================================================================
/** update bonding parameters
 *  @return status code
 */
// ============================================================================
StatusCode SolidUnion::updateBP() {
  auto c = children();
  if ( !c ) { return StatusCode::SUCCESS; }
  // get last child
  const auto& base = c.back();
  //
  setXMin( std::min( base.xMin(), xMin() ) );
  setXMax( std::max( base.xMax(), xMax() ) );

  setYMin( std::min( base.yMin(), yMin() ) );
  setYMax( std::max( base.yMax(), yMax() ) );

  setZMin( std::min( base.zMin(), zMin() ) );
  setZMax( std::max( base.zMax(), zMax() ) );

  setRMax( std::max( base.rMax(), rMax() ) );
  setRhoMax( std::max( base.rhoMax(), rhoMax() ) );
  //
  checkBP();
  createCoverTop();
  return StatusCode::SUCCESS;
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
