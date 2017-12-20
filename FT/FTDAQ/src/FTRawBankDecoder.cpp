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

//=============================================================================
// Main execution
//=============================================================================
FTLiteClusters
FTRawBankDecoder::operator()(const LHCb::RawEvent& rawEvent) const
{
  const auto& banks = rawEvent.banks(LHCb::RawBank::FTCluster);
  FTLiteClusters clus;

  if ( msgLevel(MSG::DEBUG) ) debug() << "Number of raw banks " << banks.size() << endmsg;

  for ( const LHCb::RawBank* bank : banks) {
    int source       = bank->sourceID();
    unsigned station = source/16 + 1u; // JvT: this should be done by a mapping!
    unsigned layer   = (source & 12) /4;  // JvT: this should be done by a mapping!
    unsigned quarter = source & 3; // JvT: this should be done by a mapping!
    auto size        = bank->size(); // in bytes, multiple of 4

    if ( msgLevel(MSG::VERBOSE) ) verbose() << "source " << source
                                            << " station " << station << " layer " << layer
                                            << " quarter " << quarter << " size " << size
                                            << endmsg;
    if ( bank->version() != 2 &&  bank->version() != 3) {
      error() << "** Unsupported FT bank version " << bank->version()
              << " for source " << source << " size " << size << " bytes."
              << endmsg;
      throw GaudiException("Unsupported FT bank version",
                           "FTRawBankDecoder",
                           StatusCode::FAILURE);
    }

    auto first = bank->begin<short int>();
    auto last  = bank->end<short int>();
    while( first != last ) {
      int sipmHeader = *first++;
      if ( first == last && sipmHeader == 0 ) continue;  // padding at the end...
      unsigned modulesipm = sipmHeader >> FTRawBank::sipmShift ;
      unsigned module     = modulesipm >> FTRawBank::sipmShift ;
      unsigned mat        = (modulesipm & 15 ) >> 2; // hardcoded: this should be replaced by mapping
      unsigned sipm       = modulesipm & 3;          // hardcoded: this should be replaced by mapping
      int nClus           = sipmHeader & FTRawBank::nbClusMaximum ;

      if (UNLIKELY( msgLevel(MSG::VERBOSE) && nClus > 0) )
        verbose() << " Module "    << module << " mat " << mat << " SiPM " << sipm
                  << " nClusters " << nClus  << endmsg;

      if (UNLIKELY(nClus>std::distance(first,last))) {
        warning() << "Inconsistent size of rawbank. #clusters in header="<< nClus 
                  << ", #clusters in bank=" << std::distance(first,last) << endmsg;

        throw GaudiException("Inconsistent size of rawbank",
                             "FTRawBankDecoder",
                             StatusCode::FAILURE);
      }
      
      if( module > 5 ){
        Warning("Skipping cluster(s) for non-existing module " +
                std::to_string(module) ).ignore();
        first += nClus;
        continue;
      }
      
      if(bank->version() == 3){
        auto it = first;
        for( ;  it< first+nClus;++it ){
          short int c      = *it;
          unsigned channel = ( c >> FTRawBank::cellShift     ) & FTRawBank::cellMaximum;
          int fraction     = ( c >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum;
          bool cSize       = ( c >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;
          
          //not the last cluster
          if( !cSize &&  it<first+nClus-1 ){
            short int c2      = *(it+1);
            bool cSize2       = ( c2 >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;
            
            if( !cSize2 ){ //next cluster is not last fragment
              clus.emplace_back(LHCb::FTChannelID{ station, layer, quarter, module, mat, sipm, channel },
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
                                     "FTRawBankDecoder",
                                     StatusCode::FAILURE);
              }
              // fragemted clusters, size > 2*max size
              // only edges were saved, add middles now 
              if(diff  > m_clusterMaxWidth){
                
                //add the first edge cluster
                clus.emplace_back(LHCb::FTChannelID{ station, layer, quarter, module, mat, sipm, channel },
                                  fraction, 0 ); //pseudoSize=0
                
                if ( msgLevel( MSG::VERBOSE ) ) {
                  verbose() << format( "first edge cluster %4d frac %3d size %3d code %4.4x",
                                       channel, fraction, cSize, c ) << endmsg;
                  

                }
                
                for(unsigned int  i = m_clusterMaxWidth; i < diff ; i+= m_clusterMaxWidth){
                  // all middle clusters will have same size as the first cluster, so use same fraction 
                  clus.emplace_back(LHCb::FTChannelID{ station, layer, quarter, module, mat, sipm, channel+i },
                                    fraction, 0 );
                  
                  if ( msgLevel( MSG::VERBOSE ) ) {
                    verbose() << format( "middle cluster %4d frac %3d size %3d code %4.4x", 
                                         channel+i, fraction, cSize, c )<< " added " <<diff << endmsg;
                  }
                }
                
                //add the last edge 
                clus.emplace_back(LHCb::FTChannelID{ station, layer, quarter, module, mat, sipm, channel2 },
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
                clus.emplace_back(LHCb::FTChannelID{ station, layer, quarter, module, mat, sipm, clusChanPosition},
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
            clus.emplace_back(LHCb::FTChannelID{ station, layer, quarter, module, mat, sipm, channel },
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
                                                 channel, fraction, cSize, c ) << endmsg;
                          }
                          
                          return  { LHCb::FTChannelID{ station, layer, quarter, module, mat, sipm, channel },
                              fraction, cSize };
                      } );
      }
      first += nClus;
    }//end loop over sipms
  }//end loop over rawbanks
  
  // Sort clusters
  std::sort(clus.begin(), clus.end(),
            [](const LHCb::FTLiteCluster& lhs, const LHCb::FTLiteCluster& rhs){ return lhs.channelID() < rhs.channelID(); });
  
  return clus;
}

