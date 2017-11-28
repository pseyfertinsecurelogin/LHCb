// Include files
#include <algorithm>

// Event
#include "Event/RawBank.h"
#include "Event/RawEvent.h"

#include "GaudiAlg/FunctionalUtilities.h"

// local
#include "HltLumiSummaryDecoder.h"

namespace {

template <typename T>
std::atomic<T>& operator+=( std::atomic<T>& x, T inc ) {
   auto current = x.load();
   while (!x.compare_exchange_weak(current, current + inc)) /* empty on purpose*/ ;
   return x;
}

}

using namespace LHCb;

DECLARE_ALGORITHM_FACTORY( HltLumiSummaryDecoder )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
HltLumiSummaryDecoder::HltLumiSummaryDecoder( const std::string& name,
                                              ISvcLocator* pSvcLocator)
: Transformer( name , pSvcLocator,
               KeyValue{"RawEventLocations",
                    Gaudi::Functional::concat_alternatives( LHCb::RawEventLocation::Trigger,
                                                            LHCb::RawEventLocation::Default)},
               KeyValue{"OutputContainerName", LHCb::HltLumiSummaryLocation::Default})
{
}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode HltLumiSummaryDecoder::initialize()
{
  StatusCode sc = Transformer::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  m_nbEvents    = 0;
  m_totDataSize = 0;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
HltLumiSummary HltLumiSummaryDecoder::operator() (const RawEvent& event) const {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // This should not exist already
  LHCb::HltLumiSummary hltLumiSummary;

  // Get the buffers associated with the HltLumiSummary
  const auto& banks = event.banks( RawBank::HltLumiSummary );
  // Now copy the information from all banks (normally there should only be one)
  for (const auto& ibank : banks ) {
    // get now the raw data
    const unsigned int* idata = ibank->data() ;

    // The data part
    const unsigned int* begin = idata ;
    const unsigned int* end   = idata + ibank->size()/sizeof( unsigned int ) ;
    for( const unsigned int* itW = begin; end != itW; itW++ ) {
      // decode the info
      int iKey = (*itW >> 16);
      int iVal = (*itW & 0xFFFF);
      if ( MSG::VERBOSE >= msgLevel() ) {
        verbose() << format ( " %8x %11d %11d %11d ", *itW, *itW, iKey, iVal )
                  << endmsg;
      }
      // add this counter
      hltLumiSummary.addInfo( iKey, iVal);
    }

    // keep statistics
    int totDataSize =  ibank->size()/sizeof( unsigned int );
    m_totDataSize += double(totDataSize);

    m_nbEvents++;

    if ( msgLevel( MSG::DEBUG ) ) {
      debug() << "Bank size: ";
      debug() << format( "%4d ", ibank->size() )
	      << "Total Data bank size " << totDataSize << endmsg;
    }
  }

  return hltLumiSummary;
}

//=============================================================================
// Finalize
//=============================================================================
StatusCode HltLumiSummaryDecoder::finalize()
{
  if ( 0 < m_nbEvents ) {
    info() << "Average event size : "
           << format( "%7.1f words", m_totDataSize / m_nbEvents )
           << endmsg;
  }
  return Transformer::finalize(); // must be called after all other actions
}
