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
#ifndef   CALOFUTUREDIGIT_CALOFUTUREZSUPALG_H
#define   CALOFUTUREDIGIT_CALOFUTUREZSUPALG_H 1

#include <iomanip>

// from Gaudi
#include "GaudiKernel/IRndmGenSvc.h"
#include "GaudiAlg/GaudiAlgorithm.h"

// from CaloKernel
#include "Kernel/CaloCellID.h"
#include "CaloKernel/CaloVector.h"

// CaloDet
#include "CaloDet/DeCalorimeter.h"

// CaloFutureDAQ
#include "CaloFutureDAQ/ICaloFutureEnergyFromRaw.h"
#include "futuredetails.h"

/** @class CaloFutureZSupAlg CaloFutureZSupAlg.h
 *
 *  a (sub)Algorithm responsible
 *  for digitisation of MC-information
 *
 *  @author: Olivier Callot
 *   @date:   5 June 2000
 */


namespace futuredetails {

    enum class zsupMethod_t { none, one_d, two_d };

    const char* toString(const zsupMethod_t& out);
    inline std::ostream& toStream(const zsupMethod_t& out, std::ostream& os)
    { return os << std::quoted(toString(out),'\''); }
    StatusCode parse(zsupMethod_t& result, const std::string& input );
}

class CaloFutureZSupAlg : public GaudiAlgorithm {
public:

  CaloFutureZSupAlg( const std::string& name, ISvcLocator* pSvcLocator);

  StatusCode initialize() override;
  StatusCode execute   () override;

private:
  Gaudi::Property<std::string> m_detectorName{ this, "DetectorName" } ; ///< Detector element name
  Gaudi::Property<std::string> m_outputADCData{ this, "OutputADCData" } ;         ///< Output container for CaloAdc
  Gaudi::Property<std::string> m_outputDigitData{ this, "OutputDigitData"};       ///< Output container for CaloDigit
  Gaudi::Property<futuredetails::zsupMethod_t>  m_zsupMethod { this,  "ZsupMethod", futuredetails::zsupMethod_t::one_d }; ///< Name of Zero Suppression method
  Gaudi::Property<int>         m_zsupThreshold { this,  "ZsupThreshold" }  ; ///< Initial threshold, in ADC counts
  Gaudi::Property<int>         m_zsupNeighbor  { this,  "ZsupNeighbor", -256  } ; ///< zsup (ADC) for neighbors for 2D method
  Gaudi::Property<futuredetails::OutputType_t> m_outputType { this, "OutputType", {false, true} }; // ADC: false, Digits: true
  Gaudi::Property<std::string> m_extension{ this,  "Extension" };

  DeCalorimeter*         m_calo = nullptr;    ///< Detector element pointer
  ICaloFutureEnergyFromRaw*    m_adcTool = nullptr; ///< acces to adcs in Raw buffer
  int         m_numberOfCells =0    ; ///< Number of cells of this detector.
  Gaudi::Property<bool> m_statusOnTES{ this, "StatusOnTES", true };
};



#endif //    CALOFUTUREDIGIT_CALOFUTUREZSUPALG_H
