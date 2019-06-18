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

// STL
#include <sstream>

// Gaudi
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/PhysicalConstants.h"
#include "GaudiKernel/SmartDataPtr.h"

// DetDesc
#include "DetDesc/Condition.h"
#include "DetDesc/SolidBox.h"

// local
#include "RichDet/DeRich.h"
#include "RichDet/DeRichPMT.h"
#include "RichDet/DeRichSystem.h"

// RichUtils
#include "RichUtils/FastMaths.h"

//-----------------------------------------------------------------------------
// Implementation file for class : DeRichPMT
//
// 2011-10-11 : Sajan Easo
//-----------------------------------------------------------------------------
const CLID CLID_DeRichPMT = 12025; // User defined

//=============================================================================

const CLID& DeRichPMT::classID() { return CLID_DeRichPMT; }

//=============================================================================

DeRichPMT::PMTData DeRichPMT::m_pddata;

//=============================================================================

StatusCode DeRichPMT::initialize() {
  MsgStream msg( msgSvc(), "DeRichPMT" );

  // store the name of the PMT, without the /dd/Structure part
  const auto pos = name().find( "MAPMT:" );
  setMyName( std::string::npos != pos ? name().substr( pos ) : "DeRichPMT_NO_NAME" );
  if ( msgLevel( MSG::DEBUG, msg ) ) msg << MSG::DEBUG << "Initialize " << myName() << endmsg;

  // extract PMT overall module and copy number from detector element name
  const auto pos2 = name().find( ":" );
  if ( std::string::npos == pos2 ) {
    msg << MSG::FATAL << "A PMT without a module number" << endmsg;
    return StatusCode::FAILURE;
  }
  const auto s1   = name().substr( pos2 + 1 );
  const auto pos3 = s1.find( "/MAPMT:" );
  if ( std::string::npos == pos3 ) {
    msg << MSG::FATAL << "A PMT without a copy number" << endmsg;
    return StatusCode::FAILURE;
  }

  // Common DePD init
  auto sc = DeRichPD::initialize();
  if ( !sc ) return sc;

  m_dePmtAnode = ( !childIDetectorElements().empty() ? childIDetectorElements().front() : nullptr );
  if ( !m_dePmtAnode ) {
    fatal() << "DePMT : Cannot find PmtAnode detector element" << endmsg;
    return StatusCode::FAILURE;
  }

  // register for updates when geometry changes
  updMgrSvc()->registerCondition( this, geometry(), &DeRichPMT::updateGeometry );
  updMgrSvc()->registerCondition( this, m_dePmtAnode->geometry(), &DeRichPMT::updateGeometry );
  // Update PMT QE values whenever DeRichSystem updates
  updMgrSvc()->registerCondition( this, deRichSys(), &DeRichPMT::initPMTQuantumEff );

  // Trigger first update
  sc = updMgrSvc()->update( this );
  if ( !sc ) { fatal() << "UMS updates failed" << endmsg; }

  return sc;
}

//=============================================================================

DetectorElement* DeRichPMT::getFirstRich() {
  DetectorElement*              de{nullptr};
  SmartDataPtr<DetectorElement> afterMag( dataSvc(), "/dd/Structure/LHCb/AfterMagnetRegion" );
  if ( !afterMag ) {
    error() << "Could not load AfterMagnetRegion det elem" << endmsg;
  } else {
    const auto firstRichLoc =
        ( afterMag->exists( "RichDetectorLocations" ) ? afterMag->paramVect<std::string>( "RichDetectorLocations" )[0]
                                                      : DeRichLocations::Rich1 );
    SmartDataPtr<DetectorElement> deRich( dataSvc(), firstRichLoc );
    if ( deRich ) { de = deRich; }
  }
  if ( !de ) { error() << "Could not load DeRich for DeRichPMT" << endmsg; }
  return de;
}

//=============================================================================

void DeRichPMT::PMTData::initialise( const DetectorElement* deRich ) {

  using namespace LHCb::SIMD;

  // check if already initialised
  if ( !deRich || m_initialised ) return;

  // lock update, just in case
  std::lock_guard lock( m_initLock );

  m_initialised = true;

  const auto PmtAnodeZSize         = deRich->param<double>( "RichPmtAnodeZSize" );
  const auto PmtAnodeLocationInPmt = deRich->param<double>( "RichPmtSiliconDetectorLocalZlocation" );
  const auto PmtPixelXSize         = deRich->param<double>( "RichPmtPixelXSize" );
  const auto PmtPixelYSize         = deRich->param<double>( "RichPmtPixelYSize" );
  const auto PmtPixelGap           = deRich->param<double>( "RichPmtPixelGap" );
  PmtEffectivePixelXSize           = SIMDFP( PmtPixelXSize + PmtPixelGap );
  PmtEffectivePixelYSize           = SIMDFP( PmtPixelYSize + PmtPixelGap );
  PmtAnodeHalfThickness            = SIMDFP( PmtAnodeZSize * 0.5f );
  PmtNumPixCol                     = SIMDUINT( deRich->param<int>( "RichPmtNumPixelCol" ) );
  PmtNumPixRow                     = SIMDUINT( deRich->param<int>( "RichPmtNumPixelRow" ) );
  PmtNumPixColFrac                 = simd_cast<SIMDFP>( PmtNumPixCol - SIMDUINT::One() ) * SIMDFP( 0.5f );
  PmtNumPixRowFrac                 = simd_cast<SIMDFP>( PmtNumPixRow - SIMDUINT::One() ) * SIMDFP( 0.5f );

  PmtQwZSize                = SIMDFP( deRich->param<double>( "RichPmtQuartzZSize" ) );
  const auto QwToAnodeZDist = deRich->param<double>( "RichPmtQWToSiMaxDist" );

  zShift = SIMDFP( QwToAnodeZDist + PmtAnodeLocationInPmt );

  if ( deRich->exists( "Rich2PMTArrayConfig" ) && //
       deRich->exists( "RichGrandPmtAnodeXSize" ) ) {
    const auto GrandPmtAnodeZSize = deRich->param<double>( "RichGrandPmtAnodeZSize" );
    const auto GrandPmtPixelXSize = deRich->param<double>( "RichGrandPmtPixelXSize" );
    const auto GrandPmtPixelYSize = deRich->param<double>( "RichGrandPmtPixelYSize" );
    const auto GrandPmtPixelGap   = deRich->param<double>( "RichGrandPmtPixelGap" );
    GrandPmtEdgePixelXSize        = SIMDFP( deRich->param<double>( "RichGrandPmtEdgePixelXSize" ) );
    GrandPmtEdgePixelYSize        = SIMDFP( deRich->param<double>( "RichGrandPmtEdgePixelYSize" ) );
    GrandPmtEffectivePixelXSize   = SIMDFP( GrandPmtPixelXSize + GrandPmtPixelGap );
    GrandPmtEffectivePixelYSize   = SIMDFP( GrandPmtPixelYSize + GrandPmtPixelGap );
    GrandPmtAnodeHalfThickness    = SIMDFP( GrandPmtAnodeZSize * 0.5f );
  }
}

//=============================================================================

StatusCode DeRichPMT::getPMTParameters() {

  const auto deRich = getFirstRich();
  if ( !deRich ) { return StatusCode::FAILURE; }

  {
    const std::string effnumPixCond = "RichPmtTotNumPixel";
    const auto        cExists       = deRich->exists( effnumPixCond );
    m_effNumActivePixs              = ( cExists ? (FP)deRich->param<int>( effnumPixCond ) : 64.0 );
    _ri_debug << "EffNumPixs = " << cExists << " | " << m_effNumActivePixs << endmsg;
  }

  // shared PD data
  m_pddata.initialise( deRich );

  // Default initialise some DePD base parameters
  setPmtIsGrandFlag( PmtIsGrand() );

  // cache to local matrix
  m_toLocM = geometry()->toLocalMatrix();

  // Cache SIMD info

  // transform does not like direct assignent from scalar version :(
  {
    double xx{0}, xy{0}, xz{0}, dx{0}, yx{0}, yy{0};
    double yz{0}, dy{0}, zx{0}, zy{0}, zz{0}, dz{0};
    // and to global
    using FP = Rich::SIMD::DefaultScalarFP;
    toGlobalMatrix().GetComponents( xx, xy, xz, dx, yx, yy, yz, dy, zx, zy, zz, dz );
    m_toGlobalMatrixSIMD.SetComponents( (FP)xx, (FP)xy, (FP)xz, (FP)dx, //
                                        (FP)yx, (FP)yy, (FP)yz, (FP)dy, //
                                        (FP)zx, (FP)zy, (FP)zz, (FP)dz );
  }

  return StatusCode::SUCCESS;
}

//=============================================================================

void DeRichPMT::setPmtIsGrandFlag( const bool isGrand ) {
  m_PmtIsGrand = isGrand;
  // Update cached size parameters
  const auto* deRich = getFirstRich();
  if ( deRich ) {
    if ( isGrand ) {
      const auto GrandPmtPixelXSize = deRich->param<double>( "RichGrandPmtPixelXSize" );
      const auto GrandPmtPixelYSize = deRich->param<double>( "RichGrandPmtPixelYSize" );
      m_pixelArea                   = GrandPmtPixelXSize * GrandPmtPixelYSize;
    } else {
      const auto PmtPixelXSize = deRich->param<double>( "RichPmtPixelXSize" );
      const auto PmtPixelYSize = deRich->param<double>( "RichPmtPixelYSize" );
      m_pixelArea              = PmtPixelXSize * PmtPixelYSize;
    }
    m_effPixelArea = m_pixelArea; // PMTs have no demagnification
  } else {
    error() << "Could not load First Rich" << endmsg;
  }
  if ( PmtIsGrand() && rich() == Rich::Rich1 ) { warning() << "Setting RICH1 PMT 'Grand'" << endmsg; }
}

//=============================================================================

StatusCode DeRichPMT::initPMTQuantumEff() {
  const auto* deRich = getFirstRich();
  if ( !deRich ) { return StatusCode::FAILURE; }
  const auto                      PmtQELocation = deRich->param<std::string>( "RichPmtQETableName" );
  SmartDataPtr<TabulatedProperty> pmtQuantumEffTabProp( dataSvc(), PmtQELocation );
  m_pdQuantumEffFunc = std::make_shared<Rich::TabulatedProperty1D>( pmtQuantumEffTabProp );
  return StatusCode::SUCCESS;
}

//=============================================================================

StatusCode DeRichPMT::updateGeometry() { return getPMTParameters(); }

//=============================================================================

bool DeRichPMT::detectionPoint( const LHCb::RichSmartID smartID,         //
                                Gaudi::XYZPoint&        detectPoint,     //
                                bool                    photoCathodeSide //
                                ) const {

  auto aLocalHit = getAnodeHitCoordFromMultTypePixelNum( smartID.pixelCol(), smartID.pixelRow() );
  aLocalHit.SetZ( aLocalHit.Z() + scalar( m_pddata.zShift ) );

  // for now assume negligible refraction effect at the QW.

  if ( !photoCathodeSide ) { aLocalHit.SetZ( scalar( m_pddata.PmtQwZSize ) + aLocalHit.Z() ); }

  detectPoint = ( toGlobalMatrix() * aLocalHit );

  return true;
}

//===============================================================================================

DeRichPD::SIMDFP::mask_type DeRichPMT::detectionPoint( const SmartIDs& smartID,         //
                                                       SIMDPoint&      detectPoint,     //
                                                       bool            photoCathodeSide //
                                                       ) const {

  // return status
  SIMDFP::mask_type ok( true );

  // // Just use a scalar loop here...
  // SIMDFP X(0), Y(0), Z(0);
  // for ( std::size_t i = 0; i < SIMDFP::Size; ++i )
  // {
  //   Gaudi::XYZPoint p{0,0,0};
  //   ok[i] = detectionPoint( smartID[i], p, photoCathodeSide );
  //   if ( ok[i] )
  //   {
  //     X[i] = p.X();
  //     Y[i] = p.Y();
  //     Z[i] = p.Z();
  //   }
  // }
  // detectPoint = { X, Y, Z };

  // SIMD version

  // Extract the row, col numbers
  SIMDUINT row, col;
  for ( std::size_t i = 0; i < SIMDFP::Size; ++i ) {
    row[i] = smartID[i].pixelRow();
    col[i] = smartID[i].pixelCol();
  }

  // make local hit
  auto aLocalHit = getAnodeHitCoordFromMultTypePixelNum( col, row );
  aLocalHit.SetZ( m_pddata.zShift + aLocalHit.Z() );

  // for now assume negligible refraction effect at the QW.

  if ( !photoCathodeSide ) { aLocalHit.SetZ( m_pddata.PmtQwZSize + aLocalHit.Z() ); }

  detectPoint = m_toGlobalMatrixSIMD * aLocalHit;

  return ok;
}

//===============================================================================================

Gaudi::XYZPoint DeRichPMT::detPointOnAnode( const LHCb::RichSmartID& smartID ) const {
  return ( m_dePmtAnode->geometry()->toGlobal(
      getAnodeHitCoordFromMultTypePixelNum( smartID.pixelCol(), smartID.pixelRow() ) ) );
}

//=============================================================================
