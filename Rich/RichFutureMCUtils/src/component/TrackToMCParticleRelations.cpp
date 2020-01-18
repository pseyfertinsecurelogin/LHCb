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

// Rich (Future) Kernel
#include "RichFutureKernel/RichAlgBase.h"

// Gaudi Functional
#include "GaudiAlg/Transformer.h"

// Event
#include "Event/LinksByKey.h"
#include "Event/MCParticle.h"
#include "Event/Track.h"

// Relations
#include "RichFutureMCUtils/RichMCRelations.h"

namespace Rich::Future::MC {

  // Use the functional framework
  using namespace Gaudi::Functional;

  /** @class TrackToMCParticleRelations TrackToMCParticleRelations.h
   *
   *  (Temporary) algorithm that takes the Track<->MCParticle linker and forms
   *  a relations object from it. Temporary in that the linker object as it stands
   *  is not compatible with the functional framework, given the way it internal
   *  interacts with the TES. Longer term the usage of the linkers in the future
   *  algorithms needs reviewing, and most probably removing.
   *
   *  @author Chris Jones
   *  @date   2016-12-07
   */
  class TrackToMCParticleRelations final : public Transformer<Relations::TkToMCPRels( const LHCb::Tracks&,      //
                                                                                      const LHCb::MCParticles&, //
                                                                                      const LHCb::LinksByKey& ),
                                                              Traits::BaseClass_t<AlgBase<>>> {

  public:
    /// Standard constructor
    TrackToMCParticleRelations( const std::string& name, ISvcLocator* pSvcLocator )
        : Transformer( name, pSvcLocator,
                       // inputs
                       {KeyValue{"TracksLocation", LHCb::TrackLocation::Default},
                        KeyValue{"MCParticlesLocation", LHCb::MCParticleLocation::Default},
                        KeyValue{"MCParticlesLinkLocation", "Link/" + LHCb::TrackLocation::Default}},
                       // output
                       {KeyValue{"TrackToMCParticlesRelations", Rich::Future::MC::Relations::TrackToMCParticles}} ) {}

  public:
    /// Algorithm execution via transform
    Rich::Future::MC::Relations::TkToMCPRels operator()( const LHCb::Tracks&      tks,     //
                                                         const LHCb::MCParticles& mcParts, //
                                                         const LHCb::LinksByKey&  links    //
                                                         ) const override {

      // make a relations table
      Relations::TkToMCPRels rels( 2 * tks.size() );

      // loop over the tracks and fill relations table
      for ( auto* tk : tks ) {
        if ( !tk ) continue;

        if ( m_allowMultMPs ) {
          // Save all the MCPs for each track
          links.applyToLinks( tk->key(), [&rels, &tk, &mcParts]( auto, auto tgtKey, auto weight ) {
            const auto mcp = dynamic_cast<const LHCb::MCParticle*>( mcParts.containedObject( tgtKey ) );
            if ( mcp ) { rels.i_push( tk, mcp, weight ); }
          } );
        } else {
          // find best MCP based on weight and save only that
          double                  bestWeight = -1;
          const LHCb::MCParticle* bestMCP    = nullptr;
          links.applyToLinks( tk->key(), [&bestMCP, &bestWeight, &mcParts]( auto, auto tgtKey, auto weight ) {
            if ( weight > bestWeight ) {
              const auto mcp = dynamic_cast<const LHCb::MCParticle*>( mcParts.containedObject( tgtKey ) );
              if ( mcp ) {
                bestWeight = weight;
                bestMCP    = mcp;
              }
            }
          } );
          if ( bestMCP ) { rels.i_push( tk, bestMCP, bestWeight ); }
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

      // return the final relations
      return rels;
    }

  private:
    // properties

    /// Allow more than one MCParticle per track ?
    Gaudi::Property<bool> m_allowMultMPs{this, "AllowMultipleMCPsPerTrack", true};
  };

  // Declaration of the Algorithm Factory
  DECLARE_COMPONENT( TrackToMCParticleRelations )

} // namespace Rich::Future::MC
