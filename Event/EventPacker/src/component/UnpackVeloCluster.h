#ifndef UNPACKVELOCLUSTER_H
#define UNPACKVELOCLUSTER_H 1

#include "Event/PackedCluster.h"

#include "GaudiAlg/GaudiAlgorithm.h"

/** @class UnpackVeloCluster UnpackVeloCluster.h
 *
 *  Unpack the LHCb::VeloCluster objects from a specified container.
 *
 *  This is a single-container-type version of UnpackCluster, and assumes a
 *  single input -> output mapping.
 */
class UnpackVeloCluster : public GaudiAlgorithm
{

public:

  UnpackVeloCluster(const std::string& name, ISvcLocator* svcLocator);

  StatusCode execute() override;

private:

  Gaudi::Property<std::string> m_inputName{this, "InputName", LHCb::PackedClusterLocation::Velo};

  Gaudi::Property<std::string> m_outputName{this, "OutputName", LHCb::VeloClusterLocation::Default};

  /// Flag to turn on the creation of output, even when input is missing
  Gaudi::Property<bool> m_alwaysOutput{this, "AlwaysCreateOutput", false};
};

#endif // UNPACKVELOCLUSTER_H
