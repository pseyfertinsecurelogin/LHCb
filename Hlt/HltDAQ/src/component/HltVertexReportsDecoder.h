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
class HltVertexReportsDecoder : public HltRawBankDecoderBase {
public:

  enum HeaderIDs { kVersionNumber=2 };

  /// Standard constructor
  using HltRawBankDecoderBase::HltRawBankDecoderBase;
  StatusCode execute   () override;    ///< Algorithm execution

private:
  // ----------------------- data members
  /// location of output
  Gaudi::Property<std::string> m_outputHltVertexReportsLocation{ this,"OutputHltVertexReportsLocation", LHCb::HltVertexReportsLocation::Default};
};

#endif // HLTVERTEXREPORTSDECODER_H
