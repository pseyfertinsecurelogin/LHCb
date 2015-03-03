// $Id: 3DTransformationFunctions.cpp,v 1.6 2009-11-13 10:07:31 wouter Exp $
// Include files 



// local
#include "DetDesc/3DTransformationFunctions.h"

namespace DetDesc {

const Gaudi::Transform3D localToGlobalTransformation(const std::vector<double>& translationParams,
                                                     const std::vector<double>& rotationParams,
                                                     const std::vector<double>& pivotParams) 
{
  const ROOT::Math::Translation3D pivot = ROOT::Math::Translation3D( pivotParams.begin(),
                                                                     pivotParams.end()   );

  const ROOT::Math::RotationZYX rotation = ROOT::Math::RotationZYX( rotationParams[2],
                                                                    rotationParams[1],
                                                                    rotationParams[0]  );
  const ROOT::Math::Translation3D translation = 
    ROOT::Math::Translation3D( translationParams.begin(),
                               translationParams.end()    );

  return Gaudi::Transform3D( translation*pivot*rotation*(pivot.Inverse()) );
  
}

void getZYXTransformParameters(const Gaudi::Transform3D& CDM,
                               std::vector<double>& translationParams,
                               std::vector<double>& rotationParams,
                               const std::vector<double>& pivotParams) 
{

  ROOT::Math::RotationZYX newRot = CDM.Rotation<ROOT::Math::RotationZYX>();

  const ROOT::Math::Translation3D newTrans = CDM.Translation();

  const ROOT::Math::Translation3D pivotTrans = 
    ROOT::Math::Translation3D( pivotParams.begin(), pivotParams.end()    );
  

  const Gaudi::Transform3D newRotPart = 
    pivotTrans.Inverse() * newRot * pivotTrans;

  newRot = newRotPart.Rotation<ROOT::Math::RotationZYX>();

  newRot.GetComponents(rotationParams[2], rotationParams[1], rotationParams[0]);

  newTrans.GetComponents( translationParams.begin(), translationParams.end() );
  
}

ROOT::Math::SMatrix<double,6,6> getTransformJacobian( const Gaudi::Transform3D& transform ) 
{

  // Suppose f( x ) is the transform corresponding to parameters x
  // (see routine AlParameters::transform).  This routine returns the
  // jacobian for a transformation of f: if x' is the delta in frame 0
  // and x is the delta in frame 1 and A is the transform from frame 0
  // to frame 1, then this returns the matrix Mij = dx_i / dx'_j, by taking the
  // derivative of
  //   f'(x')   =    A * f(x) * A^{-1}
  //

  typedef ROOT::Math::SMatrix<double, 3, 3> Matrix3x3 ;
  typedef ROOT::Math::SMatrix<double, 6, 6> Matrix6x6 ;
  typedef ROOT::Math::SVector<double, 3>    Vector3;

  // first decompose the transform. cannot do this quicker because of old ROOT version.
  Matrix3x3 R ;
  Vector3 T ;
  transform.GetComponents(R(0,0),R(0,1),R(0,2),T(0),
                          R(1,0),R(1,1),R(1,2),T(1),
                          R(2,0),R(2,1),R(2,2),T(2)) ;

  // We also need the inverse of R, which is of course just its transpose:
  Matrix3x3 Rinv = ROOT::Math::Transpose(R) ;
  //invR.Invert() ;
  
  // const matrices for derivative of rotations to rotation pars
  Matrix3x3 drot[3] ;
  drot[0](2,1) = drot[1](0,2) = drot[2](1,0) = 1 ;
  drot[0](1,2) = drot[1](2,0) = drot[2](0,1) = -1 ;
  
  // R ( deps ) R^{-1} for each three components
  Matrix3x3 RdrotR[3] ;
  for(int i=0; i<3; ++i) RdrotR[i] = R * (drot[i] * Rinv) ;
  
  // now construct all components of the jacobian
  Matrix6x6 jacobian ;
  
  // the derivative of the rotation to the translation (deps/deta') is zero.

  // the derivative of the translation to the translation (deta/deta')
  jacobian.Place_at( R, 0, 0) ;
  
  // the derivatives to the rotation are a bit more complicated. let's
  // first do the rotation to the rotation:
  for(int i=0; i<3; ++i) {
    jacobian(3,3+i) = RdrotR[i](2,1) ;
    jacobian(4,3+i) = RdrotR[i](0,2) ;
    jacobian(5,3+i) = RdrotR[i](1,0) ;
  }

  // SO, if you just think about it for a minute, you realize that
  // also the delta-rotation should transform with the rotation:
  // //jacobian.Place_at( R, 3, 3) ;

  // the only thing left is the derivative of the translation to the
  // rotation. it's the only part that involves T:
  for(int i=0; i<3; ++i) {
    Vector3 tmp = RdrotR[i] * T ;
    for(int j=0; j<3; ++j) jacobian(j,i+3) = - tmp(j) ;
  }
  
  // Het staat er nu zo kort, dat je geen idee meer hebt hoeveel werk
  // de algebra was ...
  return jacobian ;
}



  
} // namespace DetDesc
//=============================================================================

