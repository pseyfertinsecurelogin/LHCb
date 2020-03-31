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
  IDataProviderSvc* m_dataSvc = nullptr;
  /// Name of the data provider service
  Gaudi::Property<std::string> m_dataSvcName{this, "DataSvc", "EventDataSvc"};
  /// Name of the root leaf (obtained at initialize)
  std::string m_rootName;

public:
  /// Standard algorithm constructor
  AddressKillerAlg( const std::string& name, ISvcLocator* pSvcLocator ) : Algorithm( name, pSvcLocator ) {}

  /// Initialize
  StatusCode initialize() override {
    m_rootName.clear();
    return Algorithm::initialize()
        .andThen( [&] {
          return service( m_dataSvcName, m_dataSvc, true ).orElse( [&] {
            error() << "Failed to access service " << std::quoted( m_dataSvcName.value() ) << '.' << endmsg;
          } );
        } )
        .andThen( [&]() -> StatusCode {
          SmartIF<IDataManagerSvc> mgr( m_dataSvc );
          if ( !mgr ) {
            error() << "Failed to retrieve IDataManagerSvc interface." << endmsg;
            return StatusCode::FAILURE;
          }
          m_rootName = mgr->rootName();
          return StatusCode::SUCCESS;
        } );
  }

  /// Finalize
  StatusCode finalize() override {
    if ( m_dataSvc ) m_dataSvc->release();
    m_dataSvc = nullptr;
    return Algorithm::finalize();
  }

  /// Execute procedure
  StatusCode execute() override {
    SmartDataPtr<DataObject> root( m_dataSvc, m_rootName );
    if ( !root ) return StatusCode::FAILURE;
    explore( root->registry() );
    return StatusCode::SUCCESS;
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
