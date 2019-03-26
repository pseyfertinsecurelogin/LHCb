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
#include "DetDesc/RunChangeIncident.h"
#include "GaudiKernel/IDetDataSvc.h"
#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/IFileAccess.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/Service.h"

#define ON_DEBUG if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
#define DEBUG_MSG ON_DEBUG debug()

/** Simple service to check if the run stamp condition exists for the current
 *  event.
 *
 *  To ensure that the content of the conditions database includes alignments
 *  and calibrations for the event being processed, when these conditions are
 *  stored we also store a special condition with closed Interval Of Validity
 *  (IOV) covering only the run for which they are valid (RunStamp condition).
 *
 *  The time line of the RunStamp conditions is not completely covered, and the
 *  holes in the time line implicitly flag the events for which the alignments
 *  are not available.
 *
 *  When RunStampCheck is instantiated in a Gaudi application, it checks for
 *  each event time if the RunStamp condition exists or not, in which case the
 *  application is terminated with an error code.
 *
 *  So, to enable the check, it is enough to add to the options:
 *  \code{.py}
 *  from Configurables import CondDB
 *  CondDB().EnableRunStampCheck = True
 *  \endcode
 *
 *  \see https://its.cern.ch/jira/browse/LBCORE-831
 *  \see https://its.cern.ch/jira/browse/LHCBPS-1421
 */
class RunStampCheck : public extends<Service, IIncidentListener> {
public:
  /// Constructor. Declares properties.
  using extends::extends;

  /// Connect to the required services and register as BeginEvent listener.
  StatusCode start() override {
    StatusCode sc = Service::start();
    if ( UNLIKELY( !sc ) ) return sc;

    // ensure that we have the EventClocksvc (to get the current event time in
    // the DetectorDataSvc).
    if ( UNLIKELY( !serviceLocator()->service( "EventClockSvc" ) ) ) {
      error() << "Cannot get EventClockSvc" << endmsg;
      return StatusCode::FAILURE;
    }

    m_incSvc = serviceLocator()->service( "IncidentSvc" );
    if ( UNLIKELY( !m_incSvc ) ) {
      error() << "Cannot get IncidentSvc" << endmsg;
      return StatusCode::FAILURE;
    }
    m_incSvc->addListener( this, IncidentType::RunChange );

    m_fileAccess = serviceLocator()->service( "VFSSvc" );
    if ( UNLIKELY( !m_fileAccess ) ) {
      error() << "Cannot get VFSSvc" << endmsg;
      return StatusCode::FAILURE;
    }

    DEBUG_MSG << "opening " << m_validRunsList.value() << endmsg;
    auto data = m_fileAccess->open( m_validRunsList );
    if ( !data || !data->good() ) {
      error() << "Cannot access " << m_validRunsList.value() << endmsg;
      return StatusCode::FAILURE;
    }

    std::string  line;
    unsigned int run;
    while ( std::getline( *data, line ) ) {
      std::istringstream is{line};
      is >> run;
      m_validRuns.emplace( run );
    }
    DEBUG_MSG << "loaded " << m_validRuns.size() << " valid run numbers" << endmsg;

    m_evtProc = serviceLocator();
    if ( UNLIKELY( !m_evtProc ) ) {
      error() << "Cannot get IEventProcessor" << endmsg;
      return StatusCode::FAILURE;
    }

    return sc;
  }
  /// Deregister as BeginEvent listener and release reference to services.
  StatusCode stop() override {
    m_incSvc->removeListener( this, IncidentType::RunChange );
    m_incSvc.reset();
    m_fileAccess.reset();
    m_evtProc.reset();
    return Service::stop();
  }
  /// Handle the BeginEvent incident to check if the RunStamp condition exists.
  void handle( const Incident& inc ) override {
    DEBUG_MSG << inc.type() << " incident received" << endmsg;

    const RunChangeIncident* rci = dynamic_cast<const RunChangeIncident*>( &inc );
    if ( !rci ) {
      error() << "Cannot dynamic_cast the incident to RunChangeIncident, "
                 "run change ignored"
              << endmsg;
      return;
    }

    const auto when = rci->eventTime();

    if ( m_validRuns.find( rci->runNumber() ) == end( m_validRuns ) ) {
      error() << "Database not up-to-date. No valid data for run " << rci->runNumber() << " at "
              << when.format( false, "%Y-%m-%d %H:%M:%S" ) << "." << when.nanoformat() << " UTC" << endmsg;
      m_evtProc->stopRun();
    }
  }

private:
  Gaudi::Property<std::string> m_validRunsList{this, "ValidRunsList", "",
                                               "URI (for VFSSvc) to a file with the list of valid runs. "
                                               "If this option is set, RunStamp and CondDBReader are ignored"};

  /// reference to the incident service.
  SmartIF<IIncidentSvc> m_incSvc;
  /// reference to the VFSSvc.
  SmartIF<IFileAccess> m_fileAccess;
  /// reference to the event processor.
  SmartIF<IEventProcessor> m_evtProc;

  std::set<unsigned int> m_validRuns;
};

DECLARE_COMPONENT( RunStampCheck )
