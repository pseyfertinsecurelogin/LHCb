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

#include "details.h"

/** @class CaloReadoutTool CaloReadoutTool.h CaloDAQ/CaloReadoutTool.h
 *
 * base class for Calo readout tools :
 * (CaloEnergyFromRaw, CaloTriggerAdcsFromRaw and CaloTriggerBitsFromRaw)
 *
 *  @author Olivier Deschamps
 *  @date   2007-02-01
 */
class CaloReadoutTool : public extends<Decoder::ToolBase, ICaloReadoutTool, IIncidentListener> {
public:
  /// Standard constructor
  CaloReadoutTool( const std::string& type, const std::string& name, const IInterface* parent );

  StatusCode initialize() override;
  StatusCode finalize() override;
  StatusCode _setProperty( const std::string& p, const std::string& v ) override { return setProperty( p, v ); }

  // Useful methods  to set/get m_banks externally
  // e.g. : avoid the call to getCaloBanksFromRaw() at each call of adc(bank)
  bool getBanks() override {
    if ( m_stat ) counter( "getCaloBanks" ) += 1;
    m_getRaw = false;
    clear();
    m_ok = getCaloBanksFromRaw();
    return m_ok;
  }
  void setBanks( LHCb::span<const LHCb::RawBank*> bank ) override {
    m_getRaw = false;
    clear();
    m_banks = bank;
    m_ok    = true;
  }
  // actual implementation MUST BE in the parent tool
  void clear() override { Warning( "DUMMY CLEARING : THIS MESSAGE MUST NOT APPEAR" ).ignore(); }
  //
  void                        cleanData( int ) override { return; } // to be implemented in the parent tool
  LHCb::RawBankReadoutStatus& status() override { return *m_status; }
  void                        putStatusOnTES() override;
  bool                        ok() override {
    if ( m_getRaw ) getBanks();
    return m_ok;
  };
  DeCalorimeter* deCalo() override { return m_calo; }

  // =========================================================================
  /// Inform that a new incident has occurred
  void handle( const Incident& /* inc */ ) override {
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "IIncident Svc reset" << endmsg;
    m_getRaw = true;
    m_status.reset();
  }
  // =========================================================================

protected:
  bool getCaloBanksFromRaw();
  int  findCardbyCode( const std::vector<int>& feCards, int code ) const;
  bool checkCards( int nCards, const std::vector<int>& feCards ) const;
  void checkCtrl( int word, int sourceID );
  bool checkSrc( int source );

  Gaudi::Property<details::DetectorName_t> m_detectorName{this, "DetectorName", details::DetectorName_t::Unknown};
  Gaudi::Property<bool>                    m_packedIsDefault{this, "PackedIsDefault", false};
  LHCb::span<const LHCb::RawBank*>         m_banks;
  DeCalorimeter*                           m_calo = nullptr;
  LHCb::RawBank::BankType                  m_packedType;
  LHCb::RawBank::BankType                  m_shortType;
  LHCb::RawBank::BankType                  m_errorType;
  bool                                     m_getRaw = true;

  Gaudi::Property<bool>                     m_extraHeader{this, "DetectorSpecificHeader", false};
  bool                                      m_packed = false;
  Gaudi::Property<bool>                     m_cleanCorrupted{this, "CleanWhenCorruption", false};
  std::optional<LHCb::RawBankReadoutStatus> m_status;
  std::vector<int>                          m_readSources;
  bool                                      m_ok = false;

private:
  bool                  m_first = true;
  Gaudi::Property<bool> m_stat{this, "PrintStat", false};
};
#endif // CALODAQ_CALOREADOUTTOOL_H
