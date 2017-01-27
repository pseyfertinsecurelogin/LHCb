
// local
#include "RichFutureMCUtils/RichRecMCHelper.h"

using namespace Rich::Future::Rec::MC;

const LHCb::MCParticle * 
Helper::trueRecPhoton( const LHCb::Track & track,
                       const Rich::PDPixelCluster& cluster ) const
{
  // get the MCP for the track
  const auto trackMCP = mcParticle(track);

  // If no track association, do not bother with pixels
  return ( trackMCP ? trueRecPhoton( *trackMCP, cluster ) : nullptr );
}

const LHCb::MCParticle * 
Helper::trueRecPhoton( const LHCb::MCParticle & mcPart,
                       const Rich::PDPixelCluster& cluster ) const
{
  // loop over the channels in the cluster
  for ( const auto S : cluster.smartIDs() )
  {
    const auto * mcP = trueRecPhoton( mcPart, S );
    if ( nullptr != mcP ) return mcP;
  }
  return nullptr;
}

const LHCb::MCParticle * 
Helper::trueRecPhoton( const LHCb::MCParticle & mcPart,
                       const LHCb::RichSmartID id ) const
{
  // Get MCParticles for the channel
  const auto mcParts = mcParticles(id);

  // Loop over all MCParticles associated to the pixel
  for ( const auto * MCP : mcParts )
  {
    if ( &mcPart == MCP ) return MCP;
  }
  
  // if get here no association
  return nullptr;
}

const LHCb::MCParticle *
Helper::trueCherenkovPhoton( const LHCb::Track & track,
                             const Rich::RadiatorType rad,
                             const Rich::PDPixelCluster& cluster ) const
{
  const auto * mcP = trueRecPhoton( track, cluster );
  return ( !mcP ? nullptr : trueCherenkovRadiation(cluster,rad) );
}
