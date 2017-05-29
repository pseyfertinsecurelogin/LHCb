#ifndef PACKMCPARTICLE_H
#define PACKMCPARTICLE_H 1

#include <string>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;


/** @class PackMCParticle PackMCParticle.h
 *  Pack the MCParticles
 *
 *  @author Olivier Callot
 *  @date   2005-03-18
 */
class PackMCParticle : public GaudiAlgorithm
{

public:

  /// Standard constructor
  PackMCParticle( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override;  ///< Algorithm execution

private:

  std::string m_inputName;
  std::string m_outputName;
  bool m_alwaysOutput;      ///< Flag to turn on the creation of output, even when input is missing
  bool m_deleteInput;       ///< Flag to turn on the removal of the input data after packing

};

#endif // PACKMCPARTICLE_H
