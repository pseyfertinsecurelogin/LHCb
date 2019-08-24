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
#include "ExecutionReportsWriter.h"

DECLARE_COMPONENT( ExecutionReportsWriter )

#define ON_DEBUG if ( msgLevel( MSG::DEBUG ) )
#define DEBUG_MSG ON_DEBUG debug()

#define ON_VERBOSE if ( msgLevel( MSG::VERBOSE ) )
#define VERBOSE_MSG ON_VERBOSE verbose()

StatusCode ExecutionReportsWriter::start() {
  auto sc = Gaudi::Algorithm::start();
  if ( !sc ) return sc;
  m_schedulerPtr = dynamic_cast<HLTControlFlowMgr*>( &*service<IEventProcessor>( "HLTControlFlowMgr", false ) );

  m_name_indices.reserve( m_line_names.size() );

  auto names_to_indices = m_schedulerPtr->getNodeNamesWithIndices();
  for ( std::string_view name : m_line_names ) {
    for ( auto const& [n, i] : names_to_indices ) {
      if ( n == name ) {
        m_name_indices.emplace_back( i );
        break;
      }
    }
  }
  if ( m_name_indices.size() != m_line_names.size() ) {
    error() << "Not all line names have been found, please check" << endmsg;
    return StatusCode::FAILURE;
  }
  return m_schedulerPtr ? StatusCode::SUCCESS : StatusCode::FAILURE;
}

StatusCode ExecutionReportsWriter::execute( EventContext const& evtCtx ) const {
  auto [NodeStates, AlgStates] = evtCtx.getExtension<HLTControlFlowMgr::SchedulerStates>();

  if ( UNLIKELY( evtCtx.evt() % m_printFreq == 0 ) ) {
    DEBUG_MSG << m_schedulerPtr->buildAlgsWithStates( AlgStates ).str() << endmsg;
    DEBUG_MSG << m_schedulerPtr->buildPrintableStateTree( NodeStates ).str() << endmsg;
  }
  LHCb::HltDecReports reports{};
  reports.reserve( m_name_indices.size() );
  for ( int i = 0; i < static_cast<int>( m_name_indices.size() ); ++i ) {
    reports.insert( m_line_names[i], {NodeStates[m_name_indices[i]].passed, 0, 0, 0, i} );
  }
  m_outputHandle.put( std::make_unique<decltype( reports )>( std::move( reports ) ) ); // write down something better?
  return StatusCode::SUCCESS;
}
