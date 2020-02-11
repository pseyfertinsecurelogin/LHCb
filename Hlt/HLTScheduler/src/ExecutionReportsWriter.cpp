/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "Kernel/EventContextExt.h"

// as this one also includes a parser, lets put that first
#include "HLTControlFlowMgr.h"

#include "Event/HltDecReports.h"
#include "Gaudi/Algorithm.h"
#include "GaudiAlg/FixTESPath.h"
#include "GaudiAlg/FunctionalDetails.h"
#include "GaudiAlg/Transformer.h"
#include "GaudiKernel/EventContext.h"
#include "GaudiKernel/StatusCode.h"
#include "Kernel/IANNSvc.h"

#include <string>
#include <type_traits>
#include <vector>

/** @brief Write DecReport objects based on the status of the execution nodes in the scheduler.
 *
 * Each execution node of the HltControlFlowMgr is converted to a DecReport
 * object, with the decision corresponding to the state of the node. The ID
 * used in the DecReport is taken from the ANNSvc.
 *
 * Note that historically, the names that we expect to see in the DecReports end in "Decision",
 * and we keep this behaviour, so the ANNSvc will look for trigger decisions called <node_name> +
 * "Decision" (see the decision_name variable).
 */
class ExecutionReportsWriter final
    : public Gaudi::Functional::Transformer<LHCb::HltDecReports( EventContext const& ),
                                            Gaudi::Functional::Traits::BaseClass_t<FixTESPath<Gaudi::Algorithm>>> {
public:
  ExecutionReportsWriter( const std::string& name, ISvcLocator* pSvcLocator )
      : Transformer{name, pSvcLocator, {"DecReportsLocation", "/Event/DecReport"}} {}
  StatusCode          start() override;
  LHCb::HltDecReports operator()( EventContext const& evtCtx ) const override;

private:
  HLTControlFlowMgr* m_schedulerPtr = nullptr;

  /// ANNSvc for translating selection names to int selection IDs
  ServiceHandle<IANNSvc> m_hltANNSvc{this, "ANNSvc", "HltANNSvc", "Service to retrieve DecReport IDs"};

  /// Map from line name to index in the scheduler's execution node list and
  /// number assigned by the HltAnnSvc
  std::map<std::string, std::pair<int, IANNSvc::minor_mapped_type>> m_name_indices{};

  Gaudi::Property<int>                      m_printFreq{this, "PrintFreq", 1000, "Print Frequency for states"};
  Gaudi::Property<std::vector<std::string>> m_line_names{this, "Persist", {}, "Specify the nodes to be written to TES"};
  Gaudi::Property<std::string>              m_ann_key{this, "ANNSvcKey", "", "Key from the ANN service to query."};
};

DECLARE_COMPONENT( ExecutionReportsWriter )

StatusCode ExecutionReportsWriter::start() {
  auto sc = Transformer::start();
  if ( !sc ) return sc;
  m_schedulerPtr = dynamic_cast<HLTControlFlowMgr*>( &*service<IEventProcessor>( "HLTControlFlowMgr", false ) );
  if ( !m_schedulerPtr ) return StatusCode::FAILURE;

  const auto& scheduler_items = m_schedulerPtr->getNodeNamesWithIndices();
  const auto& ann_items       = m_hltANNSvc->items( m_ann_key.value() );
  for ( const auto& name : m_line_names ) {
    // The scheduler stores execution nodes as a flat list. We can cache the
    // index of the execution node for each line now, and retrieve the node from
    // the list in each event
    auto node_idx = std::find_if( scheduler_items.begin(), scheduler_items.end(),
                                  [&]( const auto& p ) { return std::get<0>( p ) == name; } );
    if ( node_idx == scheduler_items.end() ) {
      error() << "Line name not a known execution node: " << name << endmsg;
      return StatusCode::FAILURE;
    }

    // Historically names in DecReports end with decision, so we keep this.
    // Thus ANNSvc and is expected to contain (node name + "Decision") <-> int
    auto decision_name = name + "Decision";

    // Translate each decision name to an int, which will be written to the DecReport
    // If the decision name isn't known to the ANNSvc we can't translate it
    auto ann_idx = std::find_if( ann_items.begin(), ann_items.end(),
                                 [&]( const auto& p ) { return std::get<0>( p ) == decision_name; } );
    if ( ann_idx == ann_items.end() ) {
      error() << "Decision name not known to ANNSvc: " << decision_name << endmsg;
      return StatusCode::FAILURE;
    }

    m_name_indices[decision_name] = {std::get<1>( *node_idx ), std::get<1>( *ann_idx )};
  }
  return sc;
}

LHCb::HltDecReports ExecutionReportsWriter::operator()( EventContext const& evtCtx ) const {
  auto const& lhcbExt                 = evtCtx.getExtension<LHCb::EventContextExtension>();
  auto const& [NodeStates, AlgStates] = lhcbExt.getSchedulerExtension<HLTControlFlowMgr::SchedulerStates>();

  if ( UNLIKELY( evtCtx.evt() % m_printFreq == 0 ) && msgLevel( MSG::DEBUG ) ) {
    debug() << m_schedulerPtr->buildAlgsWithStates( AlgStates ).str() << endmsg;
    debug() << m_schedulerPtr->buildPrintableStateTree( LHCb::make_span( NodeStates ) ).str() << endmsg;
  }
  LHCb::HltDecReports reports{};
  reports.reserve( m_name_indices.size() );
  for ( const auto& item : m_name_indices ) {
    const auto& decision_name       = std::get<0>( item );
    const auto& [node_idx, ann_idx] = std::get<1>( item );
    reports.insert( decision_name, {NodeStates[node_idx].passed, 0, 0, 0, ann_idx} );
  }
  return reports; // write down something better?
}
