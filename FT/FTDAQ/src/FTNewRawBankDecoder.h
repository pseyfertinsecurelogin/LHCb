#ifndef FTNEWRAWBANKDECODER_H
#define FTNEWRAWBANKDECODER_H 1

// Include files
// from Gaudi
#include "Event/FTLiteCluster.h"
#include "GaudiAlg/Transformer.h"
#include "GaudiAlg/FunctionalUtilities.h"

#include "IFTReadoutTool.h"

using namespace Gaudi::Functional;

using FTLiteClusters = FastClusterContainer<LHCb::FTLiteCluster,int>;

/** @class FTNewRawBankDecoder FTNewRawBankDecoder.h
 *  Decode the FT raw bank into FTLiteClusters
 *
 *  @author Louis Henry
 *  @date   2018-02-25
 */
class FTNewRawBankDecoder : public Transformer< FTLiteClusters( const LHCb::RawEvent& ) >
{
 public:
  /// Standard constructor
  FTNewRawBankDecoder( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;
  
  FTLiteClusters operator()(const LHCb::RawEvent& rawEvent) const override;

 private:
  IFTReadoutTool* m_ftReadoutTool = nullptr;
  // for MC, following property has to be same as cluster creator, 
  // not sure how to ensure this (TODO)
  Gaudi::Property<unsigned int> m_clusterMaxWidth{ this, "ClusterMaxWidth", 4,
      "Maximal cluster width"};

};
#endif // FTNEWRAWBANKDECODER_H
