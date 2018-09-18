#ifndef COMPONENT_COMPARECALOFUTUREDIGITS_H 
#define COMPONENT_COMPARECALOFUTUREDIGITS_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Event/CaloDigit.h"

/** @class CompareCaloFutureDigits CompareCaloFutureDigits.h component/CompareCaloFutureDigits.h
 *  Compare the containers of CaloDigits
 *
 *  @author Olivier Callot
 *  @date   2003-11-18
 */
class CompareCaloFutureDigits final : public GaudiAlgorithm {
public:
  /// Standard constructor
  CompareCaloFutureDigits( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override;    ///< Algorithm execution

private:
  void compareContainers ( const LHCb::CaloDigits& dig1,
                           const LHCb::CaloDigits& dig2,
                           double tol) const;
  
  std::string m_extension;
  bool m_packedRawBuffer;
};
#endif // COMPONENT_COMPARECALOFUTUREDIGITS_H
