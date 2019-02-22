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
// LoKi
// ============================================================================
#include "LoKi/MCParticles3.h"
#include "LoKi/MCParticles.h"
#include "LoKi/MCParticles1.h"
// ============================================================================
/** @file Implementation file for classsed from file LoKi/MCParticles3.h
 *  @date 2017-02-07
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 */
// ============================================================================
LoKi::MCParticles::FromInclusive::FromInclusive( const LoKi::Types::MCCuts& mother, const LoKi::Types::MCCuts& daughter,
                                                 const bool decayOnly )
    : LoKi::AuxFunBase( std::tie( mother, daughter, decayOnly ) )
    , m_cut( daughter && LoKi::MCParticles::InAncestors( mother, decayOnly ) ) {}
// ============================================================================
LoKi::MCParticles::FromInclusive::FromInclusive( const Decays::iNode& mother, const Decays::iNode& daughter,
                                                 const bool decayOnly )
    : LoKi::AuxFunBase( std::tie( mother, daughter, decayOnly ) )
    , m_cut( LoKi::MCParticles::DecNode( daughter ) &&
             LoKi::MCParticles::InAncestors( LoKi::MCParticles::DecNode( mother ), decayOnly ) ) {}
// ============================================================================
LoKi::MCParticles::FromInclusive::FromInclusive( const std::string& mother, const std::string& daughter,
                                                 const bool decayOnly )
    : LoKi::AuxFunBase( std::tie( mother, daughter, decayOnly ) )
    , m_cut( LoKi::MCParticles::DecNode( daughter ) &&
             LoKi::MCParticles::InAncestors( LoKi::MCParticles::DecNode( mother ), decayOnly ) ) {}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::MCParticles::FromInclusive* LoKi::MCParticles::FromInclusive::clone() const {
  return new LoKi::MCParticles::FromInclusive( *this );
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::MCParticles::FromInclusive::operator()( const LHCb::MCParticle* p ) const { return m_cut.fun( p ); }
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::MCParticles::FromInclusive::fillStream( std::ostream& s ) const { return s << m_cut; }
// ============================================================================

// ============================================================================
// The END
// ============================================================================
