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
#include <iostream>
#include <string>
#ifdef __INTEL_COMPILER             // Disable ICC remark
#  pragma warning( disable : 1572 ) // Floating-point equality and inequality comparisons are unreliable
#endif
#include "DetDesc/Solid.h"
#include "DetDesc/SolidException.h"
#include "DetDesc/SolidIntersection.h"

// ============================================================================
/** @file
 *
 *  implementation of class SolidIntersection
 *
 * @author Vanya Belyaev Ivan.Belyaev@itep.ru
 * @date xx/xx/xxx
 */
// ============================================================================

// ============================================================================
/** constructor
 *  @param name name of the intersection
 *  @param first pointer to first/main solid
 */
// ============================================================================
SolidIntersection::SolidIntersection( const std::string& name, std::unique_ptr<ISolid> first )
    : SolidBase( name ), SolidBoolean( name, std::move( first ) ) {}
// ============================================================================

// ============================================================================
/** constructor
 *  @param name name of the intersection
 */
// ============================================================================
SolidIntersection::SolidIntersection( const std::string& name ) : SolidBase( name ), SolidBoolean( name ) {}
// ============================================================================

// ============================================================================
bool SolidIntersection::isInside( const Gaudi::XYZPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
bool SolidIntersection::isInside( const Gaudi::Polar3DPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
bool SolidIntersection::isInside( const Gaudi::RhoZPhiPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
template <class aPoint>
bool SolidIntersection::isInsideImpl( const aPoint& point ) const {
  ///  is point inside the "main" volume?
  if ( !first()->isInside( point ) ) { return false; }
  /// return the 'and' of all children
  auto c = children();
  return std::all_of( begin( c ), end( c ), Solid::isInside( point ) );
}

// ============================================================================
/** add intersections
 *  @param solid pointer         to new solid
 *  @param mtrx  pointer transformation
 *  @return status code
 */
// ============================================================================
StatusCode SolidIntersection::intersect( std::unique_ptr<ISolid> solid, const Gaudi::Transform3D* mtrx ) {
  return addChild( std::move( solid ), mtrx );
}

// ============================================================================
/** add intersections
 *  @param solid pointer         to new solid
 *  @param position position
 *  @param rotation rotation
 *  @return status code
 */
// ============================================================================
StatusCode SolidIntersection::intersect( std::unique_ptr<ISolid> solid, const Gaudi::XYZPoint& position,
                                         const Gaudi::Rotation3D& rotation ) {
  return addChild( std::move( solid ), position, rotation );
}

// ============================================================================
