/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
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
