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
//------------------------------------------------------------------------------
//
//  Description: Main Program for Gaudi applications
//
//------------------------------------------------------------------------------
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IAppMgrUI.h"
#include "GaudiKernel/IProperty.h"
#include "GaudiKernel/SmartIF.h"
#include <iostream>

extern "C" int MDFTest( int argc, char** argv ) {
  IInterface*        iface = Gaudi::createApplicationMgr();
  SmartIF<IProperty> propMgr( iface );
  SmartIF<IAppMgrUI> appMgr( iface );
  if ( !appMgr.isValid() || !propMgr.isValid() ) {
    std::cout << "Fatal error while creating the ApplicationMgr " << std::endl;
    return 1;
  }
  // Get the input configuration file from arguments
  propMgr->setProperty( "JobOptionsPath", std::string( ( argc > 1 ) ? argv[1] : "" ) );
  // Run the application manager and process events
  appMgr->run();
  // Release Application Manager
  iface->release();
  // All done - exit
  return 0;
}

extern "C" int GaudiTask( int argc, char** argv ) { return MDFTest( argc, argv ); }
