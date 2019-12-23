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
/*
 * DQFilter.h
 *
 *  Created on: Jan 31, 2012
 *      Author: marcocle
 */

#include "DetDesc/Condition.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/Incident.h"
#include "GaudiKernel/ToolHandle.h"
#include "Kernel/IAccept.h"

/** @class DQFilter
 *  Small algorithm to filter events according to the Data Quality flags stored
 *  in the conditions database on a run-by-run basis.
 *
 *  When initialized, the algorithm register itself as user of the DQ Flags
 *  conditions and, by default, it listens for BeginEvent incidents.
 *
 *  @author Marco Clemencic
 *  @date   Jan 31, 2012
 */
class DQFilter final : public extends<GaudiAlgorithm, IIncidentListener> {

public:
  /// Standard constructor
  DQFilter( const std::string& name, ISvcLocator* pSvcLocator );

  /// Initialize the algorithm.
  /// Register to the UpdateManagerSvc as user of the Data Quality flags condition
  /// and, if not requested otherwise, register as BeginEvent listener.
  StatusCode initialize() override;
  /// By default it does nothing, but if requested not to use the BeginEvent,
  /// it sets the
  StatusCode execute() override;
  /// If registered as BeginEvent listener, unregister.
  StatusCode finalize() override;

  void handle( const Incident& ) override;

private:
  /// Flag to state if we have to filter at the level of the Begin Event
  /// incident or during the execute.
  bool m_beginEvent;

  /// Type/Name of the (public) IAccept tool used to choose if the event has to
  /// be accepted or not (default: DQAcceptTool).
  std::string m_acceptToolName;

  /// Pointer to the IAccept tool.
  IAccept* m_acceptTool = nullptr;

  /// Pointer to the IncidentSvc.
  SmartIF<IIncidentSvc> m_incSvc;
};

DQFilter::DQFilter( const std::string& name, ISvcLocator* pSvcLocator ) : base_class( name, pSvcLocator ) {
  declareProperty( "UseBeginEvent", m_beginEvent = true,
                   "If set to true, the filtering is done at the level of the "
                   "BeginEvent incident." );
  declareProperty( "AcceptTool", m_acceptToolName = "DQAcceptTool", "IAccept Tool to filter the events." );
}

StatusCode DQFilter::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize();
  if ( sc.isFailure() ) return sc;

  // Instantiate the public tool.
  // It is better to get it soon because it should require the UMS, and it must
  // be registered to the incident svc before us.
  m_acceptTool = tool<IAccept>( m_acceptToolName );

  // We must ensure that the UMS is up *before* registering to IncidentSvc to
  // have the correct order of calls.
  if ( m_beginEvent ) {
    updMgrSvc();
    m_incSvc = service( "IncidentSvc" );
    if ( !m_incSvc ) {
      error() << "Failed to load IncidentSvc" << endmsg;
      return StatusCode::FAILURE;
    }

    m_incSvc->addListener( this, IncidentType::BeginEvent );
  }

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "DQFilter/" << name() << " initialized:" << endmsg;
    debug() << "  filtering on " << ( ( m_beginEvent ) ? "BeginEvent" : "execute" ) << endmsg;
    debug() << "  using " << m_acceptToolName << endmsg;
  }
  return sc;
}

StatusCode DQFilter::execute() {
  const bool accepted = m_acceptTool->accept();
  // Print the message only if we do not use the BeginEvent incident
  // otherwise it gets printed twice for good events.
  if ( ( !m_beginEvent ) && ( msgLevel() <= MSG::VERBOSE ) ) {
    verbose() << "Filter event: " << ( ( accepted ) ? "good" : "bad" ) << " event" << endmsg;
  }
  setFilterPassed( accepted );
  return StatusCode::SUCCESS;
}

StatusCode DQFilter::finalize() {
  if ( m_incSvc ) {
    m_incSvc->removeListener( this, IncidentType::BeginEvent );
    m_incSvc.reset();
  }
  if ( m_acceptTool ) {
    releaseTool( m_acceptTool ).ignore();
    m_acceptTool = nullptr;
  }

  return GaudiAlgorithm::finalize();
}

void DQFilter::handle( const Incident& ) {
  const bool accepted = m_acceptTool->accept();
  if ( msgLevel() <= MSG::VERBOSE ) {
    verbose() << "Handling incident: " << ( ( accepted ) ? "good" : "bad" ) << " event" << endmsg;
  }
  if ( !accepted ) { m_incSvc->fireIncident( Incident( name(), IncidentType::AbortEvent ) ); }
}

DECLARE_COMPONENT( DQFilter )
