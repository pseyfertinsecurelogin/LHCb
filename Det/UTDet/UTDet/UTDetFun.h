#ifndef _UTDetFun_H_
#define _UTDetFun_H_

#include <string>
#include "UTDet/DeUTBaseElement.h"

namespace UTDetFun {

  struct SortByY {
    inline bool operator() (DeUTBaseElement* obj1 , DeUTBaseElement* obj2 ) const 
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

#endif // _UTDataFunctor_H_
