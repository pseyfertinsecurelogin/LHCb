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
#include "Event/HltLumiSummary.h"
#include "Event/LumiCounters.h"
#include "Event/LumiFSR.h"
#include "Event/ODIN.h"
#include "Event/RawEvent.h"

// local
#include "PropagateFSRs.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PropagateFSRs
//
// 2010-03-26 : Rob Lambert
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( PropagateFSRs )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PropagateFSRs::PropagateFSRs( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {
  declareProperty( "RawEventLocation", m_rawEventLocation = LHCb::RawEventLocation::Default );
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode PropagateFSRs::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  // get the File Records service
  m_fileRecordSvc = service( "FileRecordDataSvc", true );

  m_incSvc = service( "IncidentSvc", true );

  // check extended file incidents are defined
#ifdef GAUDI_FILE_INCIDENTS
  m_incSvc->addListener( this, IncidentType::BeginInputFile );
  if ( msgLevel( MSG::DEBUG ) ) debug() << "registered with incSvc" << endmsg;
    // if not then the counting is not reliable
#endif

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PropagateFSRs::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // if I don't know the GUID by now, get it from the event.
  if ( !m_knownFiles[m_filename] ) AddName2GUID( m_filename, GUIDinEvent() );

  // then check for all other filenames that weren't found yet
  for ( KnowGUIDMap::iterator kF = m_knownFiles.begin(); kF != m_knownFiles.end(); kF++ ) {
    if ( !kF->second ) file2GUID( kF->first );
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode PropagateFSRs::finalize() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Finalize" << endmsg;

  // create the directory structure, propagate the FSRs

  return GaudiAlgorithm::finalize(); // must be called after all other actions
}

// ==========================================================================
// IIncindentListener interface
// ==========================================================================
void PropagateFSRs::handle( const Incident& incident ) {
  // check extended file incidents are defined
#ifdef GAUDI_FILE_INCIDENTS
  if ( incident.type() == IncidentType::BeginInputFile ) {
    m_filename               = incident.source();
    m_knownFiles[m_filename] = false;
    // do I already know the GUID?
    file2GUID( m_filename );

    // Touch/Collect all FSRs, probably in a map of filename->FSR, or call another method to do it
  }
#endif
}

// ==========================================================================
// Copied from XMLSummarySvc
// ==========================================================================

std::string PropagateFSRs::file2GUID( const std::string& filename ) {
  // MsgStream log( msgSvc(), name() );

  FidMap::iterator j = m_fidMap.find( filename );
  if ( j != m_fidMap.end() ) return j->second;
  std::string shortname = AFN2name( filename );
  j                     = m_fidMap.find( shortname );
  if ( j != m_fidMap.end() ) return j->second;

  const Gaudi::IDataConnection* con = m_ioDataManager->connection( shortname );
  if ( !con ) con = m_ioDataManager->connection( shortname );
  if ( !con ) return "";
  AddName2GUID( filename, con->fid() );
  return con->fid();
}
std::string PropagateFSRs::AFN2name( const std::string& filename ) const {
  std::string me  = "LFN:";
  size_t      pos = filename.find( me );
  if ( ( pos == 0 ) && ( pos + me.size() < filename.size() ) ) return filename.substr( pos + me.size() );
  me  = "PFN:";
  pos = filename.find( me );
  if ( ( pos == 0 ) && ( pos + me.size() < filename.size() ) ) return filename.substr( pos + me.size() );
  return filename;
}
std::string PropagateFSRs::GUIDinEvent() const {
  LHCb::RawEvent* event = getIfExists<LHCb::RawEvent>( m_rawEventLocation );
  if ( event == NULL || event->registry() == NULL ) return "";

  IOpaqueAddress* eAddr = event->registry()->address();
  if ( eAddr == NULL ) return "";
  return eAddr->par()[0];
}
void PropagateFSRs::AddName2GUID( const std::string& filename, const std::string& GUID ) {
  std::string shortname  = AFN2name( filename );
  m_fidMap[filename]     = GUID;
  m_fidMap[shortname]    = GUID;
  m_knownFiles[filename] = true;
  return;
}
