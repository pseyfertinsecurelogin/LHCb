// Include files

// local
#include "VisPrimVertTool.h"

// from Event
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"
#include "Event/MCTrackInfo.h"
#include "Event/MCHeader.h"


//-----------------------------------------------------------------------------
// Implementation file for class : VisPrimVertTool v1.3
//
// 2004-03-18 : Christian Jacoby (LPHE, EPFL, Lausanne)
//-----------------------------------------------------------------------------

DECLARE_TOOL_FACTORY( VisPrimVertTool )


//-----------------------------------------------------------------------------
// Look if b was produced at a collison
//-----------------------------------------------------------------------------

bool VisPrimVertTool::isB( const LHCb::MCVertex* pv ) const {
  if ( !pv->isPrimary() ){
    Error("Input vertex is not a primary").ignore() ;
  }

  LHCb::MCParticles* MCPartCtnr = get<LHCb::MCParticles>(LHCb::MCParticleLocation::Default);

  for ( LHCb::MCParticles::const_iterator MCPiter = MCPartCtnr->begin();
        MCPartCtnr->end() != MCPiter; MCPiter++ ) {
    if ( (*MCPiter)->primaryVertex() == pv ) {
      if ( (*MCPiter)->particleID().hasBottom()  ) return true ;
    }
  }
  return false;
} // isB

//=============================================================================
//-----------------------------------------------------------------------------
// Count number of visible tracks per collision
//-----------------------------------------------------------------------------
long VisPrimVertTool::countVisTracks( const LHCb::MCVertex* pv ) const {
  if ( !pv->isPrimary() ){
    Error("Input vertex is not a primary").ignore() ;
  }

  LHCb::MCParticles* MCPartCtnr = get<LHCb::MCParticles>(LHCb::MCParticleLocation::Default);

  MCTrackInfo trInfo( evtSvc(), msgSvc() );

  long countTracks = 0;

  for ( LHCb::MCParticles::const_iterator MCPiter = MCPartCtnr->begin();
        MCPartCtnr->end() != MCPiter; ++MCPiter) {

    if ( (*MCPiter)->primaryVertex() == pv ) {
      if (m_veloAndSeed) {
        if (trInfo.hasVeloAndT(*MCPiter)){
          ++countTracks;
        }
      } else if (trInfo.hasVelo(*MCPiter)){
        ++countTracks;
      }
    }
  }
  return countTracks;
} // countVisTracks

//=============================================================================
//-----------------------------------------------------------------------------
// Count Visible Primary Vertices
//-----------------------------------------------------------------------------
long VisPrimVertTool::countVertices() const {
  long nVertices = 0; // number of VISIBLE PVs
  LHCb::MCHeader* mch = get<LHCb::MCHeader>(LHCb::MCHeaderLocation::Default);
  for ( SmartRefVector< LHCb::MCVertex >::const_iterator ipv = mch->primaryVertices().begin() ;
        ipv != mch->primaryVertices().end() ; ++ipv ){
    if (isVisible(*ipv)) ++nVertices;
  }
  return nVertices;

} // countVertices
