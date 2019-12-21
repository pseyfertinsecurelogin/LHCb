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
#include "Event/GenHeader.h"
#include "Event/MCParticle.h"
#include "GaudiAlg/GaudiTool.h"
#include "MCInterfaces/IForcedBDecayTool.h"

/** @class ForcedBDecayTool ForcedBDecayTool.h
 *
 *  v1.0
 *  @author Marco Musy
 *  @date   2006-10-02
 */
class ForcedBDecayTool : public extends<GaudiTool, IForcedBDecayTool> {
public:
  /// Standard constructor
  using extends::extends;

  const LHCb::MCParticle* forcedB() override;

private:
  LHCb::MCParticle*                       associatedofHEP( HepMC::GenParticle* );
  DataObjectReadHandle<LHCb::HepMCEvents> m_mcEvent{this, "HepMCEvents", LHCb::HepMCEventLocation::Default};
  DataObjectReadHandle<LHCb::MCParticles> m_mcParts{this, "MCParticles", LHCb::MCParticleLocation::Default};
};

//-----------------------------------------------------------------------------
// Implementation file for class : ForcedBDecayTool v1.0
//
// 2007-02-06 : Marco Musy
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( ForcedBDecayTool )

//=============================================================================
const LHCb::MCParticle* ForcedBDecayTool::forcedB() {

  // check what is the B forced to decay
  const LHCb::MCParticle*  BS      = nullptr;
  const LHCb::HepMCEvents* hepVect = m_mcEvent.get();
  for ( auto& q : *hepVect ) {
    for ( auto p = q->pGenEvt()->particles_begin(); p != q->pGenEvt()->particles_end(); ++p ) {
      if ( ( *p )->status() != 889 ) continue;
      BS = associatedofHEP( *p );
      if ( BS ) break;
    }
  }
  return BS;
}
//============================================================================
LHCb::MCParticle* ForcedBDecayTool::associatedofHEP( HepMC::GenParticle* hepmcp ) {
  const LHCb::MCParticles* mcpart = m_mcParts.get();

  auto imc = std::find_if( mcpart->begin(), mcpart->end(), [mid = hepmcp->pdg_id()]( const LHCb::MCParticle* p ) {
    return p->particleID().hasBottom() && p->particleID().pid() == mid;
  } );
  return imc != mcpart->end() ? *imc : nullptr;
}
//=============================================================================
