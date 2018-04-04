// Include files

// from Gaudi
#include "Event/RawEvent.h"
#include "boost/container/static_vector.hpp"

// local
#include "FTRawBankDecoder.h"
#include "FTRawBankParams.h"

//-----------------------------------------------------------------------------
// Implementation file for class : FTRawBankDecoder
//
// 2012-05-11 : Olivier Callot
//-----------------------------------------------------------------------------

namespace {

unsigned channel(short int c) {
    return ( c >> FTRawBank::cellShift     ) & FTRawBank::cellMaximum;
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

//=============================================================================
// Main execution
//=============================================================================
FTLiteClusters
FTRawBankDecoder::operator()(const LHCb::RawEvent& rawEvent) const
{
  const auto& banks = rawEvent.banks(LHCb::RawBank::FTCluster);

  // Simple loop over the banks to determine total number of clusters
  // Bank size is half the number of clusters and includes headers
  // The scaling by 0.4 is obtained empirically.
  // In the future, the #clusters should be encoded in raw bank.
  FTLiteClusters clus;
  int totSize = 0;
  for ( const LHCb::RawBank* bank : banks) { totSize += bank->size();}
  clus.reserve(4 * totSize / 10);

  // Store partition points for quadrants for faster sorting
  boost::container::static_vector<int,48> partitionPoints; // 48 quadrants

  if ( msgLevel(MSG::DEBUG) ) debug() << "Number of raw banks " << banks.size() << endmsg;
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
      unsigned modulesipm = sipmHeader >> FTRawBank::sipmShift ;
      unsigned module     = modulesipm >> FTRawBank::sipmShift ;
      unsigned mat        = ( modulesipm & 15 ) >> 2; // hardcoded: this should be replaced by mapping
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

      if(bank->version() == 3) {
        // define some workers to make clusters...
        auto make_cluster = [&](unsigned chan, int fraction, int size) {
          clus.emplace_back( LHCb::FTChannelID{ station, layer,quarter, module, mat, sipm, chan},
                             fraction, size );
        };
        auto make_clusters = [&](short int c, short int c2) {
          unsigned int delta = (channel(c2) - channel(c));

          if( UNLIKELY( delta > 128 ) ) {
            this->error()<<"something went terribly wrong here first fragment: " << channel(c)
                   <<" second fragment: "  << channel(c2) << endmsg;
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
              make_cluster( channel(c)+i, fraction(c), 0 );
            }
            //add the last edge
            make_cluster( channel(c2), fraction(c2), 0 );
          } else { //big cluster size upto size 8
            unsigned int firstChan  =  channel(c) - int( (m_clusterMaxWidth-1)/2 );
            unsigned int widthClus  =  2 * delta - 1 + fraction(c2)  ;

            //add the new cluster = cluster1+cluster2
            make_cluster( firstChan + (widthClus-1)/2, (widthClus-1)%2, widthClus );
          }//end if adjacent clusters
        };

        // define the control flow
        for( auto it = first; it != first+nClus; ++it ) {
          if ( cSize(*it) || it+1 == first+nClus || !cSize(*(it+1)) ) { //regular, or last cluster (so nothing extra to do), or next cluster is not last fragment
            make_cluster( channel(*it), fraction(*it), 4 );
          } else  {
            make_clusters( *it, *(it+1) );
            ++it;
          }
        }//end loop over clusters in one sipm
      } else {
        //bank version == 3
        //normal clustering without any modification to clusters, should work for encoder=2
        std::transform( first, first+nClus,
                        std::back_inserter(clus),
                        [&](short int c) -> LHCb::FTLiteCluster {
                        return  { LHCb::FTChannelID{ station, layer, quarter, module, mat, sipm, channel(c) },
                                  fraction(c), ( cSize(c) ? 0 : 4 ) };
        } );
      }
      first += nClus;
    }//end loop over sipms
    partitionPoints.push_back(clus.size());
  }//end loop over rawbanks
  assert( partitionPoints.back() == static_cast<int>(clus.size()) );
  if ( msgLevel(MSG::VERBOSE) ) {
      for ( const auto& c : clus ) verbose() << format( " channel %4d frac %3d size %3d ",
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
                         unsigned int iQua = chanID.quarter();
                         // Swap clusters within modules
                         // if quadrant==0 or 3 for even layers or quadrant==1 or 2 for odd layers
                         if( ((chanID.layer()&1)==0) ^ (iQua>>1) ^ (iQua&1) ) {
                           reverse_each_module(first,last);
                         }
                         // Swap clusters within full quadrant
                         if( (iQua & 1) == 0 ) { // quadrant==0 or 2
                           std::reverse(first, last);  // swap clusters in quadrant
                         }
  });

  return clus;
}

