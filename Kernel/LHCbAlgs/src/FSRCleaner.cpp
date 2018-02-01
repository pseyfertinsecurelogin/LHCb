// Include files

// local
#include "FSRCleaner.h"

//-----------------------------------------------------------------------------
// Implementation file for class : FSRCleaner
//
// 2012-08-01 : Rob Lambert
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
FSRCleaner::FSRCleaner( const std::string& name,
                        ISvcLocator* pSvcLocator )
  : EmptyEventNodeCleaner( name , pSvcLocator )
{
  // Set sensible FSR defaults
  setProperty( "InputStream", "/FileRecords" );
  setProperty( "DataService", "FileRecordDataSvc" );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode FSRCleaner::finalize()
{
  // Clean performed in execute of EmptyEventNodeCleaner... so let's call it!
  const StatusCode sc = EmptyEventNodeCleaner::execute();

  // return, try to avoid shortcutting the usual finalize
  return StatusCode{ EmptyEventNodeCleaner::finalize() && sc };
}

//=============================================================================

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FSRCleaner )

//=============================================================================
