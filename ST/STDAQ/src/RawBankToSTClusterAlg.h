#ifndef RAWBANKTOSTCLUSTERALG_H 
#define RAWBANKTOSTCLUSTERALG_H 1

#include <string>
#include <vector>

#include "Event/RawBank.h"
#include "Event/STCluster.h"
#include "GaudiKernel/StatusCode.h"
#include "Kernel/STDAQDefinitions.h"
#include "STDecodingBaseAlg.h"

/** @class RawBankToSTClusterAlg RawBankToSTClusterAlg.h
 *  
 *  Algorithm to create STClusters from RawEvent object
 * 
 *  @author M. Needham
 *  @date   2004-01-07
 */


#include "Kernel/STClusterWord.h"

class STTell1Board;
class SiADCWord;

namespace LHCb{
 class STChannelID;
 class STLiteCluster;
class RawEvent;
}

class RawBankToSTClusterAlg : public STDecodingBaseAlg {

public:

  /// Standard constructor
  RawBankToSTClusterAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override; ///< finalize


private:

  StatusCode decodeBanks(const LHCb::RawEvent& rawEvt, LHCb::STClusters* digitCont ) const;


  void createCluster(const STClusterWord& aWord,
                     const STTell1Board* aBoard,
                     const std::vector<SiADCWord>& adcValues,
                     const STDAQ::version& bankVersion,
                     LHCb::STClusters* clusCont) const;
 
  double mean(const std::vector<SiADCWord>& adcValues) const;
   
  LHCb::STLiteCluster word2LiteCluster(const STClusterWord aWord, 
				       const LHCb::STChannelID chan,
				       const unsigned int fracStrip) const;

    
  double stripFraction(const double interStripPos) const;

  /// Output location for STClusters
  std::string m_clusterLocation;

  unsigned int m_nBits; 


};

#include "Event/STLiteCluster.h"
#include "Kernel/STChannelID.h"

class ISvcLocator;

inline LHCb::STLiteCluster RawBankToSTClusterAlg::word2LiteCluster(const STClusterWord aWord,
                                                             const LHCb::STChannelID chan,
                                                             const unsigned int fracStrip) const
{
  return LHCb::STLiteCluster(fracStrip,aWord.pseudoSizeBits(),aWord.hasHighThreshold(), chan, (detType()=="UT"));
}

#endif // RAWBUFFERTOSTDIGITALG_H 
