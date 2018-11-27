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

// local
#include "RichFutureMCUtils/TrackToMCParticle.h"

using namespace Rich::Future::MC::Relations;

const LHCb::MCParticle *
TrackToMCParticle::mcParticle( const LHCb::Track &track, const double minWeight ) const
{
  const LHCb::MCParticle *mcP = nullptr;

  // loop over the found relations and pick the best
  double bestWeight = -1;
  for ( const auto MC : mcParticleRange( track ) )
  {
    if ( MC.to() )
    {
      if ( MC.weight() > minWeight && MC.weight() > bestWeight )
      {
        bestWeight = MC.weight();
        mcP        = MC.to();
      }
    }
  }

  // return the best MCParticle match
  return mcP;
}

LHCb::MCParticle::ConstVector
TrackToMCParticle::mcParticles( const LHCb::Track &track,
                                const bool         includeNull,
                                const double       minWeight ) const
{
  // Get the MCParticle range for this track
  const auto mcPR = mcParticleRange( track );
  // MCPs to return
  LHCb::MCParticle::ConstVector mcPs;
  mcPs.reserve( mcPR.empty() ? 1 : mcPR.size() );
  // select MCPs to return
  for ( const auto MC : mcPR )
  {
    if ( MC.weight() > minWeight ) { mcPs.push_back( MC.to() ); }
  }
  // if none selected, add a nullptr
  if ( includeNull && mcPs.empty() ) { mcPs.push_back( nullptr ); }
  // return
  return mcPs;
}

Rich::ParticleIDType
TrackToMCParticle::mcParticleType( const LHCb::Track &track, const double minWeight ) const
{
  // get the MCParticle
  const auto mcP = mcParticle( track, minWeight );
  // return the type
  return mcParticleType( mcP );
}

Rich::ParticleIDType
TrackToMCParticle::mcParticleType( const LHCb::MCParticle *mcP ) const
{
  // get the PDG code for this particle
  const auto id = ( mcP ? abs( mcP->particleID().pid() ) : 0 );
  // map to RICH type. Hardcode PDG codes here instead of using
  // the particle properties service. Probably safe !
  return ( 11 == id ? Rich::Electron :
                      13 == id ?
                      Rich::Muon :
                      211 == id ?
                      Rich::Pion :
                      321 == id ? Rich::Kaon :
                                  2212 == id ? Rich::Proton :
                                               1000010020 == id ? Rich::Deuteron : Rich::Unknown );
}
