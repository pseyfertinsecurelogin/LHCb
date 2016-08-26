#include "DecisionBankKiller.h"

// ============================================================================
/** @file DecisionBankKiller.cpp
 *
 *    @author: Konstantin Gizdov
 *    @date:   25 August 2016
 */
// ============================================================================

DECLARE_ALGORITHM_FACTORY(DecisionBankKiller)

//=============================================================================
// Standard creator, initializes variables
//=============================================================================
DecisionBankKiller::DecisionBankKiller(const std::string& name, ISvcLocator* pSvcLocator)
  : Decoder::AlgBase(name, pSvcLocator) {
  m_rawEventLocations = {LHCb::RawEventLocation::Default};
  initRawEventSearch();

  // get input
  declareProperty("InputHltDecReportsLocation",
                  m_inputHltDecReportsLocation = LHCb::HltDecReportsLocation::Default);
  // kill all or first match
  declareProperty("KillFromAll", m_killFromAll = false,
                  "Main behaviour switch. If false (default), kill only banks in the first location found in the search string."
                  " If true, kill *all* banks found in the search string.");
  auto *flt = declareProperty("DecisionFilter",
                  m_decisionFilter = "^.*$",
                  "Regular expression to filter input. Lines that do not match this filter are ignored."
                  " By default it accepts all lines.");
  flt->declareUpdateHandler([=](Property&) {
    m_decisionFilterRegex.assign(m_decisionFilter);
  });
  flt->useUpdateHandler();  // sync filter regex to decision filter
  auto *banks = declareProperty("AlwaysKeepBanks",
                  m_alwaysKeepBanks = {"ODIN","HltRoutingBits","DAQ"},
                  "Set of Banks to always keep regardless of line conditions.");
  banks->declareUpdateHandler([=](Property&) {
    for (unsigned i = 0; i < LHCb::RawBank::LastType; ++i) {
      if (std::find(m_alwaysKeepBanks.begin(), m_alwaysKeepBanks.end(),
                    LHCb::RawBank::typeName(static_cast<LHCb::RawBank::BankType>(i))) != m_alwaysKeepBanks.end()) {
        m_alwaysKeepBanksSet[i] = 1;
      }
    }
  });
  banks->useUpdateHandler();  // sync banks bitset to always keep string names
}

//=============================================================================
// Standard destructor
//=============================================================================
DecisionBankKiller::~DecisionBankKiller() {}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode DecisionBankKiller::initialize() {
  StatusCode sc = Decoder::AlgBase::initialize();
  if (sc.isFailure()) return sc;

  if (m_rawEventLocations.empty()) return Error("You didn't give me a list of RawEventLocations to look through", StatusCode::FAILURE);

  if (m_killFromAll) {
    info() << "Killing banks from everything in '" << m_rawEventLocations << "'" << endmsg;
  } else if (m_rawEventLocations.size() != 1 || m_rawEventLocations[0] != LHCb::RawEventLocation::Default) {
    info() << "Killing banks from first raw event in '" << m_rawEventLocations << "'" << endmsg;
  }
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode DecisionBankKiller::execute() {
  const auto inputSummary = getIfExists<LHCb::HltDecReports>(m_inputHltDecReportsLocation);
  if (!inputSummary) {
    return Warning(" No HltDecReports at " + m_inputHltDecReportsLocation, StatusCode::SUCCESS, 20);
  }
  ReqBanksBitset reqBanks(m_alwaysKeepBanksSet);
  // reqBanks |= m_alwaysKeepBanksSet;
  for (const auto& rep : *inputSummary) {
      if (!std::regex_match(rep.first, m_decisionFilterRegex)) continue;
      auto decRep = LHCb::HltDecReport{rep.second.decReport()};
      if (decRep.decision()) {
        reqBanks |= rep.second.reqBanks();
      }
  }
  for (auto loc : m_rawEventLocations) {
    auto rawEvt = tryEventAt(loc);
    if (rawEvt != NULL) {
      auto sc = killFromRawEvent(rawEvt, reqBanks);
      if (sc.isFailure()) return sc;
      if (!m_killFromAll) break;
    }
  }

  return StatusCode::SUCCESS;
}

StatusCode DecisionBankKiller::killFromRawEvent(LHCb::RawEvent *rawEvent, const ReqBanksBitset &reqBanks) {
  for (unsigned int ibank = 0; ibank < (unsigned int) LHCb::RawBank::LastType; ++ibank) {
    if (reqBanks[ibank]) continue;
    LHCb::RawBank::BankType bankType = (LHCb::RawBank::BankType) ibank;
    killBankType(rawEvent, bankType, true);
  }
  return StatusCode::SUCCESS;
}

//=============================================================================

void  DecisionBankKiller::killBankType(LHCb::RawEvent *rawEvent,
                                       LHCb::RawBank::BankType bankType,
                                       bool warningmsg) {
  const std::string bankTypeName = LHCb::RawBank::typeName(bankType);
  // look for all banks of this type and remove them
  std::vector<LHCb::RawBank*> banks = rawEvent->banks(bankType);
  if (bankType == LHCb::RawBank::LastType || 0 == banks.size()) {
    if (warningmsg) {
      std::stringstream s("");
      s << " No bank of type '" << bankTypeName << "' has been found - nothing to be removed";
      Warning(s.str(), StatusCode::SUCCESS, 0).ignore();
    }
    return;
  }

  if (msgLevel(MSG::DEBUG)) debug() << "All banks of type '" << bankTypeName
                                    << "'  are to be removed - banks size =  " << banks.size() << endmsg;

  for (std::vector<LHCb::RawBank*>::const_iterator itB = banks.begin(); itB != banks.end(); ++itB) {
    bool success = rawEvent -> removeBank(*itB);
    if (!success && warningmsg) {
      if (msgLevel(MSG::DEBUG)) debug() << "The bank " << * itB << " of type '" << bankTypeName
                                        << "' has not been found to be removed " << endmsg;
      Warning("The bank of type '" + bankTypeName + "' has not been found", StatusCode::SUCCESS, 0).ignore();
    }
  }
  if (msgLevel(MSG::DEBUG)) debug() << "All banks of type '" << bankTypeName
                                    << "' have been removed  - "
                                    << " banks size is now : " << (rawEvent->banks(bankType)).size() << endmsg;
}
