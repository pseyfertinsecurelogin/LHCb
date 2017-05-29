#ifndef MUONRAWBUFFER_H
#define MUONRAWBUFFER_H 1

#include <string>
#include <utility>
#include <vector>

#include "DAQKernel/DecoderToolBase.h"
#include "Event/MuonODEData.h"
#include "Event/MuonPPEventInfo.h"
#include "Event/MuonTell1Header.h"
#include "Event/RawBankReadoutStatus.h"
#include "Event/RawEvent.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/extends.h"
#include "Kernel/MuonTileID.h"
#include "MuonDAQ/IMuonRawBuffer.h"            // Interface
#include "MuonDet/DeMuonDetector.h"
#include "MuonDet/MuonDAQHelper.h"

class DeMuonDetector;
class IIncidentListener;
class IInterface;
class IMuonRawBuffer;
class Incident;
namespace LHCb {
class RawBank;
class RawEvent;
}  // namespace LHCb

/** @class MuonRawBuffer MuonRawBuffer.h
 *
 *  @author Alessia Satta
 *  @date   2005-10-18
 */
class MuonRawBuffer :  public extends<Decoder::ToolBase, IMuonRawBuffer, IIncidentListener>
{
public:
  /// Standard constructor
  MuonRawBuffer( const std::string& type,
                 const std::string& name,
                 const IInterface* parent);

  StatusCode initialize() override;
  StatusCode finalize() override;

  StatusCode getTile(std::vector<LHCb::MuonTileID>& tile) override;
  StatusCode getTileAndTDC(std::vector<std::pair<LHCb::MuonTileID,unsigned int> > & tileAndTDC) override;
  StatusCode getPads(std::vector<LHCb::MuonTileID>& pads) override;

  StatusCode getNZSupp(std::vector<std::pair<LHCb::MuonTileID,unsigned int> > & tileAndTDC) override;


  // to get muon data belonging to one particular raw bank
private:
  StatusCode getTile(const LHCb::RawBank* r,std::vector<LHCb::MuonTileID>& tile) override;
  StatusCode getTileAndTDC(const LHCb::RawBank* r,std::vector<std::pair<LHCb::MuonTileID,unsigned int> > & tileAndTDC) override;
  StatusCode getPads(const LHCb::RawBank* r,std::vector<LHCb::MuonTileID>& pads) override;
public:
  StatusCode getTile( LHCb::RawEvent* raw,std::vector<LHCb::MuonTileID>& tile,const std::string& offset) override;
  StatusCode getTileAndTDC( LHCb::RawEvent* raw,std::vector<std::pair<LHCb::MuonTileID,unsigned int> > & tileAndTDC,const std::string& offset) override;
  StatusCode getPads( LHCb::RawEvent* raw,std::vector<LHCb::MuonTileID>& pads,const std::string& offset) override;
  StatusCode getNZSupp( LHCb::RawEvent* raw,std::vector<std::pair<LHCb::MuonTileID,unsigned int> > & tileAndTDC,const std::string& offset) override;
  StatusCode getNZSupp(const LHCb::RawBank* r,std::vector<std::pair<LHCb::MuonTileID,unsigned int> > & tileAndTDC) override;
  StatusCode dumpNZSupp(const LHCb::RawBank* r,unsigned int ode_num) override;
  MuonPPEventInfo getPPInfo(const LHCb::RawBank* r,unsigned int pp_num) override;
  MuonPPEventInfo getPPInfo(unsigned int tell1Number,unsigned int pp_num) override;
  //StatusCode getTDCInODE();

  StatusCode getPads( int tell1,std::vector<LHCb::MuonTileID>& pads) override;
  StatusCode getPads( int tell1) override;
  StatusCode getPadsInStation( int station,std::vector<std::vector<LHCb::MuonTileID>* > & pads) override;


  void forceReset() override
  {
    clearData();
  };
  MuonODEData getODEData(const LHCb::RawBank* r,unsigned int link) override;
  unsigned int BXCounter(unsigned int tell1Number) override;
  bool PPReachedHitLimit(unsigned int Tell1Number,int pp_num) override;
  bool LinkReachedHitLimit(unsigned int Tell1Number,int link_num) override;
private:
  MuonTell1Header getHeader(const LHCb::RawBank* r) override;
public:
  LHCb::RawBankReadoutStatus status() override {return m_status;}
  void putStatusOnTES() override;

  std::vector<std::pair<MuonTell1Header, unsigned int> > getHeaders(LHCb::RawEvent* raw) override;
  std::vector<std::pair<MuonTell1Header, unsigned int> > getHeaders() override;

private:

  StatusCode  decodeTileAndTDCDC06(const LHCb::RawBank* r);
  StatusCode  decodeTileAndTDCV1(const LHCb::RawBank* r);
  void clearData();
  void handle ( const Incident& incident ) override;
  StatusCode decodePadsDC06(const LHCb::RawBank* r);
  StatusCode decodePadsV1(const LHCb::RawBank* r);
  StatusCode decodeNZSupp(const LHCb::RawBank* r);
  StatusCode decodeNZSupp(int tell1Number);
  StatusCode DecodeDataPad(const LHCb::RawBank* r);

  StatusCode DecodeData(const LHCb::RawBank* r);
  StatusCode checkBankSize(const LHCb::RawBank* rawdata);
  StatusCode checkAllHeaders(LHCb::RawEvent* raw);
  void fillTell1Header(unsigned int tell1,unsigned int data);
  void setTESOffset(const std::string& offset="");
  void restoreTESOffset();
  void initStatus();	
  void initStatusNZS();	

  unsigned int m_NLink = 24;
  unsigned int m_ODEWord = 35;
  unsigned int m_M1Tell1 = 0;
  DeMuonDetector* m_muonDet = nullptr;
  mutable std::vector<std::pair<LHCb::MuonTileID, unsigned int> > m_storage[MuonDAQHelper_maxTell1Number];
  bool m_alreadyDecoded[MuonDAQHelper_maxTell1Number];
  mutable std::vector<LHCb::MuonTileID> m_padStorage[MuonDAQHelper_maxTell1Number];
  bool m_padAlreadyDecoded[MuonDAQHelper_maxTell1Number];
  bool m_ODEAlreadyDecoded[MuonDAQHelper_maxTell1Number];
  bool m_already_decoded_headerTell1[MuonDAQHelper_maxTell1Number];
  bool m_checkTell1HeaderPerformed = false;
  bool m_checkTell1HeaderResult = false;

  //mutable MuonTell1Header m_Tell1Header[[MuonDAQHelper_maxTell1Number];
  mutable MuonODEData m_ODEData[MuonDAQHelper_maxTell1Number*24];
  mutable MuonPPEventInfo m_PPEventInfo[MuonDAQHelper_maxTell1Number*4];

  mutable short m_eventHeader[MuonDAQHelper_maxTell1Number];
  unsigned int m_counter_invalid_hit[MuonDAQHelper_maxTell1Number];
  unsigned int m_processed_bank[MuonDAQHelper_maxTell1Number];
  unsigned int m_NZScounter_invalid_hit[MuonDAQHelper_maxTell1Number];
  unsigned int m_NZSprocessed_bank[MuonDAQHelper_maxTell1Number];
  unsigned int m_hitNumInLink[MuonDAQHelper_maxTell1Number*24];
  unsigned int m_hitNumInPP[MuonDAQHelper_maxTell1Number*4];
  unsigned int m_hit_checkSize[  MuonDAQHelper_maxTell1Number];
  unsigned int m_pad_checkSize[  MuonDAQHelper_maxTell1Number];
  unsigned int m_tell1_header_error[  MuonDAQHelper_maxTell1Number];
  unsigned int m_tell1_header_ORODE_error[  MuonDAQHelper_maxTell1Number];
  unsigned int m_tell1_header_SYNCH_data_error[  MuonDAQHelper_maxTell1Number];
  unsigned int m_tell1_header_SYNCH_BC_error[  MuonDAQHelper_maxTell1Number];
  unsigned int m_tell1_header_SYNCH_Evt_error[  MuonDAQHelper_maxTell1Number];
  std::string m_offsetForTES;
  bool m_TESChanged = false;
  std::string m_storeOriginalValue;

  LHCb::RawBankReadoutStatus m_status;
  LHCb::RawBankReadoutStatus m_statusFull;
  bool m_statusCreated = false;


};
#endif // MUONRAWBUFFER_H
