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
#ifndef _UTKernel_UTFun_H
#define _UTKernel_UTFun_H

#include "UTBitsPolicy.h"
#include "GaudiKernel/SmartRefVector.h"
#include "Event/UTDigit.h"
#include "Event/UTCluster.h"
#include "Kernel/UTStripRepresentation.h"
#include "Kernel/UTPPRepresentation.h"

#include <utility>

namespace UTFun{

  /** quantize the interstrip fraction  from 'float' */
  double stripFraction(const double interStripPos);

  /**
  * calculate mean strip
  * @param  digits vector of digits
  * @return mean strip
  */
  std::pair<double, unsigned int > position(const SmartRefVector<LHCb::UTDigit>& digits, double trim = 0);


  /**
  * calculate mean strip
  * @param  strips vector of strip values
  * @return mean strip
  */
  std::pair<double, unsigned int > position(const LHCb::UTCluster::ADCVector& strips, double trim = 0);

}

inline double UTFun::stripFraction(const double interStripPos){
  return (LHCb::Math::round((1<<UTBitsPolicy::interStripPrecision)*interStripPos))/double(1<<UTBitsPolicy::interStripPrecision);
}


#endif //_UTKernel_UTFun_H
