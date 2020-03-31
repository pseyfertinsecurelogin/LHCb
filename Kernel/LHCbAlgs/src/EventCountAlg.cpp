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
// Include files
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include <string>
//-----------------------------------------------------------------------------
// Implementation file for class : EventCountAlg
//
// 2007-01-08 : Robert Lambert
//-----------------------------------------------------------------------------

/** @class EventCountAlg EventCountAlg.h
 *
 *  This is a simple alg with an efficiency counter.
 *  It will presumably replace EventCountHisto for
 *  most users of the XML summary service.
 *  The property CounterName can be used to set the
 *  name of the counter at configuration time.
 *
 *  The number of events, and the number of times
 *  this algorithm are called are used to calculate
 *  the efficiency
 *
 *  @author Robert Lambert
 *  @date   2007-01-08
 */
class EventCountAlg final : public GaudiAlgorithm, virtual public IIncidentListener {

public:
  /// Standard constructor
  EventCountAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

  /// IListener interface to be triggered at begin of event
  void handle( const Incident& ) override { ++m_nHandled; }

private:
  /// two integers are used, and a StatEntity isn't defined
  /// until the last moment.
  unsigned long long    m_nHandled  = 0; ///< total events
  unsigned long long    m_nExecuted = 0; ///< number of events reaching this point
  std::string           m_counterName;   ///< name of counter, set by option CounterName
  SmartIF<IIncidentSvc> m_incSvc;        ///< the incident service
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( EventCountAlg )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
EventCountAlg::EventCountAlg( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {
  // declare the filename to write. If the string is empty, no file will be written
  declareProperty( "CounterName", m_counterName = "efficiency" );
  // I know this is a stat entity, make sure it's written as such
  StatusCode sc = setProperty( "StatEntityList", "[ \".*\" ]" );
  // m_statEntityList = {".*"};
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode EventCountAlg::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize();
  if ( sc.isFailure() ) return sc;

  // prepare the incident service
  m_incSvc = service( "IncidentSvc", false );
  if ( !m_incSvc ) return StatusCode::FAILURE;
  m_incSvc->addListener( this, IncidentType::BeginEvent );
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Initialize" << endmsg;

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode EventCountAlg::execute() {
  ++m_nExecuted;
  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode EventCountAlg::finalize() {
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Finalize" << endmsg;

  counter( m_counterName ) =
      StatEntity( m_nHandled, m_nExecuted, m_nExecuted, int( m_nHandled > m_nExecuted ), int( m_nExecuted != 0 ) );

  return GaudiAlgorithm::finalize();
}

//=============================================================================
