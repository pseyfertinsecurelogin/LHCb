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
#ifndef CALOFUTUREFILLRAWBUFFER_H
#define CALOFUTUREFILLRAWBUFFER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "CaloDet/DeCalorimeter.h"
#include "Event/RawEvent.h"

/** @class CaloFutureFillRawBuffer CaloFutureFillRawBuffer.h
 *  Fills the Raw Buffer banks for the calorimeter
 *
 *  @author Olivier Callot
 *  @date   2004-12-17
 */
class CaloFutureFillRawBuffer : public GaudiAlgorithm {
public:
  /// Standard constructor
  CaloFutureFillRawBuffer( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

protected:
  void fillDataBankShort();

  void fillPackedBank();

  void fillTriggerBank();

private:
  std::string             m_inputBank;
  std::string             m_triggerBank;
  std::string             m_detectorName = "None";
  std::string             m_detectorLocation;
  LHCb::RawBank::BankType m_bankType        = LHCb::RawBank::LastType;
  LHCb::RawBank::BankType m_triggerBankType = LHCb::RawBank::LastType;
  int                     m_numberOfBanks   = 1;
  Gaudi::Property<int>    m_dataCodingType{ this, "DataCodingType", 1,
      [this](Property&){
  if ( 2 < m_dataCodingType || 0 >= m_dataCodingType ) throw std::invalid_argument("Invalid Data Coding Type");
      }  };

  DeCalorimeter* m_calo = nullptr;

  // Statistics

  double                                 m_totDataSize = 0;
  std::vector<double>                    m_dataSize;
  double                                 m_totTrigSize = 0;
  int                                    m_nbEvents    = 0;
  std::vector<std::vector<unsigned int>> m_banks;
  std::vector<std::vector<unsigned int>> m_trigBanks;
  bool                                   m_pin = false;
};
#endif // CALOFUTUREFILLRAWBUFFER_H
