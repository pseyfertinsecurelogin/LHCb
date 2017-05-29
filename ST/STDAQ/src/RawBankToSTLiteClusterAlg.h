#ifndef RAWBANKTOSTLITECLUSTERALG_H
#define RAWBANKTOSTLITECLUSTERALG_H 1

#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "Event/RawBank.h"
#include "Event/STLiteCluster.h"
#include "GaudiKernel/StatusCode.h"
#include "Kernel/STChannelID.h"
#include "Kernel/STDAQDefinitions.h"
#include "Kernel/StripRepresentation.h"
#include "STDecodingBaseAlg.h"

/** @class RawBankToSTLiteClusterAlg RawBankToSTLiteClusterAlg.h
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

class RawBankToSTLiteClusterAlg : public STDecodingBaseAlg {

public:

  /// Standard constructor
  RawBankToSTLiteClusterAlg( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~RawBankToSTLiteClusterAlg( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override; ///< finalize

private:

  // create Clusters from this type
  StatusCode decodeBanks(LHCb::RawEvent* rawEvt, LHCb::STLiteCluster::STLiteClusters* fCont) const;

  // add a single cluster to the output container
  void createCluster(const STTell1Board* aBoard,  const STDAQ::version& bankVersion,
                     const STClusterWord& aWord, LHCb::STLiteCluster::STLiteClusters* fCont, const bool isUT) const;


  std::string m_clusterLocation;

  class Less_by_Channel : public std::binary_function<LHCb::STLiteCluster,LHCb::STLiteCluster ,bool>{
  public:

    /** compare the channel of one object with the
     *  channel of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    //
    inline bool operator() ( LHCb::STLiteCluster obj1 , LHCb::STLiteCluster obj2 ) const
    {
      return obj1.channelID() < obj2.channelID() ;
    }
  };


  class Equal_Channel : public std::binary_function<LHCb::STLiteCluster,LHCb::STLiteCluster ,bool>{
  public:

    /** compare the channel of one object with the
     *  channel of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    //
    inline bool operator() ( LHCb::STLiteCluster obj1 , LHCb::STLiteCluster obj2 ) const
    {
      return obj1.channelID() == obj2.channelID() ;
    }
  };


};

#include "Kernel/ISTReadoutTool.h"
#include "Kernel/STTell1Board.h"

class ISvcLocator;

inline void RawBankToSTLiteClusterAlg::createCluster(const STTell1Board* aBoard,  const STDAQ::version& bankVersion,
                                                     const STClusterWord& aWord, LHCb::STLiteCluster::STLiteClusters* fCont,
                                                     const bool isUT) const{

  const unsigned int fracStrip = aWord.fracStripBits();
  const STTell1Board::chanPair chan = aBoard->DAQToOffline(fracStrip, bankVersion, STDAQ::StripRepresentation(aWord.channelID()));
  LHCb::STLiteCluster liteCluster(chan.second,
                            aWord.pseudoSizeBits(),
                            aWord.hasHighThreshold(),
                            chan.first,
                            isUT);
  fCont->push_back(std::move(liteCluster));
}

#endif //  RAWBANKTOSTLITECLUSTERALG_H
