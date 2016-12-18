#ifndef VELOCLUSTERFILTER_H
#define VELOCLUSTERFILTER_H 1

#include "Kernel/VeloChannelID.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include <string>

class DeVelo;

///< A class to filter Velo clusters according to some defined criterion
///< Accepted filters are:
///<   "Right" : retains only clusters in the Velo Right
///<   "Left"  : retains only clusters in the Velo Left
///<   "R"     : retains only clusters in R sensors
///<   "Phi"   : retains only clusters in Phi sensors
///<   "PU"    : retains only clusters in PU sensors

class VeloClusterFilter : public GaudiAlgorithm {
public:
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;
  StatusCode execute()    override;

  class filter_t {
  public:
     enum class criterion_t { ALL = 0, LEFT, RIGHT, R, PHI, PU, OVERLAP };
     filter_t(criterion_t c = criterion_t::ALL) : m_criterion(c) {}
     bool operator()(LHCb::VeloChannelID id) const;
     const std::string& toString() const;
     // add support for Gaudi::Property<filter_t>
     friend std::ostream& toStream(const filter_t& crit, std::ostream& os);
     friend StatusCode parse(filter_t& result, const std::string& input );
  private:
     criterion_t m_criterion;
  };

private:
  Gaudi::Property<std::string> m_inputLiteClusterLocation{ this, "InputLiteClusterLocation", LHCb::VeloLiteClusterLocation::Default };
  Gaudi::Property<std::string> m_outputLiteClusterLocation{ this, "OutputLiteClusterLocation", "/Event/Raw/Velo/LiteClustersCopy" };
  Gaudi::Property<std::string> m_inputClusterLocation{ this, "InputClusterLocation", LHCb::VeloClusterLocation::Default };
  Gaudi::Property<std::string> m_outputClusterLocation{ this, "OutputClusterLocation", "/Event/Raw/Velo/ClustersCopy" };
  Gaudi::Property<filter_t> m_filter{ this, "FilterOption", filter_t::criterion_t::ALL };
  Gaudi::Property<int> m_minNRClustersCut{ this, "MinimumNumberOfRClusters",0 };
  Gaudi::Property<int> m_minNPhiClustersCut{ this, "MinimumNumberOfPhiClusters",0 };
  Gaudi::Property<int> m_minNClustersCut{ this, "MinimumNumberOfClusters",0 };
  Gaudi::Property<int> m_maxNRClustersCut{ this, "MaximumNumberOfRClusters",100000 };
  Gaudi::Property<int> m_maxNPhiClustersCut{ this, "MaximumNumberOfPhiClusters",100000 };
  Gaudi::Property<int> m_maxNClustersCut{ this, "MaximumNumberOfClusters",100000 };

  DeVelo* m_velo = nullptr;                  ///< Detector element
};
#endif // VELOCLUSTERFILTER_H
