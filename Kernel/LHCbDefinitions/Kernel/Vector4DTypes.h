// $Id: Vector4DTypes.h,v 1.5 2006-05-17 06:11:33 cattanem Exp $
#ifndef EVENT_VECTOR4DTYPES_H 
#define EVENT_VECTOR4DTYPES_H 1

// Include files
#include "Math/Vector4D.h"

/** @namespace Gaudi Vector4DTypes.h Event/Vector4DTypes.h
 *  
 *
 *  @author Juan PALACIOS
 *  @date   2005-11-21
 */
namespace Gaudi {

  typedef ROOT::Math::PxPyPzEVector     XYZTVector;
  typedef ROOT::Math::PxPyPzEVector     LorentzVector;
  typedef ROOT::Math::XYZTVectorF       XYZTVectorF;
  typedef ROOT::Math::XYZTVectorF       LorentzVectorF;
  typedef ROOT::Math::PtEtaPhiEVector   PtEtaPhiVector;

};
#endif // EVENT_VECTOR4DTYPES_H



