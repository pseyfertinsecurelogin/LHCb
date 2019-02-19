/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef HLTSELREPORTSDECODER_H
#define HLTSELREPORTSDECODER_H 1

// Include files
// from Gaudi
#include "HltDAQ/IReportConvert.h"
#include "HltRawBankDecoderBase.h"

/** @class HltSelReportsDecoder HltSelReportsDecoder.h
 *
 *
 *  @author Tomasz Skwarnicki
 *  @date   2008-08-02
 *
 *  Algorithm to read HltSelReports from Raw Data and create containers on TES
 *
 */

class HltSelReportsDecoder : public HltRawBankMultiDecoder<LHCb::HltSelReports, LHCb::HltObjectSummary::Container> {
public:
  /// Standard constructor
  HltSelReportsDecoder( const std::string& name, ISvcLocator* pSvcLocator );

  ///< Algorithm initialization
  StatusCode initialize() override;

  ///< Algorithm execution
  std::tuple<LHCb::HltSelReports, LHCb::HltObjectSummary::Container> operator()( const LHCb::RawEvent& ) const override;

private:
  enum HeaderIDs { kVersionNumber = 11 };
  /// for converting objects in to summaries
  IReportConvert* m_conv = nullptr;
};

#endif // HLTSELREPORTSDECODER_H
