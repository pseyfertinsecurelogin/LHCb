#ifndef _STKernel_STFun_H
#define _STKernel_STFun_H

#include <utility>

#include "Event/STCluster.h"
#include "Event/STDigit.h"
#include "GaudiKernel/SmartRefVector.h"
#include "Kernel/PPRepresentation.h"
#include "Kernel/StripRepresentation.h"
#include "LHCbMath/LHCbMath.h"
#include "STBitsPolicy.h"

namespace LHCb {
class STDigit;
}  // namespace LHCb
template <class TYPE> class SmartRefVector;

namespace STFun{

  /** quantize the interstrip fraction  from 'float' */
  double stripFraction(const double interStripPos);

  /**
  * calculate mean strip
  * @param  digits vector of digits
  * @return mean strip
  */
  std::pair<double, unsigned int > position(const SmartRefVector<LHCb::STDigit>& digits, double trim = 0);


  /**
  * calculate mean strip
  * @param  strips vector of strip values
  * @return mean strip
  */
  std::pair<double, unsigned int > position(const LHCb::STCluster::ADCVector& strips, double trim = 0);

}

inline double STFun::stripFraction(const double interStripPos){
  return (LHCb::Math::round((1<<STBitsPolicy::interStripPrecision)*interStripPos))/double(1<<STBitsPolicy::interStripPrecision);
}


#endif //_STKernel_STFun_H
