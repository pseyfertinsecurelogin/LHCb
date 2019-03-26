
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
#include <iostream>

// local
#include "LHCbMath/Geom/Vector.h"
#include "LHCbMath/Geom/XYZ.h"

/** @namespace LHCb::Geom
 *
 *  Namespace for basic geometry classes, supporting SIMD
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   12/03/2019
 */

namespace LHCb::Geom {

  /** @class Point
   *  Position vector
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   12/03/2019
   */
  template <typename TYPE>
  class Point : public XYZ<TYPE> {

  public:
    // Use the base class constructor(s)
    using XYZ<TYPE>::XYZ;

  public:
    // assignment from arbitrary (x,y,z) type
    template <typename AltVect>
    Point<TYPE>& operator=( const AltVect& v ) noexcept {
      this->setX( TYPE( v.X() ) );
      this->setY( TYPE( v.Y() ) );
      this->setZ( TYPE( v.Z() ) );
      return *this;
    }

  public:
    /// Scale by a displacement vector and return new point
    Point<TYPE> operator+( const Vector<TYPE>& v ) const noexcept {
      return {this->X() + v.X(), this->Y() + v.Y(), this->Z() + v.Z()};
    }

    /// Subtract two points, forming a direction
    Vector<TYPE> operator-( const Point<TYPE>& p ) const noexcept {
      return {this->X() - p.X(), this->Y() - p.Y(), this->Z() - p.Z()};
    }

    /// Self addition with a displacement vector
    Point<TYPE>& operator+=( const Vector<TYPE>& v ) noexcept {
      this->SetXYZ( this->X() + v.X(), this->Y() + v.Y(), this->Z() + v.Z() );
      return *this;
    }

    /// Self difference with a displacement vector
    Point<TYPE>& operator-=( const Vector<TYPE>& v ) noexcept {
      this->SetXYZ( this->X() - v.X(), this->Y() - v.Y(), this->Z() - v.Z() );
      return *this;
    }
  };

} // namespace LHCb::Geom
