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
// Tools
#include "Kernel/ICheckTool.h"

// Event Model
#include "Event/MCHeader.h"
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"

// local
#include "DumpMCEventAlg.h"

//-----------------------------------------------------------------------------
// Implementation file for class : DumpMCEventAlg
//
// 2004-07-14 : Marco Cattaneo
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( DumpMCEventAlg )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
DumpMCEventAlg::DumpMCEventAlg( const std::string& name,
                                ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "NumberOfObjectsToPrint", m_numObjects = 5 );
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode DumpMCEventAlg::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  m_checker = tool<ICheckTool>( "CheckMCEventTool" );

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode DumpMCEventAlg::execute() {

  LHCb::MCHeader* evt =
    get<LHCb::MCHeader>( LHCb::MCHeaderLocation::Default );
  info() << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
         << *evt << endmsg;

  StatusCode sc = m_checker->check();
  if( sc.isFailure() ) return sc;
  info() << "MCVertex/MCParticle tree structure is OK" << endmsg;

  LHCb::MCVertices* verts =
    get<LHCb::MCVertices>( LHCb::MCVertexLocation::Default );
  info() << "There are " << verts->size() << " Vertices:" << endmsg;

  if( 0 < m_numObjects && msgLevel( MSG::DEBUG ) ) {
    unsigned int count = 0;
    for( auto iVert = verts->begin(); iVert != verts->end(); iVert++ ) {
      if( !msgLevel(MSG::VERBOSE) && m_numObjects < ++count ) break;
      debug() << "MCVertex " << (*iVert)->key() << ":" << std::endl;
      (*iVert)->fillStream(debug().stream());
      debug() << endmsg;
    }
  }

  LHCb::MCParticles* parts =
    get<LHCb::MCParticles>( LHCb::MCParticleLocation::Default );
  info() << "There are " << parts->size() << " Particles:" << endmsg;

  if( 0 < m_numObjects && msgLevel( MSG::DEBUG ) ) {
    unsigned int count = 0;
    for(auto iPart = parts->begin(); iPart != parts->end(); iPart++ ) {
      if( !msgLevel(MSG::VERBOSE) && m_numObjects < ++count ) break;
      debug() << "MCParticle " << (*iPart)->key() << ":" << std::endl;
      (*iPart)->fillStream(debug().stream());
      debug() << endmsg;
    }
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
