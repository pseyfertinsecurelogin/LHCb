
// STL
#include <algorithm>
#include <vector>

#include "Event/MCParticle.h"
#include "Kernel/RichRadiatorType.h"
#include "Kernel/RichSmartID.h"
#include "Relations/Range.h"
#include "Relations/WEntry.h"
// local
#include "RichFutureMCUtils/RichRecMCHelper.h"
#include "RichUtils/RichPixelCluster.h"

namespace LHCb {
class Track;
}  // namespace LHCb

using namespace Rich::Future::Rec::MC;

LHCb::MCParticle::ConstVector  
Helper::trueRecPhoton( const LHCb::Track & track,
                       const Rich::PDPixelCluster& cluster ) const
{
  // get the MCP range for the track
  const auto tkMCPs = mcParticleRange(track);

  // MCPs to return
  LHCb::MCParticle::ConstVector mcPs;
  mcPs.reserve( tkMCPs.size() );

  // loop over all the MCPs to see if any match the cluster
  for ( const auto tkMCP : tkMCPs )
  {
    const LHCb::MCParticle * tMCP = tkMCP.to();
    const auto MCP = ( tMCP ? trueRecPhoton(*tMCP,cluster) : nullptr );
    // if found, and not already present, add to vector
    if ( MCP && std::find(mcPs.begin(),mcPs.end(),MCP) == mcPs.end() )
    { mcPs.push_back(MCP); }
  }

  // return the final result
  return mcPs;
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

LHCb::MCParticle::ConstVector 
Helper::trueCherenkovPhoton( const LHCb::Track & track,
                             const Rich::RadiatorType rad,
                             const Rich::PDPixelCluster& cluster ) const
{

  // get the MCPs for the track
  const auto tkMCPs = mcParticles( track );

  // true MCPs to return
  LHCb::MCParticle::ConstVector trueMCPs;
  trueMCPs.reserve( tkMCPs.size() );

  // loop over the track MCPs
  for ( const auto tkMCP : tkMCPs )
  {
    if ( !tkMCP ) continue;
    // loop over cluster smartIDs
    for ( const auto S : cluster.smartIDs() )
    {
      // Is this true Cherenkov radiation ?
      if ( trueCherenkovRadiation(S,rad) )
      {
        // get the MCPs for this smartID
        const auto clMCPs = mcParticles(S);
        // loop over the cluster MCPs
        for ( const auto clMCP : clMCPs )
        {
          // Do the hit and track have the same MCP ?
          if ( clMCP == tkMCP )
          { 
            // if not already in the selected list, add it
            if ( std::find( trueMCPs.begin(), trueMCPs.end(), clMCP ) == trueMCPs.end() )
            {
              trueMCPs.push_back(clMCP);
            }
          }
        }
      }
    }
  }

  return trueMCPs;
}
