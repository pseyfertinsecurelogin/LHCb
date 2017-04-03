#include "FakeEventTime.h"
#include "DetDesc/RunChangeIncident.h"

//-----------------------------------------------------------------------------
// Implementation file for class : FakeEventTime
//
// 2006-09-21 : Marco Clemencic
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( FakeEventTime )

#define ON_DEBUG if (msgLevel(MSG::DEBUG))
#define DEBUG_MSG ON_DEBUG debug()

//=========================================================================
//  Initialization
//=========================================================================
StatusCode FakeEventTime::initialize ( ) {
  // base class initialization
  StatusCode sc = base_class::initialize();
  if (!sc.isSuccess()) return sc;

  info() << "Event times generated from " << m_startTime.value() << " with steps of " << m_timeStep.value() << endmsg;
  info() << "Run numbers generated from " << m_startRun.value() << " every " << m_eventsPerRun.value() << " events" << endmsg;

  return sc;
}

StatusCode FakeEventTime::start() {
  StatusCode sc = base_class::start();

  m_incSvc = serviceLocator()->service("IncidentSvc", true);
  if ( !m_incSvc )
    throw GaudiException("Service [IncidentSvc] not found", name(),
                         StatusCode::FAILURE);

  return sc;
}

StatusCode FakeEventTime::stop() {
  m_incSvc.reset();
  return base_class::stop();
}

void FakeEventTime::i_increment() {
  m_startTime += m_timeStep;
  if (++m_evtCount == m_eventsPerRun) {
    m_evtCount = 0;
    m_startRun++;
  }
}

//=========================================================================
//  Return the time of current event
//=========================================================================
Gaudi::Time FakeEventTime::getTime ( ) const {

  // As a real EventTimeDecoder, the event time can be retrieved only in RUNNING
  // state, i.e. when the event can be loaded.
  if (FSMState() != Gaudi::StateMachine::RUNNING)
    return Gaudi::Time::epoch();

  // Here we should get the time from the EventDataSvc
  Gaudi::Time currentTime(m_startTime);
  if (m_evtCount == 0) {
    m_incSvc->fireIncident(RunChangeIncident(name(), m_startRun, currentTime));
  }

  // increment for the next event
  FakeEventTime *myPtr = const_cast<FakeEventTime *>(this);
  myPtr->i_increment();

  return currentTime;
}
//=============================================================================
