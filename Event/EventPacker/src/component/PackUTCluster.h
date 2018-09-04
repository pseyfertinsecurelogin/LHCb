#ifndef PACKUTCLUSTER_H
#define PACKUTCLUSTER_H 1

#include "Event/PackedCluster.h"

#include "GaudiAlg/GaudiAlgorithm.h"

/** @class PackUTCluster PackUTCluster.h
 *
 *  Pack the LHCb::UTCluster objects from a specified container.
 */
class PackUTCluster : public GaudiAlgorithm
{

public:

  PackUTCluster(const std::string& name, ISvcLocator* svcLocator);

  StatusCode execute() override;

private:

  Gaudi::Property<std::string> m_inputName{this, "InputName", LHCb::UTClusterLocation::UTClusters};

  Gaudi::Property<std::string> m_outputName{this, "OutputName", LHCb::PackedClusterLocation::UT};

  /// Flag to turn on the removal of the input data after packing
  Gaudi::Property<bool> m_deleteInput{this, "DeleteInput", false};

  /// Flag to turn on the creation of output, even when input is missing
  Gaudi::Property<bool> m_alwaysOutput{this, "AlwaysCreateOutput", false};
};

#endif // PACKUTCLUSTER_H
