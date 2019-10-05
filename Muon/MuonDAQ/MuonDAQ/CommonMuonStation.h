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
#include "MuonDAQ/CommonMuonHit.h"
#include "MuonDet/DeMuonDetector.h"

#include <array>
#include <vector>

/** @class CommonMuonStation CommonMuonStation.h
 *  Class to hold regions of hits and provide some geometry information
 */
class CommonMuonStation final {
public:
  CommonMuonStation() = default;
  CommonMuonStation( const DeMuonDetector& det, unsigned int station, std::array<double, 8> regions,
                     CommonMuonHits hits );
  CommonMuonStation( const DeMuonDetector& det, unsigned int station, std::array<double, 8> regions );

  enum { nRegionsY = 9u };

  double z() const { return m_z; }

  unsigned int station() const { return m_station; }

  CommonMuonHitRange hits( float xmin, unsigned int region ) const;
  CommonMuonHitRange hits( float xmin, float xmax, unsigned int region ) const;

  CommonMuonHitRange hits( unsigned int region ) const;

  unsigned int nRegions() const { return ( m_xboundaries.size() - 1 ) * nRegionsY; }

  // TODO: generate range of id which overlap (xMin,xMax,yMin,yMax)...
  bool overlaps( unsigned id, float xmin, float xmax, float ymin_, float ymax ) const {
    auto j = id % nRegionsY;
    auto i = id / nRegionsY;
    auto y = -m_ymax + j * 2 * m_ymax / nRegionsY;
    return !( xmax < m_xboundaries[i] || xmin > m_xboundaries[i + 1] || ymin_ > y + 2 * m_ymax / nRegionsY ||
              ymax < y );
  }

private:
  friend class CommonMuonHitManager;

  void setHits( CommonMuonHits&& hts );

  std::array<double, 8> m_xboundaries;
  CommonMuonHits        m_hits;

  // NOTE: Iterators are not invalidated after move, although it has
  // been proposed, there is no explicit requirement from the
  // standard. Default move constructors "should" therefore work.
  // See: http://www.open-std.org/JTC1/SC22/WG21/docs/lwg-active.html#2321
  std::vector<typename CommonMuonHits::iterator> m_index; // indices into m_hits

  double       m_z       = 0.;
  double       m_ymax    = 0.;
  unsigned int m_station = 0;
};