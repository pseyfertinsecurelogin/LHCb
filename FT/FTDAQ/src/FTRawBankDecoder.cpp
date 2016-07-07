// Include files

// from Gaudi
#include "Event/FTLiteCluster.h"
#include "Event/FTCluster.h"
#include "Event/RawEvent.h"
#include "GaudiAlg/FunctionalUtilities.h"

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

namespace {

StatusCode retrieveModuleMat(int quartSipmNb, int quarter, unsigned &locmod, unsigned &locmat)
{
  if (UNLIKELY(quarter<0 || quarter>3 || quartSipmNb > 127)) return StatusCode::FAILURE;

  locmat = ( quarter < 2 ? 1 : 0 );
  int intermod = quartSipmNb / 16;
  if((quarter % 2) != 0) { // x positive part
    locmod = ( intermod < 5 ? intermod : 10 );
  } else {  // x negative part
    locmod = ( intermod > 0 ? intermod + 4 : 11 );
  }

  return StatusCode::SUCCESS;
}

}
//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  FTRawBankDecoder::FTRawBankDecoder( const std::string& name,
                                      ISvcLocator* pSvcLocator)
: Transformer ( name ,
                pSvcLocator,
                { KeyValue{ "RawEventLocations",
                            Gaudi::Functional::concat_alternatives( { LHCb::RawEventLocation::Other,
                                                                      LHCb::RawEventLocation::Default } )
                          } },
                KeyValue{ "OutputLocation", LHCb::FTLiteClusterLocation::Default } )
{ }

//=============================================================================
// Main execution
//=============================================================================

FastClusterContainer<LHCb::FTLiteCluster,int>
FTRawBankDecoder::operator()(const LHCb::RawEvent& rawEvent) const
{
  const auto& banks = rawEvent.banks(LHCb::RawBank::FTCluster);

  FastClusterContainer<LHCb::FTLiteCluster,int> clus {};

  if ( msgLevel(MSG::DEBUG) ) debug() << "Number of raw banks " << banks.size() << endmsg;

  for ( const LHCb::RawBank* bank : banks) {
    int source  = bank->sourceID();
    unsigned layer   = source/4;
    int quarter = source & 3;
    auto size    = bank->size(); // in bytes, multiple of 4

    if ( msgLevel(MSG::DEBUG) ) debug() << "source " << source << " layer "
                                        << layer << " quarter "
                                        << quarter << " size " << size << endmsg;
                                        
    if ( 1 != bank->version()  ) {
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
      if ( first == last && 0 == sipmHeader ) continue;  // padding at the end...
      int QuarterSiPMNber = ( sipmHeader >> FTRawBank::sipmShift );
      unsigned module = 99;
      unsigned mat = 9;

      StatusCode sc = retrieveModuleMat(QuarterSiPMNber,quarter,module,mat);
      if(UNLIKELY(sc.isFailure())) throw GaudiException("TODO: Describe this failure mode",
                                                        "FTRawBankDecoder",
                                                        StatusCode::FAILURE);

      unsigned mySiPM = QuarterSiPMNber & 15;
      int nClus  = ( sipmHeader & FTRawBank::nbClusMaximum );
      if (UNLIKELY( msgLevel(MSG::DEBUG) && nClus > 0) )
        debug() << "   SiPM " << mySiPM << " nClus " << nClus
                << " size " << std::distance(first,last) << endmsg;
      if (UNLIKELY(nClus>std::distance(first,last))) {
        warning() << " inconsistent size of rawbank " << endmsg;
        throw GaudiException("Inconsistent size of rawbank",
                             "FTRawBankDecoder",
                             StatusCode::FAILURE);
      }
      std::transform( first, first+nClus,
                      std::back_inserter(clus),
                      [&](short int c) -> LHCb::FTLiteCluster {
        int fraction = ( c >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum;
        unsigned cell     = ( c >> FTRawBank::cellShift     ) & FTRawBank::cellMaximum;
        int sipmId   = ( c >> FTRawBank::sipmIdShift   ) & FTRawBank::sipmIdMaximum;
        int cSize    = 1+(( c >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum); // add 1 to make sure to keep clusters with size 1,2,3 and 4 using 2 bits to encode the cluster size in the data format
        int charge   = ( c >> FTRawBank::chargeShift   ) & FTRawBank::chargeMaximum;
        if ( msgLevel( MSG::VERBOSE ) ) {
          verbose() << format(  "  cell %4d sipmId %3d frac %3d charge %5d size %3d code %4.4x",
                                cell, sipmId,fraction, charge, cSize, c ) << endmsg;
        }
        return  { LHCb::FTChannelID{ layer, module, mat, mySiPM, cell },
                  fraction, cSize, charge };
      } );
      first += nClus;
    }
  }
  
  return clus;
}
