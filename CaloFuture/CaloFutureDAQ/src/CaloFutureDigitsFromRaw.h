#ifndef CALOFUTUREEVENT_CALOFUTUREDIGITSFROMRAW_H
#define CALOFUTUREEVENT_CALOFUTUREDIGITSFROMRAW_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

// from LHCb
#include "Event/CaloDigit.h"
#include "CaloFutureDAQ/ICaloFutureTriggerBitsFromRaw.h"
#include "CaloFutureDAQ/ICaloFutureEnergyFromRaw.h"
#include "GaudiAlg/ISequencerTimerTool.h"
#include "CaloDet/DeCalorimeter.h"
#include "Event/RawEvent.h"
#include "Event/RawBankReadoutStatus.h"

#include "futuredetails.h"

/** @class CaloFutureDigitsFromRaw CaloFutureDigitsFromRaw.h component/CaloFutureDigitsFromRaw.h
 *  Create the CaloFutureDIgits containers from the Raw buffer
 *
 *  @author Olivier Callot
 *  @date   2003-11-18
 */

class CaloFutureDigitsFromRaw : public GaudiAlgorithm {
public:
  /// Standard constructor
  CaloFutureDigitsFromRaw( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize () override;    ///< Algorithm initialization
  StatusCode execute    () override;    ///< Algorithm execution

private:
  void convertSpd( double energyScale );
  void convertCaloFutureEnergies( );

  ICaloFutureTriggerBitsFromRaw* m_spdTool = nullptr;
  ICaloFutureEnergyFromRaw*  m_energyTool = nullptr;
  DeCalorimeter* m_calo = nullptr;
  int         m_detectorNum      ;

  Gaudi::Property<futuredetails::OutputType_t> m_outputType { this, "OutputType", { false, true }  }; // ADC: false, Digits: true
  Gaudi::Property<std::string> m_pinContainerName { this, "PinContainer"};
  Gaudi::Property<std::string> m_outputDigits{ this, "DigitsContainer"};
  Gaudi::Property<std::string> m_outputADCs{ this, "AdcsContainer" };
  Gaudi::Property<bool> m_statusOnTES { this, "StatusOnTES", true };
  Gaudi::Property<std::string> m_extension { this,"Extension","","Added to the default container name, for tests" };

};
#endif // CALOFUTUREEVENT_CALOFUTUREDIGITSFROMRAW_H
