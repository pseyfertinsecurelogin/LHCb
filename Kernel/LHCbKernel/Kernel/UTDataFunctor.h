#ifndef _UTDataFunctor_H_
#define _UTDataFunctor_H_

#include <cmath>
#include <numeric>

#include "Kernel/UTChannelID.h"


//
// This File contains the declaration of UTDataFunctor namespace
//   - dumping ground for things that don't fit.
//
// C++ code for 'LHCb Tracking package(s)'
//
//   Author: A. Beiter (based on code by M. Needham)
//   Created: 2018-09-04

namespace UTDataFunctor {

// functors
template <class TYPE1, class TYPE2 = TYPE1 >
  class Less_by_Channel
  {
  public:

    /** compare the channel of one object with the
     *  channel of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator() ( TYPE1 obj1 , TYPE2 obj2 ) const
    {
      return
        ( !obj1 ) ? true  :
        ( !obj2 ) ? false : obj1->channelID() < obj2->channelID() ;
    }
    ///
  };

// for the detector element the channelID is called the elemented ID
template <class TYPE1, class TYPE2 = TYPE1 >
  class Less_by_ElementID
  {
  public:

    /** compare the element of one channel of one object with the
     *  channel of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator() ( TYPE1 obj1 , TYPE2 obj2 ) const
    {
      return
        ( !obj1 ) ? true  :
        ( !obj2 ) ? false : obj1->elementID() < obj2->elementID() ;
    }
    ///
  };



template <class TYPE1, class TYPE2 = TYPE1 >
  class Less_by_Key
  {
  public:

    /** compare the channel of one object with the
     *  channel of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator() ( TYPE1 obj1 , TYPE2 obj2 ) const
    {
      return
        ( !obj1 ) ? true  :
        ( !obj2 ) ? false : obj1->key() < obj2->key() ;
    }
    ///
  };

template <class TYPE1, class TYPE2 = TYPE1 >
  class Less_by_depositedCharge
  {
  public:

    /** compare the dep charge of one object with the
     *  dep Charge  of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator() ( TYPE1 obj1 , TYPE2 obj2 ) const
    {
      return
        ( !obj1 ) ? true  :
        ( !obj2 ) ? false : obj1->depositedCharge() < obj2->depositedCharge() ;
    }
    ///
  };

/// class for accumulating energy
template <class TYPE>
  struct Accumulate_Charge {
    inline double  operator() ( double& charge  , TYPE obj ) const {
      return ( !obj ) ? charge :  charge+= obj->depositedCharge() ; };
    ///
};

// class for accumulating charge2
template <class TYPE>
  struct Accumulate_Charge2 {
    inline double  operator() ( double& charge2  , TYPE obj ) const {
      return ( !obj ) ? charge2 :  charge2+= std::pow(obj->depositedCharge(),2) ; };
    ///
};

template <class TYPE>
class station_eq {
   LHCb::UTChannelID aChan;
public:
  explicit station_eq(const LHCb::UTChannelID& testChan) : aChan(testChan){}
  inline bool operator() (TYPE obj) const{
  return obj->channelID().station()==aChan.station();}
};

template <class TYPE>
class layer_eq {
   LHCb::UTChannelID aChan;
public:
  explicit layer_eq(const LHCb::UTChannelID& testChan) : aChan(testChan){}
  inline bool operator() (TYPE obj) const{
  return obj->channelID().uniqueLayer()==aChan.uniqueLayer();}
};

template <class TYPE>
class sector_eq {
   LHCb::UTChannelID aChan;
public:
  explicit sector_eq(const LHCb::UTChannelID& testChan) : aChan(testChan){}
  inline bool operator() (TYPE obj) const{
  return obj->channelID().uniqueSector()==aChan.uniqueSector();}
};

template <class TYPE>
class compByStation_LB {
public:
  inline bool operator() (const TYPE& obj, const LHCb::UTChannelID& testID) const{
    return ((!obj) ? false : testID.station() >obj->channelID().station());
  }
};

template <class TYPE>
class compByStation_UB {
public:
  inline bool operator() (const LHCb::UTChannelID& testID, const TYPE& obj) const{
    return ((!obj) ? false : testID.station() >obj->channelID().station());
  }
};

template <class TYPE>
class compByLayer_LB {
   LHCb::UTChannelID testID;
public:
  inline bool operator() (const TYPE& obj,const LHCb::UTChannelID& testID) const{
    return ((!obj) ? false : testID.uniqueLayer() >obj->channelID().uniqueLayer());
  }
};

template <class TYPE>
class compByLayer_UB {
   LHCb::UTChannelID testID;
public:
  inline bool operator()(const LHCb::UTChannelID& testID, const TYPE& obj) const{
    return ((!obj) ? false : testID.uniqueLayer() >obj->channelID().uniqueLayer());
  }
};

template <class TYPE>
class compBySector_LB {
   LHCb::UTChannelID testID;
public:
  inline bool operator() (const TYPE& obj,const LHCb::UTChannelID& testID) const{
    return ((!obj) ? false : testID.uniqueSector() >obj->channelID().uniqueSector());
  }
};

template <class TYPE>
class compBySector_UB {
   LHCb::UTChannelID testID;
public:
  inline bool operator() (const LHCb::UTChannelID& testID, const TYPE& obj) const{
    return ((!obj) ? false : testID.uniqueSector() >obj->channelID().uniqueSector());
  }
};
}
#endif // _UTDataFunctor_H_
