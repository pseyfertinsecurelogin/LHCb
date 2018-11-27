/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef _STDetFun_H_
#define _STDetFun_H_

#include <string>
#include "STDet/DeSTBaseElement.h"

namespace STDetFun {

  struct SortByY {
    inline bool operator() (DeSTBaseElement* obj1 , DeSTBaseElement* obj2 ) const 
    { 
      return 
        ( !obj1 ) ? true  : 
        ( !obj2 ) ? false : obj1->globalCentre().y() < obj2->globalCentre().y() ; 
    }
  };


  /// equal by (nick)name
  template <class TYPE>
  class equal_by_name {
    std::string name;
  public:
    explicit equal_by_name(std::string testname): name(std::move(testname)){}
    inline bool operator() (const TYPE& obj) const{
      return obj->nickname() == name;
    }  
  };

}

#endif // _STDataFunctor_H_
