
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
#include <cmath>
#include <iostream>

// local
#include "LHCbMath/Geom/XYZ.h"

/** @namespace LHCb::Geom
 *
 *  Namespace for basic geometry classes, supporting SIMD
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   12/03/2019
 */

namespace LHCb::Geom {

  /** @class Vector
   *  Direction vector
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   12/03/2019
   */
  template <typename TYPE>
  class Vector : public XYZ<TYPE> {

  public:
    // Use the base class constructor(s)
    using XYZ<TYPE>::XYZ;

  public:
    // assignment from arbitrary (x,y,z) type
    template <typename AltVect>
    Vector<TYPE>& operator=( const AltVect& v ) noexcept {
      this->setX( TYPE( v.X() ) );
      this->setY( TYPE( v.Y() ) );
      this->setZ( TYPE( v.Z() ) );
      return *this;
    }

  public:
    /// Dot product with arbitrary (x,y,z) type
    template <typename AltVect>
    TYPE Dot( const AltVect& v ) const noexcept {
      return TYPE( this->X() * v.X() ) + ( this->Y() * v.Y() ) + ( this->Z() * v.Z() );
    }

    /// Cross product with arbitrary (x,y,z) type
    template <typename AltVect>
    Vector<TYPE> Cross( const AltVect& v ) const noexcept {
      return {this->Y() * v.z() - v.y() * this->Z(),  //
              this->Z() * v.x() - v.z() * this->X(),  //
              this->X() * v.y() - v.x() * this->Y()}; //
    }

  public:
    /// Return scaled vector
    Vector<TYPE> operator*( const TYPE scale ) const noexcept {
      return {scale * this->X(), scale * this->Y(), scale * this->Z()};
    }

    /// Self addition with a displacement vector
    Vector<TYPE>& operator+=( const Vector<TYPE>& v ) noexcept {
      this->SetXYZ( this->X() + v.X(), this->Y() + v.Y(), this->Z() + v.Z() );
      return *this;
    }

    /// Self difference with a displacement vector
    Vector<TYPE>& operator-=( const Vector<TYPE>& v ) noexcept {
      this->SetXYZ( this->X() - v.X(), this->Y() - v.Y(), this->Z() - v.Z() );
      return *this;
    }

  public:
    /// Vector magnitude squared
    TYPE Mag2() const noexcept {
      return ( ( this->X() * this->X() ) + ( this->Y() * this->Y() ) + ( this->Z() * this->Z() ) );
    }
  };

  /// Return scaled vector
  template <typename TYPE>
  Vector<TYPE> operator*( const TYPE scale, const Vector<TYPE>& v ) {
    return {scale * v.X(), scale * v.Y(), scale * v.Z()};
  }

} // namespace LHCb::Geom
