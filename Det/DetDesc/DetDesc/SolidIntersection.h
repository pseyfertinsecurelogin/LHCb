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

  SolidIntersection ( const SolidIntersection& ) = delete ; ///< no copy
  SolidIntersection& operator=( const SolidIntersection& ) = delete ; ///< no =

  /** retrieve the specific type of the solid
   *  @return specific type of the solid
   */
  std::string typeName () const override { return "SolidIntersection" ; }

  /** - check for the given 3D-point.
   *    Point coordinates are in the local reference
   *    frame of the solid.
   *  - implementation of ISolid absstract interface
   *  @see ISolid
   *  @param "" point (in local reference system of the solid)
   *  @return true if the point is inside the solid
   */
  bool isInside ( const Gaudi::XYZPoint   & ) const override;
  bool isInside ( const Gaudi::Polar3DPoint& point ) const override;
  bool isInside ( const Gaudi::RhoZPhiPoint& point ) const override;
  /** add intersections
   *  @param solid pointer         to new solid
   *  @param mtrx  pointer transformation
   *  @return status code
   */
  StatusCode    intersect( std::unique_ptr<ISolid>   solid                    ,
                           const Gaudi::Transform3D* mtrx                     );

  /** add intersections
   *  @param child    pointer to new solid
   *  @param position position
   *  @param rotation rotation
   *  @return status code
   */
  StatusCode intersect( std::unique_ptr<ISolid>  child                    ,
                        const Gaudi::XYZPoint&   position                 ,
                        const Gaudi::Rotation3D& rotation = Gaudi::Rotation3D() );
protected:

  /** constructor
   *  @param name name of intersection
   */
  SolidIntersection( const std::string& name = "Anonymous Intersection" );

private:

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
