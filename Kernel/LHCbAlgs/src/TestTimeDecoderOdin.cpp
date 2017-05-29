// Include files

#include <functional>

// detector description
#include "DetDesc/RunChangeIncident.h"
// data model
#include "Event/ODIN.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/IInterface.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/StateMachine.h"
#include "GaudiKernel/Time.icpp"
// local
#include "TestTimeDecoderOdin.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TestTimeDecoderOdin
//
// 2006-09-21 : Marco Clemencic
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_NAMESPACE_TOOL_FACTORY( LHCbAlgsTests, TestTimeDecoderOdin )

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
