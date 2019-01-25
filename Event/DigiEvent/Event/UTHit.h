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
#ifndef EVENT_UTHIT_H
#define EVENT_UTHIT_H 1

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
  Hit( LHCb::UTChannelID chanID, unsigned int size, bool highThreshold,
       double dxDy, double xat0, double zat0, double yBegin,
       double yEnd, double cos, double error, unsigned int strip, double fracStrip )
    : m_cos( cos )
      , m_dxDy( dxDy )
      , m_weight( 1. / error )
      , m_xAtYEq0( xat0 )
      , m_yBegin( yBegin )
      , m_yEnd( yEnd )
      , m_zAtYEq0( zat0 )
      , m_x( xAt( yMid() ) )
      , m_chanID(chanID)
      , m_size(size)
      , m_highThreshold(highThreshold)
      , m_strip(strip)
      , m_fracStrip(fracStrip)
    {
    }

     float cos() const { return m_cos; }
     float cosT() const { return ( fabs( m_xAtYEq0 ) < 1.0E-9 ) ? 1. / std::sqrt( 1 + m_dxDy * m_dxDy ) : cos(); }
     float dxDy() const { return m_dxDy; }
     bool highThreshold() const { return m_highThreshold; }
     bool isYCompatible( const float y, const float tol ) const { return yMin() - tol <= y && y <= yMax() + tol; }
     bool isNotYCompatible( const float y, const float tol ) const { return yMin() - tol > y || y > yMax() + tol; }
     LHCb::LHCbID lhcbID() const { return LHCb::LHCbID( m_chanID ); }
     LHCb::UTChannelID chanID() const { return m_chanID; }
     int planeCode() const { return 2 * ( m_chanID.station() - 1 ) + ( m_chanID.layer() - 1 ) % 2; }
     float sinT() const { return tanT() * cosT(); }
     int size() const { return m_size; }
     float tanT() const { return -m_dxDy; }
     float weight() const { return m_weight * m_weight; }
     float error() const { return 1.0f / m_weight; }
     float xAt( const float globalY ) const { return m_xAtYEq0 + globalY * m_dxDy; }
     float xAtYEq0() const { return m_xAtYEq0; }
     float xAtYMid() const { return m_x; }
     float xMax() const { return std::max( xAt( yBegin() ), xAt( yEnd() ) ); }
     float xMin() const { return std::min( xAt( yBegin() ), xAt( yEnd() ) ); }
     float xT() const { return cos(); }
     float yBegin() const { return m_yBegin; }
     float yEnd() const { return m_yEnd; }
     float yMax() const { return std::max( yBegin(), yEnd() ); }
     float yMid() const { return 0.5 * ( yBegin() + yEnd() ); }
     float yMin() const { return std::min( yBegin(), yEnd() ); }
     float zAtYEq0() const { return m_zAtYEq0; }
     unsigned int strip() const { return m_strip; }
     double fracStrip() const { return m_fracStrip; }
     int pseudoSize() const {
      unsigned int cSize = m_size  + 1;
      if (cSize == 1){
        if (fracStrip() != 0) cSize = 2;
      } else {
        cSize = 3;
      }
      return cSize;
    }

  private:
    float m_cos;
    float m_dxDy;    ///< The dx/dy value
    float m_weight;  ///< The hit weight (1/error)
    float m_xAtYEq0; ///< The value of x at the point y=0
    float m_yBegin;  ///< The y value at the start point of the line
    float m_yEnd;    ///< The y value at the end point of the line
    float m_zAtYEq0; ///< The value of z at the point y=0
    float m_x;
    LHCb::UTChannelID m_chanID;
    unsigned int m_size;
    bool m_highThreshold;
    unsigned int m_strip;
    double m_fracStrip;
  };

  typedef std::vector<const Hit*> Hits;

}

#endif // EVENT_UTHIT_H
