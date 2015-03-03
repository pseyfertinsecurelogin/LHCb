// $Id: Transform3DTypes.h,v 1.7 2006-08-01 08:52:35 jonrob Exp $
#ifndef EVENT_TRANSFORM3DTYPES_H 
#define EVENT_TRANSFORM3DTYPES_H 1

// Include files
#include "Math/Rotation3D.h"
#include "Math/RotationX.h"
#include "Math/RotationY.h"
#include "Math/RotationZ.h"
#include "Math/AxisAngle.h"
#include "Math/Transform3D.h"
#include "Math/EulerAngles.h"
#include "Math/Quaternion.h"
#include "Math/Vector3D.h"

/** @namespace Gaudi Transform3DTypes.h Kernel/Transform3DTypes.h
 *  
 *
 *  @author Juan PALACIOS
 *  @date   2005-11-18
 */
namespace Gaudi {

  typedef ROOT::Math::Rotation3D      Rotation3D;
  typedef ROOT::Math::EulerAngles     EulerAngles;
  typedef ROOT::Math::Quaternion      Quaternion;
  typedef ROOT::Math::AxisAngle       AxisAngle;
  typedef ROOT::Math::RotationX       RotationX;
  typedef ROOT::Math::RotationY       RotationY;
  typedef ROOT::Math::RotationZ       RotationZ;
  typedef ROOT::Math::Transform3D     Transform3D;
  typedef ROOT::Math::XYZVector       TranslationXYZ;

};
#endif // EVENT_TRANSFORM3DTYPES_H
