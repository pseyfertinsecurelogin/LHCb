/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "ForcedBDecayTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : ForcedBDecayTool v1.0
//
// 2007-02-06 : Marco Musy
//-----------------------------------------------------------------------------

using namespace LHCb;

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( ForcedBDecayTool )

//=============================================================================
const MCParticle* ForcedBDecayTool::forcedB() {

  //check what is the B forced to decay
  const MCParticle *BS = nullptr;
  HepMCEvents* hepVect = get<HepMCEvents>( HepMCEventLocation::Default );
  for( auto& q : *hepVect) {
    for ( auto p = q->pGenEvt()->particles_begin(); p != q->pGenEvt()->particles_end();   ++p ) {
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
