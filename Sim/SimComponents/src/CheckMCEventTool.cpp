/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files

// Event model
#include "Event/MCHeader.h"
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"

// local
#include "CheckMCEventTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CheckMCEventTool
//
// 2003-02-24 : Marco Cattaneo
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( CheckMCEventTool )

StatusCode CheckMCEventTool::check()
{
  // If already processed this event, return previous status
  LHCb::MCHeader* evt = get<LHCb::MCHeader>( LHCb::MCHeaderLocation::Default );
  if( m_event == evt->evtNumber() ) return m_status;

  m_event  = evt->evtNumber();
  m_status = StatusCode::SUCCESS;

  // Loop over MCVertices
  LHCb::MCVertices*  mcVerts =
    get<LHCb::MCVertices>( LHCb::MCVertexLocation::Default );
  for(const auto& itV : *mcVerts ) {
    for(const auto& itP : itV->products() ) {
      // Check that mother of daughter corresponds...
      if( !itP->originVertex() ) {
        err() << "Inconsistent mother-daughter relation!!"
              << "  Particle " << itP->key()
              << " is daughter of vertex " << itV->key()
              << " but has no origin vertex" << endmsg;
        m_status = StatusCode::FAILURE;
      } else if( itP->originVertex()->key() != itV->key() ) {
        err() << "Inconsistent mother-daughter relation!!"
              << MSG::ERROR << "  Particle " << itP->key()
              << " is daughter of vertex " << itV->key()
              << " but has vertex " << itP->originVertex()->key()
              << " as mother" << endmsg;
        m_status = StatusCode::FAILURE;
      }
    }
  }
  return m_status;
}

//=============================================================================
