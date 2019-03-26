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
// STD & STL
// ============================================================================
#include <sstream>
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleID.h"
#include "Kernel/ParticleProperty.h"
// ============================================================================
// LHCbKernel
// ============================================================================
#include "Kernel/Decay.h"
// ============================================================================
/** @file
 *  Implementation file for class LHCb::Decay
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2008-03-31
 */
// ============================================================================
// the constructor from the particle property
// ============================================================================
Decays::Decay::Item::Item( const LHCb::ParticleProperty* pp ) : m_pp( pp ) {
  if ( m_pp ) {
    m_name = m_pp->particle();
    m_pid  = m_pp->particleID();
  }
}
// ============================================================================
// the constructor from the particle name
// ============================================================================
Decays::Decay::Item::Item( const std::string& name ) : m_name( name ) {}
// ============================================================================
// the constructor from the particle PID
// ============================================================================
Decays::Decay::Item::Item( const LHCb::ParticleID& pid ) : m_pid( pid ) {}
// ============================================================================
// validate the item using the service
// ============================================================================
StatusCode Decays::Decay::Item::validate( const LHCb::IParticlePropertySvc* svc ) const {
  if ( m_pp ) {
    m_name = m_pp->particle();
    m_pid  = m_pp->particleID();
    return StatusCode::SUCCESS; // RETURN
  }
  // it not possible to validate it!
  if ( 0 == svc ) { return StatusCode::FAILURE; } // RETURN
  // check by name
  if ( !m_name.empty() ) {
    m_pp = svc->find( m_name );
    if ( !m_pp ) { return StatusCode::FAILURE; } // RETURN
    m_pid = m_pp->particleID();
    return StatusCode::SUCCESS; // RETURN
  }
  // check by PID
  if ( LHCb::ParticleID() != m_pid ) {
    m_pp = svc->find( m_pid );
    if ( !m_pp ) { return StatusCode::FAILURE; } // RETURN
    m_name = m_pp->particle();
    return StatusCode::SUCCESS; // RETURN
  }
  return StatusCode::FAILURE; // RETURN
}
// ============================================================================
// validate the item using the particle property object
// ============================================================================
StatusCode Decays::Decay::Item::validate( const LHCb::ParticleProperty* pp ) const {
  if ( m_pp && !pp ) {
    m_name = m_pp->particle();
    m_pid  = m_pp->particleID();
    return StatusCode::SUCCESS; // RETURN
  } else if ( pp ) {
    m_pp   = pp;
    m_name = m_pp->particle();
    m_pid  = m_pp->particleID();
    return StatusCode::SUCCESS; // RETURN
  }
  return StatusCode::FAILURE; // RETURN
}
// ============================================================================
// the constructor from mother and daughters
// ============================================================================
Decays::Decay::Decay( const LHCb::ParticleProperty*                     mother,     // the mother
                      const std::vector<const LHCb::ParticleProperty*>& daughters ) // the daughtrers
    : m_mother( mother ) {
  setDaughters( daughters );
}
// ============================================================================
// the constructor from mother and daughters
// ============================================================================
Decays::Decay::Decay( const std::string&              mother,     // the mother
                      const std::vector<std::string>& daughters ) // the daughters
    : m_mother( mother ) {
  setDaughters( daughters );
}
// ============================================================================
// the constructor from mother and daughters
// ============================================================================
Decays::Decay::Decay( const LHCb::ParticleID&              mother,     // the mother
                      const std::vector<LHCb::ParticleID>& daughters ) // the daughters
    : m_mother( mother ) {
  setDaughters( daughters );
}
// ============================================================================
// the constructor from mother and daughters
// ============================================================================
Decays::Decay::Decay( const Decays::Decay::Item&              mother,     // the mother
                      const std::vector<Decays::Decay::Item>& daughters ) // the daughters
    : m_mother( mother ), m_daughters( daughters ) {}
// ============================================================================
// validate the decay using the service
// ============================================================================
StatusCode Decays::Decay::validate( const LHCb::IParticlePropertySvc* svc ) const {
  // validate the mother
  StatusCode sc = m_mother.validate( svc );
  if ( sc.isFailure() ) { return sc; }                       // RETURN
  if ( m_daughters.empty() ) { return StatusCode::FAILURE; } // RETURN
  // loop over daughters
  for ( auto idau = m_daughters.begin(); m_daughters.end() != idau; ++idau ) {
    sc = idau->validate( svc );
    if ( sc.isFailure() ) { return sc; } // RETURN
  }
  //
  return sc; // RETURN
}
// ============================================================================
// the default printout
// ============================================================================
std::ostream& Decays::Decay::fillStream( std::ostream& s ) const {
  s << m_mother;
  if ( m_daughters.empty() ) { return s; }
  //
  s << "->";
  // loop over daughters
  for ( const auto& dau : m_daughters ) { s << dau; }
  //
  return s;
}
// ============================================================================
// the default printout
// ============================================================================
std::ostream& Decays::Decay::Item::fillStream( std::ostream& s ) const {
  if ( m_name.empty() ) {
    if ( m_pp ) {
      m_name = m_pp->particle();
    } else if ( LHCb::ParticleID() != m_pid ) {
      return s << " " << m_pid.pid() << " ";
    } // RETURN
    else {
      return s << " ? ";
    } // RETURN
  }
  return s << " " << m_name << " ";
}
// ============================================================================
// the conversion to the string
// ============================================================================
std::string Decays::Decay::toString() const {
  std::ostringstream s;
  fillStream( s );
  return s.str();
}
// ============================================================================
// set daughters
// ============================================================================
void Decays::Decay::setDaughters( const std::vector<const LHCb::ParticleProperty*>& daugs ) {
  m_daughters.clear();
  for ( const auto& pp : daugs ) *this += pp;
}
// ============================================================================
// set daughters
// ============================================================================
void Decays::Decay::setDaughters( const std::vector<std::string>& daugs ) {
  m_daughters.clear();
  for ( const auto& pp : daugs ) *this += pp;
}
// ============================================================================
// set daughters
// ============================================================================
void Decays::Decay::setDaughters( const std::vector<LHCb::ParticleID>& daugs ) {
  m_daughters.clear();
  for ( const auto& pp : daugs ) *this += pp;
}
// ============================================================================
// set the daughters
// ============================================================================
void Decays::Decay::setDaughters( const Decays::Decay::Items& daugs ) { m_daughters = daugs; }
// ============================================================================

// ============================================================================
// add the child
// ============================================================================
Decays::Decay& Decays::Decay::operator+=( const std::string& child ) { return ( *this ) += Item( child ); }
// ============================================================================
// add the child
// ============================================================================
Decays::Decay& Decays::Decay::operator+=( const LHCb::ParticleID& child ) { return ( *this ) += Item( child ); }
// ============================================================================
// add the child
// ============================================================================
Decays::Decay& Decays::Decay::operator+=( const LHCb::ParticleProperty* child ) { return ( *this ) += Item( child ); }
// ============================================================================
// add the child
// ============================================================================
Decays::Decay& Decays::Decay::operator+=( const Decays::Decay::Item& child ) {
  m_daughters.push_back( child );
  return *this;
}
// ============================================================================
/*  get the component by the number
 *  @attention index 0 corresponds to the mother particle
 *  @param index the index (0 corresponds to the mother particle)
 *  @return the component
 */
// ============================================================================
const Decays::Decay::Item& Decays::Decay::operator()( const unsigned int index ) const {
  if ( 0 == index ) { return m_mother; }
  return m_daughters[index - 1];
}
// ============================================================================

// ============================================================================
// set the mother
// ============================================================================
void Decays::Decay::setMother( const Decays::Decay::Item& mom ) { m_mother = mom; }
// ============================================================================
// set the mother
// ============================================================================
void Decays::Decay::setMother( const LHCb::ParticleProperty* mom ) { setMother( Item( mom ) ); }
// ============================================================================
// set the mother
// ============================================================================
void Decays::Decay::setMother( const LHCb::ParticleID& mom ) { setMother( Item( mom ) ); }
// ============================================================================
// set the mother
// ============================================================================
void Decays::Decay::setMother( const std::string& mom ) { setMother( Item( mom ) ); }
// ============================================================================

// ============================================================================
// The END
// ============================================================================
