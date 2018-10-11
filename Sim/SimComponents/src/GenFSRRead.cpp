/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
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
#include "GenFSRRead.h"

// from Event
#include "Event/GenFSR.h"
#include "Event/GenCountersFSR.h"
#include "Event/CrossSectionsFSR.h"

// to write a file
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

//-----------------------------------------------------------------------------
// Implementation file for class : GenFSRRead
//
// 2015-06-23 : Davide Fazzini
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( GenFSRRead )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
GenFSRRead::GenFSRRead( const std::string& name,
                          ISvcLocator* pSvcLocator)
: GaudiAlgorithm ( name , pSvcLocator )
{
  // expect the data to be written at LHCb::GenFSRLocation::Default
  declareProperty( "FileRecordLocation" , m_fileRecordName = "/FileRecords"        );
  declareProperty( "FSRName"            , m_FSRName        = "/GenFSR"             );
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode GenFSRRead::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  // get the File Records service
  m_fileRecordSvc = service("FileRecordDataSvc", true);

  // prepare navigator tool
  m_navigatorTool = tool<IFSRNavigator>("FSRNavigator", "FSRNavigator");

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode GenFSRRead::execute() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode GenFSRRead::finalize() {

  if (msgLevel(MSG::DEBUG)) debug() << "==> Finalize" << endmsg;

  GenFSRRead::dumpFSR();

  return GaudiAlgorithm::finalize();  // must be called after all other actions
}

//=============================================================================


//=============================================================================
//  Dump the GenFSR informations
//=============================================================================

void GenFSRRead::dumpFSR()
{
  // root of store
  std::string fileRecordRoot = m_fileRecordName;
  // make an inventory of the FileRecord store
  std::vector< std::string > addresses = m_navigatorTool->navigate(fileRecordRoot, m_FSRName);

  int nFSRs = 0;

  for(std::vector< std::string >::iterator iAddr=addresses.begin(); iAddr!=addresses.end(); iAddr++)
    nFSRs += 1;

  if(nFSRs != 0)
  {
    always() << "Dump the genFSR informations!" << endmsg;

    for(std::vector< std::string >::iterator iAddr=addresses.begin(); iAddr!=addresses.end(); iAddr++)
    {
      std::string genRecordAddress = *iAddr;
      always() << "Address : " << genRecordAddress << endmsg;

      // read GenFSR
      LHCb::GenFSR* genFSR = getIfExists<LHCb::GenFSR>(m_fileRecordSvc.get(), genRecordAddress);

      if (genFSR==NULL)
      {
        Warning("A genFSR record was not found").ignore();
        if (msgLevel(MSG::DEBUG)) debug() << genRecordAddress << " not found" << endmsg;
      }
      else
      {
        always() << "READ FSR: " << genRecordAddress << endmsg;
        always() << *genFSR << endmsg;
        always() << "**********************************   " << endmsg;
      }
      always() << "*****************************************************" << endmsg;
    }
  }
  else
    if (msgLevel(MSG::DEBUG)) debug() << "There are not FSRs to write" << endmsg;

}

