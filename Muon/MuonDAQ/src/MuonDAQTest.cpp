/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
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
#include "MuonDAQTest.h"
#include "MuonDAQ/IMuonRawBuffer.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MuonDAQTest
//
// 2005-10-19 : Alessia Satta
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( MuonDAQTest )

//=============================================================================
// Initialization
//=============================================================================
StatusCode MuonDAQTest::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Initialize" << endmsg;
  m_MuonBuffer=tool<IMuonRawBuffer>("MuonRawBuffer");
  if(!m_MuonBuffer) return Error("Could not instantiate MuonRawBuffer tool");
  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode MuonDAQTest::execute() {

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute" << endmsg;

  SmartDataPtr<LHCb::MuonDigits> digit(eventSvc(),
                                 LHCb::MuonDigitLocation::MuonDigit);

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) {
    for(const auto& idigit : *digit) {
      LHCb::MuonTileID digitTile=idigit->key();
        debug()<< "["  <<  digitTile.layout() << ","
               <<  digitTile.station() << ","
               <<  digitTile.region() << ","
               <<  digitTile.quarter() << ","
               <<  digitTile.nX() << ","
               <<  digitTile.nY() << "]" <<"time "<<idigit->TimeStamp()
               << endmsg;
    }
  }
  std::vector<LHCb::MuonTileID> decodingTile;
  m_MuonBuffer->getTile(decodingTile);
  for(auto jitile=decodingTile.begin();jitile<decodingTile.end();jitile++){
    LHCb::MuonTileID digitTile=(*jitile);
    if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
      verbose()<<" ;ist of tile "<< (unsigned int) digitTile<<endmsg;
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug()<< "["  <<  digitTile.layout() << ","
             <<  digitTile.station() << ","
             <<  digitTile.region() << ","
             <<  digitTile.quarter() << ","
             <<  digitTile.nX() << ","
             <<  digitTile.nY() << "]" <<endmsg;
  }

  std::vector<std::pair<LHCb::MuonTileID,unsigned int> > decoding;
  m_MuonBuffer->getTileAndTDC(decoding);
  for(auto ji=decoding.begin();ji<decoding.end();ji++){
    std::pair<LHCb::MuonTileID,unsigned int> digit=(*ji);
    LHCb::MuonTileID digitTile=digit.first;
    unsigned int time = digit.second;
    //info()<<" alesia "<<(unsigned int) digitTile<<endmsg;
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug()<< "["  <<  digitTile.layout() << ","
             <<  digitTile.station() << ","
             <<  digitTile.region() << ","
             <<  digitTile.quarter() << ","
             <<  digitTile.nX() << ","
             <<  digitTile.nY() << "]" <<" time "<<time<<endmsg;
  }
  for(auto idigit=digit->begin();idigit<digit->end();idigit++){
    LHCb::MuonTileID digitTile=(*idigit)->key();
    bool found=false;
    for(auto ji=decoding.begin();ji<decoding.end()&&!found;ji++){
      std::pair<LHCb::MuonTileID,unsigned int> digitPair=(*ji);
      LHCb::MuonTileID digitTileDecoded=digitPair.first;
      unsigned int time = digitPair.second;
      if(digitTile==digitTileDecoded){
        if(time!=(*idigit)->TimeStamp()){
      err()<<"time not correct "<<
          time<<" "<<(*idigit)->TimeStamp()<<endmsg;
        }else{ found=true;}
    }

    }
    if(!found)info()<<" not found the digit "<<endmsg;

  }
  return StatusCode::SUCCESS;
}
//=============================================================================
