#ifndef COMPAREMCPARTICLE_H
#define COMPAREMCPARTICLE_H 1

#include <string>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;


/** @class CompareMCParticle CompareMCParticle.h
 *  Compare two containers of MCParticles
 *
 *  @author Olivier Callot
 *  @date   2005-03-18
 */
class CompareMCParticle : public GaudiAlgorithm
{

public:

  /// Standard constructor
  CompareMCParticle( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:

  std::string m_inputName;
  std::string m_testName;

};

#endif // COMPAREMCPARTICLE_H
