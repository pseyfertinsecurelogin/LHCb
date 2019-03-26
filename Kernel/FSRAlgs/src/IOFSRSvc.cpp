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
#include "IOFSRSvc.h"
#include "GaudiKernel/IAppMgrUI.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiUtils/IIODataManager.h"
#include "GaudiUtils/RegEx.h"
#include <algorithm>
//#include "GaudiUtils/IODataManager.h"
// With regex libraries

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLARE_COMPONENT( IOFSRSvc )

using namespace std;

///////////////////////////////////////////////////////////////////////////
// Constructor/Destructor
///////////////////////////////////////////////////////////////////////////

IOFSRSvc::IOFSRSvc( const std::string& name, ISvcLocator* svc )
    : base_class( name, svc )
    , m_ioDataManager( 0 )
    , m_hasinput( false )
    , m_eventCount( 0 )
    , m_navigatorTool( 0 )
    , m_fileRecordSvc( 0 )
    , m_trace( LHCb::IOFSR::StatusFlag::UNCHECKED )
    , m_merged( false ) {
  declareProperty( "BeginEventIncident", m_beginIncident = IncidentType::BeginEvent );
  declareProperty( "EndEventIncident", m_endIncident = IncidentType::EndEvent );
  declareProperty( "FileRecordLocation", m_FileRecordName = "/FileRecords" );
  declareProperty( "FSRName", m_FSRName = "/IOFSR" );
  declareProperty( "OverrideStatus", m_overrideStatus = false );
  declareProperty( "PrintIOFSR", m_printIOFSR = false );
  // The status to start with if nothing else is known
  declareProperty( "DefaultStatus", m_defaultStatusStr = "UNCHECKED" );

  // declareProperty( "NavigatorToolName"  , m_ToolName          = "FSRNavigator"   );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

///////////////////////////////////////////////////////////////////////////
// Gaudi Parts
///////////////////////////////////////////////////////////////////////////

StatusCode IOFSRSvc::initialize() {

  StatusCode sc = Service::initialize();
  if ( !sc.isSuccess() ) return sc;

  debug() << "initialize" << endmsg;

  // prepare the incident service
  sc = prepareIncSvc();
  if ( !sc.isSuccess() ) warning() << "could not init incSvc" << endmsg;

  // prepare the IODataManager service
  m_ioDataManager = service( "IODataManager" ); //, false);//,false);
  if ( !m_ioDataManager ) return StatusCode::FAILURE;

  // temp
  m_filename = "PFN:/path/filename.dst";

  // get the File Records service
  m_fileRecordSvc = serviceLocator()->service<IDataProviderSvc>( "FileRecordDataSvc" );
  if ( !m_fileRecordSvc ) return StatusCode::FAILURE;

  // prepare navigator tool
  auto toolSvc = serviceLocator()->service<IToolSvc>( "ToolSvc" );
  if ( !toolSvc ) return StatusCode::FAILURE;

  // const IInterface* TestInterface =0;

  sc = toolSvc->retrieveTool( "FSRNavigator", m_navigatorTool, this ); // m_ToolName

  if ( !sc.isSuccess() || m_navigatorTool == NULL ) return StatusCode::FAILURE;

  m_defaultStatus = LHCb::IOFSR::StatusFlagToType( m_defaultStatusStr );

  debug() << "initialized successfully" << endmsg;
  return StatusCode::SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

StatusCode IOFSRSvc::finalize() {

  debug() << "finalize" << endmsg;
  debug() << "handled " << m_handled << " incidents" << endmsg;

  // if(serviceLocator())
  //   {

  //     SmartIF<IToolSvc> toolSvc(serviceLocator()->service<IToolSvc>("ToolSvc"));
  //     if (toolSvc)
  //     {
  //       if(m_navigatorTool)
  //       {
  //         toolSvc->releaseTool(m_navigatorTool).ignore();//m_ToolName
  //       }

  //       m_navigatorTool=0;
  //     }
  //   }

  debug() << "preparing to print" << endmsg;
  if ( m_printIOFSR ) print();

  m_incSvc.reset();
  m_ioDataManager.reset();

  debug() << "finalized" << endmsg;
  return Service::finalize();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
StatusCode IOFSRSvc::stop() {
  debug() << "STOPPED" << endmsg;
  return Service::stop();
}

///////////////////////////////////////////////////////////////////////////
// Incident Listening
///////////////////////////////////////////////////////////////////////////

StatusCode IOFSRSvc::prepareIncSvc() {
  m_incSvc = serviceLocator()->service( "IncidentSvc" );
  if ( !m_incSvc ) return StatusCode::FAILURE;

  m_incSvc->addListener( this, m_endIncident );
  // m_incSvc->addListener( this, m_beginIncident);

  // check extended file incidents are defined
  m_incSvc->addListener( this, IncidentType::BeginInputFile );
  m_incSvc->addListener( this, IncidentType::BeginOutputFile );
  m_incSvc->addListener( this, IncidentType::WroteToOutputFile );
  // m_incSvc->addListener( this, IncidentType::FailOutputFile);
  // m_incSvc->addListener( this, IncidentType::FailInputFile);
  // failures not important here, since they will either be read or not
  // m_incSvc->addListener( this, IncidentType::EndOutputFile);
  // IOFSR doesn't need to know if the file was fully read or not at this stage
  // m_incSvc->addListener( this, IncidentType::EndInputFile);

  debug() << "registered with incSvc" << endmsg;

  return StatusCode::SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void IOFSRSvc::handle( const Incident& incident ) {
  if ( incident.type() != m_endIncident && incident.type() != m_beginIncident )
    verbose() << incident.type() << ":" << incident.source() << endmsg;
  // elif(incident.source()=="EventLoopMgr")

  // std::string GUID="";m_GUID;
  std::string filename = m_filename;

  if ( incident.type() == IncidentType::BeginInputFile ) {
    // update current filename and GUID
    m_filename = AFN2name( incident.source() );
    m_hasinput = true;
    // prompt filling of ID maps
    file2GUID( incident.source() );

  } else if ( incident.type() == IncidentType::BeginOutputFile ) {
    filename = incident.source();
    // prompt filling of ID maps
    file2GUID( incident.source() );
  } else if ( incident.type() == m_endIncident ) {
    m_eventCount++;
    if ( m_hasinput ) {
      m_jobInputMap[m_filename]++;
      // prompt filling of ID maps
      file2GUID( m_filename );
    }

  } else if ( incident.type() == IncidentType::WroteToOutputFile ) {
    filename = incident.source();
    m_jobOutputMap[filename]++;
    // prompt filling of ID maps
    file2GUID( incident.source() );
  }

  m_handled++;

  return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

std::string IOFSRSvc::file2GUID( const std::string& filename ) {
  FidMap::iterator j = m_fidMap.find( filename );
  if ( j != m_fidMap.end() ) return j->second;
  std::string shortname = AFN2name( filename );
  j                     = m_fidMap.find( shortname );
  if ( j != m_fidMap.end() ) return j->second;

  const Gaudi::IDataConnection* con = m_ioDataManager->connection( shortname );
  if ( !con ) con = m_ioDataManager->connection( shortname );
  if ( !con ) return "";
  m_fidMap[filename]  = con->fid();
  m_fidMap[shortname] = con->fid();
  return con->fid();
  // return "";
}
std::string IOFSRSvc::AFN2name( const std::string& filename ) const {
  std::string me  = "LFN:";
  size_t      pos = filename.find( me );
  if ( ( pos == 0 ) && ( pos + me.size() < filename.size() ) ) return filename.substr( pos + me.size() );
  me  = "PFN:";
  pos = filename.find( me );
  if ( ( pos == 0 ) && ( pos + me.size() < filename.size() ) ) return filename.substr( pos + me.size() );
  return filename;
}

///////////////////////////////////////////////////////////////////////////
// Accessors for IO information
///////////////////////////////////////////////////////////////////////////

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::vector<std::string> IOFSRSvc::parents() {
  std::vector<std::string> retVec( 0 );
  for ( LHCb::IOFSR::FileEventMap::iterator af = m_jobInputMap.begin(); af != m_jobInputMap.end(); ++af ) {
    retVec.push_back( file2GUID( af->first ) );
  }
  return retVec;
}

// Return list of GUIDs as input for this job, and events read from them
LHCb::IOFSR::FileEventMap IOFSRSvc::jobInputMap() {
  LHCb::IOFSR::FileEventMap retMap;
  for ( LHCb::IOFSR::FileEventMap::const_iterator af = m_jobInputMap.begin(); af != m_jobInputMap.end(); ++af ) {
    retMap[file2GUID( af->first )] = af->second;
  }
  return retMap;
}

// Return list of GUIDs as input for this job, and events read from them
unsigned long long IOFSRSvc::jobOutput( const std::string& PFN ) {
  LHCb::IOFSR::FileEventMap::iterator j = m_jobOutputMap.find( AFN2name( PFN ) );
  if ( j != m_jobOutputMap.end() ) return j->second;
  return 0;
}

// Return map of input GUID : #events for all GUIDs, events read __from__ those files
LHCb::IOFSR::FileEventMap IOFSRSvc::eventsReadFrom() {
  // add in the daughter files read so far
  for ( LHCb::IOFSR::FileEventMap::const_iterator af = m_jobInputMap.begin(); af != m_jobInputMap.end(); ++af ) {
    m_readFromMap[file2GUID( af->first )] = af->second;
  }
  return m_readFromMap;
}

///////////////////////////////////////////////////////////////////////////
// Manipulators of existing FSRs
///////////////////////////////////////////////////////////////////////////

// merge existing IOFSRs into the maps of this service
StatusCode IOFSRSvc::mergeIOFSRs() {
  if ( m_merged ) return StatusCode::SUCCESS;

  debug() << "Merging IOFSRs" << endmsg;

  // make an inventory of the FileRecord store
  std::string              fileRecordRoot = m_FileRecordName;
  std::vector<std::string> addresses;
  if ( m_navigatorTool ) addresses = m_navigatorTool->navigate( fileRecordRoot, m_FSRName );

  std::map<std::string, LHCb::IOFSR*> ioFSRs;

  debug() << addresses.size() << " IOFSRs found" << endmsg;
  debug() << "My Maps Before Merging: prov " << m_provenanceMap.size() << ", seen " << m_seenByMap.size() << ", read "
          << m_readFromMap.size() << ", write " << m_writtenToMap.size() << endmsg;

  // print
  if ( msgLevel( MSG::DEBUG ) ) {
    for ( std::vector<std::string>::iterator iAddr = addresses.begin(); iAddr != addresses.end(); ++iAddr ) {
      debug() << "address: " << ( *iAddr ) << endmsg;
    }
  }

  // get

  for ( std::vector<std::string>::iterator a = addresses.begin(); a != addresses.end(); ++a ) {

    std::string               fileRecordAddress = ( *a );
    SmartDataPtr<LHCb::IOFSR> ioFSR( m_fileRecordSvc, fileRecordAddress );

    // if ( !exist<LHCb::IOFSR>(m_fileRecordSvc, fileRecordAddress) ) //can't find them!!
    if ( 0 != ioFSR ) {
      LHCb::IOFSR* ioFSRCast = ioFSR;

      ioFSRs[fileRecordAddress] = ioFSRCast;
    } else {
      error() << fileRecordAddress << " not found" << endmsg;
    }
  }

  // insert their provenance maps, writtenTo, seenBy, readFrom maps
  std::map<std::string, LHCb::IOFSR*>::iterator iofsr;
  for ( iofsr = ioFSRs.begin(); iofsr != ioFSRs.end(); ++iofsr ) {
    // debug() << *(*tsfsr) << endmsg;
    LHCb::IOFSR::ProvenanceMap prov  = iofsr->second->provenance();
    LHCb::IOFSR::FileEventMap  seen  = iofsr->second->eventsSeenBy();
    LHCb::IOFSR::FileEventMap  read  = iofsr->second->eventsReadFrom();
    LHCb::IOFSR::FileEventMap  write = iofsr->second->eventsWrittenTo();

    // TODO, check for duplication here...

    for ( LHCb::IOFSR::ProvenanceMap::const_iterator it = prov.begin(); it != prov.end(); ++it ) {
      m_provenanceMap[it->first] = it->second;
    }
    for ( LHCb::IOFSR::FileEventMap::const_iterator it = seen.begin(); it != seen.end(); ++it ) {
      m_seenByMap[it->first] = it->second;
    }
    for ( LHCb::IOFSR::FileEventMap::const_iterator it = read.begin(); it != read.end(); ++it ) {
      m_readFromMap[it->first] = it->second;
    }
    for ( LHCb::IOFSR::FileEventMap::const_iterator it = write.begin(); it != write.end(); ++it ) {
      m_writtenToMap[it->first] = it->second;
    }
  }

  debug() << "My Maps Before Leading Info: prov " << m_provenanceMap.size() << ", seen " << m_seenByMap.size()
          << ", read " << m_readFromMap.size() << ", write " << m_writtenToMap.size() << endmsg;

  // insert their "leading" information
  for ( iofsr = ioFSRs.begin(); iofsr != ioFSRs.end(); ++iofsr ) {
    std::string GUID = address2GUID( iofsr->first );
    debug() << " +- merging " << GUID << " from address " << iofsr->first << endmsg;

    m_seenByMap[GUID]     = iofsr->second->eventsSeen();
    m_writtenToMap[GUID]  = iofsr->second->eventsOutput();
    m_provenanceMap[GUID] = iofsr->second->parents();
  }

  debug() << "My Maps After Leading Info: prov " << m_provenanceMap.size() << ", seen " << m_seenByMap.size()
          << ", read " << m_readFromMap.size() << ", write " << m_writtenToMap.size() << endmsg;
  // check if any traces have failed
  if ( m_trace == LHCb::IOFSR::StatusFlag::UNCHECKED ) {

    for ( iofsr = ioFSRs.begin(); iofsr != ioFSRs.end(); ++iofsr ) {
      if ( iofsr->second->statusFlag() != LHCb::IOFSR::StatusFlag::UNCHECKED &&
           iofsr->second->statusFlag() != LHCb::IOFSR::StatusFlag::VERIFIED ) {
        m_trace = iofsr->second->statusFlag();
        break;
      }
    }
  }

  // clean up original FSRs
  for ( std::vector<std::string>::iterator a = addresses.begin(); a != addresses.end(); ++a ) {
    // get FSR as keyed object and cleanup the original ones - this only cleans lumiFSRs
    m_fileRecordSvc->unlinkObject( *a ).ignore(); // get it out of TES
  }

  m_merged = true;

  return StatusCode::SUCCESS;
}

// find the last GUID in the object address
std::string IOFSRSvc::address2GUID( const std::string& address ) {
  // size_t foundEnd=address.find_last_of("/");
  size_t foundEnd   = address.find_last_of( "/" );
  size_t foundStart = address.find_last_of( "/", foundEnd - 1 );
  // oh, it's sometimes terminated with IOFSR
  // if(address.substr(foundStart+1,foundEnd-1) != "IOFSR") return address.substr(foundStart+1,foundEnd-1);
  // foundEnd=foundStart;
  // foundStart=address.find_last_of("/",foundEnd);
  return address.substr( foundStart + 1, foundEnd - foundStart - 1 );
}

// Does the accounting all add up OK?
bool IOFSRSvc::traceCounts() {
  if ( traceCountsFlag() == LHCb::IOFSR::StatusFlag::VERIFIED ) return true;
  warning() << "IOFSRs don't add up, " << traceCountsFlag() << endmsg;
  return false;
}

// determine event count reliability, following down the tree
// of input/output until something doesn't match
LHCb::IOFSR::StatusFlag IOFSRSvc::traceCountsFlag() {
  if ( m_overrideStatus ) return m_defaultStatus;
  if ( !m_hasinput ) return LHCb::IOFSR::StatusFlag::VERIFIED;

  debug() << "Tracing IOFSRs" << endmsg;

  if ( !m_merged ) {
    error() << "You did not merge the FSRs yet, so the tracing cannot be done, call merge before trace" << endmsg;
    return LHCb::IOFSR::StatusFlag::ERROR;
  }

  if ( m_trace != LHCb::IOFSR::StatusFlag::UNCHECKED ) return m_trace;
  // check that all files have been fully read
  for ( LHCb::IOFSR::FileEventMap::iterator it = m_writtenToMap.begin(); it != m_writtenToMap.end(); ++it ) {
    // check if FSR for that was found ...
    LHCb::IOFSR::FileEventMap::iterator j = m_readFromMap.find( it->first );
    // perhaps the FSR never existed
    if ( j == m_readFromMap.end() ) {
      error() << "Cannot find read record to verify all events read from " << it->first << endmsg;
      m_trace = LHCb::IOFSR::StatusFlag::ERROR;
    } else if ( it->second == j->second )
      continue;
    else if ( it->second > j->second )
      m_trace = LHCb::IOFSR::StatusFlag::EVENTSKIPPED;
    else if ( it->second < j->second )
      m_trace = LHCb::IOFSR::StatusFlag::EVENTDOUBLED;
    break;
  }

  if ( m_trace != LHCb::IOFSR::StatusFlag::UNCHECKED ) return m_trace;

  debug() << "Tracing provenance, OK" << endmsg;
  // check that the seen counts are the sum of the daughter counts right now.
  // Will be done at every level, so I only need to check this top level.

  unsigned long long expected = 0;

  if ( m_writtenToMap.size() == 0 ) {
    m_trace = LHCb::IOFSR::StatusFlag::ERROR;
    error() << "No writtenToMap, implies no IOFSRs found on parent files" << endmsg;
    return m_trace;

  } else {
    std::vector<std::string> pp = parents();
    // check only for immediate parents
    for ( unsigned int it = 0; it < pp.size(); ++it ) {
      debug() << "loop" << endmsg;
      debug() << pp[it] << endmsg;
      // check if FSR for that was found ...
      LHCb::IOFSR::FileEventMap::iterator j = m_writtenToMap.find( pp[it] );
      if ( j == m_writtenToMap.end() ) {
        m_trace = LHCb::IOFSR::StatusFlag::ERROR;
        error() << "Cannot find write record to verify all events read from " << pp[it] << endmsg;
        break;
      }

      else
        expected += j->second;
    }
  }

  if ( m_trace != LHCb::IOFSR::StatusFlag::UNCHECKED ) return m_trace;
  debug() << "Traced parents, OK" << endmsg;

  if ( expected > m_eventCount )
    m_trace = LHCb::IOFSR::StatusFlag::EVENTSKIPPED;
  else if ( expected < m_eventCount )
    m_trace = LHCb::IOFSR::StatusFlag::EVENTDOUBLED;

  if ( m_trace != LHCb::IOFSR::StatusFlag::UNCHECKED ) return m_trace;
  debug() << "Event count OK" << endmsg;

  // check for input file duplication, double loop
  for ( LHCb::IOFSR::ProvenanceMap::iterator it = m_provenanceMap.begin(); it != m_provenanceMap.end(); ++it ) {
    std::vector<std::string> pp = it->second;
    if ( pp.size() == 0 ) continue;
    for ( unsigned int itp = 0; itp < pp.size(); ++itp ) {
      // check that it is not a parent!! Otherwise you've not got a proper tree :S
      std::vector<std::string>::iterator ppj = std::find( parents().begin(), parents().end(), pp[itp] );
      if ( ppj != parents().end() ) {
        warning() << "A file was processed twice." << endmsg;
        warning() << pp[itp] << " is a parent _and_ was used to create " << it->first << endmsg;
        m_trace = LHCb::IOFSR::StatusFlag::FILEDOUBLED;
      }
      LHCb::IOFSR::ProvenanceMap::iterator jt = it;
      ++jt;

      for ( ; jt != m_provenanceMap.end(); ++jt ) {
        std::vector<std::string>::iterator itpj = std::find( jt->second.begin(), jt->second.end(), pp[itp] );
        if ( itpj == jt->second.end() ) continue;
        warning() << "A file was processed twice." << endmsg;
        warning() << pp[itp] << " was used to create both [" << it->first << "," << jt->first << "]" << endmsg;
        m_trace = LHCb::IOFSR::StatusFlag::FILEDOUBLED;
      }
    }
  }
  if ( m_trace != LHCb::IOFSR::StatusFlag::UNCHECKED ) return m_trace;
  debug() << "File duplication checked" << endmsg;
  m_trace = LHCb::IOFSR::StatusFlag::VERIFIED;

  debug() << "Final result " << m_trace << endmsg;

  return m_trace;
}

// remove any IOFSR at the top level. To be called before requesting a new FSR.
StatusCode IOFSRSvc::cleanTopFSR() {
  StatusCode sc = m_fileRecordSvc->unlinkObject( LHCb::IOFSRLocation::Default );
  if ( sc.isSuccess() )
    debug() << "Removed existing FSR at " << LHCb::IOFSRLocation::Default << endmsg;
  else
    debug() << "No IOFSR to remove from " << LHCb::IOFSRLocation::Default << endmsg;
  return sc;
}

///////////////////////////////////////////////////////////////////////////
// Create the IOFSR, and pass back to the requestor
///////////////////////////////////////////////////////////////////////////

// Create a new IOFSR, pass it back so the requesting alg can store as needed
LHCb::IOFSR* IOFSRSvc::buildIOFSR( const std::string& outputName ) {
  mergeIOFSRs().ignore();

  LHCb::IOFSR* ioFSR = new LHCb::IOFSR();
  ioFSR->setEventsOutput( jobOutput( outputName ) );
  ioFSR->setEventsSeen( eventsSeen() );
  ioFSR->setParents( parents() );
  ioFSR->setProvenance( provenanceMap() );
  ioFSR->setEventsWrittenTo( eventsWrittenTo() );
  ioFSR->setEventsReadFrom( eventsReadFrom() );
  ioFSR->setEventsSeenBy( eventsSeenBy() );
  ioFSR->setStatusFlag( traceCountsFlag() );

  // print outs in debug mode, will look a little like the xmlsummary
  debug() << "Created IOFSR for " << outputName << ": " << endmsg;
  debug() << " +-+ eventsSeen " << ioFSR->eventsSeen() << endmsg;
  debug() << " +-+ eventsOutput " << ioFSR->eventsOutput() << endmsg;
  debug() << " +-+ statusFlag " << ioFSR->statusFlag() << endmsg;
  debug() << " +-+ number of ancestors " << ioFSR->provenance().size() << endmsg;
  debug() << " +-+ number of parents " << ioFSR->parents().size() << endmsg;
  for ( unsigned int i = 0; i < ioFSR->parents().size(); ++i ) {
    debug() << "   +-" << i << " -> " << ioFSR->parents()[i] << " : "
            << ioFSR->eventsSeenBy().find( ioFSR->parents()[i] )->second << endmsg;
  }

  return ioFSR;
}

///////////////////////////////////////////////////////////////////////////
// Create the IOFSR, and register on the TES
///////////////////////////////////////////////////////////////////////////

// Create a new IOFSR, and register on the TES
StatusCode IOFSRSvc::storeIOFSR( const std::string& outputName ) {
  LHCb::IOFSR* iofsr = buildIOFSR( outputName );
  return m_fileRecordSvc->registerObject( LHCb::IOFSRLocation::Default, iofsr );
}

///////////////////////////////////////////////////////////////////////////
// Print my contained information
///////////////////////////////////////////////////////////////////////////
void IOFSRSvc::print() {
  debug() << "preparing to print" << endmsg;
  info() << "+- Merged? " << m_merged << endmsg;
  info() << "+- Input Read? " << m_hasinput << endmsg;
  info() << "+- Overide Status? " << m_overrideStatus << endmsg;
  info() << "+- Default Status " << m_defaultStatus << endmsg;
  info() << "+-+ Job Input Map (" << m_jobInputMap.size() << ")" << endmsg;
  dumpMap( m_jobInputMap );
  info() << "+-+ Job Output Map (" << m_jobOutputMap.size() << ")" << endmsg;
  dumpMap( m_jobOutputMap );
  info() << "+-+ Written To Map (" << m_writtenToMap.size() << ")" << endmsg;
  dumpMap( m_writtenToMap );
  info() << "+-+ Read From Map (" << m_readFromMap.size() << ")" << endmsg;
  dumpMap( m_readFromMap );
  info() << "+-+ Seen By Map (" << m_seenByMap.size() << ")" << endmsg;
  dumpMap( m_seenByMap );
  info() << "+-+ Provenance (" << parents().size() << ")" << endmsg;
  for ( unsigned int i = 0; i < parents().size(); ++i ) {
    std::string buffer = "| +-";
    info() << buffer << "+ " << parents()[i] << endmsg;
    buffer = "| | ";
    dumpProvenance( parents()[i], buffer );
  }
  info() << "+- Done!! " << endmsg;
}

void IOFSRSvc::dumpMap( LHCb::IOFSR::FileEventMap& map ) {
  for ( const auto& i : map ) { info() << "| +-+ " << i.first << " : " << i.second << endmsg; }
}

void IOFSRSvc::dumpProvenance( const std::string file, const std::string buffer, const unsigned int rec ) {
  // prevent infinite recursion
  if ( rec > 100 ) {
    info() << "Max recursion depth reached" << endmsg;
    return;
  }
  auto it = m_provenanceMap.find( file );
  if ( it == m_provenanceMap.end() ) return;

  for ( unsigned int itj = 0; itj < it->second.size(); ++itj ) {
    info() << buffer << "+-+ " << it->second[itj] << endmsg;
    dumpProvenance( it->second[itj], buffer + "| ", rec + 1 );
  }
}
