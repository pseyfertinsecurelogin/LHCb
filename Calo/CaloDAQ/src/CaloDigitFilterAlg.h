#ifndef CALODIGITFILTERALG_H 
#define CALODIGITFILTERALG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "CaloDAQ/ICaloDigitFilterTool.h"


/** @class CaloDigitFilterAlg CaloDigitFilterAlg.h
 *  
 *  @author Olivier Deschamps
 *  @date   2010-12-21
 */
class CaloDigitFilterAlg : public GaudiAlgorithm {
public: 
  /// Standard constructor
  CaloDigitFilterAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  int m_ecal;
  int m_hcal;
  int m_prs;
  int m_spd;
  ICaloDigitFilterTool* m_filter = nullptr;
};
#endif // CALODIGITFILTERALG_H
