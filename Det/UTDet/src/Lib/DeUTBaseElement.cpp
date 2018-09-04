
#include "UTDet/DeUTBaseElement.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "DetDesc/IGeometryInfo.h"


/** @file DeUTBaseElement.cpp
*
*  Implementation of class :  DeUTBaseElement
*
*    @author Matthew Needham
*/

using namespace LHCb;

DeUTBaseElement::DeUTBaseElement( const std::string& name ) :
  DetectorElement( name )
{
  // constructer
}


StatusCode DeUTBaseElement::initialize() {

  // initialize method

  StatusCode sc = DetectorElement::initialize();
  if (sc.isFailure() ){
    MsgStream msg(msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
    return StatusCode::FAILURE;
  }

  // cache trajectories
  sc = registerCondition(this,this->geometry(),&DeUTBaseElement::cachePoint,true);
  if (sc.isFailure() ){
    MsgStream msg(msgSvc(), name() );
    msg << MSG::ERROR << "Failed to register conditions" << endmsg;
    return StatusCode::FAILURE;
  }

  return StatusCode::SUCCESS;
}

StatusCode DeUTBaseElement::cachePoint(){

  m_globalCentre = toGlobal(Gaudi::XYZPoint(0,0,0));
  return StatusCode::SUCCESS;
}


Gaudi::XYZPoint DeUTBaseElement::globalPoint( const double x, const double y,
                                         const double z) const
{
  const Gaudi::XYZPoint lPoint(x,y,z);
  Gaudi::XYZPoint gPoint = toGlobal(lPoint);
  return gPoint;
}



