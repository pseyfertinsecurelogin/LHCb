
// local
#include "TrackToMCParticleRelations.h"

using namespace Rich::Future::MC;

//-----------------------------------------------------------------------------
// Implementation file for class : TrackToMCParticleRelations
//
// 2016-12-07 : Chris Jones
//-----------------------------------------------------------------------------

TrackToMCParticleRelations::
TrackToMCParticleRelations( const std::string& name, ISvcLocator* pSvcLocator )
  : Transformer ( name, pSvcLocator,
                  { KeyValue{ "TracksLocation",      LHCb::TrackLocation::Default },
                    KeyValue{ "MCParticlesLocation", LHCb::MCParticleLocation::Default } },
                  { KeyValue{ "TrackToMCParticlesRelations", 
                              Rich::Future::MC::Relations::TrackToMCParticles } } )
{ }

Rich::Future::MC::Relations::TkToMCPRels
TrackToMCParticleRelations::operator()( const LHCb::Tracks& tks, 
                                        const LHCb::MCParticles& ) const
{
  // make a relations table
  Relations::TkToMCPRels rels( tks.size() );

  // The location in the TES for the tracks we have been given
  const auto & tkLoc = inputLocation<0>();

  // Load the linker for the given tracks
  // Note this internally loads data from the TES
  LinkedTo<LHCb::MCParticle,LHCb::Track> linker( evtSvc(), msgSvc(), tkLoc );
  if ( UNLIKELY( linker.notFound() ) )
  {
    Warning ( "The linker table for '" + tkLoc + "' is not found!" ).ignore();
  }
  else
  {
  
    // loop over the tracks and fill relations table
    for ( const auto * tk : tks )
    {
      if ( !tk ) continue;
      const auto * mcp = linker.first( tk );
      while ( mcp )
      {
        rels.i_push( tk, mcp, linker.weight() ); // NB! i_push is used!     
        mcp = linker.next();
      }
    }

    // MANDATORY usage of i_sort after i_push 
    rels.i_sort();

  }

  // check for some "strange" status 
  if ( rels.i_relations().empty() ) { Warning("Empty relation table!").ignore(); }

  // return the final relations
  return rels;
}

//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( TrackToMCParticleRelations )

//-----------------------------------------------------------------------------
