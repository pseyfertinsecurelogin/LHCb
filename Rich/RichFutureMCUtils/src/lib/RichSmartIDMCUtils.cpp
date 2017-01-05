
// local
#include "RichFutureMCUtils/RichSmartIDMCUtils.h"

// STL
#include <algorithm>

using namespace Rich::Future::MC::Relations;

SmartIDUtils::
SmartIDUtils( const LHCb::MCRichDigitSummarys & histories )
  : m_histories(histories) 
{ 
  // Fill the summary map
  // If this class is used a lot might want to write an algorithm
  // to create the map and save it in the TES, to avoid it being created
  // multiple times each event...
  for ( const auto * sum : histories )
  {
    if ( sum ) { m_sumMap[sum->richSmartID()].push_back( sum ); }
  }
}

LHCb::MCParticle::ConstVector 
SmartIDUtils::mcParticles( const Rich::PDPixelCluster& cluster ) const
{
  LHCb::MCParticle::ConstVector mcParts;

  // Loop over the cluster smart IDs
  for ( const auto& S : cluster.smartIDs() )
  {
    // Get the MCPs for this channel
    const auto tmp_vect = mcParticles(S);
    // reserve space
    mcParts.reserve( mcParts.size() + tmp_vect.size() );
    // save if not already present
    for ( const auto* P : tmp_vect ) 
    {
      if ( std::find( mcParts.begin(), mcParts.end(), P ) == mcParts.end() )
      { mcParts.push_back(P); }
    }
  }

  return mcParts;
}

LHCb::MCParticle::ConstVector 
SmartIDUtils::mcParticles( const LHCb::RichSmartID id ) const
{
   LHCb::MCParticle::ConstVector mcParts;

   // Does this hit have an entry in the pixel summary map
   const auto iEn = m_sumMap.find( id );
   if ( iEn != m_sumMap.end() )
   {
     // Loop over the associated summaries
     for ( const auto* sum : (*iEn).second )
     {
       // Get the MCP
       const auto * mcP = sum->mcParticle();
       // protect against null references
       if ( !mcP ) continue;
       // Add to vector, once per MCParticle
       const auto iFind = std::find( mcParts.begin(), mcParts.end(), mcP );
       if ( mcParts.end() == iFind ) mcParts.push_back( mcP );
     }
   }
   
   return mcParts;
}

const LHCb::MCParticle * 
SmartIDUtils::trueCherenkovRadiation( const LHCb::RichSmartID id,
                                      const Rich::RadiatorType rad ) const
{
  // Test if hit is background
  if ( isBackground(id) ) return nullptr;

  // Test if hit is from correct radiator
  if ( !isCherenkovRadiation(id,rad) ) return nullptr;
  
  // All OK so find correct MCParticle
  const auto mcParts = mcParticles(id);

  // Should do something better than just return first, but what ??
  return ( mcParts.empty() ? nullptr : mcParts.front() );
}

const LHCb::MCParticle * 
SmartIDUtils::trueCherenkovRadiation( const Rich::PDPixelCluster& cluster,
                                      const Rich::RadiatorType rad ) const
{
  for ( const auto S : cluster.smartIDs() )
  {
    const auto * mcP = trueCherenkovRadiation(S,rad);
    if ( nullptr != mcP ) return mcP;
  }
  return nullptr;
  
}

bool SmartIDUtils::isBackground ( const LHCb::RichSmartID id ) const
{
  bool isback = true;

  // try via summary objects
  const auto iEn = m_sumMap.find( id );
  if ( iEn != m_sumMap.end() )
  {
    // returns true if hit is only background (no signal contribution)
    isback = std::none_of( (*iEn).second.begin(), (*iEn).second.end(),
                           []( const auto * sum )
                           { return sum->history().hasSignal(); } );
  }
  // return final flag
  return isback;
}

bool SmartIDUtils::isCherenkovRadiation( const LHCb::RichSmartID id,
                                         const Rich::RadiatorType rad ) const
{
  bool isCKRad = false;

  // first, try via summary objects
  const auto iEn = m_sumMap.find( id );
  if ( iEn != m_sumMap.end() )
  {
    isCKRad = std::any_of( (*iEn).second.begin(), (*iEn).second.end(),
                           [&rad]( const auto * sum )
                           { const auto & code = sum->history();
                             return ( ( Rich::Aerogel == rad && code.aerogelHit() ) ||
                                      ( Rich::C4F10   == rad && code.c4f10Hit()   ) ||
                                      ( Rich::CF4     == rad && code.cf4Hit()     ) ); } );
  }
  // return flag
  return isCKRad;
}

bool SmartIDUtils::isCherenkovRadiation( const Rich::PDPixelCluster& cluster,
                                         const Rich::RadiatorType rad ) const
{
  return std::any_of( cluster.smartIDs().begin(),
                      cluster.smartIDs().end(),
                      [&rad,this]( const auto& S ) 
                      { return this->isCherenkovRadiation(S,rad); } );
}
