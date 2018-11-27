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

#include "DetDesc/Condition.h"
#include "DetDesc/DetectorElement.h"

#include "GaudiKernel/Sleep.h"

// local
#include "TestTimeOut.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TestTimeOut
//
// 2006-08-31 : Marco CLEMENCIC
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( TestTimeOut )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TestTimeOut::TestTimeOut( const std::string& name,
                          ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty("SleepTime", m_sleepTime = 20);
}
//=============================================================================
// Destructor
//=============================================================================
TestTimeOut::~TestTimeOut() {} 

//=============================================================================
// Initialization
//=============================================================================
StatusCode TestTimeOut::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  debug() << "==> Initialize" << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode TestTimeOut::execute() {

  debug() << "==> Execute" << endmsg;
  info() << "Sleeping ..." << endmsg;
  Gaudi::Sleep(m_sleepTime);
  info() << "Try to access the db..." << endmsg;
  DetectorElement *lhcb = getDet<DetectorElement>( "/dd/Structure/LHCb" );
  
  Condition* alLHCb = lhcb->condition("Temperature");
  if (alLHCb) {
    info() << "Successfully retrieved condition '" << lhcb->condition("Temperature").path() << "'" << endmsg;
  }
  else {
    error() << "Cannot retrieve condition '" << lhcb->condition("Temperature").path() << "'" << endmsg;
    return StatusCode::FAILURE;
  }
  
  return StatusCode::SUCCESS;
}

//=============================================================================
