// @(#)root/mathcore:$Id$
// Authors: W. Brown, M. Fischler, L. Moneta    2005

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2005 , LCG LHCbROOT MathLib Team                         *
 *                                                                    *
 *                                                                    *
 **********************************************************************/

// Header file for class Translation3D
//
// Created by: Lorenzo Moneta  October 21 2005
//
//
#ifndef LHCbROOT_Math_GenVector_Translation3D
#define LHCbROOT_Math_GenVector_Translation3D  1


#ifndef LHCbROOT_Math_GenVector_DisplacementVector3D
#include "Math/TemplatedGenVector/DisplacementVector3D.h"
#endif

#ifndef LHCbROOT_Math_GenVector_PositionVector3Dfwd
#include "Math/TemplatedGenVector/PositionVector3Dfwd.h"
#endif

#ifndef LHCbROOT_Math_GenVector_LorentzVectorfwd
#include "Math/TemplatedGenVector/LorentzVectorfwd.h"
#endif

#include <iostream>



namespace LHCbROOT {

namespace Math {


  //template < typename FTYPE >
  //class Plane3D;


//____________________________________________________________________________________________________
/**
    Class describing a 3 dimensional translation. It can be combined (using the operator *)
    with the LHCbROOT::Math::Rotation3D  classes and LHCbROOT::Math::Transform3D to obtained combined
    transformations and to operate on points and vectors.
    Note that a the translation applied to a Vector object (DisplacementVector3D and LorentzVector classes)
    performes a noop, i.e. it returns the same vector. A translation can be applied only to the Point objects
    (PositionVector3D classes).

    @ingroup GenVector

*/

  template < typename FTYPE >
  class Translation3D {

  private:

    //typedef PositionVector3D<Cartesian3D<FTYPE>, DefaultCoordinateSystemTag > XYZPoint;

  public:
    
    typedef FTYPE Scalar;

    typedef DisplacementVector3D<Cartesian3D<FTYPE>, DefaultCoordinateSystemTag >  Vector;


   /**
       Default constructor ( zero translation )
   */
    Translation3D() = default;

   /**
      Construct given a pair of pointers or iterators defining the
      beginning and end of an array of 3 Scalars representing the z,y,z of the translation vector
   */
   template<class IT>
   Translation3D(IT begin, IT end)
   {
      fVect.SetCoordinates(begin,end);
   }

   /**
      Construct from x,y,z values representing the translation
   */
   Translation3D(FTYPE dx, FTYPE dy, FTYPE dz) :
      fVect( Vector(dx, dy, dz) )
   {  }


   /**
      Construct from any Displacement vector in ant tag and coordinate system
   */
   template<class CoordSystem, class Tag>
   explicit Translation3D( const DisplacementVector3D<CoordSystem,Tag> & v) :
      fVect(Vector(v.X(),v.Y(),v.Z()))
   { }


   /**
      Construct transformation from one coordinate system defined one point (the origin)
       to a new coordinate system defined by other point (origin )
      @param p1  point defining origin of original reference system
      @param p2  point defining origin of transformed reference system

   */
   template<class CoordSystem, class Tag>
   Translation3D (const  PositionVector3D<CoordSystem,Tag> & p1, const PositionVector3D<CoordSystem,Tag> & p2 ) :
      fVect(p2-p1)
   { }


   // use compiler generated copy ctor, copy assignmet and dtor


   // ======== Components ==============

   /**
       return a const reference to the underline vector representing the translation
   */
   const Vector & Vect() const { return fVect; }

   /**
      Set the 3  components given an iterator to the start of
      the desired data, and another to the end (3 past start).
   */
   template<class IT>
   void SetComponents(IT begin, IT end) {
      fVect.SetCoordinates(begin,end);
   }

   /**
      Get the 3  components into data specified by an iterator begin
      and another to the end of the desired data (12 past start).
   */
   template<class IT>
   void GetComponents(IT begin, IT end) const {
      fVect.GetCoordinates(begin,end);
   }

   /**
      Get the 3 matrix components into data specified by an iterator begin
   */
   template<class IT>
   void GetComponents(IT begin) const {
      fVect.GetCoordinates(begin);
   }


   /**
      Set the components from 3 scalars
   */
   void
   SetComponents (FTYPE  dx, FTYPE  dy, FTYPE  dz ) {
      fVect.SetCoordinates(dx,dy,dz);
   }

   /**
      Get the components into 3 scalars
   */
   void
   GetComponents (FTYPE &dx, FTYPE &dy, FTYPE &dz) const {
      fVect.GetCoordinates(dx,dy,dz);
   }


   /**
      Set the XYZ vector components from 3 scalars
   */
   void
   SetXYZ (FTYPE  dx, FTYPE  dy, FTYPE  dz ) {
      fVect.SetXYZ(dx,dy,dz);
   }


   // operations on points and vectors


   /**
      Transformation operation for Position Vector in any coordinate system and default tag
   */
   template<class CoordSystem, class Tag >
   PositionVector3D<CoordSystem,Tag> operator() (const PositionVector3D <CoordSystem,Tag> & p) const {
      PositionVector3D<CoordSystem,Tag>  tmp;
      tmp.SetXYZ (p.X() + fVect.X(),
                  p.Y() + fVect.Y(),
                  p.Z() + fVect.Z() ) ;
      return tmp;
   }

   /**
      Transformation operation for Displacement Vector in any coordinate system and default tag
      For the Displacement Vectors no translation apply so return the vector itself
   */
   template<class CoordSystem, class Tag >
   DisplacementVector3D<CoordSystem,Tag> operator() (const DisplacementVector3D <CoordSystem,Tag> & v) const {
      return  v;
   }

   /**
      Transformation operation for points between different coordinate system tags
   */
   template<class CoordSystem, class Tag1, class Tag2 >
   void Transform (const PositionVector3D <CoordSystem,Tag1> & p1, PositionVector3D <CoordSystem,Tag2> & p2  ) const {
      PositionVector3D <CoordSystem,Tag2> tmp;
      tmp.SetXYZ( p1.X(), p1.Y(), p1.Z() );
      p2 =  operator()(tmp);
    }


   /**
      Transformation operation for Displacement Vector of different coordinate systems
   */
   template<class CoordSystem,  class Tag1, class Tag2 >
   void Transform (const DisplacementVector3D <CoordSystem,Tag1> & v1, DisplacementVector3D <CoordSystem,Tag2> & v2  ) const {
      // just copy v1 in v2
      v2.SetXYZ(v1.X(), v1.Y(), v1.Z() );
   }

   /**
      Transformation operation for a Lorentz Vector in any  coordinate system
      A LorentzVector contains a displacement vector so no translation applies as well
   */
   template <class CoordSystem >
   LorentzVector<CoordSystem> operator() (const LorentzVector<CoordSystem> & q) const {
      return  q;
   }

   /**
      Transformation on a 3D plane
   */
    // Plane3D<FTYPE> operator() (const Plane3D<FTYPE> & plane) const
    // {
    //   // transformations on a 3D plane
    //   auto n = plane.Normal();
    //   // take a point on the plane. Use origin projection on the plane
    //   // ( -ad, -bd, -cd) if (a**2 + b**2 + c**2 ) = 1
    //   auto d = plane.HesseDistance();
    //   XYZPoint p( - d * n.X() , - d *n.Y(), -d *n.Z() );
    //   return Plane3D<FTYPE> ( operator() (n), operator() (p) );
    // }

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
   Translation3D & operator *= (const Translation3D  & t) {
      fVect+= t.Vect();
      return *this;
   }

   /**
      multiply (combine) two transformations
   */
   Translation3D operator * (const Translation3D  & t) const {
      return Translation3D( fVect + t.Vect() );
   }

   /**
       Invert the transformation in place
   */
   void Invert() {
      SetComponents( -fVect.X(), -fVect.Y(),-fVect.Z() );
   }

   /**
      Return the inverse of the transformation.
   */
   Translation3D Inverse() const {
      return Translation3D( -fVect.X(), -fVect.Y(),-fVect.Z() );
   }


   /**
      Equality/inequality operators
   */
   bool operator == (const Translation3D & rhs) const {
      if( fVect != rhs.fVect )  return false;
      return true;
   }

   bool operator != (const Translation3D & rhs) const {
      return ! operator==(rhs);
   }



private:

   Vector fVect;   // internal 3D vector representing the translation

};





// global functions

// TODO - I/O should be put in the manipulator form
  template < typename FTYPE >
  std::ostream & operator<< (std::ostream & os, const Translation3D<FTYPE> & t)
  {
    // TODO - this will need changing for machine-readable issues
    //        and even the human readable form needs formatiing improvements 
    FTYPE m[3];
    t.GetComponents(m, m+3);
    os << "\n" << m[0] << "  " << m[1] << "  " << m[2] << "\n";
    return os;
  }

// need a function Transform = Translation * Rotation ???

   } // end namespace Math

} // end namespace LHCbROOT


#endif /* LHCbROOT_Math_GenVector_Translation3D */
