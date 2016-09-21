
// local
#include "RichRawBankDecoder.h"

//-----------------------------------------------------------------------------
// Implementation file for class : RichRawBankDecoder
//
// 2016-09-21 : Chris Jones
//-----------------------------------------------------------------------------

using namespace Rich::Future;

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
RawBankDecoder::RawBankDecoder( const std::string& name, 
                                ISvcLocator* pSvcLocator )
  : Transformer ( name, pSvcLocator,
                  { KeyValue{ "RawEventLocations",
                        concat_alternatives( { LHCb::RawEventLocation::Rich,
                              LHCb::RawEventLocation::Default } ) },
                      KeyValue{ "OdinLocation", LHCb::ODINLocation::Default } },
                  { KeyValue{ "DecodedDataLocation", Rich::DAQ::L1MapLocation::Default } }
                  )
{ }

//=============================================================================

Rich::DAQ::L1Map 
RawBankDecoder::operator()( const LHCb::RawEvent&, const LHCb::ODIN& ) const
{
  
  
  return Rich::DAQ::L1Map();
}
