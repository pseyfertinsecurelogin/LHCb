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
#include "Kernel/UTChannelID.h"

/** @class UTHit UTHit.h PrKernel/UTHit.h
 *  Minimal Implementation of Upstream tracker hit for pattern recognition
 *  @author Renato Quagliani, Christoph Hasse
 *  @date   2016-11-18
 */

namespace UT {

  class Hit final {
  public:
    // constructor
    Hit( LHCb::UTChannelID chanID, unsigned int size, bool highThreshold, double dxDy, double xat0, double zat0,
         double yBegin, double yEnd, double cos, double error, unsigned int strip, double fracStrip )
        : m_cos( cos )
        , m_dxDy( dxDy )
        , m_weight( 1. / error )
        , m_xAtYEq0( xat0 )
        , m_yBegin( yBegin )
        , m_yEnd( yEnd )
        , m_zAtYEq0( zat0 )
        , m_x( xAt( yMid() ) )
        , m_chanID( chanID )
        , m_size( size )
        , m_highThreshold( highThreshold )
        , m_strip( strip )
        , m_fracStrip( fracStrip ) {}

    [[nodiscard]] float cos() const { return m_cos; }
    [[nodiscard]] float cosT() const {
      return ( fabs( m_xAtYEq0 ) < 1.0E-9 ) ? 1. / std::sqrt( 1 + m_dxDy * m_dxDy ) : cos();
    }
    [[nodiscard]] float dxDy() const { return m_dxDy; }
    [[nodiscard]] bool  highThreshold() const { return m_highThreshold; }
    [[nodiscard]] bool  isYCompatible( const float y, const float tol ) const {
      return yMin() - tol <= y && y <= yMax() + tol;
    }
    [[nodiscard]] bool isNotYCompatible( const float y, const float tol ) const {
      return yMin() - tol > y || y > yMax() + tol;
    }
    [[nodiscard]] LHCb::LHCbID      lhcbID() const { return LHCb::LHCbID( m_chanID ); }
    [[nodiscard]] LHCb::UTChannelID chanID() const { return m_chanID; }
    [[nodiscard]] int   planeCode() const { return 2 * ( m_chanID.station() - 1 ) + ( m_chanID.layer() - 1 ) % 2; }
    [[nodiscard]] float sinT() const { return tanT() * cosT(); }
    [[nodiscard]] int   size() const { return m_size; }
    [[nodiscard]] float tanT() const { return -m_dxDy; }
    [[nodiscard]] float weight() const { return m_weight * m_weight; }
    [[nodiscard]] float error() const { return 1.0f / m_weight; }
    [[nodiscard]] float xAt( const float globalY ) const { return m_xAtYEq0 + globalY * m_dxDy; }
    [[nodiscard]] float xAtYEq0() const { return m_xAtYEq0; }
    [[nodiscard]] float xAtYMid() const { return m_x; }
    [[nodiscard]] float xMax() const { return std::max( xAt( yBegin() ), xAt( yEnd() ) ); }
    [[nodiscard]] float xMin() const { return std::min( xAt( yBegin() ), xAt( yEnd() ) ); }
    [[nodiscard]] float xT() const { return cos(); }
    [[nodiscard]] float yBegin() const { return m_yBegin; }
    [[nodiscard]] float yEnd() const { return m_yEnd; }
    [[nodiscard]] float yMax() const { return std::max( yBegin(), yEnd() ); }
    [[nodiscard]] float yMid() const { return 0.5 * ( yBegin() + yEnd() ); }
    [[nodiscard]] float yMin() const { return std::min( yBegin(), yEnd() ); }
    [[nodiscard]] float zAtYEq0() const { return m_zAtYEq0; }
    [[nodiscard]] unsigned int strip() const { return m_strip; }
    [[nodiscard]] double       fracStrip() const { return m_fracStrip; }
    [[nodiscard]] int          pseudoSize() const {
      unsigned int cSize = m_size + 1;
      if ( cSize == 1 ) {
        if ( fracStrip() != 0 ) cSize = 2;
      } else {
        cSize = 3;
      }
      return cSize;
    }

  private:
    float             m_cos;
    float             m_dxDy;    ///< The dx/dy value
    float             m_weight;  ///< The hit weight (1/error)
    float             m_xAtYEq0; ///< The value of x at the point y=0
    float             m_yBegin;  ///< The y value at the start point of the line
    float             m_yEnd;    ///< The y value at the end point of the line
    float             m_zAtYEq0; ///< The value of z at the point y=0
    float             m_x;
    LHCb::UTChannelID m_chanID;
    unsigned int      m_size;
    bool              m_highThreshold;
    unsigned int      m_strip;
    double            m_fracStrip;
  };

  using Hits = std::vector<const Hit*>;

} // namespace UT

#endif // EVENT_UTHIT_H
