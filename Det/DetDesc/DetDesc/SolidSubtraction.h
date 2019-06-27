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

#pragma once

// STD & STL
#include <algorithm>
#include <functional>

// DetDesc package
#include "DetDesc/Solid.h"
#include "DetDesc/SolidBoolean.h"

// forward declarations from GaudiKernel and DetDesc
class StatusCode;
template <class TYPE>
class SolidFactory;

/** @class SolidSubtraction SolidSubtraction.h "DetDesc/SolidSubtraction.h"
 *
 *  Simple implementatioin of simple boolean solid - SUBTRACTION
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date xx/xx/xxxx
 */

class SolidSubtraction final : public SolidBoolean {
  /// friend factory for instantiation
  friend class SolidFactory<SolidSubtraction>;

public:
  /** constructor
   *  @param name name of the intersection
   *  @param first pointer to first/main solid
   */
  SolidSubtraction( const std::string& name, std::unique_ptr<ISolid> first );

  SolidSubtraction( const SolidSubtraction& ) = delete;            ///< no copy
  SolidSubtraction& operator=( const SolidSubtraction& ) = delete; ///< no =

  /** - retrieve the specific type of the solid
   *  - implementation of ISolid interface
   *  @see ISolid
   *  @return specific type of the solid
   */
  std::string typeName() const override { return "SolidSubtraction"; }

  /** - check for the given 3D-point.
   *    Point coordinates are in the local reference
   *    frame of the solid.
   *  - implementation of ISolid absstract interface
   *  @see ISolid
   *  @param point point (in local reference system of the solid)
   *  @return true if the point is inside the solid
   */

  bool isInside( const Gaudi::XYZPoint& point ) const override;
  bool isInside( const Gaudi::Polar3DPoint& point ) const override;
  bool isInside( const Gaudi::RhoZPhiPoint& point ) const override;

  /** subtract child solid to the solid
   *  @param solid pointer to child solid
   *  @param mtrx  pointer to transformation
   *  @return status code
   */
  StatusCode subtract( std::unique_ptr<ISolid> solid, const Gaudi::Transform3D* mtrx );

  /** subtract child solid from  the solid
   *  @param child    pointer to child solid
   *  @param position position
   *  @param rotation rotation
   *  @return status code
   */
  StatusCode subtract( std::unique_ptr<ISolid>  child,    //
                       const Gaudi::XYZPoint&   position, //
                       const Gaudi::Rotation3D& rotation = Gaudi::Rotation3D() );

protected:
  /** constructor
   *  @param Name name of the solid subtraction
   */
  SolidSubtraction( const std::string& Name = "Anonymous Subtraction" );

private:
  /**
   * implementation of isInside
   * @param  point reference to any kind of point with x(), y(), z()
   * @return bool
   */
  template <class aPoint>
  inline bool isInsideImpl( const aPoint& point ) const {
    bool isIn = ( !isOutBBox( point ) &&        // check bounding box
                  first()->isInside( point ) ); //  is point inside the "main" volume?
    if ( isIn ) {
      // find a daughter in which the given point is placed
      const auto c = children();
      isIn         = std::none_of( begin( c ), end( c ), Solid::isInside( point ) );
    }
    return isIn;
  }
};

//  ===========================================================================
