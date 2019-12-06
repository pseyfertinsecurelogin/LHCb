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

// as this one also includes a parser, lets put that first
#include "HLTControlFlowMgr.h"

#include "Event/HltDecReports.h"
#include "Gaudi/Algorithm.h"
#include "GaudiAlg/FixTESPath.h"
#include "GaudiAlg/FunctionalDetails.h"
#include "GaudiAlg/Transformer.h"
#include "GaudiKernel/EventContext.h"
#include "GaudiKernel/StatusCode.h"

#include <string>
#include <type_traits>
#include <vector>

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

  std::vector<int> m_name_indices{};

  Gaudi::Property<int>                      m_printFreq{this, "PrintFreq", 1000, "Print Frequency for states"};
  Gaudi::Property<std::vector<std::string>> m_line_names{this, "Persist", {}, "Specify the nodes to be written to TES"};
};

DECLARE_COMPONENT( ExecutionReportsWriter )

StatusCode ExecutionReportsWriter::start() {
  auto sc = Transformer::start();
  if ( !sc ) return sc;
  m_schedulerPtr = dynamic_cast<HLTControlFlowMgr*>( &*service<IEventProcessor>( "HLTControlFlowMgr", false ) );
  if ( !m_schedulerPtr ) return StatusCode::FAILURE;

  m_name_indices.reserve( m_line_names.size() );

  auto names_to_indices = m_schedulerPtr->getNodeNamesWithIndices();
  for ( std::string_view name : m_line_names ) {
    auto i = std::find_if( names_to_indices.begin(), names_to_indices.end(),
                           [&]( const auto& p ) { return std::get<0>( p ) == name; } );
    if ( i == names_to_indices.end() ) {
      error() << "Not all line names have been found, please check" << endmsg;
      return StatusCode::FAILURE;
    }
    m_name_indices.emplace_back( std::get<1>( *i ) );
  }
  return sc;
}

LHCb::HltDecReports ExecutionReportsWriter::operator()( EventContext const& evtCtx ) const {
  auto const& [NodeStates, AlgStates] = evtCtx.getExtension<HLTControlFlowMgr::SchedulerStates>();

  if ( UNLIKELY( evtCtx.evt() % m_printFreq == 0 ) && msgLevel( MSG::DEBUG ) ) {
    debug() << m_schedulerPtr->buildAlgsWithStates( AlgStates ).str() << endmsg;
    debug() << m_schedulerPtr->buildPrintableStateTree( NodeStates ).str() << endmsg;
  }
  LHCb::HltDecReports reports{};
  reports.reserve( m_name_indices.size() );
  for ( size_t i = 0; i < m_name_indices.size(); ++i ) {
    reports.insert( m_line_names[i], {NodeStates[m_name_indices[i]].passed, 0, 0, 0, static_cast<int>( i )} );
  }
  return reports; // write down something better?
}
