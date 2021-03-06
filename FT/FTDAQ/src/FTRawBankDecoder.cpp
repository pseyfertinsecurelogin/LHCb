/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files

// from Gaudi
#include "Event/RawEvent.h"

// local
#include "FTDAQ/FTDAQHelper.h"
#include "FTRawBankDecoder.h"

// make range v3 include used version specific.
#include "range/v3/version.hpp"
#if RANGE_V3_VERSION < 900
#  include "range/v3/iterator_range.hpp"
#  define MY_MAKE_RANGE ranges::make_iterator_range
// upstream has renamed namespace ranges::view ranges::views
namespace ranges::views {
  using namespace ranges::view;
}
#else
#  include "range/v3/view/subrange.hpp"
#  define MY_MAKE_RANGE ranges::make_subrange
#endif
#include "range/v3/view/transform.hpp"

//-----------------------------------------------------------------------------
// Implementation file for class : FTRawBankDecoder
//
// 2012-05-11 : Olivier Callot
//-----------------------------------------------------------------------------

namespace {
  unsigned quarterFromChannel( LHCb::FTChannelID id ) { return id.uniqueQuarter() - 16u; }

  constexpr unsigned channelInBank( short int c ) { return ( c >> FTRawBank::cellShift ); }

  constexpr unsigned getLinkInBank( short int c ) { return ( ( c >> FTRawBank::linkShift ) ); }

  constexpr int cell( short int c ) { return ( c >> FTRawBank::cellShift ) & FTRawBank::cellMaximum; }

  constexpr int fraction( short int c ) { return ( c >> FTRawBank::fractionShift ) & FTRawBank::fractionMaximum; }

  constexpr bool cSize( short int c ) { return ( c >> FTRawBank::sizeShift ) & FTRawBank::sizeMaximum; }

  constexpr auto is_in_module = []( unsigned int mod ) {
    return [mod]( const LHCb::FTLiteCluster cluster ) { return cluster.channelID().module() == mod; };
  };

  template <typename Iter>
  void reverse_each_module( Iter first, Iter last ) {
    for ( unsigned int iMod = 0; iMod < 5; ++iMod ) {
      auto finish = std::partition_point( first, last, is_in_module( iMod ) );
      std::reverse( first, finish ); // swap clusters in module
      first = finish;
    }
    assert( std::all_of( first, last, is_in_module( 5 ) ) && "Remaining partition should all be in module 5..." );
    std::reverse( first, last );
  }

  template <typename Container, typename Fun>
  void for_each_quadrant( Container& c, Fun&& f ) {
    for ( uint16_t iQuarter = 0; iQuarter < c.nSubDetectors(); ++iQuarter ) {
      auto range = c.range_( iQuarter );
      f( range.first, range.second, iQuarter );
    }
  }
} // namespace

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FTRawBankDecoder )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
FTRawBankDecoder::FTRawBankDecoder( const std::string& name, ISvcLocator* pSvcLocator )
    : Transformer( name, pSvcLocator,
                   KeyValue{"RawEventLocations", Gaudi::Functional::concat_alternatives(
                                                     LHCb::RawEventLocation::Other, LHCb::RawEventLocation::Default )},
                   KeyValue{"OutputLocation", LHCb::FTLiteClusterLocation::Default} ) {
  // For the v2 and v3 decoding versions, opt out for the default initialization
  // of m_readouttool.
  m_decodingVersion
      .declareUpdateHandler( [=]( Property& ) { this->m_readoutTool.setEnabled( this->m_decodingVersion > 3u ); } )
      .useUpdateHandler();
}

template <>
FTLiteClusters FTRawBankDecoder::decode<6>( LHCb::span<const LHCb::RawBank*> banks, unsigned int nClusters ) const {
  FTLiteClusters clus{nClusters};
  for ( const LHCb::RawBank* bank : banks ) { // Iterates over the banks
    LHCb::FTChannelID offset  = m_readoutTool->channelIDShift( bank->sourceID() );
    auto              quarter = quarterFromChannel( offset );

    // Define Lambda functions to be used in loop
    auto make_cluster = [&clus, &quarter]( unsigned chan, int fraction, int size ) {
      clus.addHit( std::forward_as_tuple( chan, fraction, size ), quarter );
    };

    // Make clusters between two channels
    auto make_clusters = [&]( unsigned firstChannel, short int c, short int c2 ) {
      unsigned int widthClus = ( cell( c2 ) - cell( c ) + 2 ); // lastCh-firstCh+4/2

      // fragmented clusters, size > 2*max size
      // only edges were saved, add middles now
      if ( widthClus > 8 ) {
        // add the first edge cluster, and then the middle clusters
        unsigned int i = 0;
        for ( ; i < widthClus - 4; i += 4 ) {
          // all middle clusters will have same size as the first cluster,
          // for max size 4, fractions is always 1
          make_cluster( firstChannel + i, 1, 0 );
        }

        // add the last edge
        unsigned int lastChannel = firstChannel + i + std::floor( ( widthClus - i - 1 ) / 2 ) - 1;
        make_cluster( lastChannel, ( widthClus - 1 ) % 2, 0 );

      } else { // big cluster size upto size 8

        make_cluster( firstChannel + ( widthClus - 1 ) / 2 - 1, ( widthClus - 1 ) % 2, widthClus );

      } // end if adjacent clusters
    };  // End lambda make_clusters

    // loop over clusters
    auto it   = bank->begin<short int>() + 2; // skip first 32b of header
    auto last = bank->end<short int>();
    if ( *( last - 1 ) == 0 ) --last; // Remove padding at the end

    for ( ; it < last; ++it ) { // loop over the clusters
      unsigned short int c       = *it;
      LHCb::FTChannelID  channel = offset + channelInBank( c );

      if ( !cSize( c ) ) // Not flagged as large
        make_cluster( channel, fraction( c ), 4 );

      else if ( fraction( c ) ) { // flagged as first edge of large cluster

        // last cluster in bank or in sipm
        if ( it + 1 == last || getLinkInBank( c ) != getLinkInBank( *( it + 1 ) ) )
          make_cluster( channel, fraction( c ), 0 );

        else {
          unsigned c2 = *( it + 1 );

          if ( cSize( c2 ) && !fraction( c2 ) ) { // this should always be true
            make_clusters( channel, c, c2 );
            ++it;
          } else { // this should never happen,
            ++m_corrupt;
          }
        }
      }
    }
  } // end loop over rawbanks
  clus.setOffsets();
  return clus;
}

template <>
FTLiteClusters FTRawBankDecoder::decode<4>( LHCb::span<const LHCb::RawBank*> banks, unsigned int nClusters ) const {
  FTLiteClusters clus{nClusters};
  for ( const LHCb::RawBank* bank : banks ) { // Iterates over the banks
    LHCb::FTChannelID offset = m_readoutTool->channelIDShift( bank->sourceID() );
    auto              first  = bank->begin<short int>() + 2; // skip first 32b of the header
    auto              last   = bank->end<short int>();
    if ( *( last - 1 ) == 0 && first < last ) --last; // Remove padding at the end

    auto r = MY_MAKE_RANGE( first, last ) |
             ranges::views::transform( [&offset]( unsigned short int c ) -> LHCb::FTLiteCluster {
               return {offset + channelInBank( c ), fraction( c ), ( cSize( c ) ? 0 : 4 )};
             } );
    clus.insert( r.begin(), r.end(), quarterFromChannel( offset ) );
  }
  return clus;
}

template <>
FTLiteClusters FTRawBankDecoder::decode<5>( LHCb::span<const LHCb::RawBank*> banks, unsigned int nClusters ) const {
  FTLiteClusters clus{nClusters};
  for ( const LHCb::RawBank* bank : banks ) { // Iterates over the banks
    LHCb::FTChannelID offset  = m_readoutTool->channelIDShift( bank->sourceID() );
    auto              quarter = quarterFromChannel( offset );

    // Define Lambda functions to be used in loop
    auto make_cluster = [&clus, &quarter]( unsigned chan, int fraction, int size ) {
      clus.addHit( std::forward_as_tuple( chan, fraction, size ), quarter );
    };

    // Make clusters between two channels
    auto make_clusters = [&]( unsigned firstChannel, short int c, short int c2 ) {
      unsigned int delta = ( cell( c2 ) - cell( c ) );

      // fragmented clusters, size > 2*max size
      // only edges were saved, add middles now
      if ( delta > m_clusterMaxWidth ) {
        // add the first edge cluster, and then the middle clusters
        for ( unsigned int i = m_clusterMaxWidth; i < delta; i += m_clusterMaxWidth ) {
          // all middle clusters will have same size as the first cluster,
          // so re-use the fraction
          make_cluster( firstChannel + i, fraction( c ), 0 );
        }
        // add the last edge
        make_cluster( firstChannel + delta, fraction( c2 ), 0 );
      } else { // big cluster size upto size 8
        unsigned int widthClus = 2 * delta - 1 + fraction( c2 );
        make_cluster( firstChannel + ( widthClus - 1 ) / 2 - int( ( m_clusterMaxWidth - 1 ) / 2 ),
                      ( widthClus - 1 ) % 2, widthClus );
      } // end if adjacent clusters
    };  // End lambda make_clusters

    // loop over clusters
    auto it   = bank->begin<short int>() + 2; // skip first 32b of header
    auto last = bank->end<short int>();
    if ( *( last - 1 ) == 0 ) --last; // Remove padding at the end
    for ( ; it < last; ++it ) {       // loop over the clusters
      unsigned short int c       = *it;
      LHCb::FTChannelID  channel = offset + channelInBank( c );

      if ( !cSize( c ) || it + 1 == last ) // No size flag or last cluster
        make_cluster( channel, fraction( c ), 4 );
      else { // Flagged or not the last one.
        unsigned c2 = *( it + 1 );
        if ( cSize( c2 ) && getLinkInBank( c ) == getLinkInBank( c2 ) ) {
          make_clusters( channel, c, c2 );
          ++it;
        } else {
          make_cluster( channel, fraction( c ), 4 );
        }
      }
    }
  } // end loop over rawbanks
  clus.setOffsets();
  return clus;
}

template <unsigned int vrsn>
FTLiteClusters FTRawBankDecoder::decode( LHCb::span<const LHCb::RawBank*> banks, unsigned int nClusters ) const {
  static_assert( vrsn == 2 || vrsn == 3 );
  FTLiteClusters clus{nClusters};
  for ( const LHCb::RawBank* bank : banks ) {
    int      source  = bank->sourceID();
    unsigned station = source / 16 + 1u;
    unsigned layer   = ( source & 12 ) / 4;
    unsigned quarter = source & 3;

    if ( msgLevel( MSG::VERBOSE ) )
      verbose() << "source " << source << " station " << station << " layer " << layer << " quarter " << quarter
                << " size " << bank->size() << endmsg;

    auto first = bank->begin<short int>();
    auto last  = bank->end<short int>();

    while ( first != last ) {
      int sipmHeader = *first++;
      if ( first == last && sipmHeader == 0 ) continue; // padding at the end...
      unsigned modulesipm = sipmHeader >> 4;
      unsigned module     = modulesipm >> 4;
      unsigned mat        = ( modulesipm & 15 ) >> 2; // hardcoded: this should be replaced by mapping
      unsigned sipm       = modulesipm & 3;           // hardcoded: this should be replaced by mapping
      int      nClus      = sipmHeader & 15;

      if ( UNLIKELY( msgLevel( MSG::VERBOSE ) && nClus > 0 ) )
        verbose() << " Module " << module << " mat " << mat << " SiPM " << sipm << " nClusters " << nClus << endmsg;

      if ( UNLIKELY( nClus > std::distance( first, last ) ) ) {
        warning() << "Inconsistent size of rawbank. #clusters in header=" << nClus
                  << ", #clusters in bank=" << std::distance( first, last ) << endmsg;

        throw GaudiException( "Inconsistent size of rawbank", "FTRawBankDecoder", StatusCode::FAILURE );
      }

      if ( UNLIKELY( module > 5 ) ) {
        ++m_nonExistingModule;
        first += nClus;
        continue;
      }

      if constexpr ( vrsn == 3u ) {

        for ( auto it = first; it < first + nClus; ++it ) {
          short int c        = *it;
          unsigned  channel  = c & 127;
          int       fraction = ( c >> 7 ) & 1;
          bool      cSize    = ( c >> 8 ) & 1;

          // not the last cluster
          if ( !cSize && it < first + nClus - 1 ) {
            short int c2     = *( it + 1 );
            bool      cSize2 = ( c2 >> 8 ) & 1;

            if ( !cSize2 ) { // next cluster is not last fragment
              clus.addHit( std::forward_as_tuple(
                               LHCb::FTChannelID{station, layer, quarter, module, mat, sipm, channel}, fraction, 4 ),
                           bank->sourceID() );

              if ( msgLevel( MSG::VERBOSE ) ) {
                verbose() << format( "size<=4  channel %4d frac %3d size %3d code %4.4x", channel, fraction, cSize, c )
                          << endmsg;
              }
            } else { // fragmented cluster, last edge found
              unsigned channel2  = c2 & 127;
              int      fraction2 = ( c2 >> 7 ) & 1;

              unsigned int diff = ( channel2 - channel );

              if ( UNLIKELY( diff > 128 ) ) {
                error() << "something went terribly wrong here first fragment: " << channel
                        << " second fragment: " << channel2 << endmsg;
                throw GaudiException( "There is an inconsistency between Encoder and Decoder!", "FTRawBankDecoder",
                                      StatusCode::FAILURE );
              }
              // fragemted clusters, size > 2*max size
              // only edges were saved, add middles now
              if ( diff > m_clusterMaxWidth ) {

                // add the first edge cluster
                clus.addHit( std::forward_as_tuple(
                                 LHCb::FTChannelID{station, layer, quarter, module, mat, sipm, channel}, fraction, 0 ),
                             bank->sourceID() ); // pseudoSize=0

                if ( msgLevel( MSG::VERBOSE ) ) {
                  verbose() << format( "first edge cluster %4d frac %3d size %3d code %4.4x", channel, fraction, cSize,
                                       c )
                            << endmsg;
                }

                for ( unsigned int i = m_clusterMaxWidth; i < diff; i += m_clusterMaxWidth ) {
                  // all middle clusters will have same size as the first cluster,
                  // so use same fraction
                  clus.addHit(
                      std::forward_as_tuple( LHCb::FTChannelID{station, layer, quarter, module, mat, sipm, channel + i},
                                             fraction, 0 ),
                      bank->sourceID() );

                  if ( msgLevel( MSG::VERBOSE ) ) {
                    verbose() << format( "middle cluster %4d frac %3d size %3d code %4.4x", channel + i, fraction,
                                         cSize, c )
                              << " added " << diff << endmsg;
                  }
                }

                // add the last edge
                clus.addHit(
                    std::forward_as_tuple( LHCb::FTChannelID{station, layer, quarter, module, mat, sipm, channel2},
                                           fraction2, 0 ),
                    bank->sourceID() );

                if ( msgLevel( MSG::VERBOSE ) ) {
                  verbose() << format( "last edge cluster %4d frac %3d size %3d code %4.4x", channel2, fraction2,
                                       cSize2, c2 )
                            << endmsg;
                }
              } else { // big cluster size upto size 8
                unsigned int firstChan = channel - int( ( m_clusterMaxWidth - 1 ) / 2 );
                unsigned int widthClus = 2 * diff - 1 + fraction2;

                unsigned int clusChanPosition = firstChan + ( widthClus - 1 ) / 2;
                int          frac             = ( widthClus - 1 ) % 2;

                // add the new cluster = cluster1+cluster2
                clus.addHit( std::forward_as_tuple(
                                 LHCb::FTChannelID{station, layer, quarter, module, mat, sipm, clusChanPosition}, frac,
                                 widthClus ),
                             bank->sourceID() );

                if ( msgLevel( MSG::VERBOSE ) ) {
                  verbose() << format( "combined cluster %4d frac %3d size %3d code %4.4x", channel, fraction, cSize,
                                       c )
                            << endmsg;
                }
              } // end if adjacent clusters
              ++it;
            }    // last edge foud
          }      // not the last cluster
          else { // last cluster, so nothing we can do
            clus.addHit( std::forward_as_tuple( LHCb::FTChannelID{station, layer, quarter, module, mat, sipm, channel},
                                                fraction, 4 ),
                         bank->sourceID() );

            if ( msgLevel( MSG::VERBOSE ) ) {
              verbose() << format( "size<=4  channel %4d frac %3d size %3d code %4.4x", channel, fraction, cSize, c )
                        << endmsg;
            }
          }    // last cluster added
        }      // end loop over clusters in one sipm
      } else { // bank vrsn == 2
        static_assert( vrsn == 2 );
        // normal clustering without any modification to clusters, should work for encoder=2
        for ( auto it = first; it < first + nClus; ++it ) {
          short int c        = *it;
          unsigned  channel  = ( c >> 0 ) & 127;
          int       fraction = ( c >> 7 ) & 1;
          int       cSize    = ( c >> 8 ) & 1;
          clus.addHit( std::forward_as_tuple( LHCb::FTChannelID{station, layer, quarter, module, mat, sipm, channel},
                                              fraction, ( cSize ? 0 : 4 ) ),
                       bank->sourceID() );
        }
      }
      first += nClus;
    } // end loop over sipms
  }   // end loop over rawbanks
  clus.setOffsets();
  return clus;
}

//=============================================================================
// Main execution
//=============================================================================
FTLiteClusters FTRawBankDecoder::operator()( const LHCb::RawEvent& rawEvent ) const {
  const auto& banks = rawEvent.banks( LHCb::RawBank::FTCluster );

  if ( msgLevel( MSG::DEBUG ) ) debug() << "Number of raw banks " << banks.size() << endmsg;
  if ( banks.empty() ) return {};
  ;

  // Testing the bank version
  unsigned int vrsn = banks[0]->version();
  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Bank version=v" << vrsn << " with decoding version=v" << m_decodingVersion.toString() << endmsg;

  // Check if decoding version corresponds with bank version (only for first bank).
  // Special case for v5 data that is decoded as v4. This possibility is added
  // temporarily to test the tracking performance versus decoding speed.
  if ( UNLIKELY( vrsn != m_decodingVersion ) && !( vrsn == 5u && m_decodingVersion == 4u ) ) {
    error() << "Bank version=v" << vrsn << " is not compatible with decoding "
            << "version=v" << m_decodingVersion.toString() << endmsg;
    throw GaudiException( "Wrong decoding version", "FTRawBankDecoder", StatusCode::FAILURE );
  }

  // Estimate total number of clusters from bank sizes
  auto clus = [&]( unsigned int nClusters ) {
    switch ( m_decodingVersion.value() ) {
    case 2:
      return decode<2>( banks, nClusters );
    case 3:
      return decode<3>( banks, nClusters );
    case 4:
      return decode<4>( banks, nClusters );
    case 5:
      return decode<5>( banks, nClusters );
    case 6:
      return decode<6>( banks, nClusters );
    default:
      throw GaudiException( "Unknown decoder version: " + std::to_string( vrsn ), __FILE__, StatusCode::FAILURE );
    };
  }( LHCb::FTDAQ::nbFTClusters( banks ) );

  if ( msgLevel( MSG::VERBOSE ) ) {
    for ( const auto& c : clus.range() )
      verbose() << format( " channel %4d frac %3d size %3d ", c.channelID(), c.fraction(), c.pseudoSize() ) << endmsg;
  }

  // Assert that clusters are sorted
  assert( std::is_sorted( clus.range().begin(), clus.range().end(),
                          []( const LHCb::FTLiteCluster& lhs, const LHCb::FTLiteCluster& rhs ) {
                            return lhs.channelID() < rhs.channelID();
                          } ) &&
          "Clusters from the RawBanks not sorted. Should be sorted by construction." );

  // sort clusters according to PrFTHits (loop over quadrants)
  for_each_quadrant( clus, []( auto first, auto last, auto iUQua ) {
    if ( first == last ) return;
    // Swap clusters within modules
    // if quadrant==0 or 3 for even layers or quadrant==1 or 2 for odd layers
    if ( ( ( ( iUQua >> 2 ) & 1 ) == 0 ) ^ ( ( iUQua & 3 ) >> 1 ) ^ ( iUQua & 1 ) ) {
      reverse_each_module( first, last );
    }
    // Swap clusters within full quadrant
    if ( ( iUQua & 1 ) == 0 ) {    // quadrant==0 or 2
      std::reverse( first, last ); // swap clusters in quadrant
    }
  } );

  return clus;
}
