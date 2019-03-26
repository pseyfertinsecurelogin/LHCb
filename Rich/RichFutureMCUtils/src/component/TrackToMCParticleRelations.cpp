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
// local
#include "TrackToMCParticleRelations.h"

using namespace Rich::Future::MC;

//-----------------------------------------------------------------------------
// Implementation file for class : TrackToMCParticleRelations
//
// 2016-12-07 : Chris Jones
//-----------------------------------------------------------------------------

TrackToMCParticleRelations::TrackToMCParticleRelations( const std::string& name, ISvcLocator* pSvcLocator )
    : Transformer( name, pSvcLocator,
                   {KeyValue{"TracksLocation", LHCb::TrackLocation::Default},
                    KeyValue{"MCParticlesLocation", LHCb::MCParticleLocation::Default}},
                   {KeyValue{"TrackToMCParticlesRelations", Rich::Future::MC::Relations::TrackToMCParticles}} ) {
  // debugging
  // setProperty( "OutputLevel", MSG::VERBOSE );
}

Rich::Future::MC::Relations::TkToMCPRels TrackToMCParticleRelations::operator()( const LHCb::Tracks& tks,
                                                                                 const LHCb::MCParticles& ) const {
  // make a relations table
  Relations::TkToMCPRels rels( 2 * tks.size() );

  // The location in the TES for the tracks we have been given
  const auto& tkLoc = inputLocation<0>();

  // Load the linker for the given tracks
  // Note this internally loads data from the TES
  LinkedTo<LHCb::MCParticle, LHCb::Track> linker( evtSvc(), msgSvc(), tkLoc );
  if ( UNLIKELY( linker.notFound() ) ) {
    Warning( "The linker table for '" + tkLoc + "' is not found!" ).ignore();
  } else {

    // loop over the tracks and fill relations table
    for ( auto* tk : tks ) {
      if ( !tk ) continue;

      if ( m_allowMultMPs ) {
        // Save all the MCPs for each track
        const auto* mcp = linker.first( tk );
        while ( mcp ) {
          _ri_verbo << std::setprecision( 20 ) << "Track " << tk->key() << " MCP " << mcp->key() << " weight "
                    << linker.weight() << endmsg;
          rels.i_push( tk, mcp, linker.weight() ); // NB! i_push is used!
          mcp = linker.next();
        }
      } else {
        // find best MCP based on weight and save only that
        double                  bestWeight = -1;
        const LHCb::MCParticle* bestMCP    = nullptr;
        const auto*             mcp        = linker.first( tk );
        while ( mcp ) {
          if ( linker.weight() > bestWeight ) {
            bestWeight = linker.weight();
            bestMCP    = mcp;
          }
          mcp = linker.next();
        }
        if ( bestMCP ) {
          _ri_verbo << std::setprecision( 20 ) << "Track " << tk->key() << " MCP " << bestMCP->key() << " weight "
                    << bestWeight << endmsg;
          rels.i_push( tk, bestMCP, bestWeight ); // NB! i_push is used!
        }
      }
    }

    if ( msgLevel( MSG::DEBUG ) ) {
      verbose() << "Rels before sort" << endmsg;
      for ( const auto& i : rels.i_relations() ) {
        verbose() << std::setprecision( 20 ) << "  TK " << i.from()->key() << "  MCP " << i.to()->key() << "  Weight "
                  << i.weight() << endmsg;
      }
    }

    // MANDATORY usage of i_sort after i_push
    rels.i_sort();

    if ( msgLevel( MSG::DEBUG ) ) {
      verbose() << "Rels after sort" << endmsg;
      for ( const auto& i : rels.i_relations() ) {
        verbose() << std::setprecision( 20 ) << "  TK " << i.from()->key() << "  MCP " << i.to()->key() << "  Weight "
                  << i.weight() << endmsg;
      }
    }
  }

  // check for some "strange" status
  if ( rels.i_relations().empty() ) { Warning( "Empty relation table!" ).ignore(); }

  // return the final relations
  return rels;
}

//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( TrackToMCParticleRelations )

//-----------------------------------------------------------------------------
