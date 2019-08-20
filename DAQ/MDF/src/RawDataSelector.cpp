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
//====================================================================
//  OnlineMDFEvtSelector.cpp
//--------------------------------------------------------------------
//
//  Package    : MDF
//
//  Description: The LHCb::RawDataSelector component is able
//               to produce a list of event references given
//               a set of "selection criteria".
//
//  Author     : M.Frank
//====================================================================

// Include files
#include "MDF/RawDataSelector.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiUtils/IIODataManager.h"
#include "MDF/RawDataAddress.h"
#include "MDF/RawDataConnection.h"
#include "MDF/RawEventHelpers.h"

static const auto S_OK    = StatusCode::SUCCESS;
static const auto S_ERROR = StatusCode::FAILURE;

using namespace LHCb;

/// Standard constructor
RawDataSelector::LoopContext::LoopContext( const RawDataSelector* pSelector )
    : m_sel( pSelector ), m_ioMgr( m_sel->fileMgr() ) {}

/// Set connection
StatusCode RawDataSelector::LoopContext::connect( const std::string& specs ) {
  close();
  m_connection  = std::make_unique<RawDataConnection>( m_sel, specs );
  StatusCode sc = m_ioMgr->connectRead( true, m_connection.get() );
  if ( sc.isSuccess() ) {
    m_conSpec  = m_connection->fid();
    m_vetoMask = ( m_sel->vetoMask().empty() ) ? 0 : &( m_sel->vetoMask() );
    m_trgMask  = ( m_sel->triggerMask().empty() ) ? 0 : &( m_sel->triggerMask() );
  } else {
    close();
  }
  return sc;
}

/// close connection
void RawDataSelector::LoopContext::close() {
  if ( m_connection ) {
    m_ioMgr->disconnect( m_connection.get() ).ignore();
    m_connection.reset();
  }
}

RawDataSelector::RawDataSelector( const std::string& nam, ISvcLocator* svcloc ) : base_class( nam, svcloc ) {
  declareProperty( "DataManager", m_ioMgrName = "Gaudi::IODataManager/IODataManager" );
  declareProperty( "TriggerMask", m_trgMask );
  declareProperty( "VetoMask", m_vetoMask );
  declareProperty( "NSkip", m_skipEvents = 0 );
  declareProperty( "PrintFreq", m_printFreq = -1 );
  declareProperty( "AddSpace", m_addSpace = 1 );
  declareProperty( "Input", m_input = "" );
}

/// IService implementation: Db event selector override
StatusCode RawDataSelector::initialize() {
  // Initialize base class
  StatusCode status = Service::initialize();
  MsgStream  log( msgSvc(), name() );
  if ( !status.isSuccess() ) {
    log << MSG::ERROR << "Error initializing base class Service!" << endmsg;
    return status;
  }
  // Retrieve conversion service handling event iteration
  status = Service::service( m_ioMgrName, m_ioMgr );
  if ( !status.isSuccess() ) {
    log << MSG::ERROR << "Unable to localize interface IID_IIODataManager from service:" << m_ioMgrName << endmsg;
    return status;
  }
  // Get DataSvc
  auto eds = serviceLocator()->service<IDataManagerSvc>( "EventDataSvc" );
  if ( !eds ) {
    log << MSG::ERROR << "Cannot get IID_IDataManagerSvc from EventDataSvc" << endmsg;
    return StatusCode::FAILURE;
  }
  m_rootCLID = eds->rootCLID();
  if ( UNLIKELY( log.level() <= MSG::DEBUG ) ) log << MSG::DEBUG << "Selection CLID:" << m_rootCLID << endmsg;
  if ( !m_trgMask.empty() ) {
    log << MSG::INFO << "Trigger mask: " << std::hex;
    for ( size_t i = 0; i < m_trgMask.size(); ++i )
      log << "0x" << std::setw( 8 ) << std::setfill( '0' ) << m_trgMask[i] << " ";
    log << endmsg;
  }
  if ( !m_vetoMask.empty() ) {
    log << MSG::INFO << "Trigger mask: " << std::hex;
    for ( size_t i = 0; i < m_vetoMask.size(); ++i )
      log << "0x" << std::setw( 8 ) << std::setfill( '0' ) << m_vetoMask[i] << " ";
    log << endmsg;
  }
  m_evtCount = 0;
  return status;
}

/// IService implementation: finalize the service
StatusCode RawDataSelector::finalize() {
  if ( m_ioMgr ) {
    m_ioMgr->release();
    m_ioMgr = nullptr;
  }
  m_evtCount = 0;
  return Service::finalize();
}

StatusCode RawDataSelector::next( Context& ctxt ) const {
  auto pCtxt = dynamic_cast<LoopContext*>( &ctxt );
  if ( pCtxt ) {
    ++m_evtCount;
    if ( m_printFreq > 0 && ( m_evtCount % m_printFreq ) == 0 ) {
      MsgStream log( msgSvc(), name() );
      log << MSG::ALWAYS << "Reading Event record " << ( m_evtCount - m_skipEvents ) << ". Record number within stream "
          << m_evtCount << endmsg;
    }
    return pCtxt->receiveData( msgSvc() );
  }
  return S_ERROR;
}

StatusCode RawDataSelector::next( Context& ctxt, int jump ) const {
  if ( jump > 0 ) {
    for ( int i = 0; i < jump; ++i ) {
      auto status = next( ctxt );
      if ( !status.isSuccess() ) { return status; }
    }
    return S_OK;
  }
  return S_ERROR;
}

StatusCode RawDataSelector::previous( Context& /* ctxt */ ) const {
  MsgStream log( msgSvc(), name() );
  log << MSG::FATAL << " EventSelector Iterator, operator -- not supported " << endmsg;
  return S_ERROR;
}

StatusCode RawDataSelector::previous( Context& ctxt, int jump ) const {
  if ( jump > 0 ) {
    for ( int i = 0; i < jump; ++i ) {
      auto status = previous( ctxt );
      if ( !status.isSuccess() ) { return status; }
    }
    return S_OK;
  }
  return S_ERROR;
}

StatusCode RawDataSelector::releaseContext( Context*& ctxt ) const {
  auto pCtxt = dynamic_cast<LoopContext*>( ctxt );
  if ( pCtxt ) {
    pCtxt->close();
    delete pCtxt;
    ctxt = nullptr;
    return S_OK;
  }
  return S_ERROR;
}

StatusCode RawDataSelector::createAddress( const Context& ctxt, IOpaqueAddress*& pAddr ) const {
  auto pctxt = dynamic_cast<const LoopContext*>( &ctxt );
  if ( pctxt ) {
    if ( pctxt->data().second > 0 ) {
      auto pA = std::make_unique<RawDataAddress>( RAWDATA_StorageType, m_rootCLID, pctxt->specs(), "0", 0, 0 );
      pA->setFileOffset( pctxt->offset() );
      pA->adoptData( pctxt->releaseData() );
      pAddr = pA.release();
      return S_OK;
    }
  }
  pAddr = nullptr;
  return S_ERROR;
}

StatusCode RawDataSelector::resetCriteria( const std::string& criteria, Context& context ) const {
  MsgStream log( msgSvc(), name() );
  auto      ctxt = dynamic_cast<LoopContext*>( &context );
  if ( ctxt ) {
    auto crit = criteria.substr( 5 );
    ctxt->close();
    auto sc = ctxt->connect( crit );
    if ( !sc.isSuccess() ) {
      log << MSG::ERROR << "Failed to connect to:" << crit << endmsg;
    } else if ( m_skipEvents > 0 ) {
      sc = ctxt->skipEvents( msgSvc(), m_skipEvents );
    }
    return sc;
  }
  log << MSG::ERROR << "Invalid iteration context." << endmsg;
  return S_ERROR;
}
