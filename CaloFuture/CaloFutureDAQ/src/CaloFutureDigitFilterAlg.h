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
#include "Event/CaloDigit.h"
#include "Event/RecVertex.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/Transformer.h"
#include "GaudiKernel/Counters.h"

/** @class CaloFutureDigitFilterAlg CaloFutureDigitFilterAlg.h
 *
 *  @author Olivier Deschamps
 *  @date   2010-12-21
 */

class CaloFutureDigitFilterAlg : public Gaudi::Functional::Transformer<LHCb::CaloDigits(
                                     const LHCb::CaloDigits& caloDigits, const LHCb::RecVertices& recVertices )> {

public:
  CaloFutureDigitFilterAlg( const std::string& name, ISvcLocator* pSvcLocator );
  StatusCode       initialize() override;
  LHCb::CaloDigits operator()( const LHCb::CaloDigits&  caloDigits,
                               const LHCb::RecVertices& recVertices ) const override;

  struct CondValue {
    double offset, offsetRMS;
  };
  using Offsets  = std::map<LHCb::CaloCellID, double>;
  using CondMaps = std::map<LHCb::CaloCellID, CondValue>;

  bool cleanDigits( const LHCb::CaloDigits& digits, const LHCb::RecVertices& verts, const std::string& det,
                    bool substr = true, bool mask = true ) const; // override;

private:
  Gaudi::Property<std::string> m_detectorName{this, "DetectorName", "Ecal", "Detector element name"};

  Gaudi::Property<int> m_ecal{this, "EcalFilter", 0x3}; // 1 = Mask , 2=Offset , 3 = both, 0 = none
  Gaudi::Property<int> m_hcal{this, "HcalFilter", 0x3}; // 1 = Mask , 2=Offset , 3 = both, 0 = none
  DeCalorimeter*       m_calo = nullptr;                ///< Detector element pointer

  Gaudi::Property<std::map<std::string, int>> m_maskMap{this, "MaskMap"};
  Gaudi::Property<bool>                       m_useCondDB{this, "UseCondDB", true};
  Gaudi::Property<int>    m_scalingMethod{this, "ScalingMethod", 1}; // 0 : SpdMult ; 1 = nPV  (+10 for clusters)
  Gaudi::Property<int>    m_scalingBin{this, "ScalingBin", 50};
  Gaudi::Property<double> m_scalingMin{this, "ScalingMin", 150};
  Gaudi::Property<bool>   m_usePV3D{this, "UsePV3D", false};
  Gaudi::Property<int>    m_setCounters{this, "SetCounterLevel", 1};

  //  m_maskMap["Default"]= CaloCellQuality::OfflineMask;

  mutable Gaudi::Accumulators::AveragingCounter<> m_offsetScaleCounter{this, "Offset scale"};
  mutable Gaudi::Accumulators::AveragingCounter<> m_maskedDigitsCounter{this, "Masked digits"};
  mutable Gaudi::Accumulators::AveragingCounter<> m_avgOffsetCounter{this, "Average offset in ADC"};

protected:
  void cleanDigit( LHCb::CaloDigit& digit, bool substr = true, int scale = -1, bool mask = true );
};

#endif // CALOFUTUREDIGITFILTERALG_H
