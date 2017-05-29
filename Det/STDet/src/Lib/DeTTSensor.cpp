#include "STDet/DeTTSensor.h"

#include <typeinfo>

#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SmartIF.h"

/** @file DeTTSensor.cpp
*
*  Implementation of class :  DeTTSensor
*
*    @author Matthew Needham
*/

using namespace LHCb;

DeTTSensor::DeTTSensor( const std::string& name ) :
  DeSTSensor( name ),
  m_parent(0)
{
  // constructer
}

const CLID& DeTTSensor::clID () const
{
  return DeTTSensor::classID() ;
}

StatusCode DeTTSensor::initialize() {

  // initialize method

  StatusCode sc = DeSTSensor::initialize();
  if (sc.isFailure() ){
    MsgStream msg(msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  }
  else {
    // get the parent
    m_parent = getParent<DeTTSensor>();
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

