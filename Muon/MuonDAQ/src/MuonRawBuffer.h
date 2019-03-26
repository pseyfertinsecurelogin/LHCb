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
#ifndef MUONRAWBUFFER_H
#define MUONRAWBUFFER_H 1

// Include files
// from Gaudi
#include "DAQKernel/DecoderToolBase.h"
#include "Event/MuonODEData.h"
#include "Event/MuonPPEventInfo.h"
#include "Event/RawEvent.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IIncidentListener.h"
#include "MuonDAQ/IMuonRawBuffer.h" // Interface
#include "MuonDet/DeMuonDetector.h"

#include <bitset>
#include <string>

/** @class MuonRawBuffer MuonRawBuffer.h
 *
 *  @author Alessia Satta
 *  @date   2005-10-18
 */
class MuonRawBuffer : public extends<Decoder::ToolBase, IMuonRawBuffer, IIncidentListener> {
public:
  /// Standard constructor
  MuonRawBuffer( const std::string& type, const std::string& name, const IInterface* parent );

  StatusCode initialize() override;
  StatusCode finalize() override;

  StatusCode getTile( const LHCb::RawEvent* raw, std::vector<LHCb::MuonTileID>& tile,
                      const std::string& offset ) override;
  StatusCode getTile( std::vector<LHCb::MuonTileID>& tile ) override;
  StatusCode getTileAndTDC( const LHCb::RawEvent*                                   raw,
                            std::vector<std::pair<LHCb::MuonTileID, unsigned int>>& tileAndTDC,
                            const std::string&                                      offset ) override;
  StatusCode getTileAndTDC( std::vector<std::pair<LHCb::MuonTileID, unsigned int>>& tileAndTDC ) override;
  StatusCode getPads( std::vector<LHCb::MuonTileID>& pads ) override;
  StatusCode getNZSupp( const LHCb::RawEvent* raw, std::vector<std::pair<LHCb::MuonTileID, unsigned int>>& tileAndTDC,
                        const std::string& offset ) override;
  StatusCode getNZSupp( std::vector<std::pair<LHCb::MuonTileID, unsigned int>>& tileAndTDC ) override;

  // to get muon data belonging to one particular raw bank
  StatusCode      getPads( const LHCb::RawEvent* raw, std::vector<LHCb::MuonTileID>& pads,
                           const std::string& offset ) override;
  StatusCode      getNZSupp( const LHCb::RawBank*                                    r,
                             std::vector<std::pair<LHCb::MuonTileID, unsigned int>>& tileAndTDC ) override;
  StatusCode      dumpNZSupp( const LHCb::RawBank* r, unsigned int ode_num ) override;
  MuonPPEventInfo getPPInfo( const LHCb::RawBank* r, unsigned int pp_num ) override;
  MuonPPEventInfo getPPInfo( unsigned int tell1Number, unsigned int pp_num ) override;

  StatusCode getPads( int tell1, std::vector<LHCb::MuonTileID>& pads ) override;
  StatusCode getPads( int tell1 ) override;
  StatusCode getPadsInStation( int station, std::vector<std::vector<LHCb::MuonTileID>*>& pads ) override;

  void         forceReset() override { clearData(); };
  MuonODEData  getODEData( const LHCb::RawBank* r, unsigned int link ) override;
  unsigned int BXCounter( unsigned int tell1Number ) override;
  bool         PPReachedHitLimit( unsigned int Tell1Number, int pp_num ) override;
  bool         LinkReachedHitLimit( unsigned int Tell1Number, int link_num ) override;

  LHCb::RawBankReadoutStatus status() override { return m_status; }
  void                       putStatusOnTES() override;

  std::vector<std::pair<MuonTell1Header, unsigned int>> getHeaders( LHCb::RawEvent* raw ) override;
  std::vector<std::pair<MuonTell1Header, unsigned int>> getHeaders() override;

private:
  StatusCode      getTile( const LHCb::RawBank* r, std::vector<LHCb::MuonTileID>& tile );
  StatusCode      getTileAndTDC( const LHCb::RawBank*                                    r,
                                 std::vector<std::pair<LHCb::MuonTileID, unsigned int>>& tileAndTDC );
  StatusCode      getPads( const LHCb::RawBank* r, std::vector<LHCb::MuonTileID>& pads );
  MuonTell1Header getHeader( const LHCb::RawBank* r );

  StatusCode decodeTileAndTDCDC06( const LHCb::RawBank* r );
  StatusCode decodeTileAndTDCV1( const LHCb::RawBank* r );
  void       clearData();
  void       handle( const Incident& incident ) override;
  StatusCode decodePadsDC06( const LHCb::RawBank* r );
  StatusCode decodePadsV1( const LHCb::RawBank* r );
  StatusCode decodeNZSupp( const LHCb::RawBank* r );
  StatusCode decodeNZSupp( int tell1Number );
  StatusCode DecodeDataPad( const LHCb::RawBank* r );

  StatusCode DecodeData( const LHCb::RawBank* r );
  StatusCode checkBankSize( const LHCb::RawBank* rawdata );
  StatusCode checkAllHeaders( const LHCb::RawEvent* raw );
  void       fillTell1Header( unsigned int tell1, unsigned int data );
  void       initStatus();
  void       initStatusNZS();

  unsigned int                                                   m_M1Tell1 = 0;
  DeMuonDetector*                                                m_muonDet = nullptr;
  mutable std::vector<std::pair<LHCb::MuonTileID, unsigned int>> m_storage[MuonDAQHelper_maxTell1Number];
  std::bitset<MuonDAQHelper_maxTell1Number>                      m_alreadyDecoded;
  mutable std::vector<LHCb::MuonTileID>                          m_padStorage[MuonDAQHelper_maxTell1Number];
  std::bitset<MuonDAQHelper_maxTell1Number>                      m_padAlreadyDecoded;
  std::bitset<MuonDAQHelper_maxTell1Number>                      m_ODEAlreadyDecoded;
  std::bitset<MuonDAQHelper_maxTell1Number>                      m_already_decoded_headerTell1;
  bool                                                           m_checkTell1HeaderPerformed = false;
  bool                                                           m_checkTell1HeaderResult    = false;

  mutable MuonODEData     m_ODEData[MuonDAQHelper_maxTell1Number * 24];
  mutable MuonPPEventInfo m_PPEventInfo[MuonDAQHelper_maxTell1Number * 4];

  mutable std::array<short, MuonDAQHelper_maxTell1Number>     m_eventHeader;
  std::array<unsigned int, MuonDAQHelper_maxTell1Number>      m_counter_invalid_hit;
  std::array<unsigned int, MuonDAQHelper_maxTell1Number>      m_processed_bank;
  std::array<unsigned int, MuonDAQHelper_maxTell1Number>      m_NZScounter_invalid_hit;
  std::array<unsigned int, MuonDAQHelper_maxTell1Number>      m_NZSprocessed_bank;
  std::array<unsigned int, MuonDAQHelper_maxTell1Number * 24> m_hitNumInLink;
  unsigned int                                                m_hitNumInPP[MuonDAQHelper_maxTell1Number * 4];
  std::array<unsigned int, MuonDAQHelper_maxTell1Number>      m_hit_checkSize;
  std::array<unsigned int, MuonDAQHelper_maxTell1Number>      m_pad_checkSize;
  std::array<unsigned int, MuonDAQHelper_maxTell1Number>      m_tell1_header_error;
  std::array<unsigned int, MuonDAQHelper_maxTell1Number>      m_tell1_header_ORODE_error;
  std::array<unsigned int, MuonDAQHelper_maxTell1Number>      m_tell1_header_SYNCH_data_error;
  std::array<unsigned int, MuonDAQHelper_maxTell1Number>      m_tell1_header_SYNCH_BC_error;
  std::array<unsigned int, MuonDAQHelper_maxTell1Number>      m_tell1_header_SYNCH_Evt_error;
  int                                                         m_TESChanged = 0;

  LHCb::RawBankReadoutStatus m_status;
  LHCb::RawBankReadoutStatus m_statusFull;
  bool                       m_statusCreated = false;
};
#endif // MUONRAWBUFFER_H
