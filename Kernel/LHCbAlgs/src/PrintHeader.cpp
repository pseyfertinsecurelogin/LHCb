// Include files 

#include <functional>

// from EventSys
#include "Event/ODIN.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
// local
#include "PrintHeader.h"

class ISvcLocator;

//-----------------------------------------------------------------------------
// Implementation file for class : PrintHeader
//
// 2003-03-16 : Gloria Corti
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory

DECLARE_ALGORITHM_FACTORY( PrintHeader )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PrintHeader::PrintHeader( const std::string& name,
                          ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty("PrintFileName",  m_printFile = false ); 
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PrintHeader::execute() 
{
  // count events
  ++m_nEvents;

  // Get event number from the ODIN bank
  auto * odin = getIfExists<LHCb::ODIN> ( LHCb::ODINLocation::Default );
  if ( odin )
  {
    info() << "Run "     << odin->runNumber()
           << ", Event " << odin->eventNumber() << endmsg;
  } else { Warning("No ODIN Header").ignore(); }
       
  setFilterPassed(true);
  
  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode PrintHeader::finalize()
{
  info() << "Filtered " << m_nEvents << " events" << endmsg;
  return GaudiAlgorithm::finalize();  // must be called after all other actions
}

//=============================================================================
