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
#ifndef OTSIMULATION_OTDATAFUNCTOR_H
#define OTSIMULATION_OTDATAFUNCTOR_H 1

#include "Kernel/OTChannelID.h"
#include <functional>

/** namespace OTDataFunctor
 *
 * @TODO: This file seems not to be used anywhere...
 *
 * This file contains the declaration of OTDataFunctor namespace.
 * It contains functors used for sorting.
 *
 *  @author  M. Needham
 *  @date    May 2002
 */

namespace OTDataFunctor {

  // functors
  struct Less_by_Channel {
    /** compare the channel of one object wOTh the
     *  channel of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    template <class TYPE1, class TYPE2 = TYPE1>
    bool operator()( TYPE1 obj1, TYPE2 obj2 ) const {
      return ( !obj1 ) ? true : ( !obj2 ) ? false : obj1->channel() < obj2->channel();
    }
    ///
  };

  struct Less_by_Key {

    /** compare the channel of one object wOTh the
     *  channel of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    template <class TYPE1, class TYPE2 = TYPE1>
    bool operator()( TYPE1 obj1, TYPE2 obj2 ) const {
      return ( !obj1 ) ? true : ( !obj2 ) ? false : obj1->key() < obj2->key();
    }
    ///
  };

  struct Less_by_tdcTime {

    /** compare the dep charge of one object wOTh the
     *  dep Charge  of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    template <class TYPE1, class TYPE2 = TYPE1>
    bool operator()( TYPE1 obj1, TYPE2 obj2 ) const {
      return ( !obj1 ) ? true : ( !obj2 ) ? false : obj1->tdcTime() < obj2->tdcTime();
    }
    ///
  };

  struct Less_by_ChannelAndTime {

    /** compare the dep charge of one object wOTh the
     *  dep Charge  of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    template <class TYPE1, class TYPE2 = TYPE1>
    bool operator()( TYPE1 obj1, TYPE2 obj2 ) const {
      if ( obj1->channel() != obj2->channel() ) {
        return ( !obj1 ) ? true : ( !obj2 ) ? false : obj1->channel() < obj2->channel();
      } else {
        return ( !obj1 ) ? true : ( !obj2 ) ? false : obj1->tdcTime() < obj2->tdcTime();
      }
    }
    ///
  };

  class station_eq {
    LHCb::OTChannelID aChan;

  public:
    explicit station_eq( const LHCb::OTChannelID& testChan ) : aChan( testChan ) {}
    template <class TYPE>
    bool operator()( TYPE obj ) const {
      return obj && ( obj->channel().station() == aChan.station() );
    }
  };

  class layer_eq {
    LHCb::OTChannelID aChan;

  public:
    explicit layer_eq( const LHCb::OTChannelID& testChan ) : aChan( testChan ) {}
    template <class TYPE>
    bool operator()( TYPE obj ) const {
      return obj->channel().uniqueLayer() == aChan.uniqueLayer();
    }
  };

  class module_eq {
    LHCb::OTChannelID aChan;

  public:
    explicit module_eq( const LHCb::OTChannelID& testChan ) : aChan( testChan ) {}
    template <class TYPE>
    bool operator()( TYPE obj ) const {
      return obj->channelID().uniqueModule() == aChan.uniqueModule();
    }
  };

  struct compByStation_LB {
    template <class TYPE>
    bool operator()( const TYPE& obj, const LHCb::OTChannelID& testID ) const {
      return obj && ( testID.station() > obj->channel().station() );
    }
  };

  struct compByStation_UB {
    template <class TYPE>
    bool operator()( const LHCb::OTChannelID& testID, const TYPE& obj ) const {
      return obj && ( testID.station() > obj->channel().station() );
    }
  };

  struct compByLayer_LB {
    template <class TYPE>
    bool operator()( const TYPE& obj, const LHCb::OTChannelID& testID ) const {
      return obj && ( testID.uniqueLayer() > obj->channel().uniqueLayer() );
    }
  };

  struct compByLayer_UB {
    template <class TYPE>
    bool operator()( const LHCb::OTChannelID& testID, const TYPE& obj ) const {
      return obj && ( testID.uniqueLayer() > obj->channel().uniqueLayer() );
    }
  };

  struct compByModule_LB {
    template <class TYPE>
    bool operator()( const TYPE& obj, const LHCb::OTChannelID& testID ) const {
      return obj && ( testID.uniqueModule() > obj->channel().uniqueModule() );
    }
  };

  struct compBModule_UB {
    template <class TYPE>
    bool operator()( const LHCb::OTChannelID& testID, const TYPE& obj ) const {
      return obj && ( testID.uniqueModule() > obj->channel().uniqueModule() );
    }
  };

} // namespace OTDataFunctor
#endif // OTDATAFUNCTOR_H
