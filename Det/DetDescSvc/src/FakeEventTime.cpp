// Include files

// from Gaudi
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/IDataProviderSvc.h"

// local
#include "FakeEventTime.h"

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

  return sc;
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

 // increment for the next event
  FakeEventTime *myPtr = const_cast<FakeEventTime *>(this);
  myPtr->m_startTime += m_timeStep;

  return currentTime;
}
//=============================================================================
