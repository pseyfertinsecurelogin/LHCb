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
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiTool.h"
// ============================================================================
// MCInterfaces
// ============================================================================
#include "MCInterfaces/IMCDecayFinder.h"
// ============================================================================
// LoKiCore
// ============================================================================
#include "LoKi/Base.h"
#include "LoKi/Interface.h"
// ============================================================================
// LoKiMC
// ============================================================================
#include "LoKi/MCFinderObj.h"
#include "LoKi/MCTypes.h"
// ============================================================================
/** @file
 *
 *  Implementation file for classes LoKi::MCFinder
 *  and class LoKi::MCFinderObj
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-03-11
 */
// ============================================================================
/*  Standard constructor
 *  @param name object name
 *  @param finder the mc-decay finder itself
 *  @param algo   data provider
 *  @param reporter error reporter
 */
// ============================================================================
LoKi::MCFinderObj::MCFinderObj( const std::string& name, const IMCDecayFinder* finder, const GaudiAlgorithm* algo,
                                const IReporter* reporter )
    : LoKi::Base( name, reporter ), m_finder( finder ), m_tool( 0 ), m_algo( algo ) {}
// ============================================================================
/*  Standard constructor
 *  @param name object name
 *  @param finder the mc-decay finder itself
 *  @param algo   data provider
 *  @param reporter error reporter
 */
// ============================================================================
LoKi::MCFinderObj::MCFinderObj( const std::string& name, const IMCDecayFinder* finder, const GaudiTool* algo,
                                const IReporter* reporter )
    : LoKi::Base( name, reporter ), m_finder( finder ), m_tool( algo ), m_algo( 0 ) {}
// ============================================================================
// clear the internal storages
// ============================================================================
void LoKi::MCFinderObj::clear() { m_decays.clear(); }
// ===========================================================================
/*  find MC decays.
 *
 *  It is just a short cut for very convinient and simple DecayFinder
 *  tool by Olivier Dormond
 *
 *  @code
 *
 *  MCPRange range = findDecays ( "B0 -> (J/psi(1S) -> mu+ mu-) KS0" );
 *
 *  @endcode
 *
 *  @see DecayFinder
 *  @param decay   decay   formula
 *  @param address location of container of MC particle
 *  @return range of found MC decays
 */
// ===========================================================================
LoKi::Types::MCRange LoKi::MCFinderObj::findDecays( const std::string& decay, const std::string& address ) const {
  if ( m_algo.validPointer() ) {
    const LHCb::MCParticles* mc = m_algo->get<LHCb::MCParticles>( address );
    return findDecays( decay, mc ); // RETURN
  } else if ( m_tool.validPointer() ) {
    const LHCb::MCParticles* mc = m_tool->get<LHCb::MCParticles>( address );
    return findDecays( decay, mc ); // RETURN
  }
  // throw exception
  Exception( "LoKi::MCFinderObj('" + name() + "')::findDecay: invalid data provider!" );
  // "fictive" return
  return LoKi::Types::MCRange();
}
// ===========================================================================
/** find MC decays.
 *
 *  It is just a short cut for very convinient and simple DecayFinder
 *  tool by Olivier Dormond
 *
 *  @code
 *
 *  MCPRange range = findDecays ( "B0 -> (J/psi(1S) -> mu+ mu-) KS0" );
 *
 *  @endcode
 *
 *  @see DecayFinder
 *  @param decay   decay   formula
 *  @param address location of container of MC particle
 *  @return range of found MC decays
 */
// ===========================================================================
LoKi::Types::MCRange LoKi::MCFinderObj::findDecays( const std::string&                 decay,
                                                    const LHCb::MCParticle::Container* particles ) const {
  if ( 0 == particles ) {
    Error( "LHCb::MCParticles* points ot NULL, return MCRange()" )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    return LoKi::Types::MCRange(); // RETURN
  }
  return _findDecays( decay, *particles );
}
// ============================================================================
/** find MC decays.
 *
 *  It is just a short cut for very convinient and simple DecayFinder
 *  tool by Olivier Dormond
 *
 *  @code
 *
 *  MCRange range = findDecays ( "B0 -> (J/psi(1S) -> mu+ mu-) KS0" );
 *
 *  @endcode
 *
 *  @see DecayFinder
 *  @param decay   decay   formula
 *  @param address location of container of MC particle
 *  @return range of found MC decays
 */
// ============================================================================L
LoKi::Types::MCRange LoKi::MCFinderObj::findDecays( const std::string&              decay,
                                                    const LHCb::MCParticle::Vector& particles ) const {
  return findDecays( decay, particles.begin(), particles.end() );
}
// ============================================================================
LoKi::Types::MCRange LoKi::MCFinderObj::findDecays( const std::string&                   decay,
                                                    const LHCb::MCParticle::ConstVector& particles ) const {
  return findDecays( decay, particles.begin(), particles.end() );
}
// ============================================================================
LoKi::Types::MCRange LoKi::MCFinderObj::findDecays( const std::string&                    decay,
                                                    const LoKi::Keeper<LHCb::MCParticle>& particles ) const {
  return findDecays( decay, particles.begin(), particles.end() );
}
// ============================================================================
LoKi::Types::MCRange LoKi::MCFinderObj::findDecays( const std::string&                          decay,
                                                    const LoKi::UniqueKeeper<LHCb::MCParticle>& particles ) const {
  return findDecays( decay, particles.begin(), particles.end() );
}
// ============================================================================
/*  find MC decays.
 *
 *  It is just a short cut for very convinient and simple DecayFinder
 *  tool by Olivier Dormond
 *
 *  @code
 *
 *  MCPRange range = findDecays ( "B0 -> (J/psi(1S) -> mu+ mu-) KS0" );
 *
 *  @endcode
 *
 *  @see DecayFinder
 *  @param decay   decay   formula
 *  @param address location of container of MC particle
 *  @return range of found MC decays
 */
// ============================================================================
LoKi::Types::MCRange LoKi::MCFinderObj::findDecays( const std::string&          decay,
                                                    const LoKi::Types::MCRange& range ) const {
  return _findDecays( decay, LoKi::MCTypes::MCContainer( range.begin(), range.end() ) );
}
// ============================================================================
// The END
// ============================================================================
