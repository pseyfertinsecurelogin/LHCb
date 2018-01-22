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


