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
#include "RunChangeHandlerSvc.h"

DECLARE_COMPONENT( RunChangeHandlerSvc )

#define ON_DEBUG if ( msgLevel( MSG::DEBUG ) )
#define DEBUG_MSG ON_DEBUG debug()

//-----------------------------------------------------------------------------
// Implementation file for class : RunChangeHandlerSvc
//
// 2008-07-24 : Marco Clemencic
//-----------------------------------------------------------------------------

//=============================================================================
// Initialize
//=============================================================================
StatusCode RunChangeHandlerSvc::initialize() {
  // base class initialization
  StatusCode sc = Service::initialize();
  if ( !sc.isSuccess() ) return sc;

  // local initialization
  DEBUG_MSG << "--- initialize ---" << endmsg;

  incidentSvc()->addListener( this, IncidentType::RunChange );
  // ensure that we can call evtProc() and updMgrSvc() while in handle
  evtProc();
  updMgrSvc();

  // Prepare the list of conditions
  m_conditions.reserve( m_condDesc.size() );
  for ( const auto& condDesc : m_condDesc ) {
    m_conditions.emplace_back( detectorSvc(), condDesc.first, condDesc.second );
    updMgrSvc()->registerCondition( this, condDesc.first );
  }
  // FIXME: (MCl) This is a hack to be sure that the UMS knows about all the
  // objects we have to modify before we get to the first event.
  return updMgrSvc()->update( this );
}

//=============================================================================
// Finalize
//=============================================================================
StatusCode RunChangeHandlerSvc::finalize() {
  // local finalization
  DEBUG_MSG << "--- finalize ---" << endmsg;

  if ( m_incSvc ) incidentSvc()->removeListener( this, IncidentType::RunChange );

  // release acquired interfaces
  m_evtSvc.reset();
  m_detSvc.reset();
  m_incSvc.reset();
  m_ums.reset();
  m_evtProc.reset();

  // base class finalization
  return Service::finalize();
}

//=========================================================================
// Handle RunChange incident
//=========================================================================
void RunChangeHandlerSvc::handle( const Incident& inc ) {
  DEBUG_MSG << inc.type() << " incident received" << endmsg;

  const RunChangeIncident* rci = dynamic_cast<const RunChangeIncident*>( &inc );
  if ( !rci ) {
    error() << "Cannot dynamic_cast the incident to RunChangeIncident, "
               "run change ignored"
            << endmsg;
    return;
  }

  if ( m_currentRun != rci->runNumber() ) {
    ON_DEBUG {
      DEBUG_MSG << "Change of run number detected " << m_currentRun;
      DEBUG_MSG << "->" << rci->runNumber() << endmsg;
    }
  } else {
    DEBUG_MSG << "Update requested without change of run (flushing XML cache)." << endmsg;
    xmlParser()->clearCache();
  }

  m_currentRun = rci->runNumber();
  // update objects
  update( m_currentRun );
}

//=========================================================================
// Flag for update all conditions
//=========================================================================
void RunChangeHandlerSvc::update( unsigned long run ) {
  std::lock_guard lock( m_updateMutex ); // we cannot run this concurrently
  FileHasher      hasher;
  std::for_each( m_conditions.begin(), m_conditions.end(), [this, run, &hasher]( CondData& cond ) {
    try {
      const bool update = cond.needsUpdate( run, hasher, m_forceUpdate );
      if ( update ) {
        // notify the UMS and the object that they have to be updated.
        updMgrSvc()->invalidate( cond.object.ptr() );
      }
    } catch ( const std::exception& x ) {
      // something went wrong, so we change the exception type and rethrow
      evtProc()->stopRun().ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ ); // schedule a stop
      throw GaudiException( x.what(), name(), StatusCode::FAILURE );
    }
  } );
}
