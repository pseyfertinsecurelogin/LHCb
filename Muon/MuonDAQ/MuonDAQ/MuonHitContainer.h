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
#ifndef MUONHITCONTAINER_H
#define MUONHITCONTAINER_H

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

  // access hit
  CommonMuonHitRange hits( double xmin, unsigned int station, unsigned int region ) const {
    return m_stations[station].hits( xmin, region );
  }
  CommonMuonHitRange hits( double xmin, double xmax, unsigned int station, unsigned int region ) const {
    return m_stations[station].hits( xmin, xmax, region );
  }

  unsigned int nRegions( unsigned int station ) const { return m_stations[station].nRegions(); };

  const CommonMuonStation& station( unsigned int id ) const { return m_stations[id]; };

private:
  // data object stored in TES and to get from algorithms using muon coords
  std::array<CommonMuonStation, 4> m_stations;
};

#endif // MUONHITCONTAINER_H
