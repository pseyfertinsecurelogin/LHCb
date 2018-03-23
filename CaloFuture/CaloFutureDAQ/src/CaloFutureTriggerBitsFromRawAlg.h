#ifndef CALOFUTURETRIGGERBITSFROMRAWALG_H 
#define CALOFUTURETRIGGERBITSFROMRAWALG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
// CaloFutureDAQ
#include "CaloFutureDAQ/ICaloFutureTriggerBitsFromRaw.h"

/** @class CaloFutureTriggerBitsFromRawAlg CaloFutureTriggerBitsFromRawAlg.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2006-04-07
 */
class CaloFutureTriggerBitsFromRawAlg final : public GaudiAlgorithm {
public: 
  /// Standard constructor
  CaloFutureTriggerBitsFromRawAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  std::string m_outputData;         ///< Output container
  std::string m_toolName;
  std::string m_toolType;
  bool m_isPrs = false;
  ICaloFutureTriggerBitsFromRaw*    m_l0BitTool = nullptr;
  std::string m_extension;  
  bool m_statusOnTES;
};
#endif // CALOFUTURETRIGGERBITSFROMRAWALG_H