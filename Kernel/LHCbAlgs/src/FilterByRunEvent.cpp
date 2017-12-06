// Include files

#include "Event/ODIN.h"

// local
#include "FilterByRunEvent.h"

//-----------------------------------------------------------------------------
// Implementation file for class : FilterByRunEvent
//
// 2007-01-18 : Patrick Spradlin
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FilterByRunEvent )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
FilterByRunEvent::FilterByRunEvent( const std::string& name,
                                    ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "RunEventNumList", m_events );
  declareProperty( "PassSelectedEvents", m_passSelect = true );
}

StatusCode FilterByRunEvent::initialize()
{
  const auto sc = GaudiAlgorithm::initialize();
  if ( !sc ) return sc;

  // sort the run/event list
  std::sort( m_events.begin(), m_events.end() );

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode FilterByRunEvent::execute()
{

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute" << endmsg;

  // code goes here
  setFilterPassed(!m_passSelect);

  // Get the run and event number from the ODIN bank
  const auto * odin = get<LHCb::ODIN> ( LHCb::ODINLocation::Default );

  // Get run/event from ODIn and make comparison object
  const auto runEv = std::make_pair( (int)odin->runNumber(),
                                     (int)odin->eventNumber() );

  const bool lcl_sel = std::any_of( m_events.begin(), m_events.end(),
                                    [&runEv]( const auto & p )
                                    { return runEv == p; } );

  const bool lcl_pass = ( m_passSelect ? lcl_sel : !lcl_sel );

  setFilterPassed( lcl_pass );

  if ( lcl_pass )
    info() << "Passing Run "   << odin->runNumber()
           << " event number " << odin->eventNumber() << endmsg;

  return StatusCode::SUCCESS;
}
//=============================================================================
