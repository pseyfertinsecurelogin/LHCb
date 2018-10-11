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
#ifndef KERNEL_BITREADER_H 
#define KERNEL_BITREADER_H 1

// Include files
#include <iostream>
#include "Tell1Kernel/VeloDecodeCore.h"


/** 
 *  
 *
 *  @author Tomasz Szumlak
 *  @date   2006-10-20
 */

namespace VeloTELL1{

  template<class T>
  void bitReader(const T& inValue)
  {
    const unsigned int data_size=sizeof(inValue);
    const unsigned int bits=8;
    const unsigned int loop_limit=data_size*bits;
    for(unsigned int nextBit=0; nextBit<loop_limit; ++nextBit){
      if(inValue&(1<<nextBit)){
        std::cout<< "1";
      }else{
        std::cout<< "0";
      }
    }
    std::cout<<std::endl;
  }

}
#endif // KERNEL_BITREADER_H
