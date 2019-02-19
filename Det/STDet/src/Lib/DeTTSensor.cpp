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
#include "STDet/DeTTSensor.h"

/** @file DeTTSensor.cpp
 *
 *  Implementation of class :  DeTTSensor
 *
 *    @author Matthew Needham
 */

#include "STDet/DeTTSector.h"

using namespace LHCb;

DeTTSensor::DeTTSensor( const std::string& name ) : DeSTSensor( name ), m_parent( 0 ) {
  // constructer
}

const CLID& DeTTSensor::clID() const { return DeTTSensor::classID(); }

StatusCode DeTTSensor::initialize() {

  // initialize method

  StatusCode sc = DeSTSensor::initialize();
  if ( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  } else {
    // get the parent
    m_parent = getParent<DeTTSensor>();
    int id   = param<int>( "sensorID" );
    setID( id );
    cacheParentProperties( m_parent );
    sc = initGeometryInfo();
    if ( sc.isFailure() ) {
      MsgStream msg( msgSvc(), name() );
      msg << MSG::ERROR << "failed to cache geometry" << endmsg;
      return sc;
    }
  }
  return StatusCode::SUCCESS;
}
