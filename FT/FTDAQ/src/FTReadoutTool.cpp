/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// FTDAQ
#include "FTReadoutTool.h"

#include "DetDesc/Condition.h"

using namespace LHCb;

DECLARE_COMPONENT( FTReadoutTool )

StatusCode FTReadoutTool::initialize(){
  if( ! m_initialized ) {
    StatusCode sc = base_class::initialize();
    if (sc.isFailure()){
      return Error("Failed to initialize", sc);
    }

    registerCondition( m_conditionLocation, &FTReadoutTool::readFile);
    sc = runUpdate();
    if (sc.isFailure()) return Error ( "Failed first UMS update for readout tool", sc );
    m_initialized = true;
  }
  return StatusCode::SUCCESS;
}

// Build a bank number from ID and conversely
LHCb::FTChannelID FTReadoutTool::channelIDShift(unsigned int bankNumber) const
{
  return m_FTBankFirstChannel[ bankNumber ];
}

unsigned int FTReadoutTool::bankNumber(LHCb::FTChannelID id) const
{
  auto it = std::find_if( m_FTBankFirstChannel.begin(), m_FTBankFirstChannel.end(),
                         [&id](const auto& firstChan){ return id < firstChan; });
  return it - m_FTBankFirstChannel.begin() - 1u;
}

StatusCode FTReadoutTool::readFile()
{
  Condition* rInfo = getDet<Condition>(m_conditionLocation);
  
  std::vector<int> stations     = rInfo->param<std::vector<int> >("FTBankStation");
  std::vector<int> layers       = rInfo->param<std::vector<int> >("FTBankLayer");
  std::vector<int> quarters     = rInfo->param<std::vector<int> >("FTBankQuarter");
  std::vector<int> firstModules = rInfo->param<std::vector<int> >("FTBankFirstModule");
  std::vector<int> firstMats    = rInfo->param<std::vector<int> >("FTBankFirstMat");

  //Construct the first channel attribute
  unsigned int nTell40s = stations.size();
  m_FTBankFirstChannel.reserve(nTell40s);
  for (unsigned int i = 0 ; i < nTell40s ; i++) {
    m_FTBankFirstChannel.emplace_back(stations[i],
                                        layers[i],
                                        quarters[i],
                                        firstModules[i],
                                        firstMats[i], 0u, 0u );
  }
  return StatusCode::SUCCESS;
}
