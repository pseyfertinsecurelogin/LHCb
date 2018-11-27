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
// Include files

// from Gaudi
#include "GaudiKernel/Incident.h"
#include "GaudiKernel/IIncidentSvc.h"

// detector description
#include "DetDesc/RunChangeIncident.h"

// data model
#include "Event/ODIN.h"

// local
#include "TestTimeDecoderOdin.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TestTimeDecoderOdin
//
// 2006-09-21 : Marco Clemencic
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( LHCbAlgsTests::TestTimeDecoderOdin )

using namespace LHCbAlgsTests;
//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TestTimeDecoderOdin::TestTimeDecoderOdin( const std::string& type,
                                  const std::string& name,
                                  const IInterface* parent )
  : GaudiTool ( type, name , parent )
{
  declareInterface<IEventTimeDecoder>(this);
  declareProperty("FirstRunNumber", m_currentRun=1);
  declareProperty("RunNumberStep", m_runNumberStep=1);
}

//=========================================================================
//  Return the time of current event
//=========================================================================
Gaudi::Time TestTimeDecoderOdin::getTime ( ) const
{

  // As a real EventTimeDecoder, the event time can be retrieved only in RUNNING
  // state, i.e. when the event can be loaded.
  if (FSMState() != Gaudi::StateMachine::RUNNING)
    return Gaudi::Time::epoch();

  static Gaudi::Time last_time(0);
  static bool first = true;

  LHCb::ODIN *odin = new LHCb::ODIN();

  if ( UNLIKELY(first) )
  {
    odin->setRunNumber(m_currentRun);
    first = false;
  }
  else
  {
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << "Firing " << IncidentType::RunChange << " incident. Old run="
              << m_currentRun;
    m_currentRun += m_runNumberStep;
    odin->setRunNumber(m_currentRun);
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << ", new run=" << m_currentRun << endmsg;
  }

  put(odin,LHCb::ODINLocation::Default);

  last_time = odin->eventTime();

  incSvc()->fireIncident(RunChangeIncident(name(), m_currentRun, last_time));

  return last_time;
}
//=============================================================================
