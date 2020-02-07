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
  using allocator_type = CommonMuonHits::allocator_type;
  CommonMuonStation( allocator_type alloc = {} ) : m_hits{alloc} {}
  CommonMuonStation( const DeMuonDetector& det, unsigned int station, CommonMuonHits hits )
      : m_hits{std::move( hits )}, m_z{det.getStationZ( station )}, m_station{station} {}

  CommonMuonStation( const DeMuonDetector& det, unsigned int station, allocator_type alloc = {} )
      : m_hits{alloc}, m_z{det.getStationZ( station )}, m_station{station} {}

  double z() const { return m_z; }

  unsigned int station() const { return m_station; }

  CommonMuonHitRange hits() const { return {m_hits.begin(), m_hits.end()}; }

  void setHits( CommonMuonHits&& hts ) { m_hits = std::move( hts ); }

private:
  CommonMuonHits m_hits;

  double       m_z       = 0.;
  unsigned int m_station = 0;
};
