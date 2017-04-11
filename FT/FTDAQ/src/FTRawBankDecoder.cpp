// Include files

// from Gaudi
#include "Event/RawEvent.h"

// local
#include "FTRawBankDecoder.h"
#include "FTRawBankParams.h"

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
: Transformer ( name , pSvcLocator,
                KeyValue{ "RawEventLocations",
                          Gaudi::Functional::concat_alternatives( LHCb::RawEventLocation::Other,
                                                                  LHCb::RawEventLocation::Default )
                        },
                KeyValue{ "OutputLocation", LHCb::FTLiteClusterLocation::Default } )
{ }

//=============================================================================
// Main execution
//=============================================================================
FTLiteClusters
FTRawBankDecoder::operator()(const LHCb::RawEvent& rawEvent) const
{
  const auto& banks = rawEvent.banks(LHCb::RawBank::FTCluster);

  FastClusterContainer<LHCb::FTLiteCluster,int> clus {};

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
      throw GaudiException("Unsupported FT bank version",
                           "FTRawBankDecoder",
                           StatusCode::FAILURE);
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
        throw GaudiException("Inconsistent size of rawbank",
                             "FTRawBankDecoder",
                             StatusCode::FAILURE);
      }

      std::transform( first, first+nClus,
                      std::back_inserter(clus),
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

  // Sort clusters
  std::sort(clus.begin(), clus.end(),
            [](const LHCb::FTLiteCluster& lhs, const LHCb::FTLiteCluster& rhs){ return lhs.channelID() < rhs.channelID(); });

  return clus;
}
