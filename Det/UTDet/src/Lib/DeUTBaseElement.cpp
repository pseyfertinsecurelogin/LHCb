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

#include "UTDet/DeUTBaseElement.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "DetDesc/IGeometryInfo.h"


/** @file DeUTBaseElement.cpp
*
*  Implementation of class :  DeUTBaseElement
*
*    @author Andy Beiter (based on code by Matthew Needham)
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



