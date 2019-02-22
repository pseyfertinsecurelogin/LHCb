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
/** @file DeRich2.cpp
 *
 *  Implementation file for detector description class : DeRich2
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */

// Include files
#include "RichDet/DeRich2.h"
#include "RichDet/DeRichHPDPanel.h"
#include "RichDet/DeRichRadiator.h"
#include "RichDet/DeRichSphMirror.h"

// Gaudi
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/SmartDataPtr.h"

// DetDesc
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/Material.h"

//-----------------------------------------------------------------------------

const CLID CLID_DERich2 = 12002; // User defined

// Standard Constructors
DeRich2::DeRich2( const std::string& name ) : DeRich( name ) {
  m_rich = Rich::Rich2;
  setMyName( "DeRich2" );
}

// Retrieve Pointer to class defininition structure
const CLID& DeRich2::classID() { return CLID_DERich2; }

//===========================================================================

StatusCode DeRich2::initialize() {
  _ri_debug << "Initialize " << name() << endmsg;

  if ( !DeRich::initialize() ) return StatusCode::FAILURE;

  // Declare dependency on mirrors
  for ( const auto mirrType : {"SphericalMirrorDetElemLocations", "SecondaryMirrorDetElemLocations"} ) {
    for ( const auto& loc : paramVect<std::string>( mirrType ) ) {
      SmartDataPtr<DeRichSphMirror> mirror( dataSvc(), loc );
      updMgrSvc()->registerCondition( this, &*mirror, &DeRich2::updateMirrorParams );
    }
  }

  const auto* pvGasWindow = geometry()->lvolume()->pvolume( "pvRich2QuartzWindow:0" );
  if ( pvGasWindow ) {
    for ( auto& mat : pvGasWindow->lvolume()->material()->tabulatedProperties() ) {
      if ( mat ) {
        if ( mat->type() == "RINDEX" ) {
          m_gasWinRefIndex.reset( new Rich::TabulatedProperty1D( mat ) );
          if ( !m_gasWinRefIndex->valid() ) {
            return Error( "Invalid RINDEX Rich::TabulatedProperty1D for " + mat->name() );
          }
        } else if ( mat->type() == "ABSLENGTH" ) {
          m_gasWinAbsLength.reset( new Rich::TabulatedProperty1D( mat ) );
          if ( !m_gasWinAbsLength->valid() ) {
            return Error( "Invalid ABSLENGTH Rich::TabulatedProperty1D for " + mat->name() );
          }
        }
      }
    }
  } else {
    return Error( "Could not find gas window properties" );
  }

  // get the nominal reflectivity of the spherical mirror
  const std::string sphCondName( "NominalSphericalMirrorReflectivityLoc" );
  const std::string sphMirrorReflLoc =
      ( exists( sphCondName ) ? param<std::string>( sphCondName )
                              : "/dd/Geometry/AfterMagnetRegion/Rich2/Rich2SurfaceTabProperties/"
                                "Rich2Mirror1SurfaceIdealReflectivityPT" );

  SmartDataPtr<TabulatedProperty> sphMirrorRefl( dataSvc(), sphMirrorReflLoc );
  if ( !sphMirrorRefl ) {
    return Error( "No info on spherical mirror reflectivity at " + sphMirrorReflLoc );
  } else {
    _ri_debug << "Loaded spherical mirror reflectivity from: " << sphMirrorReflLoc << endmsg;
    m_nominalSphMirrorRefl.reset( new Rich::TabulatedProperty1D( sphMirrorRefl ) );
    if ( !m_nominalSphMirrorRefl->valid() ) {
      return Error( "Invalid Rich::TabulatedProperty1D for " + sphMirrorRefl->name() );
    }
  }

  // get the nominal reflectivity of the secondary mirror
  const std::string secCondName( "NominalSecondaryMirrorReflectivityLoc" );
  const std::string secMirrorReflLoc =
      ( exists( secCondName ) ? param<std::string>( secCondName )
                              : "/dd/Geometry/AfterMagnetRegion/Rich2/Rich2SurfaceTabProperties/"
                                "Rich2Mirror2SurfaceIdealReflectivityPT" );

  SmartDataPtr<TabulatedProperty> secMirrorRefl( dataSvc(), secMirrorReflLoc );
  if ( !secMirrorRefl ) {
    return Error( "No info on secondary mirror reflectivity at " + secMirrorReflLoc );
  } else {
    _ri_debug << "Loaded secondary mirror reflectivity from: " << secMirrorReflLoc << endmsg;
    m_nominalSecMirrorRefl.reset( new Rich::TabulatedProperty1D( secMirrorRefl ) );
    if ( !m_nominalSecMirrorRefl->valid() ) {
      return Error( "Invalid Rich::TabulatedProperty1D for " + secMirrorRefl->name() );
    }
  }

  // initialise various local cached data
  loadPDPanels();
  loadNominalQuantumEff();

  // Trigger first update
  const auto sc = updMgrSvc()->update( this );
  if ( sc.isFailure() ) { fatal() << "UMS updates failed" << endmsg; }
  return sc;
}

//===========================================================================

StatusCode DeRich2::updateMirrorParams() {

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "CoC      " << param<std::vector<double>>( "NominalSphMirrorCoC" ) << endmsg;
    debug() << "SecPlane " << param<std::vector<double>>( "NominalSecMirrorPlane" ) << endmsg;
    debug() << "RoC      " << param<double>( "SphMirrorRadius" ) << endmsg;
  }

  // Load the primary mirror segments to compute the 'nominal' settings
  {
    m_nominalCentresOfCurvature[Rich::left]  = Gaudi::XYZPoint( 0, 0, 0 );
    m_nominalCentresOfCurvature[Rich::right] = Gaudi::XYZPoint( 0, 0, 0 );
    unsigned int nLeft( 0 ), nRight( 0 );
    double       avroc{0};
    for ( const auto& loc : paramVect<std::string>( "SphericalMirrorDetElemLocations" ) ) {
      SmartDataPtr<DeRichSphMirror> mirror( dataSvc(), loc );
      const auto                    plane = mirror->centreNormalPlane();
      const auto                    coc   = mirror->centreOfCurvature();
      const auto                    cen   = mirror->mirrorCentre();
      const auto                    roc   = mirror->radius();
      _ri_debug << loc << " Centre = " << cen << " Plane " << plane.Normal() << " " << plane.HesseDistance()
                << " RoC = " << roc << " CoC = " << coc << endmsg;
      if ( cen.x() > 0 ) {
        ++nLeft;
        m_nominalCentresOfCurvature[Rich::left] += Gaudi::XYZVector( coc );
      } else {
        ++nRight;
        m_nominalCentresOfCurvature[Rich::right] += Gaudi::XYZVector( coc );
      }
      avroc += roc;
    }
    m_nominalCentresOfCurvature[Rich::left] /= (double)nLeft;
    m_nominalCentresOfCurvature[Rich::right] /= (double)nRight;
    m_sphMirrorRadius = avroc / (double)( nLeft + nRight );
  }

  // m_sphMirrorRadius = param<double>("SphMirrorRadius");

  _ri_debug << "Nominal centre of curvature " << m_nominalCentresOfCurvature << endmsg;

  _ri_debug << "Nominal Radius of Curvature = " << m_sphMirrorRadius << endmsg;

  // get the parameters of the nominal flat mirror plane in the form
  // Ax+By+Cz+D=0

  // Load the secondary mirrors to compute the 'nominal' settings
  {
    std::vector<double> nominalFMirrorPlaneLeft{0, 0, 0, 0};
    std::vector<double> nominalFMirrorPlaneRight{0, 0, 0, 0};
    unsigned int        nLeft( 0 ), nRight( 0 );
    for ( const auto& loc : paramVect<std::string>( "SecondaryMirrorDetElemLocations" ) ) {
      SmartDataPtr<DeRichSphMirror> mirror( dataSvc(), loc );
      auto                          plane = mirror->centreNormalPlane();
      const auto                    cen   = mirror->mirrorCentre();
      _ri_debug << loc << " Centre = " << mirror->mirrorCentre() << " Plane " << plane.Normal() << " "
                << plane.HesseDistance() << " RoC = " << mirror->radius() << " CoC = " << mirror->centreOfCurvature()
                << endmsg;
      auto& params = ( cen.x() > 0 ? nominalFMirrorPlaneLeft : nominalFMirrorPlaneRight );
      params[0] += plane.A();
      params[1] += plane.B();
      params[2] += plane.C();
      params[3] += plane.D();
      ++( cen.x() > 0 ? nLeft : nRight );
    }

    m_nominalPlanes[Rich::left] =
        Gaudi::Plane3D( nominalFMirrorPlaneLeft[0] / (double)nLeft, nominalFMirrorPlaneLeft[1] / (double)nLeft,
                        nominalFMirrorPlaneLeft[2] / (double)nLeft, nominalFMirrorPlaneLeft[3] / (double)nLeft );
    m_nominalPlanes[Rich::right] =
        Gaudi::Plane3D( nominalFMirrorPlaneRight[0] / (double)nRight, nominalFMirrorPlaneRight[1] / (double)nRight,
                        nominalFMirrorPlaneRight[2] / (double)nRight, nominalFMirrorPlaneRight[3] / (double)nRight );
  }

  _ri_debug << "Nominal Plane Left " << m_nominalPlanes[Rich::left].Normal() << " "
            << m_nominalPlanes[Rich::left].HesseDistance() << endmsg;
  _ri_debug << "Nominal Plane Right " << m_nominalPlanes[Rich::right].Normal() << " "
            << m_nominalPlanes[Rich::right].HesseDistance() << endmsg;

  m_nominalNormals[Rich::left]  = m_nominalPlanes[Rich::left].Normal();
  m_nominalNormals[Rich::right] = m_nominalPlanes[Rich::right].Normal();

  _ri_debug << "Nominal normal " << m_nominalNormals << endmsg;

  // Fill SIMD caches
  fillSIMDTypes();

  return StatusCode::SUCCESS;
}

//=========================================================================
// Access the name for a given panel
//=========================================================================
const std::string DeRich2::panelName( const Rich::Side panel ) const {

  std::string pname = ( Rich::left == panel ? DeRichLocations::Rich1Panel0 : DeRichLocations::Rich1Panel1 );

  if ( RichPhotoDetConfig() == Rich::HPDConfig ) {

    if ( exists( "Rich2HPDPanelDetElemLocations" ) ) {
      const auto& panelLoc = paramVect<std::string>( "Rich2HPDPanelDetElemLocations" );
      pname                = panelLoc[panel];
    } else if ( exists( "HPDPanelDetElemLocations" ) ) { // kept for backward compatibility
      const auto& panelLoc = paramVect<std::string>( "HPDPanelDetElemLocations" );
      pname                = panelLoc[panel];
    }
  } else if ( RichPhotoDetConfig() == Rich::PMTConfig ) {

    if ( exists( "Rich2PMTPanelDetElemLocations" ) ) {
      const auto& panelLoc = paramVect<std::string>( "Rich2PMTPanelDetElemLocations" );
      pname                = panelLoc[panel];
    }
  }

  return pname;
}
