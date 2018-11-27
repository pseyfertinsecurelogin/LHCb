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
#ifndef SRC_TESTEVENTCOUNTER_H
#define SRC_TESTEVENTCOUNTER_H 1
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Kernel/IEventCounter.h"

namespace LHCbAlgsTests {

  /** @class TestEventCounter TestEventCounter.h src/TestEventCounter.h
   *
   *
   * @author Marco Clemencic
   * @date 11/06/2012
   */
  class TestEventCounter final : public GaudiAlgorithm
  {

  public:

    /// Standard constructor
    TestEventCounter(const std::string& name, ISvcLocator* pSvcLocator);
    virtual ~TestEventCounter() = default; ///< Destructor

    StatusCode initialize() override;    ///< Algorithm initialization
    StatusCode execute   () override;    ///< Algorithm execution
    StatusCode finalize  () override;    ///< Algorithm finalization

  private:

    std::string m_eventCounterName;

    IEventCounter* m_eventCounter = nullptr;

  };

} // namespace LHCbAlgsTests

#endif // SRC_TESTEVENTCOUNTER_H
