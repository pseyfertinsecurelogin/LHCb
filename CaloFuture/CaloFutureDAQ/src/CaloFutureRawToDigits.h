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
#ifndef   CALOFUTUREDIGIT_CALOFUTURERAWTODIGITS_H
#define   CALOFUTUREDIGIT_CALOFUTURERAWTODIGITS_H 1

#include <iomanip>

// from Gaudi
#include "GaudiKernel/IRndmGenSvc.h"
#include "GaudiAlg/Transformer.h"

// from CaloKernel
#include "Kernel/CaloCellID.h"
#include "CaloKernel/CaloVector.h"

// CaloDet
#include "CaloDet/DeCalorimeter.h"

// CaloDAQ
//#include "CaloDAQ/ICaloEnergyFromRaw.h"
#include "futuredetails.h"

#include "Event/RawEvent.h"
#include "Event/RawBankReadoutStatus.h"
#include "Event/CaloAdc.h"
#include "Event/CaloDigit.h"


class CaloFutureRawToDigits : 
public Gaudi::Functional::MultiTransformer<std::tuple<LHCb::CaloAdcs,
  LHCb::CaloDigits,
  LHCb::RawBankReadoutStatus>
  (const LHCb::RawEvent& rawEvt) > {
  
 public:
  
  CaloFutureRawToDigits( const std::string& name, ISvcLocator* pSvcLocator);
  StatusCode initialize() override;
  std::tuple<LHCb::CaloAdcs,LHCb::CaloDigits,LHCb::RawBankReadoutStatus> operator()(const LHCb::RawEvent& rawEvt) const override;
  std::vector<LHCb::CaloAdc> decode( const LHCb::RawBank& bank, LHCb::RawBankReadoutStatus status, bool getPinData ) const;

 private:
  
  bool checkCards(int nCards, const std::vector<int>& feCards ) const;
  int findCardbyCode(const std::vector<int>& feCards , int code) const;
  void checkCtrl(int ctrl,int sourceID, LHCb::RawBankReadoutStatus status) const;
    
  Gaudi::Property<std::string> m_detectorName  { this, "DetectorName" } ;           //< Detector element name
  Gaudi::Property<std::string> m_zsupMethod    { this, "ZSupMethod", "1D" };
  Gaudi::Property<int>         m_zsupThreshold { this,  "ZsupThreshold" };        //< Initial threshold, in ADC counts
  Gaudi::Property<int>         m_zsupNeighbor  { this,  "ZsupNeighbor", -256  } ; //< zsup (ADC) for neighbors for 2D method
  Gaudi::Property<futuredetails::OutputType_t> m_outputType { this, "OutputType", {false, true} }; // ADC: false, Digits: true
  Gaudi::Property<std::string> m_extension     { this,  "Extension" };
  Gaudi::Property<bool> m_packedIsDefault      { this, "PackedIsDefault",false };
  Gaudi::Property<std::string> m_detData       { this, "Detector", LHCb::CaloFutureAlgUtils::DeCaloFutureLocation("Ecal")};
  DeCalorimeter*         m_calo = nullptr;    ///< Detector element pointer
  LHCb::RawBank::BankType m_packedType;
  LHCb::RawBank::BankType m_shortType;
  LHCb::RawBank::BankType m_errorType;

  Gaudi::Property<bool> m_extraHeader { this, "DetectorSpecificHeader", false };
  Gaudi::Property<bool> m_cleanCorrupted {this, "CleanWhenCorruption", false };
  //bool m_packed = false;
  std::vector<int> m_readSources;
  bool m_ok = false;
  int         m_numberOfCells =0    ; ///< Number of cells of this detector.
};



#endif //    CALOFUTUREDIGIT_CALOFUTUREZSUPALG_H
