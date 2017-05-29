// Include files

#include <algorithm>
#include <iterator>

#include "Event/RawBank.h"
// from Gaudi
#include "Event/RawEvent.h"
// local
#include "FTRawBankDecoder.h"
#include "FTRawBankParams.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/MsgStream.h"
#include "Kernel/FTChannelID.h"

class ISvcLocator;

//-----------------------------------------------------------------------------
// Implementation file for class : FTRawBankDecoder
//
// 2012-05-11 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( FTRawBankDecoder )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  FTRawBankDecoder::FTRawBankDecoder( const std::string& name,
                                      ISvcLocator* pSvcLocator)
: Decoder::AlgBase ( name , pSvcLocator )
{
  //new for decoders, initialize search path, and then call the base method
  m_rawEventLocations = {LHCb::RawEventLocation::Other, LHCb::RawEventLocation::Default};
  initRawEventSearch();
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode FTRawBankDecoder::execute() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  // Retrieve the RawEvent:
  LHCb::RawEvent* rawEvent = findFirstRawEvent();

  if ( !rawEvent ) {
    if( msgLevel( MSG::DEBUG ) )
      debug() << "Raw Event not found in " << m_rawEventLocations << endmsg;
    return StatusCode::SUCCESS;
  }

  auto clus = (*this)( rawEvent->banks(LHCb::RawBank::FTCluster) );
  if (!clus) return StatusCode::FAILURE;
  if( msgLevel( MSG::DEBUG ) )
    debug() << "Created " << clus->size() << " FTLiteClusters" << endmsg;
  put( clus.release(), m_outputClusterLocation);
  return StatusCode::SUCCESS;
}

std::unique_ptr<FTLiteClusters>
FTRawBankDecoder::operator()(const std::vector<LHCb::RawBank*>& banks) const
{
  auto clus = std::make_unique<FTLiteClusters>();

  if ( msgLevel(MSG::DEBUG) ) debug() << "Number of raw banks " << banks.size() << endmsg;

  for ( const LHCb::RawBank* bank : banks) {
    int source  = bank->sourceID();
    unsigned station = source/16 + 1u; // JvT: this should be done by a mapping!
    unsigned layer   = (source & 12) /4;  // JvT: this should be done by a mapping!
    unsigned quarter = source & 3; // JvT: this should be done by a mapping!
    auto size    = bank->size(); // in bytes, multiple of 4

    if ( msgLevel(MSG::VERBOSE) ) verbose() << "source " << source
        << " station " << station << " layer " << layer
        << " quarter " << quarter << " size " << size << endmsg;
    if ( bank->version() != 2 ) {
      error() << "** Unsupported FT bank version " << bank->version()
              << " for source " << source << " size " << size << " bytes."
              << endmsg;
      return nullptr;
    }

    auto first = bank->begin<short int>();
    auto last = bank->end<short int>();
    while( first != last ) {
      int sipmHeader = *first++;
      if ( first == last && sipmHeader == 0 ) continue;  // padding at the end...
      unsigned modulesipm = ( sipmHeader >> FTRawBank::sipmShift );
      unsigned module = modulesipm >> FTRawBank::sipmShift ;
      unsigned mat  = (modulesipm & 15 ) >> 2; // hardcoded: this should be replaced by mapping
      unsigned sipm = modulesipm & 3;          // hardcoded: this should be replaced by mapping
      int nClus  = ( sipmHeader & FTRawBank::nbClusMaximum );
      if (UNLIKELY( msgLevel(MSG::VERBOSE) && nClus > 0) )
        verbose() << "  Module " << module << " mat " << mat << " SiPM " << sipm
                  << " nClusters " << nClus << endmsg;
      if (UNLIKELY(nClus>std::distance(first,last))) {
        warning() << "Inconsistent size of rawbank. #clusters in header="
                  << nClus << ", #clusters in bank=" << std::distance(first,last) << endmsg;
        return nullptr;
      }

      std::transform( first, first+nClus,
                      std::back_inserter(*clus),
                      [&](short int c) -> LHCb::FTLiteCluster {
        unsigned channel= ( c >> FTRawBank::cellShift     ) & FTRawBank::cellMaximum;
        int fraction = ( c >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum;
        int cSize    = ( c >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;
        if ( msgLevel( MSG::VERBOSE ) ) {
          verbose() << format(  "    channel %4d frac %3d size %3d code %4.4x",
                                channel, fraction, cSize, c ) << endmsg;
        }
        return  { LHCb::FTChannelID{ station, layer, quarter, module, mat, sipm, channel },
                  fraction, cSize };
      } );
      first += nClus;
    }
  }
  return clus;
}
