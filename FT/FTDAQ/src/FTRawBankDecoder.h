#ifndef FTRAWBANKDECODER_H
#define FTRAWBANKDECODER_H 1

// Include files
// from Gaudi
#include "Event/FTLiteCluster.h"
#include "GaudiAlg/Transformer.h"
#include "GaudiAlg/FunctionalUtilities.h"

using FTLiteClusters = FastClusterContainer<LHCb::FTLiteCluster,int>;

/** @class FTRawBankDecoder FTRawBankDecoder.h
 *  Decode the FT raw bank into FTLiteClusters
 *
 *  @author Olivier Callot
 *  @date   2012-05-11
 */
struct FTRawBankDecoder : Gaudi::Functional::Transformer< FTLiteClusters( const LHCb::RawEvent& ) >
{
  /// Standard constructor
  FTRawBankDecoder( const std::string& name, ISvcLocator* pSvcLocator );

  FTLiteClusters operator()(const LHCb::RawEvent& rawEvent) const override;

  // for MC, following property has to be same as cluster creator, 
  // not sure how to ensure this (TODO)
  Gaudi::Property<unsigned int> m_clusterMaxWidth{ this, "ClusterMaxWidth", 4,
      "Maximal cluster width"};

};
#endif // FTRAWBANKDECODER_H
