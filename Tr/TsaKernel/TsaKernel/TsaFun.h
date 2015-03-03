// $Id: TsaFun.h,v 1.1.1.1 2006-07-24 14:56:20 mneedham Exp $
#ifndef _TsaFun_H_
#define _TsaFun_H_

#include "gsl/gsl_math.h"

namespace Tsa{

  double estimateCurvature(const double parabCurvature, const double cnvFactor) ;

}

inline double Tsa::estimateCurvature(const double parabCurvature, const double cnvFactor) {

  double curvInMeV = cnvFactor*parabCurvature;

  // upper p limit
  double pInMeV = TsaConstants::pMax;
  if ( fabs(1.0/curvInMeV) < pInMeV ){
    pInMeV = fabs(1.0/curvInMeV);
  }

  // lower limit
  pInMeV  = GSL_MAX(pInMeV, TsaConstants::pMin);

  return ((double)GSL_SIGN(curvInMeV))/pInMeV;
}

#endif
