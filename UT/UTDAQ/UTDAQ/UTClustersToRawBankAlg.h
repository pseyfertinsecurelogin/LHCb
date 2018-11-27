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
#ifndef UTClustersToRawBankAlg_H
#define UTClustersToRawBankAlg_H 1

// from UTL
#include <string>
#include <vector>
#include <map>

// gaudi
#include "Kernel/UTCommonBase.h"

// event
#include "Event/RawEvent.h"
#include "Event/UTCluster.h"
#include "UTClustersOnBoard.h"

#include "Kernel/UTDAQDefinitions.h"
#include "Event/RawBank.h"
#include "Event/BankWriter.h"
#include "Kernel/IUTReadoutTool.h"

/** @class UTClustersToRawBankAlg UTClustersToRawBankAlg.h
 *
 *  Algorithm to fill the Raw buffer with UT information from UTClusters
 *
 *  @author A Beiter (based on code by M Needham)
 *  @date   2018-09-04
 */

class UTTell1ID;
class IUTDAQDataSvc;

#include "UTBoardToBankMap.h"
namespace LHCb {class RawBank;}

template <class IReadoutTool = IUTReadoutTool>
class UTClustersToRawBankAlgT : public UT::CommonBase<GaudiAlgorithm, IReadoutTool> {

public:

  /// Standard constructor
  UTClustersToRawBankAlgT(const std::string& name, ISvcLocator* pSvcLocator);

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute() override;       ///< Algorithm execution
  StatusCode finalize() override;      ///< Algorithm finalization

private:

  /// convert string to enum
  StatusCode configureBankType();

  /// initialize event
  void initEvent();

  /// fill the banks
  StatusCode groupByBoard(const LHCb::UTClusters* clusCont);

  unsigned int bankSize(UTClustersOnBoard::ClusterVector& clusCont) const;

  unsigned int getPCN() const;

  // create a new bank
  void writeBank(const UTClustersOnBoard::ClusterVector& clusCont ,
                 LHCb::BankWriter& bWriter,
                 const UTTell1ID aBoardID);

  Gaudi::Property<std::string> m_rawLocation{this, "rawLocation", LHCb::RawEventLocation::Default};
  Gaudi::Property<int> m_maxClustersPerPPx{this, "maxClusters", 512};

  std::string m_clusterLocation;
  std::string m_summaryLocation;

  LHCb::RawBank::BankType m_bankType;

  UTBoardToBankMap m_bankMapping;

  std::map<UTTell1ID,UTClustersOnBoard* > m_clusMap;
  std::vector<UTClustersOnBoard> m_clusVectors;

  unsigned int m_overflow = 0;
  unsigned int m_maxClusterSize = 4;
  unsigned int m_pcn = 128;

};

// Declaration of the backward compatible UTClustersToRawBankAlg class (not templated for the original UT case)
using UTClustersToRawBankAlg = UTClustersToRawBankAlgT<>;

#endif // UTClustersToRawBankAlg
