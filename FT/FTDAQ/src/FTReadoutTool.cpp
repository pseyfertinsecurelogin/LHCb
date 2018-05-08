// FTDAQ
#include "FTReadoutTool.h"

#include "DetDesc/Condition.h"
//#include "DetDesc/ConditionInfo.h"

using namespace LHCb;

DECLARE_COMPONENT( FTReadoutTool )

StatusCode FTReadoutTool::initialize(){
  StatusCode sc = base_class::initialize();
  if (sc.isFailure()){
    return Error("Failed to initialize", sc);
  }
  
  registerCondition( m_conditionLocation,
                     &FTReadoutTool::readFile);
  sc = runUpdate();
  if (sc.isFailure()) return Error ( "Failed first UMS update for readout tool", sc );
  return StatusCode::SUCCESS;
}

// Build a bank number from ID and conversely
LHCb::FTChannelID FTReadoutTool::channelIDShift(unsigned int bankNumber) const
{
  return m_FTTell40FirstChannel[ bankNumber ];
}

unsigned int FTReadoutTool::bankNumber(LHCb::FTChannelID id) const
{
  auto it = std::find_if( m_FTTell40FirstChannel.begin(), m_FTTell40FirstChannel.end(),
                         [&id](const auto& firstChan){ return id < firstChan; });
  return it - m_FTTell40FirstChannel.begin() - 1u;
}

StatusCode FTReadoutTool::readFile()
{
  Condition* rInfo = getDet<Condition>(m_conditionLocation);
  
  std::vector<int> stations     = rInfo->param<std::vector<int> >("FTTell40Station");
  std::vector<int> layers       = rInfo->param<std::vector<int> >("FTTell40Layer");
  std::vector<int> quarters     = rInfo->param<std::vector<int> >("FTTell40Quarter");
  std::vector<int> firstModules = rInfo->param<std::vector<int> >("FTTell40FirstModule");
  std::vector<int> firstMats    = rInfo->param<std::vector<int> >("FTTell40FirstMat");

  //Construct the first channel attribute
  unsigned int nTell40s = stations.size();
  m_FTTell40FirstChannel.reserve(nTell40s);
  for (unsigned int i = 0 ; i < nTell40s ; i++) {
    m_FTTell40FirstChannel.emplace_back(stations[i],
                                        layers[i],
                                        quarters[i],
                                        firstModules[i],
                                        firstMats[i], 0u, 0u );
  }

  return StatusCode::SUCCESS;
}
