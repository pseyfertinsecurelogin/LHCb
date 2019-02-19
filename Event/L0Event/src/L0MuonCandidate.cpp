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
#include "Event/L0MuonCandidate.h"
#include <cmath>
#include <vector>

LHCb::L0MuonCandidate::L0MuonCandidate() : m_pt( 0.0 ), m_theta( 0.0 ), m_phi( 0.0 ), m_encodedPt( 0 ) {}

LHCb::L0MuonCandidate::L0MuonCandidate( double pt, double theta, double phi, const std::vector<LHCb::MuonTileID>& pads,
                                        int encodedPt ) {
  m_pt        = pt;
  m_theta     = theta;
  m_phi       = phi;
  m_pads      = pads;
  m_encodedPt = encodedPt;
}

std::vector<LHCb::MuonTileID> LHCb::L0MuonCandidate::muonTileIDs( unsigned int station ) const {
  return station < m_pads.size() ? m_pads[station].layout().tiles( m_pads[station] ) : std::vector<LHCb::MuonTileID>{};
}

bool LHCb::L0MuonCandidate::operator<( const LHCb::L0MuonCandidate& lmc ) const { return pt() < lmc.pt(); }

bool LHCb::L0MuonCandidate::operator==( const LHCb::L0MuonCandidate& lmc ) const {

  if ( encodedPt() != lmc.encodedPt() ) return false;
  for ( int sta = 0; sta < 3; ++sta ) {
    if ( muonTileIDs( sta ) != lmc.muonTileIDs( sta ) ) return false;
  }
  return true;
}
