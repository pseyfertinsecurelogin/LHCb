#ifndef EVTCOUNTER_H
#define EVTCOUNTER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IIncidentListener.h"
#include "Kernel/IEventCounter.h"

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
  virtual void handle(const Incident&) { this->incrementEventCounter(); }

  /// Initialize method
  StatusCode initialize();

  /// Get Event counter
  unsigned long long getEventCounter() const noexcept { return m_eventCounter; }

  /// Increment Event counter
  void incrementEventCounter() { ++m_eventCounter; }

  /// Set Event counter
  void setEventCounter( unsigned long long n ) { m_eventCounter = n; }

 private:

  unsigned long long m_eventCounter{0}; ///< number of events processed
  unsigned long long m_initialCount; ///< allow to change the number used for the first event (property InitialCount)
  bool m_useIncident; ///< whether to increment of BeginEvent incident or not

};

#endif // EVTCOUNTER_H
