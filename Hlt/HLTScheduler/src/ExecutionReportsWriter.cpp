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
  return m_schedulerPtr ? StatusCode::SUCCESS : StatusCode::FAILURE;
}

StatusCode ExecutionReportsWriter::execute( EventContext const& evtCtx ) const {
  auto [NodeStates, AlgStates] = evtCtx.getExtension<HLTControlFlowMgr::SchedulerStates>();

  if ( UNLIKELY( evtCtx.evt() % m_printFreq == 0 ) ) {
    DEBUG_MSG << m_schedulerPtr->buildAlgsWithStates( AlgStates ).str() << endmsg;
    DEBUG_MSG << m_schedulerPtr->buildPrintableStateTree( NodeStates ).str() << endmsg;
  }
  m_outputHandle.put( decltype( NodeStates ){NodeStates} ); // write down something better?
  return StatusCode::SUCCESS;
}
