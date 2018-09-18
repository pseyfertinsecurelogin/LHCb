#ifndef CALOFUTURETRIGGERADCSFROMRAWALG_H
#define CALOFUTURETRIGGERADCSFROMRAWALG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
// CaloFutureDAQ
#include "CaloFutureDAQ/ICaloFutureTriggerAdcsFromRaw.h"

/** @class CaloFutureTriggerAdcsFromRawAlg CaloFutureTriggerAdcsFromRawAlg.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2006-04-07
 */

class CaloFutureTriggerAdcsFromRawAlg : public GaudiAlgorithm {
public:
  /// Standard constructor
  CaloFutureTriggerAdcsFromRawAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  Gaudi::Property<std::string> m_outputData { this, "OutputData" };          // Output container
  Gaudi::Property<std::string> m_pinContainer{ this, "PinContainer" } ;        // Output container for pinData
  Gaudi::Property<std::string> m_extension { this,  "Extension" };
  Gaudi::Property<bool> m_statusOnTES{ this,  "StatusOnTES"   , true };
  ICaloFutureTriggerAdcsFromRaw*    m_l0AdcTool = nullptr;
};

#endif // CALOFUTURETRIGGERADCSFROMRAWALG_H
