// ===========================================================================
#include <algorithm>
#include <memory>
#include <string> 
#ifdef __INTEL_COMPILER         // Disable ICC remark
  #pragma warning(disable:1572) // Floating-point equality and inequality comparisons are unreliable
#endif
#include "DetDesc/ISolid.h"
#include "DetDesc/Solid.h"
#include "DetDesc/SolidBase.h"
#include "DetDesc/SolidBoolean.h"
#include "DetDesc/SolidIntersection.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/Transform3DTypes.h"

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
SolidIntersection::SolidIntersection( const std::string& name  , 
                                      std::unique_ptr<ISolid> first )
  : SolidBase    ( name         )
  , SolidBoolean ( name , std::move(first) )
{}
// ============================================================================

// ============================================================================
/** constructor 
 *  @param name name of the intersection
 */
// ============================================================================
SolidIntersection::SolidIntersection( const std::string& name )
  : SolidBase    ( name )
  , SolidBoolean ( name )
{}
// ============================================================================

// ============================================================================
bool SolidIntersection::isInside( const Gaudi::XYZPoint   & point ) const 
{
  return isInsideImpl(point);
}
// ============================================================================
bool SolidIntersection::isInside( const Gaudi::Polar3DPoint& point ) const 
{
  return isInsideImpl(point);
}
// ============================================================================
bool SolidIntersection::isInside( const Gaudi::RhoZPhiPoint   & point ) const 
{
  return isInsideImpl(point);
}
// ============================================================================
template <class aPoint>
bool SolidIntersection::isInsideImpl( const aPoint   & point ) const 
{ 
  ///  is point inside the "main" volume?  
  if ( !first()->isInside( point ) ) { return false; }
  /// return the 'and' of all children
  return std::all_of( childBegin() , childEnd() , 
                      Solid::IsInside<aPoint>( point ) );
}

// ============================================================================
/** add intersections 
 *  @param solid pointer         to new solid 
 *  @param mtrx  pointer transformation
 *  @return status code
 */
// ============================================================================
StatusCode  SolidIntersection::intersect( std::unique_ptr<ISolid> solid       , 
                                          const Gaudi::Transform3D* mtrx      )
{  return addChild( std::move(solid) , mtrx ); }

// ============================================================================
/** add intersections 
 *  @param solid pointer         to new solid 
 *  @param position position 
 *  @param rotation rotation 
 *  @return status code
 */
// ============================================================================
StatusCode  SolidIntersection::intersect 
( std::unique_ptr<ISolid> solid  , 
  const Gaudi::XYZPoint&     position , 
  const Gaudi::Rotation3D&    rotation )
{ return addChild( std::move(solid) , position , rotation ) ; }

// ============================================================================
