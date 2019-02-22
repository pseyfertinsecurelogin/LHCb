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

#include "STDet/DeSTBaseElement.h"
#include "DetDesc/IGeometryInfo.h"
#include "GaudiKernel/IUpdateManagerSvc.h"

/** @file DeSTBaseElement.cpp
 *
 *  Implementation of class :  DeSTBaseElement
 *
 *    @author Matthew Needham
 */

using namespace LHCb;

DeSTBaseElement::DeSTBaseElement( const std::string& name ) : DetectorElement( name ) {
  // constructer
}

StatusCode DeSTBaseElement::initialize() {

  // initialize method

  StatusCode sc = DetectorElement::initialize();
  if ( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
    return StatusCode::FAILURE;
  }

  // cache trajectories
  sc = registerCondition( this, this->geometry(), &DeSTBaseElement::cachePoint, true );
  if ( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to register conditions" << endmsg;
    return StatusCode::FAILURE;
  }

  return StatusCode::SUCCESS;
}

StatusCode DeSTBaseElement::cachePoint() {

  m_globalCentre = toGlobal( Gaudi::XYZPoint( 0, 0, 0 ) );
  return StatusCode::SUCCESS;
}

Gaudi::XYZPoint DeSTBaseElement::globalPoint( const double x, const double y, const double z ) const {
  const Gaudi::XYZPoint lPoint( x, y, z );
  Gaudi::XYZPoint       gPoint = toGlobal( lPoint );
  return gPoint;
}
