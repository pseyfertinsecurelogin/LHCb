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
#include "EmptyEventNodeCleaner.h"

//-----------------------------------------------------------------------------
// Implementation file for class : EmptyEventNodeCleaner
//
// 2012-01-31 : Chris Jones
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
EmptyEventNodeCleaner::EmptyEventNodeCleaner( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {
  declareProperty( "InputStream", m_inputStream = "/Event" );
  declareProperty( "DataService", m_dataSvcName = "EventDataSvc" );
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode EmptyEventNodeCleaner::initialize() {
  return GaudiAlgorithm::initialize().andThen( [&] {
    // get the File Records service
    m_dataSvc = svc<IDataProviderSvc>( m_dataSvcName, true );
  } );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode EmptyEventNodeCleaner::execute() {
  // Try and load the root DataObject for the configured stream
  SmartDataPtr<DataObject> root( m_dataSvc, m_inputStream );

  // if found, recursively clean
  if ( root ) cleanNodes( root, m_inputStream );

  // return
  return StatusCode::SUCCESS;
}

//=============================================================================

void EmptyEventNodeCleaner::cleanNodes( DataObject* obj, const std::string& location, unsigned int nRecCount ) {
  // protect against infinite recursion
  if ( ++nRecCount > 99999 ) {
    Error( "Maximum recursion limit reached...." ).ignore();
    return;
  }

  SmartIF<IDataManagerSvc> mgr( m_dataSvc );
  std::vector<IRegistry*>  leaves;

  // Load the leaves
  StatusCode sc = mgr->objectLeaves( obj, leaves );
  if ( sc ) {

    if ( !leaves.empty() ) {
      for ( auto iL = leaves.begin(); leaves.end() != iL; ++iL ) {
        const std::string& id = ( *iL )->identifier();
        DataObject*        tmp( nullptr );
        sc = m_dataSvc->findObject( id, tmp );
        if ( sc && tmp ) {
          if ( CLID_DataObject == tmp->clID() ) { cleanNodes( tmp, id, nRecCount ); }
        }
      }
      // Load again, after cleaning, to see what remains
      sc = mgr->objectLeaves( obj, leaves );
    }

    if ( sc && leaves.empty() ) {
      if ( msgLevel( MSG::DEBUG ) ) debug() << "Removing node " << location << endmsg;
      sc = m_dataSvc->unlinkObject( location );
    }
  }
}

//=============================================================================

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( EmptyEventNodeCleaner )

//=============================================================================
