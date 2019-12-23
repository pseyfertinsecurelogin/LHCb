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
#include "Event/ODIN.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IEventTimeDecoder.h"
#include <atomic>

// ----------------------------------------------------------------------------
// Implementation file for class: ODINBadTimeTest
//
// 18/10/2011: Marco Clemencic
// ----------------------------------------------------------------------------

namespace LHCbAlgsTests {

  /** @class ODINBadTimeTest ODINBadTimeTest.h src/ODINBadTimeTest.h
   *
   * Small test algorithm to test the handling of invalid GPS time in OdinTimeDecoder.
   *
   * @author Marco Clemencic
   * @date 18/10/2011
   */
  class ODINBadTimeTest final : public GaudiAlgorithm {
    ToolHandle<IEventTimeDecoder>     m_evtTimeTool{"OdinTimeDecoder", this};
    mutable std::atomic<bool>         m_called{false};
    DataObjectWriteHandle<LHCb::ODIN> m_odin{this, "ODINLocation", LHCb::ODINLocation::Default};

  public:
    /// Standard constructor
    using GaudiAlgorithm::GaudiAlgorithm;

    StatusCode execute() override; ///< Algorithm execution
  };

  DECLARE_COMPONENT( ODINBadTimeTest )

  // ============================================================================
  // Main execution
  // ============================================================================
  StatusCode ODINBadTimeTest::execute() {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

    LHCb::ODIN* odin   = new LHCb::ODIN();
    bool        called = m_called.exchange( true );
    if ( !called ) {
      // valid GPS time on the first call
      odin->setGpsTime( 1318944600000000ULL ); // 18/10/2011 15:30
    } else {
      // invalid GPS time on the following calls
      odin->setGpsTime( 9239754600000000ULL ); // 18/10/2262 15:30
    }

    info() << "Set ODIN time to " << odin->gpsTime() / 1000000 << " (" << ( called ? "invalid" : "valid" ) << ")"
           << endmsg;

    m_odin.put( std::move( odin ) );
    // will not try to decode the ODIN bank
    info() << "ODIN time -> " << m_evtTimeTool->getTime() << endmsg;

    return StatusCode::SUCCESS;
  }

  // ============================================================================
} // namespace LHCbAlgsTests
