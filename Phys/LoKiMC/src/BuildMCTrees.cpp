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
// Event
// ============================================================================
#include "Event/MCParticle.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BuildMCTrees.h"
#include "LoKi/MCTypes.h"
// ============================================================================
/** @file
 *  Implementation file for functions from namespace LoKi::MCTrees
 *  @date 2006-11-29
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 */
// ============================================================================
LHCb::MCParticle::Vector LoKi::MCTrees::buildTrees( const LHCb::MCParticle::Vector& input ) {
  LHCb::MCParticle::Vector output;
  output.reserve( input.size() );
  buildTrees( input.begin(), input.end(), std::back_inserter( output ) );
  return output;
}
// ============================================================================
LHCb::MCParticle::ConstVector LoKi::MCTrees::buildTrees( const LHCb::MCParticle::ConstVector& input ) {
  LHCb::MCParticle::ConstVector output;
  output.reserve( input.size() );
  buildTrees( input.begin(), input.end(), std::back_inserter( output ) );
  return output;
}
// ============================================================================
LHCb::MCParticle::ConstVector LoKi::MCTrees::buildTrees( const LHCb::MCParticle::Container& input ) {
  if ( input.empty() ) { return LHCb::MCParticle::ConstVector(); }
  LHCb::MCParticle::ConstVector output;
  output.reserve( input.size() );
  buildTrees( input.begin(), input.end(), std::back_inserter( output ) );
  return output;
}
// ============================================================================
LHCb::MCParticle::ConstVector LoKi::MCTrees::buildTrees( const LHCb::MCParticle::Container* input ) {
  if ( 0 == input ) { return LHCb::MCParticle::ConstVector(); }
  return LoKi::MCTrees::buildTrees( *input );
}
// ============================================================================
LHCb::MCParticle::ConstVector LoKi::MCTrees::buildTrees( const LoKi::Types::MCRange& input ) {
  LHCb::MCParticle::ConstVector output;
  output.reserve( input.size() );
  buildTrees( input.begin(), input.end(), std::back_inserter( output ) );
  return output;
}
// ============================================================================
LoKi::Keeper<LHCb::MCParticle> LoKi::MCTrees::buildTrees( const LoKi::Keeper<LHCb::MCParticle>& input ) {
  LHCb::MCParticle::ConstVector _o;
  _o.reserve( input.size() );
  buildTrees( input.begin(), input.end(), std::back_inserter( _o ) );
  return LoKi::Keeper<LHCb::MCParticle>( _o.begin(), _o.end() );
}
// ============================================================================
LoKi::UniqueKeeper<LHCb::MCParticle> LoKi::MCTrees::buildTrees( const LoKi::UniqueKeeper<LHCb::MCParticle>& input ) {
  LHCb::MCParticle::ConstVector _i( input.begin(), input.end() );
  LHCb::MCParticle::ConstVector _o;
  _o.reserve( _i.size() );
  buildTrees( _i.begin(), _i.end(), std::back_inserter( _o ) );
  return LoKi::UniqueKeeper<LHCb::MCParticle>( _o.begin(), _o.end() );
}
// ============================================================================
// The END
// ============================================================================
