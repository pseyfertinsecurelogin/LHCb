// $Id$
#ifndef HLTDECREPORTSDECODER_H 
#define HLTDECREPORTSDECODER_H 1

// Include files
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
class HltDecReportsDecoder : public HltRawBankDecoderBase {
public: 
  /// Standard constructor
  using HltRawBankDecoderBase::HltRawBankDecoderBase;

  ~HltDecReportsDecoder() override = default ; ///< Destructor
  StatusCode execute() override;    ///< Algorithm execution

private:
  enum HeaderIDs { kVersionNumber=2 };

  template <typename HDRConverter,typename I, typename Table > 
  int decodeHDR(I i, I end,  LHCb::HltDecReports& output, const Table& table) const ;

  /// location of output
  Gaudi::Property<std::string> m_outputHltDecReportsLocation { this, "OutputHltDecReportsLocation", LHCb::HltDecReportsLocation::Default };
};
#endif // HLTDECREPORTSDECODER_H
