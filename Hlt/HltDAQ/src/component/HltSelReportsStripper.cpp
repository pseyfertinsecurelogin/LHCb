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
#include <algorithm>
#include <map>

// Include files
#include "Event/HltSelReports.h"
#include "Event/HltObjectSummary.h"

// from Gaudi
#include "GaudiAlg/Transformer.h"
#include "GaudiAlg/GaudiAlgorithm.h"

/** @class HltSelReportsStripper HltSelReportsStripper.cpp
 *
 * \brief Algorithm that strips HltSelReports, keeping only the selected
 *        decisions.
 *
 * Only per-selection granularity is supported, i.e. we will either keep
 * or not the report for each trigger line.
 *
 * The algorithm should be configured with:
 * - The input locations of the HltSelReports
 * - The output locations of the HltSelReports and the HltObjectSummaries.
 *   The second is expected to be the first + "/Candidates".
 * - A list of names of selections to be kept, e.g.
 *   `['Hlt2Topo2BodyDecision', 'Hlt2RecSummary']`
 *
 */
class HltSelReportsStripper: public Gaudi::Functional::MultiTransformer<
    std::tuple<LHCb::HltSelReports, LHCb::HltObjectSummary::Container>
    (const LHCb::HltSelReports&)>
{

public:
  HltSelReportsStripper(const std::string& name, ISvcLocator* pSvcLocator)
    : MultiTransformer(name, pSvcLocator,
      KeyValue{"InputHltSelReportsLocation",
               LHCb::HltSelReportsLocation::Default},
      {
        KeyValue{"OutputHltSelReportsLocation",
                 LHCb::HltSelReportsLocation::Default + "Stripped"},
        KeyValue{"OutputHltObjectSummariesLocation",
                 LHCb::HltSelReportsLocation::Default + "Stripped/Candidates"}
      })
  {};

  std::tuple<LHCb::HltSelReports, LHCb::HltObjectSummary::Container>
  operator()(const LHCb::HltSelReports& selReports) const override;

private:
  using SummaryClones = std::map<const LHCb::HltObjectSummary*, LHCb::HltObjectSummary*>;

  /// Deep copy the HltObjectSummary object while keeping track of the clones.
  LHCb::HltObjectSummary* cloneSummary(
    const LHCb::HltObjectSummary& hos, SummaryClones& clones) const;

  /// Names of selections to keep.
  Gaudi::Property< std::vector<std::string> > m_lines{this, "SelectionNames",
    {}, "Names of selections to keep."};

}; 


std::tuple<LHCb::HltSelReports, LHCb::HltObjectSummary::Container>
HltSelReportsStripper::operator()(const LHCb::HltSelReports& selReports) const
{
  std::tuple<LHCb::HltSelReports, LHCb::HltObjectSummary::Container> outputs;
  auto& outputSelReports = std::get<0>(outputs);
  auto& objectSummaries = std::get<1>(outputs);

  SummaryClones clones;

  for(const auto& selReport: selReports) {
    if (std::find(std::begin(m_lines), std::end(m_lines), selReport.first) !=
        std::end(m_lines)) {
      auto clone = cloneSummary(selReport.second, clones);
      outputSelReports.insert(selReport.first, *clone);
    }
  }

  for (const auto& pair: clones) {
    objectSummaries.push_back(pair.second);
  }

  return outputs;
}


LHCb::HltObjectSummary* HltSelReportsStripper::cloneSummary(
  const LHCb::HltObjectSummary& hos, SummaryClones& clones) const
{
  auto it = clones.find(&hos);
  if (it != std::end(clones)) {
    return it->second;
  }

  auto clone = new LHCb::HltObjectSummary();
  clones.emplace(&hos, clone);

  // Copy the basic members
  clone->setSummarizedObjectCLID(hos.summarizedObjectCLID());
  clone->setNumericalInfo(hos.numericalInfo());
  clone->setLhcbIDs(hos.lhcbIDs());

  // Deep copy the substructures
  for (const auto& sub : hos.substructure()) {
    clone->addToSubstructure(cloneSummary(*sub, clones));
  }
  for (const auto& sub : hos.substructureExtended()) {
    clone->addToSubstructureExtended(cloneSummary(*sub, clones));
  }

  return clone;
}

DECLARE_COMPONENT( HltSelReportsStripper )
