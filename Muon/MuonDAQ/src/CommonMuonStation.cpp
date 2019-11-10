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
/** Implementation of CommonMuonStation
 *
 * 2010-12-07: Roel Aaij
 */
#include <exception>
#include <vector>

#include <Event/MuonCoord.h>
#include <Kernel/MuonTileID.h>

#include "MuonDAQ/CommonMuonStation.h"

CommonMuonStation::CommonMuonStation( const DeMuonDetector& det, unsigned int station, std::array<double, 8> regions )
    : m_xboundaries{std::move( regions )}
    , m_hits{}
    , m_index{nRegionsY * ( m_xboundaries.size() - 1 ) + 2}
    , m_z{det.getStationZ( station )}
    , m_ymax{det.getOuterY( station )}
    , m_station{station} {}

CommonMuonStation::CommonMuonStation( const DeMuonDetector& det, unsigned int station, std::array<double, 8> regions,
                                      CommonMuonHits hits )
    : m_xboundaries{std::move( regions )}
    , m_index{nRegionsY * ( m_xboundaries.size() - 1 ) + 2}
    , m_z{det.getStationZ( station )}
    , m_ymax{det.getOuterY( station )}
    , m_station{station} {
  setHits( std::move( hits ) );
}

CommonMuonHitRange CommonMuonStation::hits( float xmin, unsigned int region ) const {
  auto first = std::find_if( m_index[region], m_index[region + 1],
                             [=]( const CommonMuonHit& hit ) { return ( hit.x() + hit.dx() / 2. ) > xmin; } );
  return {first, m_index[region + 1]};
}

CommonMuonHitRange CommonMuonStation::hits( float xmin, float xmax, unsigned int region ) const {
  auto first = std::find_if( m_index[region], m_index[region + 1],
                             [=]( const CommonMuonHit& hit ) { return ( hit.x() + hit.dx() / 2. ) > xmin; } );
  auto last  = std::find_if( first, m_index[region + 1], [=]( const CommonMuonHit& hit ) {
    // TODO: this weird asymmetry between first, last is for historical
    // reasons...
    return hit.x() > xmax;
    // return ( hit.x() - hit.dx() / 2.) > xmax;
  } );
  return {first, last};
}

CommonMuonHitRange CommonMuonStation::hits( unsigned int region ) const {
  return {m_index[region], m_index[region + 1]};
}

void CommonMuonStation::setHits( CommonMuonHits&& hts ) {
  m_hits = std::move( hts );

  // TODO: first get rid of invalid hits ( x < xlow, y<ymin, y>ymax )
  // m_hits.erase( std::remove_if( std::begin(m_hits), std::end(m_hits), ... )
  // );
  auto by_x  = []( const CommonMuonHit& lhs, const CommonMuonHit& rhs ) { return lhs.x() < rhs.x(); };
  auto y_lt_ = []( float ymax ) { return [=]( const CommonMuonHit& h ) { return h.y() < ymax; }; };
  auto x_lt_ = []( float xmax ) { return [=]( const CommonMuonHit& h ) { return h.x() < xmax; }; };

  auto id = std::begin( m_index );
  *id     = std::begin( m_hits );
  // partition in x
  for ( auto x = std::next( std::begin( m_xboundaries ) ); x != std::end( m_xboundaries ); ++x ) {
    auto imax = std::partition( *id, std::end( m_hits ), x_lt_( *x ) );
    // within each x partition, partition in y
    for ( unsigned j = 0; j < nRegionsY; ++j ) {
      *std::next( id ) = std::partition( *id, imax, y_lt_( -m_ymax + ( j + 1 ) * 2 * m_ymax / nRegionsY ) );
      // within each region, sort by x
      std::sort( *id, *std::next( id ), by_x );
      ++id; // next region...
    }
    assert( *id == imax ); // no invalid hits in y...
  }
  *std::next( id ) = std::end( m_hits ); // last region contains hits outside boundaries
  assert( *id == std::end( m_hits ) );
  assert( uint( std::distance( std::begin( m_index ), id ) ) == uint( m_index.size() - 1 ) );
}
