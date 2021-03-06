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

#include "Event/MCTrackGeomCriteria.h"

#include <algorithm>

LHCb::MC::MCTrackGeomCriteria::MCTrackGeomCriteria( const std::vector<std::string>& criteria ) {
  for ( const auto& S : criteria ) { m_criteria.push_back( theMap().toType( S ) ); }
}

bool LHCb::MC::MCTrackGeomCriteria::accepted( MCTrackInfo& info, const LHCb::MCParticle* aParticle ) const {
  // make the acceptance test....
  return std::all_of( m_criteria.begin(), m_criteria.end(),
                      [&info, &aParticle]( const auto& crit ) { return crit( info, aParticle ); } );
}

LHCb::MC::MCTrackGeomCriteria::FunctionMap::FunctionMap()
    : m_mapping( {{"hasVeloAndT", &MCTrackInfo::hasVeloAndT},
                  {"hasTT", &MCTrackInfo::hasTT},
                  {"hasT", &MCTrackInfo::hasT},
                  {"hasVelo", &MCTrackInfo::hasVelo},
                  {"hasVeloR", &MCTrackInfo::hasVeloR},
                  {"hasVeloPhi", &MCTrackInfo::hasVeloPhi},
                  {"hasTT1", &MCTrackInfo::hasTT1},
                  {"hasTT2", &MCTrackInfo::hasTT2},
                  {"hasT1X", &MCTrackInfo::hasT1X},
                  {"hasT1S", &MCTrackInfo::hasT1S},
                  {"hasT2X", &MCTrackInfo::hasT2X},
                  {"hasT2S", &MCTrackInfo::hasT2S},
                  {"hasT3X", &MCTrackInfo::hasT3X},
                  {"hasT3S", &MCTrackInfo::hasT3S},
                  {"hasT1", &MCTrackInfo::hasT1},
                  {"hasT2", &MCTrackInfo::hasT2},
                  {"hasT3", &MCTrackInfo::hasT3},
                  {"accVeloAndT", &MCTrackInfo::accVeloAndT},
                  {"accTT", &MCTrackInfo::accTT},
                  {"accT", &MCTrackInfo::accT},
                  {"accVelo", &MCTrackInfo::accVelo},
                  {"accVeloR", &MCTrackInfo::accVeloR},
                  {"accVeloPhi", &MCTrackInfo::accVeloPhi},
                  {"accTT1", &MCTrackInfo::accTT1},
                  {"accTT2", &MCTrackInfo::accTT2},
                  {"accT1X", &MCTrackInfo::accT1X},
                  {"accT1S", &MCTrackInfo::accT1S},
                  {"accT2X", &MCTrackInfo::accT2X},
                  {"accT2S", &MCTrackInfo::accT2S},
                  {"accT3X", &MCTrackInfo::accT3X},
                  {"accT3S", &MCTrackInfo::accT3S},
                  {"accT1", &MCTrackInfo::accT1},
                  {"accT2", &MCTrackInfo::accT2},
                  {"accT3", &MCTrackInfo::accT3}} ) {}
