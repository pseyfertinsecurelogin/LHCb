/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
/** @file DeHorizRich.cpp
 *
 *  Implementation file for detector description class : DeHorizRich
 *
 *  @author Antonis Papanestis antonis.papanestis@stfc.ac.uk
 *  @date   2012-10-18
 */

// Include files
#include "RichDet/DeHorizRich.h"
#include "RichDet/DeRichHPDPanel.h"
#include "RichDet/DeRichRadiator.h"

// Gaudi
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/SmartDataPtr.h"

// DetDesc
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/Material.h"

//-----------------------------------------------------------------------------

const CLID CLID_DEHorizRich = 12003; // User defined

// Standard Constructors
DeHorizRich::DeHorizRich( const std::string &name ) : DeRich( name )
{
  m_rich = Rich::Rich;
  setMyName( "DeHorizRich" );
}

// Retrieve Pointer to class defininition structure
const CLID &
DeHorizRich::classID()
{
  return CLID_DEHorizRich;
}

//=========================================================================
//  initialize
//=========================================================================
StatusCode
DeHorizRich::initialize()
{
  _ri_debug << "Initialize " << name() << endmsg;

  if ( !DeRich::initialize() ) return StatusCode::FAILURE;

  const std::vector< double > &nominalCoC = param< std::vector< double > >( "NominalSphMirrorCoC" );
  m_nominalCentresOfCurvature[Rich::left] =
    Gaudi::XYZPoint( nominalCoC[0], nominalCoC[1], nominalCoC[2] );
  m_nominalCentresOfCurvature[Rich::right] =
    Gaudi::XYZPoint( -nominalCoC[0], nominalCoC[1], nominalCoC[2] );

  m_sphMirrorRadius = param< double >( "SphMirrorRadius" );

  // get the parameters of the nominal flat mirror plane in the form
  // Ax+By+Cz+D=0
  std::vector< double > nominalFMirrorPlane;
  nominalFMirrorPlane = param< std::vector< double > >( "NominalSecMirrorPlane" );

  m_nominalPlanes[Rich::left]  = Gaudi::Plane3D( nominalFMirrorPlane[0],
                                                nominalFMirrorPlane[1],
                                                nominalFMirrorPlane[2],
                                                nominalFMirrorPlane[3] );
  m_nominalPlanes[Rich::right] = Gaudi::Plane3D( -nominalFMirrorPlane[0],
                                                 nominalFMirrorPlane[1],
                                                 nominalFMirrorPlane[2],
                                                 nominalFMirrorPlane[3] );

  m_nominalNormals[Rich::left]  = m_nominalPlanes[Rich::left].Normal();
  m_nominalNormals[Rich::right] = m_nominalPlanes[Rich::right].Normal();

  _ri_debug << "Spherical Mirror radius:" << m_sphMirrorRadius << endmsg;
  _ri_debug << "Nominal centre of curvature" << m_nominalCentresOfCurvature << endmsg;
  _ri_debug << "Nominal normal " << m_nominalNormals << endmsg;

  // get the refractive index of the window between gas radiator and photon detectors
  const std::string gasWinRefIndxLoc = param< std::string >( "RichGasWindowRefIndexLoc" );
  SmartDataPtr< TabulatedProperty > gasWinRefIndxTab( dataSvc(), gasWinRefIndxLoc );
  if ( gasWinRefIndxTab )
  {
    _ri_debug << "Loaded gas win ref index from: " << gasWinRefIndxLoc << endmsg;
    m_gasWinRefIndex.reset( new Rich::TabulatedProperty1D( gasWinRefIndxTab ) );
    if ( !m_gasWinRefIndex->valid() )
    {
      error() << "Invalid RINDEX Rich::TabulatedProperty1D for " << gasWinRefIndxTab->name()
              << endmsg;
      return StatusCode::FAILURE;
    }
  }
  else
  {
    error() << "No info on gas window ref index at " << gasWinRefIndxLoc << endmsg;
    return StatusCode::FAILURE;
  }

  // get the absorption length of the window between gas radiator and photon detectors
  const std::string gasWinAbsLenLoc = param< std::string >( "RichGasWindowRefIndexLoc" );
  SmartDataPtr< TabulatedProperty > gasWinAbsLenTab( dataSvc(), gasWinAbsLenLoc );
  if ( gasWinAbsLenTab )
  {
    _ri_debug << "Loaded gas win abs length from: " << gasWinAbsLenLoc << endmsg;
    m_gasWinAbsLength.reset( new Rich::TabulatedProperty1D( gasWinAbsLenTab ) );
    if ( !m_gasWinAbsLength->valid() )
    {
      error() << "Invalid ABSLENGTH Rich::TabulatedProperty1D for " << gasWinAbsLenTab->name()
              << endmsg;
      return StatusCode::FAILURE;
    }
  }
  else
  {
    error() << "No info on gas win abs length at " << gasWinAbsLenLoc << endmsg;
    return StatusCode::FAILURE;
  }

  // get the nominal reflectivity of the spherical mirror
  const std::string sphMirrorReflLoc =
    param< std::string >( "NominalSphericalMirrorReflectivityLoc" );
  SmartDataPtr< TabulatedProperty > sphMirrorRefl( dataSvc(), sphMirrorReflLoc );
  if ( !sphMirrorRefl )
  {
    error() << "No info on spherical mirror reflectivity at " << sphMirrorReflLoc << endmsg;
    return StatusCode::FAILURE;
  }
  else
  {
    _ri_debug << "Loaded spherical mirror reflectivity from: " << sphMirrorReflLoc << endmsg;
    m_nominalSphMirrorRefl.reset( new Rich::TabulatedProperty1D( sphMirrorRefl ) );
    if ( !m_nominalSphMirrorRefl->valid() )
    {
      error() << "Invalid Rich::TabulatedProperty1D for " << sphMirrorRefl->name() << endmsg;
      return StatusCode::FAILURE;
    }
  }

  // get the nominal reflectivity of the secondary mirror
  const std::string secMirrorReflLoc =
    param< std::string >( "NominalSecondaryMirrorReflectivityLoc" );

  SmartDataPtr< TabulatedProperty > secMirrorRefl( dataSvc(), secMirrorReflLoc );
  if ( !secMirrorRefl )
  {
    error() << "No info on secondary mirror reflectivity at " << secMirrorReflLoc << endmsg;
    return StatusCode::FAILURE;
  }
  else
  {
    _ri_debug << "Loaded secondary mirror reflectivity from: " << secMirrorReflLoc << endmsg;
    m_nominalSecMirrorRefl.reset( new Rich::TabulatedProperty1D( secMirrorRefl ) );
    if ( !m_nominalSecMirrorRefl->valid() )
    {
      error() << "Invalid Rich::TabulatedProperty1D for " << secMirrorRefl->name() << endmsg;
      return StatusCode::FAILURE;
    }
  }

  // initialize RichRadiators
  std::vector< std::string > radLocs = paramVect< std::string >( "RadiatorDetElemLocations" );
  for ( unsigned int rad = 0; rad < radLocs.size(); rad++ )
  {
    SmartDataPtr< DeRichRadiator > richRad( dataSvc(), radLocs[rad] );
    if ( !richRad ) error() << "Cannot initialize Rich Radiator:" << radLocs[rad] << endmsg;
  }

  // initialise various local cached data
  loadPDPanels();
  loadNominalQuantumEff();

  return StatusCode::SUCCESS;
}

//=========================================================================
// Access the name for a given panel
//=========================================================================
const std::string
DeHorizRich::panelName( const Rich::Side panel ) const
{
  const auto &panelLoc = paramVect< std::string >( "PDPanelDetElemLocations" );
  return panelLoc[panel];
}
