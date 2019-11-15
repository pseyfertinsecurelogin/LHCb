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
#include <functional>
#include <optional>

#include "MuonRawToHits.h"

namespace MuonRawHits {

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
      switch ( static_cast<MuonRawHits::ErrorCode>( code ) ) {
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
} // namespace MuonRawHits
STATUSCODE_ENUM_DECL( MuonRawHits::ErrorCode )
STATUSCODE_ENUM_IMPL( MuonRawHits::ErrorCode, MuonRawHits::ErrorCategory )

namespace {
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

  [[gnu::noreturn]] void throw_exception( MuonRawHits::ErrorCode ec, const char* tag ) {
    auto sc = StatusCode( ec );
    throw GaudiException{sc.message(), tag, std::move( sc )};
  }
#define OOPS( x ) throw_exception( x, __PRETTY_FUNCTION__ )

} // namespace

//-----------------------------------------------------------------------------
// Implementation file for class : MuonRawToHits
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( MuonRawToHits )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MuonRawToHits::MuonRawToHits( const std::string& name, ISvcLocator* pSvcLocator )
    : Transformer( name, pSvcLocator, KeyValue{"RawEventLocation", LHCb::RawEventLocation::Default},
                   KeyValue{"HitContainer", MuonHitContainerLocation::Default} ) {}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode MuonRawToHits::initialize() {
  auto sc = Transformer::initialize();
  if ( sc.isSuccess() ) {
    m_muonDetector = getDet<DeMuonDetector>( DeMuonLocation::Default );
    m_nStations    = boost::numeric_cast<size_t>( m_muonDetector->stations() );
    assert( m_nStations <= 4 );
  }

  // create the layouts
  for ( auto station = 0; station != 4; ++station ) {
    for ( auto region = 0; region != 4; ++region ) {
      unsigned int x1 = m_muonDetector->getLayoutX( 0, station, region );
      unsigned int y1 = m_muonDetector->getLayoutY( 0, station, region );
      unsigned int x2 = m_muonDetector->getLayoutX( 1, station, region );
      unsigned int y2 = m_muonDetector->getLayoutY( 1, station, region );
      if ( x1 > x2 ) {
        layouts[station + 4 * region] = {MuonLayout( x1, y1 ), MuonLayout( x2, y2 )};
      } else {
        layouts[station + 4 * region] = {MuonLayout( x2, y2 ), MuonLayout( x1, y1 )};
      }
    }
  }
  return !( m_muonDetector || m_muonPosTool )
             ? Error( "Could not read " + DeMuonLocation::Default + " or could not get MuonFastPosTool" )
             : sc;
}

//=============================================================================
// Main execution
//=============================================================================
MuonHitContainer MuonRawToHits::operator()( const LHCb::RawEvent& raw ) const {

  if ( msgLevel( MSG::DEBUG ) ) { debug() << "==> Execute the decoding" << endmsg; }
  const auto&                      mb = raw.banks( LHCb::RawBank::Muon );
  std::array<CommonMuonStation, 4> stations;
  if ( mb.empty() ) return MuonHitContainer{std::move( stations )};

  // decode tha data
  auto decoding = decodeTileAndTDC( mb );

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

  unsigned station = 0;
  for ( auto& decode : decoding ) {
    CommonMuonHits commonHits{};
    commonHits.reserve( decode.size() * 2 );

    auto itX = decode.begin();
    for ( auto jtX = itX; jtX != decode.end(); ++jtX ) {
      if ( regionAndQuarter( *jtX ) != regionAndQuarter( *itX ) ) {
        DigitsRange coordsPerRegQua = boost::make_iterator_range( itX, jtX );
        addCoordsCrossingMap( coordsPerRegQua, commonHits );
        itX = jtX;
      }
    }
    DigitsRange coordsPerRegQua = boost::make_iterator_range( itX, decode.end() );
    if ( coordsPerRegQua.size() != 0 ) addCoordsCrossingMap( coordsPerRegQua, commonHits );

    stations[station] = CommonMuonStation{*m_muonDetector, station, std::move( commonHits )};
    station++;
  }
  return MuonHitContainer{std::move( stations )};
}

//=============================================================================

void MuonRawToHits::addCoordsCrossingMap( DigitsRange& digits, CommonMuonHits& commonHits ) const {
  // need to calculate the shape of the horizontal and vertical logical strips

  // used flags
  std::bitset<400> used; // (to be updated with new readout) the maximum # of channels per quadrant is currently 384
                         // (from M2R2)
  // partition into the two directions of digits
  // vertical and horizontal stripes
  const auto mid =
      std::partition( digits.begin(), digits.end(), []( const Digit& digit ) { return digit.tile.isHorizontal(); } );

  auto digitsOne = boost::make_iterator_range( digits.begin(), mid );
  auto digitsTwo = boost::make_iterator_range( mid, digits.end() );

  // check how many cross
  if ( !digitsOne.empty() && !digitsTwo.empty() ) {
    int thisGridX = digitsOne.front().tile.layout().xGrid();
    int thisGridY = digitsOne.front().tile.layout().yGrid();

    int otherGridX = digitsTwo.front().tile.layout().xGrid();
    int otherGridY = digitsTwo.front().tile.layout().yGrid();

    unsigned i = 0;
    for ( const Digit& one : digitsOne ) {
      unsigned int calcX = one.tile.nX() * otherGridX / thisGridX;
      unsigned     j     = mid - digits.begin();

      for ( const Digit& two : digitsTwo ) {
        unsigned int calcY = two.tile.nY() * thisGridY / otherGridY;
        if ( calcX == two.tile.nX() && calcY == one.tile.nY() ) {
          LHCb::MuonTileID pad( one.tile );
          pad.setY( two.tile.nY() );
          pad.setLayout( MuonLayout( thisGridX, otherGridY ) );

          double x = 0., dx = 0., y = 0., dy = 0., z = 0., dz = 0.;
          m_muonPosTool->calcTilePos( pad, x, dx, y, dy, z, dz ).ignore();
          commonHits.emplace_back( std::move( pad ), one.tile, two.tile, x, dx, y, dy, z, dz, one.tdc,
                                   one.tdc - two.tdc, 0 );
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
    if ( !used[m] ) {

      double x = 0., dx = 0., y = 0., dy = 0., z = 0., dz = 0.;

      if ( digit.tile.station() > ( m_nStations - 3 ) && digit.tile.region() == 0 ) {
        m_muonPosTool->calcTilePos( digit.tile, x, dx, y, dy, z, dz ).ignore();
      } else {
        m_muonPosTool->calcStripXPos( digit.tile, x, dx, y, dy, z, dz ).ignore();
      }
      commonHits.emplace_back( digit.tile, x, dx, y, dy, z, dz, 1, digit.tdc, digit.tdc );
    }
    ++m;
  }

  for ( const Digit& digit : digitsTwo ) {
    if ( !used[m] ) {
      double x = 0., dx = 0., y = 0., dy = 0., z = 0., dz = 0.;

      if ( digit.tile.station() > ( m_nStations - 3 ) && digit.tile.region() == 0 ) {
        m_muonPosTool->calcTilePos( digit.tile, x, dx, y, dy, z, dz ).ignore();
      } else {
        m_muonPosTool->calcStripYPos( digit.tile, x, dx, y, dy, z, dz ).ignore();
      }
      commonHits.emplace_back( digit.tile, x, dx, y, dy, z, dz, 1, digit.tdc, digit.tdc );
    }
    ++m;
  }
}

std::array<std::vector<Digit>, 4> MuonRawToHits::decodeTileAndTDC( LHCb::span<const LHCb::RawBank*> mb ) const {

  // array of vectors of hits
  // each element of the array correspond to hits from a single station
  // this will ease the sorting after
  std::array<std::vector<Digit>, 4> storage;
  constexpr auto stationOfTell1 = std::array{0, 0, 0, 0, 1, 1, 2, 2, 3, 3}; // to be updated with tell40
  for ( auto& decode : storage ) { decode.reserve( nDigits( mb ) ); }

  for ( const auto& r : mb ) {
    if ( LHCb::RawBank::MagicPattern != r->magic() ) { OOPS( MuonRawHits::ErrorCode::BAD_MAGIC ); }
    unsigned int tell1Number = r->sourceID();
    if ( tell1Number >= MuonDAQHelper_maxTell1Number ) { OOPS( MuonRawHits::ErrorCode::INVALID_TELL1 ); }

    // decide in which array put the digits according to the Tell1 they come from
    int station = stationOfTell1[tell1Number];

    // minimum length is three 32 bit words --> 12 bytes -> 6 unsigned shorts
    if ( r->size() < 12 ) { OOPS( MuonRawHits::ErrorCode::BANK_TOO_SHORT ); }
    auto range         = r->range<unsigned short>();
    auto preamble_size = 2 * ( ( range[0] + 3 ) / 2 );
    if ( range.size() < preamble_size ) { OOPS( MuonRawHits::ErrorCode::PADDING_TOO_LONG ); }
    range = range.subspan( preamble_size );
    for ( int i = 0; i < 4; i++ ) {
      if ( UNLIKELY( range.empty() ) ) { OOPS( MuonRawHits::ErrorCode::BANK_TOO_SHORT ); }
      if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) { verbose() << " hit in PP " << range[0] << endmsg; }
      if ( UNLIKELY( range.size() < 1 + range[0] ) ) { OOPS( MuonRawHits::ErrorCode::TOO_MANY_HITS ); }
      for ( unsigned int pp : range.subspan( 1, range[0] ) ) {
        unsigned int     add       = ( pp & 0x0FFF );
        unsigned int     tdc_value = ( ( pp & 0xF000 ) >> 12 );
        LHCb::MuonTileID tile      = m_muonDetector->getDAQInfo()->getADDInTell1( tell1Number, add );
        if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) verbose() << " add " << add << " " << tile << endmsg;
        if ( tile.isValid() ) {
          if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << " valid add " << add << " " << tile << endmsg;
          storage[station].emplace_back( tile, tdc_value );
        } else {
          info() << "invalid add " << add << " " << tile << endmsg;
        }
      }
      range = range.subspan( 1 + range[0] );
    }
    assert( range.size() < 2 );
  }

  return storage;
}
