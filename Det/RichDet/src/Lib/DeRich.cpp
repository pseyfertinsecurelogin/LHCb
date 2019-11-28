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
/** @file DeRich.cpp
 *
 *  Implementation file for detector description class : DeRich
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */

#include <sstream>

// Gaudi
#include "GaudiKernel/SmartDataPtr.h"

// DetDesc
#include "DetDesc/AlignmentCondition.h"

// local
#include "RichDet/DeRich.h"
#include "RichDet/DeRichHPDPanel.h"
#include "RichDet/DeRichPDPanel.h"
#include "RichDet/DeRichPMTPanel.h"
#include "RichDet/DeRichPMTPanelClassic.h"
#include "RichDet/DeRichSphMirror.h"

//-----------------------------------------------------------------------------
// Implementation file for class : DeRich
//
// 2004-06-30 : Antonis Papanestis
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
DeRich::DeRich( const std::string& name ) : DeRichBase( name ) {}

//=========================================================================
//  initialize
//=========================================================================
StatusCode DeRich::initialize() {
  _ri_debug << "Initialize " << name() << endmsg;

  if ( exists( "SphMirrorSegRows" ) ) {
    m_sphMirrorSegRows = param<int>( "SphMirrorSegRows" );
    m_sphMirrorSegCols = param<int>( "SphMirrorSegColumns" );
    m_positionInfo     = true;
  }

  if ( exists( "SecMirrorSegRows" ) ) {
    m_secMirrorSegRows = param<int>( "SecMirrorSegRows" );
    m_secMirrorSegCols = param<int>( "SecMirrorSegColumns" );
  } else if ( exists( "FlatMirrorSegRows" ) ) {
    m_secMirrorSegRows = param<int>( "FlatMirrorSegRows" );
    m_secMirrorSegCols = param<int>( "FlatMirrorSegColumns" );
  }

  if ( exists( "RichPhotoDetectorConfig" ) ) {
    m_RichPhotoDetConfig = (Rich::RichPhDetConfigType)param<int>( "RichPhotoDetectorConfig" );
  } else {
    // assume HPDs
    m_RichPhotoDetConfig = Rich::HPDConfig;
  }

  if ( exists( "RichGeometryRunConfiguration" ) ) {
    m_RichGeometryConfig = param<int>( "RichGeometryRunConfiguration" );
  } else {
    // assume current RICH configuration
    m_RichGeometryConfig = 0;
  }

  m_Rich2PhotoDetectorArrayConfig = 0;
  m_Rich2UseGrandPmt              = false;
  m_Rich2UseMixedPmt              = false;

  if ( exists( "Rich2PMTArrayConfig" ) ) {
    m_Rich2PhotoDetectorArrayConfig = param<int>( "Rich2PMTArrayConfig" );
    if ( m_Rich2PhotoDetectorArrayConfig >= 1 ) {
      m_Rich2UseGrandPmt = true;
      if ( m_Rich2PhotoDetectorArrayConfig == 2 || m_Rich2PhotoDetectorArrayConfig == 3 ) { m_Rich2UseMixedPmt = true; }
    }
  }

  // beampipe
  const auto loc = ( rich() == Rich::Rich1 ? DeRichLocations::Rich1BeamPipe : DeRichLocations::Rich2BeamPipe );
  SmartDataPtr<DeRichBeamPipe> pipe( dataSvc(), loc );
  m_beampipe = pipe;

  return StatusCode::SUCCESS;
}

//=========================================================================

void DeRich::loadNominalQuantumEff() {
  if ( m_RichPhotoDetConfig == Rich::HPDConfig ) {
    m_nominalPDQuantumEff = loadNominalHPDQuantumEff();
  } else if ( m_RichPhotoDetConfig == Rich::PMTConfig ) {
    m_nominalPDQuantumEff = loadNominalPMTQuantumEff();
  }
}

//=========================================================================

std::shared_ptr<const Rich::TabulatedProperty1D> DeRich::loadNominalHPDQuantumEff() {
  std::shared_ptr<const Rich::TabulatedProperty1D> nominalHPDQuantumEff;

  // find the HPD quantum efficiency
  std::string HPD_QETabPropLoc;
  if ( exists( "RichHpdQETableName" ) ) {
    HPD_QETabPropLoc = param<std::string>( "RichHpdQETableName" );
  } else {
    if ( exists( "RichNominalHpdQuantumEffTableName" ) ) {
      HPD_QETabPropLoc = param<std::string>( "RichNominalHpdQuantumEffTableName" );
    } else {
      throw GaudiException( "Cannot find HPD QE location RichNominalHpdQuantumEffTableName", "DeRich",
                            StatusCode::FAILURE );
    }
  }

  SmartDataPtr<TabulatedProperty> tabQE( dataSvc(), HPD_QETabPropLoc );
  if ( !tabQE ) {
    throw GaudiException( "No information on HPD Quantum Efficiency", "DeRich", StatusCode::FAILURE );
  } else {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Loaded HPD QE from: " << HPD_QETabPropLoc << endmsg;
    nominalHPDQuantumEff = std::make_shared<Rich::TabulatedProperty1D>( tabQE );
    if ( !nominalHPDQuantumEff->valid() ) {
      throw GaudiException( "Invalid HPD QE RichTabulatedProperty1D for " + tabQE->name(), "DeRich",
                            StatusCode::FAILURE );
    }
  }

  return nominalHPDQuantumEff;
}

//=========================================================================

std::shared_ptr<const Rich::TabulatedProperty1D> DeRich::loadNominalPMTQuantumEff() {
  std::shared_ptr<const Rich::TabulatedProperty1D> nominalPMTQuantumEff;

  // find the PMT quantum efficiency
  std::string PMT_QETabPropLoc;
  if ( exists( "RichPmtQETableName" ) ) {
    PMT_QETabPropLoc = param<std::string>( "RichPmtQETableName" );
  } else {
    if ( exists( "RichNominalPmtQuantumEffTableName" ) ) {
      PMT_QETabPropLoc = param<std::string>( "RichNominalPmtQuantumEffTableName" );
    } else {
      throw GaudiException( "Cannot find PMT QE location RichNominalPmtQuantumEffTableName", "DeRich",
                            StatusCode::FAILURE );
    }
  }

  SmartDataPtr<TabulatedProperty> tabQE( dataSvc(), PMT_QETabPropLoc );
  if ( !tabQE ) {
    throw GaudiException( "No information on PMT Quantum Efficiency", "DeRich", StatusCode::FAILURE );
  } else {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Loaded PMT QE from: " << PMT_QETabPropLoc << endmsg;
    nominalPMTQuantumEff = std::make_shared<Rich::TabulatedProperty1D>( tabQE );
    if ( !nominalPMTQuantumEff->valid() ) {
      throw GaudiException( "Invalid PMT QE RichTabulatedProperty1D for " + tabQE->name(), "DeRich",
                            StatusCode::FAILURE );
    }
  }

  return nominalPMTQuantumEff;
}

//=========================================================================

Rich::MirrorSegPosition DeRich::sphMirrorSegPos( const int mirrorNumber ) const {

  Rich::MirrorSegPosition mirrorPos;

  if ( m_positionInfo ) {
    int row = mirrorNumber / m_sphMirrorSegCols;
    if ( row >= m_sphMirrorSegRows ) row -= m_sphMirrorSegRows;
    mirrorPos.setRow( row );
    mirrorPos.setColumn( mirrorNumber % m_sphMirrorSegCols );
  } else {
    error() << "No position information for mirrors" << endmsg;
  }

  return mirrorPos;
}

//=========================================================================

Rich::MirrorSegPosition DeRich::secMirrorSegPos( const int mirrorNumber ) const {

  Rich::MirrorSegPosition mirrorPos;

  if ( m_positionInfo ) {
    int row = mirrorNumber / m_secMirrorSegCols;
    if ( row >= m_secMirrorSegRows ) row -= m_secMirrorSegRows;
    mirrorPos.setRow( row );
    mirrorPos.setColumn( mirrorNumber % m_secMirrorSegCols );
  } else {
    error() << "No position information for mirrors" << endmsg;
  }

  return mirrorPos;
}

//=========================================================================
// Return a RichSmartID for a particular point in the LHCb coord system
//=========================================================================
int DeRich::sensitiveVolumeID( const Gaudi::XYZPoint& globalPoint ) const {
  return ( pdPanel( side( globalPoint ) )->sensitiveVolumeID( globalPoint ) );
}

//=========================================================================
// Access the name for a given panel
//=========================================================================
const std::string DeRich::panelName( const Rich::Side /* panel */ ) const {
  throw GaudiException( "Not implemented for DeRich class" + myName(), "DeRich::panelName", StatusCode::FAILURE );
}

//=============================================================================
// Load the PD panels
//=============================================================================
void DeRich::loadPDPanels() {
  for ( const auto panel : Rich::sides() ) {
    const std::string pName = panelName( panel );

    DeRichPDPanel* phdePanel = nullptr;

    if ( RichPhotoDetConfig() == Rich::HPDConfig ) {
      SmartDataPtr<DeRichHPDPanel> aHpdPanel( dataSvc(), pName );
      phdePanel = aHpdPanel;
    } else if ( RichPhotoDetConfig() == Rich::PMTConfig ) {
      if ( m_Rich2PhotoDetectorArrayConfig != 3 ) {
        SmartDataPtr<DeRichPMTPanelClassic> aPmtPanel( dataSvc(), pName );
        phdePanel = aPmtPanel;
      } else {
        SmartDataPtr<DeRichPMTPanel> aPmtPanel( dataSvc(), pName );
        phdePanel = aPmtPanel;
      }
    }

    if ( !phdePanel ) {
      std::ostringstream mess;
      mess << "Failed to load DeRichPDPanel at " << pName;
      throw GaudiException( mess.str(), "DeRich::PhDetPanel", StatusCode::FAILURE );
    }

    m_PDPanels[panel] = phdePanel;
  }
}

//=============================================================================
// Cache various SIMD type
//=============================================================================

void DeRich::fillSIMDTypes() {
  // this loop also does RICH1 Up and down as enums have same numerical value
  for ( const Rich::Side side : {Rich::firstSide, Rich::secondSide} ) {
    const auto& p = m_nominalPlanes[side];
    m_nominalPlanesSIMD[side] =
        Rich::SIMD::Plane<Rich::SIMD::DefaultScalarFP>( (FP)p.A(), (FP)p.B(), (FP)p.C(), (FP)p.D() );
    m_nominalNormalsSIMD[side]            = m_nominalNormals[side];
    m_nominalCentresOfCurvatureSIMD[side] = m_nominalCentresOfCurvature[side];
    m_sphMirrorRadiusSIMD                 = m_sphMirrorRadius;
  }
}

//=============================================================================
// Ray trace a given direction with the given PD panel (scalar)
//=============================================================================

LHCb::RichTraceMode::RayTraceResult DeRich::rayTrace( const Rich::Side          side,        //
                                                      const Gaudi::XYZPoint&    pGlobal,     //
                                                      const Gaudi::XYZVector&   vGlobal,     //
                                                      Gaudi::XYZPoint&          hitPosition, //
                                                      LHCb::RichSmartID&        smartID,     //
                                                      const DeRichPD*&          dePD,        //
                                                      const LHCb::RichTraceMode mode ) const {
  // are we configured to test individual PD acceptance or just interset the plane ?
  return ( mode.detPlaneBound() == LHCb::RichTraceMode::DetectorPlaneBoundary::RespectPDTubes ?
                                                                                              // Full PD acceptance
               pdPanel( side )->PDWindowPoint( pGlobal, vGlobal, hitPosition, smartID, dePD, mode )
                                                                                              :
                                                                                              // just the plane
               pdPanel( side )->detPlanePoint( pGlobal, vGlobal, hitPosition, smartID, dePD, mode ) );
}

//=============================================================================
// Ray trace a given direction with the given PD panel (SIMD)
//=============================================================================

DeRich::SIMDRayTResult::Results DeRich::rayTrace( const Rich::Side              side,        //
                                                  const Rich::SIMD::Point<FP>&  pGlobal,     //
                                                  const Rich::SIMD::Vector<FP>& vGlobal,     //
                                                  Rich::SIMD::Point<FP>&        hitPosition, //
                                                  SIMDRayTResult::SmartIDs&     smartID,     //
                                                  SIMDRayTResult::PDs&          PDs,         //
                                                  const LHCb::RichTraceMode     mode ) const {
  // are we configured to test individual PD acceptance or just interset the plane ?
  return ( mode.detPlaneBound() == LHCb::RichTraceMode::DetectorPlaneBoundary::RespectPDTubes ?
                                                                                              // Full PD acceptance
               pdPanel( side )->PDWindowPointSIMD( pGlobal, vGlobal, hitPosition, smartID, PDs, mode )
                                                                                              :
                                                                                              // just the plane
               pdPanel( side )->detPlanePointSIMD( pGlobal, vGlobal, hitPosition, smartID, PDs, mode ) );
}

//=============================================================================
// Ray trace a given direction with the correct PD panel (SIMD)
//=============================================================================
DeRich::SIMDRayTResult::Results DeRich::rayTrace( const Rich::SIMD::Sides&      sides,       //
                                                  const Rich::SIMD::Point<FP>&  pGlobal,     //
                                                  const Rich::SIMD::Vector<FP>& vGlobal,     //
                                                  Rich::SIMD::Point<FP>&        hitPosition, //
                                                  SIMDRayTResult::SmartIDs&     smartID,     //
                                                  SIMDRayTResult::PDs&          PDs,         //
                                                  const LHCb::RichTraceMode     mode ) const {
  using namespace LHCb::SIMD;

  // If all sides are the same, shortcut to a single call
  // hopefully the most common situation ...

  // side 1 mask
  const auto m1 = ( sides == Rich::SIMD::Sides( (int)Rich::firstSide ) );
  if ( all_of( m1 ) ) { return rayTrace( Rich::firstSide, pGlobal, vGlobal, hitPosition, smartID, PDs, mode ); }

  // side 2 mask
  const auto m2 = ( sides == Rich::SIMD::Sides( (int)Rich::secondSide ) );
  if ( all_of( m2 ) ) { return rayTrace( Rich::secondSide, pGlobal, vGlobal, hitPosition, smartID, PDs, mode ); }

  // we have a mixture... So must run both and merge..
  // Is there a better way to handle this ... ?

  // copy input objects for second call
  auto hitPosition2 = hitPosition;
  auto smartID2     = smartID;
  auto PDs2         = PDs;

  // call for the first side
  auto res1 = rayTrace( Rich::firstSide, pGlobal, vGlobal, hitPosition, smartID, PDs, mode );
  // call for the second side
  auto res2 = rayTrace( Rich::secondSide, pGlobal, vGlobal, hitPosition2, smartID2, PDs2, mode );

  // merge results2 into the returned results

  const auto fm2 = LHCb::SIMD::simd_cast<SIMDFP::mask_type>( m2 );
  SIMDFP     hx( hitPosition.x() ), hy( hitPosition.y() ), hz( hitPosition.z() );
  hx( fm2 )   = hitPosition2.x();
  hy( fm2 )   = hitPosition2.y();
  hz( fm2 )   = hitPosition2.z();
  hitPosition = {hx, hy, hz};

  // copy m2 values from res2 to res1
  res1( m2 ) = res2;

  // scalar loop for non-Vc types
  for ( std::size_t i = 0; i < SIMDFP::Size; ++i ) {
    if ( m2[i] ) {
      // res1[i]    = res2[i];
      smartID[i] = smartID2[i];
      PDs[i]     = PDs2[i];
    }
  }

  // return
  return res1;
}

//=============================================================================
// creates a vector of all mirrors of given type
//=============================================================================
std::vector<const DeRichSphMirror*> DeRich::getMirrors( std::string mirrType ) const {

  // list of mirror locations for given type
  const auto locations = paramVect<std::string>( mirrType );

  // vector of mirror pointers to return
  std::vector<const DeRichSphMirror*> mirrors;
  mirrors.reserve( locations.size() );

  // load the mirrors
  for ( const auto& loc : locations ) {
    SmartDataPtr<DeRichSphMirror> mirror( dataSvc(), loc );
    mirrors.push_back( mirror );
  }

  // return the vector
  return mirrors;
}
