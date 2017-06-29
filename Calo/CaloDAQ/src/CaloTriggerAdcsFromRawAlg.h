#ifndef CALOTRIGGERADCSFROMRAWALG_H
#define CALOTRIGGERADCSFROMRAWALG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
// CaloDAQ
#include "CaloDAQ/ICaloTriggerAdcsFromRaw.h"

/** @class CaloTriggerAdcsFromRawAlg CaloTriggerAdcsFromRawAlg.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2006-04-07
 */

class CaloTriggerAdcsFromRawAlg : public GaudiAlgorithm {
public:
  /// Standard constructor
  CaloTriggerAdcsFromRawAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  Gaudi::Property<std::string> m_outputData { this, "OutputData" };          // Output container
  Gaudi::Property<std::string> m_pinContainer{ this, "PinContainer" } ;        // Output container for pinData
  Gaudi::Property<std::string> m_extension { this,  "Extension" };
  Gaudi::Property<bool> m_statusOnTES{ this,  "StatusOnTES"   , true };
  ICaloTriggerAdcsFromRaw*    m_l0AdcTool = nullptr;
};

#endif // CALOTRIGGERADCSFROMRAWALG_H
