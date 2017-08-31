#ifndef UNPACKSTCLUSTER_H
#define UNPACKSTCLUSTER_H 1

#include "Event/PackedCluster.h"

#include "GaudiAlg/GaudiAlgorithm.h"

/** @class UnpackSTCluster UnpackSTCluster.h
 *
 *  Unpack the LHCb::STCluster objects from a specified container.
 *
 *  This is a single-container-type version of UnpackCluster, and assumes a
 *  single input -> output mapping.
 */
class UnpackSTCluster : public GaudiAlgorithm
{

public:

  UnpackSTCluster(const std::string& name, ISvcLocator* svcLocator);

  StatusCode execute() override;

private:

  Gaudi::Property<std::string> m_inputName{this, "InputName", LHCb::PackedClusterLocation::TT};

  Gaudi::Property<std::string> m_outputName{this, "OutputName", LHCb::STClusterLocation::TTClusters};

  /// Flag to turn on the creation of output, even when input is missing
  Gaudi::Property<bool> m_alwaysOutput{this, "AlwaysCreateOutput", false};
};

#endif // UNPACKSTCLUSTER_H
