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

// from Event
#include "Event/MCParticle.h"

// from MCInterfaces
#include "MCInterfaces/IPrintMCDecayTreeTool.h"

// local
#include "PrintMCDecayTreeAlg.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PrintMCDecayTreeAlg
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( PrintMCDecayTreeAlg )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PrintMCDecayTreeAlg::PrintMCDecayTreeAlg( const std::string& name,
                                          ISvcLocator* pSvcLocator)
: GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "PrintTool", m_printToolName );
  declareProperty( "MCParticleLocation", m_particleLocation );
}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode PrintMCDecayTreeAlg::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Initialise" << endmsg;

  m_printTool = tool<IPrintMCDecayTreeTool>( m_printToolName, this );

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PrintMCDecayTreeAlg::execute() {
  
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) {
    debug() << "==> Execute" << endmsg;
    if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
      verbose() << "Getting MCParticles from " << m_particleLocation << endmsg;
  }

  for( const auto& part : *get<LHCb::MCParticles>( m_particleLocation) ) {
    if( !part->originVertex() || !part->originVertex()->mother()  )
      m_printTool->printTree( part, -1 );
  }
  
  return StatusCode::SUCCESS;
}
//=============================================================================
