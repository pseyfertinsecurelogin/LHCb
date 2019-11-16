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

// STD
#include <algorithm>
#include <cassert>
#include <iostream>

// Event model
#include "Event/RecVertex.h"

// Gaudi
#include "GaudiKernel/IRegistry.h"

//-----------------------------------------------------------------------------
// Implementation file for class : RecVertex
//
// 2012-02-17 : Chris Jones
//-----------------------------------------------------------------------------

void LHCb::RecVertex::removeFromTracks( const LHCb::Track* track ) {
  // Possible to have tracks but no weights, if the vertex was persisted
  // before the weights where added, without packing. If this happens, give
  // each track a default weight of 1
  if ( !m_tracks.empty() && m_weights.empty() ) { m_weights.resize( m_tracks.size(), 1.0 ); }

  // At this point, the two vectors *must* have the same size
  assert( m_tracks.size() == m_weights.size() );

  // If we have tracks, look for the one to remove
  if ( !m_tracks.empty() ) {
    // Does the track vector have this track in it ?
    const SmartRef<LHCb::Track> tkRef( track );
    const auto                  iTk = std::find( m_tracks.begin(), m_tracks.end(), tkRef );
    if ( iTk != m_tracks.end() ) {
      // Get the index
      const auto index = (unsigned int)( iTk - m_tracks.begin() );
      // remove the track
      m_tracks.erase( iTk );
      // remove the weight
      m_weights.erase( m_weights.begin() + index );
    }
  }
}

LHCb::RecVertex::TrackWithWeightVector LHCb::RecVertex::tracksWithWeights() const {
  TrackWithWeightVector twV;
  twV.reserve( m_tracks.size() );
  auto iW = m_weights.begin();
  for ( auto iTk = m_tracks.begin(); iTk != m_tracks.end() && iW != m_weights.end(); ++iTk, ++iW ) {
    twV.emplace_back( iTk->target(), *iW );
  }
  return twV;
}

void LHCb::RecVertex::setTracksWithWeights( const TrackWithWeightVector& tracksAndWeights ) {
  // Clear the current vectors
  clearTracks();

  // Loop over the tracks and weights and store them
  for ( const auto& TW : tracksAndWeights ) { addToTracks( TW.first, TW.second ); }
}

bool LHCb::RecVertex::setTrackWeight( const LHCb::Track* track, const float weight ) {
  // Possible to have tracks but no weights, if the vertex was persisted
  // before the weights where added, without packing. If this happens, give
  // each track a default weight of 1
  if ( !m_tracks.empty() && m_weights.empty() ) { m_weights = std::vector<float>( m_tracks.size(), 1.0 ); }

  // At this point, the two vectors *must* have the same size
  assert( m_tracks.size() == m_weights.size() );

  bool OK = false;
  if ( !m_tracks.empty() ) {
    // Does the track vector have this track in it ?
    const SmartRef<LHCb::Track> tkRef( track );
    const auto                  iTk = std::find( m_tracks.begin(), m_tracks.end(), tkRef );
    if ( iTk != m_tracks.end() ) {
      // Get the index from the iterator
      const auto index = (unsigned int)( iTk - m_tracks.begin() );
      // Set the weight
      m_weights[index] = weight;
      // Set the return status to true
      OK = true;
    }
  }
  return OK;
}

std::pair<bool, float> LHCb::RecVertex::trackWeight( const LHCb::Track* track ) const {
  std::pair<bool, float> weight( false, 0.0 );

  // Find the weight for this track
  if ( !m_tracks.empty() ) {
    // Try and find the track
    const SmartRef<LHCb::Track> tkRef( track );
    const auto                  iTk = std::find( m_tracks.begin(), m_tracks.end(), tkRef );
    if ( iTk != m_tracks.end() ) {
      // Get the index from the iterator
      const auto index = (unsigned int)( iTk - m_tracks.begin() );
      // Get the weight
      weight.second = ( m_tracks.size() == m_weights.size() ? m_weights[index] : 1.0 );
      weight.first  = true;
    }
  }
  return weight;
}

std::ostream& LHCb::RecVertex::fillStream( std::ostream& s ) const {
  VertexBase::fillStream( s );
  s << "{ "
    << "technique :	" << technique() << std::endl
    << " tracks :	[ ";
  for ( const auto& tk : tracks() ) { s << tk << " "; }
  s << "]" << std::endl;
  s << "weights : [ ";
  for ( const auto& w : weights() ) { s << w << " "; }
  s << "]" << std::endl;
  const std::string testLocation = ( parent() && parent()->registry() ? parent()->registry()->identifier() : "" );
  if ( !testLocation.empty() ) { s << "TES=" << testLocation; }
  return s << " }";
}

bool LHCb::RecVertex::isPrimary() const { return RecVertexType::Primary == technique(); }

LHCb::RecVertex* LHCb::RecVertex::clone() const { return new RecVertex( *this ); }
