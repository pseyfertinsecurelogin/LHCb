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
// Include files

// local
#include "Event/MCHeader.h"
#include "Event/MCParticle.h"
#include "Event/MCTrackInfo.h"
#include "Event/MCVertex.h"
#include "GaudiAlg/GaudiTool.h"
#include "MCInterfaces/IVisPrimVertTool.h"
#include <string>

/** @class VisPrimVertTool VisPrimVertTool.h VisPrimVertTool.h
 *
 *  v1.3
 *  @author Christian Jacoby (LPHE, EPFL, Lausanne)
 *  @date   2004-03-18
 */
class VisPrimVertTool : public extends<GaudiTool, IVisPrimVertTool> {
public:
  using extends::extends;

  long countVertices() const override;
  bool isVisible( const LHCb::MCVertex* pv ) const override { return ( countVisTracks( pv ) >= m_nTracks ); }
  bool isB( const LHCb::MCVertex* ) const override;
  long countVisTracks( const LHCb::MCVertex* ) const override;

private:
  // Number of tracks defining a PV: m_nTracks, by default= 2
  Gaudi::Property<long>                   m_nTracks{this, "TrackNum", 2};
  Gaudi::Property<bool>                   m_veloAndSeed{this, "VeloAndSeed", true};
  DataObjectReadHandle<LHCb::MCParticles> m_mcParts{this, "MCParticles", LHCb::MCParticleLocation::Default};
  DataObjectReadHandle<LHCb::MCHeader>    m_mcHeader{this, "MCHeader", LHCb::MCHeaderLocation::Default};
};

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
  if ( !pv->isPrimary() ) Error( "Input vertex is not a primary" ).ignore();
  const LHCb::MCParticles* MCPartCtnr = m_mcParts.get();
  return std::any_of( MCPartCtnr->begin(), MCPartCtnr->end(), [&]( const LHCb::MCParticle* p ) {
    return p->primaryVertex() == pv && p->particleID().hasBottom();
  } );
} // isB

//=============================================================================
//-----------------------------------------------------------------------------
// Count number of visible tracks per collision
//-----------------------------------------------------------------------------
long VisPrimVertTool::countVisTracks( const LHCb::MCVertex* pv ) const {
  if ( !pv->isPrimary() ) Error( "Input vertex is not a primary" ).ignore();
  const LHCb::MCParticles* MCPartCtnr = m_mcParts.get();
  const MCTrackInfo        trInfo     = make_MCTrackInfo( evtSvc(), msgSvc() );
  return std::count_if( MCPartCtnr->begin(), MCPartCtnr->end(), [&]( const LHCb::MCParticle* p ) {
    if ( p->primaryVertex() != pv ) return false;
    return m_veloAndSeed ? trInfo.hasVeloAndT( p ) : trInfo.hasVelo( p );
  } );
} // countVisTracks

//=============================================================================
//-----------------------------------------------------------------------------
// Count Visible Primary Vertices
//-----------------------------------------------------------------------------
long VisPrimVertTool::countVertices() const {
  const LHCb::MCHeader* mch = m_mcHeader.get();
  return std::count_if( mch->primaryVertices().begin(), mch->primaryVertices().end(),
                        [&]( const LHCb::MCVertex* pv ) { return isVisible( pv ); } );

} // countVertices
