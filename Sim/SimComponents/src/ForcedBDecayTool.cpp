#include "ForcedBDecayTool.h"

#include <algorithm>
#include <functional>
#include <vector>

#include "Event/HepMCEvent.h"
#include "Event/MCParticle.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/IInterface.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "Kernel/ParticleID.h"

//-----------------------------------------------------------------------------
// Implementation file for class : ForcedBDecayTool v1.0
//
// 2007-02-06 : Marco Musy 
//-----------------------------------------------------------------------------

using namespace LHCb;

// Declaration of the Algorithm Factory
DECLARE_TOOL_FACTORY( ForcedBDecayTool )

//=============================================================================
ForcedBDecayTool::ForcedBDecayTool( const std::string& type,
				    const std::string& name,
				    const IInterface* parent ) :
  GaudiTool ( type, name, parent ) { 
  declareInterface<IForcedBDecayTool>(this);
}

//=============================================================================
const MCParticle* ForcedBDecayTool::forcedB() {

  //check what is the B forced to decay
  const MCParticle *BS = nullptr;
  HepMCEvents* hepVect = get<HepMCEvents>( HepMCEventLocation::Default );
  for( auto q=hepVect->begin(); q!=hepVect->end(); ++q ) {
    for ( auto p = (*q)->pGenEvt()->particles_begin(); p != (*q)->pGenEvt()->particles_end();   ++p ) {
      if( (*p)->status() != 889 ) continue;
      BS = associatedofHEP(*p);
      if(BS) break; 
    }
  }
  return BS;

}
//============================================================================
MCParticle* ForcedBDecayTool::associatedofHEP(HepMC::GenParticle* hepmcp) {
  MCParticles* mcpart = get<MCParticles> ( MCParticleLocation::Default );

  int mid = hepmcp->pdg_id();
  auto imc = std::find_if( mcpart->begin(), mcpart->end(),
                           [&](const MCParticle* p) {
    return p->particleID().hasBottom() && 
           p->particleID().pid() == mid; 
  });
  return imc!=mcpart->end() ? *imc :  nullptr ;
}
//=============================================================================
