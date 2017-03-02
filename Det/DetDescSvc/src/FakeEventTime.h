#pragma once

// Include files
// from Gaudi
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/IEventTimeDecoder.h"

class IDataProviderSvc;

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

  // --- implementation of IEventTimeDecoder ---

  /// Return the time of current event.
  Gaudi::Time getTime() const override;

private:

  Gaudi::Property<long long> m_startTime { this, "StartTime",  0,
    "First event time (when simluating them)." };
  Gaudi::Property<long long> m_timeStep { this, "TimeStep",  0,
    "Event time increment (when simluating event times), 0 means no simulation." };
};
