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

// from Gaudi
#include "GaudiKernel/IOpaqueAddress.h"
#include "GaudiKernel/IRegistry.h"

// event model
#include "Event/FileId.h"
#include "Event/RawEvent.h"

// local
#include "FileIdBankWriter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : FileIdBankWriter
//
// 2009-10-01 : Jaap Panman
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FileIdBankWriter )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
FileIdBankWriter::FileIdBankWriter( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ), m_count_files( 0 ) {
  declareProperty( "RawEventLocation", m_rawEventLocation = LHCb::RawEventLocation::Default );
}
//=============================================================================
// Initialization
//=============================================================================
StatusCode FileIdBankWriter::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  // file counting
  m_current_fname = "";
  m_count_files   = 0;

  // for output
  m_bank.reserve( 10 );

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode FileIdBankWriter::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // registry from raw data - only correct if file catalogue used
  std::string     event_fname;
  LHCb::RawEvent* event = getIfExists<LHCb::RawEvent>( m_rawEventLocation );
  if ( event == NULL ) {
    return Warning( "RawBank cannot be loaded", StatusCode::SUCCESS );
  } else {
    IOpaqueAddress* eAddr = event->registry()->address();
    // obtain the fileID
    if ( eAddr ) {
      event_fname = eAddr->par()[0];
      if ( msgLevel( MSG::DEBUG ) ) debug() << "RunInfo record: " << event_fname << endmsg;
    } else {
      return Error( "Registry cannot be loaded from Event", StatusCode::SUCCESS );
    }
  }

  // check if the file ID is new
  if ( event_fname != m_current_fname ) {
    m_count_files++; // this is a new file
    m_current_fname = event_fname;
    if ( msgLevel( MSG::INFO ) ) info() << "RunInfo record: " << event_fname << endmsg;
    // calculate identifier
    m_bank = m_fileId.fileIDstr2int( event_fname );
  }
  // add now to the raw data: set source, type, version
  event->addBank( 0, LHCb::RawBank::FileID, 0, m_bank );

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode FileIdBankWriter::finalize() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Finalize" << endmsg;

  if ( msgLevel( MSG::INFO ) ) { info() << "number of files seen: " << m_count_files << endmsg; }
  return GaudiAlgorithm::finalize(); // must be called after all other actions
}

//=============================================================================
