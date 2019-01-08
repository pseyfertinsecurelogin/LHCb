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
#include <type_traits>

// Gaudi
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "GaudiKernel/Transform3DTypes.h"

// VectorClass
#include "vector3d.h"

namespace LHCb
{
  namespace Math
  {

    /** @namespace VectorClass VectorClassTypes.h
     *
     *  Namespace for classes implementing geometrical entities using the
     *  VectorClass library. Internal implementation choosen to best utilise 
     *  the vectorisation capabilities of the library. See
     *
     *  http://www.agner.org/optimize/
     *
     *  For more details.
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     */
    namespace VectorClass
    {

      /** @class XYZVector VectorClassTypes.h LHCbMath/VectorClassTypes.h
       *
       *  Implementation of a 3D vector.
       *  Uses internally a 4D vector in order to utilise
       *  automatic vectorisation of the classes and operations.
       *
       *  This class is designed to 'smooth' the interaction
       *  and conversions from and to the Gaudi classes.
       *
       *  @author Chris Jones
       *  @date   2015-01-28
       */
      template < typename FTYPE = float,
                 typename BASEVEC = typename std::conditional<std::is_same<FTYPE,float>::value,Vec3f,Vec3d>::type >
      class XYZVector : public BASEVEC
      {

      public:

        /// The floating point type in use
        using Scalar = FTYPE;
        
      public:

        /// Standard constructor
        XYZVector( const FTYPE x = 0.0f, 
                   const FTYPE y = 0.0f,
                   const FTYPE z = 0.0f ) : BASEVEC(x,y,z) { }

        /// Constructor from a Gaudi Point
        XYZVector( const Gaudi::XYZVector& p ) : BASEVEC( p.x(), p.y(), p.z() ) { }

        /// Templated copy constructor
        template < class TYPE >
        XYZVector( const TYPE& v ) : BASEVEC(v) { }
        
        /// Implicit conversion back to a Gaudi Vector
        inline operator Gaudi::XYZVector() const { return { x(), y(), z() }; }
        
        /// Implement dot method
        inline FTYPE dot( const XYZVector& v ) const { return dot_product(*this,v); }

        /// Implement Dot method (alias to dot, to match ROOT...)
        inline FTYPE Dot( const XYZVector& v ) const { return dot(v); }
        
        /// Magnitude squared
        inline FTYPE Mag2() const { return horizontal_add(square(*this)); }

        /// Magnitude squared
        inline FTYPE mag2() const { return Mag2(); }

        /// Get the polar R Coordinate
        inline FTYPE R() const { return std::sqrt(Mag2()); }

        /// 3D Cross product
        inline XYZVector Cross( const XYZVector& v ) const { return cross_product(*this,v); }

        /// Returns a normalised unit vector
        inline XYZVector Unit() const { return normalize_vector(*this); }

        /// Returns a normalised unit vector
        inline XYZVector unit() const { return Unit(); }

      public:

        /// X accessor
        inline FTYPE x() const noexcept { return BASEVEC::get_x(); }

        /// Y accessor
        inline FTYPE y() const noexcept { return BASEVEC::get_y(); }

        /// Z accessor
        inline FTYPE z() const noexcept { return BASEVEC::get_z(); }

        /// Get X Coordinate
        inline FTYPE X() const noexcept { return x(); }

        /// Get Y Coordinate
        inline FTYPE Y() const noexcept { return y(); }

        /// Get Z Coordinate
        inline FTYPE Z() const noexcept { return z(); }

        /// Set X Coordinate
        inline void SetX( const FTYPE x ) noexcept { insert(0,x); }

        /// Set Y Coordinate
        inline void SetY( const FTYPE y ) noexcept { insert(1,y); }

        /// Set Z Coordinate
        inline void SetZ( const FTYPE z ) noexcept { insert(2,z); }

      public:

        /// Overload output to ostream
        friend inline std::ostream& operator << ( std::ostream& os, const XYZVector & v )
        { return os << "(" << v.x() << "," << v.y() << "," << v.z() << ")"; }

      };

      /** @class XYZPoint VectorClassTypes.h LHCbMath/VectorClassTypes.h
       *
       *  Implementation of a 3D point as an Eigen Matrix.
       *  Uses internally a 4D vector in order to utilise
       *  automatic vectorisation of the classes and operations.
       *
       *  This class is designed to 'smooth' the interaction
       *  and conversions from and to the Gaudi classes.
       *
       *  @author Chris Jones
       *  @date   2015-01-28
       */
      
      template < typename FTYPE = float,
                 typename BASEVEC = typename std::conditional<std::is_same<FTYPE,float>::value,Vec3f,Vec3d>::type >
      class XYZPoint : public BASEVEC
      {

      public:
        
        /// The floating point type in use
        using Scalar = FTYPE;

      public:

        /// Standard constructor
        XYZPoint( const FTYPE x = 0.0f, 
                  const FTYPE y = 0.0f,
                  const FTYPE z = 0.0f ) : BASEVEC(x,y,z) { }

        /// Constructor from a Gaudi Point
        XYZPoint( const Gaudi::XYZPoint& p ) : BASEVEC( p.x(), p.y(), p.z() ) { }

        /// Templated copy constructor
        template < class TYPE >
        XYZPoint( const TYPE& p ) : BASEVEC(p) { }
        
        /// Implicit conversion back to a Gaudi Point
        inline operator Gaudi::XYZPoint() const { return { x(), y(), z() }; }

      public:
        
        /// X accessor
        inline FTYPE x() const noexcept { return BASEVEC::get_x(); }
        
        /// Y accessor
        inline FTYPE y() const noexcept { return BASEVEC::get_y(); }
        
        /// Z accessor
        inline FTYPE z() const noexcept { return BASEVEC::get_z(); }

        /// Get X Coordinate
        inline FTYPE X() const noexcept { return x(); }

        /// Get Y Coordinate
        inline FTYPE Y() const noexcept { return y(); }

        /// Get Z Coordinate
        inline FTYPE Z() const noexcept { return z(); }

        /// Set X Coordinate
        inline void SetX( const FTYPE x ) noexcept { insert(0,x); }

        /// Set Y Coordinate
        inline void SetY( const FTYPE y ) noexcept { insert(1,y); }

        /// Set Z Coordinate
        inline void SetZ( const FTYPE z ) noexcept { insert(2,z); }

      public:

        /// Overload output to ostream
        friend inline std::ostream& operator << ( std::ostream& os, const XYZPoint & p )
        { return os << "(" << p.x() << "," << p.y() << "," << p.z() << ")"; }

      };


      /// Shortcut to double type
      using XYZVectorD = XYZVector<double>;
      /// Shortcut to float type
      using XYZVectorF = XYZVector<float>;

      /// Shortcut to double type
      using XYZPointD = XYZPoint<double>;
      /// Shortcut to float type
      using XYZPointF = XYZPoint<float>;

    }
  }
}
