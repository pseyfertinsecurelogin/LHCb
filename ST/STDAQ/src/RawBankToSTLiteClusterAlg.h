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
#ifndef RAWBANKTOSTLITECLUSTERALG_H
#define RAWBANKTOSTLITECLUSTERALG_H 1

#include "STDecodingBaseAlg.h"
#include "Event/RawBank.h"
#include "Kernel/STDAQDefinitions.h"
#include "GaudiAlg/Transformer.h"

#include "Event/STLiteCluster.h"

#include <vector>
#include <string>
#include <utility>

/** @class RawBankToSTLiteClusterAlg RawBankToSTLiteClusterAlg.h
 *
 *  Algorithm to create STClusters from RawEvent object
 *
 *  @author M. Needham
 *  @author S. Ponce
 */


#include "Kernel/STClusterWord.h"

class SiADCWord;
class STTell1Board;

namespace LHCb{
 class STChannelID;
 class STLiteCluster;
}

typedef Gaudi::Functional::Transformer<LHCb::STLiteCluster::STLiteClusters(const LHCb::ODIN&, const LHCb::RawEvent&),
  Gaudi::Functional::Traits::BaseClass_t<STDecodingBaseAlg>> RawBankToSTLiteClusterAlgBaseClass;

class RawBankToSTLiteClusterAlg final : public RawBankToSTLiteClusterAlgBaseClass {

public:

  /// Standard constructor
  RawBankToSTLiteClusterAlg(const std::string& name, ISvcLocator* pSvcLocator);

  StatusCode initialize() override;   ///< Algorithm initialization
  StatusCode finalize() override;     ///< Algorithm finalization
  LHCb::STLiteCluster::STLiteClusters operator()(const LHCb::ODIN&, const LHCb::RawEvent&) const override;

private:

  // create Clusters from this type
  StatusCode decodeBanks(const LHCb::RawEvent& rawEvt, LHCb::STLiteCluster::STLiteClusters& fCont) const;

  // add a single cluster to the output container
  void createCluster(const STTell1Board* aBoard,  const STDAQ::version& bankVersion,
                     const STClusterWord& aWord, LHCb::STLiteCluster::STLiteClusters& fCont, const bool isUT) const;

  mutable Gaudi::Accumulators::AveragingCounter<unsigned int> m_lostBanks{ this, "lost Banks" };
  mutable Gaudi::Accumulators::Counter<> m_noBanksFound{ this, "no banks found" };
  mutable Gaudi::Accumulators::AveragingCounter<unsigned int> m_skippedBanks{ this, "skipped Banks" };
  mutable Gaudi::Accumulators::Counter<> m_validBanks{ this, "# valid banks" };
  mutable Gaudi::Accumulators::Counter<> m_validSourceID{ this, "# valid source ID" };
};

#include "Kernel/STTell1Board.h"
#include "Kernel/ISTReadoutTool.h"

inline void RawBankToSTLiteClusterAlg::createCluster(const STTell1Board* aBoard,  const STDAQ::version& bankVersion,
                                                     const STClusterWord& aWord, LHCb::STLiteCluster::STLiteClusters& fCont,
                                                     const bool isUT) const{

  const unsigned int fracStrip = aWord.fracStripBits();
  const STTell1Board::chanPair chan = aBoard->DAQToOffline(fracStrip, bankVersion, STDAQ::StripRepresentation(aWord.channelID()));
  fCont.emplace_back( chan.second,
                      aWord.pseudoSizeBits(),
                      aWord.hasHighThreshold(),
                      chan.first,
                      isUT);
}

#endif //  RAWBANKTOSTLITECLUSTERALG_H
