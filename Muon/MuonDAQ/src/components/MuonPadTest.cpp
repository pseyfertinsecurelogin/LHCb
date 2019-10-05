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
#include "MuonPadTest.h"
#include "Event/MuonCoord.h"
#include "MuonDAQ/IMuonRawBuffer.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MuonPadTest
//
// 2006-03-19 : Alessia Satta
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( MuonPadTest )

//=============================================================================
// Initialization
//=============================================================================
StatusCode MuonPadTest::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Initialize" << endmsg;
  m_MuonBuffer = tool<IMuonRawBuffer>( "MuonRawBuffer" );
  if ( !m_MuonBuffer ) return Error( "Could not instantiate MuonRawBuffer tool" );
  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode MuonPadTest::execute() {

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Execute" << endmsg;

  std::vector<LHCb::MuonTileID> decodingTile;
  m_MuonBuffer->getPads( decodingTile ).ignore();
  SmartDataPtr<LHCb::MuonCoords> coord( eventSvc(), LHCb::MuonCoordLocation::MuonCoords );
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) {
    debug() << " coord " << coord->size() << endmsg;
    debug() << " pad " << decodingTile.size() << endmsg;
  }
  for ( auto icoord = coord->begin(); icoord < coord->end(); icoord++ ) {
    LHCb::MuonTileID tileCoord = ( *icoord )->key();
    if ( tileCoord.station() > 0 ) {
      if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
        debug() << "[" << tileCoord.layout() << "," << tileCoord.station() << "," << tileCoord.region() << ","
                << tileCoord.quarter() << "," << tileCoord.nX() << "," << tileCoord.nY() << "]" << endmsg;

      bool found = false;
      for ( auto ipad = decodingTile.begin(); ipad < decodingTile.end(); ipad++ ) {
        if ( *ipad == tileCoord ) {
          if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
            debug() << " found the matching coord "
                    << "[" << ipad->layout() << "," << ipad->station() << "," << ipad->region() << ","
                    << ipad->quarter() << "," << ipad->nX() << "," << ipad->nY() << "]" << endmsg;
          found = true;
          break;
        }
      }
      if ( !found ) {
        if ( ( tileCoord.station() == 1 || tileCoord.station() == 2 ) &&
             ( tileCoord.region() == 0 || tileCoord.region() == 1 ) && ( ( *icoord )->uncrossed() ) )
          continue;
        info() << " not found the tile " << tileCoord.station() << " " << tileCoord.region() << " "
               << ( *icoord )->uncrossed() << endmsg;
      }
    }
  }

  return StatusCode::SUCCESS;
}
//=============================================================================
