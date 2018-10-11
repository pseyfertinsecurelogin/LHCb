/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef HLTLUMIWRITER_H
#define HLTLUMIWRITER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Event/RawEvent.h"

/** @class HltLumiWriter HltLumiWriter.h
 *  Fills the Raw Buffer banks for the LumiSummary
 *
 *  @author Jaap Panman
 *  @date   2004-07-22
 */
class HltLumiWriter : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

private:

  void fillDataBankShort ( );

  Gaudi::Property<std::string> m_inputBank = { this, "InputBank", LHCb::HltLumiSummaryLocation::Default };

  // Statistics
  double m_totDataSize = 0;
  int m_nbEvents = 0;
  std::vector<unsigned int> m_bank;
  LHCb::RawBank::BankType m_bankType = LHCb::RawBank::HltLumiSummary;
  Gaudi::Property<std::string> m_inputRawEventLocation { this, "RawEventLocation" };
  std::vector<std::string> m_rawEventLocations;

};
#endif // HLTLUMIWRITER_H
