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

// from PartProp
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"
// from LHCb
#include "MCInterfaces/IPrintMCDecayTreeTool.h"

// local
#include "PrintMCTree.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PrintMCTree
//
// 2004-09-10 : Patrick KOPPENBURG
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory

DECLARE_COMPONENT( PrintMCTree )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PrintMCTree::PrintMCTree( const std::string& name,
                          ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "ParticleNames", m_particleNames );
  declareProperty( "Depth", m_depth );
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode PrintMCTree::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Initialize" << endmsg;

  if (m_particleNames.empty()){
    fatal() << "You need to give a list of particles" << endmsg;
    return StatusCode::FAILURE ;
  }

  m_printMCTree = tool<IPrintMCDecayTreeTool>( "PrintMCDecayTreeTool", this );

  auto ppSvc = service<LHCb::IParticlePropertySvc>("LHCb::ParticlePropertySvc");

  for (auto PN=m_particleNames.begin() ; PN!=m_particleNames.end() ; ++PN ){
    const LHCb::ParticleProperty *pp = ppSvc->find(*PN);
    if (!pp) {
      fatal() << " Unable to retrieve particle property for "
              << *PN << endmsg;
      return StatusCode::FAILURE;
    }
    m_particleIDs.push_back(pp->pid().pid());
  }

  info() << "Will print tree for MC particles " << m_particleNames << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PrintMCTree::execute() {

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute" << endmsg;

  LHCb::MCParticles* kmcparts = get<LHCb::MCParticles>(LHCb::MCParticleLocation::Default );

  std::vector<LHCb::MCParticle*> mcparts(kmcparts->begin(), kmcparts->end());
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << "There are " <<  mcparts.size() << " MC particles" << endmsg;

  bool printed = false ;
  for (auto MCP=mcparts.begin() ; MCP!=mcparts.end() ; ++MCP ){
    int pid = (*MCP)->particleID().pid() ;
    if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
      verbose() << "MC Particle is a " << pid << endmsg ;
    for (auto PID=m_particleIDs.begin() ; PID!=m_particleIDs.end() ; ++PID ){
      if ( pid==(*PID)) {
        info() << "Printing MC tree for particle with ID " << pid << endmsg ;
        m_printMCTree->printTree( (*MCP), m_depth ) ;
        printed = true ;
        break ;
      }
    }
  }

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    if (!printed) debug() << "No MC particles found to print in a tree" << endmsg;

  setFilterPassed(printed);

  return StatusCode::SUCCESS;
}

//=============================================================================
