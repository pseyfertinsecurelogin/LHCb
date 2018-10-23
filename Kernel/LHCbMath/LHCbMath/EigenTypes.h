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

#pragma once

// STL
#include <cmath>

// Gaudi
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "GaudiKernel/Plane3DTypes.h"
#include "GaudiKernel/Transform3DTypes.h"

// Eigen
#include <Eigen/Geometry>

// VDT
#include "vdt/sqrt.h"

namespace LHCb
{
  namespace Math
  {

    /** @namespace Eigen EigenTypes.h
     *
     *  Namespace for classes implementing geometrical entities using the
     *  Eigen library. Internal implementation choosen to utilise the
     *  vectorisation capabilities of the library. For details see
     *
     *  http://eigen.tuxfamily.org
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     */
    namespace Eigen
    {
      
      /** @class XYZVector EigenTypes.h LHCbMath/EigenTypes.h
       *
       *  Implementation of a 3D vector as an Eigen Matrix.
       *  Uses internally a 4D float Matrix in order to utilise
       *  automatic vectorisation of the classes and operations.
       *
       *  This class is designed to 'smooth' the interaction
       *  and conversions from and to the Gaudi classes.
       *
       *  @author Chris Jones
       *  @date   2015-01-28
       */

      template < typename FTYPE = float,
                 typename E4Vector = typename ::Eigen::Matrix< FTYPE , 4 , 1 >,
                 typename E3Vector = typename ::Eigen::Matrix< FTYPE , 3 , 1 > >
      class XYZVector : public E4Vector
      {

      public:
        
        /// The floating point type in use
        using Scalar = FTYPE;

      public:

        /// Standard constructor
        XYZVector( const FTYPE x = 0.0, 
                   const FTYPE y = 0.0,
                   const FTYPE z = 0.0 ) : E4Vector(x,y,z,0) { }

        /// Constructor from a Gaudi Point
        XYZVector( const Gaudi::XYZVector& p ) : E4Vector( p.x(), p.y(), p.z(), 0 ) { }

        /// Constructor from an Eigen 3D Vector
        XYZVector( const E3Vector& v ) : E4Vector( v.x(), v.y(), v.z(), 0 ) { }

        /// Templated copy constructor
        template < class TYPE >
        XYZVector( const TYPE& v ) : E4Vector(v) { }
        
        /// Implicit conversion back to a Gaudi Vector
        inline operator Gaudi::XYZVector() const { return { X(), Y(), Z() }; }

      public:

        /// Dot product
        template < class TYPE >
        inline FTYPE dot( const TYPE& v ) const { return E4Vector::dot(v); }

        /// Dot product against a Gaudi vector
        inline FTYPE Dot( const Gaudi::XYZVector& v ) const { return dot(XYZVector(v)); }

        /// Implement 'Dot' method ( alias for dot)
        template < class TYPE >
        inline FTYPE Dot( const TYPE& v ) const { return dot(v); }

        /// Magnitude squared
        inline FTYPE Mag2() const { return dot(*this); }

        /// Magnitude squared
        inline FTYPE mag2() const { return Mag2(); }

        /// Get the polar R Coordinate
        inline FTYPE R() const { return std::sqrt(Mag2()); }

        /// 3D Cross product
        template < class TYPE >
        inline XYZVector Cross( const TYPE& v ) const { return cross3(v); }

        /// Returns a normalised unit vector
        inline XYZVector Unit() const { return (*this) * vdt::fast_isqrt(Mag2()); }

        /// Returns a normalised unit vector
        inline XYZVector unit() const { return Unit(); }

      public: /// Alias methods for accessors and setters

        /// Get X Coordinate
        inline FTYPE x() const noexcept { return E4Vector::x(); }

        /// Get Y Coordinate
        inline FTYPE y() const noexcept { return E4Vector::y(); }

        /// Get Z Coordinate
        inline FTYPE z() const noexcept { return E4Vector::z(); }

        /// Get X Coordinate
        inline FTYPE X() const noexcept { return x(); }

        /// Get Y Coordinate
        inline FTYPE Y() const noexcept { return y(); }

        /// Get Z Coordinate
        inline FTYPE Z() const noexcept { return z(); }

        /// Set X Coordinate
        inline void SetX( const FTYPE x ) noexcept { (*this)[0] = x; }

        /// Set Y Coordinate
        inline void SetY( const FTYPE y ) noexcept { (*this)[1] = y; }

        /// Set Z Coordinate
        inline void SetZ( const FTYPE z ) noexcept { (*this)[2] = z; }

      public:

        /// Overload output to ostream
        friend inline std::ostream& operator << ( std::ostream& os, const XYZVector & v )
        { 
          return os << "(" << v.x() << "," << v.y() << "," << v.z() << ")"; 
        }

      };

      /** @class XYZPoint EigenTypes.h LHCbMath/EigenTypes.h
       *
       *  Implementation of a 3D point as an Eigen Matrix.
       *  Uses internally a 4D FloatType Matrix in order to utilise
       *  automatic vectorisation of the classes and operations.
       *
       *  This class is designed to 'smooth' the interaction
       *  and conversions from and to the Gaudi classes.
       *
       *  @author Chris Jones
       *  @date   2015-01-28
       */

      template < typename FTYPE = float,
                 typename E4Vector = typename ::Eigen::Matrix< FTYPE , 4 , 1 >,
                 typename E3Vector = typename ::Eigen::Matrix< FTYPE , 3 , 1 > >
      class XYZPoint : public E4Vector
      {

      public:
        
        /// The floating point type in use
        using Scalar = FTYPE;

      public:

        /// Standard constructor
        XYZPoint( const FTYPE x = 0.0, 
                  const FTYPE y = 0.0,
                  const FTYPE z = 0.0 ) : E4Vector(x,y,z,0) { }

        /// Constructor from a Gaudi Point
        XYZPoint( const Gaudi::XYZPoint& p ) : E4Vector( p.x(), p.y(), p.z(), 0 ) { }

        /// Templated copy constructor
        template < class TYPE >
        XYZPoint( const TYPE& p ) : E4Vector(p) { }
        
        /// Implicit conversion back to a Gaudi Vector
        inline operator Gaudi::XYZPoint() const { return { x(), y(), z() }; }

      public:

        // /// Operator -
        // inline XYZVector operator-( const Gaudi::XYZPoint& p ) const 
        // { 
        //   return (*this) - E4Vector( p.x(), p.y(), p.z(), 0 );
        // }

        // /// Operator -
        // inline XYZPoint operator-( const Gaudi::XYZVector& v ) const 
        // { 
        //   return (*this) - E4Vector( v.x(), v.y(), v.z(), 0 ); 
        // }

      public: /// Alias methods for accessors and setters

        /// Get the polar R Coordinate
        inline FTYPE R() const { return std::sqrt( dot(*this) ); }

        /// Get X Coordinate
        inline FTYPE x() const noexcept { return E4Vector::x(); }

        /// Get Y Coordinate
        inline FTYPE y() const noexcept { return E4Vector::y(); }

        /// Get Z Coordinate
        inline FTYPE z() const noexcept { return E4Vector::z(); }

        /// Get X Coordinate
        inline FTYPE X() const noexcept { return x(); }

        /// Get Y Coordinate
        inline FTYPE Y() const noexcept { return y(); }

        /// Get Z Coordinate
        inline FTYPE Z() const noexcept { return z(); }

        /// Set X Coordinate
        inline void SetX( const FTYPE x ) noexcept { (*this)[0] = x; }

        /// Set Y Coordinate
        inline void SetY( const FTYPE y ) noexcept { (*this)[1] = y; }

        /// Set Z Coordinate
        inline void SetZ( const FTYPE z ) noexcept { (*this)[2] = z; }

        /// Set all three coordinates
        inline void SetCoordinates( const FTYPE x,
                                    const FTYPE y,
                                    const FTYPE z ) noexcept { *this = E4Vector(x,y,z,0); }

      public:

        /// Overload output to ostream
        friend inline std::ostream& operator << ( std::ostream& os, const XYZPoint & p )
        { 
          return os << "(" << p.x() << "," << p.y() << "," << p.z() << ")"; 
        }

      };

      /** @class Rotation3D EigenTypes.h LHCbMath/EigenTypes.h
       *
       *  Implementation of a 3D Rotation
       *
       *  This class is designed to 'smooth' the interaction
       *  and conversions from and to the Gaudi classes.
       *
       *  @author Chris Jones
       *  @date   2015-01-28
       */
      class Rotation3D
      {
      };

      /** @class Plane3D EigenTypes.h LHCbMath/EigenTypes.h
       *
       *  Implementation of a 3D plane as an Eigen Hyperplane
       *
       *  This class is designed to 'smooth' the interaction
       *  and conversions from and to the Gaudi classes.
       *
       *  @author Chris Jones
       *  @date   2015-01-28
       */
    
      template < typename FTYPE  = float,
                 typename EPlane = typename ::Eigen::Hyperplane< FTYPE, 3 > >
      class Plane3D : public EPlane
      {

      public:
        
        /// The floating point type in use
        using Scalar = FTYPE;

      private:

        typedef ::Eigen::Matrix<FTYPE,3,1> Vec3f;

      private:

        /// Stupid method to deal with fact Gaudi::XYZPlane's accessors are not const..
        template < typename TYPE >
        inline static TYPE& nonc( const TYPE& t ) 
        {
          return const_cast<TYPE&>(t);
        } 
        
      public :   

        /// Default constructor
        Plane3D() = default;
      
        /** Constructor from parameters from the general form
         *  according to the equation ax + by + cz + d = 0 */
        Plane3D( const FTYPE a, const FTYPE b,
                 const FTYPE c, const FTYPE d ) 
          : EPlane( Vec3f(a,b,c), d ) { }

        /// Constructor from a normal vector and a point in the plane
        template < class VECTOR, class POINT >
        Plane3D( const VECTOR& v, const POINT& p ) 
          : EPlane( Vec3f(v.x(),v.y(),v.z()), 
                    Vec3f(p.x(),p.y(),p.z()) ) { }

        /// Constructor from three points in the plane
        template < class POINT >
        Plane3D( const POINT& p1, const POINT& p2, const POINT& p3 )
          : EPlane( EPlane::Through( Vec3f(p1.x(),p1.y(),p1.z()),
                                     Vec3f(p2.x(),p2.y(),p2.z()),
                                     Vec3f(p3.x(),p3.y(),p3.z()) ) ) { }       

        /** Constructor from a Gaudi Plane3D
         *  Why on Earth are the Plane acccessors not const... */
        Plane3D( const Gaudi::Plane3D& p )
          : EPlane( Vec3f(nonc(p).A(),nonc(p).B(),nonc(p).C()), nonc(p).D() ) { }

        /// Implicit conversion back to a Gaudi Plane3D
        inline operator Gaudi::Plane3D() const 
        {
          const auto n = EPlane::normal();
          return Gaudi::Plane3D( n.x(), n.y(), n.z(), EPlane::offset() );
        }

      public:

        /// Access the plane normal 
        inline XYZVector<FTYPE> Normal() const 
        { 
          const auto n = EPlane::normal();
          return { n.x(), n.y(), n.z() };
        }

        /// Distance to a point
        template < class POINT >
        inline FTYPE Distance( const POINT& p ) const 
        { 
          return EPlane::signedDistance( Vec3f(p.x(),p.y(),p.z()) ); 
        }

      public:

        /// Overload output to ostream
        friend inline std::ostream& operator << ( std::ostream& os, const Plane3D & p )
        { 
          return os << "(" << p.coeffs() << ")"; 
        }

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
