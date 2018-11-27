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
#ifndef RAWBANKTOSTCLUSTERALG_H
#define RAWBANKTOSTCLUSTERALG_H 1

#include "STDecodingBaseAlg.h"
#include "Event/RawBank.h"
#include "Kernel/STDAQDefinitions.h"
#include "GaudiAlg/Transformer.h"
#include "Kernel/STLExtensions.h"

#include "Event/STSummary.h"
#include "Event/STCluster.h"

#include <boost/container/small_vector.hpp>
#include <vector>
#include <string>

/** @class RawBankToSTClusterAlg RawBankToSTClusterAlg.h
 *
 *  Algorithm to create STClusters from RawEvent object
 *
 *  @author M. Needham
 *  @date   2004-01-07
 */


#include "Kernel/STClusterWord.h"

class SiADCWord;
class STTell1Board;

namespace LHCb{
 class STChannelID;
 class STLiteCluster;
}

typedef Gaudi::Functional::MultiTransformer<std::tuple<LHCb::STClusters,LHCb::STSummary>(const LHCb::ODIN&, const LHCb::RawEvent&),
  Gaudi::Functional::Traits::BaseClass_t<STDecodingBaseAlg>> RawBankToSTClusterAlgBaseClass;

class RawBankToSTClusterAlg : public RawBankToSTClusterAlgBaseClass {

public:

  /// Standard constructor
  RawBankToSTClusterAlg( const std::string& name, ISvcLocator* pSvcLocator );

  /// initialize
  StatusCode initialize() override;
  /// finalize
  StatusCode finalize() override;
  /// Algorithm execution
  std::tuple<LHCb::STClusters,LHCb::STSummary> operator()(const LHCb::ODIN&, const LHCb::RawEvent&) const override;

private:

  LHCb::STSummary decodeBanks(const LHCb::RawEvent& rawEvt,
                              LHCb::STClusters& clusCont) const;

  void createCluster(const STClusterWord& aWord,
                     const STTell1Board* aBoard,
                     LHCb::span<const SiADCWord> adcValues,
                     const STDAQ::version& bankVersion,
                     LHCb::STClusters& clusCont) const;

  double mean(LHCb::span<const SiADCWord> adcValues) const;

  LHCb::STLiteCluster word2LiteCluster(const STClusterWord aWord,
				       const LHCb::STChannelID chan,
				       const unsigned int fracStrip) const;

  LHCb::STSummary createSummaryBlock(const LHCb::RawEvent& rawEvt,
                                     const unsigned int& nclus,
                                     const unsigned int& pcn,
                                     const bool pcnsync,
                                     const unsigned int bytes,
                                     const std::vector<unsigned int>& bankList,
                                     const std::vector<unsigned int>& missing,
                                     const LHCb::STSummary::RecoveredInfo& recoveredBanks) const;

  double stripFraction(const double interStripPos) const;

  std::string m_pedestalBankString;
  LHCb::RawBank::BankType m_pedestalType;

  std::string m_fullBankString;
  LHCb::RawBank::BankType m_fullType;

  unsigned int m_nBits;

  mutable Gaudi::Accumulators::AveragingCounter<unsigned int> m_lostBanks{ this, "lost Banks" };
  mutable Gaudi::Accumulators::Counter<> m_noBanksFound{ this, "no banks found" };
  mutable Gaudi::Accumulators::Counter<> m_banksFound{ this, "# banks found" };
  mutable Gaudi::Accumulators::AveragingCounter<unsigned int> m_skippedBanks{ this, "skipped Banks" };
  mutable Gaudi::Accumulators::Counter<> m_validBanks{ this, "# valid banks" };
  mutable Gaudi::Accumulators::Counter<> m_validSourceID{ this, "# valid source ID" };

};

#include "Event/STLiteCluster.h"
#include "Kernel/STChannelID.h"

inline LHCb::STLiteCluster RawBankToSTClusterAlg::word2LiteCluster(const STClusterWord aWord,
                                                                   const LHCb::STChannelID chan,
                                                                   const unsigned int fracStrip) const
{
  return LHCb::STLiteCluster(fracStrip,aWord.pseudoSizeBits(),aWord.hasHighThreshold(), chan, (detType()=="UT"));
}

#endif // RAWBUFFERTOSTDIGITALG_H
