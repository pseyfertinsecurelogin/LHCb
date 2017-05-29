#ifndef STClustersToRawBankAlg_H
#define STClustersToRawBankAlg_H 1

#include <map>
// from STL
#include <string>
#include <vector>

#include "Event/BankWriter.h"
#include "Event/RawBank.h"
// event
#include "Event/STCluster.h"
#include "GaudiKernel/StatusCode.h"
// gaudi
#include "Kernel/STAlgBase.h"
#include "Kernel/STDAQDefinitions.h"
#include "Kernel/STTell1ID.h"
#include "STClustersOnBoard.h"

/** @class STClustersToRawBankAlg STClustersToRawBankAlg.h
 *
 *  Algorithm to fill the Raw buffer with ST information from STClusters
 *
 *  @author M Needham
 *  @date   2004-08-01
 */

#include "STBoardToBankMap.h"

class ISTDAQDataSvc;
class ISvcLocator;
class STTell1ID;
namespace LHCb {
class BankWriter;
}  // namespace LHCb
namespace LHCb {class RawBank;}
struct ISTReadoutTool;


class STClustersToRawBankAlg : public ST::AlgBase {

public:

  /// Standard constructor
  STClustersToRawBankAlg(const std::string& name, ISvcLocator* pSvcLocator);

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute() override;       ///< Algorithm execution
  StatusCode finalize() override;      ///< Algorithm finalization

private:

  /// convert string to enum
  StatusCode configureBankType();

  /// initialize event
  void initEvent();

  /// fill the banks
  StatusCode groupByBoard(const LHCb::STClusters* clusCont);

  unsigned int bankSize(STClustersOnBoard::ClusterVector& clusCont) const;

  unsigned int getPCN() const;

  // create a new bank
  void writeBank(const STClustersOnBoard::ClusterVector& clusCont ,
                 LHCb::BankWriter& bWriter,
                 const STTell1ID aBoardID);

  std::string m_clusterLocation;
  std::string m_summaryLocation;
  std::string m_rawLocation;

  LHCb::RawBank::BankType m_bankType;

  STBoardToBankMap m_bankMapping;

  std::map<STTell1ID,STClustersOnBoard* > m_clusMap;
  std::vector<STClustersOnBoard> m_clusVectors;

  unsigned int m_overflow = 0;
  int m_maxClustersPerPPx;
  unsigned int m_maxClusterSize = 4;
  unsigned int m_pcn = 128;

};
#endif // STClustersToRawBankAlg
