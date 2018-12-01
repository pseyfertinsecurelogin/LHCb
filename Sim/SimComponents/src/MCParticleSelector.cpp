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

// Gaudi
#include "MCParticleSelector.h"
#include "Event/MCFun.h"


DECLARE_COMPONENT( MCParticleSelector )

StatusCode MCParticleSelector::initialize()
{
  // Initialize base class
  const StatusCode sc = base_class::initialize();
  if ( sc.isFailure() ) { return sc; }

  // printout selection criteria
  info() << "MCParticle Momentum cut     : " << m_pMin.value()/Gaudi::Units::GeV << " GeV/c < P < " << m_pMax.value()/Gaudi::Units::GeV << " GeV/c" << endmsg;
  info() << "           Beta * gamma cut : " << m_betaGammaMin.value() << " < beta*gamma" << endmsg;
  info() << "           Eta cut          : " << m_etaMin.value() << " < P < " << m_etaMax.value() << endmsg;
  if ( m_rejectElectrons ) info() << "           Will reject electrons" << endmsg;
  if ( m_selBprods       ) info() << "           Will only select B decay products" << endmsg;
  if ( m_rejectInteractions ) info() << "           Will reject particles from interations before z=" << m_zInteraction.value() << endmsg;
  if ( !m_selCharged ) info() << "           Will reject charged particles" << endmsg;
  if ( !m_selNeutral ) info() << "           Will reject neutral particles" << endmsg;

  return sc;
}

bool MCParticleSelector::accept(const LHCb::MCParticle* aParticle) const
{
  // select particles of some quality ...

  // Check for NULL pointers
  if ( !aParticle ) return false;

  // charge selection
  const int charge = aParticle->particleID().threeCharge();
  if ( (0 == charge && !m_selNeutral) ||
       (0 != charge && !m_selCharged) ) return false;

  // origin vertex
  const LHCb::MCVertex* origin = aParticle->originVertex();
  if ( (NULL == origin) ||
       (origin->position().z() > m_zOrigin) ) return false;


  // momentum cuts
  const double tMomentum = aParticle->p();
  if ( tMomentum < m_pMin || tMomentum > m_pMax ) return false;

  // beta * gamma - skip it for photons
  if( aParticle->virtualMass() > 1.*Gaudi::Units::keV ) {
    const double betaGamma = aParticle->betaGamma();
    if ( betaGamma < m_betaGammaMin ) return false;
  }

  // eta
  const double tEta = aParticle->pseudoRapidity();
  if ( tEta < m_etaMin || tEta > m_etaMax ) return false;


  // reject electrons ?
  if ( m_rejectElectrons &&
       (aParticle->particleID().abspid() == 11) ) return false;

  // reject interactions
  if ( m_rejectInteractions &&
       LHCb::MC::zInteraction(aParticle) < m_zInteraction) return false;

  // select only b decay products ?
  if ( m_selBprods && !fromBdecay(aParticle) ) return false;

  // all OK
  return true;
}

bool MCParticleSelector::fromBdecay( const LHCb::MCParticle * aParticle ) const
{
  // loop back and see if there is a B in the history
  bool fromB = false;
  const LHCb::MCParticle * motherP = aParticle->mother();
  while ( motherP && !fromB ) {
    fromB   = motherP->particleID().hasBottom();
    motherP = motherP->mother();
  }
  return fromB;
}
