/// ===========================================================================
#ifndef DETDESC_SOLID_H
#define DETDESC_SOLID_H 1
/// STD & STL
#include <string>
#include <functional>
/// Geometry
#include "GaudiKernel/Point3DTypes.h"
/// DetDesc
#include "DetDesc/ISolid.h"

/** @namespace Solid Solid.h "DetDesc/Solid.h"
 *
 *  A number of useful methods to deal with solids
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   03/08/2001
 */

namespace Solid
{
  /** @class IsInside Solid.h "DetDesc/Solid.h"
   *
   *  useful class to check if point is inside of
   *  the given solid
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date 03/08/2001
   */
  template<class aPoint>
  class IsInside
  {
  public:

    /** (explicit) constructor
     *   @param LocalPoint  point (in local reference system of solid)
     */
    explicit IsInside( const aPoint& LocalPoint  )
      : m_point( LocalPoint ){};

    /** check if the point is inside of solid
     *  @param solid pointer to ISolid object
     *  @return true, if point is inside the solid
     */
    inline bool operator() ( const ISolid* solid ) const
    { return solid && solid->isInside( m_point ); };

    inline bool operator() ( const ISolid& solid ) const
    { return solid.isInside( m_point ); };

  private:

    aPoint m_point; ///< point in the Local Reference System of Solid

  };


  template <typename Point>
  IsInside<Point> isInside( Point&& p ) { return IsInside<Point>{std::forward<Point>(p)}; }

} ///< end of namespace Solid

/// ===========================================================================
#endif ///< DETDESC_SOLID_H
/// ===========================================================================
