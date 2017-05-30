// Include files

// local
#include "STErrorDecoding.h"

// Event
#include "Event/STTELL1BoardErrorBank.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : STErrorDecoding
//
// 2007-11-29: Mathias Knecht
// Update 2008 M Needham
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( STErrorDecoding )

//=============================================================================
// Main execution
//=============================================================================
StatusCode STErrorDecoding::execute() {

  // in fact all the work is delegated to the base class

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute " << endmsg;

  STTELL1BoardErrorBanks* errorBanks = getErrorBanks();

  // was it not possible to decode the banks?
  if (!errorBanks) return StatusCode::FAILURE;

  // print out the error banks
  if ( m_PrintErrorInfo ){
    for (const auto& b : *errorBanks) info() << b << endmsg;
  }

  return StatusCode::SUCCESS;
}

