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
// $Id: CaloFillRawBuffer.h,v 1.7 2007-12-06 09:31:24 odescham Exp $
#ifndef CALOFILLRAWBUFFER_H 
#define CALOFILLRAWBUFFER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Event/RawEvent.h"
#include "CaloDet/DeCalorimeter.h"

/** @class CaloFillRawBuffer CaloFillRawBuffer.h
 *  Fills the Raw Buffer banks for the calorimeter
 *
 *  @author Olivier Callot
 *  @date   2004-12-17
 */
class CaloFillRawBuffer : public GaudiAlgorithm {
public: 
  /// Standard constructor
  CaloFillRawBuffer( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

protected:

  void fillDataBankShort ( );

  void fillPackedBank ( );

  void fillTriggerBank ( );

private:
  std::string m_inputBank;
  std::string m_triggerBank;
  std::string m_detectorName = "None";  
  std::string m_detectorLocation;
  LHCb::RawBank::BankType m_bankType = LHCb::RawBank::LastType;
  LHCb::RawBank::BankType m_triggerBankType = LHCb::RawBank::LastType;
  int    m_numberOfBanks = 0;
  int    m_dataCodingType;

  DeCalorimeter* m_calo = nullptr;

  // Statistics
  
  double m_totDataSize = 0;
  std::vector<double> m_dataSize;
  double m_totTrigSize = 0;
  int m_nbEvents = 0;
  std::vector< std::vector<unsigned int> > m_banks;
  std::vector< std::vector<unsigned int> > m_trigBanks;
  bool m_pin;
  
};
#endif // CALOFILLRAWBUFFER_H
