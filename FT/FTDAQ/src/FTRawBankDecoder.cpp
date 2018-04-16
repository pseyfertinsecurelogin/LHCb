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
  if (version != 4 && version != 5)
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
      auto first = bank->begin<short int>();
      auto last  = bank->end<short int>();
      for ( auto it = first ; it != last ; it++) {//loop over clusters      
        unsigned short int c      = *it;
        if (c==0) continue;//padding at the end
        unsigned modulesipm = c >> FTRawBank::cellShift ;
        int fraction     = ( c >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum;
        bool cSize       = ( c >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;
        clus.emplace_back(LHCb::FTChannelID(0,0,0,0,0,0,modulesipm+source),
                          fraction, ( cSize ? 0 : 4 ));
      }//end loop over sipms
      partitionPoints.push_back(clus.size());
    }//end loop over rawbanks
  }//version == 4  
  else{
    for ( const LHCb::RawBank* bank : banks) {//Iterates over the Tell40
      LHCb::FTChannelID source       = m_readoutTool->channelIDShift(bank->sourceID());
      auto first = bank->begin<short int>();
      auto last  = bank->end<short int>();
      for( auto it = first ;  it != last; it++ ){
        short int c         = *it;
        if (c==0) continue;//padding at the end
        unsigned modulesipm = c >> FTRawBank::cellShift;
        unsigned sipm       = ( c >> FTRawBank::sipmShift     ) & FTRawBank::sipmMaximum;
        unsigned channel    = ( c >> FTRawBank::cellShift     ) & FTRawBank::cellMaximum;
        int fraction        = ( c >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum;
        bool cSize          = ( c >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;

        short int c2         = *(it+1);
        unsigned modulesipm2 = c2 >> FTRawBank::cellShift;
        //not the last cluster        
        //        if( !cSize &&  it<last-1 && (((c2 >> FTRawBank::sipmShift) & FTRawBank::sipmMaximum) == sipm) && () ){
        if( !cSize &&  it<last-1 && ((modulesipm >> (FTRawBank::sipmShift - FTRawBank::cellShift)) == (modulesipm2 >> (FTRawBank::sipmShift - FTRawBank::cellShift))) ){
          bool cSize2       = ( c2 >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;
          
          if( !cSize2 ){ //next cluster is not last fragment
            clus.emplace_back(LHCb::FTChannelID(0,0,0,0,0,0,source+modulesipm),
                              fraction, 4 );
          }
          else {//fragmented cluster, last edge found
            unsigned channel2 = ( c2 >> FTRawBank::cellShift     ) & FTRawBank::cellMaximum;
            int fraction2     = ( c2 >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum;
            
            unsigned int diff = (channel2 - channel);
            
            if( UNLIKELY( diff > 128 ) ) {
              error()<<"something went terribly wrong here first fragment: " << channel
                     <<" second fragment: "  << channel2 << endmsg;
              throw GaudiException("There is an inconsistency between Encoder and Decoder!",
                                   "FTRawBankDecoder",
                                   StatusCode::FAILURE);
            }
            // fragmented clusters, size > 2*max size
            // only edges were saved, add middles now 
            if(diff  > m_clusterMaxWidth){
              //add the first edge cluster
              clus.emplace_back(LHCb::FTChannelID(0,0,0,0,0,0,source+modulesipm),
                                fraction, 0 );//pseudoSize=0
              
              for(unsigned int  i = m_clusterMaxWidth; i < diff ; i+= m_clusterMaxWidth){
                // all middle clusters will have same size as the first cluster,
                // so use same fraction
                clus.emplace_back(LHCb::FTChannelID(0,0,0,0,0,0,source+modulesipm+i),
                                  fraction, 0 );
              }              
              //add the last edge 
              clus.emplace_back(LHCb::FTChannelID(0,0,0,0,0,0,source+modulesipm+diff),
                                fraction2, 0 );

              if ( msgLevel( MSG::VERBOSE ) ) {
                verbose() << format(  "last edge cluster %4d frac %3d size %3d code %4.4x",
                                      channel2, fraction2, cSize2, c2 ) << endmsg;
              }
            }
            else{//big cluster size upto size 8
              unsigned int firstChan  =  channel - int( (m_clusterMaxWidth-1)/2 );
              unsigned int widthClus  =  2 * diff - 1 + fraction2  ;
              
              unsigned int clusChanPosition = firstChan + (widthClus-1)/2;
              int frac                      = (widthClus-1)%2;
              
              //add the new cluster = cluster1+cluster2
              clus.emplace_back(LHCb::FTChannelID(0,0,0,0,0,0,source+modulesipm-channel+clusChanPosition),
                                frac, widthClus );
              
            }//end if adjacent clusters
            ++it;
          }//last edge found
        }//not the last cluster
        else{ //last cluster, so nothing we can do
          clus.emplace_back(LHCb::FTChannelID(0,0,0,0,0,0,source+modulesipm),
                            fraction, 4 );
        }//last cluster added
      }//end loop over clusters in one sipm
    }//end loop over rawbanks        
  }//version !=5
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

