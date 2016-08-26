#ifndef DECISIONBANKKILLER_H
#define DECISIONBANKKILLER_H 1

// STL
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <regex>

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "DAQKernel/DecoderAlgBase.h"
// from Event
#include "Event/RawEvent.h"
#include "Event/HltDecReports.h"

/** @class DecisionBankKiller DecisionBankKiller.h
 *
 *   Decision Bank Killer class to keep only banks requested by lines
 *
 *   Input:
 *    - InputHltDecReportsLocation : Decision Reports location
 *    - KillFromAll : Main behaviour switch. If false (default), kill only banks in the first location found in the search string. If true, kill *all* banks found in the search string.
 *    - DecisionFilter : Regular expression to filter input. Lines that do not match this filter are ignored. By default it accepts all lines.
 *    - AlwaysKeepBanks: Set of Banks to always keep regardless of line conditions.
 *
 *    @author: Konstantin Gizdov
 *    @date:   25 August 2016
 */

class DecisionBankKiller : public Decoder::AlgBase {
  friend class AlgFactory<DecisionBankKiller>;
 public:
  DecisionBankKiller(const std::string& name, ISvcLocator* pSvcLocator);

  virtual ~DecisionBankKiller();

  virtual StatusCode initialize();
  virtual StatusCode execute();

 protected:
  using ReqBanksBitset = std::bitset<LHCb::RawBank::LastType>;
  ///. kill all banks of given type
  void  killBankType(LHCb::RawEvent *rawEvent, LHCb::RawBank::BankType bankType, bool warningmsg);

  ///. kill all banks with the current m_rawEvent
  StatusCode  killFromRawEvent(LHCb::RawEvent *rawEvent, const ReqBanksBitset &reqBanks);

 private:
  /// Input Location for HltDecReports
  std::string m_inputHltDecReportsLocation{};
  /// Regular Expression Decision filter to filter input lines
  std::string m_decisionFilter{};
  /// Regular Expression container
  std::regex m_decisionFilterRegex{};
  /// Always keep these banks
  ReqBanksBitset m_alwaysKeepBanksSet{};
  /// Override always keep these banks
  std::vector<std::string> m_alwaysKeepBanks{};
  /// kill from all raw event locations, or just from the first one
  bool m_killFromAll;
};
#endif  // DECISIONBANKKILLER_H
