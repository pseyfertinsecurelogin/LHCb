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
#include "Event/MCParticle.h"
#include "GaudiAlg/Consumer.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CompareMCParticle
//
// 2005-03-18 : Olivier Callot
//-----------------------------------------------------------------------------

/** @class CompareMCParticle CompareMCParticle.h
 *  Compare two containers of MCParticles
 *
 *  @author Olivier Callot
 *  @date   2005-03-18
 */
struct CompareMCParticle : Gaudi::Functional::Consumer<void( const LHCb::MCParticles&, const LHCb::MCParticles& )> {

  /// Standard constructor
  CompareMCParticle( const std::string& name, ISvcLocator* pSvcLocator )
      : Consumer{name,
                 pSvcLocator,
                 {KeyValue{"InputName", LHCb::MCParticleLocation::Default},
                  KeyValue{"TestName", LHCb::MCParticleLocation::Default + "Test"}}} {}

  void operator()( const LHCb::MCParticles& old, const LHCb::MCParticles& test ) const override {
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Execute" << endmsg;

    if ( old.size() != test.size() ) {
      err() << "Old MCParticle size " << old.size() << " differs form Test " << test.size() << endmsg;
      throw GaudiException{"Sizes differ", "CompareMCParticle", StatusCode::FAILURE};
    }
    auto itOld  = old.begin();
    auto itTest = test.begin();

    while ( old.end() != itOld ) {
      LHCb::MCParticle* oPart = ( *itOld++ );
      LHCb::MCParticle* tPart = ( *itTest++ );
      if ( oPart->key() != tPart->key() ) {
        warning() << "Wrong key : old " << oPart->key() << " test " << tPart->key() << endmsg;
      }
      bool isOK = true;
      if ( 5.e-3 < fabs( oPart->momentum().px() - tPart->momentum().px() ) ) isOK = false;
      if ( 5.e-3 < fabs( oPart->momentum().py() - tPart->momentum().py() ) ) isOK = false;
      if ( 5.e-3 < fabs( oPart->momentum().pz() - tPart->momentum().pz() ) ) isOK = false;
      double massTol = 1.e-5 * fabs( oPart->momentum().M() ) +  /// float, relative...
                       1.e-5 * fabs( oPart->momentum().pz() ) + // high energy photons rounding
                       .001;                                    // null mass/p protection.

      if ( massTol < fabs( oPart->momentum().M() - tPart->momentum().M() ) ) isOK = false;
      if ( oPart->originVertex() != tPart->originVertex() ) isOK = false;
      if ( !std::equal( oPart->endVertices().begin(), oPart->endVertices().end(), tPart->endVertices().begin(),
                        tPart->endVertices().end(), []( const auto& dum, const auto& dum1 ) {
                          return dum.target() == dum1.target(); // convert smartref to pointers
                        } ) ) {
        isOK = false;
      }

      if ( !isOK || MSG::VERBOSE >= msgLevel() ) {
        if ( !isOK ) Warning( "Packed MCParticle info truncated. Set VERBOSE OutputLevel for details" ).ignore();
        debug() << "=== MCParticle key " << oPart->key() << endmsg;
        debug() << format( "  old momentum %12.3f %12.3f %12.3f %12.4f", oPart->momentum().px(), oPart->momentum().py(),
                           oPart->momentum().pz(), oPart->momentum().M() )
                << endmsg;
        debug() << format( " test momentum %12.3f %12.3f %12.3f %12.4f", tPart->momentum().px(), tPart->momentum().py(),
                           tPart->momentum().pz(), tPart->momentum().M() )
                << endmsg;
        debug() << format( "  old pid %6d ", oPart->particleID().pid() ) << " endVert " << oPart->originVertex()
                << endmsg;
        debug() << format( " test pid %6d ", tPart->particleID().pid() ) << " endVert " << tPart->originVertex()
                << endmsg << "  old endVertices ";
        for ( const auto& kk : oPart->endVertices() ) { debug() << " " << kk; }
        debug() << endmsg << " test endVertices ";
        for ( const auto& kk : tPart->endVertices() ) { debug() << " " << kk; }
        debug() << endmsg;
      }
    }
  }
};

DECLARE_COMPONENT( CompareMCParticle )
