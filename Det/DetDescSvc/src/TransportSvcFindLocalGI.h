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
#ifndef DETDESC_TRANSPORTSVC_TRANSPORTSVCFINDLOCALGI_H
#define DETDESC_TRANSPORTSVC_TRANSPORTSVCFINDLOCALGI_H 1

#include <sstream>

// local
#include "DetDesc/IGeometryInfo.h"
#include "TransportSvc.h"

/** @file
 *
 *  simple implementation of TransportSvc::findLocalGI method
 *  @see TranpsortSvc
 *  @see ITransportSvc
 *
 *  @author: Vanya Belyaev
 */

IGeometryInfo* TransportSvc::findLocalGI( const Gaudi::XYZPoint& point1, const Gaudi::XYZPoint& point2,
                                          IGeometryInfo* gi, IGeometryInfo* topGi ) const {
  if ( !gi ) { return nullptr; }

  /// output :-))
  IGeometryInfo* goodGI = nullptr;

  try {
    /// find the nearest "upper" volume, which contains the  first point
    if ( !gi->isInside( point1 ) ) { return nullptr; }
    IGeometryInfo* gi1 = gi->belongsTo( point1, -1 );

    /// find the nearest "upper" volume, which contains the second point
    IGeometryInfo* gi2 = gi1;
    {
      bool loc = false;
      for ( loc = gi2->isInside( point2 ); !loc && nullptr != gi2; gi2 = gi2->parentIGeometryInfo() ) {
        loc = gi2->isInside( point2 );
        if ( loc ) break;
      }
      if ( nullptr == gi2 ) { return nullptr; }
    }

    // Here both points are located, gi2 is a parent of gi1. Get the first
    // parent of gi2 which is a good GI.

    for ( IGeometryInfo* gl = gi2; nullptr != gl; gl = gl->parentIGeometryInfo() ) {
      if ( nullptr == gl ) return nullptr;
      if ( goodLocalGI( point1, point2, gl ) ) { return gl; }
      if ( topGi == gl ) return nullptr;
    }
    /// we have failed to find "good" element
    return nullptr; /// RETURN !!!

  } catch ( const GaudiException& Exception ) {
    /// throw new exception:
    std::string message( "TransportSvc::findLocalGI(...), exception caught; Params: " );
    {
      std::ostringstream ost;
      ost << "Point1=" << point1 << "Point2=" << point2;
      message += ost.str();
      Assert( false, message, Exception );
    }
  } catch ( ... ) {
    /// throw new exception:
    std::string message( "TransportSvc::findLocalGI(...), unknown exception caught; Params: " );
    {
      std::ostringstream ost;
      ost << "Point1=" << point1 << "Point2=" << point2;
      message += ost.str();
      Assert( false, message );
    }
  }

  ///
  return goodGI;
}

// ============================================================================
// The End
// ============================================================================
#endif //   __DETDESC_TRANSPORTSVC_TRANSPORTSVCFINDLOCALGI_H__
