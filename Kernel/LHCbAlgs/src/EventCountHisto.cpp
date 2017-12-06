// Include files 

// local
#include "EventCountHisto.h"

//-----------------------------------------------------------------------------
// Implementation file for class : EventCountHisto
//
// 2007-01-08 : Robert Lambert
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( EventCountHisto )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
EventCountHisto::EventCountHisto( const std::string& name,
                                  ISvcLocator* pSvcLocator)
: GaudiHistoAlg ( name , pSvcLocator ) { }

//=============================================================================
// Main execution
//=============================================================================
StatusCode EventCountHisto::execute() {

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute" << endmsg;

  // code goes here

  ++m_nEvents;

  setFilterPassed(true);   // Mandatory. Set to true if event is accepted.
  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode EventCountHisto::finalize()
{

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Finalize" << endmsg;

  info() <<  m_nEvents << " events processed" << endmsg;

  const std::string tag="Events";
  const std::string name="N Events";
  plot(1., tag, name ,0.,2,3, m_nEvents);

  return GaudiHistoAlg::finalize();
}

//=============================================================================
