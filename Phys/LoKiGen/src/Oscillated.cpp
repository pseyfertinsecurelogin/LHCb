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
// ============================================================================
// Include files
// ============================================================================
// HepMC
// ============================================================================
#include "HepMC/GenEvent.h"
// ============================================================================
// local
// ============================================================================
#include "LoKi/GenOscillated.h"
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
 *  @date 2011-11-11
 *
 */
// ===========================================================================
/* check the oscillation for the particle
 *  @param p the particle
 *  @return the oscillated mother particle
 *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
 *  @date 2008-06-03
 */
// ============================================================================
const HepMC::GenParticle* LoKi::GenParticles::oscillated1( const HepMC::GenParticle* p ) {
  if ( !p ) { return nullptr; }
  const HepMC::GenVertex* pv = p->production_vertex();
  if ( !pv ) { return nullptr; }
  if ( 1 != pv->particles_in_size() ) { return nullptr; }
  const HepMC::GenParticle* m = *( pv->particles_in_const_begin() );
  if ( !m ) { return nullptr; }
  // oscillated ?
  if ( p->pdg_id() != -m->pdg_id() ) { return nullptr; }
  //
  return m;
}
// ============================================================================
/*  check the oscillation for the particle
 *  @param p the particle
 *  @return the oscillated daughter particle
 *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
 *  @date 2008-06-03
 */
const HepMC::GenParticle* LoKi::GenParticles::oscillated2( const HepMC::GenParticle* p ) {
  if ( !p ) { return nullptr; }
  const HepMC::GenVertex* ev = p->end_vertex();
  if ( !ev ) { return nullptr; }
  if ( 1 != ev->particles_out_size() ) { return nullptr; }
  const HepMC::GenParticle* d = *( ev->particles_out_const_begin() );
  if ( !d ) { return nullptr; }
  // oscillated ?
  if ( p->pdg_id() != -d->pdg_id() ) { return nullptr; }
  //
  return d;
}
// ==========================================================================
// The END
// ==========================================================================
