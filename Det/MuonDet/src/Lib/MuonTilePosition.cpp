/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "MuonDet/MuonTilePosition.h"

namespace MuonTilePositionEC {

  enum class ErrorCode : StatusCode::code_t { DET_TILE2XYZ_FAILED = 10 };
  struct ErrorCategory : StatusCode::Category {
    const char* name() const override { return "MuonTilePosition"; }
    bool        isRecoverable( StatusCode::code_t ) const override { return false; }
    std::string message( StatusCode::code_t code ) const override {
      switch ( static_cast<MuonTilePositionEC::ErrorCode>( code ) ) {
      case ErrorCode::DET_TILE2XYZ_FAILED:
        return "DeMuonDetector could not convert tileID to X,Y,Z";
      default:
        return StatusCode::default_category().message( code );
      }
    }
  };
} // namespace MuonTilePositionEC
STATUSCODE_ENUM_DECL( MuonTilePositionEC::ErrorCode )
STATUSCODE_ENUM_IMPL( MuonTilePositionEC::ErrorCode, MuonTilePositionEC::ErrorCategory )

namespace {
  [[gnu::noreturn]] void throw_exception( MuonTilePositionEC::ErrorCode ec, const char* tag ) {
    auto sc = StatusCode( ec );
    throw GaudiException{sc.message(), tag, std::move( sc )};
  }
#define OOPS( x ) throw_exception( x, __PRETTY_FUNCTION__ )

  constexpr auto c_padGridX = std::array<unsigned int, 5>{24, 48, 48, 12, 12};
  constexpr auto c_padGridY = std::array<unsigned int, 5>{8, 8, 8, 8, 8};
  constexpr auto c_stripXGridX =
      std::array<unsigned int, 20>{24, 24, 24, 24, 48, 48, 48, 48, 48, 48, 48, 48, 12, 12, 12, 12, 12, 12, 12, 12};
  constexpr auto c_stripXGridY =
      std::array<unsigned int, 20>{8, 8, 8, 8, 1, 2, 2, 2, 1, 2, 2, 2, 8, 2, 2, 2, 8, 2, 2, 2};
  constexpr auto c_stripYGridX =
      std::array<unsigned int, 20>{24, 24, 24, 24, 8, 4, 2, 2, 8, 4, 2, 2, 12, 4, 2, 2, 12, 4, 2, 2};
  constexpr auto c_stripYGridY =
      std::array<unsigned int, 20>{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};
} // namespace

namespace LHCb::Muon {

  ComputeTilePosition::ComputeTilePosition( const DeMuonDetector& det ) {

    auto           stationNumber = det.stations();
    constexpr auto regionNumber  = 4;
    assert( stationNumber == 4 || stationNumber == 5 );
    auto N = ( stationNumber == 4 ? 16 : 20 );

    m_padSizeX.resize( N );
    m_padSizeY.resize( N );
    m_stripXSizeX.resize( N );
    m_stripXSizeY.resize( N );
    m_stripYSizeX.resize( N );
    m_stripYSizeY.resize( N );
    m_padGridX    = make_span( c_padGridX ).last( stationNumber );
    m_padGridY    = make_span( c_padGridY ).last( stationNumber );
    m_stripXGridX = make_span( c_stripXGridX ).last( N );
    m_stripXGridY = make_span( c_stripXGridY ).last( N );
    m_stripYGridX = make_span( c_stripYGridX ).last( N );
    m_stripYGridY = make_span( c_stripYGridY ).last( N );

    for ( int i = 0; i < stationNumber; i++ ) {
      int channels = 48 * ( m_padGridX[i] * m_padGridY[i] );
      m_posPad[i].resize( channels );
      channels = 12 * std::inner_product( std::next( m_stripXGridX.begin(), 4 * i ),
                                          std::next( m_stripXGridX.begin(), 4 * i + 4 ),
                                          std::next( m_stripXGridY.begin(), 4 * i ), 0 );
      m_posStripX[i].resize( channels );
      channels = 12 * std::inner_product( std::next( m_stripYGridX.begin(), 4 * i ),
                                          std::next( m_stripYGridX.begin(), 4 * i + 4 ),
                                          std::next( m_stripYGridY.begin(), 4 * i ), 0 );
      m_posStripY[i].resize( channels );
    }

    // fill pad vectors
    LHCb::MuonTileID tile;

    for ( int station = 0; station < stationNumber; station++ ) {

      tile.setStation( station );
      tile.setLayout( MuonLayout( m_padGridX[station], m_padGridY[station] ) );
      int index = 0;

      for ( int region = 0; region < regionNumber; region++ ) {
        tile.setRegion( region );
        const auto s4r = station * 4 + region;
        for ( int quarter = 0; quarter < 4; quarter++ ) {
          tile.setQuarter( quarter );
          for ( unsigned y = 0; y < m_padGridY[station]; y++ ) {
            tile.setY( y );
            for ( unsigned x = m_padGridX[station]; x < 2 * m_padGridX[station]; x++ ) {
              tile.setX( x );
              auto pos = det.position( tile );
              if ( !pos ) OOPS( MuonTilePositionEC::ErrorCode::DET_TILE2XYZ_FAILED );
              assert( index < static_cast<int>( m_posPad[station].size() ) );
              m_posPad[station][index++] = pos->position;
              m_padSizeX[s4r]            = pos->dX;
              m_padSizeY[s4r]            = pos->dY;
            }
          }

          for ( unsigned y = m_padGridY[station]; y < 2 * m_padGridY[station]; y++ ) {
            tile.setY( y );
            for ( unsigned x = 0; x < 2 * m_padGridX[station]; x++ ) {
              tile.setX( x );
              auto pos = det.position( tile );
              if ( !pos ) OOPS( MuonTilePositionEC::ErrorCode::DET_TILE2XYZ_FAILED );
              assert( index < static_cast<int>( m_posPad[station].size() ) );
              m_posPad[station][index++] = pos->position;
            }
          }
        }
      }

      for ( int station = 0; station < stationNumber; station++ ) {
        tile.setStation( station );
        int index = 0;
        for ( int region = 0; region < regionNumber; region++ ) {
          const auto s4r      = station * 4 + region;
          m_stripXOffset[s4r] = index;
          tile.setLayout( MuonLayout( m_stripXGridX[s4r], m_stripXGridY[s4r] ) );
          tile.setRegion( region );
          for ( int quarter = 0; quarter < 4; quarter++ ) {
            tile.setQuarter( quarter );
            for ( unsigned y = 0; y < m_stripXGridY[s4r]; y++ ) {
              tile.setY( y );
              for ( unsigned x = m_stripXGridX[s4r]; x < 2 * m_stripXGridX[s4r]; x++ ) {
                tile.setX( x );
                auto pos = det.position( tile );
                if ( !pos ) OOPS( MuonTilePositionEC::ErrorCode::DET_TILE2XYZ_FAILED );
                m_posStripX[station][index++] = pos->position;
                if ( pos->dX > m_stripXSizeX[s4r] ) m_stripXSizeX[s4r] = pos->dX;
                if ( pos->dY > m_stripXSizeY[s4r] ) m_stripXSizeY[s4r] = pos->dY;
              }
            }
            for ( unsigned y = m_stripXGridY[s4r]; y < 2 * m_stripXGridY[s4r]; y++ ) {
              tile.setY( y );
              for ( unsigned x = 0; x < 2 * m_stripXGridX[s4r]; x++ ) {
                tile.setX( x );
                auto pos = det.position( tile );
                if ( !pos ) OOPS( MuonTilePositionEC::ErrorCode::DET_TILE2XYZ_FAILED );
                m_posStripX[station][index++] = pos->position;
              }
            }
          }
        }
      }
      // stripY

      for ( int station = 0; station < stationNumber; station++ ) {
        tile.setStation( station );
        int index = 0;
        for ( int region = 0; region < regionNumber; region++ ) {
          const auto s4r      = station * 4 + region;
          m_stripYOffset[s4r] = index;
          tile.setLayout( MuonLayout( m_stripYGridX[s4r], m_stripYGridY[s4r] ) );
          tile.setRegion( region );
          for ( int quarter = 0; quarter < 4; quarter++ ) {
            tile.setQuarter( quarter );
            for ( unsigned y = 0; y < m_stripYGridY[s4r]; y++ ) {
              tile.setY( y );
              for ( unsigned x = m_stripYGridX[s4r]; x < 2 * m_stripYGridX[s4r]; x++ ) {
                tile.setX( x );

                auto pos = det.position( tile );
                if ( !pos ) OOPS( MuonTilePositionEC::ErrorCode::DET_TILE2XYZ_FAILED );
                m_posStripY[station][index++] = pos->position;
                if ( pos->dX > m_stripYSizeX[s4r] ) m_stripYSizeX[s4r] = pos->dX;
                if ( pos->dY > m_stripYSizeY[s4r] ) m_stripYSizeY[s4r] = pos->dY;
              }
            }
            for ( unsigned y = m_stripYGridY[s4r]; y < 2 * m_stripYGridY[s4r]; y++ ) {
              tile.setY( y );
              for ( unsigned x = 0; x < 2 * m_stripYGridX[s4r]; x++ ) {
                tile.setX( x );
                auto pos = det.position( tile );
                if ( !pos ) OOPS( MuonTilePositionEC::ErrorCode::DET_TILE2XYZ_FAILED );
                m_posStripY[station][index++] = pos->position;
              }
            }
          }
        }
      }
    }
  }
} // namespace LHCb::Muon
