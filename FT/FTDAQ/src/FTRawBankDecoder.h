#ifndef FTRAWBANKDECODER_H
#define FTRAWBANKDECODER_H 1

// Include files
// from Gaudi
#include "Event/FTLiteCluster.h"
#include "GaudiAlg/Transformer.h"
#include "GaudiAlg/FunctionalUtilities.h"
#include "Kernel/MultiIndexedContainer.h"
#include "IFTReadoutTool.h"
#include "FTRawBankParams.h"

using namespace Gaudi::Functional;

using FTLiteClusters = LHCb::FTLiteCluster::FTLiteClusters;


/** @class FTRawBankDecoder FTRawBankDecoder.h
 *  Decode the FT raw bank into FTLiteClusters
 *
 *  @author Olivier Callot
 *  @date   2012-05-11
 */
class FTRawBankDecoder : public Transformer< FTLiteClusters( const LHCb::RawEvent& ) >
{
 public:
  /// Standard constructor
  FTRawBankDecoder( const std::string& name, ISvcLocator* pSvcLocator );
  
  FTLiteClusters operator()(const LHCb::RawEvent& rawEvent) const override;

 private:
  PublicToolHandle<IFTReadoutTool> m_readoutTool = { this, "FTReadoutTool", "FTReadoutTool" };

  // for MC, following property has to be same as cluster creator, 
  // not sure how to ensure this
  Gaudi::Property<unsigned int> m_clusterMaxWidth{ this, "ClusterMaxWidth", 4,
      "Maximal cluster width"};
  Gaudi::Property<bool> m_forceVersion4{ this, "ForceVersion4", false,
      "Force to decode v5 raw banks as v4"};

};
#endif // FTRAWBANKDECODER_H
