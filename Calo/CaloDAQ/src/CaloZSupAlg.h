#ifndef   CALODIGIT_CALOZSUPALG_H
#define   CALODIGIT_CALOZSUPALG_H 1

#include <string>

// CaloDAQ
#include "CaloDAQ/ICaloEnergyFromRaw.h"
// CaloDet
#include "CaloDet/DeCalorimeter.h"
#include "CaloKernel/CaloVector.h"
#include "GaudiAlg/GaudiAlgorithm.h"
// from Gaudi 
#include "GaudiKernel/IRndmGenSvc.h" 
#include "GaudiKernel/StatusCode.h"
// from CaloKernel
#include "Kernel/CaloCellID.h"

class DeCalorimeter;
class ISvcLocator;
struct ICaloEnergyFromRaw;
template <class T> class AlgFactory;

/** @class CaloZSupAlg CaloZSupAlg.h   
 *
 *  a (sub)Algorithm responsible 
 *  for digitisation of MC-information 
 *  
 *  @author: Olivier Callot
 *   @date:   5 June 2000
 */

class CaloZSupAlg : public GaudiAlgorithm { 
  friend class AlgFactory<CaloZSupAlg>;
public:
  
  CaloZSupAlg( const std::string& name, ISvcLocator* pSvcLocator);
  
  StatusCode initialize() override;
  StatusCode execute   () override;
  
private:   
  std::string m_detectorName;       ///< Detector element name
  std::string m_inputToolName;
  std::string m_inputToolType;
  std::string m_outputADCData;         ///< Output container for CaloAdc
  std::string m_outputDigitData;       ///< Output container for CaloDigit
  std::string m_zsupMethod        ; ///< Name of Zero Suppression method
  bool        m_zsup2D = false            ; ///< do we use 2D-zero sup. scheme ?
  int         m_zsupThreshold     ; ///< Initial threshold, in ADC counts 
  int         m_zsupNeighbor      ; ///< zsup (ADC) for neighbors for 2D method
  bool m_adcOnTES = false;
  bool m_digitOnTES = false;
  std::string m_outputType;
  std::string m_extension;
  

  DeCalorimeter*         m_calo = nullptr;    ///< Detector element pointer
  ICaloEnergyFromRaw*    m_adcTool = nullptr; ///< acces to adcs in Raw buffer
  int         m_numberOfCells =0    ; ///< Number of cells of this detector.
  bool m_statusOnTES;
};


#endif //    CALODIGIT_CALOZSUPALG_H
