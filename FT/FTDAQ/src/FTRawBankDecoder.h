// $Id: $
#ifndef FTRAWBANKDECODER_H 
#define FTRAWBANKDECODER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/Transformer.h"
#include "DAQKernel/DecoderAlgBase.h"


/** @class FTRawBankDecoder FTRawBankDecoder.h
 *  Decode the FT raw bank into FTLiteClusters
 *
 *  @author Olivier Callot
 *  @date   2012-05-11
 */
struct FTRawBankDecoder : Gaudi::Functional::Transformer<
   FastClusterContainer<LHCb::FTLiteCluster,int>( const LHCb::RawEvent& )
>
{
  /// Standard constructor
  FTRawBankDecoder( const std::string& name, ISvcLocator* pSvcLocator );

  FastClusterContainer<LHCb::FTLiteCluster,int>
  operator()(const LHCb::RawEvent& rawEvent) const override;

};
#endif // FTRAWBANKDECODER_H
