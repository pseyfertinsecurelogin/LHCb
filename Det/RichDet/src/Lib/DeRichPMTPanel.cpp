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
#include <vector>

// Gaudi
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/PhysicalConstants.h"
#include "GaudiKernel/SmartDataPtr.h"

// local
#include "RichDet/DeRichPMTPanel.h"
#include "RichDet/DeRichSystem.h"

// MathCore files
#include "GaudiKernel/Transform3DTypes.h"

// DetDesc
#include "DetDesc/SolidBox.h"
#include "DetDesc/SolidSphere.h"
#include "DetDesc/SolidTubs.h"
#include "DetDesc/TabulatedProperty.h"

// GSL
#include "gsl/gsl_math.h"

//-----------------------------------------------------------------------------

const CLID CLID_DeRichPMTPanel = 12021; // User defined

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
DeRichPMTPanel::DeRichPMTPanel( const std::string& name ) //
    : DeRichPDPanel( name ), m_ModuleIsWithGrandPMT( 350 ) {
  // Set the PD type to PMT
  m_pdType = LHCb::RichSmartID::MaPMTID;
}

//=============================================================================
// Retrieve Pointer to class defininition structure
const CLID& DeRichPMTPanel::classID() { return CLID_DeRichPMTPanel; }

//=========================================================================
//  Initialize
//=========================================================================
StatusCode DeRichPMTPanel::initialize() {
  // store the name of the panel, without the /dd/Structure part
  const auto pos = name().find( "Rich" );
  setMyName( std::string::npos != pos ? name().substr( pos ) : "DeRichPMTPanel_NO_NAME" );

  debug() << "Initialize " << name() << endmsg;

  // set rich and side
  if ( !setRichAndSide() ) return StatusCode::FAILURE;

  // register UMS dependency on local geometry
  updMgrSvc()->registerCondition( this, geometry(), &DeRichPMTPanel::geometryUpdate );

  // trigger first UMS update
  return updMgrSvc()->update( this );
}

//=========================================================================
//  getFirstDeRich
//=========================================================================
const DetectorElement* DeRichPMTPanel::getFirstDeRich() const {
  SmartDataPtr<DetectorElement> afterMag( dataSvc(), "/dd/Structure/LHCb/AfterMagnetRegion" );
  if ( !afterMag ) { error() << "Could not load AfterMagnetRegion det elem" << endmsg; }

  const std::string firstRichLoc =
      ( afterMag->exists( "RichDetectorLocations" ) ? afterMag->paramVect<std::string>( "RichDetectorLocations" )[0]
                                                    : DeRichLocations::Rich1 );

  SmartDataPtr<DetectorElement> deRich( dataSvc(), firstRichLoc );
  if ( !deRich ) { error() << "Could not load DeRich for DeRichPMTPanel" << endmsg; }

  return deRich;
}

//=========================================================================
// generate the transforms for global <-> local frames
//=========================================================================
StatusCode DeRichPMTPanel::geometryUpdate() {
  using GP = Gaudi::XYZPoint;

  MsgStream msg( msgSvc(), "DeRichPMTPanel" );

  auto* firstRich = getFirstDeRich();
  if ( !firstRich ) return StatusCode::FAILURE;

  // geom update
  StatusCode sc = getPanelGeometryInfo();

  // get the pmtmodule and pmt detector elements
  m_DePMTs.clear();
  m_DePMTModules.clear();
  m_DePMTAnodes.clear();
  m_DePMTModulesZeroPtn.clear();
  m_DePMTAnodesZeroPtn.clear();

  const auto numCurModules = getNumModulesInThisPanel();

  m_DePMTModules.reserve( numCurModules );
  m_DePMTs.reserve( numCurModules );
  m_DePMTAnodes.reserve( numCurModules );
  m_DePMTModulesZeroPtn.reserve( numCurModules );
  m_DePMTAnodesZeroPtn.reserve( numCurModules );

  // reset copy number to PMT vector
  // m_copyNumToPMT.assign( m_totNumPMTs, nullptr );

  const auto& detelems = childIDetectorElements();

  for ( auto det_it = detelems.begin(); det_it != detelems.end(); ++det_it ) {
    if ( std::string::npos != ( *det_it )->name().find( "MAPMT_MODULE:" ) ) {

      // get PMT Module
      SmartDataPtr<IDetectorElement> dePMTModule( dataSvc(), ( *det_it )->name() );
      if ( !dePMTModule ) {
        msg << MSG::FATAL << "Non DeRichPMT Module detector element " << ( *det_it )->name() << endmsg;
        return StatusCode::FAILURE;
      }

      // save to list of PMT Modules
      m_DePMTModules.push_back( dePMTModule->geometry() );
      const auto aCurrentModuleCopyNumber = getModuleCopyNumber( dePMTModule->name() );

      // save module {0,0,0} in local panel coords
      {
        const auto zInGlobal = dePMTModule->geometry()->toGlobalMatrix() * GP{0, 0, 0};
        // Move to panel local and save
        m_DePMTModulesZeroPtn.emplace_back( geometry()->toLocalMatrix() * zInGlobal );
      }

      //(max) number of PMTs in a module
      const auto aNumPmtInCurrentRichModule =
          ModuleIsWithGrandPMT( aCurrentModuleCopyNumber ) ? m_NumPmtInRichGrandModule : m_NumPmtInRichModule;
      std::vector<DeRichPMT*>           DePmtsInCurModule( aNumPmtInCurrentRichModule, nullptr );
      std::vector<const IGeometryInfo*> DePmtAnodesInCurModule( aNumPmtInCurrentRichModule, nullptr );
      std::vector<Gaudi::XYZPoint>      DePmtAnodeZeroPsInCurModule( aNumPmtInCurrentRichModule, {0, 0, 0} );

      // register UMS dependency.
      updMgrSvc()->registerCondition( this, dePMTModule->geometry(), &DeRichPMTPanel::geometryUpdate );

      // get PMT
      // for now assume the only det elements inside a module is just the pmts and
      // that inside the pmt is the anode. This may be changed in the future if needed with
      // some modif to the following lines.

      if ( !dePMTModule->childIDetectorElements().empty() ) {
        for ( auto det_it_pm = dePMTModule->childIDetectorElements().begin();
              det_it_pm != dePMTModule->childIDetectorElements().end(); ++det_it_pm ) {
          if ( std::string::npos != ( *det_it_pm )->name().find( "MAPMT:" ) ) {
            // get PMT
            // info() << (*det_it_pm)->name() << endmsg;
            SmartDataPtr<DeRichPMT> dePMT( dataSvc(), ( *det_it_pm )->name() );

            if ( dePMT ) {

              // register UMS dependency
              updMgrSvc()->registerCondition( this, dePMT->geometry(), &DeRichPMTPanel::geometryUpdate );

              // get the current pmt number in a module (from its name)
              const std::string s_pmt( "MAPMT:" );
              const auto        pmtNumberInAModule =
                  ( std::stoi(
                        ( *det_it_pm )->name().substr( ( *det_it_pm )->name().find( s_pmt ) + s_pmt.length() ) ) %
                    m_NumPmtInRichModule );

              // const auto curPmtCopyNum = dePMT->pmtCopyNumber();

              // CRJ - These should be set by the DePMT class itself....
              // dePMT->setPmtLensFlag   ( isCurrentPmtWithLens(curPmtCopyNum) );
              dePMT->setPmtIsGrandFlag( ModuleIsWithGrandPMT( aCurrentModuleCopyNumber ) );
              auto id = panelID();
              id.setPD_PMT( aCurrentModuleCopyNumber, pmtNumberInAModule );
              id.setLargePMT( ModuleIsWithGrandPMT( aCurrentModuleCopyNumber ) );
              dePMT->setPDSmartID( id );

              // pmtNumberInAModule is SmartID pdInCol
              // aCurrentModuleCopyNumber is pdCol

              // Set the position of PD's {0,0,0} in this panel's local frame
              {
                const auto zInGlobal = dePMT->toGlobalMatrix() * GP{0, 0, 0};
                // Move to panel local
                const auto zInLocal( geometry()->toLocalMatrix() * zInGlobal );
                // save
                dePMT->setZeroInPanelLocal( zInLocal );
              }

              DePmtsInCurModule[pmtNumberInAModule] = dePMT;

              if ( !dePMT->childIDetectorElements().empty() ) {
                for ( auto det_it_pm_an = dePMT->childIDetectorElements().begin();
                      det_it_pm_an != dePMT->childIDetectorElements().end(); ++det_it_pm_an ) {
                  if ( std::string::npos != ( *det_it_pm_an )->name().find( "MAPMTAnode:" ) ) {
                    SmartDataPtr<IDetectorElement> dePmtAnode( dataSvc(), ( *det_it_pm_an )->name() );

                    if ( dePmtAnode ) {
                      // register UMS dependency
                      updMgrSvc()->registerCondition( this, dePmtAnode->geometry(), &DeRichPMTPanel::geometryUpdate );
                      DePmtAnodesInCurModule[pmtNumberInAModule] = dePmtAnode->geometry();

                      // Set the position of anodes's {0,0,0} in this panel's local frame
                      {
                        const auto zInGlobal = dePmtAnode->geometry()->toGlobalMatrix() * GP{0, 0, 0};
                        const auto zInLocal( geometry()->toLocalMatrix() * zInGlobal );
                        DePmtAnodeZeroPsInCurModule[pmtNumberInAModule] = zInLocal;
                      }
                    } else {
                      msg << MSG::FATAL << "Non DeRichPMT Anode detector element " << ( *det_it_pm_an )->name()
                          << endmsg;
                      return StatusCode::FAILURE;
                    }
                  } // end test anode name

                }    // end loop over anodes in a pmt. We assume there is only 1 det elem inside a pmt
                     // which is the anode.
              }      // end test on pt child det elem
            } else { // end test existence of pmt
              msg << MSG::FATAL << "No DeRichPMT  detector element " << ( *det_it_pm )->name() << endmsg;
              return StatusCode::FAILURE;
            }

          } // end test pmt name

        } // end loop over pmts in a module

        // move here as we are done with them afterwards
        m_DePMTs.push_back( std::move( DePmtsInCurModule ) );
        m_DePMTAnodes.push_back( std::move( DePmtAnodesInCurModule ) );
        m_DePMTAnodesZeroPtn.push_back( std::move( DePmtAnodeZeroPsInCurModule ) );
      } else {
        msg << MSG::FATAL << "Problem getting PMT Det elem from " << ( *det_it )->name() << endmsg;
        return StatusCode::FAILURE;
      }
    } // end check on module

  } // end loop over modules

  // load basic info (required by PanelPD)
  // for PMTs, the meaning of the following parameters is different than in HPDs
  // m_PDColumns  : number of modules (in the panel)
  // m_PDNumInCol : (max) number of PMTs in a module
  // m_PDMax      : max PMT copy number (local - in panel)
  m_PDColumns  = getNumModulesInThisPanel();
  m_PDNumInCol = m_NumPmtInRichModule;
  m_PDMax      = m_PDColumns * m_PDNumInCol;

  double          aOffset = 0.0;
  Gaudi::XYZPoint aPon( 0, 0, 0 );
  Int             sign = 0;

  using GP = Gaudi::XYZPoint;

  if ( rich() == Rich::Rich1 ) {
    sign = ( side() == Rich::top ? 1 : -1 );

    // create the actual detection plane using the Z Shift wrt the current panel.
    const auto aZShiftR1 = firstRich->param<double>( "Rich1PmtDetPlaneZInPmtPanel" );
    m_detectionPlane =
        Gaudi::Plane3D( geometry()->toGlobal( GP( 0, 0, aZShiftR1 ) ), geometry()->toGlobal( GP( 0, 100, aZShiftR1 ) ),
                        geometry()->toGlobal( GP( 50, 50, aZShiftR1 ) ) );

    aPon = geometry()->toGlobal( GP( 0, 0, aZShiftR1 ) );

    m_xyHalfSizeSIMD[0] = fabs( m_PmtModulePlaneHalfSizeR1SIMD[0][0] );
    m_xyHalfSizeSIMD[1] = fabs( m_PmtModulePlaneHalfSizeR1SIMD[1][0] );
    aOffset             = fabs( m_PmtModulePlaneHalfSizeR1SIMD[1][0] );
  } else {

    // create the actual detection plane using the Z Shift wrt the current panel.
    const auto aZShiftR2 = firstRich->param<double>( "Rich2PmtDetPlaneZInPmtPanel" );
    m_detectionPlane =
        Gaudi::Plane3D( geometry()->toGlobal( GP( 0, 0, aZShiftR2 ) ), geometry()->toGlobal( GP( 0, 100, aZShiftR2 ) ),
                        geometry()->toGlobal( GP( 50, 50, aZShiftR2 ) ) );

    aPon = geometry()->toGlobal( GP( 0, 0, aZShiftR2 ) );

    // determine which panel
    const RowCol rci = ( side() == Rich::left ? RowCol{0, 1} : RowCol{2, 3} );

    sign = ( side() == Rich::left ? 1 : -1 );

    auto aOffsetX =
        ( !m_Rich2UseGrandModule ? fabs( m_PmtModulePlaneHalfSizeR2SIMD[rci[0]][0] )
                                 : !m_Rich2UseMixedModule ? fabs( m_GrandPmtModulePlaneHalfSizeR2SIMD[rci[0]][0] )
                                                          : fabs( m_MixedPmtModulePlaneHalfSizeR2SIMD[0][0] ) );
    auto aOffsetY =
        ( !m_Rich2UseGrandModule ? fabs( m_PmtModulePlaneHalfSizeR2SIMD[rci[1]][0] )
                                 : !m_Rich2UseMixedModule ? fabs( m_GrandPmtModulePlaneHalfSizeR2SIMD[rci[1]][0] )
                                                          : fabs( m_MixedPmtModulePlaneHalfSizeR2SIMD[1][0] ) );
    aOffset             = aOffsetX;
    m_xyHalfSizeSIMD[0] = aOffsetX;
    m_xyHalfSizeSIMD[1] = aOffsetY;
  }
  m_localOffset = aOffset;
  m_detPlaneZ   = geometry()->toLocal( aPon ).z();

  //  const ROOT::Math::Translation3D localTranslation =
  //  ROOT::Math::Translation3D(aPon.x(),sign*aOffset,aPon.z());
  const ROOT::Math::Translation3D localTranslation =
      ( rich() == Rich::Rich1 ? ROOT::Math::Translation3D( aPon.x(), sign * localOffset(), detectPlaneZcoord() )
                              : ROOT::Math::Translation3D( sign * localOffset(), aPon.y(), detectPlaneZcoord() ) );

  m_globalToPDPanelTransform = localTranslation * geometry()->toLocalMatrix();
  m_PDPanelToGlobalTransform = m_globalToPDPanelTransform.Inverse();

  // Define function pointers

  if ( Rich::Rich1 == rich() ) {
    m_getModuleNumsSIMD = ( side() == Rich::top ? &DeRichPMTPanel::getModuleNums_R1Up_NoLens_SIMD
                                                : &DeRichPMTPanel::getModuleNums_R1Dn_NoLens_SIMD );
  } else {
    if ( !m_Rich2UseGrandModule ) {
      m_getModuleNumsSIMD = ( side() == Rich::left ? &DeRichPMTPanel::getModuleNums_R2Le_Small_SIMD
                                                   : &DeRichPMTPanel::getModuleNums_R2Ri_Small_SIMD );
    } else {
      if ( !m_Rich2UseMixedModule ) {
        m_getModuleNumsSIMD = ( side() == Rich::left ? &DeRichPMTPanel::getModuleNums_R2Le_Grand_SIMD
                                                     : &DeRichPMTPanel::getModuleNums_R2Ri_Grand_SIMD );
      } else {
        // use same for both panels...
        m_getModuleNumsSIMD = &DeRichPMTPanel::getModuleNums_R2_Mixed_SIMD;
      }
    }
  }

  // Sanity checks

  // loop over DePDs and compare information
  for ( const auto& m : m_DePMTs ) {
    for ( const auto& pd : m ) {
      if ( pd ) {
        // smartID for this PD
        const auto pdID = pd->pdSmartID();
        // test method to get dePD from smartID
        const auto test_pd = dePMT( pdID );
        if ( UNLIKELY( test_pd != pd ) ) {
          error() << "Inconsistent results from dePMT(RichSmartID)" << endmsg;
          error() << "   -> requested " << pdID << endmsg;
          error() << "   -> retrieved " << test_pd->pdSmartID() << endmsg;
        }
      }
    }
  }

  // make SIMD caches for various quantities

  // m_Rich1LensDemagnificationFactorSIMD = m_Rich1LensDemagnificationFactor;
  m_GrandPmtAnodeXEdgeSIMD = m_GrandPmtAnodeXEdge;
  m_GrandPmtAnodeYEdgeSIMD = m_GrandPmtAnodeYEdge;
  m_PmtAnodeXEdgeSIMD      = m_PmtAnodeXEdge;
  m_PmtAnodeYEdgeSIMD      = m_PmtAnodeYEdge;

  m_detectionPlaneNormalSIMD = m_detectionPlane.Normal();

  for ( const auto i : {0, 1} ) {
    m_NumGrandPmtInRowColSIMD[i] = m_NumGrandPmtInRowCol[i];
    m_NumPmtInRowColSIMD[i]      = m_NumPmtInRowCol[i];
  }
  for ( const auto i : {0, 1, 2, 3} ) {
    m_RichPmtNumModulesInRowColSIMD[i]      = m_RichPmtNumModulesInRowCol[i];
    m_RichPmtModuleCopyNumBeginPanelSIMD[i] = m_RichPmtModuleCopyNumBeginPanel[i];
    m_RichPmtModuleCopyNumEndPanelSIMD[i]   = m_RichPmtModuleCopyNumEndPanel[i];
  }
  for ( const auto i : m_Rich2MixedModuleArrayColumnSize ) { m_Rich2MixedModuleArrayColumnSizeSIMD.emplace_back( i ); }

  // transform does not like direct assignment from scalar version :(
  {
    double xx{0}, xy{0}, xz{0}, dx{0}, yx{0}, yy{0};
    double yz{0}, dy{0}, zx{0}, zy{0}, zz{0}, dz{0};

    // Set to local matrix
    geometry()->toLocalMatrix().GetComponents( xx, xy, xz, dx, yx, yy, yz, dy, zx, zy, zz, dz );
    m_toLocalMatrixSIMD.SetComponents( (FP)xx, (FP)xy, (FP)xz, (FP)dx, (FP)yx, (FP)yy, (FP)yz, (FP)dy, (FP)zx, (FP)zy,
                                       (FP)zz, (FP)dz );
    // and to global
    geometry()->toGlobalMatrix().GetComponents( xx, xy, xz, dx, yx, yy, yz, dy, zx, zy, zz, dz );
    m_toGlobalMatrixSIMD.SetComponents( (FP)xx, (FP)xy, (FP)xz, (FP)dx, (FP)yx, (FP)yy, (FP)yz, (FP)dy, (FP)zx, (FP)zy,
                                        (FP)zz, (FP)dz );

    // panel
    m_globalToPDPanelTransform.GetComponents( xx, xy, xz, dx, yx, yy, yz, dy, zx, zy, zz, dz );
    m_globalToPDPanelTransformSIMD.SetComponents( (FP)xx, (FP)xy, (FP)xz, (FP)dx, (FP)yx, (FP)yy, (FP)yz, (FP)dy,
                                                  (FP)zx, (FP)zy, (FP)zz, (FP)dz );
  }

  {
    const auto& p        = m_detectionPlane;
    m_detectionPlaneSIMD = Rich::SIMD::Plane<Rich::SIMD::DefaultScalarFP>( (FP)p.A(), (FP)p.B(), (FP)p.C(), (FP)p.D() );
  }

  return sc;
}

bool DeRichPMTPanel::smartID( const Gaudi::XYZPoint& globalPoint, //
                              LHCb::RichSmartID&     id ) const {
  id                 = panelID(); // sets RICH, panel and type
  const auto inPanel = m_toLocalMatrixSIMD * SIMDPoint( globalPoint );
  // return false if invalid id is found (e.g. in non-existent PMT)
  const auto&& [a, m] = findPMTArraySetupSIMD( inPanel );
  const auto ok       = m[0];
  if ( ok ) {
    // get PMT module number in panel
    const Int a0 = ( a[0] )[0];
    const Int a1 = ( a[1] )[0];
    const Int a2 = ( a[2] )[0];
    const Int a3 = ( a[3] )[0];
    id.setLargePMT( ModuleIsWithGrandPMT( a0 ) );
    setRichPmtSmartID( a0, a1, a2, a3, id );
  }
  return ok;
}

StatusCode DeRichPMTPanel::getPanelGeometryInfo() {
  StatusCode sc = StatusCode::SUCCESS;

  auto* firstRich = getFirstDeRich();
  if ( !firstRich ) { return Error( "Cannot locate first RICH detector" ); }

  // get config info
  m_Rich2UseGrandModule = false;
  m_Rich2UseMixedModule = false;
  m_Rich2ArrayConfig    = 0;
  if ( firstRich->exists( "Rich2PMTArrayConfig" ) ) {
    m_Rich2ArrayConfig = firstRich->param<int>( "Rich2PMTArrayConfig" );
    if ( m_Rich2ArrayConfig >= 1 ) {
      m_Rich2UseGrandModule = true;
      if ( m_Rich2ArrayConfig == 2 || m_Rich2ArrayConfig == 3 ) { m_Rich2UseMixedModule = true; }
    }
  }

  // load geometry parameters
  m_PmtModulePlaneHalfSizeR1SIMD =
      toarray<SIMDFP, 4>( firstRich->param<std::vector<double>>( "Rich1PMTModulePlaneHalfSize" ) );
  const auto aRich1PmtNumModulesInRow = firstRich->param<int>( "Rich1NumberOfModulesInRow" );
  const auto aRich1PmtNumModulesInCol = firstRich->param<int>( "Rich1NumberOfModulesInCol" );
  m_RichPmtNumModulesInRowCol[0]      = aRich1PmtNumModulesInRow;
  m_RichPmtNumModulesInRowCol[1]      = aRich1PmtNumModulesInCol;

  std::array<SIMDFP, 6> PmtAllModulePitch =
      toarray<SIMDFP, 6>( firstRich->param<std::vector<double>>( "RichPmtAllModulePitch" ) );
  for ( const auto i : {0, 1, 2, 3, 4, 5} ) {
    m_PmtAllModulePitchInvSIMD[i] = ( fabs( PmtAllModulePitch[i][0] ) > 0 ? 1.0 / PmtAllModulePitch[i][0] : 0.0 );
  }

  const auto aRich2PmtNumModulesInRow = firstRich->param<int>( "Rich2NumberOfModulesInRow" );
  const auto aRich2PmtNumModulesInCol = firstRich->param<int>( "Rich2NumberOfModulesInCol" );
  m_RichPmtNumModulesInRowCol[2]      = aRich2PmtNumModulesInRow;
  m_RichPmtNumModulesInRowCol[3]      = aRich2PmtNumModulesInCol;

  // info()<< "DeRichPmtplane  getPanelGeometryInfo  m_RichPmtNumModulesInRowCol "
  //       <<  m_RichPmtNumModulesInRowCol <<endmsg;

  m_RichPmtModuleCopyNumBeginPanel =
      toarray<Int, 4>( firstRich->param<std::vector<int>>( "RichPmtModuleNumBeginInPanels" ) );
  m_RichPmtModuleCopyNumEndPanel =
      toarray<Int, 4>( firstRich->param<std::vector<int>>( "RichPmtModuleNumEndInPanels" ) );

  m_RichPmtModuleActiveAreaHalfSizeSIMD =
      toarray<SIMDFP, 2>( firstRich->param<std::vector<double>>( "Rich1StdPMTModuleActiveAreaHalfSize" ) );

  const auto PmtPitch = firstRich->param<double>( "RichPmtPitch" );
  m_PmtPitchInvSIMD   = ( fabs( PmtPitch ) > 0 ? 1.0 / PmtPitch : 0.0 );

  m_NumPmtInRowCol[0]       = firstRich->param<int>( "RichPmtNumInModuleRow" );
  m_NumPmtInRowCol[1]       = firstRich->param<int>( "RichPmtNumInModuleCol" );
  m_NumPmtInRichModule      = firstRich->param<int>( "RichTotNumPmtInModule" );
  m_NumPmtInRichGrandModule = firstRich->param<int>( "RichTotNumPmtInGrandModule" );

  const auto aRich1NumModules = firstRich->param<int>( "Rich1TotNumModules" );
  const auto aRich2NumModules = firstRich->param<int>( "Rich2TotNumModules" );
  m_totNumPmtModuleInRich     = aRich1NumModules + aRich2NumModules;

  m_NumPmtModuleInRich[0] = aRich1NumModules / 2; // rich1top
  m_NumPmtModuleInRich[1] = aRich1NumModules / 2; // rich1bottom
  m_NumPmtModuleInRich[2] = aRich2NumModules / 2; // rich2left
  m_NumPmtModuleInRich[3] = aRich2NumModules / 2; // rich2right

  const auto PmtAnodeXSize = firstRich->param<double>( "RichPmtAnodeXSize" );
  const auto PmtAnodeYSize = firstRich->param<double>( "RichPmtAnodeYSize" );

  m_PmtPixelsInRowSIMD                   = firstRich->param<int>( "RichPmtNumPixelCol" );
  m_PmtPixelsInColSIMD                   = firstRich->param<int>( "RichPmtNumPixelRow" );
  m_PmtAnodeXEdge                        = -0.5 * ( PmtAnodeXSize );
  m_PmtAnodeYEdge                        = -0.5 * ( PmtAnodeYSize );
  const auto AnodeXPixelSize             = firstRich->param<double>( "RichPmtPixelXSize" );
  const auto AnodeYPixelSize             = firstRich->param<double>( "RichPmtPixelYSize" );
  const auto PmtAnodeEffectiveXPixelSize = AnodeXPixelSize;
  const auto PmtAnodeEffectiveYPixelSize = AnodeYPixelSize;
  m_PmtAnodeEffectiveXPixelSizeInvSIMD   = 1.0 / PmtAnodeEffectiveXPixelSize;
  m_PmtAnodeEffectiveYPixelSizeInvSIMD   = 1.0 / PmtAnodeEffectiveYPixelSize;
  m_PmtMasterLateralSize                 = firstRich->param<double>( "RichPmtMasterLateralSize" );
  const auto Rich1TotNumPmts             = firstRich->param<int>( "Rich1TotNumPmt" );
  const auto Rich2TotNumPmts             = firstRich->param<int>( "Rich2TotNumPmt" );
  m_totNumPMTs                           = Rich1TotNumPmts + Rich2TotNumPmts;

  // grand/mixed modules

  m_RichGrandPmtModuleActiveAreaHalfSizeSIMD =
      toarray<SIMDFP, 2>( firstRich->param<std::vector<double>>( "Rich2GrandPMTModuleActiveAreaHalfSize" ) );

  const auto GrandPmtPitch = firstRich->param<double>( "RichGrandPmtPitch" );
  m_GrandPmtPitchInvSIMD   = ( fabs( GrandPmtPitch ) > 0 ? 1.0 / GrandPmtPitch : 0.0 );

  const auto GrandPmtAnodeXSize = firstRich->param<double>( "RichGrandPmtAnodeXSize" );
  const auto GrandPmtAnodeYSize = firstRich->param<double>( "RichGrandPmtAnodeYSize" );

  m_GrandPmtPixelsInRowSIMD                   = firstRich->param<int>( "RichGrandPmtNumPixelCol" );
  m_GrandPmtPixelsInColSIMD                   = firstRich->param<int>( "RichGrandPmtNumPixelRow" );
  m_GrandPmtAnodeXEdge                        = -0.5 * ( GrandPmtAnodeXSize );
  m_GrandPmtAnodeYEdge                        = -0.5 * ( GrandPmtAnodeYSize );
  const auto GrandAnodeXPixelSize             = firstRich->param<double>( "RichGrandPmtPixelXSize" );
  const auto GrandAnodeYPixelSize             = firstRich->param<double>( "RichGrandPmtPixelYSize" );
  const auto GrandPmtAnodeEffectiveXPixelSize = GrandAnodeXPixelSize;
  const auto GrandPmtAnodeEffectiveYPixelSize = GrandAnodeYPixelSize;
  m_GrandPmtAnodeEffectiveXPixelSizeInvSIMD   = 1.0 / GrandPmtAnodeEffectiveXPixelSize;
  m_GrandPmtAnodeEffectiveYPixelSizeInvSIMD   = 1.0 / GrandPmtAnodeEffectiveYPixelSize;
  const auto GrandPmtEdgePixelSizeXSIMD       = firstRich->param<double>( "RichGrandPmtEdgePixelXSize" );
  const auto GrandPmtEdgePixelSizeYSIMD       = firstRich->param<double>( "RichGrandPmtEdgePixelYSize" );
  m_GrandPmtEdgePixelSizeDiffXSIMD            = fabs( GrandPmtEdgePixelSizeXSIMD - GrandAnodeXPixelSize );
  m_GrandPmtEdgePixelSizeDiffYSIMD            = fabs( GrandPmtEdgePixelSizeYSIMD - GrandAnodeYPixelSize );
  m_GrandPmtMasterLateralSize                 = firstRich->param<double>( "RichGrandPmtMasterLateralSize" );

  m_NumGrandPmtInRowCol[0] = firstRich->param<int>( "RichGrandPmtNumInModuleRow" );
  m_NumGrandPmtInRowCol[1] = firstRich->param<int>( "RichGrandPmtNumInModuleCol" );

  m_MixedPmtModulePlaneHalfSizeR2SIMD =
      toarray<SIMDFP, 4>( firstRich->param<std::vector<double>>( "Rich2MixedPMTModulePlaneHalfSize" ) );

  m_MixedStdPmtModulePlaneHalfSizeR2SIMD =
      toarray<SIMDFP, 4>( firstRich->param<std::vector<double>>( "Rich2MixedStdPMTModulePlaneHalfSize" ) );

  m_Rich2MixedModuleArrayColumnSize = firstRich->param<std::vector<int>>( "Rich2MixedNumModulesArraySetup" );

  // Setup flags for large PMTs
  RichSetupMixedSizePmtModules();

  return sc;
}

void DeRichPMTPanel::RichSetupMixedSizePmtModules() {

  m_ModuleIsWithGrandPMT.assign( m_totNumPmtModuleInRich, false );

  // Now set the flags for module according to the geometry setup

  if ( m_Rich2UseGrandModule ) {

    const auto aTotNumModInRich1 = m_NumPmtModuleInRich[0] + m_NumPmtModuleInRich[1];

    for ( Int im = aTotNumModInRich1; im < m_totNumPmtModuleInRich; ++im ) {

      Int imLocal = im - aTotNumModInRich1;
      if ( imLocal > m_NumPmtModuleInRich[2] ) imLocal -= m_NumPmtModuleInRich[2];
      const Int aColNum = imLocal / ( m_RichPmtNumModulesInRowCol[3] );
      const Int aRowNum = imLocal - aColNum * ( m_RichPmtNumModulesInRowCol[3] );
      if ( aRowNum < m_Rich2MixedModuleArrayColumnSize[0] ||
           aRowNum >= ( m_Rich2MixedModuleArrayColumnSize[0] + m_Rich2MixedModuleArrayColumnSize[1] ) ) {
        m_ModuleIsWithGrandPMT[im] = true;
      }

    } // end loop over modules in rich2

  } // end if rich2useGrandModule
}

//=========================================================================
// Gets the PMT information (SIMD)
//=========================================================================
DeRichPMTPanel::ArrayWithMask                                                  //
DeRichPMTPanel::findPMTArraySetupSIMD( const SIMDPoint&        aLocalPoint,    //
                                       const bool              includePixInfo, //
                                       const SIMDFP::mask_type in_mask ) const {

  using namespace LHCb::SIMD;

  // return object
  ArrayWithMask am( in_mask );
  // shortcuts
  auto& aCh     = am.array;
  auto& mask_sc = am.mask;

  // First, deduce the PD module numbers...

  ModuleNumbersSIMD nums;
  getModuleNumsSIMD( aLocalPoint.x(), aLocalPoint.y(), nums );

#ifndef NDEBUG
  if ( UNLIKELY( any_of( nums.aModuleNum < SIMDINT32::Zero() ) ) ) {
    error() << "DeRichPmtPanel : findPMTArraySetupSIMD : Problem getting module numbers" << endmsg;
    mask_sc = SIMDFP::MaskType( false );
    return am;
  }
#endif

  // set the module number
  aCh[0] = nums.aModuleNum;

  // Use cached positions
  SIMDFP xp( aLocalPoint.X() ), yp( aLocalPoint.Y() );
  GAUDI_LOOP_UNROLL( SIMDINT32::Size )
  for ( std::size_t i = 0; i < SIMDINT32::Size; ++i ) {
    const auto& p = m_DePMTModulesZeroPtn[nums.aModuleNumInPanel[i]];
    xp[i] -= p.X();
    yp[i] -= p.Y();
  }

  SIMDINT32 aPmtCol = -SIMDINT32::One();
  SIMDINT32 aPmtRow = -SIMDINT32::One();

  auto& aPmtNum = aCh[1];

  const bool is_r2     = ( rich() == Rich::Rich2 );
  const auto gmask     = ModuleIsWithGrandPMT( nums.aModuleNum );
  const auto any_gmask = is_r2 && any_of( gmask );
  const auto all_gmask = any_gmask && all_of( gmask );

  if ( !any_gmask ) {

    // All small PMTs
    if ( !is_r2 ) {
      // RICH1

      // In Rich::top (Rich::bottom) module zero is at the top left (bottom right) corner
      // the following lines assume that all points in SIMD operation are in the same panel
      const auto sign = ( side() == Rich::top ? -SIMDFP::One() : SIMDFP::One() );
      aPmtCol =
          simd_cast<SIMDINT32>( abs( ( sign * xp - m_RichPmtModuleActiveAreaHalfSizeSIMD[0] ) * m_PmtPitchInvSIMD ) );
      aPmtRow =
          simd_cast<SIMDINT32>( abs( ( sign * yp - m_RichPmtModuleActiveAreaHalfSizeSIMD[1] ) * m_PmtPitchInvSIMD ) );

    } else {
      // RICH2

      aPmtCol = simd_cast<SIMDINT32>( abs( ( -yp - m_RichPmtModuleActiveAreaHalfSizeSIMD[0] ) * m_PmtPitchInvSIMD ) );
      aPmtRow = simd_cast<SIMDINT32>( abs( ( xp - m_RichPmtModuleActiveAreaHalfSizeSIMD[1] ) * m_PmtPitchInvSIMD ) );
    }

    aPmtNum = getPmtNumFromRowCol( aPmtRow, aPmtCol );

  } else if ( all_gmask ) {

    // All grand PMTs. Only in RICH2.
    assert( is_r2 );
    aPmtCol =
        simd_cast<SIMDINT32>( abs( ( -yp - m_RichGrandPmtModuleActiveAreaHalfSizeSIMD[1] ) * m_GrandPmtPitchInvSIMD ) );
    aPmtRow =
        simd_cast<SIMDINT32>( abs( ( xp - m_RichGrandPmtModuleActiveAreaHalfSizeSIMD[0] ) * m_GrandPmtPitchInvSIMD ) );
    aPmtNum = getGrandPmtNumFromRowCol( aPmtRow, aPmtCol );

  } else {

    // we have a mixture of grand and small PMTs
    // in current (upgrade) system this is only possible for R2
    assert( is_r2 );
    aPmtCol = iif(
        gmask, //
        simd_cast<SIMDINT32>( abs( ( -yp - m_RichGrandPmtModuleActiveAreaHalfSizeSIMD[1] ) * m_GrandPmtPitchInvSIMD ) ),
        simd_cast<SIMDINT32>( abs( ( -yp - m_RichPmtModuleActiveAreaHalfSizeSIMD[0] ) * m_PmtPitchInvSIMD ) ) );
    aPmtRow = iif(
        gmask, //
        simd_cast<SIMDINT32>( abs( ( xp - m_RichGrandPmtModuleActiveAreaHalfSizeSIMD[0] ) * m_GrandPmtPitchInvSIMD ) ),
        simd_cast<SIMDINT32>( abs( ( xp - m_RichPmtModuleActiveAreaHalfSizeSIMD[1] ) * m_PmtPitchInvSIMD ) ) );
    aPmtNum = iif( gmask,                                        //
                   getGrandPmtNumFromRowCol( aPmtRow, aPmtCol ), //
                   getPmtNumFromRowCol( aPmtRow, aPmtCol ) );
  }

#ifndef NDEBUG
  if ( UNLIKELY( any_of( aPmtNum < SIMDINT32::Zero() ) ) ) {
    error() << "DeRichPmtPanel : findPMTArraySetupSIMD : Problem getting PMT numbers" << endmsg;
    mask_sc = SIMDFP::MaskType( false );
    return am;
  }
#endif

  // Now, if required, the pixel numbers
  if ( UNLIKELY( includePixInfo ) ) {

    SIMDFP xpi, ypi;
    // this is not ideal. Should see if we can reduce what we need to do here...
    // start by apply global transform using SIMD
    const auto pointInPmtAnode = m_toGlobalMatrixSIMD * aLocalPoint;
    // have to fall back to scalar loop for PD specific transform....
    // Would be good to improve this as will adversely impact CPU performance
    GAUDI_LOOP_UNROLL( SIMDINT32::Size )
    for ( std::size_t i = 0; i < SIMDINT32::Size; ++i ) {
      if ( m_DePMTAnodes[nums.aModuleNumInPanel[i]][aPmtNum[i]] ) {
        // transform for PD
        const auto& mToLocalAnode = ( m_DePMTAnodes[nums.aModuleNumInPanel[i]][aPmtNum[i]] )->toLocalMatrix();
        // get point in pd
        const auto pointInPmtAnode_sc = mToLocalAnode * Gaudi::XYZPoint{pointInPmtAnode.x()[i], //
                                                                        pointInPmtAnode.y()[i], //
                                                                        pointInPmtAnode.z()[i]};
        xpi[i]                        = pointInPmtAnode_sc.x();
        ypi[i]                        = pointInPmtAnode_sc.y();
      } else {
        mask_sc[i] = false;
      }
    }

    auto& aPmtPixelCol = aCh[2];
    auto& aPmtPixelRow = aCh[3];

    if ( !any_gmask ) {

      // All small PMTs
      aPmtPixelCol =
          simd_cast<SIMDINT32>( abs( ( xpi - m_PmtAnodeXEdgeSIMD ) * m_PmtAnodeEffectiveXPixelSizeInvSIMD ) );
      aPmtPixelCol( aPmtPixelCol >= m_PmtPixelsInColSIMD ) = m_PmtPixelsInColSIMD - SIMDINT32::One();
      aPmtPixelRow =
          simd_cast<SIMDINT32>( abs( ( ypi - m_PmtAnodeYEdgeSIMD ) * m_PmtAnodeEffectiveYPixelSizeInvSIMD ) );
      aPmtPixelRow( aPmtPixelRow >= m_PmtPixelsInRowSIMD ) = m_PmtPixelsInRowSIMD - SIMDINT32::One();

    } else if ( all_gmask ) {

      // All grand PMTs
      // try to find pixel coord normally after offsetting the xpi/ypi by difference in size of edge pixel in grand PMT
      aPmtPixelCol =
          simd_cast<SIMDINT32>( abs( ( ( xpi - m_GrandPmtEdgePixelSizeDiffXSIMD ) - m_GrandPmtAnodeXEdgeSIMD ) *
                                     m_GrandPmtAnodeEffectiveXPixelSizeInvSIMD ) );
      aPmtPixelRow =
          simd_cast<SIMDINT32>( abs( ( ( ypi - m_GrandPmtEdgePixelSizeDiffYSIMD ) - m_GrandPmtAnodeYEdgeSIMD ) *
                                     m_GrandPmtAnodeEffectiveYPixelSizeInvSIMD ) );

      // if the hit falls into 'extra' edge pixel area, set it accordingly
      const auto maskInEdgePixelXFirst =
          simd_cast<SIMDINT32::MaskType>( ( xpi < m_GrandPmtEdgePixelSizeDiffXSIMD ) && ( xpi > 0 ) );
      const auto maskInEdgePixelXLast = simd_cast<SIMDINT32::MaskType>(
          xpi < -m_GrandPmtAnodeXEdgeSIMD && xpi > -m_GrandPmtAnodeXEdgeSIMD - m_GrandPmtEdgePixelSizeDiffXSIMD );
      const auto maskInEdgePixelYFirst =
          simd_cast<SIMDINT32::MaskType>( ( ypi < m_GrandPmtEdgePixelSizeDiffYSIMD ) && ( ypi > 0 ) );
      const auto maskInEdgePixelYLast = simd_cast<SIMDINT32::MaskType>(
          ypi < -m_GrandPmtAnodeYEdgeSIMD && xpi > -m_GrandPmtAnodeYEdgeSIMD - m_GrandPmtEdgePixelSizeDiffYSIMD );
      aPmtPixelCol( maskInEdgePixelXFirst ) = SIMDINT32::Zero();
      aPmtPixelRow( maskInEdgePixelYFirst ) = SIMDINT32::Zero();
      aPmtPixelCol( maskInEdgePixelXLast )  = m_GrandPmtPixelsInColSIMD - SIMDINT32::One();
      aPmtPixelRow( maskInEdgePixelYLast )  = m_GrandPmtPixelsInRowSIMD - SIMDINT32::One();

      aPmtPixelCol( aPmtPixelCol >= m_GrandPmtPixelsInColSIMD ) = m_GrandPmtPixelsInColSIMD - SIMDINT32::One();
      aPmtPixelRow( aPmtPixelRow >= m_GrandPmtPixelsInRowSIMD ) = m_GrandPmtPixelsInRowSIMD - SIMDINT32::One();

    } else {

      // we have a mixture of grand and small PMTs
      // for grand PMTs: try to find pixel coord normally after offsetting the xpi/ypi by difference in size of edge
      // pixel in grand PMT
      const auto not_gmask = !gmask;
      aPmtPixelCol =
          iif( gmask,
               simd_cast<SIMDINT32>( abs( ( ( xpi - m_GrandPmtEdgePixelSizeDiffXSIMD ) - m_GrandPmtAnodeXEdgeSIMD ) *
                                          m_GrandPmtAnodeEffectiveXPixelSizeInvSIMD ) ),
               simd_cast<SIMDINT32>( abs( ( xpi - m_PmtAnodeXEdgeSIMD ) * m_PmtAnodeEffectiveXPixelSizeInvSIMD ) ) );
      aPmtPixelRow =
          iif( gmask,
               simd_cast<SIMDINT32>( abs( ( ( ypi - m_GrandPmtEdgePixelSizeDiffYSIMD ) - m_GrandPmtAnodeYEdgeSIMD ) *
                                          m_GrandPmtAnodeEffectiveYPixelSizeInvSIMD ) ),
               simd_cast<SIMDINT32>( abs( ( ypi - m_PmtAnodeYEdgeSIMD ) * m_PmtAnodeEffectiveYPixelSizeInvSIMD ) ) );

      // for grand PMTs: if the hit falls into 'extra' edge pixel area, set it accordingly
      const auto maskInEdgePixelXFirst =
          gmask && simd_cast<SIMDINT32::MaskType>( ( xpi < m_GrandPmtEdgePixelSizeDiffXSIMD ) && ( xpi > 0 ) );
      const auto maskInEdgePixelXLast =
          gmask && simd_cast<SIMDINT32::MaskType>( xpi < -m_GrandPmtAnodeXEdgeSIMD &&
                                                   xpi > -m_GrandPmtAnodeXEdgeSIMD - m_GrandPmtEdgePixelSizeDiffXSIMD );
      const auto maskInEdgePixelYFirst =
          gmask && simd_cast<SIMDINT32::MaskType>( ( ypi < m_GrandPmtEdgePixelSizeDiffYSIMD ) && ( ypi > 0 ) );
      const auto maskInEdgePixelYLast =
          gmask && simd_cast<SIMDINT32::MaskType>( ypi < -m_GrandPmtAnodeYEdgeSIMD &&
                                                   xpi > -m_GrandPmtAnodeYEdgeSIMD - m_GrandPmtEdgePixelSizeDiffYSIMD );
      aPmtPixelCol( maskInEdgePixelXFirst ) = SIMDINT32::Zero();
      aPmtPixelRow( maskInEdgePixelYFirst ) = SIMDINT32::Zero();
      aPmtPixelCol( maskInEdgePixelXLast )  = m_GrandPmtPixelsInColSIMD - SIMDINT32::One();
      aPmtPixelRow( maskInEdgePixelYLast )  = m_GrandPmtPixelsInRowSIMD - SIMDINT32::One();

      aPmtPixelCol( gmask && aPmtPixelCol >= m_GrandPmtPixelsInColSIMD ) = m_GrandPmtPixelsInColSIMD - SIMDINT32::One();
      aPmtPixelCol( not_gmask && aPmtPixelCol >= m_PmtPixelsInColSIMD )  = m_PmtPixelsInColSIMD - SIMDINT32::One();
      aPmtPixelRow( gmask && aPmtPixelRow >= m_GrandPmtPixelsInRowSIMD ) = m_GrandPmtPixelsInRowSIMD - SIMDINT32::One();
      aPmtPixelRow( not_gmask && aPmtPixelRow >= m_PmtPixelsInRowSIMD )  = m_PmtPixelsInRowSIMD - SIMDINT32::One();
    }

    aPmtPixelCol.setZero( aPmtPixelCol < SIMDINT32::Zero() );
    aPmtPixelRow.setZero( aPmtPixelRow < SIMDINT32::Zero() );
  } // include pixel info

  // finally return the data
  return am;
}

//=========================================================================
// returns the (SIMD) intersection point with the detection plane
//=========================================================================
DeRichPMTPanel::SIMDRayTResult::Results                                   //
DeRichPMTPanel::detPlanePointSIMD( const SIMDPoint&          pGlobal,     //
                                   const SIMDVector&         vGlobal,     //
                                   SIMDPoint&                hitPosition, //
                                   SIMDRayTResult::SmartIDs& smartID,     //
                                   SIMDRayTResult::PDs&      PDs,         //
                                   const LHCb::RichTraceMode mode         //
                                   ) const {

  using namespace LHCb::SIMD;

  // results to return. defaults to outside panel.
  SIMDRayTResult::Results res( (unsigned int)LHCb::RichTraceMode::RayTraceResult::OutsidePDPanel );

  // panel intersection in global coords
  hitPosition = getPanelInterSection( pGlobal, vGlobal );

  // set hit position to plane intersection in local frame
  const auto panelIntersection = m_toLocalMatrixSIMD * hitPosition;

  // maybe should be an option ?
  const bool includePixInfo = false;

  // get the PMT info (SIMD)
  const auto aC = findPMTArraySetupSIMD( panelIntersection, includePixInfo );
  if ( LIKELY( any_of( aC.mask ) ) ) {

    // get PMT module number in panel
    // Note module number should *always* be for current panel, so use method that assumes this
    const auto pdNumInPanel = PmtModuleNumInPanelFromModuleNum( aC.array[0] );
    // Sanity checks, only in debug builds
    assert( all_of( pdNumInPanel == PmtModuleNumInPanelFromModuleNumAlone( aC.array[0] ) ) );

    // in panel mask
    auto pmask = aC.mask && isInPmtPanel( panelIntersection );

    // Resort to a scalar loop at this point. To be improved...
    for ( std::size_t i = 0; i < SIMDFP::Size; ++i ) {
      if ( LIKELY( pmask[i] ) ) {

        // get the DePMT object
        PDs[i] = dePMT( pdNumInPanel[i], aC.array[1][i] );
        if ( PDs[i] ) {
          // Set the SmartID to the PD ID
          smartID[i] = PDs[i]->pdSmartID();
          // set the pixel parts
          if ( UNLIKELY( includePixInfo ) ) { setRichPmtSmartIDPix( aC.array[2][i], aC.array[3][i], smartID[i] ); }
        } else {
          pmask[i] = false;
        }

        // set final status
        res[i] = ( mode.detPlaneBound() >= LHCb::RichTraceMode::DetectorPlaneBoundary::RespectPDPanel
                       ? pmask[i] ? LHCb::RichTraceMode::RayTraceResult::InPDPanel
                                  : LHCb::RichTraceMode::RayTraceResult::OutsidePDPanel
                       : LHCb::RichTraceMode::RayTraceResult::InPDPanel );
      }
    }
  }

  return res;
}

//===================================================================================
// Returns the intersection point with the detector plane given a vector and a point.
//===================================================================================
LHCb::RichTraceMode::RayTraceResult                                   //
DeRichPMTPanel::detPlanePoint( const Gaudi::XYZPoint&    pGlobal,     //
                               const Gaudi::XYZVector&   vGlobal,     //
                               Gaudi::XYZPoint&          hitPosition, //
                               LHCb::RichSmartID&        smartID,     //
                               const DeRichPD*&          pd,          //
                               const LHCb::RichTraceMode mode         //
                               ) const {

  // Use the SIMD method
  // Note this will not be as efficient as properly using the SIMD methods,
  // but eventually the scalar calls should be fully depreciated anyway so this
  // is just a short term placeholder.

  // form some temporary objects
  SIMDRayTResult::SmartIDs simdIDs;
  simdIDs.fill( smartID );
  SIMDPoint           simdHitPoint( hitPosition );
  SIMDRayTResult::PDs PDs;
  PDs.fill( pd );

  // Call the SIMD method
  const auto simdRes = detPlanePointSIMD( SIMDPoint( pGlobal ),  //
                                          SIMDVector( vGlobal ), //
                                          simdHitPoint, simdIDs, PDs, mode );

  // copy results back to scalars. All entries are the same so use [0]
  hitPosition = {simdHitPoint.x()[0], simdHitPoint.y()[0], simdHitPoint.z()[0]};
  pd          = PDs[0];
  smartID     = simdIDs[0];

  // return
  return LHCb::RichTraceMode::RayTraceResult( (int)simdRes[0] );
}

//=========================================================================
// find an intersection with the PMT window (SIMD)
//=========================================================================
DeRichPMTPanel::SIMDRayTResult::Results                                   //
DeRichPMTPanel::PDWindowPointSIMD( const SIMDPoint&          pGlobal,     //
                                   const SIMDVector&         vGlobal,     //
                                   SIMDPoint&                hitPosition, //
                                   SIMDRayTResult::SmartIDs& smartID,     //
                                   SIMDRayTResult::PDs&      PDs,         //
                                   const LHCb::RichTraceMode mode         //
                                   ) const {

  using namespace LHCb::SIMD;

  // results to return. Default to outside panel.
  SIMDRayTResult::Results res( (unsigned int)LHCb::RichTraceMode::RayTraceResult::OutsidePDPanel );

  // panel intersection in global
  hitPosition = getPanelInterSection( pGlobal, vGlobal );

  // transform to local
  const auto panelIntersection = m_toLocalMatrixSIMD * hitPosition;

  // sets RICH, panel and type
  smartID.fill( panelID() );

  // maybe should be an option ?
  const bool includePixInfo = false;

  // are we in the panel ?
  auto mask = isInPmtPanel( panelIntersection );
  if ( LIKELY( any_of( mask ) ) ) {

    // Set res flag for those in mask to InPDPanel
    res( simd_cast<SIMDRayTResult::Results::mask_type>( mask ) ) =
        SIMDRayTResult::Results( (unsigned int)LHCb::RichTraceMode::RayTraceResult::InPDPanel );

    // check PD acceptance ?
    if ( LIKELY( mode.detPlaneBound() != LHCb::RichTraceMode::DetectorPlaneBoundary::IgnorePDAcceptance ) ) {

      // get the PMT info (SIMD) - has to be checked here - findPMTArraySetupSIMD checks if smartID is valid (so
      // non-existent PMTs should give OutsidePDPanel)
      // is the smartID valid ?
      auto aC = findPMTArraySetupSIMD( panelIntersection, includePixInfo, mask );

      // get module in panel number
      // Note module number should *always* be for current panel, so use method that assumes this
      const auto aModuleNumInPanel = PmtModuleNumInPanelFromModuleNum( aC.array[0] );
      // Sanity checks, only in debug builds
      assert( all_of( aModuleNumInPanel == PmtModuleNumInPanelFromModuleNumAlone( aC.array[0] ) ) );

      // Is Grand PD Mask
      SIMDFP::mask_type gPdMask( false );

      // SIMD {X,Y} for PD centres in panel frame
      SIMDFP X( SIMDFP::Zero() ), Y( SIMDFP::Zero() );

      // Resort to a scalar loop at this point to extract PD specific info.
      for ( std::size_t i = 0; i < SIMDFP::Size; ++i ) {
        // skip those already failed
        if ( LIKELY( aC.mask[i] ) ) {

          // get the DePMT object
          const auto pmt = dePMT( aModuleNumInPanel[i], aC.array[1][i] );
          PDs[i]         = pmt;
          if ( LIKELY( pmt ) ) {

            // Set the SmartID to the PD ID
            smartID[i] = pmt->pdSmartID();

            // set the pixel parts
            if ( UNLIKELY( includePixInfo ) ) { setRichPmtSmartIDPix( aC.array[2][i], aC.array[3][i], smartID[i] ); }

            // Update SIMD PD X,Y in local panel
            X[i] = pmt->zeroInPanelLocal().X();
            Y[i] = pmt->zeroInPanelLocal().Y();

            // grand PD ?
            gPdMask[i] = pmt->PmtIsGrand();

          } else {
            aC.mask[i] = false;
          }
        }
      } // scalar loop

      // SIMD PMT acceptance check
      const auto pmt_mask = simd_cast<SIMDRayTResult::Results::mask_type>(
          aC.mask && checkPDAcceptance( panelIntersection.X() - X, panelIntersection.Y() - Y, gPdMask ) );
      res( pmt_mask ) = SIMDRayTResult::Results( (unsigned int)LHCb::RichTraceMode::RayTraceResult::InPDTube );

    } // check pd acceptance
  }   // in panel

  // return
  return res;
}

//=========================================================================
// Returns the intersection with the PD window
//=========================================================================
LHCb::RichTraceMode::RayTraceResult                                         //
DeRichPMTPanel::PDWindowPoint( const Gaudi::XYZPoint&    pGlobal,           //
                               const Gaudi::XYZVector&   vGlobal,           //
                               Gaudi::XYZPoint&          windowPointGlobal, //
                               LHCb::RichSmartID&        smartID,           //
                               const DeRichPD*&          pd,                //
                               const LHCb::RichTraceMode mode               //
                               ) const {

  // Use the SIMD method
  // Note this will not be as efficient as properly using the SIMD methods,
  // but eventually the scalar calls should be fully depreciated anyway so this
  // is just a short term placeholder.

  // form some temporary objects
  SIMDRayTResult::SmartIDs simdIDs;
  simdIDs.fill( smartID );
  SIMDPoint           simdHitPoint( windowPointGlobal );
  SIMDRayTResult::PDs PDs;
  PDs.fill( pd );

  // Call the SIMD method
  const auto simdRes = PDWindowPointSIMD( SIMDPoint( pGlobal ),  //
                                          SIMDVector( vGlobal ), //
                                          simdHitPoint, simdIDs, PDs, mode );

  // copy results back to scalars. All entries are the same so use [0]
  windowPointGlobal = {simdHitPoint.x()[0], simdHitPoint.y()[0], simdHitPoint.z()[0]};
  pd                = PDs[0];
  smartID           = simdIDs[0];

  // return
  return LHCb::RichTraceMode::RayTraceResult( (int)simdRes[0] );
}

Rich::DAQ::PDPanelIndex DeRichPMTPanel::pdNumber( const LHCb::RichSmartID& smartID ) const {
  return _pdNumber( smartID );
}

const DeRichPD* DeRichPMTPanel::dePD( const LHCb::RichSmartID pdID ) const { return dePMT( pdID ); }

//  return a list with all the valid readout channels (smartIDs)
//=========================================================================
bool DeRichPMTPanel::readoutChannelList( LHCb::RichSmartID::Vector& readoutChannels ) const {
  const auto aBeginM = m_RichPmtModuleCopyNumBeginPanel[m_CurPanelNum];
  const auto aEndM   = m_RichPmtModuleCopyNumEndPanel[m_CurPanelNum];

  // to do - Need to set the isLarge PMT flags here

  for ( Int iM = aBeginM; iM <= aEndM; ++iM ) {
    const auto aNumPmtInCurrentRichModule =
        ModuleIsWithGrandPMT( iM ) ? m_NumPmtInRichGrandModule : m_NumPmtInRichModule;
    for ( Int iP = 0; iP < aNumPmtInCurrentRichModule; ++iP ) {
      // create readout channels only for existing PMTs
      if ( dePMT( PmtModuleNumInPanelFromModuleNumAlone( iM ), iP ) ) {
        for ( Int iPx = 0; iPx < m_PmtPixelsInRowSIMD[0]; ++iPx ) {
          for ( Int iPy = 0; iPy < m_PmtPixelsInColSIMD[0]; ++iPy ) {
            auto tmpSmartId = LHCb::RichSmartID( rich(), side(), iP, iM, iPy, iPx, pdType() );
            tmpSmartId.setLargePMT( ModuleIsWithGrandPMT( iM ) );
            readoutChannels.emplace_back( tmpSmartId );
          }
        }
      }
    }
  }

  return true;
}

int DeRichPMTPanel::sensitiveVolumeID( const Gaudi::XYZPoint& globalPoint ) const {
  // Create a RichSmartID for this RICH and panel
  auto id = panelID();
  // set the remaining fields from the position
  return ( smartID( globalPoint, id ) ? id : panelID() );
}

//=========================================================================
//  setRichAndSide
//=========================================================================
bool DeRichPMTPanel::setRichAndSide() {

  bool OK = true;

  const auto centreGlobal = geometry()->toGlobal( Gaudi::XYZPoint{0, 0, 0} );

  // Work out what Rich/panel I am
  if ( name().find( "Rich1" ) != std::string::npos ) {
    setRichSide( Rich::Rich1, centreGlobal.y() > 0.0 ? Rich::top : Rich::bottom );
  } // Rich2
  else if ( name().find( "Rich2" ) != std::string::npos ) {
    setRichSide( Rich::Rich2, centreGlobal.x() > 0.0 ? Rich::left : Rich::right );
  }
  // Single Rich
  else if ( name().find( "Rich/" ) != std::string::npos ) {
    setRichSide( Rich::Rich, centreGlobal.x() > 0.0 ? Rich::left : Rich::right );
  }
  // problem
  else {
    error() << "Cannot identify Rich/side for " << name() << endmsg;
    OK = false;
  }

  if ( rich() == Rich::InvalidDetector || side() == Rich::InvalidSide ) {
    OK = false;
  } else {
    // cache the panel idex
    m_CurPanelNum = ( rich() == Rich::Rich1 ? ( side() == Rich::top ? 0 : 1 ) : ( side() == Rich::left ? 2 : 3 ) );
  }

  return OK;
}

DeRichPMTPanel::Int DeRichPMTPanel::getModuleCopyNumber( const std::string& aModuleName ) {

  Int        anumber = -1;
  const auto pos2    = aModuleName.find( ":" );
  if ( std::string::npos == pos2 ) {
    error() << "A PMTModule without a number! " << aModuleName << endmsg;
  } else {
    anumber = atoi( aModuleName.substr( pos2 + 1 ).c_str() );
  }

  return anumber;
}

bool DeRichPMTPanel::isLargePD( const LHCb::RichSmartID smartID ) const {
  return ModuleIsWithGrandPMT( smartID.pdCol() );
}
