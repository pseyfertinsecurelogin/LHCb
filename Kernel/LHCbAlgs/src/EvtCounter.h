#ifndef EVTCOUNTER_H
#define EVTCOUNTER_H 1

#include <string>
#include <vector>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/extends.h"
#include "Kernel/IEventCounter.h"

class IIncidentListener;
class IInterface;
class Incident;
struct IEventCounter;

/** @class EvtCounter EvtCounter.h
 *
 *  @author Patrick KOPPENBURG
 *  @date   2004-06-15
 */
class EvtCounter final : public extends2<GaudiTool,
                                         IEventCounter,
                                         IIncidentListener>
{

 public:

  /// Standard constructor
  EvtCounter( const std::string& type,
              const std::string& name,
              const IInterface* parent);

  virtual ~EvtCounter() = default; ///< Destructor

  /// IListener interface to be triggered at begin of event
  void handle(const Incident&) override { this->incrementEventCounter(); }

  /// Initialize method
  StatusCode initialize() override;

  /// Get Event counter
  unsigned long long getEventCounter() const noexcept override { return m_eventCounter; }

  /// Increment Event counter
  void incrementEventCounter() override { ++m_eventCounter; }

  /// Set Event counter
  void setEventCounter( unsigned long long n ) override { m_eventCounter = n; }

 private:

  unsigned long long m_eventCounter{0}; ///< number of events processed
  unsigned long long m_initialCount; ///< allow to change the number used for the first event (property InitialCount)
  bool m_useIncident; ///< whether to increment of BeginEvent incident or not

};

#endif // EVTCOUNTER_H
