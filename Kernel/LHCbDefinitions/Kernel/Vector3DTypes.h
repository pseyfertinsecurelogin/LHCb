// $Id: Vector3DTypes.h,v 1.7 2006-04-04 18:05:17 jpalac Exp $
#ifndef EVENT_VECTOR3DTYPES_H 
#define EVENT_VECTOR3DTYPES_H 1

// Include files
#include "Math/Vector3D.h"

/** @namespace Gaudi Vector3DTypes.h Event/Vector3DTypes.h
 *  
 *
 *  @author Juan PALACIOS
 *  @date   2005-11-21
 */
namespace Gaudi {

  typedef ROOT::Math::XYZVector            XYZVector;
  typedef ROOT::Math::Polar3DVector        Polar3DVector;
  typedef ROOT::Math::RhoEtaPhiVector      RhoEtaPhiVector;
  typedef ROOT::Math::RhoZPhiVector        RhoZPhiVector;
  typedef ROOT::Math::XYZVectorF           XYZVectorF;
  typedef ROOT::Math::Polar3DVectorF       Polar3DVectorF;
  typedef ROOT::Math::RhoEtaPhiVectorF     RhoEtaPhiVectorF;
  typedef ROOT::Math::RhoZPhiVectorF       RhoZPhiVectorF;

};
#endif // EVENT_VECTOR3DTYPES_H
