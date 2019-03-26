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
#include "LoKi/MCExtractDicts.h"
#include "LoKi/MCExtract.h"
// ============================================================================
/** @file
 *  Implementation file for class for functions LoKi::Dicts::Exract::MCExtractDicts
 *  @author Vanya BELYAEV ibelyev@physics.syr.edu
 *  @date 2007-08-11
 */
// ============================================================================
// get MC-particles form decay tree
// ============================================================================
LHCb::MCParticle::ConstVector LoKi::MCExtract::mcParticles( const LHCb::MCParticle*      particle,
                                                            const LoKi::MCTypes::MCCuts& cuts, const bool decayOnly ) {
  LHCb::MCParticle::ConstVector result;
  result.reserve( 15 );
  LoKi::Extract::getMCParticles( particle, std::back_inserter( result ), cuts, decayOnly );
  return result;
}
// ============================================================================
// get MC-particles form decay tree
// ============================================================================
LHCb::MCParticle::ConstVector LoKi::MCExtract::mcParticles( const LHCb::MCVertex*        vertex,
                                                            const LoKi::MCTypes::MCCuts& cuts, const bool decayOnly ) {
  LHCb::MCParticle::ConstVector result;
  result.reserve( 15 );
  LoKi::Extract::getMCParticles( vertex, std::back_inserter( result ), cuts, decayOnly );
  return result;
}
// ============================================================================
// The END
// ============================================================================
