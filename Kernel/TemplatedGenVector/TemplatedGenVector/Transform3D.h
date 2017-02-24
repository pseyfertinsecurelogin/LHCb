// @(#)root/mathcore:$Id$
// Authors: W. Brown, M. Fischler, L. Moneta    2005

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2005 , LCG LHCbROOT MathLib Team                         *
 *                                                                    *
 *                                                                    *
 **********************************************************************/

// Header file for class Transform3D
//
// Created by: Lorenzo Moneta  October 21 2005
//
//
#ifndef LHCbROOT_Math_GenVector_Transform3D
#define LHCbROOT_Math_GenVector_Transform3D  1



#ifndef LHCbROOT_Math_GenVector_DisplacementVector3D
#include "TemplatedGenVector/DisplacementVector3D.h"
#endif

#ifndef LHCbROOT_Math_GenVector_PositionVector3D
#include "TemplatedGenVector/PositionVector3D.h"
#endif

#ifndef LHCbROOT_Math_GenVector_Rotation3D
#include "TemplatedGenVector/Rotation3D.h"
#endif

#ifndef LHCbROOT_Math_GenVector_Translation3D
#include "TemplatedGenVector/Translation3D.h"
#endif


#include "TemplatedGenVector/AxisAnglefwd.h"
#include "TemplatedGenVector/EulerAnglesfwd.h"
#include "TemplatedGenVector/Quaternionfwd.h"
#include "TemplatedGenVector/RotationZYXfwd.h"
#include "TemplatedGenVector/RotationXfwd.h"
#include "TemplatedGenVector/RotationYfwd.h"
#include "TemplatedGenVector/RotationZfwd.h"

#include "TemplatedGenVector/Plane3D.h"

#include <iostream>

//#include "Math/Vector3Dfwd.h"



namespace LHCbROOT {

namespace Math {

//_________________________________________________________________________________________
/**
    Basic 3D Transformation class describing  a rotation and then a translation
    The internal data are a 3D rotation data (represented as a 3x3 matrix) and a 3D vector data.
    They are represented and held in this class like a 3x4 matrix (a simple array of 12 numbers).

    The class can be constructed from any 3D rotation object
    (LHCbROOT::Math::Rotation3D, LHCbROOT::Math::AxisAngle, LHCbROOT::Math::Quaternion, etc...) and/or
    a 3D Vector (LHCbROOT::Math::DislacementVector3D or via LHCbROOT::Math::Translation ) representing a Translation.
    The Transformation is defined by applying first the rotation and then the translation.
    A transformation defined by applying first a translation and then a rotation is equivalent to the
    transformation obtained applying first the rotation and then a translation equivalent to the rotated vector.
    The operator * can be used to obtain directly such transformations, in addition to combine various
    transformations.
    Keep in mind that the operator * (like in the case of rotations ) is not commutative.
    The operator * is used (in addition to operator() ) to apply a transformations on the vector
    (DisplacementVector3D and LorentzVector classes) and point (PositionVector3D)  classes.
    In the case of Vector objects the transformation only rotates them and does not translate them.
    Only Point objects are able to be both rotated and translated.


    @ingroup GenVector

*/

template < typename FTYPE >
class Transform3D {


public:
  
   typedef FTYPE Scalar;
   typedef DisplacementVector3D<Cartesian3D<FTYPE>, DefaultCoordinateSystemTag >  Vector;
   typedef PositionVector3D<Cartesian3D<FTYPE>, DefaultCoordinateSystemTag >      Point;


   enum ETransform3DMatrixIndex {
      kXX = 0, kXY = 1, kXZ = 2, kDX = 3,
      kYX = 4, kYY = 5, kYZ = 6, kDY = 7,
      kZX = 8, kZY = 9, kZZ =10, kDZ = 11
   };



   /**
       Default constructor (identy rotation) + zero translation
   */
   Transform3D()
   {
      SetIdentity();
   }

   /**
      Construct given a pair of pointers or iterators defining the
      beginning and end of an array of 12 Scalars
   */
   template<class IT>
   Transform3D(IT begin, IT end)
   {
      SetComponents(begin,end);
   }

   /**
      Construct from a rotation and then a translation described by a Vector
   */
   Transform3D( const Rotation3D & r, const Vector & v)
   {
      AssignFrom( r, v );
   }
   /**
      Construct from a rotation and then a translation described by a Translation3D class
   */
    Transform3D( const Rotation3D & r, const Translation3D<FTYPE> & t)
   {
      AssignFrom( r, t.Vect() );
   }

   /**
      Construct from a rotation (any rotation object)  and then a translation
      (represented by any DisplacementVector)
      The requirements on the rotation and vector objects are that they can be transformed in a
      Rotation3D class and in a Cartesian3D Vector
   */
   template <class ARotation, class CoordSystem, class Tag>
   Transform3D( const ARotation & r, const DisplacementVector3D<CoordSystem,Tag> & v)
   {
      AssignFrom( Rotation3D(r), Vector (v.X(),v.Y(),v.Z()) );
   }

   /**
      Construct from a rotation (any rotation object)  and then a translation
      represented by a Translation3D class
      The requirements on the rotation is that it can be transformed in a
      Rotation3D class
   */
   template <class ARotation>
   Transform3D( const ARotation & r, const Translation3D<FTYPE> & t)
   {
      AssignFrom( Rotation3D(r), t.Vect() );
   }


#ifdef OLD_VERSION
   /**
      Construct from a translation and then a rotation (inverse assignment)
   */
   Transform3D( const Vector & v, const Rotation3D & r)
   {
      // is equivalent from having first the rotation and then the translation vector rotated
      AssignFrom( r, r(v) );
   }
#endif

   /**
      Construct from a 3D Rotation only with zero translation
   */
   explicit Transform3D( const Rotation3D & r) {
      AssignFrom(r);
   }

   // convenience methods for constructing a Transform3D from all the 3D rotations classes
   // (cannot use templates for conflict with LA)

   explicit Transform3D( const AxisAngle & r) {
      AssignFrom(Rotation3D(r));
   }
   explicit Transform3D( const EulerAngles & r) {
      AssignFrom(Rotation3D(r));
   }
   explicit Transform3D( const Quaternion & r) {
      AssignFrom(Rotation3D(r));
   }
   explicit Transform3D( const RotationZYX & r) {
      AssignFrom(Rotation3D(r));
   }

   // Constructors from axial rotations
   // TO DO: implement direct methods for axial rotations without going through Rotation3D
   explicit Transform3D( const RotationX & r) {
      AssignFrom(Rotation3D(r));
   }
   explicit Transform3D( const RotationY & r) {
      AssignFrom(Rotation3D(r));
   }
   explicit Transform3D( const RotationZ & r) {
      AssignFrom(Rotation3D(r));
   }

   /**
      Construct from a translation only, represented by any DisplacementVector3D
      and with an identity rotation
   */
   template<class CoordSystem, class Tag>
   explicit Transform3D( const DisplacementVector3D<CoordSystem,Tag> & v) {
      AssignFrom(Vector(v.X(),v.Y(),v.Z()));
   }
   /**
      Construct from a translation only, represented by a Cartesian 3D Vector,
      and with an identity rotation
   */
   explicit Transform3D( const Vector & v) {
      AssignFrom(v);
   }
   /**
      Construct from a translation only, represented by a Translation3D class
      and with an identity rotation
   */
   explicit Transform3D( const Translation3D<FTYPE> & t) {
     AssignFrom(t.Vect());
   }



   //#if !defined(__MAKECINT__) && !defined(G__DICTIONARY)  // this is ambigous with Scalar * , Scalar *


#ifdef OLD_VERSION
   /**
      Construct from a translation (using any type of DisplacementVector )
      and then a rotation (any rotation object).
      Requirement on the rotation and vector objects are that they can be transformed in a
      Rotation3D class and in a Vector
   */
   template <class ARotation, class CoordSystem, class Tag>
   Transform3D(const DisplacementVector3D<CoordSystem,Tag> & v , const ARotation & r)
   {
      // is equivalent from having first the rotation and then the translation vector rotated
      Rotation3D r3d(r);
      AssignFrom( r3d, r3d( Vector(v.X(),v.Y(),v.Z()) ) );
   }
#endif


   /**
      Construct transformation from one coordinate system defined by three
      points (origin + two axis) to
      a new coordinate system defined by other three points (origin + axis)
      @param fr0  point defining origin of original reference system
      @param fr1  point defining first axis of original reference system
      @param fr2  point defining second axis of original reference system
      @param to0  point defining origin of transformed reference system
      @param to1  point defining first axis transformed reference system
      @param to2  point defining second axis transformed reference system

   */
   Transform3D
   (const Point & fr0, const Point & fr1, const Point & fr2,
    const Point & to0, const Point & to1, const Point & to2 )
   {
     // takes impl. from CLHEP ( E.Chernyaev). To be checked
     
     Vector x1,y1,z1, x2,y2,z2;
     x1 = (fr1 - fr0).Unit();
     y1 = (fr2 - fr0).Unit();
     x2 = (to1 - to0).Unit();
     y2 = (to2 - to0).Unit();
     
     //   C H E C K   A N G L E S
     
     Scalar cos1, cos2;
     cos1 = x1.Dot(y1);
     cos2 = x2.Dot(y2);
     
     if (std::fabs(1.0-cos1) <= 0.000001 || std::fabs(1.0-cos2) <= 0.000001) {
       std::cerr << "Transform3D: Error : zero angle between axes" << std::endl;
       SetIdentity();
     } 
     else 
     {
       if (std::fabs(cos1-cos2) > 0.000001) {
         std::cerr << "Transform3D: Warning: angles between axes are not equal"
                   << std::endl;
       }
      
       //   F I N D   R O T A T I O N   M A T R I X
       
       z1 = (x1.Cross(y1)).Unit();
       y1  = z1.Cross(x1);
       
       z2 = (x2.Cross(y2)).Unit();
       y2  = z2.Cross(x2);
       
       Scalar x1x = x1.x();      Scalar x1y = x1.y();      Scalar x1z = x1.z();
       Scalar y1x = y1.x();      Scalar y1y = y1.y();      Scalar y1z = y1.z();
       Scalar z1x = z1.x();      Scalar z1y = z1.y();      Scalar z1z = z1.z();
       
       Scalar x2x = x2.x();      Scalar x2y = x2.y();      Scalar x2z = x2.z();
       Scalar y2x = y2.x();      Scalar y2y = y2.y();      Scalar y2z = y2.z();
       Scalar z2x = z2.x();      Scalar z2y = z2.y();      Scalar z2z = z2.z();
       
       Scalar detxx =  (y1y *z1z  - z1y *y1z );
       Scalar detxy = -(y1x *z1z  - z1x *y1z );
       Scalar detxz =  (y1x *z1y  - z1x *y1y );
       Scalar detyx = -(x1y *z1z  - z1y *x1z );
       Scalar detyy =  (x1x *z1z  - z1x *x1z );
       Scalar detyz = -(x1x *z1y  - z1x *x1y );
       Scalar detzx =  (x1y *y1z  - y1y *x1z );
       Scalar detzy = -(x1x *y1z  - y1x *x1z );
       Scalar detzz =  (x1x *y1y  - y1x *x1y );
       
       Scalar txx = x2x *detxx + y2x *detyx + z2x *detzx;
       Scalar txy = x2x *detxy + y2x *detyy + z2x *detzy;
       Scalar txz = x2x *detxz + y2x *detyz + z2x *detzz;
       Scalar tyx = x2y *detxx + y2y *detyx + z2y *detzx;
       Scalar tyy = x2y *detxy + y2y *detyy + z2y *detzy;
       Scalar tyz = x2y *detxz + y2y *detyz + z2y *detzz;
       Scalar tzx = x2z *detxx + y2z *detyx + z2z *detzx;
       Scalar tzy = x2z *detxy + y2z *detyy + z2z *detzy;
       Scalar tzz = x2z *detxz + y2z *detyz + z2z *detzz;
       
       //   S E T    T R A N S F O R M A T I O N
       
       Scalar dx1 = fr0.x(), dy1 = fr0.y(), dz1 = fr0.z();
       Scalar dx2 = to0.x(), dy2 = to0.y(), dz2 = to0.z();
       
       SetComponents(txx, txy, txz, dx2-txx*dx1-txy*dy1-txz*dz1,
                     tyx, tyy, tyz, dy2-tyx*dx1-tyy*dy1-tyz*dz1,
                     tzx, tzy, tzz, dz2-tzx*dx1-tzy*dy1-tzz*dz1);
     }
   }


   // use compiler generated copy ctor, copy assignmet and dtor

   /**
      Construct from a linear algebra matrix of size at least 3x4,
      which must support operator()(i,j) to obtain elements (0,0) thru (2,3).
      The 3x3 sub-block is assumed to be the rotation part and the translations vector
      are described by the 4-th column
   */
   template<class ForeignMatrix>
   explicit Transform3D(const ForeignMatrix & m) {
      SetComponents(m);
   }

   /**
      Raw constructor from 12 Scalar components
   */
   Transform3D(Scalar  xx, Scalar  xy, Scalar  xz, Scalar dx,
               Scalar  yx, Scalar  yy, Scalar  yz, Scalar dy,
               Scalar  zx, Scalar  zy, Scalar  zz, Scalar dz)
   {
      SetComponents (xx, xy, xz, dx, yx, yy, yz, dy, zx, zy, zz, dz);
   }


   /**
      Construct from a linear algebra matrix of size at least 3x4,
      which must support operator()(i,j) to obtain elements (0,0) thru (2,3).
      The 3x3 sub-block is assumed to be the rotation part and the translations vector
      are described by the 4-th column
   */
   template<class ForeignMatrix>
   Transform3D & operator= (const ForeignMatrix & m) {
      SetComponents(m);
      return *this;
   }


   // ======== Components ==============


   /**
      Set the 12 matrix components given an iterator to the start of
      the desired data, and another to the end (12 past start).
   */
   template<class IT>
#ifndef NDEBUG
   void SetComponents(IT begin, IT end) {
#else
   void SetComponents(IT begin, IT ) {
#endif
      for (int i = 0; i <12; ++i) {
         fM[i] = *begin;
         ++begin;
      }
      assert (end==begin);
   }

   /**
      Get the 12 matrix components into data specified by an iterator begin
      and another to the end of the desired data (12 past start).
   */
   template<class IT>
#ifndef NDEBUG
   void GetComponents(IT begin, IT end) const {
#else
   void GetComponents(IT begin, IT ) const {
#endif
      for (int i = 0; i <12; ++i) {
         *begin = fM[i];
         ++begin;
      }
      assert (end==begin);
   }

   /**
      Get the 12 matrix components into data specified by an iterator begin
   */
   template<class IT>
   void GetComponents(IT begin) const {
      std::copy ( fM, fM+12, begin );
   }

   /**
      Set components from a linear algebra matrix of size at least 3x4,
      which must support operator()(i,j) to obtain elements (0,0) thru (2,3).
      The 3x3 sub-block is assumed to be the rotation part and the translations vector
      are described by the 4-th column
   */
   template<class ForeignMatrix>
   void
   SetTransformMatrix (const ForeignMatrix & m) {
      fM[kXX]=m(0,0);  fM[kXY]=m(0,1);  fM[kXZ]=m(0,2); fM[kDX]=m(0,3);
      fM[kYX]=m(1,0);  fM[kYY]=m(1,1);  fM[kYZ]=m(1,2); fM[kDY]=m(1,3);
      fM[kZX]=m(2,0);  fM[kZY]=m(2,1);  fM[kZZ]=m(2,2); fM[kDZ]=m(2,3);
   }

   /**
      Get components into a linear algebra matrix of size at least 3x4,
      which must support operator()(i,j) for write access to elements
      (0,0) thru (2,3).
   */
   template<class ForeignMatrix>
   void
   GetTransformMatrix (ForeignMatrix & m) const {
      m(0,0)=fM[kXX];  m(0,1)=fM[kXY];  m(0,2)=fM[kXZ];  m(0,3)=fM[kDX];
      m(1,0)=fM[kYX];  m(1,1)=fM[kYY];  m(1,2)=fM[kYZ];  m(1,3)=fM[kDY];
      m(2,0)=fM[kZX];  m(2,1)=fM[kZY];  m(2,2)=fM[kZZ];  m(2,3)=fM[kDZ];
   }


   /**
      Set the components from 12 scalars
   */
   void
   SetComponents (Scalar  xx, Scalar  xy, Scalar  xz, Scalar dx,
                  Scalar  yx, Scalar  yy, Scalar  yz, Scalar dy,
                  Scalar  zx, Scalar  zy, Scalar  zz, Scalar dz) {
      fM[kXX]=xx;  fM[kXY]=xy;  fM[kXZ]=xz;  fM[kDX]=dx;
      fM[kYX]=yx;  fM[kYY]=yy;  fM[kYZ]=yz;  fM[kDY]=dy;
      fM[kZX]=zx;  fM[kZY]=zy;  fM[kZZ]=zz;  fM[kDZ]=dz;
   }

   /**
      Get the components into 12 scalars
   */
   void
   GetComponents (Scalar &xx, Scalar &xy, Scalar &xz, Scalar &dx,
                  Scalar &yx, Scalar &yy, Scalar &yz, Scalar &dy,
                  Scalar &zx, Scalar &zy, Scalar &zz, Scalar &dz) const {
      xx=fM[kXX];  xy=fM[kXY];  xz=fM[kXZ];  dx=fM[kDX];
      yx=fM[kYX];  yy=fM[kYY];  yz=fM[kYZ];  dy=fM[kDY];
      zx=fM[kZX];  zy=fM[kZY];  zz=fM[kZZ];  dz=fM[kDZ];
   }


   /**
      Get the rotation and translation vector representing the 3D transformation
      in any rotation and any vector (the Translation class could also be used)
   */
   template<class AnyRotation, class V>
   void GetDecomposition(AnyRotation &r, V &v) const {
      GetRotation(r);
      GetTranslation(v);
   }


   /**
      Get the rotation and translation vector representing the 3D transformation
   */
   void GetDecomposition(Rotation3D &r, Vector &v) const {
      GetRotation(r);
      GetTranslation(v);
   }

   /**
      Get the 3D rotation representing the 3D transformation
   */
   Rotation3D Rotation() const {
      return Rotation3D( fM[kXX], fM[kXY], fM[kXZ],
                         fM[kYX], fM[kYY], fM[kYZ],
                         fM[kZX], fM[kZY], fM[kZZ] );
   }

   /**
      Get the rotation representing the 3D transformation
   */
   template <class AnyRotation>
   AnyRotation Rotation() const {
      return AnyRotation(Rotation3D(fM[kXX], fM[kXY], fM[kXZ],
                                    fM[kYX], fM[kYY], fM[kYZ],
                                    fM[kZX], fM[kZY], fM[kZZ] ) );
   }

   /**
      Get the  rotation (any type) representing the 3D transformation
   */
   template <class AnyRotation>
   void GetRotation(AnyRotation &r) const {
      r = Rotation();
   }

   /**
      Get the translation representing the 3D transformation in a Cartesian vector
   */
   Translation3D<FTYPE> Translation() const {
      return Translation3D<FTYPE>( fM[kDX], fM[kDY], fM[kDZ] );
   }

   /**
      Get the translation representing the 3D transformation in any vector
      which implements the SetXYZ method
   */
   template <class AnyVector>
   void GetTranslation(AnyVector &v) const {
      v.SetXYZ(fM[kDX], fM[kDY], fM[kDZ]);
   }



   // operations on points and vectors

   /**
      Transformation operation for Position Vector in Cartesian coordinate
      For a Position Vector first a rotation and then a translation is applied
   */
   Point operator() (const Point & p) const {
      return Point ( fM[kXX]*p.X() + fM[kXY]*p.Y() + fM[kXZ]*p.Z() + fM[kDX],
                     fM[kYX]*p.X() + fM[kYY]*p.Y() + fM[kYZ]*p.Z() + fM[kDY],
                     fM[kZX]*p.X() + fM[kZY]*p.Y() + fM[kZZ]*p.Z() + fM[kDZ] );
   }


   /**
      Transformation operation for Displacement Vectors in Cartesian coordinate
      For the Displacement Vectors only the rotation applies - no translations
   */
   Vector operator() (const Vector & v) const {
      return Vector( fM[kXX]*v.X() + fM[kXY]*v.Y() + fM[kXZ]*v.Z() ,
                     fM[kYX]*v.X() + fM[kYY]*v.Y() + fM[kYZ]*v.Z() ,
                     fM[kZX]*v.X() + fM[kZY]*v.Y() + fM[kZZ]*v.Z()  );
   }


   /**
      Transformation operation for Position Vector in any coordinate system
   */
   template<class CoordSystem >
   PositionVector3D<CoordSystem> operator() (const PositionVector3D <CoordSystem> & p) const {
      Point xyzNew = operator() ( Point(p) );
      return  PositionVector3D<CoordSystem> (xyzNew);
   }

   /**
      Transformation operation for Displacement Vector in any coordinate system
   */
   template<class CoordSystem >
   DisplacementVector3D<CoordSystem> operator() (const DisplacementVector3D <CoordSystem> & v) const {
      auto xyzNew = operator() ( Vector(v) );
      return  DisplacementVector3D<CoordSystem> (xyzNew);
   }

   /**
      Transformation operation for points between different coordinate system tags
   */
   template<class CoordSystem, class Tag1, class Tag2 >
   void Transform (const PositionVector3D <CoordSystem,Tag1> & p1, PositionVector3D <CoordSystem,Tag2> & p2  ) const {
      Point xyzNew = operator() ( Point(p1.X(), p1.Y(), p1.Z()) );
      p2.SetXYZ( xyzNew.X(), xyzNew.Y(), xyzNew.Z() );
   }


   /**
      Transformation operation for Displacement Vector of different coordinate systems
   */
   template<class CoordSystem,  class Tag1, class Tag2 >
   void Transform (const DisplacementVector3D <CoordSystem,Tag1> & v1, DisplacementVector3D <CoordSystem,Tag2> & v2  ) const {
      auto xyzNew = operator() ( Vector(v1.X(), v1.Y(), v1.Z() ) );
      v2.SetXYZ( xyzNew.X(), xyzNew.Y(), xyzNew.Z() );
   }

   /**
      Transformation operation for a Lorentz Vector in any  coordinate system
   */
   template <class CoordSystem >
   LorentzVector<CoordSystem> operator() (const LorentzVector<CoordSystem> & q) const {
      Vector xyzNew = operator() ( Vector(q.Vect() ) );
      return  LorentzVector<CoordSystem> (xyzNew.X(), xyzNew.Y(), xyzNew.Z(), q.E() );
   }

   /**
      Transformation on a 3D plane
   */
   Plane3D<FTYPE> operator() (const Plane3D<FTYPE> & plane) const
   {
     // transformations on a 3D plane
     auto n = plane.Normal();
     // take a point on the plane. Use origin projection on the plane
     // ( -ad, -bd, -cd) if (a**2 + b**2 + c**2 ) = 1
     auto d = plane.HesseDistance();
     Point p( - d * n.X() , - d *n.Y(), -d *n.Z() );
     return Plane3D<FTYPE> ( operator() (n), operator() (p) );
   }


   // skip transformation for arbitrary vectors - not really defined if point or displacement vectors

   // same but with operator *
   /**
      Transformation operation for Vectors. Apply same rules as operator()
      depending on type of vector.
      Will work only for DisplacementVector3D, PositionVector3D and LorentzVector
   */
   template<class AVector >
   AVector operator * (const AVector & v) const {
      return operator() (v);
   }



   /**
      multiply (combine) with another transformation in place
   */
   inline Transform3D & operator *= (const Transform3D  & t);

   /**
      multiply (combine) two transformations
   */
   inline Transform3D operator * (const Transform3D  & t) const;

   /**
       Invert the transformation in place
   */
   void Invert()
   {
     //
     // Name: Transform3D::inverse                     Date:    24.09.96
     // Author: E.Chernyaev (IHEP/Protvino)            Revised:
     //
     // Function: Find inverse affine transformation.
     
     Scalar detxx = fM[kYY]*fM[kZZ] - fM[kYZ]*fM[kZY];
     Scalar detxy = fM[kYX]*fM[kZZ] - fM[kYZ]*fM[kZX];
     Scalar detxz = fM[kYX]*fM[kZY] - fM[kYY]*fM[kZX];
     Scalar det   = fM[kXX]*detxx - fM[kXY]*detxy + fM[kXZ]*detxz;
     if (det == 0) {
       std::cerr << "Transform3D::inverse error: zero determinant" << std::endl;
       return;
     }
     det = 1./det; detxx *= det; detxy *= det; detxz *= det;
     Scalar detyx = (fM[kXY]*fM[kZZ] - fM[kXZ]*fM[kZY] )*det;
     Scalar detyy = (fM[kXX]*fM[kZZ] - fM[kXZ]*fM[kZX] )*det;
     Scalar detyz = (fM[kXX]*fM[kZY] - fM[kXY]*fM[kZX] )*det;
     Scalar detzx = (fM[kXY]*fM[kYZ] - fM[kXZ]*fM[kYY] )*det;
     Scalar detzy = (fM[kXX]*fM[kYZ] - fM[kXZ]*fM[kYX] )*det;
     Scalar detzz = (fM[kXX]*fM[kYY] - fM[kXY]*fM[kYX] )*det;
     SetComponents
       (detxx, -detyx,  detzx, -detxx*fM[kDX]+detyx*fM[kDY]-detzx*fM[kDZ],
        -detxy,  detyy, -detzy,  detxy*fM[kDX]-detyy*fM[kDY]+detzy*fM[kDZ],
        detxz, -detyz,  detzz, -detxz*fM[kDX]+detyz*fM[kDY]-detzz*fM[kDZ]);
   }
   

   /**
      Return the inverse of the transformation.
   */
   Transform3D Inverse() const {
      Transform3D t(*this);
      t.Invert();
      return t;
   }


   /**
      Equality operator. Check equality for each element
      To do: use Scalar tolerance
   */
   bool operator == (const Transform3D & rhs) const {
      if( fM[0] != rhs.fM[0] )  return false;
      if( fM[1] != rhs.fM[1] )  return false;
      if( fM[2] != rhs.fM[2] )  return false;
      if( fM[3] != rhs.fM[3] )  return false;
      if( fM[4] != rhs.fM[4] )  return false;
      if( fM[5] != rhs.fM[5] )  return false;
      if( fM[6] != rhs.fM[6] )  return false;
      if( fM[7] != rhs.fM[7] )  return false;
      if( fM[8] != rhs.fM[8] )  return false;
      if( fM[9] != rhs.fM[9] )  return false;
      if( fM[10]!= rhs.fM[10] ) return false;
      if( fM[11]!= rhs.fM[11] ) return false;
      return true;
   }

   /**
      Inequality operator. Check equality for each element
      To do: use Scalar tolerance
   */
   bool operator != (const Transform3D & rhs) const {
      return ! operator==(rhs);
   }


protected:

   /**
      make transformation from first a rotation then a translation
   */
   void  AssignFrom( const Rotation3D & r, const Vector & v)
   {
     // assignment  from rotation + translation
     
     Scalar rotData[9];
     r.GetComponents(rotData, rotData +9);
     // first raw
     for (int i = 0; i < 3; ++i)
       fM[i] = rotData[i];
     // second raw
     for (int i = 0; i < 3; ++i)
       fM[kYX+i] = rotData[3+i];
     // third raw
     for (int i = 0; i < 3; ++i)
       fM[kZX+i] = rotData[6+i];
     
     // translation data
     Scalar vecData[3];
     v.GetCoordinates(vecData, vecData+3);
     fM[kDX] = vecData[0];
     fM[kDY] = vecData[1];
     fM[kDZ] = vecData[2];
   }

   /**
      make transformation from only rotations (zero translation)
   */
   void  AssignFrom( const Rotation3D & r)
   {
     // assign from only a rotation  (null translation)
     Scalar rotData[9];
     r.GetComponents(rotData, rotData +9);
     for (int i = 0; i < 3; ++i) {
       for (int j = 0; j < 3; ++j)
         fM[4*i + j] = rotData[3*i+j];
       // empty vector data
       fM[4*i + 3] = 0;
     }
   }

   /**
      make transformation from only translation (identity rotations)
   */
   void  AssignFrom( const Vector & v)
   {
     // assign from a translation only (identity rotations)
     fM[kXX] = 1.0;  fM[kXY] = 0.0; fM[kXZ] = 0.0; fM[kDX] = v.X();
     fM[kYX] = 0.0;  fM[kYY] = 1.0; fM[kYZ] = 0.0; fM[kDY] = v.Y();
     fM[kZX] = 0.0;  fM[kZY] = 0.0; fM[kZZ] = 1.0; fM[kDZ] = v.Z();
   }
   
   /**
      Set identity transformation (identity rotation , zero translation)
   */
   void SetIdentity()
   {
     //set identity ( identity rotation and zero translation)
     fM[kXX] = 1.0;  fM[kXY] = 0.0; fM[kXZ] = 0.0; fM[kDX] = 0.0;
     fM[kYX] = 0.0;  fM[kYY] = 1.0; fM[kYZ] = 0.0; fM[kDY] = 0.0;
     fM[kZX] = 0.0;  fM[kZY] = 0.0; fM[kZZ] = 1.0; fM[kDZ] = 0.0;
   }
   
private:


   Scalar fM[12];    // transformation elements (3x4 matrix)

};




// inline functions (combination of transformations)
template < typename FTYPE >
inline Transform3D<FTYPE> & Transform3D<FTYPE>::operator *= (const Transform3D<FTYPE>  & t)
{
   // combination of transformations

   SetComponents(fM[kXX]*t.fM[kXX]+fM[kXY]*t.fM[kYX]+fM[kXZ]*t.fM[kZX],
                 fM[kXX]*t.fM[kXY]+fM[kXY]*t.fM[kYY]+fM[kXZ]*t.fM[kZY],
                 fM[kXX]*t.fM[kXZ]+fM[kXY]*t.fM[kYZ]+fM[kXZ]*t.fM[kZZ],
                 fM[kXX]*t.fM[kDX]+fM[kXY]*t.fM[kDY]+fM[kXZ]*t.fM[kDZ]+fM[kDX],

                 fM[kYX]*t.fM[kXX]+fM[kYY]*t.fM[kYX]+fM[kYZ]*t.fM[kZX],
                 fM[kYX]*t.fM[kXY]+fM[kYY]*t.fM[kYY]+fM[kYZ]*t.fM[kZY],
                 fM[kYX]*t.fM[kXZ]+fM[kYY]*t.fM[kYZ]+fM[kYZ]*t.fM[kZZ],
                 fM[kYX]*t.fM[kDX]+fM[kYY]*t.fM[kDY]+fM[kYZ]*t.fM[kDZ]+fM[kDY],

                 fM[kZX]*t.fM[kXX]+fM[kZY]*t.fM[kYX]+fM[kZZ]*t.fM[kZX],
                 fM[kZX]*t.fM[kXY]+fM[kZY]*t.fM[kYY]+fM[kZZ]*t.fM[kZY],
                 fM[kZX]*t.fM[kXZ]+fM[kZY]*t.fM[kYZ]+fM[kZZ]*t.fM[kZZ],
                 fM[kZX]*t.fM[kDX]+fM[kZY]*t.fM[kDY]+fM[kZZ]*t.fM[kDZ]+fM[kDZ]);

   return *this;
}


template < typename FTYPE >
inline Transform3D<FTYPE> Transform3D<FTYPE>::operator * (const Transform3D<FTYPE>  & t) const
{
   // combination of transformations

   return Transform3D<FTYPE>(fM[kXX]*t.fM[kXX]+fM[kXY]*t.fM[kYX]+fM[kXZ]*t.fM[kZX],
                      fM[kXX]*t.fM[kXY]+fM[kXY]*t.fM[kYY]+fM[kXZ]*t.fM[kZY],
                      fM[kXX]*t.fM[kXZ]+fM[kXY]*t.fM[kYZ]+fM[kXZ]*t.fM[kZZ],
                      fM[kXX]*t.fM[kDX]+fM[kXY]*t.fM[kDY]+fM[kXZ]*t.fM[kDZ]+fM[kDX],

                      fM[kYX]*t.fM[kXX]+fM[kYY]*t.fM[kYX]+fM[kYZ]*t.fM[kZX],
                      fM[kYX]*t.fM[kXY]+fM[kYY]*t.fM[kYY]+fM[kYZ]*t.fM[kZY],
                      fM[kYX]*t.fM[kXZ]+fM[kYY]*t.fM[kYZ]+fM[kYZ]*t.fM[kZZ],
                      fM[kYX]*t.fM[kDX]+fM[kYY]*t.fM[kDY]+fM[kYZ]*t.fM[kDZ]+fM[kDY],

                      fM[kZX]*t.fM[kXX]+fM[kZY]*t.fM[kYX]+fM[kZZ]*t.fM[kZX],
                      fM[kZX]*t.fM[kXY]+fM[kZY]*t.fM[kYY]+fM[kZZ]*t.fM[kZY],
                      fM[kZX]*t.fM[kXZ]+fM[kZY]*t.fM[kYZ]+fM[kZZ]*t.fM[kZZ],
                      fM[kZX]*t.fM[kDX]+fM[kZY]*t.fM[kDY]+fM[kZZ]*t.fM[kDZ]+fM[kDZ]  );

}




//--- global functions resulting in Transform3D -------


// ------ combination of a  translation (first)  and a rotation ------


/**
   combine a translation and a rotation to give a transform3d
   First the translation then the rotation
 */
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Rotation3D & r, const Translation3D<FTYPE> & t) {
   return Transform3D<FTYPE>( r, r(t.Vect()) );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const RotationX & r, const Translation3D<FTYPE> & t) {
   Rotation3D r3(r);
   return Transform3D<FTYPE>( r3, r3(t.Vect()) );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const RotationY & r, const Translation3D<FTYPE> & t) {
   Rotation3D r3(r);
   return Transform3D<FTYPE>( r3, r3(t.Vect()) );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const RotationZ & r, const Translation3D<FTYPE> & t) {
   Rotation3D r3(r);
   return Transform3D<FTYPE>( r3, r3(t.Vect()) );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const RotationZYX & r, const Translation3D<FTYPE> & t) {
   Rotation3D r3(r);
   return Transform3D<FTYPE>( r3, r3(t.Vect()) );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const AxisAngle & r, const Translation3D<FTYPE> & t) {
   Rotation3D r3(r);
   return Transform3D<FTYPE>( r3, r3(t.Vect()) );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const EulerAngles & r, const Translation3D<FTYPE> & t) {
   Rotation3D r3(r);
   return Transform3D<FTYPE>( r3, r3(t.Vect()) );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Quaternion & r, const Translation3D<FTYPE> & t) {
   Rotation3D r3(r);
   return Transform3D<FTYPE>( r3, r3(t.Vect()) );
}

// ------ combination of a  rotation (first)  and then a translation ------

/**
   combine a rotation and a translation to give a transform3d
   First a rotation then the translation
 */
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Translation3D<FTYPE> & t, const Rotation3D & r) {
   return Transform3D<FTYPE>( r, t.Vect());
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Translation3D<FTYPE> & t, const RotationX & r) {
   return Transform3D<FTYPE>( Rotation3D(r) , t.Vect());
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Translation3D<FTYPE> & t, const RotationY & r) {
   return Transform3D<FTYPE>( Rotation3D(r) , t.Vect());
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Translation3D<FTYPE> & t, const RotationZ & r) {
   return Transform3D<FTYPE>( Rotation3D(r) , t.Vect());
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Translation3D<FTYPE> & t, const RotationZYX & r) {
   return Transform3D<FTYPE>( Rotation3D(r) , t.Vect());
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Translation3D<FTYPE> & t, const EulerAngles & r) {
   return Transform3D<FTYPE>( Rotation3D(r) , t.Vect());
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Translation3D<FTYPE> & t, const Quaternion & r) {
   return Transform3D<FTYPE>( Rotation3D(r) , t.Vect());
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Translation3D<FTYPE> & t, const AxisAngle & r) {
   return Transform3D<FTYPE>( Rotation3D(r) , t.Vect());
}

// ------ combination of a Transform3D and a pure translation------

/**
   combine a transformation and a translation to give a transform3d
   First the translation then the transform3D
 */
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Transform3D<FTYPE> & t, const Translation3D<FTYPE> & d) {
   Rotation3D r = t.Rotation();
   return Transform3D<FTYPE>( r, r( d.Vect() ) + t.Translation().Vect()  );
}

/**
   combine a translation and a transformation to give a transform3d
   First the transformation then the translation
 */
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Translation3D<FTYPE> & d, const Transform3D<FTYPE> & t) {
   return Transform3D<FTYPE>( t.Rotation(), t.Translation().Vect() + d.Vect());
}

// ------ combination of a Transform3D and any rotation------


/**
   combine a transformation and a rotation to give a transform3d
   First the rotation then the transform3D
 */
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Transform3D<FTYPE> & t, const Rotation3D & r) {
   return Transform3D<FTYPE>( t.Rotation()*r ,  t.Translation()  );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Transform3D<FTYPE> & t, const RotationX & r) {
   return Transform3D<FTYPE>( t.Rotation()*r ,  t.Translation()  );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Transform3D<FTYPE> & t, const RotationY & r) {
   return Transform3D<FTYPE>( t.Rotation()*r ,  t.Translation()  );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Transform3D<FTYPE> & t, const RotationZ & r) {
   return Transform3D<FTYPE>( t.Rotation()*r ,  t.Translation()  );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Transform3D<FTYPE> & t, const RotationZYX & r) {
   return Transform3D<FTYPE>( t.Rotation()*r ,  t.Translation()  );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Transform3D<FTYPE> & t, const EulerAngles & r) {
   return Transform3D<FTYPE>( t.Rotation()*r ,  t.Translation()  );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Transform3D<FTYPE> & t, const AxisAngle & r) {
   return Transform3D<FTYPE>( t.Rotation()*r ,  t.Translation()  );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Transform3D<FTYPE> & t, const Quaternion & r) {
   return Transform3D<FTYPE>( t.Rotation()*r ,  t.Translation()  );
}



/**
   combine a rotation and a transformation to give a transform3d
   First the transformation then the rotation
 */
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Rotation3D & r, const Transform3D<FTYPE> & t) {
   return Transform3D<FTYPE>( r * t.Rotation(), r * t.Translation().Vect() );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const RotationX & r, const Transform3D<FTYPE> & t) {
   Rotation3D r3d(r);
   return Transform3D<FTYPE>( r3d * t.Rotation(), r3d * t.Translation().Vect() );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const RotationY & r, const Transform3D<FTYPE> & t) {
   Rotation3D r3d(r);
   return Transform3D<FTYPE>( r3d * t.Rotation(), r3d * t.Translation().Vect() );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const RotationZ & r, const Transform3D<FTYPE> & t) {
   Rotation3D r3d(r);
   return Transform3D<FTYPE>( r3d * t.Rotation(), r3d * t.Translation().Vect() );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const RotationZYX & r, const Transform3D<FTYPE> & t) {
   Rotation3D r3d(r);
   return Transform3D<FTYPE>( r3d * t.Rotation(), r3d * t.Translation().Vect() );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const EulerAngles & r, const Transform3D<FTYPE> & t) {
   Rotation3D r3d(r);
   return Transform3D<FTYPE>( r3d * t.Rotation(), r3d * t.Translation().Vect() );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const AxisAngle & r, const Transform3D<FTYPE> & t) {
   Rotation3D r3d(r);
   return Transform3D<FTYPE>( r3d * t.Rotation(), r3d * t.Translation().Vect() );
}
template < typename FTYPE >
inline Transform3D<FTYPE> operator * (const Quaternion & r, const Transform3D<FTYPE> & t) {
   Rotation3D r3d(r);
   return Transform3D<FTYPE>( r3d * t.Rotation(), r3d * t.Translation().Vect() );
}


//---I/O functions
// TODO - I/O should be put in the manipulator form

/**
   print the 12 components of the Transform3D
 */
  template < typename FTYPE >
  std::ostream & operator<< (std::ostream & os, const Transform3D<FTYPE> & t)
  {
    // TODO - this will need changing for machine-readable issues
    //        and even the human readable form needs formatting improvements
    
    typename Transform3D<FTYPE>::Scalar m[12];
    t.GetComponents(m, m+12);
    os << "\n" << m[0] << "  " << m[1] << "  " << m[2] << "  " << m[3] ;
    os << "\n" << m[4] << "  " << m[5] << "  " << m[6] << "  " << m[7] ;
    os << "\n" << m[8] << "  " << m[9] << "  " << m[10]<< "  " << m[11] << "\n";
    return os;
  }

   } // end namespace Math

} // end namespace LHCbROOT


#endif /* LHCbROOT_Math_GenVector_Transform3D */
