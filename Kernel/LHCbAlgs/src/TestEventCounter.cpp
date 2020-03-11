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

// local
#include "TestEventCounter.h"

using namespace LHCbAlgsTests;

// ----------------------------------------------------------------------------
// Implementation file for class: TestEventCounter
//
// 11/06/2012: Marco Clemencic
// ----------------------------------------------------------------------------
DECLARE_COMPONENT( TestEventCounter )

// ============================================================================
// Standard constructor, initializes variables
// ============================================================================
TestEventCounter::TestEventCounter( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {
  declareProperty( "EvtCounter", m_eventCounterName = "EvtCounter", "Type/Name of the (public) event counter to use." );
}

// ============================================================================
// Initialization
// ============================================================================
StatusCode TestEventCounter::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  m_eventCounter = tool<IEventCounter>( m_eventCounterName );

  return sc;
}

// ============================================================================
// Main execution
// ============================================================================
StatusCode TestEventCounter::execute() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  info() << "Event count = " << m_eventCounter->getEventCounter() << endmsg;

  return StatusCode::SUCCESS;
}

// ============================================================================
// Finalize
// ============================================================================
StatusCode TestEventCounter::finalize() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Finalize" << endmsg;

  release( m_eventCounter ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  m_eventCounter = nullptr;

  return GaudiAlgorithm::finalize(); // must be called after all other actions
}

namespace LHCbAlgsTests {
  struct ForceEventCounter : Gaudi::Algorithm {
    using Gaudi::Algorithm::Algorithm;
    StatusCode execute( const EventContext& ctx ) const override {
      m_cntr->setEventCounter( ctx.evt() * 10 );
      return StatusCode::SUCCESS;
    }

  private:
    mutable PublicToolHandle<IEventCounter> m_cntr{this, "Counter", "EvtCounter/Forcing"};
  };
  DECLARE_COMPONENT( ForceEventCounter )
} // namespace LHCbAlgsTests
// ============================================================================
