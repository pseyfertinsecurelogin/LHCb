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
                   // inputs
                   {
                       KeyValue{"TracksLocation", LHCb::TrackLocation::Default},
                       KeyValue{"MCParticlesLocation", LHCb::MCParticleLocation::Default},
                       KeyValue{"MCParticlesLinkLocation", "Link/" + LHCb::TrackLocation::Default},
                   },
                   // output
                   {KeyValue{"TrackToMCParticlesRelations", Rich::Future::MC::Relations::TrackToMCParticles}} ) {}

Rich::Future::MC::Relations::TkToMCPRels                                  //
TrackToMCParticleRelations::operator()( const LHCb::Tracks&      tks,     //
                                        const LHCb::MCParticles& mcParts, //
                                        const LHCb::LinksByKey&  links    //
                                        ) const {

  // make a relations table
  Relations::TkToMCPRels rels( 2 * tks.size() );

  // loop over the tracks and fill relations table
  for ( auto* tk : tks ) {
    if ( !tk ) continue;

    if ( m_allowMultMPs ) {
      // Save all the MCPs for each track
      links.applyToLinks( tk->key(), [&rels, &tk, &mcParts]( unsigned int, unsigned int tgtKey, float weight ) {
        rels.i_push( tk, static_cast<const LHCb::MCParticle*>( mcParts.containedObject( tgtKey ) ),
                     weight ); // NB! i_push is used!
      } );
    } else {
      // find best MCP based on weight and save only that
      double                 bestWeight = -1;
      const ContainedObject* bestMCP    = nullptr;
      links.applyToLinks( tk->key(),
                          [&bestMCP, &bestWeight, &mcParts]( unsigned int, unsigned int tgtKey, float weight ) {
                            if ( weight > bestWeight ) {
                              bestWeight = weight;
                              bestMCP    = mcParts.containedObject( tgtKey );
                            }
                          } );
      if ( bestMCP ) {
        rels.i_push( tk, static_cast<const LHCb::MCParticle*>( bestMCP ), bestWeight ); // NB! i_push is used!
      }
    }
  }

  if ( msgLevel( MSG::VERBOSE ) ) {
    verbose() << "Rels before sort" << endmsg;
    for ( const auto& i : rels.i_relations() ) {
      verbose() << std::setprecision( 20 ) << "  TK " << i.from()->key() << "  MCP " << i.to()->key() << "  Weight "
                << i.weight() << endmsg;
    }
  }

  // MANDATORY usage of i_sort after i_push
  rels.i_sort();

  if ( msgLevel( MSG::VERBOSE ) ) {
    verbose() << "Rels after sort" << endmsg;
    for ( const auto& i : rels.i_relations() ) {
      verbose() << std::setprecision( 20 ) << "  TK " << i.from()->key() << "  MCP " << i.to()->key() << "  Weight "
                << i.weight() << endmsg;
    }
  }

  // check for some "strange" status
  if ( rels.i_relations().empty() ) { ++m_emptyWarn; }

  // return the final relations
  return rels;
}

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( TrackToMCParticleRelations )
