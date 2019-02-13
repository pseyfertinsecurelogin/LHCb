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
#pragma once

// Include files
#include "Kernel/LHCbID.h"
#include "UTDet/DeUTSector.h"
#include "Kernel/UTChannelID.h"

/** @class UTHit UTHit.h PrKernel/UTHit.h
 *  Minimal Implementation of Upstream tracker hit for pattern recognition
 *  @author Renato Quagliani, Christoph Hasse
 *  @date   2016-11-18
 */


namespace UT
{

  class Hit final
  {
  public:
    // constructor
  Hit( LHCb::UTChannelID chanID, unsigned int size,
       double dxDy, double xat0, double zat0, double yBegin,
       double yEnd, double error, double fracStrip )
    : xAtYEq0( xat0 )
      , yBegin( yBegin )
      , zAtYEq0( zat0 )
      , yEnd( yEnd )
      , dxdy( dxDy )
      , error( error )
      , id(chanID)
      , m_size(size)
      , m_fracStrip(fracStrip)
    {
    }

    // Actually used.
    float xAt( const float globalY ) const { return xAtYEq0 + globalY * dxdy; }

    // Expand/move.
    int planeCode() const { return 2 * ( id.station() - 1 ) + ( id.layer() - 1 ) % 2; }  // used in PrAddUTHitsTool, DumpUTHits, PrVeloUT, PrLongLivedTracking, PrTrackerDumper, PrTrackerDumper2 and PrTrackRecoDumper
    float weight() const { return 1.0f/(error * error); } // used in PrVeloUT, PrLongLivedTracking, PrTrackerDumper, PrTrackerDumper2 and PrTrackRecoDumper
    float xAtYMid() const { return xAt((yBegin + yEnd)*0.5f); } // used in PrAddUTHitsTool, PrTrackerDumper, PrTrackerDumper2 and PrTrackRecoDumper
    
    // Ask about.
    int size() const { return m_size; } // used in PrDebugUTTruthTool, PrTrackerDumper and PrTrackerDumper2
    double fracStrip() const { return m_fracStrip; } // used in MeasurementProviderT
    int pseudoSize() const { // used in MeasurementProviderT
      unsigned int cSize = m_size  + 1;
      if (cSize == 1){
        if (m_fracStrip != 0) cSize = 2;
      } else {
        cSize = 3;
      }
      return cSize;
    }

  public:
    float xAtYEq0; ///< The value of x at the point y=0
    float yBegin;  ///< The y value at the start point of the line
    float zAtYEq0; ///< The value of z at the point y=0
    float yEnd;    ///< The y value at the end point of the line
    float dxdy;    ///< The dx/dy value
    float error;   ///< I don't actually know...
    LHCb::UTChannelID id;
    unsigned int m_size;
    double m_fracStrip;
  };

  typedef std::vector<const Hit*> Hits;


  inline float CosUtFiberAngle(const Hit& hit) {
    // cos(fiber angle) = 1 / sqrt(dxdy^2 + 1)
    // This is a fourth order Taylor-series approximation.
    // It is fairly accurate in the range -0.08 < dxdy < 0.08, or -5 deg < angle < 5 deg.
    // Error is in the order of floating point epsilon.
    // Second order approximation has and error of about 0.004% in aforementioned range.
    float x = hit.dxdy;
    float x2 = x*x;
    float x4 = x2*x2;
    float c_approx = 1 - 0.5f*x2 + 0.375*x4;
    return c_approx;
  }

  inline float SinUtFiberAngle(const Hit& hit) {
    return hit.dxdy * CosUtFiberAngle(hit);
  }

}
