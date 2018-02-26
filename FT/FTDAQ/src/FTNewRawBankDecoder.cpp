// Include files

// from Gaudi
#include "Event/RawEvent.h"

// local
#include "FTNewRawBankDecoder.h"
#include "FTRawBankParams.h"

//-----------------------------------------------------------------------------
// Implementation file for class : FTNewRawBankDecoder
//
// 2012-05-11 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FTNewRawBankDecoder )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
FTNewRawBankDecoder::FTNewRawBankDecoder( const std::string& name,
                                    ISvcLocator* pSvcLocator)
: Transformer ( name , pSvcLocator,
                KeyValue{ "RawEventLocations",
                    Gaudi::Functional::concat_alternatives( LHCb::RawEventLocation::Other,
                                                            LHCb::RawEventLocation::Default )},
                KeyValue{ "OutputLocation", LHCb::FTLiteClusterLocation::Default } )
{ }

StatusCode FTNewRawBankDecoder::initialize()
{
  StatusCode sc = Transformer::initialize();
  m_ftReadoutTool = tool<IFTReadoutTool>("FTReadoutTool",this);
  m_ftReadoutTool->initialize();
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
FTLiteClusters
FTNewRawBankDecoder::operator()(const LHCb::RawEvent& rawEvent) const
{
  const auto& banks = rawEvent.banks(LHCb::RawBank::FTCluster);

  // Simple loop over the banks to determine total number of clusters
  // Bank size is half the number of clusters and includes headers
  // No 0.4 scaling
  // In the future, the #clusters should be encoded in raw bank.
  FTLiteClusters clus;
  int totSize = 0;
  for ( const LHCb::RawBank* bank : banks) { totSize += bank->size();}
  clus.reserve(totSize);

  // Store partition points for quadrants for faster sorting
  std::vector<int> partitionPoints;
  partitionPoints.reserve(48); // 48 quadrants

  if ( msgLevel(MSG::DEBUG) ) debug() << "Number of raw banks " << banks.size() << endmsg;
  for ( const LHCb::RawBank* bank : banks) {
    // This source, at the contrary of the former one, contains also enough information
    // to know which SiPM.
    LHCb::FTChannelID source = bank->sourceID();
    unsigned uniqueQuarter   = source.uniqueQuarter();
    unsigned module          = source.module();
    unsigned sipmInModule    = source.sipmInModule();
    
    auto size        = bank->size(); // in bytes, multiple of 4

    if ( msgLevel(MSG::VERBOSE) )
      {
        verbose() << "source " << source          
                  << " uniqueQuarter " << uniqueQuarter 
                  << " = (" << source.station()
                  << " "    << source.layer()
                  << " "    << source.quarter()
                  << ")"
                  << " module " << module
                  << " siPM " << sipmInModule
                  << " size " << size << endmsg;
      }
    if ( bank->version() != 2 &&  bank->version() != 3) {
      error() << "** Unsupported FT bank version " << bank->version()
              << " for source " << source << " size " << size << " bytes."
              << endmsg;
      throw GaudiException("Unsupported FT bank version",
                           "FTNewRawBankDecoder",
                           StatusCode::FAILURE);
    }

    auto first = bank->begin<short int>();
    auto last  = bank->end<short int>();

    while( first != last ) {
      int sipmHeader = *first++;
      if ( first == last && sipmHeader == 0 ) continue;  // padding at the end...
      unsigned modulesipm = sipmHeader >> FTRawBank::sipmShift ;
      unsigned module     = modulesipm >> FTRawBank::sipmShift ;
      LHCb::FTChannelID chanModuleSiPM = m_ftReadoutTool->sipm(modulesipm);
      unsigned mat        = chanModuleSiPM.mat ();
      unsigned sipm       = chanModuleSiPM.sipm();
      int nClus           = sipmHeader & FTRawBank::nbClusMaximum ;

      if (UNLIKELY( msgLevel(MSG::VERBOSE) && nClus > 0) )
        verbose() << " Module "    << module << " mat " << mat << " SiPM " << sipm
                  << " nClusters " << nClus  << endmsg;

      if (UNLIKELY(nClus>std::distance(first,last))) {
        warning() << "Inconsistent size of rawbank. #clusters in header="<< nClus 
                  << ", #clusters in bank=" << std::distance(first,last) << endmsg;

        throw GaudiException("Inconsistent size of rawbank",
                             "FTNewRawBankDecoder",
                             StatusCode::FAILURE);
      }
      
      if( module > 5 ){
        Warning("Skipping cluster(s) for non-existing module " +
                std::to_string(module) ).ignore();
        first += nClus;
        continue;
      }
      
      if(bank->version() == 3){
       
        for( auto it = first ;  it< first+nClus;++it ){
          short int c      = *it;
          unsigned channel = ( c >> FTRawBank::cellShift     ) & FTRawBank::cellMaximum;
          int fraction     = ( c >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum;
          bool cSize       = ( c >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;
          
          //not the last cluster
          if( !cSize &&  it<first+nClus-1 ){
            short int c2      = *(it+1);
            bool cSize2       = ( c2 >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;
            
            if( !cSize2 ){ //next cluster is not last fragment
              clus.emplace_back(LHCb::FTChannelID{ 0,0,uniqueQuarter,
                                                   module, mat, sipm, channel },
                                fraction, 4 );
              
              if ( msgLevel( MSG::VERBOSE ) ) {
                verbose() << format( "size<=4  channel %4d frac %3d size %3d code %4.4x",
                                     channel, fraction, cSize, c ) << endmsg;
              }
            }
            else {//fragmented cluster, last edge found
              unsigned channel2 = ( c2 >> FTRawBank::cellShift     ) & FTRawBank::cellMaximum;
              int fraction2     = ( c2 >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum;
              
              unsigned int diff = (channel2 - channel);
              
              if( UNLIKELY( diff > 128 ) ) {
                error()<<"something went terribly wrong here first fragment: " << channel
                       <<" second fragment: "  << channel2 << endmsg;
                throw GaudiException("There is an inconsistency between Encoder and Decoder!",
                                     "FTNewRawBankDecoder",
                                     StatusCode::FAILURE);
              }
              // fragemted clusters, size > 2*max size
              // only edges were saved, add middles now 
              if(diff  > m_clusterMaxWidth){
                
                //add the first edge cluster
                clus.emplace_back(LHCb::FTChannelID{ 0,0,uniqueQuarter,
                                                     module, mat, sipm, channel },
                                  fraction, 0 ); //pseudoSize=0
                
                if ( msgLevel( MSG::VERBOSE ) ) {
                  verbose() << format( "first edge cluster %4d frac %3d size %3d code %4.4x",
                                       channel, fraction, cSize, c ) << endmsg;
                  

                }
                
                for(unsigned int  i = m_clusterMaxWidth; i < diff ; i+= m_clusterMaxWidth){
                  // all middle clusters will have same size as the first cluster,
                  // so use same fraction
                  clus.emplace_back(LHCb::FTChannelID{ 0,0,uniqueQuarter, module,
                                                       mat, sipm, channel+i },
                                    fraction, 0 );
                  
                  if ( msgLevel( MSG::VERBOSE ) ) {
                    verbose() << format( "middle cluster %4d frac %3d size %3d code %4.4x", 
                                         channel+i, fraction, cSize, c )<< " added " <<diff << endmsg;
                  }
                }
                
                //add the last edge 
                clus.emplace_back(LHCb::FTChannelID{ 0,0,uniqueQuarter,
                                                     module, mat, sipm, channel2 },
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
                clus.emplace_back(LHCb::FTChannelID{ 0,0,uniqueQuarter, module,
                                                     mat, sipm, clusChanPosition},
                                  frac, widthClus );
                
                if ( msgLevel( MSG::VERBOSE ) ) {
                  verbose() << format( "combined cluster %4d frac %3d size %3d code %4.4x", 
                                       channel, fraction, cSize, c ) << endmsg;
                }
              }//end if adjacent clusters
              ++it;
            }//last edge foud
          }//not the last cluster
          else{ //last cluster, so nothing we can do
            clus.emplace_back(LHCb::FTChannelID{ 0,0,uniqueQuarter,
                                                 module, mat, sipm, channel },
                              fraction, 4 );
            
            if ( msgLevel( MSG::VERBOSE ) ) {
              verbose() << format( "size<=4  channel %4d frac %3d size %3d code %4.4x",
                                   channel, fraction, cSize, c ) << endmsg;
            }
          }//last cluster added
        }//end loop over clusters in one sipm
      }//bank version == 3
      else{
        //normal clustering without any modification to clusters, should work for encoder=2
        std::transform( first, first+nClus,
                        std::back_inserter(clus),
                        [&](short int c) -> LHCb::FTLiteCluster {
                          unsigned channel = ( c >> FTRawBank::cellShift     ) & FTRawBank::cellMaximum;
                          int fraction     = ( c >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum;
                          int cSize        = ( c >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;
                          
                          if ( msgLevel( MSG::VERBOSE ) ) {
                            verbose() << format( " channel %4d frac %3d size %3d code %4.4x",
                                                 channel, fraction, ( cSize ? 0 : 4 ), c ) << endmsg;
                          }
                          
                          return  { LHCb::FTChannelID{ 0, 0, uniqueQuarter, module, mat, sipm, channel },
                              fraction, ( cSize ? 0 : 4 ) };
                        } );
      }
      first += nClus;
    }//end loop over sipms
    partitionPoints.push_back(clus.size());
  }//end loop over rawbanks
  
  // Assert that clusters are sorted
  assert( std::is_sorted(clus.begin(), clus.end(),
         [](const LHCb::FTLiteCluster& lhs, const LHCb::FTLiteCluster& rhs){
         return lhs.channelID() < rhs.channelID(); }) &&
      "Clusters from the RawBanks not sorted. Should be sorted by construction.") ;

  // sort clusters according to PrFTHits (loop over quadrants)
  auto iClusFirst = clus.begin();
  for( auto pPoint : partitionPoints ) {
    auto partitionPoint = std::next(clus.begin(),pPoint);
    if( iClusFirst != partitionPoint ) { // container must not be empty
      auto chanID = (*iClusFirst).channelID(); // FTChannelID first cluster
      unsigned int iQua = chanID.quarter();

      // Swap clusters within modules
      // if quadrant==0 or 3 for even layers or quadrant==1 or 2 for odd layers
      if( ((chanID.layer()&1)==0) ^ (iQua>>1) ^ (iQua&1) ) {
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
      if( (iQua & 1) == 0 ) { // quadrant==0 or 2
        std::reverse(iClusFirst, partitionPoint);  // swap clusters in quadrant
      }
    }
    iClusFirst = partitionPoint;
  }

  return clus;
}

