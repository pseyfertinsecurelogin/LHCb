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
#include "BcmDet/DeBcm.h"

DeBcm::DeBcm(int nStation) :
  m_StationNumber(nStation)
{
}


StatusCode DeBcm::initialize()
{

  StatusCode sc = DetectorElement::initialize();
  if( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failure to initialize DetectorElement" << endmsg;
    return sc ;
  }

  int station = param<int>("stationId");
  setStationNumber(station);
  return sc;
}

int DeBcm::sensitiveVolumeID( const Gaudi::XYZPoint& point ) const {
  const IDetectorElement* sensor = childDEWithPoint(point);
  if( !sensor ) return -1;
  int nSensor = sensor->sensitiveVolumeID(point);
  if( nSensor == -1) return nSensor;
  return (stationNumber()<<DeBcmShifts::shiftStationID)|(nSensor<<DeBcmShifts::shiftSensorID);
}

