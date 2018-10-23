/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
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
#include <iostream>
#include <string>
#include <algorithm>
/** DetDesc */
#ifdef __INTEL_COMPILER         // Disable ICC remark
  #pragma warning(disable:1572) // Floating-point equality and inequality comparisons are unreliable
#endif
#include "DetDesc/Solid.h"
#include "DetDesc/SolidSubtraction.h"
#include "DetDesc/SolidException.h"

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
SolidSubtraction::SolidSubtraction( const std::string& name  ,
                                    std::unique_ptr<ISolid> first )
  : SolidBase    ( name         )
  , SolidBoolean ( name , std::move(first) )
{
  if( !SolidBoolean::first() )
    { throw SolidException(" SolidSubtraction:: ISolid* points to NULL! "); }
}
// ============================================================================

// ============================================================================
/** constructor
 *  @param name name of the solid subtraction
 */
// ============================================================================
SolidSubtraction::SolidSubtraction( const std::string& Name)
  : SolidBase    ( Name )
  , SolidBoolean ( Name )
{}
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
bool SolidSubtraction::isInside( const Gaudi::XYZPoint   & point ) const
{
  return isInsideImpl(point);
}
// ============================================================================
bool SolidSubtraction::isInside( const Gaudi::Polar3DPoint& point ) const
{
  return isInsideImpl(point);
}
// ============================================================================
bool SolidSubtraction::isInside( const Gaudi::RhoZPhiPoint   & point ) const
{
  return isInsideImpl(point);
}
// ============================================================================
template <class aPoint>
bool SolidSubtraction::isInsideImpl( const aPoint& point ) const
{
  /// check bounding box
  if ( isOutBBox( point )          ) { return false ; }
  ///  is point inside the "main" volume?
  if ( !first()->isInside( point ) ) { return false; }
  /// find a daughter in which the given point is placed
  auto c = children();
  return std::none_of( begin(c) , end(c) , Solid::isInside( point ) );
}

// ============================================================================
/** subtract child solid to the solid
 *  @param solid pointer to child solid
 *  @param mtrx  pointer to transformation
 *  @return status code
 */
// ============================================================================
StatusCode  SolidSubtraction::subtract( std::unique_ptr<ISolid>    solid    ,
                                        const Gaudi::Transform3D*  mtrx     )
{  return addChild( std::move(solid) , mtrx ); }

// ============================================================================
/** subtract child solid from  the solid
 *  @param child    pointer to child solid
 *  @param position position
 *  @param rotation rotation
 *  @return status code
 */
// ============================================================================
StatusCode  SolidSubtraction::subtract ( std::unique_ptr<ISolid>  child    ,
                                         const Gaudi::XYZPoint&   position ,
                                         const Gaudi::Rotation3D& rotation )
{ return addChild( std::move(child) , position , rotation ) ; }
// ============================================================================

// ============================================================================
// The END
// ============================================================================
