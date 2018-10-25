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
#ifndef CALOFUTUREDAQ_CALOFUTUREREADOUTTOOL_H
#define CALOFUTUREDAQ_CALOFUTUREREADOUTTOOL_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"
// from LHCb
#include "CaloFutureDAQ/ICaloFutureReadoutTool.h"
#include "DAQKernel/DecoderToolBase.h"

#include "futuredetails.h"




/** @class CaloFutureReadoutTool CaloFutureReadoutTool.h CaloFutureDAQ/CaloFutureReadoutTool.h
 *
 * base class for CaloFuture readout tools :
 * (CaloFutureEnergyFromRaw, CaloFutureTriggerAdcsFromRaw and CaloFutureTriggerBitsFromRaw)
 *
 *  @author Olivier Deschamps
 *  @date   2007-02-01
 */
class CaloFutureReadoutTool
  : public extends<Decoder::ToolBase, ICaloFutureReadoutTool, IIncidentListener> {
public:

  /// Standard constructor
  CaloFutureReadoutTool( const std::string& type,
               const std::string& name,
               const IInterface* parent);

  StatusCode initialize() override;
  StatusCode finalize() override;
  StatusCode  _setProperty(const std::string& p,const std::string& v) override {return  setProperty(p,v);}

  // Useful methods  to set/get m_banks externally
  // e.g. : avoid the call to getCaloFutureBanksFromRaw() at each call of adc(bank)
  bool getBanks() override {
    if(m_stat)counter("getCaloFutureBanks") += 1;
    m_getRaw = false;
    clear();
    m_ok = getCaloFutureBanksFromRaw();
    return m_ok;
  }
  void setBanks(LHCb::span<const LHCb::RawBank*> bank) override {
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
  DeCalorimeter* deCaloFuture() override {return m_calo;}


  // =========================================================================
  /// Inform that a new incident has occurred
  void handle(const Incident& /* inc */ ) override {
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << "IIncident Svc reset" << endmsg;
    m_getRaw = true ;
  }
  // =========================================================================

protected:

  bool getCaloFutureBanksFromRaw();
  int findCardbyCode(const std::vector<int>& feCards, int code ) const;
  bool checkCards(int nCards, const std::vector<int>& feCards ) const;
  void checkCtrl(int word, int sourceID);
  bool checkSrc(int source);

  Gaudi::Property<futuredetails::DetectorName_t> m_detectorName{ this, "DetectorName", futuredetails::DetectorName_t::Unknown };
  Gaudi::Property<bool> m_packedIsDefault { this, "PackedIsDefault",false };
  LHCb::span<const LHCb::RawBank*> m_banks ;
  DeCalorimeter*   m_calo = nullptr;
  LHCb::RawBank::BankType m_packedType;
  LHCb::RawBank::BankType m_shortType;
  LHCb::RawBank::BankType m_errorType;
  bool m_getRaw = true;


  Gaudi::Property<bool> m_extraHeader { this, "DetectorSpecificHeader", false };
  bool m_packed = false;
  Gaudi::Property<bool> m_cleanCorrupted {this, "CleanWhenCorruption", false };
  LHCb::RawBankReadoutStatus m_status;
  std::vector<int> m_readSources;
  bool m_ok = false;
private:
  bool m_first = true;
  Gaudi::Property<bool> m_stat { this, "PrintStat", false };
};
#endif // CALOFUTUREDAQ_CALOFUTUREREADOUTTOOL_H
