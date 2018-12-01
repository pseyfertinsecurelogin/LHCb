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
#ifndef CALOFILLPRSSPDRAWBUFFER_H 
#define CALOFILLPRSSPDRAWBUFFER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "CaloDet/DeCalorimeter.h"

#include "Event/RawBank.h"

/** @class CaloFillPrsSpdRawBuffer CaloFillPrsSpdRawBuffer.h
 *  
 *
 *  @author Olivier Callot
 *  @date   2005-01-04
 */
class CaloFillPrsSpdRawBuffer : public GaudiAlgorithm {
public: 
  /// Standard constructor
  CaloFillPrsSpdRawBuffer( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

private:

  void fillDataBank ( );

  void fillDataBankShort ( );

  void fillTriggerBank ( );

  void fillTriggerBankShort ( );

  void fillPackedBank ( );

  std::string m_inputBank;
  std::string m_prsBank;
  std::string m_spdBank;
  LHCb::RawBank::BankType m_bankType;
  LHCb::RawBank::BankType m_triggerBankType;
  int    m_numberOfBanks;
  int    m_dataCodingType;

  DeCalorimeter* m_calo;

  // Statistics
  
  double m_totDataSize;
  double m_totTrigSize;
  std::vector<double> m_dataSize;
  int m_nbEvents;
  std::vector< std::vector<unsigned int> > m_banks;
  std::vector< std::vector<unsigned int> > m_trigBanks;
};
#endif // CALOFILLPRSSPDRAWBUFFER_H
