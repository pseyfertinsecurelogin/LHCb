#ifndef RAWBANKTOSTCLUSTERALG_H
#define RAWBANKTOSTCLUSTERALG_H 1

#include "UTDecodingBaseAlg.h"
#include "Event/RawBank.h"
#include "Kernel/UTDAQDefinitions.h"
#include "GaudiAlg/Transformer.h"
#include "Kernel/STLExtensions.h"

#include "Event/UTSummary.h"
#include "Event/UTCluster.h"

#include <boost/container/small_vector.hpp>
#include <vector>
#include <string>

/** @class RawBankToUTClusterAlg RawBankToUTClusterAlg.h
 *
 *  Algorithm to create UTClusters from RawEvent object
 *
 *  @author A Beiter (based on code by M. Needham)
 *  @date   2018-09-04
 */


#include "Kernel/UTClusterWord.h"

class SiADCWord;
class UTTell1Board;

namespace LHCb{
 class UTChannelID;
 class UTLiteCluster;
}

typedef Gaudi::Functional::MultiTransformer<std::tuple<LHCb::UTClusters,LHCb::UTSummary>(const LHCb::ODIN&, const LHCb::RawEvent&),
  Gaudi::Functional::Traits::BaseClass_t<UTDecodingBaseAlg>> RawBankToUTClusterAlgBaseClass;

class RawBankToUTClusterAlg : public RawBankToUTClusterAlgBaseClass {

public:

  /// Standard constructor
  RawBankToUTClusterAlg( const std::string& name, ISvcLocator* pSvcLocator );

  /// initialize
  StatusCode initialize() override;
  /// finalize
  StatusCode finalize() override;
  /// Algorithm execution
  std::tuple<LHCb::UTClusters,LHCb::UTSummary> operator()(const LHCb::ODIN&, const LHCb::RawEvent&) const override;

private:

  LHCb::UTSummary decodeBanks(const LHCb::RawEvent& rawEvt,
                              LHCb::UTClusters& clusCont) const;

  void createCluster(const UTClusterWord& aWord,
                     const UTTell1Board* aBoard,
                     LHCb::span<const SiADCWord> adcValues,
                     const UTDAQ::version& bankVersion,
                     LHCb::UTClusters& clusCont) const;

  double mean(LHCb::span<const SiADCWord> adcValues) const;

  LHCb::UTLiteCluster word2LiteCluster(const UTClusterWord aWord,
				       const LHCb::UTChannelID chan,
				       const unsigned int fracStrip) const;

  LHCb::UTSummary createSummaryBlock(const LHCb::RawEvent& rawEvt,
                                     const unsigned int& nclus,
                                     const unsigned int& pcn,
                                     const bool pcnsync,
                                     const unsigned int bytes,
                                     const std::vector<unsigned int>& bankList,
                                     const std::vector<unsigned int>& missing,
                                     const LHCb::UTSummary::RecoveredInfo& recoveredBanks) const;

  double stripFraction(const double interStripPos) const;

  std::string m_pedestalBankString;
  LHCb::RawBank::BankType m_pedestalType;

  std::string m_fullBankString;
  LHCb::RawBank::BankType m_fullType;

  unsigned int m_nBits;


};

#include "Event/UTLiteCluster.h"
#include "Kernel/UTChannelID.h"

inline LHCb::UTLiteCluster RawBankToUTClusterAlg::word2LiteCluster(const UTClusterWord aWord,
                                                                   const LHCb::UTChannelID chan,
                                                                   const unsigned int fracStrip) const
{
  return LHCb::UTLiteCluster(fracStrip,aWord.pseudoSizeBits(),aWord.hasHighThreshold(), chan, (detType()=="UT"));
}

#endif // RAWBUFFERTOSTDIGITALG_H
