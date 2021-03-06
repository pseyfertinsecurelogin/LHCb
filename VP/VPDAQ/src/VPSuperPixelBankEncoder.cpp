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

#include "Kernel/VPChannelID.h"

#include "VPSuperPixelBankEncoder.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : VPSuperPixelBankEncoder
//
// 2014-03-06 : Karol Hennessy, Kurt Rinnert
//-----------------------------------------------------------------------------
namespace {
  /// bank version. (change this every time semantics change!)
  constexpr unsigned int c_bankVersion = 2;

  /// to sort super pixels by column (major) and row (minor)
  auto SPLowerThan = []( unsigned int lhs, unsigned int rhs ) { return ( lhs & 0x7FFF00 ) < ( rhs & 0x7FFF00 ); };

  // convert channelID to super pixel words
  struct sp_word {
    sp_word( VPChannelID id ) noexcept {
      const unsigned int chip      = id.chip();
      const unsigned int row       = id.row();
      const unsigned int col       = id.col();
      const unsigned int sensorCol = col + 256 * chip;
      const unsigned int spCol     = sensorCol / 2;
      const unsigned int spRow     = row / 4;
      addr                         = ( ( spCol << 6 ) | spRow );
      ix                           = 0x1u << ( ( ( col % 2 ) * 4 ) + row % 4 );
    }
    unsigned int addr;
    unsigned int ix;
    unsigned int to_uint() const noexcept { return ( addr << 8 ) | ix; }
  };

} // namespace

DECLARE_COMPONENT( VPSuperPixelBankEncoder )

//=============================================================================
// Constructor
//=============================================================================
VPSuperPixelBankEncoder::VPSuperPixelBankEncoder( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {
  declareProperty( "DigitLocation", m_digitLocation = LHCb::VPDigitLocation::Default );
  declareProperty( "RawEventLocation", m_rawEventLocation = LHCb::RawEventLocation::Default );
}

//=============================================================================
//  Execution
//=============================================================================
StatusCode VPSuperPixelBankEncoder::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;
  ++m_evt;
  const VPDigits* digits = getIfExists<VPDigits>( m_digitLocation );
  if ( !digits ) { return Error( " ==> There are no VPDigits in TES! " ); }

  // Check if RawEvent exists
  RawEvent* rawEvent = getIfExists<RawEvent>( m_rawEventLocation );
  if ( !rawEvent ) {
    // Create RawEvent
    rawEvent = new LHCb::RawEvent();
    put( rawEvent, m_rawEventLocation );
  }

  for ( auto& s : m_spBySensor ) {
    // put header word as first element (but don't know content yet)
    s = {0};
  }

  // Loop over digits create super pixel words and store them.
  // No assumption about the order of digits is made.
  for ( const auto& seed : *digits ) {
    auto& sensor = m_spBySensor[seed->channelID().sensor()];
    auto  sp     = sp_word{seed->channelID()};

    auto j = std::find_if( sensor.begin(), sensor.end(), [&]( unsigned int s ) { return ( s >> 8 ) == sp.addr; } );

    if ( j != sensor.end() ) {
      *j |= sp.ix;
    } else {
      sensor.push_back( sp.to_uint() );
    }
  }

  constexpr auto dx = std::array{-1, 0, 1, -1, 0, 1, -1, 1};
  constexpr auto dy = std::array{-1, -1, -1, 1, 1, 1, 0, 0};

  int total  = 0;
  int sensor = 0;
  for ( LHCb::span<unsigned int> data : m_spBySensor ) {
    assert( !data.empty() ); // must have at least a header!

    auto spixels = data.subspan<1>(); // skip header word

    // encode header.
    data[0] = spixels.size(); // set correct number of super pixels

    // sort super pixels column major on each sensor
    std::sort( spixels.begin(), spixels.end(), SPLowerThan );

    // set 'no neighbour' hint flags
    std::array<bool, VP::NPixelsPerSensor> buffer = {false}; // buffer for checking super pixel neighbours
    for ( auto spw : spixels ) {
      assert( ( spw & 0xFFu ) != 0 );
      unsigned int idx = ( spw >> 8 );
      buffer[idx]      = true;
    }

    for ( auto& spw : spixels ) {
      const unsigned int idx          = ( spw >> 8 );
      const unsigned int row          = idx & 0x3FU;
      const unsigned int col          = ( idx >> 6 );
      unsigned int       no_neighbour = 1;
      for ( unsigned int ni = 0; ni < 8; ++ni ) {
        const int nrow = row + dy[ni];
        if ( nrow < 0 || nrow > 63 ) continue;
        const int ncol = col + dx[ni];
        if ( ncol < 0 || ncol > 383 ) continue;
        const unsigned int nidx = ( ncol << 6 ) | nrow;
        if ( buffer[nidx] ) {
          no_neighbour = 0;
          break;
        }
      }
      spw |= ( no_neighbour << 31 );
    }

    total += data.size_bytes();
    if ( msgLevel( MSG::DEBUG ) ) debug() << "evt " << m_evt << "sensor " << sensor << " sp " << data.size() << endmsg;

    rawEvent->addBank( sensor++, LHCb::RawBank::VP, c_bankVersion, data );
  }

  if ( msgLevel( MSG::DEBUG ) ) debug() << "total " << total << endmsg;

  return StatusCode::SUCCESS;
}
