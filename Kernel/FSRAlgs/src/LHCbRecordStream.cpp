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
#define LHCB_RECORDSTREAM_CPP

// Framework include files
#include "LHCbRecordStream.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/DataStoreItem.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IOpaqueAddress.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/MsgStream.h"

// Define the algorithm factory for the standard output data writer
DECLARE_COMPONENT( LHCbRecordStream )

// Standard Constructor
LHCbRecordStream::LHCbRecordStream( const std::string& name, ISvcLocator* pSvcLocator )
    : LHCbOutputStream( name, pSvcLocator ) {
  /// in the baseclass, always fire the incidents by default
  /// in LHCbRecordStream this will be set to false, and configurable
  m_fireIncidents = false;
  declareProperty( "FireIncidents", m_fireIncidents = false );
}

StatusCode LHCbRecordStream::finalize() {
  MsgStream log( msgSvc(), name() );
  log << MSG::INFO << "Set up File Summary Record" << endmsg;
  if ( !m_fireIncidents && log.level() <= MSG::VERBOSE ) log << MSG::VERBOSE << "will not fire incidents" << endmsg;
  StatusCode sc = LHCbOutputStream::execute();
  if ( !sc.isSuccess() ) { log << MSG::WARNING << "Error writing run summary record....." << endmsg; }
  return LHCbOutputStream::finalize();
}
