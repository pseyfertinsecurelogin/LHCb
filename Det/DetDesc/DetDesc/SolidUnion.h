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
#ifndef       DETDESC_SOLIDUNION_H
#define       DETDESC_SOLIDUNION_H 1

/** STD & STL */
#include <algorithm>
#include <functional>

/** DetDesc */
#include "DetDesc/ISolid.h"
#include "DetDesc/SolidBoolean.h"
#include "DetDesc/SolidBox.h"

/** forward declarations from GaudiKernel and DetDesc  */
class StatusCode;
template <class TYPE>
class SolidFactory;

/** @class SolidUnion SolidUnion.h "DetDesc/SolidUnion.h"
 *
 *  implementation of simple boolean solid - UNION
 *
 *  @bug wrong cover method!
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date xx/xx/xxx
 */

class SolidUnion final: public virtual SolidBoolean
{
  /// frined factory for instantiation
  friend class SolidFactory<SolidUnion>;

public:

  /** constructor
   *  @param name name of the intersection
   *  @param first pointer to first/main solid
   */
  SolidUnion( const std::string& name , std::unique_ptr<ISolid> first );

  SolidUnion           ( const SolidUnion& ) = delete ; ///< no copy
  SolidUnion& operator=( const SolidUnion& ) = delete ; ///< no =

  /** - retrieve the specific type of the solid
   *  - implementation of ISolid interface
   *  @see ISolid
   *  @return specific type of the solid
   */
  std::string typeName () const override { return "SolidUnion" ; }

  /** - check for the given 3D-point.
   *    Point coordinates are in the local reference
   *    frame of the solid.
   *  - implementation of ISolid absstract interface
   *  @see ISolid
   *  @param point point (in local reference system of the solid)
   *  @return true if the point is inside the solid
   */
  bool isInside ( const Gaudi::XYZPoint   & point ) const override;
  bool isInside ( const Gaudi::Polar3DPoint& point ) const override;
  bool isInside ( const Gaudi::RhoZPhiPoint& point ) const override;

  /** retrieve the pointer to "the most simplified cover",
   *  ideally to something like "the bounding box"
   *  @see ISolid::cover()
   *  @return pointer to the most simplified cover
   */
  inline const ISolid* coverTop() const override { return m_coverTop.get(); }

  /** add child solid to the solid union
   *  @param solid pointer to child solid
   *  @param mtrx  pointer to transformation
   *  @return status code
   */
  StatusCode unite ( std::unique_ptr<ISolid>  solid ,
                     const Gaudi::Transform3D* mtrx  );

  /** add child solid to the solid union
   *  @param child    pointer to child solid
   *  @param position position
   *  @param rotation rotation
   *  @return status code
   */
  StatusCode unite ( std::unique_ptr<ISolid>    child                    ,
                     const Gaudi::XYZPoint&     position                 ,
                     const Gaudi::Rotation3D&    rotation = Gaudi::Rotation3D() );

 protected:

  /** constructor
   *  @param Name name of the solid union
   */
  SolidUnion( const std::string& Name = "Anonymous Union");

  /** update bounding parameters
   *  @return StatusCode
   */
  StatusCode updateBP();

 private:

  void createCoverTop(); /// create/reset the cover pointer

  std::unique_ptr<SolidBox> m_coverTop;
  template <class aPoint>
  bool isInsideImpl(const aPoint& point) const;

};

// ===========================================================================
#endif  ///<  DETDESC_SOLIDUNION_H
// ===========================================================================
