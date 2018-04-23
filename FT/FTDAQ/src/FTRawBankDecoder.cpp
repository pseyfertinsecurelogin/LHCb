// Include files

// from Gaudi
#include "Event/RawEvent.h"
#include "boost/container/static_vector.hpp"

// local
#include "FTRawBankDecoder.h"
#include "FTRawBankParams.h"

#include "boost/container/static_vector.hpp"

//-----------------------------------------------------------------------------
// Implementation file for class : FTRawBankDecoder
//
// 2012-05-11 : Olivier Callot
//-----------------------------------------------------------------------------

namespace {  
unsigned channelInTell40(short int c) {
  return ( c >> FTRawBank::cellShift);
}
  
unsigned getSipm(short int c){
  //  return (c >> (FTRawBank::sipmShift - FTRawBank::cellShift)); 
  return ((c >> FTRawBank::sipmShift) & FTRawBank::sipmMaximum); 
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


template <typename Container, typename Partitions, typename Fun >
void for_each_quadrant( Container& c, const Partitions& parts, Fun&& f)
{
    using std::begin; using std::end;
    auto first = begin(c);
    for (auto p : parts) {
        auto last = std::next(begin(c),p);
        f(first,last);
        first = last;
    }
    assert( first == end(c) );
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
  
  // Store partition points for Tell40 for faster sorting
  boost::container::static_vector<int,FTRawBank::NbBanks> partitionPoints;
  
  if ( msgLevel(MSG::DEBUG) ) debug() << "Number of raw banks " << banks.size() << endmsg;
  if( version==2 || version ==3 ) {
  for ( const LHCb::RawBank* bank : banks) {
    int source       = bank->sourceID();
    unsigned station = source/16 + 1u; // JvT: this should be done by a mapping!
    unsigned layer   = (source & 12) /4;  // JvT: this should be done by a mapping!
    unsigned quarter = source & 3; // JvT: this should be done by a mapping!

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
              clus.emplace_back(LHCb::FTChannelID{ station, layer, quarter,
                                                   module, mat, sipm, channel },
                                fraction, 4 );

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
                clus.emplace_back(LHCb::FTChannelID{ station, layer, quarter,
                                                     module, mat, sipm, channel },
                                  fraction, 0 ); //pseudoSize=0

                if ( msgLevel( MSG::VERBOSE ) ) {
                  verbose() << format( "first edge cluster %4d frac %3d size %3d code %4.4x",
                                       channel, fraction, cSize, c ) << endmsg;


                }

                for(unsigned int  i = m_clusterMaxWidth; i < diff ; i+= m_clusterMaxWidth){
                  // all middle clusters will have same size as the first cluster,
                  // so use same fraction
                  clus.emplace_back(LHCb::FTChannelID{ station, layer, quarter, module,
                                                       mat, sipm, channel+i },
                                    fraction, 0 );

                  if ( msgLevel( MSG::VERBOSE ) ) {
                    verbose() << format( "middle cluster %4d frac %3d size %3d code %4.4x",
                                         channel+i, fraction, cSize, c )<< " added " <<diff << endmsg;
                  }
                }

                //add the last edge
                clus.emplace_back(LHCb::FTChannelID{ station, layer, quarter,
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
                clus.emplace_back(LHCb::FTChannelID{ station, layer, quarter, module,
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
            clus.emplace_back(LHCb::FTChannelID{ station, layer, quarter,
                                                 module, mat, sipm, channel },
                              fraction, 4 );

            if ( msgLevel( MSG::VERBOSE ) ) {
              verbose() << format( "size<=4  channel %4d frac %3d size %3d code %4.4x",
                                   channel, fraction, cSize, c ) << endmsg;
            }
          }//last cluster added
        }//end loop over clusters in one sipm
      } else { //bank version == 2
        //normal clustering without any modification to clusters, should work for encoder=2
        std::transform( first, first+nClus,
                        std::back_inserter(clus),
                        [&](short int c) -> LHCb::FTLiteCluster {
                        unsigned channel = ( c >> 0 ) & 127;
                        int fraction     = ( c >> 7 ) & 1;
                        int cSize        = ( c >> 8 ) & 1;
                        return  { LHCb::FTChannelID{ station, layer, quarter, module, mat, sipm, channel },
                                  fraction, ( cSize ? 0 : 4 ) };
                      } );
      }
      first += nClus;
    }//end loop over sipms
    partitionPoints.push_back(clus.size());
  }//end loop over rawbanks
  }

  //Body of the decoder
  if (version == 4){
    for ( const LHCb::RawBank* bank : banks) {//Iterates over the Tell40
      LHCb::FTChannelID source = m_readoutTool->channelIDShift(bank->sourceID());
      auto first = bank->begin<short int>();
      auto last  = bank->end<short int>();
      if (*(last-1) == 0) last--;//Remove padding at the end
      for ( auto it = first ; it != last ; ++it) { //loop over clusters
        unsigned short int c = *it;
        clus.emplace_back(source+channelInTell40(c), fraction(c), ( cSize(c) ? 0 : 4 ));
      }//end loop over sipms
      partitionPoints.push_back(clus.size());
    }//end loop over rawbanks
  }//version == 4  
  else if (version == 5) {    
    for ( const LHCb::RawBank* bank : banks) {//Iterates over the Tell40
      LHCb::FTChannelID source = m_readoutTool->channelIDShift(bank->sourceID());      
      //Define the work flow
      auto first = bank->begin<short int>();
      auto last  = bank->end<short int>();
      if (*(last-1) == 0) last--;//Remove padding at the end
      for( auto it = first ;  it != last; ++it ){
        unsigned short int c = *it;
        unsigned modulesipm = channelInTell40(c);
        LHCb::FTChannelID channel = source+modulesipm;
        //Define the Lambda functions        
        //    //Basic make cluster
        auto make_cluster = [&](unsigned chan, int fraction, int size) {
          clus.emplace_back( chan,
                             fraction, size );
        };//End lambda make_cluster
        //        //Allows to remove last cluster
        auto removeLastCluster = [&](){
          clus.pop_back();
        };
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
              if (i==0)continue;
              // all middle clusters will have same size as the first cluster,
              // so re-use the fraction
              make_cluster( firstChannel+i, fraction(c), 0 );
            }
            //add the last edge
            make_cluster  ( firstChannel+cell(c2)-cell(c), fraction(c2), 0 );
          } else { //big cluster size upto size 8
            unsigned int widthClus  =  2 * delta - 1 + fraction(c2);            
            //add the new cluster = cluster1+cluster2, and remove the one that was formed before.
            removeLastCluster();
            make_cluster( firstChannel + (widthClus-1)/2 - int( (m_clusterMaxWidth-1)/2 ), (widthClus-1)%2, widthClus );            
          }//end if adjacent clusters
        };//End lambda make_clusters
        
        // Workflow
        if (!cSize(c) || it == first)//No previous cluster or no size flag.
          make_cluster(channel,fraction(c),4);
        else{//Flagged and not the first one.
          unsigned c2 = *(it-1);
          if (cSize(c2) || (getSipm(c) != getSipm(c2)))//Not the same SiPM or flag size
            make_cluster(channel,fraction(c),4);
          else
            make_clusters(source+channelInTell40(c2),c2,c);
        }
      }
    }//end loop over rawbanks
  }//version ==5

  assert( partitionPoints.back() == static_cast<int>(clus.size()) );
  if ( msgLevel(MSG::VERBOSE) ) {
      for ( const auto& c : clus )
        verbose() << format( " channel %4d frac %3d size %3d ",
            c.channelID(), c.fraction(), c.pseudoSize() ) << endmsg;
  }

  // Assert that clusters are sorted
  assert( std::is_sorted(clus.begin(), clus.end(),
         [](const LHCb::FTLiteCluster& lhs, const LHCb::FTLiteCluster& rhs){
         return lhs.channelID() < rhs.channelID(); }) &&
      "Clusters from the RawBanks not sorted. Should be sorted by construction.") ;

  // sort clusters according to PrFTHits (loop over quadrants)
  for_each_quadrant( clus, partitionPoints,
                     [](auto first, auto last) {
                         if (first==last) return;
                         auto chanID = first->channelID(); // FTChannelID first cluster
                         unsigned int iUQua = chanID.uniqueQuarter();
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

