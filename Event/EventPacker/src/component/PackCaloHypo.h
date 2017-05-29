#ifndef PACKCALOHYPO_H
#define PACKCALOHYPO_H 1

#include <string>

#include "Event/PackedCaloHypo.h"
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

/** @class PackCaloHypo PackCaloHypo.h
 *
 *  Pack a CaloHypo container
 *
 *  @author Olivier Callot
 *  @date   2008-11-12
 */
class PackCaloHypo : public GaudiAlgorithm
{

public:

  /// Standard constructor
  PackCaloHypo( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:

  std::string m_inputName;
  std::string m_outputName;
  bool m_alwaysOutput;      ///< Flag to turn on the creation of output, even when input is missing
  bool m_deleteInput;       ///< Flag to turn on the removal of the input data after packing
  bool m_enableCheck;       ///< Flag to turn on automatic unpacking and checking of the output post-packing
  bool m_clearRegistry;     ///< Flag to turn on the clearing of the registry if the input data is not kept

};

#endif // PACKCALOHYPO_H
