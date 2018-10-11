/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "STDet/DeUTSensor.h"
#include "STDet/DeUTSector.h"

using namespace LHCb;

/** @file DeUTSensor.cpp
 *
 *  Implementation of class :  DeUTSensor
 *
 *  @author Jianchun Wang, Matt Needham
 *  @date   2012-07-10
 *
 */

DeUTSensor::DeUTSensor( const std::string& name ) :
  DeSTSensor( name ),
  m_parent(0)
{
}


const CLID& DeUTSensor::clID () const
{
  return DeUTSensor::classID();
}


StatusCode DeUTSensor::initialize()
{
  // initialize method
  StatusCode sc = DeSTSensor::initialize();
  if (sc.isFailure() ){
    MsgStream msg(msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  }
  else {
    // get the parent
    m_parent = getParent<DeUTSensor>();
    int id = param<int>("sensorID");
    setID(id);
    cacheParentProperties(m_parent);
    sc = initGeometryInfo();
    if (sc.isFailure()){
      MsgStream msg(msgSvc(), name() );
      msg<< MSG::ERROR << "failed to cache geometry" << endmsg;
      return sc;
    }
  }
  return StatusCode::SUCCESS;
}
