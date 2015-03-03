// $Id: Point3DTypes.h,v 1.5 2005-12-06 18:52:55 jpalac Exp $
#ifndef EVENT_POINT3DTYPES_H 
#define EVENT_POINT3DTYPES_H 1

// Include files
#include "Math/Point3D.h"

/** @namespace Gaudi Point3DTypes.h Event/Point3DTypes.h
 *  
 *
 *  @author Juan PALACIOS
 *  @date   2005-11-18
 */
namespace Gaudi {
  typedef ROOT::Math::XYZPoint         XYZPoint;
  typedef ROOT::Math::Polar3DPoint     Polar3DPoint;
  typedef ROOT::Math::RhoEtaPhiPoint   RhoEtaPhiPoint;
  typedef ROOT::Math::RhoEtaPhiPoint   RhoZPhiPoint;

  typedef ROOT::Math::XYZPointF        XYZPointF;
  typedef ROOT::Math::Polar3DPointF    Polar3DPointF;
  typedef ROOT::Math::RhoEtaPhiPointF  RhoEtaPhiPointF;
  typedef ROOT::Math::RhoEtaPhiPointF  RhoZPhiPointF;
};
#endif // EVENT_POINT3DTYPES_H
