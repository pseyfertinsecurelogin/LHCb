#ifndef HLTSELREPORTSDECODER_H
#define HLTSELREPORTSDECODER_H 1

// Include files
// from Gaudi
#include "HltRawBankDecoderBase.h"

#include "HltDAQ/IReportConvert.h"
#include "HltDAQ/ReportConvertTool.h"

/** @class HltSelReportsDecoder HltSelReportsDecoder.h
 *
 *
 *  @author Tomasz Skwarnicki
 *  @date   2008-08-02
 *
 *  Algorithm to read HltSelReports from Raw Data and create containers on TES
 *
 */

class HltSelReportsDecoder : public HltRawBankDecoderBase {
public:
  /// Standard constructor
  using HltRawBankDecoderBase::HltRawBankDecoderBase;
  StatusCode initialize() override;
  StatusCode execute   () override;    ///< Algorithm execution

private:
  enum HeaderIDs { kVersionNumber=11 };
  /// location of output
  Gaudi::Property<std::string> m_outputHltSelReportsLocation{ this,"OutputHltSelReportsLocation", LHCb::HltSelReportsLocation::Default};
  /// for converting objects in to summaries
  IReportConvert* m_conv = nullptr;
};

#endif // HLTSELREPORTSDECODER_H
