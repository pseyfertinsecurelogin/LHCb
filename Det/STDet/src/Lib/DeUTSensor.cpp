#include "STDet/DeUTSensor.h"

#include <typeinfo>

#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SmartIF.h"

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
