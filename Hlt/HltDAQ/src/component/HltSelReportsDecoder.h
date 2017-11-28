#ifndef HLTSELREPORTSDECODER_H 
#define HLTSELREPORTSDECODER_H 1

// Include files
// from Gaudi
#include "HltRawBankDecoderBase.h"
#include "HltDAQ/IReportConvert.h"

/** @class HltSelReportsDecoder HltSelReportsDecoder.h
 *  
 *
 *  @author Tomasz Skwarnicki
 *  @date   2008-08-02
 *
 *  Algorithm to read HltSelReports from Raw Data and create containers on TES
 *
 */

class HltSelReportsDecoder : public HltRawBankMultiDecoder<LHCb::HltSelReports,LHCb::HltObjectSummary::Container> {
public: 
  /// Standard constructor
  HltSelReportsDecoder( const std::string& name, ISvcLocator* pSvcLocator );

  ///< Algorithm initialization
  StatusCode initialize() override;

  ///< Algorithm execution
  std::tuple<LHCb::HltSelReports,LHCb::HltObjectSummary::Container> operator()(const LHCb::RawEvent&) const override;

private:
  enum HeaderIDs { kVersionNumber=11 };
  /// for converting objects in to summaries
  IReportConvert* m_conv = nullptr;
};

#endif // HLTSELREPORTSDECODER_H
