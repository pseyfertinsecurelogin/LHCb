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
#ifndef _VPDataFunctor_H_
#define _VPDataFunctor_H_
#include "Kernel/VPChannelID.h"

// Functors used in VP classes
//
//   Author: M. Kucharczyk
//   Created: 12.10.2009

namespace VPDataFunctor {

template <class TYPE1, class TYPE2 = TYPE1>
struct Less_by_Channel final {
  /** Compare the channel of one object with the channel of another object
   *  @param obj1   first object
   *  @param obj2   second object
   *  @return  result of the comparison
   */
  inline bool operator()(TYPE1 obj1, TYPE2 obj2) const {
    return (!obj1) ? true :
           (!obj2) ? false :
           obj1->channelID() < obj2->channelID();
  }
};

template <class TYPE1, class TYPE2 = TYPE1>
struct Less_by_Key final {
  /** Compare the key of one object with the key of another object
   *  @param obj1   first object
   *  @param obj2   second object
   *  @return  result of the comparison
   */
  inline bool operator()(TYPE1 obj1, TYPE2 obj2) const {
    return (!obj1) ? true :
           (!obj2) ? false :
           obj1->key() < obj2->key();
  }
};

}

#endif // _VPDataFunctor_H
