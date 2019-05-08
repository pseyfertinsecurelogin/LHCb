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
#include "Event/RecVertex_v3.h"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <range/v3/view.hpp>

std::ostream& LHCb::Event::v3::RecVertex::fillStream( std::ostream& s ) const {
  s << "{ "
    << "position  : " << m_position << "\n"
    << "covMatrix : \n"
    << m_covMatrix << "\n"
    << "chi2PerDoF: " << chi2PerDoF() << "\n"
    << "nDoF      : " << nDoF() << "\n"
    << "technique :	" << technique() << "\n"
    << "tracks/weight :	[ ";
  for ( const auto& tk : tracks() ) { s << tk.track << "/" << tk.weight << " "; }
  return s << "]\n }";
}

bool LHCb::Event::v3::RecVertex::removeFromTracks( std::size_t track ) {
  auto it = std::find_if( begin( m_tracks ), end( m_tracks ), [track]( auto& wt ) { return wt.track == track; } );
  if ( it == end( m_tracks ) ) return false;
  m_tracks.erase( it );
  return true;
}

std::optional<float> LHCb::Event::v3::RecVertex::trackWeight( std::size_t track ) const {
  auto it = std::find_if( begin( m_tracks ), end( m_tracks ), [track]( auto& wt ) { return wt.track == track; } );
  if ( it != end( m_tracks ) ) { return it->weight; }
  return {};
}