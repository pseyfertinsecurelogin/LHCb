// $Id: MCFun.h,v 1.1.1.1 2006-07-24 14:56:20 mneedham Exp $
#ifndef _MCFun_H_
#define _MCFun_H_

#include <functional>
#include <algorithm>

// all the fun you could want with MC !
namespace MCFun{

   template <class TYPE1, class TYPE2 = TYPE1 >
   class Less_by_z
     : public std::binary_function<TYPE1,TYPE2,bool>
   {
   public:

     /** compare the z of one object with the
     *  Station  of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator() ( TYPE1 obj1 , TYPE2 obj2 ) const
    {
 
      double z1 = 0.5*(obj1->entry().z() + obj1->exit().z());  
      double z2 = 0.5*(obj2->entry().z() + obj2->exit().z());  

      return
        ( !obj1 ) ? true  :
        ( !obj2 ) ? false : z1 < z2 ;
    }
    ///
   };
};

#endif // _MCFun_H
