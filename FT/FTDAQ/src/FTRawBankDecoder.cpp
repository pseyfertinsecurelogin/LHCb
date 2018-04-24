// Include files

// from Gaudi
#include "Event/RawEvent.h"

// local
#include "FTRawBankDecoder.h"

#include "range/v3/view/transform.hpp"
#include "range/v3/iterator_range.hpp"

//-----------------------------------------------------------------------------
// Implementation file for class : FTRawBankDecoder
//
// 2012-05-11 : Olivier Callot
//-----------------------------------------------------------------------------

namespace {  
unsigned channelInTell40(short int c) {
  return ( c >> FTRawBank::cellShift     );
}
  
unsigned getSipm(short int c){
  return (c >> (FTRawBank::sipmShift - FTRawBank::cellShift)); 
}

int cell(short int c) {
  return ( c >> FTRawBank::cellShift ) & FTRawBank::cellMaximum;
}

int fraction(short int c) {
    return ( c >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum;
}

bool cSize(short int c) {
    return ( c >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;
}

const auto is_in_module = [](unsigned int mod) {
    return [mod](const LHCb::FTLiteCluster cluster) { return cluster.channelID().module() == mod; };
};

template <typename Iter>
void reverse_each_module(Iter first, Iter last) {
        for( unsigned int iMod = 0; iMod < 6; ++iMod ) {
          auto finish = std::partition_point(first,last,is_in_module(iMod));
          std::reverse(first, finish);  // swap clusters in module
          first = finish;
        }
        //TODO: need only to call partition_point until iMod=4, as the remainder
        //      must be module 5...
        assert(first==last);
}


template <typename Container, typename Fun >
void for_each_quadrant( Container& c, Fun&& f)
{
  for( uint16_t iQuarter = 0; iQuarter < FTRawBank::nTotQuarters; ++iQuarter ) {
    auto range = c.range_(iQuarter);
    f(range.first,range.second, iQuarter);
  }
}
}

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
  
  FTLiteClusters clus;
  if ( msgLevel(MSG::DEBUG) )
    debug() << "Number of raw banks " << banks.size() << endmsg;
  if( banks.empty() ) return clus;

  //Testing the bank version
  unsigned int version = banks[0]->version();
  if ( msgLevel(MSG::DEBUG) ) debug() << "Bank version " << version << endmsg;
  if (version < 2 && version > 5) {
      error() << "** Unsupported FT bank version " << version << endmsg;
      throw GaudiException("Unsupported FT bank version",
                           "FTRawBankDecoder", StatusCode::FAILURE);
  }

  // Simple loop over the banks to determine total number of clusters
  int totSize = 0;
  for ( const LHCb::RawBank* bank : banks) { totSize += bank->size();}
  if( version==2 || version == 3) {
    // Bank size is half the number of clusters and includes headers
    clus.reserve(4 * totSize / 10);
  } else {
    clus.reserve(totSize);
  }
  
  if ( msgLevel(MSG::DEBUG) ) debug() << "Number of raw banks " << banks.size() << endmsg;
  if( version==2 || version ==3 ) {
  for ( const LHCb::RawBank* bank : banks) {
    int source       = bank->sourceID();
    unsigned station = source/16 + 1u;
    unsigned layer   = (source & 12) /4;
    unsigned quarter = source & 3;

    if ( msgLevel(MSG::VERBOSE) ) verbose() << "source " << source
                                            << " station " << station << " layer " << layer
                                            << " quarter " << quarter << " size " << bank->size()
                                            << endmsg;
    if ( bank->version() != 2 &&  bank->version() != 3) {
      error() << "** Unsupported FT bank version " << bank->version()
              << " for source " << source << " size " << bank->size() << " bytes."
              << endmsg;
      throw GaudiException("Unsupported FT bank version",
                           "FTRawBankDecoder",
                           StatusCode::FAILURE);
    }

    auto first = bank->begin<short int>();
    auto last  = bank->end<short int>();

    while ( first != last ) {
      int sipmHeader = *first++;
      if ( first == last && sipmHeader == 0 ) continue;  // padding at the end...
      unsigned modulesipm = sipmHeader >> 4 ;
      unsigned module     = modulesipm >> 4 ;
      unsigned mat        = ( modulesipm & 15 ) >> 2; // hardcoded: this should be replaced by mapping
      unsigned sipm       = modulesipm & 3;          // hardcoded: this should be replaced by mapping
      int nClus           = sipmHeader & 15 ;

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

        for( auto it = first ;  it< first+nClus;++it ){
          short int c      = *it;
          unsigned channel = c & 127;
          int fraction     = ( c >> 7 ) & 1;
          bool cSize       = ( c >> 8 ) & 1;

          //not the last cluster
          if( !cSize &&  it<first+nClus-1 ){
            short int c2      = *(it+1);
            bool cSize2       = ( c2 >> 8) & 1;

            if( !cSize2 ){ //next cluster is not last fragment
              clus.addHit(std::make_tuple(LHCb::FTChannelID{ station, layer, quarter,
                                                             module, mat, sipm, channel },
                                          fraction, 4), bank->sourceID() );

              if ( msgLevel( MSG::VERBOSE ) ) {
                verbose() << format( "size<=4  channel %4d frac %3d size %3d code %4.4x",
                                     channel, fraction, cSize, c ) << endmsg;
              }
            }
            else {//fragmented cluster, last edge found
              unsigned channel2 = c2 & 127;
              int fraction2     = (c2 >> 7 ) & 1;

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
                clus.addHit(std::make_tuple(LHCb::FTChannelID{ station, layer, quarter,
                                                               module, mat, sipm, channel },
                                            fraction, 0), bank->sourceID() ); //pseudoSize=0

                if ( msgLevel( MSG::VERBOSE ) ) {
                  verbose() << format( "first edge cluster %4d frac %3d size %3d code %4.4x",
                                       channel, fraction, cSize, c ) << endmsg;
                }

                for(unsigned int  i = m_clusterMaxWidth; i < diff ; i+= m_clusterMaxWidth){
                  // all middle clusters will have same size as the first cluster,
                  // so use same fraction
                  clus.addHit(std::make_tuple(LHCb::FTChannelID{ station, layer, quarter,
                                                                 module, mat, sipm, channel+i },
                                              fraction, 0), bank->sourceID() );

                  if ( msgLevel( MSG::VERBOSE ) ) {
                    verbose() << format( "middle cluster %4d frac %3d size %3d code %4.4x",
                                         channel+i, fraction, cSize, c )<< " added " <<diff << endmsg;
                  }
                }

                //add the last edge
                clus.addHit(std::make_tuple(LHCb::FTChannelID{ station, layer, quarter,
                                                               module, mat, sipm, channel2 },
                                            fraction2, 0), bank->sourceID() );

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
                clus.addHit(std::make_tuple(LHCb::FTChannelID{ station, layer, quarter,
                                                               module, mat, sipm, clusChanPosition },
                                            frac, widthClus), bank->sourceID() );

                if ( msgLevel( MSG::VERBOSE ) ) {
                  verbose() << format( "combined cluster %4d frac %3d size %3d code %4.4x",
                                       channel, fraction, cSize, c ) << endmsg;
                }
              }//end if adjacent clusters
              ++it;
            }//last edge foud
          }//not the last cluster
          else{ //last cluster, so nothing we can do
            clus.addHit(std::make_tuple(LHCb::FTChannelID{ station, layer, quarter,
                                                           module, mat, sipm, channel },
                                        fraction, 4), bank->sourceID() );

            if ( msgLevel( MSG::VERBOSE ) ) {
              verbose() << format( "size<=4  channel %4d frac %3d size %3d code %4.4x",
                                   channel, fraction, cSize, c ) << endmsg;
            }
          }//last cluster added
        }//end loop over clusters in one sipm
      } else { //bank version == 2
        //normal clustering without any modification to clusters, should work for encoder=2
        for( auto it = first ;  it< first+nClus;++it ){
          short int c      = *it;
          unsigned channel = ( c >> 0 ) & 127;
          int fraction     = ( c >> 7 ) & 1;
          int cSize        = ( c >> 8 ) & 1;
          clus.addHit(std::make_tuple(LHCb::FTChannelID{ station, layer, quarter,
                                                         module, mat, sipm, channel },
                                      fraction, ( cSize ? 0 : 4 )), bank->sourceID() );
        }
      }
      first += nClus;
    }//end loop over sipms
  }//end loop over rawbanks
  }

  //Body of the decoder
  if (version == 4){
    for ( const LHCb::RawBank* bank : banks) {//Iterates over the Tell40
      LHCb::FTChannelID source = m_readoutTool->channelIDShift(bank->sourceID());
      auto last  = bank->end<short int>();
      if( *(last-1)==0 ) last--; // remove zero padding at the end

      auto r = ranges::make_iterator_range(bank->begin<short int>(), last )
              | ranges::view::transform( [&source](unsigned short int c) -> LHCb::FTLiteCluster
                                        { return { source+channelInTell40(c),
                                                   fraction(c), ( cSize(c) ? 0 : 4 ) };} );
      clus.insert(r.begin(),r.end(), bank->sourceID()/3);
    }//end loop over rawbanks
  }//version == 4  
  /*else if (version == 5) {
    for ( const LHCb::RawBank* bank : banks) {//Iterates over the Tell40
      LHCb::FTChannelID source = m_readoutTool->channelIDShift(bank->sourceID());      
      //Define the work flow
      auto first = bank->begin<short int>();
      auto last  = bank->end<short int>();
      if( *(last-1)==0 ) last--; // remove zero padding at the end
      for( auto it = first ;  it != last; ++it ){
        unsigned short int c = *it;
        unsigned modulesipm = channelInTell40(c);
        int fraction1       = fraction(c);
        bool cSize1         = cSize(c);                
        
        unsigned short int c2         = *(it+1);
        unsigned modulesipm2 = channelInTell40(c2);
        LHCb::FTChannelID firstChannel = source+modulesipm;
        //Define the Lambda functions
        //    //Basic make cluster
        auto make_cluster = [&](unsigned chan, int fraction, int size) {
          clus.emplace_back( chan,
                             fraction, size );
        };//End lambda make_cluster
        
        //    //Make clusters between two channels
        auto make_clusters = [&](unsigned firstChannel, short int c, short int c2) {
          unsigned int delta = (cell(c2) - cell(c));
          
          if( UNLIKELY( delta > 128 ) ) {
            this->error()<<"something went terribly wrong here first fragment: " << cell(c)
                         <<" second fragment: "  << cell(c2) << endmsg;
            throw GaudiException("There is an inconsistency between Encoder and Decoder!",
                                 "FTRawBankDecoder",
                                 StatusCode::FAILURE);
          }
          // fragmented clusters, size > 2*max size
          // only edges were saved, add middles now
          if ( delta  > m_clusterMaxWidth ) {
            //add the first edge cluster, and then the middle clusters
            for(unsigned int  i = 0; i < delta ; i+= m_clusterMaxWidth){
              // all middle clusters will have same size as the first cluster,
              // so re-use the fraction
              make_cluster( firstChannel+i, fraction(c), 0 );
            }
            //add the last edge
            make_cluster  ( firstChannel+cell(c2)-cell(c), fraction(c2), 0 );
          } else { //big cluster size upto size 8

            unsigned int widthClus  =  2 * delta - 1 + fraction(c2);            
            //add the new cluster = cluster1+cluster2
            make_cluster( firstChannel + (widthClus-1)/2 - int( (m_clusterMaxWidth-1)/2 ), (widthClus-1)%2, widthClus );
          }//end if adjacent clusters
        };//End lambda make_clusters
        
        // Workflow
        //        //not the last cluster
        if( !cSize1 &&  it<last-1 && (getSipm(modulesipm) == getSipm(modulesipm2)))
          {
            bool cSize2       = ( c2 >> FTRawBank::sizeShift     ) & FTRawBank::sizeMaximum;
            
            if( !cSize2 ){ //next cluster is not last fragment
              make_cluster(firstChannel,fraction1, 4 );
            }
            else {//fragmented cluster, last edge found
              make_clusters(firstChannel,c,c2);
              ++it;
            }//last edge found
          }//not the last cluster
        
        else{ //last cluster, so nothing we can do
          make_cluster(source+modulesipm, fraction1, 4 );
        }//last cluster added
      }//end loop over clusters in one sipm
    }//end loop over rawbanks        
  }//version ==5
  */
  if ( msgLevel(MSG::VERBOSE) ) {
      for ( const auto& c : clus.range() )
        verbose() << format( " channel %4d frac %3d size %3d ",
            c.channelID(), c.fraction(), c.pseudoSize() ) << endmsg;
  }

  // Assert that clusters are sorted
  assert( std::is_sorted(clus.range().begin(), clus.range().end(),
         [](const LHCb::FTLiteCluster& lhs, const LHCb::FTLiteCluster& rhs){
         return lhs.channelID() < rhs.channelID(); }) &&
      "Clusters from the RawBanks not sorted. Should be sorted by construction.") ;

  for_each_quadrant( clus,
                     [](auto first, auto last, auto iUQua) {
                         if (first==last) return;
                         // Swap clusters within modules
                         // if quadrant==0 or 3 for even layers or quadrant==1 or 2 for odd layers
                         if( (((iUQua >> 2)&1)==0) ^ ((iUQua & 3)>>1) ^ (iUQua & 1) ) {
                           reverse_each_module(first,last);
                         }
                         // Swap clusters within full quadrant
                         if( (iUQua & 1) == 0 ) { // quadrant==0 or 2
                           std::reverse(first, last);  // swap clusters in quadrant
                         }
  });

  return clus;
}

