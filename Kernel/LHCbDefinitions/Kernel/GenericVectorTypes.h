// $Id: GenericVectorTypes.h,v 1.2 2006-05-08 07:05:55 jpalac Exp $
#ifndef KERNEL_GENERICVECTORTYPES_H 
#define KERNEL_GENERICVECTORTYPES_H 1

// Include files
#include "Math/SVector.h"
/** @namespace Gaudi GenericVectorTypes.h Kernel/GenericVectorTypes.h
 *  
 *
 *  @author Juan PALACIOS
 *  @date   2005-12-08
 */
namespace Gaudi {

  typedef ROOT::Math::SVector<double, 1> Vector1;
  typedef ROOT::Math::SVector<double, 2> Vector2;  
  typedef ROOT::Math::SVector<double, 3> Vector3;
  typedef ROOT::Math::SVector<double, 4> Vector4;
  typedef ROOT::Math::SVector<double, 5> Vector5;
  typedef ROOT::Math::SVector<double, 6> Vector6;  
  typedef ROOT::Math::SVector<double, 7> Vector7;
  typedef ROOT::Math::SVector<double, 8> Vector8;
  typedef ROOT::Math::SVector<double, 9> Vector9;

  typedef ROOT::Math::SVector<float, 1> Vector1F;
  typedef ROOT::Math::SVector<float, 2> Vector2F;
  typedef ROOT::Math::SVector<float, 3> Vector3F;
  typedef ROOT::Math::SVector<float, 4> Vector4F;
  typedef ROOT::Math::SVector<float, 5> Vector5F;
  typedef ROOT::Math::SVector<float, 6> Vector6F;
  typedef ROOT::Math::SVector<float, 7> Vector7F;
  typedef ROOT::Math::SVector<float, 8> Vector8F;
  typedef ROOT::Math::SVector<float, 9> Vector9F;

};
#endif // KERNEL_GENERICVECTORTYPES_H
