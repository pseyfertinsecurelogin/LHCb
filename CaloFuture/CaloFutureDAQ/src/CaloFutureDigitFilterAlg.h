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
#ifndef CALOFUTUREDIGITFILTERALG_H
#define CALOFUTUREDIGITFILTERALG_H 1

// Include files
// from Gaudi
#include "CaloDet/DeCalorimeter.h"
#include "DetDesc/ConditionAccessor.h"
#include "DetDesc/ConditionAccessorHolder.h"
#include "Event/CaloDigit.h"
#include "Event/RecVertex.h"
#include "GaudiAlg/Transformer.h"
#include "GaudiKernel/Counters.h"

namespace LHCb::Calo::DAQ::Algorithms {

  /** @class DigitFilter CaloFutureDigitFilterAlg.h
   *
   *  @author Olivier Deschamps
   *  @date   2010-12-21
   */

  class DigitFilter
      : public Gaudi::Functional::Transformer<CaloDigits( const CaloDigits&, const RecVertices&, const DeCalorimeter& ),
                                              DetDesc::usesConditions<DeCalorimeter>> {
  public:
    DigitFilter( const std::string& name, ISvcLocator* pSvcLocator );
    CaloDigits operator()( const CaloDigits&, const RecVertices&, const DeCalorimeter& ) const override;

  private:
    bool cleanDigits( const CaloDigits&, const RecVertices&, const DeCalorimeter&, bool substr = true,
                      bool mask = true ) const; // override;

    Gaudi::Property<int> m_ecal{this, "EcalFilter", 0x3}; // 1 = Mask , 2=Offset , 3 = both, 0 = none
    Gaudi::Property<int> m_hcal{this, "HcalFilter", 0x3}; // 1 = Mask , 2=Offset , 3 = both, 0 = none

    Gaudi::Property<std::map<std::string, int>> m_maskMap{this, "MaskMap"};
    //  m_maskMap["Default"]= CaloCellQuality::OfflineMask;
    Gaudi::Property<bool>   m_useCondDB{this, "UseCondDB", true};
    Gaudi::Property<int>    m_scalingMethod{this, "ScalingMethod", 1}; // 0 : SpdMult ; 1 = nPV  (+10 for clusters)
    Gaudi::Property<int>    m_scalingBin{this, "ScalingBin", 50};
    Gaudi::Property<double> m_scalingMin{this, "ScalingMin", 150};
    Gaudi::Property<bool>   m_usePV3D{this, "UsePV3D", false};
    Gaudi::Property<int>    m_setCounters{this, "SetCounterLevel", 1};

    mutable Gaudi::Accumulators::AveragingCounter<>       m_offsetScaleCounter{this, "Offset scale"};
    mutable Gaudi::Accumulators::AveragingCounter<>       m_maskedDigitsCounter{this, "Masked digits"};
    mutable Gaudi::Accumulators::AveragingCounter<>       m_avgOffsetCounter{this, "Average offset in ADC"};
    mutable Gaudi::Accumulators::MsgCounter<MSG::WARNING> m_filterfailed{this, "digits filtering failed"};
  };

} // namespace LHCb::Calo::DAQ::Algorithms

#endif // CALOFUTUREDIGITFILTERALG_H
