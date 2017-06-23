#ifndef RAWBANKTOSTCLUSTERALG_H 
#define RAWBANKTOSTCLUSTERALG_H 1

#include "STDecodingBaseAlg.h"
#include "Event/RawBank.h"
#include "Kernel/STDAQDefinitions.h"
#include "GaudiAlg/Transformer.h"

#include "Event/STSummary.h"
#include "Event/STCluster.h"

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
                     const std::vector<SiADCWord>& adcValues,
                     const STDAQ::version& bankVersion,
                     LHCb::STClusters& clusCont) const;
 
  double mean(const std::vector<SiADCWord>& adcValues) const;
   
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
