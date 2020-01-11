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
#include "GaudiAlg/FilterPredicate.h"
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"
#include "MCInterfaces/IPrintMCDecayTreeTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PrintMCTree
//
// 2004-09-10 : Patrick KOPPENBURG
//-----------------------------------------------------------------------------

/** @class PrintMCTree PrintMCTree.h
 *
 *  Prints the decay tree of all MC particles with a given PID
 *
 *  @author Patrick KOPPENBURG
 *  @date   2004-09-10
 */
class PrintMCTree : public Gaudi::Functional::FilterPredicate<bool( const LHCb::MCParticles& )> {
public:
  /// Standard constructor
  PrintMCTree( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  bool       operator()( const LHCb::MCParticles& mcparts ) const override;

private:
  IPrintMCDecayTreeTool*                    m_printMCTree = nullptr;
  Gaudi::Property<std::vector<std::string>> m_particleNames{this, "ParticleNames", {}}; ///< particle names
  std::vector<int>                          m_particleIDs;
  Gaudi::Property<int>                      m_depth{this, "Depth", -1}; ///< depth of tree
};

// Declaration of the Algorithm Factory

DECLARE_COMPONENT( PrintMCTree )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PrintMCTree::PrintMCTree( const std::string& name, ISvcLocator* pSvcLocator )
    : FilterPredicate{name, pSvcLocator, {"MCParticles", LHCb::MCParticleLocation::Default}} {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode PrintMCTree::initialize() {
  StatusCode sc = FilterPredicate::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;               // error printed already by GaudiAlgorithm

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Initialize" << endmsg;

  if ( m_particleNames.empty() ) {
    fatal() << "You need to give a list of particles" << endmsg;
    return StatusCode::FAILURE;
  }

  m_printMCTree = tool<IPrintMCDecayTreeTool>( "PrintMCDecayTreeTool", this );

  auto ppSvc = service<LHCb::IParticlePropertySvc>( "LHCb::ParticlePropertySvc" );

  for ( auto PN = m_particleNames.begin(); PN != m_particleNames.end(); ++PN ) {
    const LHCb::ParticleProperty* pp = ppSvc->find( *PN );
    if ( !pp ) {
      fatal() << " Unable to retrieve particle property for " << *PN << endmsg;
      return StatusCode::FAILURE;
    }
    m_particleIDs.push_back( pp->pid().pid() );
  }

  info() << "Will print tree for MC particles " << m_particleNames << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
bool PrintMCTree::operator()( const LHCb::MCParticles& mcparts ) const {

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Execute" << endmsg;
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "There are " << mcparts.size() << " MC particles" << endmsg;

  bool printed = false;
  for ( const auto* MCP : mcparts ) {
    int pid = MCP->particleID().pid();
    if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) verbose() << "MC Particle is a " << pid << endmsg;
    if ( std::find( m_particleIDs.begin(), m_particleIDs.end(), pid ) != m_particleIDs.end() ) {
      info() << "Printing MC tree for particle with ID " << pid << endmsg;
      m_printMCTree->printTree( MCP, m_depth );
      printed = true;
    }
  }

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
    if ( !printed ) debug() << "No MC particles found to print in a tree" << endmsg;

  return printed;
}

//=============================================================================
