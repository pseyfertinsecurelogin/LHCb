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
#ifndef   CALODIGIT_CALOZSUPALG_H
#define   CALODIGIT_CALOZSUPALG_H 1

#include <iomanip>

// from Gaudi
#include "GaudiKernel/IRndmGenSvc.h"
#include "GaudiAlg/GaudiAlgorithm.h"

// from CaloKernel
#include "Kernel/CaloCellID.h"
#include "CaloKernel/CaloVector.h"

// CaloDet
#include "CaloDet/DeCalorimeter.h"

// CaloDAQ
#include "CaloDAQ/ICaloEnergyFromRaw.h"
#include "details.h"

/** @class CaloZSupAlg CaloZSupAlg.h
 *
 *  a (sub)Algorithm responsible
 *  for digitisation of MC-information
 *
 *  @author: Olivier Callot
 *   @date:   5 June 2000
 */


namespace details {

    enum class zsupMethod_t { none, one_d, two_d };

    const char* toString(const zsupMethod_t& out);
    inline std::ostream& toStream(const zsupMethod_t& out, std::ostream& os)
    { return os << std::quoted(toString(out),'\''); }
    StatusCode parse(zsupMethod_t& result, const std::string& input );
}

class CaloZSupAlg : public GaudiAlgorithm {
public:

  CaloZSupAlg( const std::string& name, ISvcLocator* pSvcLocator);

  StatusCode initialize() override;
  StatusCode execute   () override;

private:
  Gaudi::Property<std::string> m_detectorName{ this, "DetectorName" } ; ///< Detector element name
  Gaudi::Property<std::string> m_outputADCData{ this, "OutputADCData" } ;         ///< Output container for CaloAdc
  Gaudi::Property<std::string> m_outputDigitData{ this, "OutputDigitData"};       ///< Output container for CaloDigit
  Gaudi::Property<details::zsupMethod_t>  m_zsupMethod { this,  "ZsupMethod", details::zsupMethod_t::one_d }; ///< Name of Zero Suppression method
  Gaudi::Property<int>         m_zsupThreshold { this,  "ZsupThreshold" }  ; ///< Initial threshold, in ADC counts
  Gaudi::Property<int>         m_zsupNeighbor  { this,  "ZsupNeighbor", -256  } ; ///< zsup (ADC) for neighbors for 2D method
  Gaudi::Property<details::OutputType_t> m_outputType { this, "OutputType", {false, true} }; // ADC: false, Digits: true
  Gaudi::Property<std::string> m_extension{ this,  "Extension" };

  DeCalorimeter*         m_calo = nullptr;    ///< Detector element pointer
  ICaloEnergyFromRaw*    m_adcTool = nullptr; ///< acces to adcs in Raw buffer
  int         m_numberOfCells =0    ; ///< Number of cells of this detector.
  Gaudi::Property<bool> m_statusOnTES{ this, "StatusOnTES", true };
};



#endif //    CALODIGIT_CALOZSUPALG_H
