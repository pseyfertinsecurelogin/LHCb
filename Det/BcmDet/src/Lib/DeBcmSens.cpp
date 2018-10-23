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
#include "BcmDet/DeBcmSens.h"


DeBcmSens::DeBcmSens(int nSensor) :
  m_SensorNumber(nSensor)
{
}


StatusCode DeBcmSens::initialize()
{
  StatusCode sc = DetectorElement::initialize();
  if( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failure to initialize DetectorElement" << endmsg;
    return sc ;
  }
  int sensor = param<int>("sensId");
  setSensorNumber(sensor);
  return sc;
}

int DeBcmSens::sensitiveVolumeID( const Gaudi::XYZPoint& point )const{
  return isInside( point ) ? sensorNumber() : -1 ;
}


