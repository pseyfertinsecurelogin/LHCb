#ifndef CALOTRIGGERBITSFROMRAWALG_H 
#define CALOTRIGGERBITSFROMRAWALG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
// CaloDAQ
#include "CaloDAQ/ICaloTriggerBitsFromRaw.h"

/** @class CaloTriggerBitsFromRawAlg CaloTriggerBitsFromRawAlg.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2006-04-07
 */
class CaloTriggerBitsFromRawAlg final : public GaudiAlgorithm {
public: 
  /// Standard constructor
  CaloTriggerBitsFromRawAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  std::string m_outputData;         ///< Output container
  std::string m_toolName;
  std::string m_toolType;
  bool m_isPrs = false;
  ICaloTriggerBitsFromRaw*    m_l0BitTool = nullptr;
  std::string m_extension;  
  bool m_statusOnTES;
};
#endif // CALOTRIGGERBITSFROMRAWALG_H
