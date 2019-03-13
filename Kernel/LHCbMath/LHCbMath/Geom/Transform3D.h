
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

// STL
#include <array>

// local
#include "LHCbMath/Geom/Point.h"
#include "LHCbMath/Geom/Vector.h"

/** @namespace LHCb::Geom
 *
 *  Namespace for basic geometry classes, supporting SIMD
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   12/03/2019
 */

namespace LHCb::Geom {

  /** @class Transform3D
   *  Position vector
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   12/03/2019
   */
  template <typename T>
  class Transform3D {

  private:
    enum ETransform3DMatrixIndex {
      kXX = 0,
      kXY = 1,
      kXZ = 2,
      kDX = 3,
      kYX = 4,
      kYY = 5,
      kYZ = 6,
      kDY = 7,
      kZX = 8,
      kZY = 9,
      kZZ = 10,
      kDZ = 11
    };

    std::array<T, 12> fM = {{}}; ///< transformation elements (3x4 matrix)

  public:
    void SetComponents( T xx, T xy, T xz, T dx, T yx, T yy, T yz, T dy, T zx, T zy, T zz, T dz ) {
      fM[kXX] = xx;
      fM[kXY] = xy;
      fM[kXZ] = xz;
      fM[kDX] = dx;
      fM[kYX] = yx;
      fM[kYY] = yy;
      fM[kYZ] = yz;
      fM[kDY] = dy;
      fM[kZX] = zx;
      fM[kZY] = zy;
      fM[kZZ] = zz;
      fM[kDZ] = dz;
    }

  public:
    // Transformation operation for Position Vector in Cartesian coordinate
    // For a Position Vector first a rotation and then a translation is applied
    Point<T> operator()( const Point<T>& p ) const noexcept {
      return Point<T>{fM[kXX] * p.X() + fM[kXY] * p.Y() + fM[kXZ] * p.Z() + fM[kDX],
                      fM[kYX] * p.X() + fM[kYY] * p.Y() + fM[kYZ] * p.Z() + fM[kDY],
                      fM[kZX] * p.X() + fM[kZY] * p.Y() + fM[kZZ] * p.Z() + fM[kDZ]};
    }

    // Transformation operation for Displacement Vectors in Cartesian coordinate
    // For the Displacement Vectors only the rotation applies - no translations
    Vector<T> operator()( const Vector<T>& v ) const noexcept {
      return Vector<T>{fM[kXX] * v.X() + fM[kXY] * v.Y() + fM[kXZ] * v.Z(),
                       fM[kYX] * v.X() + fM[kYY] * v.Y() + fM[kYZ] * v.Z(),
                       fM[kZX] * v.X() + fM[kZY] * v.Y() + fM[kZZ] * v.Z()};
    }

    // point transform operator
    Point<T> operator*( const Point<T>& p ) const noexcept { return operator()( p ); }

    // vector transform operator
    Vector<T> operator*( const Vector<T>& v ) const noexcept { return operator()( v ); }
  };

} // namespace LHCb::Geom
