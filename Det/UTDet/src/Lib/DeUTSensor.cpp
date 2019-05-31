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
#include "UTDet/DeUTSensor.h"
#include "UTDet/DeUTSector.h"

#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/SolidBox.h"

#include <algorithm>

// Kernel
#include "Kernel/LHCbID.h"
#include "Kernel/LineTraj.h"

#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/SystemOfUnits.h"

#include "GaudiKernel/IUpdateManagerSvc.h"

using namespace LHCb;

/** @file DeUTSensor.cpp
 *
 *  Implementation of class :  DeUTSensor
 *
 *  @author Andy Beiter (based on code by Jianchun Wang, Matt Needham)
 *  @date   2018-09-04
 *
 */

DeUTSensor::DeUTSensor( const std::string& name ) : DeUTBaseElement( name ) {}

void DeUTSensor::clear() { m_midTraj.reset(); }

std::ostream& DeUTSensor::printOut( std::ostream& os ) const {

  // stream to cout

  const ILVolume* lv      = this->geometry()->lvolume();
  const SolidBox* mainBox = dynamic_cast<const SolidBox*>( lv->solid() );

  return os << " Sensor :  " << name() << "\n ID " << id() << "\n pitch " << m_pitch << "\n strip " << m_nStrip
            << "\n active width" << m_uMaxLocal - m_uMinLocal << "\n total width " << mainBox->xsize()
            << "\n active height" << m_vMaxLocal - m_vMinLocal << "\n total height " << mainBox->ysize()
            << "\n dead width " << m_deadWidth << "\n center " << globalCentre() << '\n';
}

MsgStream& DeUTSensor::printOut( MsgStream& os ) const {

  // stream to Msg service
  return os << " Sensor : \n " << name() << "\n ID " << id() << " pitch \n " << m_pitch << "n strip \n " << m_nStrip
            << " u min \n " << m_uMinLocal << " u max \n " << m_uMaxLocal << " v min \n " << m_vMinLocal
            << " v max  \n " << m_vMaxLocal << "dead width \n " << m_deadWidth << "\n center " << globalCentre()
            << '\n';
}

const CLID& DeUTSensor::clID() const { return DeUTSensor::classID(); }

StatusCode DeUTSensor::initialize() {
  // initialize method
  StatusCode sc = DeUTBaseElement::initialize();
  if ( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  } else {
    // get the parent
    m_parent = getParent<DeUTSensor>();
    int id   = param<int>( "sensorID" );
    setID( id );
    cacheParentProperties( m_parent );
    sc = initGeometryInfo();
    if ( sc.isFailure() ) {
      MsgStream msg( msgSvc(), name() );
      msg << MSG::ERROR << "failed to cache geometry" << endmsg;
      return sc;
    }
  }
  return StatusCode::SUCCESS;
}

unsigned int DeUTSensor::localUToStrip( const double u ) const {

  // convert local u to a strip
  unsigned int strip;
  if ( m_xInverted == true ) {
    strip = (unsigned int)floor( ( ( m_uMaxLocal - u ) / m_pitch ) + 0.5 );
  } else {
    strip = (unsigned int)floor( ( ( u - m_uMinLocal ) / m_pitch ) + 0.5 );
  }

  return ( isStrip( strip ) ? strip : 0 );
}

double DeUTSensor::localU( const unsigned int strip, const double offset ) const {
  // strip to local

  double u      = -999.;
  double tStrip = strip + offset;
  if ( m_xInverted == true ) {
    u = m_uMaxLocal + m_pitch * ( 0.5 - tStrip );
  } else {
    u = m_uMinLocal + m_pitch * ( tStrip - 0.5 );
  }
  return u;
}

bool DeUTSensor::localInActive( const Gaudi::XYZPoint& point, Gaudi::XYZPoint tol ) const {
  const double u = point.x();
  const double v = point.y();
  return ( ( u - tol.X() ) < ( m_uMaxLocal + ( 0.5 * m_pitch ) ) &&
           ( u + tol.X() ) > ( m_uMinLocal - ( 0.5 * m_pitch ) ) && ( ( v - tol.Y() ) < m_vMaxLocal ) &&
           ( ( v + tol.Y() ) > m_vMinLocal ) );
}

LHCb::LineTraj<double> DeUTSensor::trajectory( const unsigned int strip, const double offset ) const {

  const double arclen = ( offset + strip - m_firstStrip ) * m_pitch;

  Gaudi::XYZPoint midPoint = m_midTraj->position( arclen + m_midTraj->beginRange() );

  return {midPoint, m_direction, m_range, Trajectory<double>::DirNormalized{true}};
}

void DeUTSensor::determineSense() {
  Gaudi::XYZPoint g1 = globalPoint( m_uMinLocal, m_vMinLocal, 0. );
  Gaudi::XYZPoint g2 = globalPoint( m_uMaxLocal, m_vMinLocal, 0. );
  if ( g1.x() > g2.x() ) { m_xInverted = true; }

  Gaudi::XYZPoint g3 = globalPoint( m_uMinLocal, m_vMaxLocal, 0. );
  if ( g1.y() > g3.y() ) { m_yInverted = true; }
}

StatusCode DeUTSensor::cacheInfo() {

  clear();

  float yUpper = m_vMaxLocal;
  float yLower = m_vMinLocal;
  if ( m_yInverted ) std::swap( yUpper, yLower );

  float xUpper = m_uMaxLocal - 0.5 * m_pitch;
  float xLower = m_uMinLocal + 0.5 * m_pitch;
  if ( m_xInverted ) std::swap( xUpper, xLower );

  // direction
  const Gaudi::XYZPoint g1 = globalPoint( xLower, yLower, 0. );
  const Gaudi::XYZPoint g2 = globalPoint( xLower, yUpper, 0. );
  m_direction              = g2 - g1;
  m_direction              = m_direction.Unit();

  // trajectory of middle
  const Gaudi::XYZPoint g3 = globalPoint( xLower, 0., 0. );
  const Gaudi::XYZPoint g4 = globalPoint( xUpper, 0., 0. );
  m_midTraj.emplace( g3, g4 );

  // range ---> strip Length
  m_range = std::make_pair( -0.5 * m_stripLength, 0.5 * m_stripLength );

  // plane
  m_plane = Gaudi::Plane3D( g1, g2, g4 );

  m_entryPlane = Gaudi::Plane3D( m_plane.Normal(), globalPoint( 0., 0., -0.5 * m_thickness ) );
  m_exitPlane  = Gaudi::Plane3D( m_plane.Normal(), globalPoint( 0., 0., 0.5 * m_thickness ) );

  return StatusCode::SUCCESS;
}

StatusCode DeUTSensor::initGeometryInfo() {

  // geometry: uMin, uMax
  const ILVolume* lv      = this->geometry()->lvolume();
  const SolidBox* mainBox = dynamic_cast<const SolidBox*>( lv->solid() );

  m_uMaxLocal = 0.5 * ( m_pitch * m_nStrip );
  m_uMinLocal = -m_uMaxLocal;

  // and vMin, vMax
  m_vMaxLocal = 0.5 * mainBox->ysize() - m_deadWidth;
  m_vMinLocal = -m_vMaxLocal;

  m_stripLength = fabs( m_vMaxLocal - m_vMinLocal );

  // thickness
  m_thickness = mainBox->zsize();

  // sense in x and y...
  determineSense();

  StatusCode sc = registerConditionsCallbacks();
  if ( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to registerConditions call backs";
    return sc;
  }
  return StatusCode::SUCCESS;
}

StatusCode DeUTSensor::registerConditionsCallbacks() {

  // cache trajectories
  // initialize method

  StatusCode sc = registerCondition( this, this->geometry(), &DeUTSensor::cacheInfo, true );
  if ( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to register geometry conditions" << endmsg;
    return StatusCode::FAILURE;
  }

  return StatusCode::SUCCESS;
}

void DeUTSensor::cacheParentProperties( const DeUTSector* parent ) {

  m_versionString = parent->versionString();
  m_pitch         = parent->pitch();
  m_nStrip        = parent->nStrip();
  m_deadWidth     = parent->deadWidth();
  setElementID( parent->elementID() );
}
