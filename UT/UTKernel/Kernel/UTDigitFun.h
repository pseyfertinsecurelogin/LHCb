#ifndef _UTDigitFun_H
#define _UTDigitFun_H 1

#include "Event/UTDigit.h"
#include "Kernel/SiChargeFun.h"

/**  specialization for UTDigit */
namespace SiChargeFun{
  template <> 
  inline double charge(LHCb::UTDigit* input) {
  return double(input->depositedCharge());
  }
  template <> 
  inline double charge(const LHCb::UTDigit* input) {
  return double(input->depositedCharge());
 }
}


#endif
