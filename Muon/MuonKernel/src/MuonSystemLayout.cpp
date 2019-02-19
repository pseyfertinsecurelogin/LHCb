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
// Include files
#include "MuonKernel/MuonSystemLayout.h"
#include "Kernel/MuonTileID.h"
#include <iostream>

namespace {
  // note: this properly deals with both references (in which case C is-a reference)
  //       and rvalue references (in which case C is a value type)
  //       (thanks to C++ reference collapsing)
  template <typename C>
  C setStation( C&& c, int s ) {
    for ( auto& i : c ) i.setStation( s );
    return c;
  }
} // namespace
//------------------------------------------------------------------------------
//
/// Implementation of class :  MuonSystemLayout
//
/// Author: A.Tsaregorodtsev
//
//------------------------------------------------------------------------------

MuonSystemLayout::MuonSystemLayout( const MuonStationLayout& lq1, const MuonStationLayout& lq2,
                                    const MuonStationLayout& lq3, const MuonStationLayout& lq4,
                                    const MuonStationLayout& lq5 )
    : m_station_layouts{lq1, lq2, lq3, lq4, lq5} {}

std::vector<LHCb::MuonTileID> MuonSystemLayout::tiles( const LHCb::MuonTileID& pad ) const {
  return tilesInArea( pad, 0, 0 );
}

std::vector<LHCb::MuonTileID> MuonSystemLayout::tilesInArea( const LHCb::MuonTileID& pad, int areaX, int areaY ) const {

  int st = pad.station();
  return m_station_layouts[st].tilesInArea( pad, areaX, areaY );
}

std::vector<LHCb::MuonTileID> MuonSystemLayout::tiles() const {

  std::vector<LHCb::MuonTileID> result;
  for ( int is = 0; is < 5; is++ ) {
    auto tmp = setStation( m_station_layouts[is].tiles(), is );
    result.insert( result.end(), tmp.begin(), tmp.end() );
  }
  return result;
}

std::vector<LHCb::MuonTileID> MuonSystemLayout::tiles( int iq ) const {

  std::vector<LHCb::MuonTileID> result;
  for ( int is = 0; is < 5; is++ ) {
    auto tmp = setStation( m_station_layouts[is].tiles( iq ), is );
    result.insert( result.end(), tmp.begin(), tmp.end() );
  }
  return result;
}

std::vector<LHCb::MuonTileID> MuonSystemLayout::tiles( int iq, int ir ) const {

  std::vector<LHCb::MuonTileID> result;
  for ( int is = 0; is < 5; is++ ) {
    auto tmp = setStation( m_station_layouts[is].tiles( iq, ir ), is );
    result.insert( result.end(), tmp.begin(), tmp.end() );
  }
  return result;
}

std::vector<LHCb::MuonTileID> MuonSystemLayout::tilesInRegion( const LHCb::MuonTileID& pad, int pregion ) const {

  int st = pad.station();
  return setStation( m_station_layouts[st].tilesInRegion( pad, pregion ), st );
}

std::vector<LHCb::MuonTileID> MuonSystemLayout::neighbours( const LHCb::MuonTileID& pad ) const {

  int st = pad.station();
  return setStation( m_station_layouts[st].neighbours( pad ), st );
}

std::vector<LHCb::MuonTileID> MuonSystemLayout::neighbours( const LHCb::MuonTileID& pad, int dirX, int dirY ) const {

  int st = pad.station();
  return setStation( m_station_layouts[st].neighbours( pad, dirX, dirY ), st );
}

std::vector<LHCb::MuonTileID> MuonSystemLayout::neighbours( const LHCb::MuonTileID& pad, int dirX, int dirY,
                                                            int depth ) const {
  int st = pad.station();
  return setStation( m_station_layouts[st].neighbours( pad, dirX, dirY, depth ), st );
}

std::vector<LHCb::MuonTileID> MuonSystemLayout::neighboursInArea( const LHCb::MuonTileID& pad, int dirX, int dirY,
                                                                  int depthX, int depthY ) const {
  int st = pad.station();
  return setStation( m_station_layouts[st].neighboursInArea( pad, dirX, dirY, depthX, depthY ), st );
}

bool MuonSystemLayout::isValidID( const LHCb::MuonTileID& pad ) const {

  int st = pad.station();
  return m_station_layouts[st].isValidID( pad );
}

LHCb::MuonTileID MuonSystemLayout::contains( const LHCb::MuonTileID& pad ) const {
  // It is responsibility of the user to assure that the pad
  // layout is finer than the containing layout
  int              st     = pad.station();
  LHCb::MuonTileID result = m_station_layouts[st].contains( pad );
  result.setStation( st );
  return result;
}
