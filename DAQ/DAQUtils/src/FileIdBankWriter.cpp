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
#include "Event/FileId.h"
#include "Event/RawEvent.h"
#include "Gaudi/Accumulators.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IOpaqueAddress.h"
#include "GaudiKernel/IRegistry.h"

//-----------------------------------------------------------------------------
// Implementation file for class : FileIdBankWriter
//
// 2009-10-01 : Jaap Panman
//-----------------------------------------------------------------------------

/** @class FileIdBankWriter FileIdBankWriter.h
 *
 *
 *  @author Jaap Panman
 *  @date   2009-10-01
 */
class FileIdBankWriter : public GaudiAlgorithm {
public:
  /// Standard constructor
  FileIdBankWriter( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

private:
  Gaudi::Property<std::string> m_rawEventLocation{
      this, "RawEventLocation", LHCb::RawEventLocation::Default};             // Location where we get the RawEvent
  std::string                    m_current_fname;                             // current file ID string
  Gaudi::Accumulators::Counter<> m_count_files{this, "number of files seen"}; // number of files read

  std::vector<unsigned int> m_bank;

  LHCb::FileId m_fileId;
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FileIdBankWriter )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
FileIdBankWriter::FileIdBankWriter( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {}
//=============================================================================
// Initialization
//=============================================================================
StatusCode FileIdBankWriter::initialize() {
  return GaudiAlgorithm::initialize().andThen( [&] {
    // file counting
    m_current_fname.clear();
    m_count_files.reset();
    m_bank.reserve( 10 ); // for output
  } );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode FileIdBankWriter::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // registry from raw data - only correct if file catalogue used
  std::string     event_fname;
  LHCb::RawEvent* event = getIfExists<LHCb::RawEvent>( m_rawEventLocation );
  if ( !event ) {
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
  if ( msgLevel( MSG::INFO ) ) { info() << "number of files seen: " << m_count_files.value() << endmsg; }
  return GaudiAlgorithm::finalize(); // must be called after all other actions
}

//=============================================================================
