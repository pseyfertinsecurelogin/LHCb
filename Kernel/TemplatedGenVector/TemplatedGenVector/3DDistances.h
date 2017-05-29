// @(#)root/mathcore:$Id$
// Authors: W. Brown, M. Fischler, L. Moneta    2005

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2005, LCG LHCbROOT FNAL MathLib Team                    *
  *                                                                    *
  *                                                                    *
  **********************************************************************/

// Header file for class implementatoins of DIstance() functions among
// Rotations in 3 dimensions
//
// Created by: Mark Fischler and Walter Brown Thurs July 7, 2005
//
// Last update: $Id$
//
#ifndef LHCbROOT_Math_GenVector_3DDistances
#define LHCbROOT_Math_GenVector_3DDistances  1

#include "TemplatedGenVector/AxisAnglefwd.h"
#include "TemplatedGenVector/EulerAnglesfwd.h"
#include "TemplatedGenVector/Quaternionfwd.h"
#include "TemplatedGenVector/Rotation3Dfwd.h"
#include "TemplatedGenVector/RotationXfwd.h"
#include "TemplatedGenVector/RotationYfwd.h"
#include "TemplatedGenVector/RotationZYXfwd.h"
#include "TemplatedGenVector/RotationZfwd.h"

namespace LHCbROOT {
namespace Math {
class AxisAngle;
class EulerAngles;
class Quaternion;
class Rotation3D;
class RotationX;
class RotationY;
class RotationZ;
class RotationZYX;
}  // namespace Math
}  // namespace LHCbROOT

namespace LHCbROOT {
namespace Math {
namespace gv_detail {

// Technical note:
//
// Most of these functions are identical, converting to Quaternions
// and then taking the simple distance between two Quaternions.  This
// makes it tempting to use a template function (which could appear in
// each individual rotation header).
//
// However, then we would have to include Quaternion.h in this header,
// and by induction, each rotation class would depend on Quaternion, which
// would be undesirable.


// ----------------------------------------------------------------------
// Distance from Rotation3D

double dist( Rotation3D const & r1, Rotation3D  const & r2);
double dist( Rotation3D const & r1, AxisAngle   const & r2);
double dist( Rotation3D const & r1, EulerAngles const & r2);
double dist( Rotation3D const & r1, Quaternion  const & r2);
double dist( Rotation3D const & r1, RotationZYX const & r2);
double dist( Rotation3D const & r1, RotationX   const & r2);
double dist( Rotation3D const & r1, RotationY   const & r2);
double dist( Rotation3D const & r1, RotationZ   const & r2);


// ----------------------------------------------------------------------
// Distance from AxisAngle

double dist( AxisAngle const & r1, Rotation3D  const & r2);
double dist( AxisAngle const & r1, AxisAngle   const & r2);
double dist( AxisAngle const & r1, EulerAngles const & r2);
double dist( AxisAngle const & r1, Quaternion  const & r2);
double dist( AxisAngle const & r1, RotationZYX const & r2);
double dist( AxisAngle const & r1, RotationX   const & r2);
double dist( AxisAngle const & r1, RotationY   const & r2);
double dist( AxisAngle const & r1, RotationZ   const & r2);


// ----------------------------------------------------------------------
// Distance from EulerAngles

double dist( EulerAngles const & r1, Rotation3D  const & r2);
double dist( EulerAngles const & r1, AxisAngle   const & r2);
double dist( EulerAngles const & r1, EulerAngles const & r2);
double dist( EulerAngles const & r1, Quaternion  const & r2);
double dist( EulerAngles const & r1, RotationZYX const & r2);
double dist( EulerAngles const & r1, RotationX   const & r2);
double dist( EulerAngles const & r1, RotationY   const & r2);
double dist( EulerAngles const & r1, RotationZ   const & r2);


// ----------------------------------------------------------------------
// Distance from Quaternion

double dist( Quaternion const & r1, Rotation3D  const & r2);
double dist( Quaternion const & r1, AxisAngle   const & r2);
double dist( Quaternion const & r1, EulerAngles const & r2);
double dist( Quaternion const & r1, Quaternion  const & r2);
double dist( Quaternion const & r1, RotationZYX const & r2);
double dist( Quaternion const & r1, RotationX   const & r2);
double dist( Quaternion const & r1, RotationY   const & r2);
double dist( Quaternion const & r1, RotationZ   const & r2);

// ----------------------------------------------------------------------
// Distance from RotationZYX

double dist( RotationZYX const & r1, Rotation3D  const & r2);
double dist( RotationZYX const & r1, AxisAngle   const & r2);
double dist( RotationZYX const & r1, EulerAngles const & r2);
double dist( RotationZYX const & r1, Quaternion  const & r2);
double dist( RotationZYX const & r1, RotationZYX const & r2);
double dist( RotationZYX const & r1, RotationX   const & r2);
double dist( RotationZYX const & r1, RotationY   const & r2);
double dist( RotationZYX const & r1, RotationZ   const & r2);



// ----------------------------------------------------------------------
// Distance from RotationX

double dist( RotationX const & r1, Rotation3D  const & r2);
double dist( RotationX const & r1, AxisAngle   const & r2);
double dist( RotationX const & r1, EulerAngles const & r2);
double dist( RotationX const & r1, Quaternion  const & r2);
double dist( RotationX const & r1, RotationX   const & r2);
double dist( RotationX const & r1, RotationY   const & r2);
double dist( RotationX const & r1, RotationZ   const & r2);


// ----------------------------------------------------------------------
// Distance from RotationY

double dist( RotationY const & r1, Rotation3D  const & r2);
double dist( RotationY const & r1, AxisAngle   const & r2);
double dist( RotationY const & r1, EulerAngles const & r2);
double dist( RotationY const & r1, Quaternion  const & r2);
double dist( RotationY const & r1, RotationX   const & r2);
double dist( RotationY const & r1, RotationY   const & r2);
double dist( RotationY const & r1, RotationZ   const & r2);


// ----------------------------------------------------------------------
// Distance from RotationZ

double dist( RotationZ const  & r1, Rotation3D  const & r2);
double dist( RotationZ const  & r1, AxisAngle   const & r2);
double dist( RotationZ const  & r1, EulerAngles const & r2);
double dist( RotationZ const  & r1, Quaternion  const & r2);
double dist( RotationZ const  & r1, RotationX   const & r2);
double dist( RotationZ const  & r1, RotationY   const & r2);
double dist( RotationZ const  & r1, RotationZ   const & r2);


} //namespace gv_detail
} //namespace Math
} //namespace LHCbROOT

#endif // LHCbROOT_Math_GenVector_3DDistances
