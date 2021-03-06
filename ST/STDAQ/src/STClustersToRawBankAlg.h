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
#ifndef STClustersToRawBankAlg_H
#define STClustersToRawBankAlg_H 1

// from STL
#include <map>
#include <string>
#include <vector>

// gaudi
#include "Kernel/STCommonBase.h"

// event
#include "Event/RawEvent.h"
#include "Event/STCluster.h"
#include "STClustersOnBoard.h"

#include "Event/BankWriter.h"
#include "Event/RawBank.h"
#include "Kernel/ISTReadoutTool.h"
#include "Kernel/STDAQDefinitions.h"

/** @class STClustersToRawBankAlg STClustersToRawBankAlg.h
 *
 *  Algorithm to fill the Raw buffer with ST information from STClusters
 *
 *  @author M Needham
 *  @date   2004-08-01
 */

class STTell1ID;
class ISTDAQDataSvc;

#include "STBoardToBankMap.h"
namespace LHCb {
  class RawBank;
}

template <class IReadoutTool = ISTReadoutTool>
class STClustersToRawBankAlgT : public ST::CommonBase<GaudiAlgorithm, IReadoutTool> {

public:
  /// Standard constructor
  STClustersToRawBankAlgT( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

private:
  /// convert string to enum
  StatusCode configureBankType();

  /// initialize event
  void initEvent();

  /// fill the banks
  StatusCode groupByBoard( const LHCb::STClusters* clusCont );

  unsigned int bankSize( STClustersOnBoard::ClusterVector& clusCont ) const;

  unsigned int getPCN() const;

  // create a new bank
  void writeBank( const STClustersOnBoard::ClusterVector& clusCont, LHCb::BankWriter& bWriter,
                  const STTell1ID aBoardID );

  Gaudi::Property<std::string> m_rawLocation{this, "rawLocation", LHCb::RawEventLocation::Default};
  Gaudi::Property<int>         m_maxClustersPerPPx{this, "maxClusters", 512};

  std::string m_clusterLocation;
  std::string m_summaryLocation;

  LHCb::RawBank::BankType m_bankType;

  STBoardToBankMap m_bankMapping;

  std::map<STTell1ID, STClustersOnBoard*> m_clusMap;
  std::vector<STClustersOnBoard>          m_clusVectors;

  unsigned int m_overflow       = 0;
  unsigned int m_maxClusterSize = 4;
  unsigned int m_pcn            = 128;
};

// Declaration of the backward compatible STClustersToRawBankAlg class (not templated for the original ST case)
using STClustersToRawBankAlg = STClustersToRawBankAlgT<>;

#endif // STClustersToRawBankAlg
