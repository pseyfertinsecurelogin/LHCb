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
/** STD & STL */
#include <algorithm>
#include <iostream>
#include <string>
/** DetDesc */
#ifdef __INTEL_COMPILER             // Disable ICC remark
#  pragma warning( disable : 1572 ) // Floating-point equality and inequality comparisons are unreliable
#endif

#include "DetDesc/SolidException.h"
#include "DetDesc/SolidSubtraction.h"

// ============================================================================
/** @file
 *
 *  implementation of class SolidSubtraction
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date xx/xx/xxxx
 */
// ============================================================================

// ============================================================================
/** constructor
 *  @param name name of the intersection
 *  @param first pointer to first/main solid
 */
// ============================================================================
SolidSubtraction::SolidSubtraction( const std::string& name, std::unique_ptr<ISolid> first )
    : SolidBase( name ), SolidBoolean( name, std::move( first ) ) {
  if ( !SolidBoolean::first() ) { throw SolidException( " SolidSubtraction:: ISolid* points to NULL! " ); }
}
// ============================================================================

// ============================================================================
/** constructor
 *  @param name name of the solid subtraction
 */
// ============================================================================
SolidSubtraction::SolidSubtraction( const std::string& Name ) : SolidBase( Name ), SolidBoolean( Name ) {}
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
bool SolidSubtraction::isInside( const Gaudi::XYZPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
bool SolidSubtraction::isInside( const Gaudi::Polar3DPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================
bool SolidSubtraction::isInside( const Gaudi::RhoZPhiPoint& point ) const { return isInsideImpl( point ); }
// ============================================================================

// ============================================================================
/** subtract child solid to the solid
 *  @param solid pointer to child solid
 *  @param mtrx  pointer to transformation
 *  @return status code
 */
// ============================================================================
StatusCode SolidSubtraction::subtract( std::unique_ptr<ISolid>   solid, //
                                       const Gaudi::Transform3D* mtrx   //
) {
  return addChild( std::move( solid ), mtrx );
}

// ============================================================================
/** subtract child solid from  the solid
 *  @param child    pointer to child solid
 *  @param position position
 *  @param rotation rotation
 *  @return status code
 */
// ============================================================================
StatusCode SolidSubtraction::subtract( std::unique_ptr<ISolid>  child,    //
                                       const Gaudi::XYZPoint&   position, //
                                       const Gaudi::Rotation3D& rotation  //
) {
  return addChild( std::move( child ), position, rotation );
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
