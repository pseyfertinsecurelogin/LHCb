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
#ifndef CALOTRIGGERADCSFROMCALORAW_H
#define CALOTRIGGERADCSFROMCALORAW_H 1

// Include files
// from Gaudi
#include "CaloDAQ/ICaloTriggerAdcsFromRaw.h" // Interface
#include "GaudiAlg/GaudiTool.h"

class DeCalorimeter;
struct ICaloDataProvider;

/** @class CaloTriggerAdcsFromCaloRaw CaloTriggerAdcsFromCaloRaw.h
 *  Compute Trigger Adcs from Calo Adcs
 *
 *  @author Patrick Robbe
 *  @date   2014-03-04
 */
class CaloTriggerAdcsFromCaloRaw : public GaudiTool, virtual public ICaloTriggerAdcsFromRaw {
public:
  /// Standard constructor
  CaloTriggerAdcsFromCaloRaw( const std::string& type, const std::string& name, const IInterface* parent );

  StatusCode initialize() override;

  const std::vector<LHCb::L0CaloAdc>& adcs() override;
  const std::vector<LHCb::L0CaloAdc>& adcs( int source ) override;
  const std::vector<LHCb::L0CaloAdc>& adcs( const LHCb::RawBank& bank ) override;
  const std::vector<LHCb::L0CaloAdc>& pinAdcs() override;

  StatusCode _setProperty( const std::string&, const std::string& ) override { return StatusCode::SUCCESS; }
  bool       getBanks() override { return true; }
  void       setBanks( LHCb::span<const LHCb::RawBank*> ) override {}
  void       clear() override { m_data.clear(); }
  void       cleanData( int ) override { clear(); }
  LHCb::RawBankReadoutStatus& status() override {
    m_theSt = LHCb::RawBankReadoutStatus( LHCb::RawBank::EcalTrig );
    return m_theSt;
  }
  void           putStatusOnTES() override { return; }
  bool           ok() override { return true; }
  DeCalorimeter* deCalo() override { return m_calo; }

private:
  LHCb::RawBankReadoutStatus   m_theSt;
  std::vector<LHCb::L0CaloAdc> m_data;
  std::string                  m_detectorName;
  ICaloDataProvider*           m_adcs = nullptr;
  DeCalorimeter*               m_calo = nullptr;

  bool                    m_doubleScale;
  bool                    m_fixFor2016;
  LHCb::RawBank::BankType m_bank;
  /// List of locations in the transient store to search the RawEvent object.
  std::vector<std::string> m_rawEventLocations;

  int l0adcFromAdc( const int adc, const LHCb::CaloCellID& id ) const;
};
#endif // CALOTRIGGERADCSFROMCALORAW_H
