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
#include "UTDet/DeUTModule.h"
using namespace LHCb;

/** @file DeUTModule.cpp
 *
 *  Implementation of class :  DeUTModule
 *
 *  @author Andy Beiter (based on code by Jianchun Wang, Matt Needham)
 *  @date   2018-09-04
 *
 */

DeUTModule::DeUTModule( const std::string& name ) : DeUTStave( name ) {}

StatusCode DeUTModule::initialize() {
  // initialize method
  StatusCode sc = DeUTStave::initialize();

  if ( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
    return StatusCode::FAILURE;
  }

  return StatusCode::SUCCESS;
}
