#ifndef L0CONDDBPROVIDER_H
#define L0CONDDBPROVIDER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
// from LHCb
#include "CaloDet/DeCalorimeter.h"
#include "L0Interfaces/IL0CondDBProvider.h"
/** @class L0CondDBProvider L0CondDBProvider.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2007-10-13
 */
class L0CondDBProvider : public extends<GaudiTool, IL0CondDBProvider> {
public:
  /// Standard constructor
  using extends<GaudiTool, IL0CondDBProvider>::extends;

  StatusCode initialize() override;

  double scale(unsigned int base) const override;
  double caloEtScale() const override;
  double muonPtScale() const override;
  const std::vector<int> RAMBCID(std::string vsn) const override;
  int RAMBCID(std::string vsn,int bcid) const override;

private:
  const DeCalorimeter* m_ecal = nullptr;
  const Condition* m_gain = nullptr;
  Gaudi::Property<std::map<std::string,std::vector<int>>> m_mapRam { this, "RAMBCID" };
  unsigned int m_cycle = 3564;
  std::string m_rams;

  mutable Gaudi::Accumulators::Counter<> m_unknownScale{ this, "Unknown scale type" };
  mutable Gaudi::Accumulators::Counter<> m_gainNotFound{ this, "'Gain' condition not found for Ecal" };
  mutable Gaudi::Accumulators::Counter<> m_L0EtBinNotFound{ this, "'L0EtBin' parameter not found in 'Gain' condition" };

};
#endif // L0CONDDBPROVIDER_H
