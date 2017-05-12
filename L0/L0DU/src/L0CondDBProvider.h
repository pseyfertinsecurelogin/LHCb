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
  L0CondDBProvider( const std::string& type,
            const std::string& name,
            const IInterface* parent);

  StatusCode initialize() override;
  double scale(unsigned int base) override;
  double caloEtScale() override;
  double muonPtScale() override;
  const std::vector<int> RAMBCID(std::string vsn) override;
  int RAMBCID(std::string vsn,int bcid) override;

private:
  DeCalorimeter* m_ecal = nullptr;
  Condition* m_gain = nullptr;
  std::map<std::string,std::vector<int> > m_mapRam;
  unsigned int m_cycle = 3564;
  std::string m_rams;
};
#endif // L0CONDDBPROVIDER_H
