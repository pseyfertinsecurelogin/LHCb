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
#include "Event/MCHeader.h"
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"
#include "GaudiAlg/Consumer.h"
#include "Kernel/ICheckTool.h"

/** @class DumpMCEventAlg DumpMCEventAlg.h
 *  Dump contents of an MCEvent.
 *  Reimplements functionality previously in DumpEventExample
 *  Amount printed depends on OutputLevel:
 *    INFO: prints EventHeader and Collisions
 *   DEBUG: prints also first "NumberOfObjectsToPrint" MCVertices and
 *          MCParticles (default is 5)
 * VERBOSE: prints all MCVertices and MCParticles
 *
 *  @author Marco Cattaneo
 *  @date   2004-07-14
 */
class DumpMCEventAlg : public Gaudi::Functional::Consumer<void(
                           const LHCb::MCHeader& evt, const LHCb::MCVertices& verts, const LHCb::MCParticles& parts )> {
public:
  /// Standard constructor
  DumpMCEventAlg( const std::string& name, ISvcLocator* pSvcLocator );

  void operator()( const LHCb::MCHeader& evt, const LHCb::MCVertices& verts,
                   const LHCb::MCParticles& parts ) const override;

private:
  Gaudi::Property<unsigned int> m_numObjects{this, "NumberOfObjectsToPrint", 5};  ///< Number of objects to print
  PublicToolHandle<ICheckTool>  m_checker{this, "CheckTool", "CheckMCEventTool"}; ///< Tool to check event integrity
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( DumpMCEventAlg )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
DumpMCEventAlg::DumpMCEventAlg( const std::string& name, ISvcLocator* pSvcLocator )
    : Consumer( name, pSvcLocator,
                {KeyValue{"MCHeader", LHCb::MCHeaderLocation::Default},
                 KeyValue{"MCVertices", LHCb::MCVertexLocation::Default},
                 KeyValue{"MCParticles", LHCb::MCParticleLocation::Default}} ) {}

//=============================================================================
// Main execution
//=============================================================================
void DumpMCEventAlg::operator()( const LHCb::MCHeader& evt, const LHCb::MCVertices& verts,
                                 const LHCb::MCParticles& parts ) const {

  info() << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n" << evt << endmsg;

  m_checker->check().orThrow( "Checker Failed", "DumpMCEventAlg" ).ignore();
  info() << "MCVertex/MCParticle tree structure is OK" << endmsg;

  info() << "There are " << verts.size() << " Vertices:" << endmsg;
  if ( 0 < m_numObjects && msgLevel( MSG::DEBUG ) ) {
    unsigned int count = 0;
    for ( const auto& iVert : verts ) {
      if ( !msgLevel( MSG::VERBOSE ) && m_numObjects < ++count ) break;
      debug() << "MCVertex " << iVert->key() << ":\n";
      iVert->fillStream( debug().stream() );
      debug() << endmsg;
    }
  }

  info() << "There are " << parts.size() << " Particles:" << endmsg;
  if ( 0 < m_numObjects && msgLevel( MSG::DEBUG ) ) {
    unsigned int count = 0;
    for ( const auto& iPart : parts ) {
      if ( !msgLevel( MSG::VERBOSE ) && m_numObjects < ++count ) break;
      debug() << "MCParticle " << iPart->key() << ":" << std::endl;
      iPart->fillStream( debug().stream() );
      debug() << endmsg;
    }
  }
}

//=============================================================================
