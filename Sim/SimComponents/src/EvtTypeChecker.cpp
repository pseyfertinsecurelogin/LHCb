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
#include "Event/GenCollision.h"
#include "Event/GenHeader.h"
#include "Event/HepMCEvent.h"
#include "Gaudi/Accumulators.h"
#include "GaudiAlg/Consumer.h"
#include "Kernel/IEvtTypeSvc.h"
#include "MCInterfaces/IMCDecayFinder.h"
#include <string>

/** @class EvtTypeChecker EvtTypeChecker.h
 *  Algorithm that verifies if at least one decay of the desired type
 *  is present in the event and counts for how many this occurs.
 *
 *  @author Gloria CORTI
 *  @date   2005-08-08
 */
class EvtTypeChecker : public Gaudi::Functional::Consumer<void( const LHCb::GenHeader& )> {
public:
  /// Standard constructor
  EvtTypeChecker( const std::string& name, ISvcLocator* pSvcLocator )
      : Consumer{name, pSvcLocator, {"GenHeader", LHCb::GenHeaderLocation::Default}} {}

  StatusCode initialize() override;                               ///< Algorithm initialization
  void       operator()( const LHCb::GenHeader& ) const override; ///< Algorithm execution
  StatusCode finalize() override;                                 ///< Algorithm finalization

private:
  /** Set the description of the decay to be found
   *  @param evtCode 7-digits code to identify event types
   */
  StatusCode setDecayToFind( const int evtCode ) const;

  Gaudi::Property<bool>        m_fromData{this, "EvtCodeFromData", true}; ///< flag read event code from data
  mutable Gaudi::Property<int> m_evtCode{this, "EvtCode", 0};             ///< event code to test
  mutable Gaudi::Accumulators::BinomialCounter<> m_nMCFound{this, "Fraction of decays found"};
  mutable bool                                   m_setDecay = false; ///< Flag is decay has been set
  ServiceHandle<IEvtTypeSvc>                     m_evtTypeSvc{this, "EvtTypeSvc", "EvtTypeSvc"}; ///< Pointer to service
  mutable ToolHandle<IMCDecayFinder> m_mcFinder{this, "MCDecayFinder", "MCDecayFinder"};         ///< Pointer to tool
};
//-----------------------------------------------------------------------------
// Implementation file for class : EvtTypeChecker
//
// 2004-04-26 : Gloria CORTI
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( EvtTypeChecker )

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode EvtTypeChecker::initialize() {

  StatusCode sc = Consumer::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;        // error printed already

  // Check that EvtType code has been set with appropriate value
  // if it will not be read from data
  if ( !m_fromData.value() ) {
    if ( m_evtCode == 0 ) {
      fatal() << "With EvtCodeFromData = false you MUST set EvtCode" << endmsg;
      return StatusCode::FAILURE;
    }

    // Set the decay descriptor to pass to the MCDecayFinder if using evtCode
    if ( ( setDecayToFind( m_evtCode.value() ) ).isFailure() ) {
      fatal() << " 'setDecayToFind' failed in 'initialize' " << endmsg;
      return StatusCode::FAILURE;
    }
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// Set the decay descriptor of the MCdecayFinder tool based on the evtCode,
// after checking it exists
//=============================================================================
StatusCode EvtTypeChecker::setDecayToFind( const int evtCode ) const {

  // Check if code exist
  if ( !( m_evtTypeSvc->typeExists( evtCode ) ) ) {
    fatal() << "EvtCode " << evtCode << "is not known by the EvtTypeSvc" << endmsg;
    return StatusCode::FAILURE;
  }

  // Retrieve tool nd set decay descriptor
  m_mcFinder.retrieve().ignore();
  std::string sdecay = m_evtTypeSvc->decayDescriptor( evtCode );
  if ( ( m_mcFinder->setDecay( sdecay ) ).isFailure() ) {
    fatal() << "Unable to set decay for EvtCode " << evtCode << endmsg;
    return StatusCode::FAILURE;
  }

  m_setDecay = true;
  m_evtCode  = evtCode; // in case called when reading data

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
void EvtTypeChecker::operator()( const LHCb::GenHeader& header ) const {
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Execute" << endmsg;
  // If reading event type from data retrieve event header and set decay
  // unless already done (do it only for first time)
  if ( m_fromData.value() && !m_setDecay ) {
    setDecayToFind( header.evType() ).orThrow( " 'setDecayToFind' failed ", "EvtTypeChecker" ).ignore();
  }
  m_nMCFound += m_mcFinder->hasDecay();
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode EvtTypeChecker::finalize() {

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Finalize" << endmsg;

  auto decayAnalyzed = ( m_mcFinder ? m_mcFinder->decay() : "Unknown" );

  info() << endmsg << " EvtType analyzed = " << m_evtCode << endmsg << " Decay            = " << decayAnalyzed << endmsg
         << " NickName         = " << m_evtTypeSvc->nickName( m_evtCode ) << endmsg
         << "   events processed = " << format( "%8d", m_nMCFound.nEntries() ) << endmsg
         << "   events found     = " << format( "%8d", m_nMCFound.nTrueEntries() ) << endmsg
         << "   fraction/event   = " << format( "%8.2f", m_nMCFound.efficiency() ) << endmsg;

  return Consumer::finalize(); // must be called after all other actions
}

//=============================================================================
