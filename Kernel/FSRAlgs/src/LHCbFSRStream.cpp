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
#define LHCB_FSRSTREAM_CPP

// Framework include files
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IOpaqueAddress.h"
#include "GaudiKernel/DataStoreItem.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "LHCbFSRStream.h"

// Define the algorithm factory for the standard output data writer
DECLARE_COMPONENT( LHCbFSRStream )

// Standard Constructor
LHCbFSRStream::LHCbFSRStream(const std::string& name, ISvcLocator* pSvcLocator)
  : LHCbRecordStream(name, pSvcLocator)
{
  declareProperty("FSRCleaningDirectory",m_cleanRoot="/FileRecords");
  declareProperty("CleanTree",m_cleanTree=true);
  declareProperty("AddIOFSR",m_doIOFsr=true);
}


StatusCode LHCbFSRStream::initialize()
{
  StatusCode sc=LHCbRecordStream::initialize();
  if (!sc.isSuccess() ) return sc;
  if (!m_doIOFsr) return sc;

  m_ioFsrSvc = serviceLocator()->service("IOFSRSvc");
  if( !m_ioFsrSvc )  {
    MsgStream log(msgSvc(), name());
    log << MSG::ERROR << "Error retrieving IOFSRSvc." << endmsg;
    return StatusCode::FAILURE;
  }

  return sc;
}


StatusCode LHCbFSRStream::finalize()
{
  MsgStream log(msgSvc(), name());
  if (m_doIOFsr)
  {

    //clean any existing top-level FSR
    StatusCode sc=m_ioFsrSvc->cleanTopFSR();
    //it probably isn't there...
    sc.ignore();


    //add the IOFSR to the TES
    sc=m_ioFsrSvc->storeIOFSR(m_outputName);
    if( !sc.isSuccess() )
    {
      log << MSG::ERROR << "Error storing IOFSR." << endmsg;
    }
  }

  //TODO cleanup the existing store, removing all empty directories
  if (m_cleanTree)
  {
    // Try and load the root DataObject for the configured stream
    SmartDataPtr<DataObject> root( m_pDataProvider, m_cleanRoot );

    // if found, recursively clean
    if ( root ) cleanNodes( root, m_cleanRoot );
  }

  m_ioFsrSvc.reset();
  return LHCbRecordStream::finalize();

}

void LHCbFSRStream::cleanNodes( DataObject * obj,
                                const std::string & location,
                                unsigned int nRecCount )
{
  MsgStream log(msgSvc(), name());
  // protect against infinite recursion
  if ( ++nRecCount > 99999 )
  {
    log << MSG::ERROR << "Maximum recursion limit reached...." << endmsg;
    return;
  }

  SmartIF<IDataManagerSvc> mgr( m_pDataManager );

  typedef std::vector<IRegistry*> Leaves;
  Leaves leaves;

  // Load the leaves
  StatusCode sc = mgr->objectLeaves( obj, leaves );
  if ( sc )
  {

    if ( !leaves.empty() )
    {
      for ( Leaves::const_iterator iL = leaves.begin(); leaves.end() != iL; ++iL )
      {
        const std::string & id = (*iL)->identifier();
        DataObject* tmp(NULL);
        sc = m_pDataProvider->findObject( id, tmp );
        if ( sc && tmp )
        {
          if ( CLID_DataObject == tmp->clID() )
          {
            cleanNodes( tmp, id, nRecCount );
          }
        }
      }
      // Load again, after cleaning, to see what remains
      sc = mgr->objectLeaves( obj, leaves );
    }

    if ( sc && leaves.empty() )
    {
      log << MSG::DEBUG << "Removing node " << location << endmsg;
      sc = m_pDataProvider->unlinkObject(location);
    }

  }

}
