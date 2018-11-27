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

// local
#include "EventCountAlg.h"

//-----------------------------------------------------------------------------
// Implementation file for class : EventCountAlg
//
// 2007-01-08 : Robert Lambert
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( EventCountAlg )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
EventCountAlg::EventCountAlg( const std::string& name,
                      ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  //declare the filename to write. If the string is empty, no file will be written
  declareProperty("CounterName",m_counterName="efficiency");
  //I know this is a stat entity, make sure it's written as such
  StatusCode sc = setProperty ( "StatEntityList" , "[ \".*\" ]" );
  // m_statEntityList = {".*"};
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode EventCountAlg::initialize() 
{
  StatusCode sc = GaudiAlgorithm::initialize();
  if ( sc.isFailure() ) return sc;

  //prepare the incident service
  m_incSvc = service("IncidentSvc", false);
  if(!m_incSvc) return StatusCode::FAILURE;
  m_incSvc->addListener( this, IncidentType::BeginEvent);
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Initialize" << endmsg;

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode EventCountAlg::execute() 
{
  ++m_nExecuted;

  setFilterPassed(true);   // Mandatory. Set to true if event is accepted.
  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode EventCountAlg::finalize() 
{
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Finalize" << endmsg;
 
  counter(m_counterName) = StatEntity( m_nHandled, m_nExecuted,
                                       m_nExecuted, int(m_nHandled>m_nExecuted),
                                       int(m_nExecuted!=0));

  return GaudiAlgorithm::finalize();
}

//=============================================================================
