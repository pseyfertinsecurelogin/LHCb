
// local
#include "RichFutureMCUtils/TrackToMCParticle.h"

using namespace Rich::Future::MC::Relations;

const LHCb::MCParticle *
TrackToMCParticle::mcParticle( const LHCb::Track & track, 
                               const double minWeight ) const
{
  const LHCb::MCParticle * mcP = nullptr;

  // Get the range for the given track
  const auto mcRange = m_rels.relations( &track );

  // loop over the found relations and pick the best
  double bestWeight = -1;
  for ( const auto MC : mcRange )
  {
    if ( MC.to() )
    {
      if ( MC.weight() > minWeight && MC.weight() > bestWeight )
      {
        bestWeight = MC.weight();
        mcP = MC.to();
      }
    }
  }

  // return the best MCParticle match
  return mcP;
}

Rich::ParticleIDType
TrackToMCParticle::mcParticleType( const LHCb::Track & track,
                                   const double minWeight ) const
{
  // get the MCParticle
  const auto mcP = mcParticle( track, minWeight );

  // get the PDG code for this particle
  const auto id = ( mcP ? abs(mcP->particleID().pid()) : 0 );
  
  // map to RICH type. Hardcode PDG codes here instead of using
  // the particle properties service. Probably safe !
  return ( 11         == id ? Rich::Electron :
           13         == id ? Rich::Muon :
           211        == id ? Rich::Pion :
           321        == id ? Rich::Kaon :
           2212       == id ? Rich::Proton :
           1000010020 == id ? Rich::Deuteron :
           Rich::Unknown );
}
