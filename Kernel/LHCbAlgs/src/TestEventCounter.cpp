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
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Kernel/IEventCounter.h"

// ----------------------------------------------------------------------------
// Implementation file for class: TestEventCounter
//
// 11/06/2012: Marco Clemencic
// ----------------------------------------------------------------------------
namespace LHCbAlgsTests {

  /** @class TestEventCounter TestEventCounter.h src/TestEventCounter.h
   *
   *
   * @author Marco Clemencic
   * @date 11/06/2012
   */
  class TestEventCounter final : public GaudiAlgorithm {
    PublicToolHandle<IEventCounter> m_eventCounter{this, "EvtCounter", "EvtCounter",
                                                   "Type/Name of the (public) event counter to use."};

  public:
    /// Standard constructor
    using GaudiAlgorithm::GaudiAlgorithm;

    StatusCode execute() override {
      if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;
      info() << "Event count = " << m_eventCounter->getEventCounter() << endmsg;
      return StatusCode::SUCCESS;
    }
  };

  DECLARE_COMPONENT( TestEventCounter )

  class ForceEventCounter final : public Gaudi::Algorithm {
    mutable PublicToolHandle<IEventCounter> m_cntr{this, "Counter", "EvtCounter/Forcing"};

  public:
    using Gaudi::Algorithm::Algorithm;
    StatusCode execute( const EventContext& ctx ) const override {
      m_cntr->setEventCounter( ctx.evt() * 10 );
      return StatusCode::SUCCESS;
    }
  };
  DECLARE_COMPONENT( ForceEventCounter )
} // namespace LHCbAlgsTests
// ============================================================================
