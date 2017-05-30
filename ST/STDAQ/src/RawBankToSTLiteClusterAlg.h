#ifndef RAWBANKTOSTLITECLUSTERALG_H
#define RAWBANKTOSTLITECLUSTERALG_H 1

#include "STDecodingBaseAlg.h"
#include "Event/RawBank.h"
#include "Kernel/STDAQDefinitions.h"

#include "Event/STLiteCluster.h"

#include <vector>
#include <string>
#include <utility>

/** @class RawBankToSTLiteClusterAlg RawBankToSTLiteClusterAlg.h
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

class RawBankToSTLiteClusterAlg : public STDecodingBaseAlg {

public:

  /// Standard constructor
  RawBankToSTLiteClusterAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override; ///< finalize

private:

  // create Clusters from this type
  StatusCode decodeBanks(LHCb::RawEvent* rawEvt, LHCb::STLiteCluster::STLiteClusters* fCont) const;

  // add a single cluster to the output container
  void createCluster(const STTell1Board* aBoard,  const STDAQ::version& bankVersion,
                     const STClusterWord& aWord, LHCb::STLiteCluster::STLiteClusters& fCont, const bool isUT) const;


  std::string m_clusterLocation;
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
