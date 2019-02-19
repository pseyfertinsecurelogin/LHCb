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
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/MsgStream.h"

// local
#include "EvtTypeSvc.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>

namespace {
  // Function to compare evtcode of an evttype
  class EvtCodeEqual {
    int m_code;

  public:
    EvtCodeEqual( int code = 10000000 ) : m_code( code ) {}

    inline bool operator()( const EvtTypeInfo& type ) const { return ( type.evtCode() == m_code ); }
  };
} // namespace

//-----------------------------------------------------------------------------
// Implementation file for class : EvtTypeSvc
//
// 2004-06-24 : Gloria CORTI
//-----------------------------------------------------------------------------

// Instantiation of a static factory class used by clients to create
// instances of this service
DECLARE_COMPONENT( EvtTypeSvc )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
EvtTypeSvc::EvtTypeSvc( const std::string& name, ISvcLocator* svc ) : base_class( name, svc ) {
  // m_mcFinder=NULL; //don't make it unless needed

  // Default file to parse
  if ( getenv( "DECFILESROOT" ) ) { m_inputFile = std::string( getenv( "DECFILESROOT" ) ) + "/doc/table_event.txt"; }

  declareProperty( "EvtTypesFile", m_inputFile );
}

//=============================================================================
// Initialize
//=============================================================================
StatusCode EvtTypeSvc::initialize() {

  StatusCode status = Service::initialize();
  if ( status.isFailure() ) return status;

  // Check if the input file is set
  if ( "" == inputFile() ) {
    fatal() << "==> You MUST set the input file containing the event type" << endmsg
            << "    the default is set to $DECFILESROOT/doc/table_event.txt" << endmsg
            << "    check if $DECFILESROOT is set" << endmsg;
    return StatusCode::FAILURE;
  }

  return parseFile( inputFile() );
}

//=============================================================================
// Finalize
//=============================================================================
StatusCode EvtTypeSvc::finalize() {

  MsgStream msg( msgSvc(), name() );
  if ( msg.level() <= MSG::DEBUG ) {
    debug() << "==> Finalize" << endmsg;
    debug() << "Table size before clean up" << m_evtTypeInfos.size() << endmsg;
  }

  // Clean up list of evttypes
  m_evtTypeInfos.clear();

  if ( msg.level() <= MSG::DEBUG ) debug() << "Table size after clean up" << m_evtTypeInfos.size() << endmsg;

  return Service::finalize();
}

//=============================================================================
// Read data relative to event type from ASCII file
//=============================================================================
StatusCode EvtTypeSvc::parseFile( const std::string input ) {
  std::ifstream infile( input.c_str() );
  if ( !infile ) {
    fatal() << "File with event types information cannot be opened :" << input << endmsg;
    return StatusCode::FAILURE;
  }
  info() << "Opened file with the event types information : " << inputFile() << endmsg;

  std::string            token = " | ";
  long                   evtCode;
  std::string            line, sEvtCode, subLine, evtNick, evtDesc;
  std::string::size_type index0, index1;

  // Ignore the first line, since it is an header
  getline( infile, line, '\n' );

  // Now get all remaining lines in the file
  unsigned int nlines = 0;
  while ( getline( infile, line, '\n' ) ) {

    nlines++;

    index0 = line.find_first_of( token );
    if ( ( std::string::npos == index0 ) || ( 0 == index0 ) ) {
      error() << "Unexpected format" << endmsg;
      return StatusCode::FAILURE;
    }

    // This is the integer code
    sEvtCode = line.substr( 0, index0 );
    evtCode  = atoi( sEvtCode.c_str() );
    // Now get the nick name
    index0 = index0 + token.size();
    index1 = line.find_first_of( token, index0 );
    if ( ( std::string::npos == index1 ) || ( 0 == index1 ) ) {
      error() << "Unexpected format" << endmsg;
      return StatusCode::FAILURE;
    }
    evtNick = line.substr( index0, index1 - index0 );
    index1  = index1 + token.size();
    evtDesc = line.substr( index1 );
    m_evtTypeInfos.emplace_back( evtCode, evtNick, evtDesc );
  }

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "Read   " << nlines << " entries in file " << endmsg;
    debug() << "Stored " << m_evtTypeInfos.size() << " entries in table" << endmsg;
    if ( msgLevel( MSG::VERBOSE ) ) {
      verbose() << "List of entries: EvtType, NickName, DecayDescriptor" << std::endl;
      for ( const auto& iEntry : m_evtTypeInfos ) {
        verbose() << iEntry.evtCode() << token << iEntry.nickName() << token << iEntry.decayDescriptor() << std::endl;
      }
      verbose() << endmsg;
    }
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// Given the integer code of an Event type checks that it exists in the
// list of used values
//=============================================================================
bool EvtTypeSvc::typeExists( const int evtCode ) const {

  return std::any_of( m_evtTypeInfos.begin(), m_evtTypeInfos.end(), EvtCodeEqual( evtCode ) );
}

//=============================================================================
// Given the integer code of an Event type provides its Nick Name
//=============================================================================
std::string EvtTypeSvc::nickName( const int evtCode ) const {

  auto iEvtTypeInfo = std::find_if( m_evtTypeInfos.begin(), m_evtTypeInfos.end(), EvtCodeEqual( evtCode ) );
  if ( m_evtTypeInfos.end() == iEvtTypeInfo ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::WARNING << evtCode << "not known" << endmsg;
    return {};
  }
  return iEvtTypeInfo->nickName();
}

//=============================================================================
// Given an EventType provide its ASCII decay descriptor
//=============================================================================
std::string EvtTypeSvc::decayDescriptor( const int evtCode ) const {

  auto iEvtTypeInfo = std::find_if( m_evtTypeInfos.begin(), m_evtTypeInfos.end(), EvtCodeEqual( evtCode ) );
  if ( m_evtTypeInfos.end() == iEvtTypeInfo ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::WARNING << evtCode << "not known" << endmsg;
    return {};
  }
  return iEvtTypeInfo->decayDescriptor();
}

//=============================================================================
// Returns a LHCb::EventTypeSet of all known types
// this is a typedef of std::set<long unsigned int, std::greater<long unsigned int> >
// an ordered, unique, list
//=============================================================================
LHCb::EventTypeSet EvtTypeSvc::allTypes() const {
  LHCb::EventTypeSet types;
  std::transform( m_evtTypeInfos.begin(), m_evtTypeInfos.end(), std::inserter( types, types.end() ),
                  []( const EvtTypeInfo& eti ) { return eti.evtCode(); } );
  return types;
}
