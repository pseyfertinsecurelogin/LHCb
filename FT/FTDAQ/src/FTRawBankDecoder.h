// $Id: $
#ifndef FTRAWBANKDECODER_H 
#define FTRAWBANKDECODER_H 1

// Include files
// from Gaudi
#include "Event/FTLiteCluster.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "DAQKernel/DecoderAlgBase.h"

/** @class FTRawBankDecoder FTRawBankDecoder.h
 *  Decode the FT raw bank into FTLiteClusters
 *
 *  @author Olivier Callot
 *  @date   2012-05-11
 */

typedef FastClusterContainer<LHCb::FTLiteCluster,int> FTLiteClusters;

class FTRawBankDecoder : public Decoder::AlgBase {
public: 
  /// Standard constructor
  FTRawBankDecoder( const std::string& name, ISvcLocator* pSvcLocator );
  
  ~FTRawBankDecoder( ) override = default; ///< Destructor
  
  StatusCode execute() override; ///< Algorithm execution


private:
  
  Gaudi::Property<std::string> m_outputClusterLocation  {this, "OutputLocation"  , LHCb::FTLiteClusterLocation::Default };
  std::unique_ptr<FTLiteClusters> operator()(const std::vector<LHCb::RawBank*>& banks) const;

};
#endif // FTRAWBANKDECODER_H
