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
#include "Event/CrossSectionsFSR.h"
#include "Event/GenCountersFSR.h"
#include "Event/GenFSR.h"
#include "FSRAlgs/IFSRNavigator.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

//-----------------------------------------------------------------------------
// Implementation file for class : GenFSRRead
//
// 2015-06-23 : Davide Fazzini
//-----------------------------------------------------------------------------

/** @class GenFSRRead GenFSRRead.h
 *
 *
 *  @author Davide Fazzini
 *  @date   2015-06-23
 */

class GenFSRRead : public GaudiAlgorithm {
public:
  // Standard constructor
  GenFSRRead( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; // Algorithm initialization
  StatusCode execute() override;    // Algorithm execution
  StatusCode finalize() override;   // Algorithm finalization

  void printFSR(); // Print the GenFSR

private:
  Gaudi::Property<std::string> m_fileRecordName{this, "FileRecordLocation", "/FileRecords",
                                                "TES location where FSRs are persisted"};
  Gaudi::Property<std::string> m_FSRName{this, "FSRName", "/GenFSR", "Name of the genFSR tree"};

  SmartIF<IDataProviderSvc> m_fileRecordSvc;
  IFSRNavigator*            m_navigatorTool = nullptr; // tool to navigate FSR
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( GenFSRRead )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
GenFSRRead::GenFSRRead( const std::string& name, ISvcLocator* pSvcLocator ) : GaudiAlgorithm( name, pSvcLocator ) {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode GenFSRRead::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first

  if ( sc.isFailure() ) return sc; // error printed already by GaudiAlgorithm
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  // get the File Records service
  m_fileRecordSvc = Gaudi::svcLocator()->service( "FileRecordDataSvc" );
  m_navigatorTool = tool<IFSRNavigator>( "FSRNavigator", "FSRNavigator" );

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode GenFSRRead::execute() { return StatusCode::SUCCESS; }

//=============================================================================
//  Finalize
//=============================================================================
StatusCode GenFSRRead::finalize() {
  GenFSRRead::printFSR();
  return GaudiAlgorithm::finalize(); // must be called after all other actions
}

//=============================================================================

//=============================================================================
//  Print the GenFSR informations
//=============================================================================

void GenFSRRead::printFSR() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Print the genFSR informations!" << endmsg;

  std::vector<std::string> addresses = m_navigatorTool->navigate( m_fileRecordName, m_FSRName );

  for ( const auto& genRecordAddress : addresses ) {
    // read GenFSR
    DataObject* obj = nullptr;
    StatusCode  sc  = m_fileRecordSvc->retrieveObject( genRecordAddress, obj );

    if ( !sc.isSuccess() ) {
      error() << "Unable to retrieve object '" << genRecordAddress << "'" << endmsg;
      continue;
    }

    LHCb::GenFSR* genFSR = dynamic_cast<LHCb::GenFSR*>( obj );

    if ( genFSR == nullptr ) {
      warning() << "genFSR record not found" << endmsg;
      if ( msgLevel( MSG::DEBUG ) ) debug() << genRecordAddress << " not found" << endmsg;
    } else {
      always() << "READ FSR: " << genRecordAddress << endmsg;
      always() << *genFSR << endmsg;
      always() << "**********************************   " << endmsg;
    }
    always() << "*****************************************************" << endmsg;
  }
}
