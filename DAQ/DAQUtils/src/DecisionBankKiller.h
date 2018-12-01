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
#ifndef DECISIONBANKKILLER_H
#define DECISIONBANKKILLER_H 1

// STL
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <regex>

#include "GaudiAlg/Consumer.h"
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Event/RawEvent.h"
#include "Event/HltDecReports.h"
#include "DAQKernel/DecoderAlgBase.h"
#include "LoKi/IHDRFilterTool.h"
#include "Kernel/ILinePersistenceSvc.h"

/** @class DecisionBankKiller DecisionBankKiller.h
 *
 *   Decision Bank Killer class to keep only banks requested by lines
 *
 *   Input:
 *    - InputHltDecReportsLocation : HltDecReports location
 *    - KillFromAll : Main behaviour switch. If false (default), kill only banks
 *      in the first location found in the search string. If true, kill *all*
 *      banks found in the search string.
 *    - LineFilter : An HDRFilterTool configured with an HDR filter selecting the
 *      subset of lines the decision and request of which will be considered.
 *    - AlwaysKeepBanks: Set of bank types to always keep regardless of decisions.
 *
 *    @author: Konstantin Gizdov, Rosen Matev
 *    @date:   25 August 2016, 23 June 2017
 */

class DecisionBankKiller : public Gaudi::Functional::Consumer<
  void(const LHCb::HltDecReports&),
  Gaudi::Functional::Traits::BaseClass_t<Decoder::AlgBase>> {

 public:
  DecisionBankKiller(const std::string& name, ISvcLocator* pSvcLocator);

  StatusCode initialize() override;
  void operator()(const LHCb::HltDecReports& decReports) const override;

 protected:
  using ReqBanksBitset = ILinePersistenceSvc::RawBanks;

  /// Find the line decision names that could pass the filter in m_lineFilterTool
  ILinePersistenceSvc::LineDecNames filteredLines(const LHCb::HltDecReports& hdr) const;

  /// kill all banks of given type in a given raw event
  void killBankType(LHCb::RawEvent *rawEvent, LHCb::RawBank::BankType bankType, bool warningmsg) const;

  /// kill all non-requested banks in a given raw event
  void killFromRawEvent(LHCb::RawEvent *rawEvent, const ReqBanksBitset &reqBanks) const;

 private:
  /// Set of banks to be always kept
  ReqBanksBitset m_alwaysKeepBanksSet{};
  Gaudi::Property<std::vector<std::string>> m_alwaysKeepBanks{this,
    "AlwaysKeepBanks", {"ODIN", "HltRoutingBits", "DAQ"},
    [=](auto&) {
        for (unsigned i = 0; i < LHCb::RawBank::LastType; ++i) {
          auto type = LHCb::RawBank::typeName(static_cast<LHCb::RawBank::BankType>(i));
          if (std::find(std::begin(m_alwaysKeepBanks), std::end(m_alwaysKeepBanks), type)
              != std::end(m_alwaysKeepBanks)) {
            m_alwaysKeepBanksSet[i] = 1;
          }
        }
    },
    Gaudi::Details::Property::ImmediatelyInvokeHandler{true},
    "Set of Banks to always keep regardless of line conditions."};
  Gaudi::Property<bool> m_killFromAll{this, "KillFromAll", false,
    "Whether to kill from all raw event locations, or just from the first one"};
  /// Implementation of ILinePersistenceSvc used to get the requested raw banks
  Gaudi::Property<std::string> m_linePersistenceSvcName{this, "ILinePersistenceSvc", ""};
  /// LoKi Tool to execute line filter
  ToolHandle<LoKi::IHDRFilterTool> m_lineFilterTool{this,"LineFilter","LoKi::HDRFilterTool","LoKi Tool to execute line filter" };

  SmartIF<ILinePersistenceSvc> m_linePersistenceSvc;
  mutable std::map<unsigned int, ILinePersistenceSvc::LineDecNames> m_filteredLinesCache;
};
#endif  // DECISIONBANKKILLER_H
