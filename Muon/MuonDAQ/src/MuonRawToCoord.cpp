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
#include "MuonRawToCoord.h"

#include <functional>
#include <optional>
#include "range/v3/view/remove_if.hpp"
#include "range/v3/view/take_exactly.hpp"
#include "range/v3/view/drop_exactly.hpp"
#include "range/v3/view/map.hpp"
#include "range/v3/view/zip.hpp"

using namespace LHCb;

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
    bool isRecoverable( StatusCode::code_t ) const override { return false; }
    std::string message( StatusCode::code_t code ) const override {
      switch ( static_cast<MuonRaw::ErrorCode>( code ) ) {
        case ErrorCode::BAD_MAGIC: return "Incorrect Magic pattern in raw bank";
        case ErrorCode::BANK_TOO_SHORT: return "Muon bank is too short";
        case ErrorCode::PADDING_TOO_LONG: return "Muon bank has too much padding for its size";
        case ErrorCode::TOO_MANY_HITS: return "Muon bank has too many hits for its size";
        case ErrorCode::INVALID_TELL1: return "Invalid TELL1 source ID";
        default: return StatusCode::default_category().message( code );
      }
    }
  };
}
STATUSCODE_ENUM_DECL( MuonRaw::ErrorCode )
STATUSCODE_ENUM_IMPL( MuonRaw::ErrorCode, MuonRaw::ErrorCategory )

namespace{
  //TODO make Digit and Digits common to all MuonDAQ
  struct Digit {
      LHCb::MuonTileID tile;
      unsigned int tdc;
  };
  using Digits = std::vector<Digit>;

  [[gnu::noreturn]] void throw_exception( MuonRaw::ErrorCode ec, const char* tag ) {
      auto sc = StatusCode( ec );
      throw GaudiException{ sc.message(), tag, std::move(sc) };
  }
#define OOPS(x) throw_exception(x,__PRETTY_FUNCTION__)

  /// fills in the two readout layouts by querying the DeMuonRegion
  template <int N = 0>
  MuonLayout  makeStripLayout(const DeMuonDetector& det, const MuonTileID& tile) {
    static_assert(N==0 || N==1);
    unsigned int station = tile.station();
    unsigned int region  = tile.region();
    unsigned int x = det.getLayoutX(N,station,region);
    unsigned int y = det.getLayoutY(N,station,region);
    return { x, y };
  }

  int nDigits(const RawBank& rb) {
    auto range = rb.range<unsigned short>();
    if (range.empty()) return 0;
    auto preamble_size = 2*((range[0]+3)/2);
    auto overhead = preamble_size+4;
    return range.size() > overhead ? range.size()-overhead : 0;
  }

  int nDigits( LHCb::span<const RawBank*> rbs) {
    return std::accumulate(rbs.begin(),rbs.end(),0,[](int s, const RawBank* rb) {
        return rb ? s + nDigits(*rb) : s ;
    } );
  }

  /// Copy MuonTileID from digits to coord by crossing the digits
  template <typename Iterator>
  Iterator addCoordsCrossingMap(const DeMuonDetector& det, Iterator first, Iterator last, std::vector<LHCb::MuonCoord>& retVal) {
    assert(first!=last);
    static_assert( std::is_same_v< typename std::iterator_traits<Iterator>::value_type, Digit > );
    using namespace ranges;

    // used flags
    std::vector<bool> used(last-first, false);

    // partition into the two directions of digits
    // vertical and horizontal stripes
    const auto N = std::distance(first,std::partition(first, last,
                                    [layout = makeStripLayout<0>(det, first->tile)]
                                    (const Digit& digit) { return digit.tile.layout() == layout; }));

    auto usedAndDigits = view::zip( used, iterator_range{ first, last }) ;
    auto digitsOne = ( usedAndDigits | view::take_exactly( N ) );
    auto digitsTwo = ( usedAndDigits | view::drop_exactly( N ) );

    // check how many cross
    retVal.reserve(digitsOne.size() * digitsTwo.size() + (last-first));
    for (auto&& [ used_one, digit_one ] : digitsOne ) {
      for (auto&& [ used_two, digit_two ] : digitsTwo ) {
        LHCb::MuonTileID pad = digit_one.tile.intercept(digit_two.tile);
        if (!pad.isValid()) continue;

        retVal.emplace_back(pad, digit_one.tile, digit_two.tile, digit_one.tdc, digit_two.tdc );
        // set used flag
        used_one = used_two = true;
      }
    }
    // copy over "uncrossed" digits
    for ( const Digit& digit : usedAndDigits
                             | view::remove_if( [](const auto& p ) { return p.first; } )
                             | view::values ) {
      retVal.emplace_back(digit.tile, digit.tdc );
    }
    return last;
  }

  template <typename Projection, typename Cmp = std::less<>>
  auto orderByProjection(Projection&& projection, Cmp&& cmp = {}) {
      return [proj=std::forward<Projection>(projection),
              cmp =std::forward<Cmp>(cmp)]
             (const auto& lhs, const auto& rhs)
             { return std::invoke(cmp,std::invoke(proj,lhs),std::invoke(proj,rhs)); };
  }
  template <typename Projection, typename Value>
  auto hasEqualProjection(Projection&& projection, const Value& ref) {
      auto proj_ref = std::invoke(projection,ref);
      return [proj_ref,proj=std::forward<Projection>(projection)](const Value& val)
             { return proj_ref == std::invoke(proj,val); };
  }
  auto stationRegion(const Digit& d) {
      return std::tuple{d.tile.station(),d.tile.region()};
  }
}

//-----------------------------------------------------------------------------
// Implementation file for class : MuonRawToCoord
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( MuonRawToCoord )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MuonRawToCoord::MuonRawToCoord( const std::string& name,
                  ISvcLocator* pSvcLocator)
  : Transformer ( name , pSvcLocator ,
        KeyValue{"RawEventLocation" , LHCb::RawEventLocation::Default},
        KeyValue{"MuonCoordLocation", LHCb::MuonCoordLocation::MuonCoords})
{
  m_forceResetDAQ=(context()=="TAE");
}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode MuonRawToCoord::initialize() {
  auto sc = Transformer::initialize();
  if (sc.isSuccess()) {
    m_muonDetector=getDet<DeMuonDetector>(DeMuonLocation::Default);
  }
  return !m_muonDetector ? Error("Could not read " + DeMuonLocation::Default ) : sc;
}

//=============================================================================
// Main execution
//=============================================================================
std::vector<LHCb::MuonCoord> MuonRawToCoord::operator()(const LHCb::RawEvent &raw) const {

  std::vector<LHCb::MuonCoord> coords;
  if( msgLevel(MSG::DEBUG) ) {
    debug() << "==> Execute" << endmsg;
  }
  const auto& mb = raw.banks(RawBank::Muon);
  if (mb.empty()) {
    return coords;
  }
  std::vector<Digit>  decoding;
  decoding.reserve( nDigits(mb) );
  for ( const auto& r : mb ) {
    decodeTileAndTDCV1(*r, std::back_inserter(decoding));
  }
  m_digits+=decoding.size();
  if (decoding.empty()) {
    error() << "Error in decoding the muon raw data ";
    return coords;
  }
  auto first = decoding.begin(); auto last = decoding.end();
  while ( first != last ) {
    std::nth_element( first, first, last, orderByProjection( stationRegion ) );
    auto next = std::partition( std::next(first), last, hasEqualProjection( stationRegion, *first ) );
    first = addCoordsCrossingMap( *m_muonDetector, first, next , coords);
  }
  m_coords += coords.size();
  return coords;
}

template <typename OutputIterator>
OutputIterator MuonRawToCoord::decodeTileAndTDCV1(const RawBank& rawdata, OutputIterator out) const {

  if (RawBank::MagicPattern != rawdata.magic() ) {
    OOPS( MuonRaw::ErrorCode::BAD_MAGIC );
  }
  unsigned int tell1Number=rawdata.sourceID();
  if (tell1Number>=MuonDAQHelper_maxTell1Number){
    OOPS( MuonRaw::ErrorCode::INVALID_TELL1 );
  }
  auto make_tile = [&di = m_muonDetector->getDAQInfo()->getADDInTell1(tell1Number)](unsigned int add) {
    return add<di.size() ? std::optional{di[add]} : std::nullopt;
  };
  //minimum length is 3 words --> 12 bytes
  if (rawdata.size()<12) {
    OOPS( MuonRaw::ErrorCode::BANK_TOO_SHORT );
  }
  auto range = rawdata.range<unsigned short>();
  auto preamble_size = 2*((range[0]+3)/2);
  if (range.size()<preamble_size) {
    OOPS( MuonRaw::ErrorCode::PADDING_TOO_LONG );
  }
  range = range.subspan( preamble_size );

  auto invalid_add = m_invalid_add.buffer();
  for(int i=0;i<4;++i){
    if (UNLIKELY(range.empty())) {
      OOPS( MuonRaw::ErrorCode::BANK_TOO_SHORT );
    }
    if( UNLIKELY( msgLevel(MSG::VERBOSE) ) ) {
      verbose()<<" hit in PP "<< range[0] <<endmsg;
    }
    if (UNLIKELY( range.size() < 1 + range[0] ) ) {
      OOPS( MuonRaw::ErrorCode::TOO_MANY_HITS );
    }
    for (unsigned int pp : range.subspan(1,range[0])) {
      unsigned int add       =  (pp&0x0FFF);
      unsigned int tdc_value = ((pp&0xF000)>>12);
      std::optional<LHCb::MuonTileID> tile = make_tile(add);
      invalid_add += !tile.has_value();
      if (UNLIKELY(!tile.has_value())) continue;
      *out++ = { std::move(*tile), tdc_value };
    }
    range = range.subspan(1+range[0]);
  }
  assert(range.empty());
  return out;
}
