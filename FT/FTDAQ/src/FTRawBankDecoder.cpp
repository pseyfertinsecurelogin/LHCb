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
  
  return StatusCode::SUCCESS;
}

//StatusCode FTRawBankDecoder::finalize()
//{
  //  StatusCode sc = readoutTool()->finalize();
  //  if (sc.isFailure()) return Error("Failed to finalize the readout tool", sc);
//  return Transformer::finalize();
//}



//=============================================================================
// Initializer of the ReadoutTool (taken from STCommonBase)
//=============================================================================

IFTReadoutTool* FTRawBankDecoder::getReadoutTool() const {
  m_readoutTool = this -> template tool<IFTReadoutTool>("FTReadoutTool","FTReadoutTool");
  return m_readoutTool;
}


IFTReadoutTool* FTRawBankDecoder::readoutTool() const {
  return m_readoutTool != 0 ? m_readoutTool : getReadoutTool();
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
  clus.reserve(banks.size());
  
  // Store partition points for Tell40 for faster sorting
  boost::container::static_vector<int,FTRawBank::NbBanks> partitionPoints;
  if ( msgLevel(MSG::DEBUG) ) debug() << "Number of raw banks " << banks.size() << endmsg;
  unsigned int iBank = 0;
  for ( const LHCb::RawBank* bank : banks) {//Iterates over the Tell40
    LHCb::FTChannelID source       = readoutTool()->uniqueQuarter(bank->sourceID());
    unsigned uniqueQuarter = source.uniqueQuarter();
    auto size        = bank->size(); // in bytes, multiple of 4

    if ( msgLevel(MSG::DEBUG) )
      {
        debug() << "source " << source          
                << " = (" << source.station()
                << " "    << source.layer()
                << " "    << source.quarter()
                << ")"
                << " size " << size << endmsg;
      }
    if ( bank->version() != 4) {
      error() << "** Unsupported FT bank version " << bank->version()
              << " for source " << source << " size " << size << " bytes."
              << endmsg;
      throw GaudiException("Unsupported FT bank version",
                           "FTRawBankDecoder",
                           StatusCode::FAILURE);
    }
    //Todo: We do not take advantage of the specific structure
    auto first = bank->begin<short int>();
    auto last  = bank->end<short int>();
    auto it    = first;
    for ( auto it = first ; it != last ; it++) {//loop over clusters      
      unsigned short int c      = *it;
      if (c==0) continue;//padding at the end
      if(bank->version() == 4){
        unsigned modulesipm = c >> FTRawBank::clusSipmShift ;//todo
        LHCb::FTChannelID chanModuleSiPM = m_readoutTool->sipm(modulesipm);
        unsigned module     = chanModuleSiPM.module() + readoutTool()->moduleShift(source+modulesipm);
        unsigned mat        = chanModuleSiPM.mat ();
        unsigned sipm    = ( c >> FTRawBank::clusSipmShift ) & FTRawBank::sipmMaximum;
        unsigned channel = ( c >> FTRawBank::cellShift     ) & FTRawBank::cellMaximum;
        int fraction     = ( c >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum;
        bool cSize       = ( c >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;
        unsigned relsipm = sipm - 4*mat - 16*module;
        if( module > 5 ){
          Warning("Skipping cluster(s) for non-existing module " +
                  std::to_string(module) ).ignore();
          continue;
        }
        warning() << "LoH: Africa: " << module << " " << mat << " " << relsipm << " " << channel << " " << fraction << " " << cSize << endmsg;
        
        //not the last cluster
        if( !cSize &&  it < (last-1) && (((*(it+1) >> FTRawBank::clusSipmShift ) & FTRawBank::sipmMaximum) == sipm)){
          warning() << "LoH: Not the last" << endmsg;
          //          warning() << "LoH: Not last cluster" << endmsg;
          short int c2      = *(it+1);          
          bool cSize2       = ( c2 >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;
          
          if( !cSize2 ){ //next cluster is not last fragment
            clus.emplace_back(LHCb::FTChannelID{ 0 ,0 ,uniqueQuarter,
                  module, mat, relsipm, channel },
              fraction, 4 );
            
            if ( msgLevel( MSG::VERBOSE ) ) {
              verbose() << format( "size<=4  channel %4d frac %3d size %3d code %4.4x",
                                   channel, fraction, cSize, c ) << endmsg;
            }
          }
          else {//fragmented cluster, last edge found
            unsigned channel2 = ( c2 >> FTRawBank::cellShift     ) & FTRawBank::cellMaximum;
            int fraction2     = ( c2 >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum;
            warning() << "LoH: Not the last: " << channel << " " << channel2 << endmsg;
            
            unsigned int diff = (channel2 - channel);
            //            warning() << "LoH: Last edge found (diff=" << diff << ")" << endmsg;
            
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
              clus.emplace_back(LHCb::FTChannelID{ 0,0,uniqueQuarter,
                    module, mat, relsipm, channel },
                fraction, 0 ); //pseudoSize=0
              
              if ( msgLevel( MSG::WARNING ) ) {
                warning() << format( "first edge cluster %4d frac %3d size %3d code %4.4x",
                                     channel, fraction, cSize, c ) << endmsg;                               
              }
              
              for(unsigned int  i = m_clusterMaxWidth; i < diff ; i+= m_clusterMaxWidth){
                // all middle clusters will have same size as the first cluster,
                // so use same fraction
                clus.emplace_back(LHCb::FTChannelID{ 0,0,uniqueQuarter, module,
                      mat, relsipm, channel+i },
                  fraction, 0 );
                
                if ( msgLevel( MSG::WARNING ) ) {
                  warning() << format( "middle cluster %4d frac %3d size %3d code %4.4x", 
                                       channel+i, fraction, cSize, c )<< " added " <<diff << endmsg;
                }
              }
                
              //add the last edge 
              clus.emplace_back(LHCb::FTChannelID{ 0,0,uniqueQuarter,
                    module, mat, relsipm, channel2 },
                fraction2, 0 );
              
              if ( msgLevel( MSG::WARNING ) ) {
                warning() << format(  "last edge cluster %4d frac %3d size %3d code %4.4x",
                                      channel2, fraction2, cSize2, c2 ) << endmsg;
              }
            }
            else{//big cluster size upto size 8
              warning() << "LoH: Big cluster"<< endmsg;
              unsigned int firstChan  =  channel - int( (m_clusterMaxWidth-1)/2 );
              unsigned int widthClus  =  2 * diff - 1 + fraction2  ;
              
              unsigned int clusChanPosition = firstChan + (widthClus-1)/2;
              int frac                      = (widthClus-1)%2;
              
              //add the new cluster = cluster1+cluster2
              clus.emplace_back(LHCb::FTChannelID{ 0,0,uniqueQuarter, module,
                    mat, relsipm, clusChanPosition},
                frac, widthClus );
              
              if ( msgLevel( MSG::VERBOSE ) ) {
                verbose() << format( "combined cluster %4d frac %3d size %3d code %4.4x", 
                                     channel, fraction, cSize, c ) << endmsg;
              }
            }//end if adjacent clusters
            ++it;
          }//last edge found          
        }//not the last cluster
        else{ //last cluster, so nothing we can do
          clus.emplace_back(LHCb::FTChannelID{ 0,0,uniqueQuarter,
                module, mat, relsipm, channel },
            fraction, 4 );
          
          if ( msgLevel( MSG::VERBOSE ) ) {
            verbose() << format( "size<=4  channel %4d frac %3d size %3d code %4.4x",
                                 channel, fraction, cSize, c ) << endmsg;            
          }
        }//last cluster added
      }//bank ;version == 4
      else{
        short int c      = *it;
        //      unsigned modulesipm = c >> FTRawBank::sipmShift ;//todo
        unsigned modulesipm = c >> FTRawBank::clusSipmShift ;//todo
        LHCb::FTChannelID chanModuleSiPM = m_readoutTool->sipm(modulesipm);
        warning() << "LoH: " << source << " " << modulesipm << endmsg;
        unsigned module     = chanModuleSiPM.module() + 16 * readoutTool()->moduleShift(source+modulesipm);//Problem: this is a relative module. We need to get the absolute one.
        //      unsigned module     = chanModuleSiPM.module();//Problem: this is a relative module. We need to get the absolute one.
        unsigned mat        = chanModuleSiPM.mat ();
        
        unsigned sipm    = ( c >> FTRawBank::clusSipmShift ) & FTRawBank::sipmMaximum;
        unsigned channel = ( c >> FTRawBank::cellShift     ) & FTRawBank::cellMaximum;
        int fraction     = ( c >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum;
        bool cSize       = ( c >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;
        
        clus.push_back({LHCb::FTChannelID{ 0, 0, uniqueQuarter, module, mat, sipm, channel },
              fraction, ( cSize ? 0 : 4 )});
      }
    }//end loop over sipms
    partitionPoints.push_back(clus.size());
  }//end loop over rawbanks
  warning() << "LoH: Finished the loop" << endmsg;
  // Assert that clusters are sorted
  assert( std::is_sorted(clus.begin(), clus.end(),
                         [](const LHCb::FTLiteCluster& lhs, const LHCb::FTLiteCluster& rhs){
                           return lhs.channelID() < rhs.channelID(); }) &&
          "Clusters from the RawBanks not sorted. Should be sorted by construction.") ;
  
  for (auto clu : clus)
    warning() << "LoH: Clusters: " << clu.channelID() << " " << clu.fraction()  << endmsg;
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
  warning() << "LoH: Finished decoding" << endmsg;
  return clus;
}

