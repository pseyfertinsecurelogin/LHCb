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
// Update 2016 S Ponce
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( STErrorDecoding )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
STErrorDecoding::STErrorDecoding( const std::string& name,
                                  ISvcLocator* pSvcLocator )
: Consumer(name, pSvcLocator,
           KeyValue{"RawEventLocations",
                     Gaudi::Functional::concat_alternatives(LHCb::RawEventLocation::Tracker,
                                                            LHCb::RawEventLocation::Other,
                                                            LHCb::RawEventLocation::Default)}) {}

//=============================================================================
// Main execution
//=============================================================================
void STErrorDecoding::operator()(const LHCb::RawEvent& raw) const {
  // in fact all the work is delegated to the base class
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute " << endmsg;
  auto errorBanks = decodeErrors(raw);
  // print out the error banks
  if ( m_PrintErrorInfo ) {
    for (const auto& b : *errorBanks) info() << b << endmsg;
  }
}
