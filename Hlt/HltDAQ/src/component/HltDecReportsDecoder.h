#ifndef HLTDECREPORTSDECODER_H
#define HLTDECREPORTSDECODER_H 1

// Include files
#include "Event/HltDecReports.h"
#include "HltRawBankDecoderBase.h"

/** @class HltDecReportsDecoder HltDecReportsDecoder.h
 *
 *
 *  @author Tomasz Skwarnicki
 *  @date   2008-08-02
 *
 *  Algorithm to read HltDecReports from Raw Data and create container on TES
 *
 */
class HltDecReportsDecoder : public HltRawBankDecoder<LHCb::HltDecReports> {
public:
  /// Standard constructor
  HltDecReportsDecoder( const std::string& name, ISvcLocator* pSvcLocator);

  ///< Algorithm execution
  LHCb::HltDecReports operator()(const LHCb::RawEvent&) const override;

private:
  enum HeaderIDs { kVersionNumber=2 };

  template <typename HDRConverter,typename I, typename Table >
  int decodeHDR(I i, I end,  LHCb::HltDecReports& output, const Table& table) const ;

};
#endif // HLTDECREPORTSDECODER_H
