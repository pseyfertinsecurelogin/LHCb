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
// Gaudi files
#include "GaudiKernel/SystemOfUnits.h"

// local
#include "OTReadOutWindow.h"

/** @file OTReadOutWindow.cpp
 *
 *  Implementation of OTReadOutWindow tool.
 *
 *  @author Jeroen van Tilburg
 *  @date   11/11/2002
 */

// Declaration of the tool Factory
DECLARE_COMPONENT( OTReadOutWindow )

StatusCode OTReadOutWindow::initialize() {
  StatusCode sc = GaudiTool::initialize();
  if ( sc.isFailure() ) return sc;

  if ( existDet<DataObject>( detSvc(), "Conditions/Calibration/OT/ReadOutGates" ) ) {
    try {
      registerCondition( "Conditions/Calibration/OT/ReadOutGates", condReadOutGate,
                         &OTReadOutWindow::condReadOutGateUpdate );
      StatusCode statusUpdate = runUpdate();
      if ( statusUpdate.isFailure() ) return Error( "Unable to update conditions from database.", statusUpdate );
    } catch ( GaudiException& ex ) {
      fatal() << ex << endmsg;
      return StatusCode::FAILURE;
    }
  }

  return StatusCode::SUCCESS;
}

StatusCode OTReadOutWindow::condReadOutGateUpdate() {
  std::vector<double> readoutGates = condReadOutGate->paramVect<double>( "ReadOutGates" );
  m_startReadOutGate[0]            = readoutGates[0];
  m_startReadOutGate[1]            = readoutGates[1];
  m_startReadOutGate[2]            = readoutGates[2];

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << " :: readOutGateT1 = " << readoutGates[0] << endmsg;
    debug() << " :: readOutGateT2 = " << readoutGates[1] << endmsg;
    debug() << " :: readOutGateT3 = " << readoutGates[2] << endmsg;
  }

  return StatusCode::SUCCESS;
}
