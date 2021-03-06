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
//  ====================================================================
//  RawDataCnvSvc.cpp
//  --------------------------------------------------------------------
//
//  Author    : Markus Frank
//
//  ====================================================================
#include "MDF/RawDataCnvSvc.h"
#include "MDF/MDFHeader.h"
#include "MDF/RawDataAddress.h"
#include "MDF/RawDataConnection.h"

#include "Event/RawEvent.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IPersistencySvc.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/strcasecmp.h"
#include "GaudiUtils/IIODataManager.h"

#include <map>
#include <memory>
#include <vector>

using namespace std;
using namespace LHCb;
using namespace Gaudi;
namespace LHCb {
  /// Unpacks the buffer given by the start and end pointers, and fill the rawevent structure
  StatusCode unpackTAE( const char* start, const char* end, const std::string& loc, RawEvent* raw );
  /// Return vector of TAE event names
  std::vector<std::string> buffersTAE( const char* start );
} // namespace LHCb

namespace {
  struct MDFMapEntry final {
    string                           name;
    std::unique_ptr<IDataConnection> connection;
    StreamDescriptor                 desc;
  };
  static bool               s_recursiveFlag = true;
  struct RecursiveDetection final {
    RecursiveDetection() { s_recursiveFlag = !s_recursiveFlag; }
    ~RecursiveDetection() { s_recursiveFlag = !s_recursiveFlag; }
    bool isRecursive() const { return s_recursiveFlag; }
  };

  // Temporary I/O context for buffered I/O
  typedef std::pair<char*, int>      MDFDescriptor;
  typedef pair<void*, MDFDescriptor> io_context_t;
} // namespace

static string RAWDATA_INPUT = "RAW";

// Initializing constructor
RawDataCnvSvc::RawDataCnvSvc( CSTR nam, ISvcLocator* loc )
    : ConversionSvc( nam, loc, RAWDATA_StorageType ), MDFIO( MDFIO::MDF_RECORDS, nam ), m_wrFlag( false ) {
  // m_data.reserve(48*1024);
  declareProperty( "Compress", m_compress = 2 );                                 // File compression
  declareProperty( "ChecksumType", m_genChecksum = 1 );                          // Generate checksum
  declareProperty( "EventsBefore", m_evtsBefore = 0 );                           // Events before T0
  declareProperty( "EventsAfter", m_evtsAfter = 0 );                             // Events after T0
  declareProperty( "DataType", m_dataType = MDFIO::MDF_RECORDS );                // Input data type
  declareProperty( "BankLocation", m_bankLocation = RawEventLocation::Default ); // Location of RAW banks in the TES
  declareProperty( "DstLocation", m_dstLocation = "DAQ/DstEvent" );              // Location of DST banks in the TES
  declareProperty( "DataManager", m_ioMgrName = "Gaudi::IODataManager/IODataManager" );
  declareProperty( "SourceType", m_sourceType = RAWDATA_INPUT );
}

/// Service initialization
StatusCode RawDataCnvSvc::initialize() {
  StatusCode sc = ConversionSvc::initialize();
  MsgStream  log( msgSvc(), name() );
  if ( !sc.isSuccess() ) {
    log << MSG::ERROR << "Unable to initialize base class ConversionSvc." << endmsg;
    return sc;
  }

  // Add this conversion service to the EventPersistencySvc
  // Note: I assume that the lack of a release is on purpose, to keep
  //       this EventPersistencySvc instance alive, and avoid it going
  //       out of scope, and then being resurrected without 'us' being
  //       added... (as createIf is set to true here!)
  IPersistencySvc* pSvc = nullptr;
  sc                    = service( "EventPersistencySvc", pSvc, true );
  if ( !sc.isSuccess() ) {
    log << MSG::ERROR << "Unable to localize EventPersistencySvc." << endmsg;
    return sc;
  }

  sc = pSvc->addCnvService( this );
  if ( !sc.isSuccess() ) {
    log << MSG::ERROR << "Unable to add conversion service" << endmsg;
    return sc;
  }

  // get the IDataManagerSvc interface from the EventPersistencySvc
  m_dataMgr = dataProvider();
  if ( !m_dataMgr ) {
    log << MSG::ERROR << "Conversion service " << name() << "not registered to EventPersistencySvc." << endmsg;
    return StatusCode::FAILURE;
  }

  // Retrieve conversion service handling event iteration
  m_ioMgr = Service::service( m_ioMgrName );
  if ( !m_ioMgr ) {
    log << MSG::ERROR << "Unable to localize interface IID_IIODataManager from service:" << m_ioMgrName << endmsg;
    return StatusCode::FAILURE;
  }
  /// All OK
  return sc;
}

/// Service finalization
StatusCode RawDataCnvSvc::finalize() {
  long typ = repSvcType();
  for ( auto& i : m_fileMap ) {
    if ( typ == RAWDATA_StorageType && i.second ) { closeIO( i.second ).ignore(); }
  }
  m_fileMap.clear();
  m_dataMgr.reset();
  m_ioMgr.reset();
  return ConversionSvc::finalize();
}

/// Helper to print errors and return bad status
StatusCode RawDataCnvSvc::error( CSTR msg ) const {
  MsgStream err( msgSvc(), name() );
  err << MSG::ERROR << msg << endmsg;
  return StatusCode::FAILURE;
}

/// Concrete class type
const CLID& RawDataCnvSvc::objType() const { return DataObject::classID(); }

/// Connect the output file to the service with open mode.
StatusCode RawDataCnvSvc::connectOutput( CSTR outputFile, CSTR openMode ) {
  m_wrFlag  = false;
  m_current = m_fileMap.find( outputFile );
  if ( m_current == m_fileMap.end() ) {
    void* entry = openIO( outputFile, openMode );
    closeDisconnected();
    if ( entry ) {
      m_fileMap.emplace( outputFile, entry );
      m_current = m_fileMap.find( outputFile );
      return StatusCode::SUCCESS;
    }
    return error( "connectOutput> FAILED to bind I/O channel:" + outputFile );
  }
  return StatusCode::SUCCESS;
}

/// Connect the input file to the service with READ mode
StatusCode RawDataCnvSvc::connectInput( CSTR fname, void*& iodesc ) {
  FileMap::const_iterator it = m_fileMap.find( fname );
  if ( it == m_fileMap.end() ) {
    iodesc = openIO( fname, "READ" );
    closeDisconnected();
    if ( iodesc ) {
      m_fileMap.emplace( fname, iodesc );
      return StatusCode::SUCCESS;
    }
    return error( "connectInput> FAILED to bind I/O channel:" + fname );
  }
  iodesc = ( *it ).second;
  return StatusCode::SUCCESS;
}

/// Object creation callback
StatusCode RawDataCnvSvc::createObj( IOpaqueAddress* pA, DataObject*& refpObj ) {
  if ( pA ) {
    bool default_reader = m_bankLocation == RawEventLocation::Default;
    // default_reader =
    if ( default_reader ) {
      if ( pA->clID() == CLID_DataObject ) {
        refpObj = new DataObject();
        return StatusCode::SUCCESS;
      } else if ( pA->clID() == RawEvent::classID() ) {
        try {
          IRegistry*      pReg    = pA->registry();
          RawDataAddress* pAddRaw = dynamic_cast<RawDataAddress*>( pA );
          refpObj                 = nullptr;
          // if ( rand() < int(RAND_MAX*0.05) )
          //  return error("TEST: Triggered access failure for "+pReg->identifier());
          if ( pReg && pAddRaw ) {
            auto          raw = std::make_unique<RawEvent>();
            MDFDescriptor dat = accessRawData( pAddRaw );
            if ( dat.second > 0 ) {
              StatusCode sc = unpackTAE( dat.first, dat.first + dat.second, pReg->identifier(), raw.get() );
              if ( sc.isSuccess() ) {
                refpObj = raw.release();
                return sc;
              }
            }
            return error( "Failed to decode raw data from input from:" + pA->par()[0] );
          }
          return error( "No valid object address present:" + ( pReg ? pReg->identifier() : pA->par()[0] ) );
        } catch ( exception& e ) { return error( string( "Exception:" ) + e.what() ); } catch ( ... ) {
          return error( "Exception: (Unknown)" );
        }
      }
      return error( "No valid event object address present [INVALID_CLID]." );
    } else {
      RawDataAddress* rawAdd = dynamic_cast<RawDataAddress*>( pA );
      if ( rawAdd ) {
        rawAdd->setSvcType( ROOT_StorageType );
        return dataProvider()->retrieveObject( "/Event", refpObj );
      }
      return error( "No valid object address present for object \"/Event\"" );
    }
  }
  return StatusCode::FAILURE;
}

StatusCode RawDataCnvSvc::regAddr( IRegistry* pReg, RawDataAddress* pA, CSTR path, const CLID& clid ) {
  auto paddr = std::make_unique<RawDataAddress>( *pA );
  paddr->setClID( clid );
  const StatusCode sc = ( pReg ? m_dataMgr->registerAddress( pReg, path, paddr.get() )
                               : m_dataMgr->registerAddress( path, paddr.get() ) );
  if ( sc.isSuccess() ) {
    paddr.release();
    return sc;
  }
  paddr->release();
  return error( "Failed to register address for object " + path );
}

/// Access the raw data from MDF file
MDFDescriptor RawDataCnvSvc::accessRawData( RawDataAddress* pAddRaw ) {
  MDFDescriptor dat = pAddRaw->data();
  if ( dat.second == 0 ) {
    // Need to open MDF file to get access!
    if ( readRawBanks( pAddRaw ).isSuccess() ) { return pAddRaw->data(); }
  }
  return dat;
}

/// Decode a TAE event record from MDF banks
StatusCode RawDataCnvSvc::registerRawAddresses( IRegistry* pReg, RawDataAddress* pAddRaw,
                                                const vector<string>& names ) {
  if ( !names.empty() ) {
    StatusCode sc, iret = StatusCode::SUCCESS;
    for ( const auto& s : names ) {
      auto path = s.length() > 7 ? s.substr( 7 ) : string( "/DAQ" );
      sc        = regAddr( pReg, pAddRaw, path, CLID_DataObject );
      if ( !sc.isSuccess() ) iret = sc;
    }
    return iret.isSuccess() ? iret : error( "Error registering RawEvent leaves in TES." );
  }
  return regAddr( pReg, pAddRaw, "/DAQ", DataObject::classID() );
}

/// Callback for reference processing (misused to attach leaves)
StatusCode RawDataCnvSvc::fillObjRefs( IOpaqueAddress* pA, DataObject* pObj ) {
  if ( pA && pObj ) {
    try {
      auto pReg    = pA->registry();
      auto pAddRaw = dynamic_cast<RawDataAddress*>( pA );
      if ( pReg && pAddRaw ) {
        auto id = pReg->identifier().substr( 6 );
        if ( id.empty() ) {
          MDFDescriptor dat = pAddRaw->data();
          if ( dat.second > 0 ) { return registerRawAddresses( pReg, pAddRaw, buffersTAE( dat.first ) ); }
          return error( "Failed to access raw data input:" + pA->par()[0] );
        } else if ( id.substr( id.length() - 4 ) == "/DAQ" ) {
          MDFDescriptor dat   = pAddRaw->data();
          char *        start = dat.first, *end = start + dat.second;
          while ( start < end ) {
            auto b = (RawBank*)start;
            if ( b->type() == RawBank::DstAddress ) {
              // cout << "Reg:" << pReg->identifier() << "  " << m_dstLocation << endl;
              StatusCode  sc     = StatusCode::FAILURE;
              const char* objLoc = m_dstLocation.c_str();
              const char* regLoc = pReg->identifier().c_str();
              size_t      regLen = pReg->identifier().length();
              if ( objLoc[0] == '/' && strncmp( objLoc, regLoc, regLen ) == 0 )
                sc = regAddr( pReg, pAddRaw, objLoc + regLen + 1, RawEvent::classID() );
              else if ( objLoc[0] != '/' && strncmp( objLoc, "DAQ/", 4 ) == 0 )
                sc = regAddr( pReg, pAddRaw, objLoc + 4, RawEvent::classID() );
              else
                sc = regAddr( pReg, pAddRaw, m_dstLocation, RawEvent::classID() );
              if ( sc.isSuccess() ) {
                auto              ptr  = b->begin<unsigned int>();
                long unsigned int clid = *ptr++, ip[2] = {*ptr++, *ptr++}, svc_typ = *ptr++;
                size_t            len     = strlen( (char*)ptr ) + 1;
                string            p[2]    = {std::string( (char*)ptr ), std::string( ( (char*)ptr ) + len )};
                string            raw_loc = ( (char*)ptr ) + len + 1 + p[1].length();
                IOpaqueAddress*   addr    = nullptr;
                // cout << "P0:" << p[0] << " P1:" << p[1] << " IP0:" << ip[0] << " IP1:" << ip[1] << " " << svc_typ <<
                // endl;
                sc = m_addressCreator->createAddress( svc_typ, clid, p, ip, addr );
                if ( sc.isSuccess() ) {
                  sc = m_dataMgr->registerAddress( raw_loc, addr );
                  if ( sc.isSuccess() ) { return sc; }
                  return error( "Failed to register address to raw data " + p[1] + " in " + raw_loc );
                }
                return error( "Failed to create address to raw data " + p[1] + " in " + raw_loc );
              }
              return error( "Failed to register address to DstEvent" );
            }
            start += b->totalSize();
          }
          return regAddr( pReg, pAddRaw, "/RawEvent", RawEvent::classID() );
        } else if ( id.length() == 6 && ( id[1] == 'N' || id[1] == 'P' ) ) {
          return regAddr( pReg, pAddRaw, "/DAQ", DataObject::classID() );
        }
        // Nothing to do otherwise. Return success.
        return StatusCode::SUCCESS;
      }
    } catch ( exception& e ) { return error( string( "Exception:" ) + e.what() ); } catch ( ... ) {
      return error( "Exception: (Unknown)" );
    }
  }
  return error( "No valid event object/address present." );
}

/// Commit pending output.
StatusCode RawDataCnvSvc::commitOutput( CSTR, bool doCommit ) {
  if ( doCommit && m_wrFlag ) {
    if ( m_current != m_fileMap.end() ) {
      long typ = repSvcType();
      setupMDFIO( msgSvc(), dataProvider() );
      if ( typ == RAWDATA_StorageType ) {
        io_context_t ctx( ( *m_current ).second, MDFDescriptor( 0, 0 ) );
        const auto   sc = commitRawBanks( m_compress, m_genChecksum, &ctx, m_bankLocation );
        if ( ctx.second.first ) ::free( ctx.second.first );
        m_current = m_fileMap.end();
        return sc;
      }
      return error( "commitOutput> Unknown channel type." );
    }
    return error( "commitOutput> No valid output channel known." );
  }
  return StatusCode::SUCCESS;
}

/// Convert the transient object to the requested representation.
StatusCode RawDataCnvSvc::createRep( DataObject* pObj, IOpaqueAddress*& refpAddr ) {
  if ( pObj ) {
    if ( m_current != m_fileMap.end() ) {
      auto          reg    = pObj->registry();
      string        spar[] = {( *m_current ).first, reg->identifier()};
      unsigned long ipar[] = {0, 0};
      return createAddress( repSvcType(), pObj->clID(), spar, ipar, refpAddr );
    }
    return error( "createRep> Cannot write object: No output file is connected!" );
  }
  return error( "createRep> Cannot write object: Object pointer is NULL!" );
}

/// Resolve the references of the converted object.
StatusCode RawDataCnvSvc::fillRepRefs( IOpaqueAddress* /* pAddr */, DataObject* /* pObj */ ) {
  if ( m_current != m_fileMap.end() ) {
    m_wrFlag = true;
    return StatusCode::SUCCESS;
  }
  return error( "fillRepRefs> Cannot write object: No output file is connected!" );
}

/// Create a Generic address using explicit arguments to identify a single object.
StatusCode RawDataCnvSvc::createAddress( long typ, const CLID& clid, const string* par, const unsigned long* ip,
                                         IOpaqueAddress*& refpAddress ) {
  refpAddress = new RawDataAddress( typ, clid, par[0], par[1], ip[0], ip[1] );
  return StatusCode::SUCCESS;
}

/// Close all files disconnected from the IO manager
void RawDataCnvSvc::closeDisconnected() {
  for ( auto i = m_fileMap.begin(); i != m_fileMap.end(); ) {
    auto e = (MDFMapEntry*)( *i ).second;
    if ( e && e->connection && !e->connection->isConnected() ) {
      closeIO( e ).ignore();
      m_fileMap.erase( i );
      i = m_fileMap.begin();
      continue;
    }
    ++i;
  }
}

/// Open MDF file
void* RawDataCnvSvc::openIO( CSTR fname, CSTR mode ) const {
  MsgStream log( msgSvc(), name() );
  auto      ent = std::make_unique<MDFMapEntry>();
  ent->name     = fname;
  if ( strncasecmp( mode.c_str(), "N", 1 ) == 0 || strncasecmp( mode.c_str(), "REC", 3 ) == 0 ) {
    ent->connection = std::make_unique<RawDataConnection>( this, fname );
    if ( m_ioMgr->connectWrite( ent->connection.get(), IDataConnection::RECREATE, "MDF" ).isSuccess() ) {
      log << MSG::INFO << "Opened(NEW)  MDF stream:" << ent->name << " ID:" << (void*)ent.get() << endmsg;
      return ent.release();
    }
  } else if ( strncasecmp( mode.c_str(), "O", 1 ) == 0 || strncasecmp( mode.c_str(), "REA", 3 ) == 0 ) {
    ent->connection = std::make_unique<RawDataConnection>( this, fname );
    if ( m_ioMgr->connectRead( false, ent->connection.get() ).isSuccess() ) {
      log << MSG::INFO << "Opened(READ) MDF stream:" << ent->name << " ID:" << (void*)ent.get() << endmsg;
      return ent.release();
    }
  }
  error( "Unknown openmode " + mode + " for MDF file :" + fname );
  return nullptr;
}

/// Close MDF file
StatusCode RawDataCnvSvc::closeIO( void* ioDesc ) const {
  MDFMapEntry* ent = (MDFMapEntry*)ioDesc;
  if ( ent ) {
    MsgStream log( msgSvc(), name() );
    bool      connected = ent->connection->isConnected();
    m_ioMgr->disconnect( ent->connection.get() ).ignore();
    log << MSG::INFO << ( connected ? "Closed " : "Removed dis" ) << "connected MDF stream:" << ent->name
        << " ID:" << (void*)ent << endmsg;
    delete ent;
  }
  return StatusCode::SUCCESS;
}

/// Read raw banks
StatusCode RawDataCnvSvc::readRawBanks( RawDataAddress* pAddr ) {
  io_context_t  ctx( 0, MDFDescriptor( 0, 0 ) );
  const string* par = pAddr->par();
  StatusCode    sc  = connectInput( par[0], ctx.first );
  if ( sc.isSuccess() ) {
    long long offset = pAddr->fileOffset();
    auto      ent    = (MDFMapEntry*)ctx.first;
    if ( ent->connection ) {
      if ( m_ioMgr->seek( ent->connection.get(), offset, SEEK_SET ) != -1 ) {
        setupMDFIO( msgSvc(), dataProvider() );
        MDFDescriptor result = readBanks( &ctx );
        if ( result.first ) {
          pAddr->adoptData( ctx.second );
          return StatusCode::SUCCESS;
        }
        if ( ctx.second.first ) ::free( ctx.second.first );
        return error( "Failed read raw data input from:" + par[0] );
      }
      return error( "Cannot seek data record: [Invalid I/O operation]" );
    }
    return error( "Cannot read data record: [Invalid I/O descriptor]" );
  }
  return error( "Cannot read data record: [Failed to open file: " + par[0] + "]" );
}

/// MDFIO interface: Allocate data space for output
MDFDescriptor RawDataCnvSvc::getDataSpace( void* const ioDesc, size_t len ) {
  auto ctx = (io_context_t*)ioDesc;
  if ( ctx->second.second < int( len ) ) {
    ctx->second.first  = (char*)::realloc( ctx->second.first, len );
    ctx->second.second = len;
  }
  return ctx->second;
}

/// MDFIO interface: Read raw byte buffer from input stream
StatusCode RawDataCnvSvc::readBuffer( void* const ioDesc, void* const data, size_t len ) {
  auto ctx = (io_context_t*)ioDesc;
  if ( ctx && ctx->first ) {
    auto ent = (MDFMapEntry*)ctx->first;
    if ( ent->connection ) { return m_ioMgr->read( ent->connection.get(), data, len ); }
  }
  return StatusCode::FAILURE;
}

/// MDFIO interface: Write data block to stream
StatusCode RawDataCnvSvc::writeBuffer( void* ioDesc, const void* data, size_t len ) {
  auto ctx = (io_context_t*)ioDesc;
  if ( ctx && ctx->first ) {
    auto ent = (MDFMapEntry*)ctx->first;
    if ( ent->connection ) {
      if ( m_ioMgr->write( ent->connection.get(), data, len ).isSuccess() ) { return StatusCode::SUCCESS; }
      return error( "Cannot write data record: [Invalid I/O operation]" );
    }
  }
  return error( "Cannot write data record: [Invalid I/O descriptor]" );
}
