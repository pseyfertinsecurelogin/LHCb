// $Id: TsaDataFunctor.h,v 1.2 2006-12-06 14:35:19 mneedham Exp $
#ifndef _TsaDataFunctor_H_
#define _TsaDataFunctor_H_

#include <functional>
#include <algorithm>
 
namespace TsaDataFunctor{

template <class TYPE1, class TYPE2 = TYPE1 >
  class Less_by_Station_tX
    : public std::binary_function<TYPE1,TYPE2,bool>
  {
  public:

    /** compare the tX of one object with the
     *  tX  of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator() ( TYPE1 obj1 , TYPE2 obj2 ) const
    {

      if (obj1->station() != obj2->station()) {
        return (obj1->station() < obj2->station());
      } 
      else {
        return  (obj1->tX() < obj2->tX());
      }
    }
    ///
  };

template <class TYPE1, class TYPE2 = TYPE1 >
  class Less_by_Tx
    : public std::binary_function<TYPE1,TYPE2,bool>
  {
  public:

    /** compare the tX of one object with the
     *  tX  of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator() ( TYPE1 obj1 , TYPE2 obj2 ) const
    {
      return
        ( !obj1 ) ? true  :
        ( !obj2 ) ? false : obj1->tX() < obj2->tX() ;
    }
    ///
  };

template <class TYPE1, class TYPE2 = TYPE1 >
  class Less_by_Station
    : public std::binary_function<TYPE1,TYPE2,bool>
  {
  public:

    /** compare the Station of one object with the
     *  Station  of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator() ( TYPE1 obj1 , TYPE2 obj2 ) const
    {
      return
        ( !obj1 ) ? true  :
        ( !obj2 ) ? false : obj1->station() < obj2->station() ;
    }
    ///
  };

template <class TYPE1, class TYPE2 = TYPE1 >
  class Greater_by_Meas
    : public std::binary_function<TYPE1,TYPE2,bool>
  {
  public:

    /** Sort by n Measurement
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator() ( TYPE1 obj1 , TYPE2 obj2 ) const
    {
      return
        ( !obj1 ) ? true  :
        ( !obj2 ) ? false : obj1->nMeas() > obj2->nMeas() ;
    }
    ///
  };

template <class TYPE1, class TYPE2 = TYPE1 >
  class Greater_by_P
    : public std::binary_function<TYPE1,TYPE2,bool>
  {
  public:

    /** Sort by p Measurement
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator() ( TYPE1 obj1 , TYPE2 obj2 ) const
    {
      return
        ( !obj1 ) ? true  :
        ( !obj2 ) ? false : fabs(1/obj1->qDivP()) > fabs(1/obj2->qDivP()) ;
    }
    ///
  };

template <class TYPE1, class TYPE2 = TYPE1 >
  class Less_by_Chi2
    : public std::binary_function<TYPE1,TYPE2,bool>
  {
  public:

    /** compare by chi2
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator() ( TYPE1 obj1 , TYPE2 obj2 ) const
    {
      return
        ( !obj1 ) ? true  :
        ( !obj2 ) ? false : obj1->chi2()/obj1->nMeas() < obj2->chi2()/obj2->nMeas() ;
    }
    ///
  };

template <class TYPE>
class compByStation_LB: public std::binary_function<const TYPE, const unsigned int, bool>{
public:
  inline bool operator() (const TYPE& obj, const unsigned int& testID) const{
    return ((!obj) ? false : testID >obj->station());
  }
};

template <class TYPE>
class compByStation_UB: public std::binary_function<const unsigned int,const TYPE ,bool>{
public:
  inline bool operator() (const unsigned int& testID, const TYPE& obj) const{
    return ((!obj) ? false : testID >obj->station());
  }
};

}

#endif






