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

/** @namespace LHCb::Geom
 *
 *  Namespace for basic geometry classes, supporting SIMD
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   12/03/2019
 */

namespace LHCb::Geom {

  /** @class XYZ
   *  Basic XYZ type
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   12/03/2019
   */
  template <typename TYPE>
  class XYZ {
  public:
    /// Basic type alias
    using Scalar = TYPE;

  private:
    /// Coordinates
    TYPE _X{}, _Y{}, _Z{};

  public:
    /// Default constructor
    XYZ() = default;
    /// constructor from (x,y,z) values
    XYZ( const Scalar x, const Scalar y, const Scalar z ) : _X( x ), _Y( y ), _Z( z ) {}
    /// Constructor from an arbitrary (x,y,z) type
    template <typename AltXYZ>
    XYZ( const AltXYZ& xyz ) : _X( xyz.X() ), _Y( xyz.Y() ), _Z( xyz.Z() ) {}

  public:
    /// X coordinate
    inline TYPE X() const noexcept { return _X; }
    /// Y coordinate
    inline TYPE Y() const noexcept { return _Y; }
    /// Z coordinate
    inline TYPE Z() const noexcept { return _Z; }

  public:
    /// X coordinate
    inline TYPE x() const noexcept { return X(); }
    /// Y coordinate
    inline TYPE y() const noexcept { return Y(); }
    /// Z coordinate
    inline TYPE z() const noexcept { return Z(); }

  public:
    /// Set X
    inline void SetX( const TYPE x ) noexcept { _X = x; }
    /// Set Y
    inline void SetY( const TYPE y ) noexcept { _Y = y; }
    /// Set Z
    inline void SetZ( const TYPE z ) noexcept { _Z = z; }
    /// Set X,Y,Z
    inline void SetXYZ( const TYPE x, const TYPE y, const TYPE z ) noexcept {
      SetX( x );
      SetY( y );
      SetZ( z );
    }

  public:
    /// Set X
    inline void setX( const TYPE x ) noexcept { SetX( x ); }
    /// Set Y
    inline void setY( const TYPE y ) noexcept { SetY( y ); }
    /// Set Z
    inline void setZ( const TYPE z ) noexcept { SetZ( z ); }
    /// Set X,Y,Z
    inline void setXYZ( const TYPE x, const TYPE y, const TYPE z ) noexcept { SetXYZ( x, y, z ); }

  public:
    /// send to std::ostream
    friend std::ostream& operator<<( std::ostream& s, const XYZ<TYPE>& xyz ) {
      return s << "{ " << xyz.X() << ", " << xyz.Y() << ", " << xyz.Z() << " }";
    }
  };

} // namespace LHCb::Geom
