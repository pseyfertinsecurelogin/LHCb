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
#include "MCInterfaces/IPrintMCDecayTreeTool.h"

/** @class PrintMCDecayTreeAlg PrintMCDecayTreeAlg.h
 *  Prints a dump of the MC event tree, using an implementation of the
 *  IPrintMCDecayTreeTool interface.
 *
 *  The MC event tree is defined by the TES location of an
 *  LHCb::MCParticle container. This is controlled by the property
 *  <b>MCParticleLocation</b>. The default is LHCb::MCParticleLocation::Default
 *
 *  The IPrintMCDecayTreeTool implementation to use is controlled by the
 *  property <b>PrintTool</b>. the default is "PrintMCDecayTreeTool".
 *
 *  @author Vladimir Gligorov, adapted by Marco Cattaneo
 *  @date   26/11/2007
 */
class PrintMCDecayTreeAlg : public Gaudi::Functional::Consumer<void( const LHCb::MCParticles& )> {
public:
  /// Standard constructor
  PrintMCDecayTreeAlg( const std::string& name, ISvcLocator* pSvcLocator );

  void operator()( const LHCb::MCParticles& ) const override; ///< Algorithm execution

private:
  ToolHandle<IPrintMCDecayTreeTool> m_printTool{this, "PrintTool", "PrintMCDecayTreeTool"};
};

//-----------------------------------------------------------------------------
// Implementation file for class : PrintMCDecayTreeAlg
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( PrintMCDecayTreeAlg )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PrintMCDecayTreeAlg::PrintMCDecayTreeAlg( const std::string& name, ISvcLocator* pSvcLocator )
    : Consumer( name, pSvcLocator, KeyValue{"MCParticleLocation", LHCb::MCParticleLocation::Default} ) {}

//=============================================================================
// Main execution
//=============================================================================
void PrintMCDecayTreeAlg::operator()( const LHCb::MCParticles& parts ) const {

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) {
    debug() << "==> Execute" << endmsg;
    if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) verbose() << "Getting MCParticles from " << inputLocation() << endmsg;
  }

  for ( const auto& part : parts ) {
    if ( !part->originVertex() || !part->originVertex()->mother() ) m_printTool->printTree( part, -1 );
  }
}
//=============================================================================
