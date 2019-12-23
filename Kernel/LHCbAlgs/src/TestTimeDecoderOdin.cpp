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
#include "DetDesc/RunChangeIncident.h"
#include "Event/ODIN.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IEventTimeDecoder.h" // Interface
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TestTimeDecoderOdin
//
// 2006-09-21 : Marco Clemencic
//-----------------------------------------------------------------------------

namespace LHCbAlgsTests {

  /** @class TestTimeDecoderOdin TestTimeDecoderOdin.h
   *
   *  Fake EventTimeDecoder that generates an ODIN object with incremented run
   *  number at each call of getTime().
   *
   *  Used in the test "lhcbalgs.runchangehandler".
   *
   *  @author Marco Clemencic
   *  @date   2006-09-21
   */
  class TestTimeDecoderOdin final : public extends<GaudiTool, IEventTimeDecoder> {

  public:
    /// Standard constructor
    using extends::extends;

    // --- implementation of IEventTimeDecoder ---
    /// Get the time of the current event from the ODIN object.
    /// @return The time of current event.
    Gaudi::Time getTime() const override;

  private:
    // --- local data ---
    /// Used to remember the run number and spot a change of run number.
    mutable Gaudi::Property<unsigned int> m_currentRun{this, "FirstRunNumber", 1};

    /// How much to increase the run number at every call (default 1).
    Gaudi::Property<bool>             m_runNumberStep{this, "RunNumberStep", 1};
    DataObjectWriteHandle<LHCb::ODIN> m_odin{this, "ODINLocation", LHCb::ODINLocation::Default};
  };

} // namespace LHCbAlgsTests

// Declaration of the Tool Factory
DECLARE_COMPONENT( LHCbAlgsTests::TestTimeDecoderOdin )

using namespace LHCbAlgsTests;

//=========================================================================
//  Return the time of current event
//=========================================================================
Gaudi::Time TestTimeDecoderOdin::getTime() const {

  // As a real EventTimeDecoder, the event time can be retrieved only in RUNNING
  // state, i.e. when the event can be loaded.
  if ( FSMState() != Gaudi::StateMachine::RUNNING ) return Gaudi::Time::epoch();

  static Gaudi::Time last_time( 0 );
  static bool        first = true;

  auto odin = std::make_unique<LHCb::ODIN>();

  if ( UNLIKELY( first ) ) {
    odin->setRunNumber( m_currentRun.value() );
    first = false;
  } else {
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
      debug() << "Firing " << IncidentType::RunChange << " incident. Old run=" << m_currentRun.value();
    m_currentRun += m_runNumberStep.value();
    odin->setRunNumber( m_currentRun.value() );
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << ", new run=" << m_currentRun.value() << endmsg;
  }

  last_time = odin->eventTime();

  incSvc()->fireIncident( RunChangeIncident( name(), m_currentRun, last_time ) );
  m_odin.put( std::move( odin ) );

  return last_time;
}
//=============================================================================
