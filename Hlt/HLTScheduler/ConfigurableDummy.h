#include <memory>

#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/RegistryEntry.h"
#include "GaudiKernel/DataObjectHandle.h"


class ConfigurableDummy : public Algorithm
{
  using Algorithm::Algorithm;

public:
  /// the execution of the algorithm
  StatusCode execute() override;
  /// Its initialization
  StatusCode initialize() override;

private:

  /// Pick up late-attributed data outputs
  Gaudi::Property<bool> m_CFD{this, "CFD", true, "ControlFlowDecision"};

  Gaudi::Property<std::vector<std::string>> m_inpKeys{this, "inpKeys", {}, ""};
  Gaudi::Property<std::vector<std::string>> m_outKeys{this, "outKeys", {}, ""};


  std::vector<std::unique_ptr<DataObjectReadHandle<DataObject>>> m_inputHandles;
  std::vector<std::unique_ptr<DataObjectWriteHandle<DataObject>>> m_outputHandles;

};
