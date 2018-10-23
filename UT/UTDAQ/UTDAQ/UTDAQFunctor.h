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
#ifndef _UTDataFunctor_H_
#define _UTDataFunctor_H_

#include <numeric>
#include "Kernel/UTTell1ID.h"

namespace UTDAQFunctor{

template<class TYPE>
struct Less_by_Tell1Board {

    /** compare the channel of one object with the
     *  channel of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    bool operator() ( TYPE obj1 , TYPE obj2 ) const {
      return
        ( !obj1 ) ? true  :
        ( !obj2 ) ? false : obj1->boardID() < obj2->boardID() ;
    }
};

template <class TYPE>
class Tell1Board_eq {
   UTTell1ID aBoard;
public:
  explicit Tell1Board_eq(const UTTell1ID& testBoard) : aBoard(testBoard){}
  inline bool operator() (TYPE obj) const{ return obj->boardID()==aBoard; }
};

template <class TYPE>
struct compByTell1Board_LB {
  bool operator() (const TYPE& obj, const UTTell1ID& testID) const{
    return ((!obj) ? false : testID >obj->boardID());
  }
};

template <class TYPE>
struct compByTell1Board_UB {
  bool operator() (const UTTell1ID& testID, const TYPE& obj) const{
    return ((!obj) ? false : testID >obj->boardID());
  }
};

}
#endif // UTDAQFunctor
