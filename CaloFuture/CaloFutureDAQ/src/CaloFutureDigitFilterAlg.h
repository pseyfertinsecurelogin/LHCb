#ifndef CALOFUTUREDIGITFILTERALG_H
#define CALOFUTUREDIGITFILTERALG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "CaloFutureDAQ/ICaloFutureDigitFilterTool.h"


/** @class CaloFutureDigitFilterAlg CaloFutureDigitFilterAlg.h
 *
 *  @author Olivier Deschamps
 *  @date   2010-12-21
 */


class CaloFutureDigitFilterAlg : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  Gaudi::Property<int> m_ecal { this, "EcalFilter" , 0x3 }; // 1 = Mask , 2=Offset , 3 = both, 0 = none
  Gaudi::Property<int> m_hcal { this, "HcalFilter" , 0x3 }; // 1 = Mask , 2=Offset , 3 = both, 0 = none
  Gaudi::Property<int> m_prs  { this, "PrsFilter"  , 0x3 }; // 1 = Mask , 2=Offset , 3 = both, 0 = none
  Gaudi::Property<int> m_spd  { this, "SpdFilter"  , 0x1 }; // 1 = Mask , 2=Offset , 3 = both, 0 = none
  ICaloFutureDigitFilterTool* m_filter = nullptr;
};

#endif // CALOFUTUREDIGITFILTERALG_H
