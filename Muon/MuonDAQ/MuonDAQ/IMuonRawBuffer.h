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
#ifndef MUONDAQ_IMUONRAWBUFFER_H
#define MUONDAQ_IMUONRAWBUFFER_H 1

// Include files
#include "Kernel/MuonTileID.h"

// GaudiKernel
#include "Event/MuonODEData.h"
#include "Event/MuonPPEventInfo.h"
#include "Event/MuonTell1Header.h"
#include "Event/RawBankReadoutStatus.h"
#include "Event/RawEvent.h"
#include "GaudiKernel/IAlgTool.h"
#include <utility>
#include <vector>

/** @class IMuonRawBuffer IMuonRawBuffer.h MuonTools/IMuonRawBuffer.h
 *
 *  Interface for the tools to convert MuonTileID to coordinates
 *
 *  @author David Hutchcroft
 *  @date   11/03/2002
 */

struct IMuonRawBuffer : extend_interfaces<IAlgTool> {

  /** static interface identification
   *  @return unique interface identifier
   */
  DeclareInterfaceID( IMuonRawBuffer, 3, 0 );
  // to get all Muon data in one shot
  virtual StatusCode getTile( std::vector<LHCb::MuonTileID>& tile )                                      = 0;
  virtual StatusCode getTileAndTDC( std::vector<std::pair<LHCb::MuonTileID, unsigned int>>& tileAndTDC ) = 0;
  virtual StatusCode getPads( std::vector<LHCb::MuonTileID>& pads )                                      = 0;
  virtual StatusCode getNZSupp( std::vector<std::pair<LHCb::MuonTileID, unsigned int>>& tileAndTDC )     = 0;

  virtual LHCb::RawBankReadoutStatus status()         = 0;
  virtual void                       putStatusOnTES() = 0;

  virtual std::vector<std::pair<MuonTell1Header, unsigned int>> getHeaders( LHCb::RawEvent* raw ) = 0;
  virtual std::vector<std::pair<MuonTell1Header, unsigned int>> getHeaders()                      = 0;

  virtual StatusCode      getNZSupp( const LHCb::RawBank*                                    r,
                                     std::vector<std::pair<LHCb::MuonTileID, unsigned int>>& tileAndTDC ) = 0;
  virtual StatusCode      dumpNZSupp( const LHCb::RawBank* r, unsigned int ode_num )                      = 0;
  virtual MuonPPEventInfo getPPInfo( const LHCb::RawBank* r, unsigned int pp_num )                        = 0;
  virtual MuonPPEventInfo getPPInfo( unsigned int tell1Number, unsigned int pp_num )                      = 0;
  virtual StatusCode      getPads( const LHCb::RawEvent* raw, std::vector<LHCb::MuonTileID>& pads,
                                   const std::string& offset )                                            = 0;

  virtual StatusCode getTile( const LHCb::RawEvent* raw, std::vector<LHCb::MuonTileID>& tile,
                              const std::string& offset )                                               = 0;
  virtual StatusCode getTileAndTDC( const LHCb::RawEvent*                                   raw,
                                    std::vector<std::pair<LHCb::MuonTileID, unsigned int>>& tileAndTDC,
                                    const std::string&                                      offset )                                         = 0;
  virtual StatusCode getNZSupp( const LHCb::RawEvent*                                   raw,
                                std::vector<std::pair<LHCb::MuonTileID, unsigned int>>& tileAndTDC,
                                const std::string&                                      offset )                                             = 0;
  virtual StatusCode getPads( int tell1, std::vector<LHCb::MuonTileID>& pads )                          = 0;
  virtual StatusCode getPads( int tell1 )                                                               = 0;
  virtual StatusCode getPadsInStation( int station, std::vector<std::vector<LHCb::MuonTileID>*>& pads ) = 0;

  virtual void         forceReset()                                            = 0;
  virtual MuonODEData  getODEData( const LHCb::RawBank* r, unsigned int link ) = 0;
  virtual unsigned int BXCounter( unsigned int tell1Number )                   = 0;

  virtual bool PPReachedHitLimit( unsigned int Tell1Number, int pp_num )     = 0;
  virtual bool LinkReachedHitLimit( unsigned int Tell1Number, int link_num ) = 0;
};
#endif // MUONDAQ_IMUONRAWBUFFER_H
