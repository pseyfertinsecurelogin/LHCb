#ifndef PACKVELOCLUSTER_H
#define PACKVELOCLUSTER_H 1

#include "Event/PackedCluster.h"

#include "GaudiAlg/GaudiAlgorithm.h"

/** @class PackVeloCluster PackVeloCluster.h
 *
 *  Pack the LHCb::VeloCluster objects from a specified container.
 */
class PackVeloCluster : public GaudiAlgorithm
{

public:

  PackVeloCluster(const std::string& name, ISvcLocator* svcLocator);

  StatusCode execute() override;

private:

  Gaudi::Property<std::string> m_inputName{this, "InputName", LHCb::VeloClusterLocation::Default};

  Gaudi::Property<std::string> m_outputName{this, "OutputName", LHCb::PackedClusterLocation::Velo};

  /// Flag to turn on the removal of the input data after packing
  Gaudi::Property<bool> m_deleteInput{this, "DeleteInput", false};

  /// Flag to turn on the creation of output, even when input is missing
  Gaudi::Property<bool> m_alwaysOutput{this, "AlwaysCreateOutput", false};
};

#endif // PACKVELOCLUSTER_H
