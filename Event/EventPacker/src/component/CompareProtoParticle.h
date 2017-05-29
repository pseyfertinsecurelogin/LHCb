#ifndef COMPAREPROTOPARTICLE_H
#define COMPAREPROTOPARTICLE_H 1

#include <string>

#include "Event/PackedProtoParticle.h"
#include "Event/ProtoParticle.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

/** @class CompareProtoParticle CompareProtoParticle.h
 *  Compare two containers of ProtoParticles
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
class CompareProtoParticle : public GaudiAlgorithm
{

public:

  /// Standard constructor
  CompareProtoParticle( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:

  std::string m_inputName;
  std::string m_testName;

};

#endif // COMPAREPROTOPARTICLE_H
