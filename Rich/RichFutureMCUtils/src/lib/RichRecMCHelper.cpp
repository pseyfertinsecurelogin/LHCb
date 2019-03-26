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

// STL
#include <algorithm>

// local
#include "RichFutureMCUtils/RichRecMCHelper.h"

using namespace Rich::Future::Rec::MC;

LHCb::MCParticle::ConstVector Helper::trueRecPhoton( const LHCb::Track&          track,
                                                     const Rich::PDPixelCluster& cluster ) const {
  // get the MCP range for the track
  const auto tkMCPs = mcParticleRange( track );

  // MCPs to return
  LHCb::MCParticle::ConstVector mcPs;
  mcPs.reserve( tkMCPs.size() );

  // loop over all the MCPs to see if any match the cluster
  for ( const auto tkMCP : tkMCPs ) {
    const LHCb::MCParticle* tMCP = tkMCP.to();
    const auto              MCP  = ( tMCP ? trueRecPhoton( *tMCP, cluster ) : nullptr );
    // if found, and not already present, add to vector
    if ( MCP && std::find( mcPs.begin(), mcPs.end(), MCP ) == mcPs.end() ) { mcPs.push_back( MCP ); }
  }

  // return the final result
  return mcPs;
}

const LHCb::MCParticle* Helper::trueRecPhoton( const LHCb::MCParticle&     mcPart,
                                               const Rich::PDPixelCluster& cluster ) const {
  // Primary ID
  auto mcP = trueRecPhoton( mcPart, cluster.primaryID() );
  if ( !mcP ) {
    // now try secondary IDs
    for ( const auto S : cluster.secondaryIDs() ) {
      mcP = trueRecPhoton( mcPart, S );
      if ( mcP ) break;
    }
  }
  // return found pointer
  return mcP;
}

const LHCb::MCParticle* Helper::trueRecPhoton( const LHCb::MCParticle& mcPart, const LHCb::RichSmartID id ) const {
  // Get MCParticles for the channel
  const auto mcParts = mcParticles( id );

  // Loop over all MCParticles associated to the pixel
  for ( const auto* MCP : mcParts ) {
    if ( &mcPart == MCP ) return MCP;
  }

  // if get here no association
  return nullptr;
}

LHCb::MCParticle::ConstVector Helper::trueCherenkovPhoton( const LHCb::Track& track, const Rich::RadiatorType rad,
                                                           const Rich::PDPixelCluster& cluster ) const {

  // get the MCPs for the track
  const auto tkMCPs = mcParticles( track );

  // true MCPs to return
  LHCb::MCParticle::ConstVector trueMCPs;
  trueMCPs.reserve( tkMCPs.size() );

  // save to vector functor
  auto saveMCP = [&]( const auto tkMCP, const auto id ) {
    // Is this true Cherenkov radiation ?
    if ( trueCherenkovRadiation( id, rad ) ) {
      // get the MCPs for this smartID
      const auto clMCPs = mcParticles( id );
      // loop over the cluster MCPs
      for ( const auto clMCP : clMCPs ) {
        // Do the hit and track have the same MCP ?
        if ( clMCP == tkMCP ) {
          // if not already in the selected list, add it
          if ( std::find( trueMCPs.begin(), trueMCPs.end(), clMCP ) == trueMCPs.end() ) { trueMCPs.push_back( clMCP ); }
        }
      }
    }
  };

  // loop over the track MCPs
  for ( const auto tkMCP : tkMCPs ) {
    if ( !tkMCP ) continue;
    // save primary ID
    saveMCP( tkMCP, cluster.primaryID() );
    // loop over secondary IDs
    for ( const auto S : cluster.secondaryIDs() ) { saveMCP( tkMCP, S ); }
  }

  return trueMCPs;
}
