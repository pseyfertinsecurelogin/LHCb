#ifndef CALOTRIGGERADCSFROMRAWALG_H 
#define CALOTRIGGERADCSFROMRAWALG_H 1

#include <string>

// CaloDAQ
#include "CaloDAQ/ICaloTriggerAdcsFromRaw.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;
struct ICaloTriggerAdcsFromRaw;

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
  std::string m_outputData;          // Output container
  std::string m_pinContainer;        // Output container for pinData
  std::string m_toolName;
  std::string m_toolType;
  ICaloTriggerAdcsFromRaw*    m_l0AdcTool;
  std::string m_extension;
  bool m_statusOnTES;
};
#endif // CALOTRIGGERADCSFROMRAWALG_H
