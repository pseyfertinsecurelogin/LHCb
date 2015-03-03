// $Id: PrintHeader.cpp,v 1.5 2009-03-17 10:58:30 cattanem Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/AlgFactory.h"

// from EventSys
#include "Event/ODIN.h"

// local
#include "PrintHeader.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PrintHeader
//
// 2003-03-16 : Gloria Corti
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory

DECLARE_ALGORITHM_FACTORY( PrintHeader );


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PrintHeader::PrintHeader( const std::string& name,
                          ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
  , m_nEvents(0)
{
  declareProperty("PrintFileName",  m_printFile = false ); 
}

//=============================================================================
// Destructor
//=============================================================================
PrintHeader::~PrintHeader() {}; 


//=============================================================================
// Main execution
//=============================================================================
StatusCode PrintHeader::execute() {

  m_nEvents++;

  // Get event number from the ODIN bank
  if ( exist<LHCb::ODIN>( LHCb::ODINLocation::Default )){
    LHCb::ODIN* odin = get<LHCb::ODIN> ( LHCb::ODINLocation::Default );
    info() << "Run "     << odin->runNumber()
           << ", Event " << odin->eventNumber() << endmsg;
  } else Warning("No ODIN Header").ignore();
       
  setFilterPassed(true);
  
  return StatusCode::SUCCESS;
};

//=============================================================================
//  Finalize
//=============================================================================
StatusCode PrintHeader::finalize() {

  info() << "Filtered " << m_nEvents << " events" << endmsg;

  return GaudiAlgorithm::finalize();  // must be called after all other actions
}

//=============================================================================
