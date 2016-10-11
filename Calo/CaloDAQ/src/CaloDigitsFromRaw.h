#ifndef CALOEVENT_CALODIGITSFROMRAW_H 
#define CALOEVENT_CALODIGITSFROMRAW_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

// from LHCb
#include "Event/CaloDigit.h"
#include "CaloDAQ/ICaloTriggerBitsFromRaw.h"
#include "CaloDAQ/ICaloEnergyFromRaw.h"
#include "GaudiAlg/ISequencerTimerTool.h"
#include "CaloDet/DeCalorimeter.h"
#include "Event/RawEvent.h"
#include "Event/RawBankReadoutStatus.h"

/** @class CaloDigitsFromRaw CaloDigitsFromRaw.h component/CaloDigitsFromRaw.h
 *  Create the CaloDIgits containers from the Raw buffer
 *
 *  @author Olivier Callot
 *  @date   2003-11-18
 */
class CaloDigitsFromRaw : public GaudiAlgorithm {
public:
  /// Standard constructor
  CaloDigitsFromRaw( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize () override;    ///< Algorithm initialization
  StatusCode execute    () override;    ///< Algorithm execution


protected:
  
  void convertSpd( double energyScale );
  
  void convertCaloEnergies( );

private:
  std::string m_extension ; ///< Added to the default container name, for tests
  int         m_detectorNum      ;

  ICaloTriggerBitsFromRaw* m_spdTool = nullptr;
  ICaloEnergyFromRaw*  m_energyTool = nullptr;

  bool m_adcOnTES = false;
  bool m_digitOnTES = false;
  std::string m_outputType;
  std::string m_pinContainerName;
  std::string m_outputDigits;
  std::string m_outputADCs;
  DeCalorimeter* m_calo = nullptr;
  bool m_statusOnTES;  
};
#endif // CALOEVENT_CALODIGITSFROMRAW_H
