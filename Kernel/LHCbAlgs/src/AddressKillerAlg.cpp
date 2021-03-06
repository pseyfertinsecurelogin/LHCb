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
// ====================================================================
//  AddressKillerAlg.cpp
// --------------------------------------------------------------------
//
// Author    : Markus Frank
//
// ====================================================================
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IOpaqueAddress.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/SmartIF.h"

/**@class AddressKillerAlg
 *
 * Small algorithm to make the entire datastore anonymous.
 * All persistent info of all addresses is entirely removed.
 *
 * @author:  M.Frank
 * @version: 1.0
 */
class AddressKillerAlg : public Algorithm {

private:
  /// Reference to data provider service
  IDataProviderSvc* m_dataSvc;
  /// Name of the data provider service
  std::string m_dataSvcName;
  /// Name of the root leaf (obtained at initialize)
  std::string m_rootName;

public:
  /// Standard algorithm constructor
  AddressKillerAlg( const std::string& name, ISvcLocator* pSvcLocator )
      : Algorithm( name, pSvcLocator ), m_dataSvc( 0 ) {
    declareProperty( "DataSvc", m_dataSvcName = "EventDataSvc" );
  }

  /// Standard Destructor
  virtual ~AddressKillerAlg() {}

  /// Initialize
  StatusCode initialize() override {
    MsgStream log( msgSvc(), name() );
    m_rootName    = "";
    StatusCode sc = service( m_dataSvcName, m_dataSvc, true );
    if ( sc.isSuccess() ) {
      SmartIF<IDataManagerSvc> mgr( m_dataSvc );
      if ( mgr ) {
        m_rootName = mgr->rootName();
        return sc;
      }
      log << MSG::ERROR << "Failed to retrieve IDataManagerSvc interface." << endmsg;
      return StatusCode::FAILURE;
    }
    log << MSG::ERROR << "Failed to access service \"" << m_dataSvcName << "\"." << endmsg;
    return StatusCode::FAILURE;
  }

  /// Finalize
  StatusCode finalize() override {
    if ( m_dataSvc ) m_dataSvc->release();
    m_dataSvc = 0;
    return StatusCode::SUCCESS;
  }

  /// Execute procedure
  StatusCode execute() override {
    SmartDataPtr<DataObject> root( m_dataSvc, m_rootName );
    if ( root ) {
      explore( root->registry() );
      return StatusCode::SUCCESS;
    }
    return StatusCode::FAILURE;
  }

private:
  void explore( IRegistry* pObj, const unsigned int depth = 0 ) {
    if ( depth > 9999999 ) return; // infinite recursion protection..

    SmartIF<IDataManagerSvc> mgr( m_dataSvc );
    if ( mgr && pObj ) {
      std::vector<IRegistry*> leaves;
      const IOpaqueAddress*   addr = pObj->address();
      if ( addr ) {
        // NOT for MDF top level address!!!!
        if ( !( addr->svcType() == RAWDATA_StorageType && pObj->identifier() == m_rootName ) ) {
          MsgStream log( msgSvc(), name() );
          log << MSG::DEBUG << "Remove store address \"" << pObj->identifier() << "\"." << endmsg;
          pObj->setAddress( 0 );
        }
      }
      const StatusCode sc = mgr->objectLeaves( pObj, leaves );
      if ( sc.isSuccess() ) {
        for ( IRegistry* leaf : leaves ) { explore( leaf, depth + 1 ); }
      }
    }
  }
};

DECLARE_COMPONENT( AddressKillerAlg )
