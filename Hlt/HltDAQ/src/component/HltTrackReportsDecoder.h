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
#ifndef HLTTRACKREPORTSDECODER_H
#define HLTTRACKREPORTSDECODER_H 1

// Include files
#include <atomic>
// from Gaudi
#include "Event/Track.h"
#include "HltRawBankDecoderBase.h"

/** @class HltTrackReportsDecoder HltTrackReportsDecoder.h
 *
 *
 *  @author Sebastian Neubert
 *  @date   2014-01-16
 *
 *  Algorithm to read HltTracks from Raw Data and create containers on TES
 *
 */
class HltTrackReportsDecoder : public HltRawBankSplittingDecoder<LHCb::Tracks> {
public:
  enum HeaderIDs { kVersionNumber = 1 };

  /// Standard constructor
  HltTrackReportsDecoder( const std::string& name, ISvcLocator* pSvcLocator );

  ///< Algorithm initialization
  StatusCode initialize() override;

  ///< Algorithm execution
  Gaudi::Functional::vector_of_optional_<LHCb::Tracks> operator()( const LHCb::RawEvent& ) const override;

private:
  /// location of HltTrackReports
  StringProperty m_HltTrackReportsLocation;

  /// SourceID to decode. source ids are linked to track stages in TrackNames.trackingSources
  std::vector<unsigned> m_map;

  mutable std::atomic<unsigned int> m_callcount{0};
};

#endif // HLTTRACKREPORTSDECODER_H
