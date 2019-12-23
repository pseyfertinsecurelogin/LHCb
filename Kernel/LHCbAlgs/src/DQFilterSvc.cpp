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
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/Incident.h"
#include "GaudiKernel/Service.h"
#include "Kernel/IAccept.h"

/** Simple service installing itself in the IncidentSvc to filter events on
 * DQ Flags via an IAccept tool (by default DQAcceptTool).
 *
 * @author Marco Clemencic
 * @date 2012-02-10
 */
class DQFilterSvc final : public extends<Service, IIncidentListener> {

public:
  /// Constructor
  DQFilterSvc( const std::string& name, ISvcLocator* svc );

  /// Initialize the service
  StatusCode initialize() override;

  /// Finalize the service
  StatusCode finalize() override;

  /// @see IIncidentListener::handle
  void handle( const Incident& ) override;

private:
  /// Type/Name of the (public) IAccept tool used to choose if the event has to
  /// be accepted or not (default: DQAcceptTool).
  std::string m_acceptToolName;

  /// Pointer to the IAccept tool.
  IAccept* m_acceptTool = nullptr;

  /// Pointer to the ToolSvc.
  SmartIF<IToolSvc> m_toolSvc;

  /// Pointer to the IncidentSvc.
  SmartIF<IIncidentSvc> m_incSvc;
};

DECLARE_COMPONENT( DQFilterSvc )

DQFilterSvc::DQFilterSvc( const std::string& name, ISvcLocator* svc ) : base_class( name, svc ) {
  declareProperty( "AcceptTool", m_acceptToolName = "DQAcceptTool", "IAccept Tool to filter the events." );
}

StatusCode DQFilterSvc::initialize() {
  StatusCode sc = Service::initialize();
  if ( sc.isFailure() ) return sc;

  if ( !( m_toolSvc = serviceLocator()->service( "ToolSvc" ) ) ) // assignment meant
  {
    error() << "Failed to retrieve ToolSvc" << endmsg;
    return StatusCode::FAILURE;
  }

  // Instantiate the public tool.
  // It is better to get it soon because it should require the UMS, and it must
  // be registered to the incident svc before us.
  sc = m_toolSvc->retrieveTool( m_acceptToolName, m_acceptTool );
  if ( sc.isFailure() ) return sc;

  // We must ensure that the UMS is up *before* registering to IncidentSvc to
  // have the correct order of calls.
  serviceLocator()->service( "UpdateManagerSvc" );
  if ( !( m_incSvc = serviceLocator()->service( "IncidentSvc" ) ) ) // assignment meant
  {
    error() << "Failed to retrieve Incident service." << endmsg;
    return StatusCode::FAILURE;
  }
  m_incSvc->addListener( this, IncidentType::BeginEvent );

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "DQFilterSvc/" << name() << " initialized:" << endmsg;
    debug() << "  filtering using " << m_acceptToolName << endmsg;
  }

  return sc;
}

StatusCode DQFilterSvc::finalize() {
  if ( m_incSvc ) { // we may not have retrieved the IncidentSvc
    m_incSvc->removeListener( this, IncidentType::BeginEvent );
    m_incSvc.reset();
  }

  if ( m_toolSvc ) { // we may not have retrieved the ToolSvc
    // Do not call releaseTool if the ToolSvc was already finalized.
    if ( SmartIF<IStateful>( m_toolSvc )->FSMState() > Gaudi::StateMachine::CONFIGURED ) {
      m_toolSvc->releaseTool( m_acceptTool ).ignore();
    } else {
      info() << "ToolSvc already finalized: cannot release tools. Check options." << endmsg;
    }
    m_acceptTool = nullptr;
    m_toolSvc.reset();
  }

  return Service::finalize();
}

void DQFilterSvc::handle( const Incident& ) {
  const bool accepted = m_acceptTool->accept();
  if ( msgLevel( MSG::VERBOSE ) ) {
    verbose() << "Handling incident: " << ( ( accepted ) ? "good" : "bad" ) << " event" << endmsg;
  }
  if ( !accepted ) { m_incSvc->fireIncident( Incident( name(), IncidentType::AbortEvent ) ); }
}
