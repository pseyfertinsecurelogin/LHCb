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
//  Package    : GaudiConf
//
//  Description: Main Program for Gaudi applications
//               Sets up default job options path as ../options/job.opts
//
//------------------------------------------------------------------------------
// Include files
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IAppMgrUI.h"
#include "GaudiKernel/IProperty.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"
#include <iostream>

//--- Example main program
int main( int argc, char** argv ) {
  // Create an instance of an application manager
  IInterface*        iface = Gaudi::createApplicationMgr();
  SmartIF<IProperty> propMgr( iface );
  SmartIF<IAppMgrUI> appMgr( iface );

  if ( !appMgr.isValid() || !propMgr.isValid() ) {
    std::cout << "Fatal error while creating the ApplicationMgr " << std::endl;
    return 1;
  }

  // Get the input configuration file from arguments
  std::string opts = ( argc > 1 ) ? argv[1] : "";

  propMgr->setProperty( "JobOptionsPath", opts ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );

  // Set the program name and version using macros defined in compilation flags
#if defined APPNAME
  propMgr->setProperty( "AppName", APPNAME );
#endif
#if defined APPVERS
  propMgr->setProperty( "AppVersion", APPVERS );
#endif

  if ( opts != "" && opts.substr( opts.length() - 3, 3 ) == ".py" ) {
    propMgr->setProperty( "EvtSel", "NONE" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    propMgr->setProperty( "JobOptionsType", "NONE" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    propMgr->setProperty( "DLLs", "['GaudiPython']" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    propMgr->setProperty( "Runable", "PythonScriptingSvc" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  }

  // Run the application manager and process events
  StatusCode sc = appMgr->run();

  // Release Application Manager
  iface->release();

  // All done - exit
  return ( sc == StatusCode::SUCCESS ) ? 0 : 1;
}
