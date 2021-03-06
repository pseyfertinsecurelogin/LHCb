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

/** @file DeRichSphMirror.cpp
 *
 *  Implementation file for detector description class : DeRichSphMirror
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */

// Include files
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/RegistryEntry.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/SystemOfUnits.h"

#include "GaudiKernel/Transform3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

#include "Kernel/RichDetectorType.h"

/// Detector description classes
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/SolidBoolean.h"
#include "DetDesc/SolidSphere.h"
#include "DetDesc/Surface.h"

#include "RichDet/DeRich.h"
#include "RichDet/DeRichSphMirror.h"

//======================================================================

const CLID CLID_DeRichSphMirror = 12030; // User defined

// Standard Constructor
DeRichSphMirror::DeRichSphMirror( const std::string& name ) : DeRichBase( name ) { setMyName( "DeRichSphMirror" ); }

// Retrieve Pointer to class defininition structure
const CLID& DeRichSphMirror::classID() { return CLID_DeRichSphMirror; }

StatusCode DeRichSphMirror::initialize() {
  // Run first update of cached information
  // Sets up UMS as well for future updates
  return DeRichSphMirror::updateGeometry();
}

//=========================================================================
//  intersects with return point
//=========================================================================
StatusCode DeRichSphMirror::intersects( const Gaudi::XYZPoint& globalP, const Gaudi::XYZVector& globalV,
                                        Gaudi::XYZPoint& intersectionPoint ) const {
  const auto pLocal = geometry()->toLocal( globalP );
  const auto vLocal = geometry()->toLocalMatrix() * globalV;

  ISolid::Ticks ticks;
  const auto    noTicks = m_solid->intersectionTicks( pLocal, vLocal, ticks );

  if ( 0 == noTicks ) {
    return StatusCode::FAILURE;
  } else {
    intersectionPoint = geometry()->toGlobal( pLocal + ticks[0] * vLocal );
    return StatusCode::SUCCESS;
  }
}

//=========================================================================
//  intersects
//=========================================================================
StatusCode DeRichSphMirror::intersects( const Gaudi::XYZPoint& globalP, const Gaudi::XYZVector& globalV ) const {
  const auto vLocal = geometry()->toLocalMatrix() * globalV;

  ISolid::Ticks ticks;
  const auto    noTicks = m_solid->intersectionTicks( geometry()->toLocal( globalP ), vLocal, ticks );

  return ( 0 == noTicks ? StatusCode::FAILURE : StatusCode::SUCCESS );
}

//=========================================================================
//  updateGeometry
//=========================================================================
StatusCode DeRichSphMirror::updateGeometry() {
  // If first update, set dependency on geometry
  if ( m_firstUpdate ) updMgrSvc()->registerCondition( this, geometry(), &DeRichSphMirror::updateGeometry );

  // find if this mirror is in Rich1 or Rich2 and
  // if this is a primary or secondary mirror
  bool                         secondary( false );
  Rich::DetectorType           rich = Rich::InvalidDetector;
  const std::string::size_type pos  = name().find( "Rich" );
  if ( std::string::npos != pos ) {
    setMyName( name().substr( pos ) );
    std::string richNum = myName().substr( 4, 1 );
    if ( "1" == richNum ) {
      rich              = Rich::Rich1;
      const auto secPos = name().find( "Mirror2" );
      if ( std::string::npos != secPos ) secondary = true;
    } else if ( "2" == richNum ) {
      rich              = Rich::Rich2;
      const auto secPos = name().find( "SecMirror" );
      if ( std::string::npos != secPos ) secondary = true;
    } else if ( "/" == richNum ) {
      rich              = Rich::Rich;
      const auto secPos = name().find( "SecMirror" );
      if ( std::string::npos != secPos ) secondary = true;
    } else {
      fatal() << "Could not identify Rich (1/2==" << richNum << " )" << endmsg;
      return StatusCode::FAILURE;
    }
  } else {
    // setMyName("DeRichSphMirror_NO_NAME");
    fatal() << "Cannot identify Rich number!" << endmsg;
    return StatusCode::FAILURE;
  }

  if ( msgLevel( MSG::DEBUG ) ) debug() << "Initializing spherical mirror" << endmsg;

  // extract mirror number from detector element name
  const auto pos2 = name().rfind( ':' );
  if ( std::string::npos == pos2 ) {
    fatal() << "A spherical mirror without a number!" << endmsg;
    return StatusCode::FAILURE;
  }
  m_mirrorNumber                 = atoi( name().substr( pos2 + 1 ).c_str() );
  const std::string mirNumString = name().substr( pos2 + 1 );

  // get DeRich2
  //  SmartDataPtr<DeRich> deRich2(dataSvc(), DeRichLocations::Rich2);
  //  if (!deRich2) {
  //    fatal() << "Cannot locate " << DeRichLocations::Rich2 << endmsg;
  //    return StatusCode::FAILURE;
  //  }

  //  double hexRadius = 510.0*Gaudi::Units::mm;
  //  if ( deRich2->exists("Rich2SphMirrorHexDiameter") )
  //    hexRadius = deRich2->param<double>("Rich2SphMirrorHexDiameter")/2.0;

  m_solid                = geometry()->lvolume()->solid();
  const std::string type = m_solid->typeName();

  const SolidSphere* sphereSolid = nullptr;
  // find the sphere of the spherical mirror
  if ( type == "SolidSphere" ) {
    sphereSolid = dynamic_cast<const SolidSphere*>( m_solid );
  } else {
    // assume boolean solid
    const auto* compSolid = dynamic_cast<const SolidBoolean*>( m_solid );
    if ( !compSolid ) {
      error() << "Problem loading SolidBoolean" << endmsg;
      return StatusCode::FAILURE;
    } else {
      if ( compSolid->first()->typeName() == "SolidSphere" ) {
        sphereSolid = dynamic_cast<const SolidSphere*>( compSolid->first() );
      } else {
        for ( const auto& c : compSolid->children() ) {
          if ( c.solid()->typeName() == "SolidSphere" ) sphereSolid = dynamic_cast<const SolidSphere*>( c.solid() );
        }
      }
      if ( !sphereSolid ) {
        fatal() << "Problem finding a sphere solid in :- " << endmsg;
        compSolid->printOut();
        return StatusCode::FAILURE;
      }
    }
  }

  // Now that we have the sphere, try to find out its radius
  m_radius = sphereSolid->insideRadius();

  // and its centre
  m_localOrigin = Gaudi::XYZVector( 0.0, 0.0, 0.0 );
  Gaudi::Polar3DVector toSphCentre( 1.0, 0.0, 0.0 );
  if ( 0.0 != sphereSolid->startThetaAngle() ) {
    toSphCentre.SetCoordinates( 1.0, sphereSolid->startThetaAngle() + sphereSolid->deltaThetaAngle() / 2.0,
                                sphereSolid->startPhiAngle() + sphereSolid->deltaPhiAngle() / 2.0 );
  }

  // convert to XYZ
  const Gaudi::XYZVector toSphCentreXYZ( toSphCentre );

  ISolid::Ticks sphTicks;
  const auto    sphTicksSize = sphereSolid->intersectionTicks( m_localOrigin, toSphCentreXYZ, sphTicks );
  if ( sphTicksSize != 2 ) {
    fatal() << "Problem getting mirror radius, noTicks: " << sphTicksSize << endmsg;
    return StatusCode::FAILURE;
  }

  m_localMirrorCentre = Gaudi::XYZPoint( sphTicks[0] * toSphCentreXYZ );
  m_mirrorCentre      = geometry()->toGlobal( m_localMirrorCentre );
  m_centreOfCurvature = geometry()->toGlobal( m_localOrigin );

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "Mirror #" << m_mirrorNumber << " " << rich << " Radius:" << m_radius << " Centre of curvature "
            << m_centreOfCurvature << endmsg;
    debug() << "Centre of mirror " << m_mirrorCentre << endmsg;
  }

  // the following lines can be uncommented for debuging
  // right and left middle points are for verification of the hex segment position
  // const double flatToCentre = hexRadius*sin(60*degree);
  // const Gaudi::XYZPoint middleRightSide
  //  (sqrt(m_radius*m_radius-flatToCentre*flatToCentre),0.0,flatToCentre);
  // const Gaudi::XYZPoint middleLeftSide
  //  (sqrt(m_radius*m_radius-flatToCentre*flatToCentre),0.0,-flatToCentre);
  // if( rich == Rich::Rich2 ) {
  //  if( (m_mirrorNumber == 31) || (m_mirrorNumber < 28 && m_mirrorNumber != 0) )
  //    msg << MSG::VERBOSE << "Right middle "
  //        << geometry()->toGlobal(middleRightSide) << endmsg;
  //  else
  //    msg << MSG::VERBOSE << "Left middle "
  //        << geometry()->toGlobal(middleLeftSide) << endmsg;
  //}

  // centre normal vector
  const Gaudi::XYZVector localCentreNormal(
      Gaudi::XYZVector( -m_localMirrorCentre.x(), -m_localMirrorCentre.y(), -m_localMirrorCentre.z() ).unit() );
  m_centreNormal = geometry()->toGlobalMatrix() * localCentreNormal;
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Normal vector at the centre" << m_centreNormal << endmsg;

  m_centreNormalPlane = Gaudi::Plane3D( m_centreNormal, m_mirrorCentre );
  if ( msgLevel( MSG::VERBOSE ) ) verbose() << "centreNormalPlane " << m_centreNormalPlane << endmsg;

  Surface*    surf( 0 );
  std::string surfLocation;
  if ( !exists( "SurfaceLocation" ) ) {
    // find surface properties the old way
    std::string sphMirrorName, surfName;
    if ( rich == Rich::Rich1 ) {
      surfLocation  = "/dd/Geometry/BeforeMagnetRegion/Rich1/Rich1Surfaces";
      sphMirrorName = ( secondary ? "Mirror2" : "Mirror1" );
      surfName      = ":" + mirNumString;
    } else {
      surfLocation  = "/dd/Geometry/AfterMagnetRegion/Rich2/Rich2Surfaces";
      sphMirrorName = ( secondary ? "SecMirror" : "SphMirror" );
      surfName      = "Seg" + mirNumString;
    }

    // get the surface catalog
    SmartDataPtr<DataObject> rich2SurfCat( dataSvc(), surfLocation );
    if ( !rich2SurfCat ) {
      fatal() << "Cannot locate surface for mirror " << name() << endmsg;
      return StatusCode::FAILURE;
    }

    auto* rich2Reg = dynamic_cast<DataSvcHelpers::RegistryEntry*>( rich2SurfCat->registry() );
    if ( !rich2Reg ) {
      error() << "Problem accessing surface registry" << endmsg;
      return StatusCode::FAILURE;
    }

    // find the surface in the registry
    auto iStore = std::find_if( rich2Reg->begin(), rich2Reg->end(), [&]( const auto& child ) {
      return child->name().find( sphMirrorName ) != std::string::npos &&
             child->name().find( surfName ) != std::string::npos;
    } );

    // get the surface, get the tabulated properties and find REFLECTIVITY
    if ( iStore != rich2Reg->end() ) {
      SmartDataPtr<DataObject> obj( dataSvc(), ( *iStore )->identifier() );
      DataObject*              pObj = obj;
      if ( pObj ) {
        if ( msgLevel( MSG::VERBOSE ) ) verbose() << "Dynamic cast to surface " << pObj->name() << endmsg;
        surf = dynamic_cast<Surface*>( pObj );
      } else {
        error() << "Failed to load storeReg identifier" << endmsg;
        return StatusCode::FAILURE;
      }
    } else {
      fatal() << "Could not find surface for mirror " << myName() << endmsg;
      return StatusCode::FAILURE;
    }
  } else {
    // new way, use user parameter for the surface location
    surfLocation = param<std::string>( "SurfaceLocation" );
    SmartDataPtr<Surface> s( dataSvc(), surfLocation );
    if ( !s ) {
      fatal() << "Could not find surface for mirror " << myName() << " at location " << surfLocation << endmsg;
      return StatusCode::FAILURE;
    }
    surf = s;
  }

  bool foundRefl( false );
  for ( auto& table : surf->tabulatedProperties() ) {
    if ( table->type() == "REFLECTIVITY" ) {
      m_reflectivity = std::make_unique<Rich::TabulatedProperty1D>( table );
      foundRefl      = true;
      break;
    }
  }
  if ( !foundRefl ) {
    fatal() << "Could not find REFLECTIVITY " << surf->name() << endmsg;
    return StatusCode::FAILURE;
  }

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "Reflectivity is from TabProp " << m_reflectivity->tabProperty()->name() << endmsg;
    debug() << "Second volume is " << surf->secondVol() << endmsg;
  }
  _ri_verbo << m_reflectivity->tabProperty() << endmsg;

  // update SIMD float mirror data cache
  m_mirrorData.radius = radius();
  m_mirrorData.planeA = centreNormalPlane().A();
  m_mirrorData.planeB = centreNormalPlane().B();
  m_mirrorData.planeC = centreNormalPlane().C();
  m_mirrorData.planeD = centreNormalPlane().D();
  m_mirrorData.cocX   = centreOfCurvature().X();
  m_mirrorData.cocY   = centreOfCurvature().Y();
  m_mirrorData.cocZ   = centreOfCurvature().Z();

  m_firstUpdate = false;

  return StatusCode::SUCCESS;
}
