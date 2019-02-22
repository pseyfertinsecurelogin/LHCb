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
#ifndef TESTS_RAWEVENTSELECTIVECOPY_H
#define TESTS_RAWEVENTSELECTIVECOPY_H 1

#include "Event/RawBank.h"
#include "Event/RawEvent.h"

#include "GaudiAlg/GaudiAlgorithm.h"

#include <string>
#include <vector>

/** @class RawEventSelectiveCopy RawEventSelectiveCopy.h
 *  Copies selected RawBanks to RawEvent at a new TES location.
 *
 *  @author Tomasz Skwarnicki
 *  @date   2009-06-22
 */
class RawEventSelectiveCopy : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

private:
  Gaudi::Property<std::vector<std::string>> m_banksToCopy{
      this, "RawBanksToCopy", {}, "Create a new RawEvent copying only these banks"};
  Gaudi::Property<std::vector<std::string>> m_banksToRemove{
      this, "RawBanksToRemove", {}, "Create a RawEvent copy, with these banks removed"};
  Gaudi::Property<std::string> m_inputLocation{this, "InputRawEventLocation", LHCb::RawEventLocation::Default};
  Gaudi::Property<std::string> m_outputLocation{this, "OutputRawEventLocation", LHCb::RawEventLocation::Copied};

  std::vector<LHCb::RawBank::BankType> m_bankTypes;
};

#endif // TESTS_RAWEVENTSELECTIVECOPY_H
