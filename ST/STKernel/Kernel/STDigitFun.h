/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef _STDigitFun_H
#define _STDigitFun_H 1

#include "Event/STDigit.h"
#include "Kernel/SiChargeFun.h"

/**  specialization for STDigit */
namespace SiChargeFun{
  template <> 
  inline double charge(LHCb::STDigit* input) {
  return double(input->depositedCharge());
  }
  template <> 
  inline double charge(const LHCb::STDigit* input) {
  return double(input->depositedCharge());
 }
}


#endif
