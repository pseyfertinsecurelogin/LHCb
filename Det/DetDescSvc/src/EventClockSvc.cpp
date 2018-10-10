/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files
#include "GaudiKernel/IDetDataSvc.h"
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Time.h"
#include "GaudiKernel/IEventTimeDecoder.h"

// local
#include "EventClockSvc.h"

DECLARE_COMPONENT( EventClockSvc )

//-----------------------------------------------------------------------------
// Implementation file for class : EventClockSvc
//
// 2005-07-08 : Marco CLEMENCIC
//-----------------------------------------------------------------------------

//=========================================================================
//
//=========================================================================
StatusCode EventClockSvc::initialize() {
  // base class initialization
  StatusCode sc = Service::initialize();
  if (!sc.isSuccess()) return sc;

  if( msgLevel(MSG::DEBUG) )
    debug() << "--- initialize ---" << endmsg;

  // find the detector data svc
  m_detDataSvc = service(m_detDataSvcName,true);
  if (!m_detDataSvc) {
    error() << "Unable to get a handle to the detector data service" << endmsg;
    return StatusCode::FAILURE;
  }
  if( msgLevel(MSG::DEBUG) )
    debug() << "Got pointer to IDetDataSvc \"" << m_detDataSvcName.value() << '"' << endmsg;

  // get a pointer to the tool service
  m_toolSvc = service( "ToolSvc", true );
  if (!m_toolSvc) {
    error() << "Unable to get a handle to the tool service" << endmsg;
    return StatusCode::FAILURE;
  }
  if( msgLevel(MSG::DEBUG) )
      debug() << "Got pointer to ToolSvc " << endmsg;

  sc = m_toolSvc->retrieveTool(m_eventTimeDecoderName, m_eventTimeDecoder, this);
  if (!sc.isSuccess()) {
    error() << "Unable to get a handle to the IEventTimeDecoder \"" << m_eventTimeDecoderName.value() << '"' << endmsg;
    return sc;
  }
  if( msgLevel(MSG::DEBUG) )
      debug() << "Got pointer to IEventTimeDecoder \"" <<  m_eventTimeDecoderName.value() << '"' << endmsg;

  // Set the first event time at initialization.
  Gaudi::Time initTime = m_eventTimeDecoder->getTime(); // try to get the actual event time
  if (initTime.ns() == 0) {
    // no event time available yet, use the configuration
    initTime = ( m_initialTime ? Gaudi::Time(m_initialTime)
                               : Gaudi::Time::current() );
  }
  if( msgLevel(MSG::DEBUG) )
    debug() << "Initialize event time to " << initTime << endmsg;
  m_detDataSvc->setEventTime(initTime);

  // register to the incident service for BeginEvent incidents
  m_incidentSvc = service("IncidentSvc", false);
  if ( m_incidentSvc ) {
    m_incidentSvc->addListener(this,IncidentType::BeginEvent);
    if( msgLevel(MSG::DEBUG) )
      debug() << "Got pointer to IncidentSvc" << endmsg;
  } else {
    warning() << "Unable to register to the incident service." << endmsg;
  }
  return StatusCode::SUCCESS;
}

//=========================================================================
//
//=========================================================================
StatusCode EventClockSvc::finalize ( ) {
  // local finalization
  if( msgLevel(MSG::DEBUG) )
    debug() << "--- finalize ---" << endmsg;

  // release the interfaces used
  if (m_toolSvc && m_eventTimeDecoder) {
      m_toolSvc->releaseTool(m_eventTimeDecoder).ignore();
  }
  m_toolSvc.reset();
  m_detDataSvc.reset();
  if (m_incidentSvc) {
    // unregister from the incident svc
    m_incidentSvc->removeListener(this,IncidentType::BeginEvent);
  }
  m_incidentSvc.reset();

  return Service::finalize();
}
//=========================================================================
//  Handle BeginEvent incident
//=========================================================================
void EventClockSvc::handle(const Incident &inc) {
  if ( inc.type() == IncidentType::BeginEvent ) {
    m_detDataSvc->setEventTime(m_eventTimeDecoder->getTime());
    if( msgLevel(MSG::DEBUG) ) {
      debug() << "New BeginEvent incident received" << endmsg;
      debug() << "DetDataSvc::eventTime() -> " << m_detDataSvc->eventTime() << endmsg;
    }
  } else {
    warning() << inc.type() << " incident ignored" << endmsg;
  }
}
//=============================================================================
