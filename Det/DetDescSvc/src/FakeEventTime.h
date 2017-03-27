#pragma once

#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/IEventTimeDecoder.h"
#include "GaudiKernel/IIncidentSvc.h"

/** Basic implementation of an EventTimeDecoder.
 *  It just provides fake event times.
 *
 *  @author Marco Clemencic
 *  @date   2006-09-21
 */
class FakeEventTime : public extends<AlgTool, IEventTimeDecoder> {
public:
  /// Standard constructor
  using extends::extends;

  /// Initialization
  StatusCode initialize() override;
  StatusCode start() override;
  StatusCode stop() override;

  // --- implementation of IEventTimeDecoder ---

  /// Return the time of current event.
  Gaudi::Time getTime() const override;

private:

  Gaudi::Property<long long> m_startTime { this, "StartTime",  0,
    "First event time (when simluating them)." };
  Gaudi::Property<long long> m_timeStep { this, "TimeStep",  0,
    "Event time increment (when simluating event times), 0 means no simulation." };

  Gaudi::Property<unsigned long> m_startRun { this, "StartRun",  0,
    "First run number to simulate" };
  Gaudi::Property<unsigned long> m_eventsPerRun { this, "EventsPerRun",  0,
    "Number of events (calls) before changing run number, 0 means no change" };

  unsigned long m_evtCount = 0;

  SmartIF<IIncidentSvc> m_incSvc;

  /// internal method to increment the state
  void i_increment();
};
