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
#include "DetDesc/Condition.h"
#include "DetDesc/ConditionAccessorHolder.h"
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "GaudiAlg/Transformer.h"
#include "GaudiKernel/ToolHandle.h"
#include "MuonDAQ/MuonHitContainer.h"
#include "MuonDet/DeMuonDetector.h"
#include "MuonDet/MuonTilePosition.h"
#include <array>
#include <boost/numeric/conversion/cast.hpp>
#include <functional>
#include <optional>
#include <string>
#include <vector>

/** @class RawToHits RawToHits.h
 *  This is the muon reconstruction algorithm
 *  This just crosses the logical strips back into pads
 */

namespace LHCb::Muon::DAQ {

  namespace {
    struct Digit {
      MuonTileID   tile;
      unsigned int tdc;
      Digit( MuonTileID tile, unsigned int tdc ) : tile{tile}, tdc{tdc} {} // C++20: please remove this line
    };
    int nDigits( const RawBank& rb ) {
      auto range = rb.range<unsigned short>();
      if ( range.empty() ) return 0;
      auto preamble_size = 2 * ( ( range[0] + 3 ) / 2 );
      auto overhead      = preamble_size + 4;
      return range.size() > overhead ? range.size() - overhead : 0;
    }

    int nDigits( span<const RawBank*> rbs ) {
      return std::accumulate( rbs.begin(), rbs.end(), 0,
                              []( int s, const RawBank* rb ) { return rb ? s + nDigits( *rb ) : s; } );
    }
    unsigned short make_tdc( unsigned short w ) {
      constexpr unsigned short TDCmask = 0xF000;
      return ( w & TDCmask ) >> 12;
    }
  } // namespace

  namespace EC::RawToHits {

    enum class ErrorCode : StatusCode::code_t {
      BAD_MAGIC = 10,
      BANK_TOO_SHORT,
      PADDING_TOO_LONG,
      TOO_MANY_HITS,
      INVALID_TELL1
    };
    struct ErrorCategory : StatusCode::Category {
      const char* name() const override { return "MuonRawBankDecoding"; }
      bool        isRecoverable( StatusCode::code_t ) const override { return false; }
      std::string message( StatusCode::code_t code ) const override {
        switch ( static_cast<ErrorCode>( code ) ) {
        case ErrorCode::BAD_MAGIC:
          return "Incorrect Magic pattern in raw bank";
        case ErrorCode::BANK_TOO_SHORT:
          return "Muon bank is too short";
        case ErrorCode::PADDING_TOO_LONG:
          return "Muon bank has too much padding for its size";
        case ErrorCode::TOO_MANY_HITS:
          return "Muon bank has too many hits for its size";
        case ErrorCode::INVALID_TELL1:
          return "Invalid TELL1 source ID";
        default:
          return StatusCode::default_category().message( code );
        }
      }
    };
  } // namespace EC::RawToHits
} // namespace LHCb::Muon::DAQ

STATUSCODE_ENUM_DECL( LHCb::Muon::DAQ::EC::RawToHits::ErrorCode )
STATUSCODE_ENUM_IMPL( LHCb::Muon::DAQ::EC::RawToHits::ErrorCode, LHCb::Muon::DAQ::EC::RawToHits::ErrorCategory )

namespace {
  [[gnu::noreturn]] void throw_exception( LHCb::Muon::DAQ::EC::RawToHits::ErrorCode ec, const char* tag ) {
    auto sc = StatusCode( ec );
    throw GaudiException{sc.message(), tag, std::move( sc )};
  }
} // namespace
#define OOPS( x ) throw_exception( x, __PRETTY_FUNCTION__ )

namespace LHCb::Muon::DAQ {
  namespace {
    template <typename Iterator>
    Iterator addCoordsCrossingMap( Iterator first, Iterator last, CommonMuonHits& commonHits,
                                   const ComputeTilePosition& compute, size_t nStations ) {
      // need to calculate the shape of the horizontal and vertical logical strips

      // used flags
      assert( std::distance( first, last ) < 400 );
      std::bitset<400> used; // (to be updated with new readout) the maximum # of channels per quadrant is currently 384
                             // (from M2R2)
      // partition into the two directions of digits
      // vertical and horizontal stripes
      const auto mid = std::partition( first, last, []( const Digit& digit ) { return digit.tile.isHorizontal(); } );

      auto digitsOne = make_span( first, mid );
      auto digitsTwo = make_span( mid, last );

      // check how many cross
      if ( first != mid && mid != last ) {
        auto thisGridX = first->tile.layout().xGrid();
        auto thisGridY = first->tile.layout().yGrid();

        auto otherGridX = mid->tile.layout().xGrid();
        auto otherGridY = mid->tile.layout().yGrid();

        unsigned i = 0;
        for ( const Digit& one : digitsOne ) {
          unsigned int calcX = one.tile.nX() * otherGridX / thisGridX;
          unsigned     j     = mid - first;
          for ( const Digit& two : digitsTwo ) {
            unsigned int calcY = two.tile.nY() * thisGridY / otherGridY;
            if ( calcX == two.tile.nX() && calcY == one.tile.nY() ) {
              MuonTileID pad( one.tile );
              pad.setY( two.tile.nY() );
              pad.setLayout( {thisGridX, otherGridY} );
              auto&& [pos, dx, dy] = compute.tilePosition( pad );
              commonHits.emplace_back( std::move( pad ), one.tile, two.tile, pos.X(), dx, pos.Y(), dy, pos.Z(), 0,
                                       one.tdc, one.tdc - two.tdc, 0 );
              used[i] = used[j] = true;
            }
            ++j;
          }
          ++i;
        }
      }

      // copy over "uncrossed" digits
      unsigned m = 0;
      for ( const Digit& digit : digitsOne ) {
        if ( !used[m++] ) {
          auto pos = ( ( digit.tile.station() > ( nStations - 3 ) && digit.tile.region() == 0 )
                           ? compute.tilePosition( digit.tile )
                           : compute.stripXPosition( digit.tile ) );
          commonHits.emplace_back( digit.tile, pos.p.X(), pos.dX, pos.p.Y(), pos.dY, pos.p.Z(), 0., 1, digit.tdc,
                                   digit.tdc );
        }
      }
      for ( const Digit& digit : digitsTwo ) {
        if ( !used[m++] ) {
          auto pos = ( ( digit.tile.station() > ( nStations - 3 ) && digit.tile.region() == 0 )
                           ? compute.tilePosition( digit.tile )
                           : compute.stripYPosition( digit.tile ) );
          commonHits.emplace_back( digit.tile, pos.p.X(), pos.dX, pos.p.Y(), pos.dY, pos.p.Z(), 0., 1, digit.tdc,
                                   digit.tdc );
        }
      }
      return last;
    }

    template <typename Iterator, typename MakeTile>
    void decodeTileAndTDC( const RawBank& rb, Iterator output, MakeTile&& make_tile ) {
      // minimum length is three 32 bit words --> 12 bytes -> 6 unsigned shorts
      if ( rb.size() < 12 ) { OOPS( EC::RawToHits::ErrorCode::BANK_TOO_SHORT ); }
      auto range         = rb.range<unsigned short>();
      auto preamble_size = 2 * ( ( range[0] + 3 ) / 2 );
      if ( range.size() < preamble_size ) { OOPS( EC::RawToHits::ErrorCode::PADDING_TOO_LONG ); }
      range = range.subspan( preamble_size );
      for ( int i = 0; i < 4; i++ ) {
        if ( UNLIKELY( range.empty() ) ) { OOPS( EC::RawToHits::ErrorCode::BANK_TOO_SHORT ); }
        if ( UNLIKELY( range.size() < 1 + range[0] ) ) { OOPS( EC::RawToHits::ErrorCode::TOO_MANY_HITS ); }
        for ( unsigned short data : range.subspan( 1, range[0] ) ) {
          auto tile = make_tile( data );
          if ( LIKELY( tile.has_value() ) ) *output++ = {std::move( *tile ), make_tdc( data )};
        }
        range = range.subspan( 1 + range[0] );
      }
      assert( range.size() < 2 );
    }

  } // namespace
  //-----------------------------------------------------------------------------
  // Implementation file for class : RawToHits
  //-----------------------------------------------------------------------------
  class RawToHits final
      : public Gaudi::Functional::Transformer<MuonHitContainer( const RawEvent&, const DeMuonDetector&,
                                                                const ComputeTilePosition& ),
                                              DetDesc::usesConditions<DeMuonDetector, ComputeTilePosition>> {
  public:
    /// Standard constructor
    RawToHits( const std::string& name, ISvcLocator* pSvcLocator );

    StatusCode       initialize() override; ///< Algorithm initialization
    MuonHitContainer operator()( const RawEvent&, const DeMuonDetector&, const ComputeTilePosition& ) const override;

  private:
    mutable Gaudi::Accumulators::BinomialCounter<> m_invalid_add{this, "invalid add"};
  };

  DECLARE_COMPONENT_WITH_ID( RawToHits, "MuonRawToHits" )

  //=============================================================================
  // Standard constructor
  //=============================================================================
  RawToHits::RawToHits( const std::string& name, ISvcLocator* pSvcLocator )
      : Transformer( name, pSvcLocator,
                     {KeyValue{"RawEventLocation", RawEventLocation::Default},
                      KeyValue{"MuonDetectorLocation", DeMuonLocation::Default},
                      KeyValue{"TilePositionCalculator", "AlgorithmSpecific-" + name + "-TilePositionCalculator"}},
                     KeyValue{"HitContainer", MuonHitContainerLocation::Default} ) {}

  //=============================================================================
  // Initialisation
  //=============================================================================
  StatusCode RawToHits::initialize() {
    auto sc = Transformer::initialize();
    if ( sc.isSuccess() )
      addConditionDerivation<ComputeTilePosition( const DeMuonDetector& )>( DeMuonLocation::Default,
                                                                            inputLocation<2>() );
    return sc;
  }

  //=============================================================================
  // Main execution
  //=============================================================================
  MuonHitContainer RawToHits::operator()( const RawEvent& raw, const DeMuonDetector& det,
                                          const ComputeTilePosition& compute ) const {

    if ( msgLevel( MSG::DEBUG ) ) { debug() << "==> Execute the decoding" << endmsg; }
    size_t nStations = boost::numeric_cast<size_t>( det.stations() );
    assert( nStations <= 4 );
    const auto&                      mb = raw.banks( RawBank::Muon );
    std::array<CommonMuonStation, 4> stations;
    if ( mb.empty() ) return MuonHitContainer{std::move( stations )};

    // array of vectors of hits
    // each element of the array correspond to hits from a single station
    // this will ease the sorting after
    std::array<std::vector<Digit>, 4> decoding;

    auto n_digits = nDigits( mb );
    for ( auto& decode : decoding ) { decode.reserve( n_digits ); }

    constexpr auto stationOfTell1 = std::array{0, 0, 0, 0, 1, 1, 2, 2, 3, 3}; // to be updated with tell40
    for ( const auto& rb : mb ) {
      if ( RawBank::MagicPattern != rb->magic() ) { OOPS( EC::RawToHits::ErrorCode::BAD_MAGIC ); }
      unsigned int tell1 = rb->sourceID();
      if ( tell1 >= MuonDAQHelper_maxTell1Number ) { OOPS( EC::RawToHits::ErrorCode::INVALID_TELL1 ); }
      int station = stationOfTell1[tell1];

      // decode the bank -- decide in which array to put the digits according to the Tell1 source
      auto make_tile = [& ADDToTile = det.getDAQInfo()->getADDInTell1( tell1 ),
                        invalid_add = m_invalid_add.buffer()]( unsigned short w ) mutable {
        constexpr unsigned short ADDmask = 0x0FFF; // there is no short literal suffix...
        unsigned short           add     = w & ADDmask;
        bool                     valid   = add < ADDToTile.size();
        invalid_add += !valid;
        return valid ? std::optional{ADDToTile[add]} : std::nullopt;
      };
      decodeTileAndTDC( *rb, back_inserter( decoding[station] ), make_tile );
    }

    if ( std::all_of( decoding.begin(), decoding.end(), []( const auto& v ) { return v.empty(); } ) ) {
      error() << "Error in decoding the muon raw data" << endmsg;
    }

    // sort the digits to ease the crossing
    // the hits come directly sorted by station due to tell1 reading
    // each element of the array represent one station
    constexpr auto regionAndQuarter = []( const Digit& i ) { return i.tile.region() * 4 + i.tile.quarter(); };
    for ( auto& decode : decoding ) {
      std::sort( decode.begin(), decode.end(),
                 [&]( const Digit& a, const Digit& b ) { return regionAndQuarter( a ) < regionAndQuarter( b ); } );
    }

    auto addCrossings = [&]( auto f, auto l, auto& dest ) {
      auto next = std::find_if( std::next( f ), l, [=, rq = regionAndQuarter( *f )]( const auto& k ) {
        return regionAndQuarter( k ) != rq;
      } );
      return addCoordsCrossingMap( f, next, dest, compute, nStations );
    };

    unsigned station = 0;
    for ( auto& decode : decoding ) {
      CommonMuonHits commonHits{};
      commonHits.reserve( decode.size() * 2 );

      for ( auto i = decode.begin(); i != decode.end(); i = addCrossings( i, decode.end(), commonHits ) )
        ; /* empty on purpose */

      stations[station] = CommonMuonStation{det, station, std::move( commonHits )};
      station++;
    }
    return MuonHitContainer{std::move( stations )};
  }
  //=============================================================================

} // namespace LHCb::Muon::DAQ
