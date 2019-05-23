
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

// Local
#include "LHCbMath/Geom/Vector.h"

/** @namespace LHCb::Geom
 *
 *  Namespace for basic geometry classes, supporting SIMD
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   12/03/2019
 */

namespace LHCb::Geom {

  /** @class Plane
   *  Basic representation of a SIMD vectorised plane.
   *  Described by the form A.x + B.y + C.z + D = 0 where (x,y,z) is
   *  any point in the plane.
   */
  template <typename TYPE>
  class Plane {
  public:
    /// Basic type alias
    using Scalar = TYPE;

  private:
    /// Plane Normal (X,Y,Z) == plane parameters (A,B,C)
    Vector<TYPE> m_normal;
    /// Parameter D
    TYPE m_D{};

  public:
    /// Default constructor
    Plane() = default;
    /// Constructor from values
    Plane( const TYPE a, const TYPE b, const TYPE c, const TYPE d ) : m_normal( a, b, c ), m_D( d ) { normalize(); }

  public:
    /// Normal Vector
    inline const Vector<TYPE>& Normal() const noexcept { return m_normal; }
    /// Distance to a point
    template <typename POINT>
    inline TYPE Distance( const POINT& p ) const noexcept {
      return m_normal.Dot( p ) + D();
    }

  public:
    inline TYPE A() const noexcept { return m_normal.X(); }
    inline TYPE B() const noexcept { return m_normal.Y(); }
    inline TYPE C() const noexcept { return m_normal.Z(); }
    inline TYPE D() const noexcept { return m_D; }

  public:
    /// send to std::ostream
    friend std::ostream& operator<<( std::ostream& s, const Plane<TYPE>& p ) {
      return s << "{ Normal=" << p.Normal() << " D=" << p.D() << " }";
    }

  private:
    /// normalize the plane components
    void normalize() {
      auto A = m_normal.X();
      auto B = m_normal.Y();
      auto C = m_normal.Z();
      if constexpr ( std::is_arithmetic<TYPE>::value ) {
        // normalize the plane
        const auto s = std::sqrt( ( A * A ) + ( B * B ) + ( C * C ) );
        // what to do if s = 0 ?
        if ( s == TYPE( 0 ) ) {
          m_D = TYPE( 0 );
        } else {
          const auto w = Scalar( 1 ) / s;
          A *= w;
          B *= w;
          C *= w;
          m_D *= w;
          m_normal.setX( A );
          m_normal.setY( B );
          m_normal.setZ( C );
        }
      } else {
        // normalize the plane
        auto s = sqrt( ( A * A ) + ( B * B ) + ( C * C ) );
        // what to do if s = 0 ?
        const auto m = ( s == TYPE( 0 ) );
        // set zero entries to 1 in the vector to avoid /0 later on
        s( m )       = TYPE( 1 );
        m_D( m )     = TYPE( 0 );
        const auto w = TYPE( 1 ) / s;
        A *= w;
        B *= w;
        C *= w;
        m_D *= w;
        m_normal.setX( A );
        m_normal.setY( B );
        m_normal.setZ( C );
      }
    }
  };

} // namespace LHCb::Geom
