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

DECLARE_COMPONENT( VisPrimVertTool )


//-----------------------------------------------------------------------------
// Look if b was produced at a collison
//-----------------------------------------------------------------------------

bool VisPrimVertTool::isB( const LHCb::MCVertex* pv ) const {
  if ( !pv->isPrimary() ) Error("Input vertex is not a primary").ignore() ;
  LHCb::MCParticles* MCPartCtnr = get<LHCb::MCParticles>(LHCb::MCParticleLocation::Default);
  return std::any_of(  MCPartCtnr->begin(), MCPartCtnr->end(),
                       [&](const LHCb::MCParticle* p) {
                           return p->primaryVertex()==pv && p->particleID().hasBottom();
                       });
} // isB

//=============================================================================
//-----------------------------------------------------------------------------
// Count number of visible tracks per collision
//-----------------------------------------------------------------------------
long VisPrimVertTool::countVisTracks( const LHCb::MCVertex* pv ) const {
  if ( !pv->isPrimary() ) Error("Input vertex is not a primary").ignore() ;
  LHCb::MCParticles* MCPartCtnr = get<LHCb::MCParticles>(LHCb::MCParticleLocation::Default);
  const MCTrackInfo trInfo = make_MCTrackInfo( evtSvc(), msgSvc() );
  return std::count_if( MCPartCtnr->begin(), MCPartCtnr->end(),
                        [&](const LHCb::MCParticle* p) {
                            if (p->primaryVertex() != pv) return false;
                            if (trInfo.hasVelo(p)) return true;
                            return m_veloAndSeed && trInfo.hasVeloAndT(p) ;
                        } );
} // countVisTracks

//=============================================================================
//-----------------------------------------------------------------------------
// Count Visible Primary Vertices
//-----------------------------------------------------------------------------
long VisPrimVertTool::countVertices() const {
  LHCb::MCHeader* mch = get<LHCb::MCHeader>(LHCb::MCHeaderLocation::Default);
  return std::count_if( mch->primaryVertices().begin(), mch->primaryVertices().end(),
                        [&](const LHCb::MCVertex* pv) { return isVisible(pv); } );

} // countVertices
