// Include files

// from Gaudi
#include "Event/RawEvent.h"

// local
#include "FTRawBankDecoder.h"
#include "FTRawBankParams.h"

#include "boost/container/static_vector.hpp"

//-----------------------------------------------------------------------------
// Implementation file for class : FTRawBankDecoder
//
// 2012-05-11 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FTRawBankDecoder )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
FTRawBankDecoder::FTRawBankDecoder( const std::string& name,
                                    ISvcLocator* pSvcLocator)
: Transformer ( name , pSvcLocator,
                KeyValue{ "RawEventLocations",
                    Gaudi::Functional::concat_alternatives( LHCb::RawEventLocation::Other,
                                                            LHCb::RawEventLocation::Default )},
                KeyValue{ "OutputLocation", LHCb::FTLiteClusterLocation::Default } )
{ }


StatusCode FTRawBankDecoder::initialize()
{
  StatusCode sc = Transformer::initialize();
  if (sc.isFailure()) return Error("Failed to initialize", sc);
  m_readoutTool = this -> template tool<IFTReadoutTool>("FTReadoutTool","FTReadoutTool");
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
FTLiteClusters
FTRawBankDecoder::operator()(const LHCb::RawEvent& rawEvent) const
{
  const auto& banks = rawEvent.banks(LHCb::RawBank::FTCluster);
  
  // Simple loop over the banks to determine total number of clusters
  // Bank size is half the number of clusters and includes headers
  // In the future, the #clusters should be encoded in raw bank.
  FTLiteClusters clus;
  int totSize = 0;
  //  clus.reserve(banks.size());
  
  // Store partition points for Tell40 for faster sorting
  boost::container::static_vector<int,FTRawBank::NbBanks> partitionPoints;
  if ( msgLevel(MSG::DEBUG) ) debug() << "Number of raw banks " << banks.size() << endmsg;
  
  //Testing the bank version
  unsigned int version = banks[0]->version();
  if (version != 4)
    {
      error() << "** Unsupported FT bank version " << version
              << endmsg;
      throw GaudiException("Unsupported FT bank version",
                           "FTRawBankDecoder",
                           StatusCode::FAILURE);
      
    }
  //Body of the decoder
  if (version == 4){
    for ( const LHCb::RawBank* bank : banks) {//Iterates over the Tell40
      LHCb::FTChannelID source       = m_readoutTool->channelIDShift(bank->sourceID());
      //      if ( msgLevel(MSG::DEBUG) )
      //        {
      //          debug() << "source " << source          
      //                  << " = (" << source.station()
      //                  << " "    << source.layer()
      //                  << " "    << source.quarter()
      //                  << ")"
      //                  << " size " << bank->size() << endmsg;
      //        }
      auto first = bank->begin<short int>();
      auto last  = bank->end<short int>();
      for ( auto it = first ; it != last ; it++) {//loop over clusters      
        unsigned short int c      = *it;
        if (c==0) continue;//padding at the end
        //        unsigned modulesipm = c >> FTRawBank::sipmShift ;//todo
        unsigned modulesipm = c >> FTRawBank::cellShift ;//todo
        LHCb::FTChannelID chanModuleSiPM = m_readoutTool->channel(modulesipm);
        chanModuleSiPM.addToChannel(source);
        int fraction     = ( c >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum;
        bool cSize       = ( c >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;
        clus.emplace_back(chanModuleSiPM,
                          fraction, ( cSize ? 0 : 4 ));
      }//end loop over sipms
      partitionPoints.push_back(clus.size());
    }//end loop over rawbanks
  }//version == 4  
  // Assert that clusters are sorted
  assert( std::is_sorted(clus.begin(), clus.end(),
                         [](const LHCb::FTLiteCluster& lhs, const LHCb::FTLiteCluster& rhs){
                           return lhs.channelID() < rhs.channelID(); }) &&
          "Clusters from the RawBanks not sorted. Should be sorted by construction.") ;
  
  // sort clusters according to PrFTHits (loop over TELL40)
  // right now it is inefficient because it loops over 6 modules when it
  // should loop over 2 maximum.
  // iTell40 = (module==1 || module ==2) + ((module==3 || module==4)<<1)
  auto iClusFirst = clus.begin();
  for( auto pPoint : partitionPoints ) {
    auto partitionPoint = std::next(clus.begin(),pPoint);
    if( iClusFirst != partitionPoint ) { // container must not be empty
      auto chanID = (*iClusFirst).channelID(); // FTChannelID first cluster
      unsigned int iUQua = chanID.uniqueQuarter();
      
      // Swap clusters within modules
      // if quadrant==0 or 3 for even layers or quadrant==1 or 2 for odd layers
      if( (((iUQua >> 2)&1)==0) ^ ((iUQua & 3)>>1) ^ (iUQua & 1) ) {
        auto iClusFirstM = iClusFirst; // copy
        for( unsigned int iMod = 0; iMod < 6; ++iMod ) {
          auto thisModule = [&iMod](const LHCb::FTLiteCluster cluster)
            { return cluster.channelID().module() == iMod; };
          auto iClusLastM = std::partition_point(iClusFirstM,partitionPoint,thisModule);
          std::reverse(iClusFirstM, iClusLastM);  // swap clusters in module
          iClusFirstM = iClusLastM;
        }
      }
      
      // Swap clusters within full quadrant
      if( (iUQua & 1) == 0 ) { // quadrant==0 or 2
        std::reverse(iClusFirst, partitionPoint);  // swap clusters in quadrant
      }
    }
    iClusFirst = partitionPoint;
  }  
  return clus;
}

