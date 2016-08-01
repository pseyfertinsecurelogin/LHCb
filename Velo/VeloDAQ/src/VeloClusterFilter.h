#ifndef VELOCLUSTERFILTER_H 
#define VELOCLUSTERFILTER_H 1

#include "Kernel/VeloChannelID.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/AnyDataHandle.h"
#include "GaudiKernel/DataObjectHandle.h"
#include <string>

class DeVelo;

///< A class to filter Velo clusters according to some defined criterion
///< Accepted filters are:
///<    "Right": retains only clusters in the Velo Right
///<    "Left" : retains only clusters in the Velo Left
///<    "R"    : retains only clusters in R sensors
///<    "Phi"  : retains only clusters in Phi sensors
///<    "PU"  : retains only clusters in PU sensors

class VeloClusterFilter : public GaudiAlgorithm {
public:
  VeloClusterFilter( const std::string& name, ISvcLocator* pSvcLocator );

  virtual StatusCode initialize();
  virtual StatusCode execute   ();

  bool passesFilter(LHCb::VeloChannelID id);
  void incrementCounters(LHCb::VeloChannelID, int&, int&, int&);

protected:
  std::string m_filterCriterion;

  AnyDataHandle<LHCb::VeloLiteCluster::FastContainer> m_inputLiteClusterDh = {LHCb::VeloLiteClusterLocation::Default, Gaudi::DataHandle::Reader, this };
  AnyDataHandle<LHCb::VeloLiteCluster::FastContainer> m_outputLiteClusterDh = {"/Event/Raw/Velo/LiteClustersCopy", Gaudi::DataHandle::Writer, this };
  DataObjectHandle<LHCb::VeloClusters> m_inputClusterDh = { LHCb::VeloLiteClusterLocation::Default , Gaudi::DataHandle::Writer, this };
  DataObjectHandle<LHCb::VeloClusters> m_outputClusterDh = {"/Event/Raw/Velo/ClustersCopy" , Gaudi::DataHandle::Writer, this };

  int m_minNRClustersCut;
  int m_minNPhiClustersCut;
  int m_minNClustersCut;
  int m_maxNRClustersCut;
  int m_maxNPhiClustersCut;
  int m_maxNClustersCut;
 private:
  DeVelo* m_velo = nullptr;                  ///< Detector element
};
#endif // VELOCLUSTERFILTER_H
