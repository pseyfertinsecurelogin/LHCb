/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "Event/RecVertex_v3.h"
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <range/v3/view.hpp>

std::ostream& LHCb::Event::v3::RecVertex::fillStream(std::ostream& s) const {
  s << "{ "
    << "position  : " << m_position << "\n"
    << "covMatrix : \n" << m_covMatrix << "\n"
    << "chi2PerDoF: " << chi2PerDoF() << "\n"
    << "nDoF      : " << nDoF() << "\n"
    << "technique :	" << technique() << "\n"
    << " tracks :	[ ";
  for ( const auto& tk : tracks() ) { s << tk << " "; }
  s << "]" << "\n"
    << "weights : [ ";
  for ( const auto& w : weights() ) { s << w << " "; }
  s << "]\n";
  return s << " }";
}

void LHCb::Event::v3::RecVertex::removeFromTracks(const RecVertex::Track* track) {
  auto it = std::find(begin(m_tracks), end(m_tracks), track);
  if (it != end(m_tracks)) {
    m_tracks.erase(it);
    m_weights.erase(std::next(begin(m_weights), std::distance(begin(m_tracks), it)));
  }
}

bool LHCb::Event::v3::RecVertex::setTrackWeight( const RecVertex::Track* track,
                                                 const float weight ) {
  auto it = std::find(begin(m_tracks), end(m_tracks), track);
  if (it != end(m_tracks)) {
    *std::next(begin(m_weights), std::distance(begin(m_tracks), it)) = weight;
    return true;
  }
  return false;
}

std::optional<float> LHCb::Event::v3::RecVertex::trackWeight( const RecVertex::Track* track ) const
{
  auto it = std::find(begin(m_tracks), end(m_tracks), track);
  if (it != end(m_tracks)) {
    return *std::next(begin(m_weights), std::distance(begin(m_tracks), it));
  }
  return {};
}
