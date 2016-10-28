#ifndef UNPACKMCPARTICLE_H
#define UNPACKMCPARTICLE_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"


/** @class UnpackMCParticle UnpackMCParticle.h
 *  Unpack the MCParticles
 *
 *  @author Olivier Callot
 *  @date   2005-03-18
 */
class UnpackMCParticle : public GaudiAlgorithm
{

public:

  /// Standard constructor
  UnpackMCParticle( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:

  std::string m_inputName;
  std::string m_outputName;
  bool m_alwaysOutput;      ///< Flag to turn on the creation of output, even when input is missing

};

#endif // UNPACKMCPARTICLE_H
