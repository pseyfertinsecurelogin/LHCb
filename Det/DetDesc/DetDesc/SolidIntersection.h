/// ==========================================================================
#ifndef      DETDESC_SOLIDINTERSECTION_H
#define      DETDESC_SOLIDINTERSECTION_H 1
// STD & STL
#include <algorithm>
#include <functional>
/// DetDesc
#include "DetDesc/ISolid.h"
#include "DetDesc/SolidBoolean.h"
#include "DetDesc/SolidChild.h"
/// Forward declarations
template <class TYPE>
class SolidFactory;


/** @class SolidIntersection SolidIntersection.h DetDesc/SolidIntersection.h
 *
 *  implementation of boolean solid - intersection of several solids
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   xx/xx/xxxx
 */

class SolidIntersection final : public SolidBoolean
{
  ///
  friend class SolidFactory<SolidIntersection>;

public:

  /** constructor
   *  @param name name of the intersection
   *  @param first pointer to first/main solid
   *  @exception SolidException wrong parameters
   */
  SolidIntersection( const std::string& name , std::unique_ptr<ISolid> first );
  [[deprecated("please call with an std::unique_ptr<ISolid> as first argument")]]
  SolidIntersection( const std::string& name , ISolid* first ) : SolidIntersection(name,std::unique_ptr<ISolid>(first) ) {};

public:

  /** retrieve the specific type of the solid
   *  @return specific type of the solid
   */
  virtual std::string typeName () const { return "SolidIntersection" ; };

  /** - check for the given 3D-point.
   *    Point coordinates are in the local reference
   *    frame of the solid.
   *  - implementation of ISolid absstract interface
   *  @see ISolid
   *  @param "" point (in local reference system of the solid)
   *  @return true if the point is inside the solid
   */
  bool isInside ( const Gaudi::XYZPoint   & ) const ;
  bool isInside ( const Gaudi::Polar3DPoint& point ) const ;
  bool isInside ( const Gaudi::RhoZPhiPoint& point ) const ;
  /** add intersections
   *  @param solid pointer         to new solid
   *  @param mtrx  pointer transformation
   *  @return status code
   */
  StatusCode    intersect( std::unique_ptr<ISolid>   solid                    ,
                           const Gaudi::Transform3D* mtrx                     );
  [[deprecated("please call with an std::unique_ptr<ISolid> as first argument")]]
  StatusCode    intersect( ISolid*               solid                    ,
                           const Gaudi::Transform3D* mtrx                     )
  { return intersect( std::unique_ptr<ISolid>(solid),mtrx); }

  /** add intersections
   *  @param child    pointer to new solid
   *  @param position position
   *  @param rotation rotation
   *  @return status code
   */
  StatusCode intersect( std::unique_ptr<ISolid>  child                    ,
                        const Gaudi::XYZPoint&   position                 ,
                        const Gaudi::Rotation3D& rotation = Gaudi::Rotation3D() );
  [[deprecated("please call with an std::unique_ptr<ISolid> as first argument")]]
  StatusCode intersect( ISolid*                  child                    ,
                        const Gaudi::XYZPoint&   position                 ,
                        const Gaudi::Rotation3D& rotation = Gaudi::Rotation3D() )
  { return intersect( std::unique_ptr<ISolid>(child), position, rotation); }
protected:

  /** constructor
   *  @param name name of intersection
   */
  SolidIntersection( const std::string& name = "Anonymous Intersection" );

private:

  SolidIntersection ( const SolidIntersection& ); ///< no copy
  SolidIntersection& operator=( const SolidIntersection& ); ///< no =

  /**
   * implementation of isInside
   * @param  point reference to any kind of point with x(), y(), z()
   * @return bool
   */
  template <class aPoint>
  bool isInsideImpl(const aPoint& point) const;

};

/// ===========================================================================
#endif  ///< DETDESC_SOLIDINTERSECTION_H
/// ===========================================================================




