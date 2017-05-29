#ifndef COMPARECALOHYPO_H
#define COMPARECALOHYPO_H 1

#include <string>

#include "Event/PackedCaloHypo.h"
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

/** @class CompareCaloHypo CompareCaloHypo.h
 *  Compare two containers of CaloHypo
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
class CompareCaloHypo : public GaudiAlgorithm
{

public:

  /// Standard constructor
  CompareCaloHypo( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:

  std::string m_inputName;
  std::string m_testName;

};

#endif // COMPARECALOHYPO_H
