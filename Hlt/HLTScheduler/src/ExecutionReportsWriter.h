/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

//as this one also includes a parser, lets put that first
#include "HLTControlFlowMgr.h"

#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/EventContext.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiAlg/FunctionalDetails.h"

#include <type_traits>
#include <vector>
#include <string>

class ExecutionReportsWriter : public Gaudi::Algorithm
{
  using Gaudi::Algorithm::Algorithm;

public:
  /// the execution of the algorithm
  StatusCode execute(EventContext const & evtCtx) const override;
  StatusCode start() override;

private:

  HLTControlFlowMgr* m_schedulerPtr = nullptr;

  Gaudi::Property<int> m_printFreq{this, "PrintFreq", 1000, "Print Frequency for states"};
  DataObjectWriteHandle<std::vector<NodeState>> m_outputHandle{this, "DecReportsLocation", std::string("/Event/DecReport")};
};
