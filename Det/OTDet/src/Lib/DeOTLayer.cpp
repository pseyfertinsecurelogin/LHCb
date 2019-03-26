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
/// GaudiKernel
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/SystemOfUnits.h"

/// DetDesc
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/SolidBox.h"

/// OTDet
#include "OTDet/DeOTLayer.h"
#include "OTDet/DeOTQuarter.h"

/** @file DeOTLayer.cpp
 *
 *  Implementation of class :  DeOTLayer
 *
 *  @author Jeroen van Tilburg jtilburg@nikhef.nl
 */

using namespace LHCb;

DeOTLayer::DeOTLayer( const std::string& name ) : DetectorElement( name ) { m_quarters.reserve( 4 ); }

const CLID& DeOTLayer::clID() const { return DeOTLayer::classID(); }

StatusCode DeOTLayer::initialize() {

  /// Loop over quarters
  for ( auto iQ = this->childBegin(); iQ != this->childEnd(); ++iQ ) {
    DeOTQuarter* quarter = dynamic_cast<DeOTQuarter*>( *iQ );
    if ( quarter ) {
      m_quarters.push_back( quarter );
      m_mapQuarters.insert( ( quarter->elementID() ).quarter(), quarter );
    }
  } /// iQ

  IDetectorElement* station = this->parentIDetectorElement();
  m_stationID               = (unsigned int)station->params()->param<int>( "stationID" );
  m_layerID                 = (unsigned int)param<int>( "layerID" );
  OTChannelID aChan( m_stationID, m_layerID, 0u, 0u, 0u, 0u );
  setElementID( aChan );

  m_stereoAngle = param<double>( "stereoAngle" );

  /// Update and chache planes
  MsgStream msg( msgSvc(), name() );
  try {
    if ( msg.level() <= MSG::DEBUG ) msg << MSG::DEBUG << "Registering conditions" << endmsg;
    updMgrSvc()->registerCondition( this, this->geometry(), &DeOTLayer::cachePlane );
    if ( msg.level() <= MSG::DEBUG ) msg << MSG::DEBUG << "Start first update" << endmsg;
    StatusCode sc = updMgrSvc()->update( this );
    if ( !sc.isSuccess() ) { return sc; }
  } catch ( DetectorElementException& e ) {
    msg << MSG::ERROR << e << endmsg;
    return StatusCode::FAILURE;
  }
  return StatusCode::SUCCESS;
}

/// Find the quarter for a given XYZ point
const DeOTQuarter* DeOTLayer::findQuarter( const Gaudi::XYZPoint& aPoint ) const {
  auto iQ = std::find_if( m_quarters.begin(), m_quarters.end(),
                          [&]( const DetectorElement* e ) { return e->isInside( aPoint ); } );
  return iQ != m_quarters.end() ? *iQ : nullptr;
}

StatusCode DeOTLayer::cachePlane() {

  auto toGlobal = [&]( double x, double y, double z ) {
    return this->geometry()->toGlobal( Gaudi::XYZPoint{x, y, z} );
  };
  m_plane = Gaudi::Plane3D( toGlobal( 0.0, 0.0, 0.0 ), toGlobal( 3.5 * Gaudi::Units::m, 0.0, 0.0 ),
                            toGlobal( 0.0, 2.5 * Gaudi::Units::m, 0.0 ) );
  return StatusCode::SUCCESS;
}
