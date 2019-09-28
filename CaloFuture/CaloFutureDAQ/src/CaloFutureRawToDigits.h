/*****************************************************************************\
* (c) Copyright 2018-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef CALOFUTUREDIGIT_CALOFUTURERAWTODIGITS_H
#define CALOFUTUREDIGIT_CALOFUTURERAWTODIGITS_H 1

// from Gaudi
#include "GaudiAlg/Transformer.h"
#include "GaudiKernel/Counters.h"
#include "GaudiKernel/IRndmGenSvc.h"

#include "DetDesc/Condition.h"
#include "DetDesc/ConditionAccessorHolder.h"

// from CaloKernel
#include "CaloKernel/CaloVector.h"
#include "Kernel/CaloCellID.h"

// CaloDet
#include "CaloDet/DeCalorimeter.h"

// CaloDAQ
#include "futuredetails.h"

#include "Event/CaloAdc.h"
#include "Event/CaloDigit.h"
#include "Event/RawBankReadoutStatus.h"
#include "Event/RawEvent.h"

class CaloFutureRawToDigits : public Gaudi::Functional::MultiTransformer<
                                  std::tuple<LHCb::CaloAdcs, LHCb::CaloDigits, LHCb::RawBankReadoutStatus>(
                                      const LHCb::RawEvent& rawEvt, const DeCalorimeter& ),
                                  LHCb::DetDesc::usesConditions<DeCalorimeter>> {

public:
  CaloFutureRawToDigits( const std::string& name, ISvcLocator* pSvcLocator );
  StatusCode initialize() override;
  std::tuple<LHCb::CaloAdcs, LHCb::CaloDigits, LHCb::RawBankReadoutStatus>
  operator()( const LHCb::RawEvent&, const DeCalorimeter& ) const override;

private:
  enum class Decode { PinData, Cell };

  template <Decode>
  std::vector<LHCb::CaloAdc> decode_( const LHCb::RawBank&, LHCb::RawBankReadoutStatus&, const DeCalorimeter& ) const;
  template <Decode>
  std::vector<LHCb::CaloAdc> decode_v1( int sourceID, LHCb::span<const unsigned int>, LHCb::RawBankReadoutStatus&,
                                        const DeCalorimeter& ) const;
  template <Decode>
  std::vector<LHCb::CaloAdc> decode_v2( int sourceID, LHCb::span<const unsigned int>, LHCb::RawBankReadoutStatus&,
                                        const DeCalorimeter& ) const;
  template <Decode>
  std::vector<LHCb::CaloAdc> decode_v3( int sourceID, LHCb::span<const unsigned int>, LHCb::RawBankReadoutStatus&,
                                        const DeCalorimeter& ) const;

  bool checkCards( const DeCalorimeter&, int nCards, const std::vector<int>& feCards ) const;
  int  findCardbyCode( const DeCalorimeter&, const std::vector<int>& feCards, int code ) const;
  void checkCtrl( int ctrl, int sourceID, LHCb::RawBankReadoutStatus& status ) const;

  Gaudi::Property<std::string> m_zsupMethod{this, "ZSupMethod", "1D"};
  Gaudi::Property<int>         m_zsupThreshold{this, "ZSupThreshold", -1000, "Initial threshold, in ADC counts"};
  Gaudi::Property<int>         m_zsupNeighbour{this, "ZSupNeighbour", -256, "zsup (ADC) for neighbours for 2D method"};
  Gaudi::Property<futuredetails::OutputType_t> m_outputType{
      this, "OutputType", {false, true}, "ADC: false, Digits: true"};
  Gaudi::Property<std::string> m_extension{this, "Extension"};
  Gaudi::Property<bool>        m_packedIsDefault{this, "PackedIsDefault", false};

  Gaudi::Property<bool> m_extraHeader{this, "DetectorSpecificHeader", false};
  Gaudi::Property<bool> m_cleanCorrupted{this, "CleanWhenCorruption", false};

  mutable Gaudi::Accumulators::BinomialCounter<> m_noBanksCounter{this, "No bank found"};
  mutable Gaudi::Accumulators::Counter<>         m_duplicateADCDigits{this, "# duplicate ADC/Digits"};
};

#endif //    CALOFUTUREDIGIT_CALOFUTUREZSUPALG_H
