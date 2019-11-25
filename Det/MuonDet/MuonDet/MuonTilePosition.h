/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

#include "Kernel/STLExtensions.h"
#include "MuonDet/DeMuonDetector.h"
#include "boost/container/static_vector.hpp"
#include <array>
#include <vector>

namespace LHCb::Muon {

  class ComputeTilePosition {
    span<const unsigned int> m_padGridX;
    span<const unsigned int> m_padGridY;

    span<const unsigned int> m_stripXGridX;
    span<const unsigned int> m_stripXGridY;
    span<const unsigned int> m_stripYGridX;
    span<const unsigned int> m_stripYGridY;

    std::array<unsigned int, 20> m_stripYOffset;
    std::array<unsigned int, 20> m_stripXOffset;

    std::array<std::vector<Gaudi::XYZPoint>, 5> m_posPad; // TODO: make this a _single_ vector instead, use XYZPointF
    std::array<std::vector<Gaudi::XYZPoint>, 5> m_posStripX;
    std::array<std::vector<Gaudi::XYZPoint>, 5> m_posStripY;

    boost::container::static_vector<float, 20> m_padSizeX;
    boost::container::static_vector<float, 20> m_padSizeY;
    boost::container::static_vector<float, 20> m_stripYSizeX;
    boost::container::static_vector<float, 20> m_stripXSizeY;
    boost::container::static_vector<float, 20> m_stripXSizeX;
    boost::container::static_vector<float, 20> m_stripYSizeY;

  public:
    ComputeTilePosition( const DeMuonDetector& det );

    struct Result {
      Gaudi::XYZPoint p;
      double          dX, dY;
    };

    Result tilePosition( LHCb::MuonTileID tile ) const {
      const int    station    = tile.station();
      const int    s4r        = station * 4 + tile.region();
      const int    perQuarter = 3 * m_padGridX[station] * m_padGridY[station];
      unsigned int index      = ( tile.region() * 4 + tile.quarter() ) * perQuarter;
      index += ( tile.nY() < m_padGridY[station] )
                   ? ( m_padGridX[station] * tile.nY() + tile.nX() - m_padGridX[station] )
                   : ( m_padGridX[station] * m_padGridY[station] +
                       2 * m_padGridX[station] * ( tile.nY() - m_padGridY[station] ) + tile.nX() );

      return {m_posPad[station][index], m_padSizeX[s4r], m_padSizeY[s4r]};
    };

    Result stripXPosition( LHCb::MuonTileID tile ) const {
      const int    station    = tile.station();
      const int    s4r        = station * 4 + tile.region();
      const int    perQuarter = 3 * m_stripXGridX[s4r] * m_stripXGridY[s4r];
      unsigned int index      = m_stripXOffset[s4r] + tile.quarter() * perQuarter;
      index += ( tile.nY() < m_stripXGridY[s4r] )
                   ? ( m_stripXGridX[s4r] * tile.nY() + tile.nX() - m_stripXGridX[s4r] )
                   : ( m_stripXGridX[s4r] * m_stripXGridY[s4r] +
                       2 * m_stripXGridX[s4r] * ( tile.nY() - m_stripXGridY[s4r] ) + tile.nX() );

      return {m_posStripX[station][index], m_stripXSizeX[s4r], m_stripXSizeY[s4r]};
    }

    Result stripYPosition( LHCb::MuonTileID tile ) const {
      const int    station    = tile.station();
      const int    s4r        = station * 4 + tile.region();
      const int    perQuarter = 3 * m_stripYGridX[s4r] * m_stripYGridY[s4r];
      unsigned int index      = m_stripYOffset[s4r] + tile.quarter() * perQuarter;
      index += ( tile.nY() < m_stripYGridY[s4r] )
                   ? ( m_stripYGridX[s4r] * tile.nY() + tile.nX() - m_stripYGridX[s4r] )
                   : ( m_stripYGridX[s4r] * m_stripYGridY[s4r] +
                       2 * m_stripYGridX[s4r] * ( tile.nY() - m_stripYGridY[s4r] ) + tile.nX() );

      return {m_posStripY[station][index], m_stripYSizeX[s4r], m_stripYSizeY[s4r]};
    }
  };
} // namespace LHCb::Muon
