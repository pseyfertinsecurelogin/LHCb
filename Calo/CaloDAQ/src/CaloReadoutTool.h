#ifndef CALODAQ_CALOREADOUTTOOL_H
#define CALODAQ_CALOREADOUTTOOL_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"
// from LHCb
#include "CaloDAQ/ICaloReadoutTool.h"
#include "DAQKernel/DecoderToolBase.h"





/** @class CaloReadoutTool CaloReadoutTool.h CaloDAQ/CaloReadoutTool.h
 *
 * base class for Calo readout tools :
 * (CaloEnergyFromRaw, CaloTriggerAdcsFromRaw and CaloTriggerBitsFromRaw)
 *
 *  @author Olivier Deschamps
 *  @date   2007-02-01
 */
class CaloReadoutTool
  : public Decoder::ToolBase
    , virtual public ICaloReadoutTool
    , virtual public IIncidentListener{
public:

  /// Standard constructor
  CaloReadoutTool( const std::string& type,
               const std::string& name,
               const IInterface* parent);

  StatusCode initialize() override;
  StatusCode finalize() override;
  std::string _rootInTES() override { return rootInTES(); }
  StatusCode  _setProperty(const std::string& p,const std::string& v) override {return  setProperty(p,v);}

  // Useful methods  to set/get m_banks externally
  // e.g. : avoid the call to getCaloBanksFromRaw() at each call of adc(bank)
  bool getBanks() override {
    if(m_stat)counter("getCaloBanks") += 1;
    m_getRaw = false;
    clear();
    m_ok = getCaloBanksFromRaw();
    return m_ok;
  }
  void setBanks(const std::vector<LHCb::RawBank*>* bank ) override {
    m_getRaw = false;
    clear();
    m_banks = bank;
    m_ok = true;
  }
  //actual implementation MUST BE in the parent tool
  void clear() override {Warning("DUMMY CLEARING : THIS MESSAGE MUST NOT APPEAR").ignore() ; return;}
  //
  void cleanData(int) override {return; } // to be implemented in the parent tool
  LHCb::RawBankReadoutStatus& status() override {return m_status;}
  void putStatusOnTES() override;
  bool ok() override {
    if(m_getRaw)getBanks() ;
    return m_ok;
  };
  DeCalorimeter* deCalo() override {return m_calo;}


  // =========================================================================
  /// Inform that a new incident has occurred
  void handle(const Incident& /* inc */ ) override {
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << "IIncident Svc reset" << endmsg;
    m_getRaw = true ;
  }
  // =========================================================================

protected:

  bool getCaloBanksFromRaw();
  int findCardbyCode(std::vector<int> feCards, int code ) const;
  bool checkCards(int nCards, std::vector<int> feCards ) const;
  void checkCtrl(int word, int sourceID);
  bool checkSrc(int source);

  std::string  m_detectorName;
  bool m_packedIsDefault;
  const std::vector<LHCb::RawBank*>* m_banks;
  DeCalorimeter*   m_calo;
  LHCb::RawBank::BankType m_packedType;
  LHCb::RawBank::BankType m_shortType;
  LHCb::RawBank::BankType m_errorType;
  bool m_getRaw;


  bool m_extraHeader;
  bool m_packed;
  bool m_cleanCorrupted;
  LHCb::RawBankReadoutStatus m_status;
  std::vector<int> m_readSources;
  bool m_ok;
private:
  bool m_first;
  bool m_stat;
};
#endif // CALODAQ_CALOREADOUTTOOL_H
