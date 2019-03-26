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

#ifndef __DETDESC_TRANSPORTSVC_TRANSPORTSVCGOODLOCALGI_H__
#define __DETDESC_TRANSPORTSVC_TRANSPORTSVCGOODLOCALGI_H__ 1

#include "DetDesc/IGeometryInfo.h"
#include "TransportSvc.h"

/** @file TransportSvcGoodLocalGI.h
 *
 * simple implementation of TransportSvc::goodLocalGI method
 *
 * @attention mandatory that at least one point is INSIDE volume!
 */
inline bool TransportSvc::goodLocalGI( const Gaudi::XYZPoint& point1, const Gaudi::XYZPoint& point2,
                                       IGeometryInfo* gi ) const {
  // constexpr bool debug = false;

  if ( !gi ) {
    // if ( debug ) std::cout << " goodLocalGI : no gi" << std::endl;
    return false;
  }

  const auto* lv = gi->lvolume();

  if ( !lv ) {
    // if ( debug ) std::cout << " goodLocalGI : gi has no lv" << std::endl;
    return false;
  }

  if ( lv->isAssembly() ) {
    // if ( debug ) std::cout << " goodLocalGI : lv " << lv->name()
    //                       << " is Assembly " << std::endl;
    return false;
  }

  ISolid::Tick  tickMin( 0 ), tickMax( 1 );
  ISolid::Ticks local_ticks;
  auto          nInt = lv->solid()->intersectionTicks( gi->toLocalMatrix() * point1,
                                              gi->toLocalMatrix() * Gaudi::XYZVector( point2 - point1 ), tickMin,
                                              tickMax, local_ticks );

  // if ( debug ) std::cout << " goodLocalGI : nInt " << nInt << std::endl;

  return ( 2 == nInt && tickMin == local_ticks.front() && tickMax == local_ticks.back() );
}

// ============================================================================
// The End
// ============================================================================
#endif //        __DETDESC_TRANSPORTSVC_TRANSPORTSVCGOODLOCALGI_H__
