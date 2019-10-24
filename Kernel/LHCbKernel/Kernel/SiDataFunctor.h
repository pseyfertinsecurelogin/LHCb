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
#ifndef _SiDataFunctor_H_
#define _SiDataFunctor_H_

#include <numeric>

//
// This File contains the declaration of STDataFunctor namespace
//   - dumping ground for things that don't fit.
//
// C++ code for 'LHCb Tracking package(s)'
//
//   Author: M. Needham
//   Created:

namespace SiDataFunctor {

  // functors
  template <typename TYPE>
  struct CompareByChannel final {

    using comp_type = typename TYPE::chan_type;

    bool operator()( const TYPE& obj, const comp_type& testID ) const { return ( testID > obj.channelID() ); }

    bool operator()( const comp_type& testID, const TYPE& obj ) const { return ( obj.channelID() > testID ); }
  };

  /// class for accumulating energy
  template <class TYPE>
  struct Accumulate_Charge final {
    double operator()( double& charge, TYPE obj ) const { return ( !obj ) ? charge : charge += obj->totalCharge(); };
    ///
  };

  template <class TYPE1, class TYPE2 = TYPE1>
  struct Less_by_Channel final {
    /** compare the channel of one object with the
     *  channel of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator()( TYPE1 obj1, TYPE2 obj2 ) const { return obj1.channelID() < obj2.channelID(); }
    ///
  };

  template <class TYPE1, class TYPE2 = TYPE1>
  struct Less_by_Charge final {

    /** compare the dep charge of one object with the
     *  dep Charge  of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator()( TYPE1 obj1, TYPE2 obj2 ) const {
      return ( !obj1 ) ? true : ( !obj2 ) ? false : obj1->totalCharge() < obj2->totalCharge();
    }
    ///
  };
} // namespace SiDataFunctor

#endif // _SiDataFunctor_H_
