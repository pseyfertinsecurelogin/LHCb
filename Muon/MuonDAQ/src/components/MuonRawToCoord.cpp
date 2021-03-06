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
// local
#include "Gaudi/Algorithm.h"
#include "GaudiAlg/FixTESPath.h"
#include "GaudiAlg/Transformer.h"

#include "DetDesc/Condition.h"
#include "DetDesc/ConditionAccessorHolder.h"
#include "Event/MuonCoord.h"
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "MuonDet/DeMuonDetector.h"
#include "MuonDet/MuonBasicGeometry.h"

#include "range/v3/view/drop_exactly.hpp"
#include "range/v3/view/map.hpp"
#include "range/v3/view/remove_if.hpp"
#include "range/v3/view/take_exactly.hpp"
#include "range/v3/view/zip.hpp"

#include "range/v3/version.hpp"
#if RANGE_V3_VERSION < 900
#  include "range/v3/iterator_range.hpp"
namespace ranges {
  template <typename... Args>
  auto make_subrange( Args&&... args ) {
    return make_iterator_range( std::forward<Args>( args )... );
  }
} // namespace ranges
#else
#  include "range/v3/view/subrange.hpp"
#endif

#include <functional>
#include <optional>

// define error enum / category
namespace MuonRaw {
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
      switch ( static_cast<MuonRaw::ErrorCode>( code ) ) {
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
} // namespace MuonRaw
STATUSCODE_ENUM_DECL( MuonRaw::ErrorCode )
STATUSCODE_ENUM_IMPL( MuonRaw::ErrorCode, MuonRaw::ErrorCategory )

namespace {
  struct Digit {
    LHCb::MuonTileID tile;
    unsigned int     tdc;
  };
  using Digits = std::vector<Digit>;

  [[gnu::noreturn]] void throw_exception( MuonRaw::ErrorCode ec, const char* tag ) {
    auto sc = StatusCode( ec );
    throw GaudiException{sc.message(), tag, std::move( sc )};
  }
#define OOPS( x ) throw_exception( x, __PRETTY_FUNCTION__ )

  /// fills in the two readout layouts by querying the DeMuonRegion
  template <int N = 0>
  MuonLayout makeStripLayout( const DeMuonDetector& det, const LHCb::MuonTileID& tile ) {
    static_assert( N == 0 || N == 1 );
    unsigned int station = tile.station();
    unsigned int region  = tile.region();
    unsigned int x       = det.getLayoutX( N, station, region );
    unsigned int y       = det.getLayoutY( N, station, region );
    return {x, y};
  }

  int nDigits( const LHCb::RawBank& rb ) {
    auto range = rb.range<unsigned short>();
    if ( range.empty() ) return 0;
    auto preamble_size = 2 * ( ( range[0] + 3 ) / 2 );
    auto overhead      = preamble_size + 4;
    return range.size() > overhead ? range.size() - overhead : 0;
  }

  int nDigits( LHCb::span<const LHCb::RawBank*> rbs ) {
    return std::accumulate( rbs.begin(), rbs.end(), 0,
                            []( int s, const LHCb::RawBank* rb ) { return rb ? s + nDigits( *rb ) : s; } );
  }

  /// convert raw data into tiles
  template <typename MakeTile, typename OutputIterator>
  OutputIterator decodeTileAndTDCV1( LHCb::span<const unsigned short> rawdata, MakeTile&& make_tile,
                                     OutputIterator out ) {
    // minimum length is three 32 bit words --> 12 bytes -> 6 unsigned shorts
    if ( rawdata.size() < 6 ) OOPS( MuonRaw::ErrorCode::BANK_TOO_SHORT );
    auto preamble_size = 2 * ( ( rawdata[0] + 3 ) / 2 );
    if ( rawdata.size() < preamble_size ) OOPS( MuonRaw::ErrorCode::PADDING_TOO_LONG );
    rawdata = rawdata.subspan( preamble_size );
    for ( int i = 0; i < 4; ++i ) {
      if ( UNLIKELY( rawdata.empty() ) ) OOPS( MuonRaw::ErrorCode::BANK_TOO_SHORT );
      if ( UNLIKELY( rawdata.size() < 1 + rawdata[0] ) ) OOPS( MuonRaw::ErrorCode::TOO_MANY_HITS );
      for ( unsigned int pp : rawdata.subspan( 1, rawdata[0] ) ) {
        unsigned int                    add       = ( pp & 0x0FFF );
        unsigned int                    tdc_value = ( ( pp & 0xF000 ) >> 12 );
        std::optional<LHCb::MuonTileID> tile      = make_tile( add );
        if ( UNLIKELY( !tile.has_value() ) ) continue;
        *out++ = {std::move( *tile ), tdc_value};
      }
      rawdata = rawdata.subspan( 1 + rawdata[0] );
    }
    assert( rawdata.size() < 2 );
    return out;
  }

  /// Copy MuonTileID from digits to coord by crossing the digits
  template <typename Iterator>
  Iterator addCoordsCrossingMap( Iterator first, Iterator pivot, Iterator last, std::vector<LHCb::MuonCoord>& retVal ) {
    assert( first != last );
    assert( first <= pivot );
    assert( pivot <= last );

    static_assert( std::is_same_v<typename std::iterator_traits<Iterator>::value_type, Digit> );

    // used flags
    std::vector<bool> used( last - first, false );

    // partition into the two directions of digits
    // vertical and horizontal stripes
    const auto N = std::distance( first, pivot );

    auto usedAndDigits = ranges::view::zip( used, ranges::make_subrange( first, last ) );
    auto digitsOne     = ( usedAndDigits | ranges::view::take_exactly( N ) );
    auto digitsTwo     = ( usedAndDigits | ranges::view::drop_exactly( N ) );

    // check how many cross
    retVal.reserve( digitsOne.size() * digitsTwo.size() + ( last - first ) );
    for ( auto&& [used_one, digit_one] : digitsOne ) {
      for ( auto&& [used_two, digit_two] : digitsTwo ) {
        LHCb::MuonTileID pad = digit_one.tile.intercept( digit_two.tile );
        if ( !pad.isValid() ) continue;

        retVal.emplace_back( pad, digit_one.tile, digit_two.tile, digit_one.tdc, digit_two.tdc );
        // set used flag
        used_one = used_two = true;
      }
    }
    // copy over "uncrossed" digits
    for ( const Digit& digit :
          usedAndDigits | ranges::view::remove_if( []( const auto& p ) { return p.first; } ) | ranges::view::values ) {
      retVal.emplace_back( digit.tile, digit.tdc );
    }
    return last;
  }

  template <typename Projection, typename Cmp = std::less<>>
  auto orderByProjection( Projection&& projection, Cmp&& cmp = {} ) {
    return [proj = std::forward<Projection>( projection ), cmp = std::forward<Cmp>( cmp )]( const auto& lhs,
                                                                                            const auto& rhs ) {
      return std::invoke( cmp, std::invoke( proj, lhs ), std::invoke( proj, rhs ) );
    };
  }
  template <typename Projection, typename Value>
  auto hasEqualProjection( Projection&& projection, const Value& ref ) {
    auto proj_ref = std::invoke( projection, ref );
    return [proj_ref, proj = std::forward<Projection>( projection )]( const Value& val ) {
      return proj_ref == std::invoke( proj, val );
    };
  }
  auto stationRegion( const Digit& d ) { return std::tuple{d.tile.station(), d.tile.region()}; }
} // namespace

//-----------------------------------------------------------------------------
// Implementation file for class : MuonRawToCoord
//-----------------------------------------------------------------------------
/** @class MuonRawToCoord MuonRawToCoord.h
 *  This is the muon reconstruction algorithm
 *  This just crosses the logical strips back into pads
 */

class MuonRawToCoord final : public Gaudi::Functional::Transformer<std::vector<LHCb::MuonCoord>(
                                                                       const DeMuonDetector&, const LHCb::RawEvent& ),
                                                                   LHCb::DetDesc::usesConditions<DeMuonDetector>> {
public:
  /// Standard constructor
  MuonRawToCoord( const std::string& name, ISvcLocator* pSvcLocator );

  std::vector<LHCb::MuonCoord> operator()( const DeMuonDetector&, const LHCb::RawEvent& ) const override;

private:
  mutable Gaudi::Accumulators::BinomialCounter<>  m_invalid_add{this, "invalid add"};
  mutable Gaudi::Accumulators::AveragingCounter<> m_digits{this, "#digits"};
  mutable Gaudi::Accumulators::AveragingCounter<> m_coords{this, "#coords"};
  // TODO: add min/max to m_deltaEstimate...
  mutable Gaudi::Accumulators::AveragingCounter<> m_deltaEstimate{this, "#digits - estimated # digits"};
  mutable Gaudi::Accumulators::BinomialCounter<>  m_estNotOK{this, "#digits > estimated # digits"};
};

DECLARE_COMPONENT( MuonRawToCoord )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MuonRawToCoord::MuonRawToCoord( const std::string& name, ISvcLocator* pSvcLocator )
    : Transformer{name,
                  pSvcLocator,
                  {KeyValue{"MuonDetectorPath", DeMuonLocation::Default},
                   KeyValue{"RawEventLocation", LHCb::RawEventLocation::Default}},
                  KeyValue{"MuonCoordLocation", LHCb::MuonCoordLocation::MuonCoords}} {}

//=============================================================================
// Main execution
//=============================================================================
std::vector<LHCb::MuonCoord> MuonRawToCoord::operator()( const DeMuonDetector& muonDet,
                                                         const LHCb::RawEvent& raw ) const {

  std::vector<LHCb::MuonCoord> coords;
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;
  const auto& mb = raw.banks( LHCb::RawBank::Muon );
  if ( mb.empty() ) return coords;
  std::vector<Digit> decoding;
  int                est_nDigits = nDigits( mb );
  decoding.reserve( est_nDigits );
  for ( const auto* r : mb ) {
    if ( LHCb::RawBank::MagicPattern != r->magic() ) OOPS( MuonRaw::ErrorCode::BAD_MAGIC );
    unsigned int tell1 = r->sourceID();
    if ( tell1 >= MuonDAQHelper_maxTell1Number ) OOPS( MuonRaw::ErrorCode::INVALID_TELL1 );
    decodeTileAndTDCV1(
        r->range<unsigned short>(),
        [& di        = muonDet.getDAQInfo()->getADDInTell1( tell1 ),
         invalid_add = m_invalid_add.buffer()]( unsigned int add ) mutable {
          bool valid = add < di.size();
          invalid_add += !valid;
          return valid ? std::optional{di[add]} : std::nullopt;
        },
        std::back_inserter( decoding ) );
  }

  m_deltaEstimate += static_cast<int>( decoding.size() ) - est_nDigits;
  m_estNotOK += ( static_cast<int>( decoding.size() ) > est_nDigits );
  m_digits += decoding.size();
  if ( decoding.empty() ) {
    error() << "Error in decoding the muon raw data ";
    return coords;
  }
  auto first = decoding.begin();
  auto last  = decoding.end();
  while ( first != last ) {
    std::nth_element( first, first, last, orderByProjection( stationRegion ) );
    auto next = std::partition( std::next( first ), last, hasEqualProjection( stationRegion, *first ) );
    auto pivot =
        std::partition( first, next, [layout = makeStripLayout<0>( muonDet, first->tile )]( const Digit& digit ) {
          return digit.tile.layout() == layout;
        } );
    first = addCoordsCrossingMap( first, pivot, next, coords );
  }
  m_coords += coords.size();
  return coords;
}
