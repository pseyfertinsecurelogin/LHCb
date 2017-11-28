#ifndef HLTVERTEXREPORTSDECODER_H 
#define HLTVERTEXREPORTSDECODER_H 1

// Include files
// from Gaudi
#include "HltRawBankDecoderBase.h"

/** @class HltvertexReportsDecoder HltvertexReportsDecoder.h
 *  
 *  @author Tomasz Skwarnicki
 *  @date   2008-08-05
 *
 *  Algorithm to translate HltSummary  into HltVertexReports
 *
 */
class HltVertexReportsDecoder : public HltRawBankSplittingDecoder<LHCb::VertexBase::Container> {
public:
  enum HeaderIDs { kVersionNumber=2 };

  /// Standard constructor
  HltVertexReportsDecoder( const std::string& name, ISvcLocator* pSvcLocator );

  ///< Algorithm execution
  Gaudi::Functional::vector_of_optional_<LHCb::VertexBase::Container> operator()(const LHCb::RawEvent&) const override; 

private:
  std::vector<std::string> m_decode;               /// which containers to decode

};

#endif // HLTVERTEXREPORTSDECODER_H
