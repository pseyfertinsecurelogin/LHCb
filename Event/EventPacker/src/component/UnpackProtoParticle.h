#ifndef UNPACKPROTOPARTICLE_H
#define UNPACKPROTOPARTICLE_H 1

#include <string>

#include "Event/PackedProtoParticle.h"
#include "Event/ProtoParticle.h"
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

/** @class UnpackProtoParticle UnpackProtoParticle.h
 *  Unpack a ProtoParticle
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
class UnpackProtoParticle : public GaudiAlgorithm
{

public:

  /// Standard constructor
  UnpackProtoParticle( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override;    ///< Algorithm execution

private:

  std::string m_inputName;
  std::string m_outputName;
  bool m_alwaysOutput;      ///< Flag to turn on the creation of output, even when input is missing

};

#endif // UNPACKPROTOPARTICLE_H
