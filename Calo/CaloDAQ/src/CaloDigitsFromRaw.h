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

#include "details.h"

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

private:
  void convertSpd( double energyScale );
  void convertCaloEnergies( );

  ICaloTriggerBitsFromRaw* m_spdTool = nullptr;
  ICaloEnergyFromRaw*  m_energyTool = nullptr;
  DeCalorimeter* m_calo = nullptr;
  int         m_detectorNum      ;

  Gaudi::Property<details::OutputType_t> m_outputType { this, "OutputType", { false, true }  }; // ADC: false, Digits: true
  Gaudi::Property<std::string> m_pinContainerName { this, "PinContainer"};
  Gaudi::Property<std::string> m_outputDigits{ this, "DigitsContainer"};
  Gaudi::Property<std::string> m_outputADCs{ this, "AdcsContainer" };
  Gaudi::Property<bool> m_statusOnTES { this, "StatusOnTES", true };
  Gaudi::Property<std::string> m_extension { this,"Extension","","Added to the default container name, for tests" };

};
#endif // CALOEVENT_CALODIGITSFROMRAW_H
