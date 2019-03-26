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
#ifndef TESTTIMEDECODERODIN_H
#define TESTTIMEDECODERODIN_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IEventTimeDecoder.h" // Interface

// forward declarations
class IDataProviderSvc;
class IIncidentSvc;
namespace LHCb {
  class ODIN;
}

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
  class TestTimeDecoderOdin final : public GaudiTool, virtual public IEventTimeDecoder {

  public:
    /// Standard constructor
    TestTimeDecoderOdin( const std::string& type, const std::string& name, const IInterface* parent );

    virtual ~TestTimeDecoderOdin() = default; ///< Destructor

    // --- implementation of IEventTimeDecoder ---
    /// Get the time of the current event from the ODIN object.
    /// @return The time of current event.
    Gaudi::Time getTime() const override;

  private:
    // --- local data ---
    /// Used to remember the run number and spot a change of run number.
    mutable unsigned int m_currentRun;

    /// How much to increase the run number at every call (default 1).
    bool m_runNumberStep;
  };

} // namespace LHCbAlgsTests

#endif // ODINTIMEDECODER_H
