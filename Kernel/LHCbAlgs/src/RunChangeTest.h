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
#ifndef COMPONENT_RUNCHANGETEST_H
#define COMPONENT_RUNCHANGETEST_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IIncidentListener.h"

class IEventTimeDecoder;
class IIncidentSvc;

namespace LHCbAlgsTests {

  /** @class RunChangeTest RunChangeTest.h component/RunChangeTest.h
   *
   *  Simple algorithm to test the RunChange incident.
   *  During the execute, it generates an ODIN object with incremented run number
   *  and calls the standard OdinTimeDecoder, which detects the change of run
   *  number and fires the incident.
   *  The incident is received by this algorithm and a message is printed.
   *
   *  Used in the test "lhcbalgs.runchange".
   *
   *  @author Marco CLEMENCIC
   *  @date   2008-07-24
   */
  class RunChangeTest final : public GaudiAlgorithm, virtual public IIncidentListener {

  public:
    /// Standard constructor
    RunChangeTest( const std::string& name, ISvcLocator* pSvcLocator );

    virtual ~RunChangeTest() = default; ///< Destructor

    StatusCode initialize() override; ///< Algorithm initialization
    StatusCode execute() override;    ///< Algorithm execution
    StatusCode finalize() override;   ///< Algorithm finalization

    /// Handle the ChangeRun incident
    void handle( const Incident& incident ) override;

  private:
    /// used to count the number of calls to decide which case to test
    size_t             m_counter{0};
    IEventTimeDecoder* m_eventTimeDecoder = nullptr;
    IIncidentSvc*      m_incSvc           = nullptr;
  };

} // namespace LHCbAlgsTests
#endif // COMPONENT_RUNCHANGETEST_H
