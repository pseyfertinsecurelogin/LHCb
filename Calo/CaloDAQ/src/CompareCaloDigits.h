#ifndef COMPONENT_COMPARECALODIGITS_H 
#define COMPONENT_COMPARECALODIGITS_H 1

#include <string>

#include "Event/CaloDigit.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

/** @class CompareCaloDigits CompareCaloDigits.h component/CompareCaloDigits.h
 *  Compare the containers of CaloDigits
 *
 *  @author Olivier Callot
 *  @date   2003-11-18
 */
class CompareCaloDigits final : public GaudiAlgorithm {
public:
  /// Standard constructor
  CompareCaloDigits( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override;    ///< Algorithm execution

private:
  void compareContainers ( const LHCb::CaloDigits& dig1,
                           const LHCb::CaloDigits& dig2,
                           double tol) const;
  
  std::string m_extension;
  bool m_packedRawBuffer;
};
#endif // COMPONENT_COMPARECALODIGITS_H
