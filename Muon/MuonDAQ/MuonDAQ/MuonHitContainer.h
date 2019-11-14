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

#include <array>
#include <string>
#include <vector>

#include "Event/MuonCoord.h"
#include "MuonDAQ/CommonMuonHit.h"
#include "MuonDAQ/CommonMuonStation.h"
#include "MuonDet/DeMuonDetector.h"

namespace MuonHitContainerLocation {
  inline const std::string Default = "Muon/MuonHits";
}

class MuonHitContainer final {
public:
  MuonHitContainer( std::array<CommonMuonStation, 4> stations ) : m_stations( std::move( stations ) ) {}
  MuonHitContainer( std::array<CommonMuonStation, 5> stations ) { // TODO: this should not be called in upgrade
                                                                  // conditions
    for ( int s = 0; s < 4; s++ ) { m_stations[s] = stations[s]; }
  }

  CommonMuonHitRange hits( unsigned int station ) const { return m_stations[station].hits(); }

  const CommonMuonStation& station( unsigned int id ) const { return m_stations[id]; };

private:
  // data object stored in TES and to get from algorithms using muon coords
  std::array<CommonMuonStation, 4> m_stations;
};
