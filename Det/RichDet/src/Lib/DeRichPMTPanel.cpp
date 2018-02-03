
// STL
#include <sstream>
#include <vector>

// Gaudi
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/PhysicalConstants.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IUpdateManagerSvc.h"

// local
#include "RichDet/DeRichPMTPanel.h"
#include "RichDet/DeRichSystem.h"

// MathCore files
#include "GaudiKernel/Transform3DTypes.h"

// DetDesc
#include "DetDesc/SolidBox.h"
#include "DetDesc/SolidTubs.h"
#include "DetDesc/SolidSphere.h"
#include "DetDesc/TabulatedProperty.h"

// GSL
#include "gsl/gsl_math.h"

//-----------------------------------------------------------------------------

const CLID CLID_DeRichPMTPanel = 12020;  // User defined

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
DeRichPMTPanel::DeRichPMTPanel( const std::string & name )
  : DeRichPDPanel ( name ),
    m_RichPmtModuleLensFlag(350),
    m_ModuleIsWithGrandPMT(350)
{
  // Set the PD type to PMT
  m_pdType = LHCb::RichSmartID::MaPMTID;
}

//=============================================================================
// Retrieve Pointer to class defininition structure
const CLID& DeRichPMTPanel::classID()
{
  return CLID_DeRichPMTPanel;
}

//=========================================================================
//  Initialize
//=========================================================================
StatusCode DeRichPMTPanel::initialize()
{
  // store the name of the panel, without the /dd/Structure part
  const auto pos = name().find("Rich");
  setMyName( std::string::npos != pos ? name().substr(pos) : "DeRichPMTPanel_NO_NAME" );

  debug() << "Initialize " << name() << endmsg;

  // set rich and side
  if ( !setRichAndSide() ) return StatusCode::FAILURE;

  // register UMS dependency on local geometry
  updMgrSvc()->registerCondition( this, geometry(), &DeRichPMTPanel::geometryUpdate );

  // trigger first UMS update
  return updMgrSvc()->update(this);
}

//=========================================================================
//  max PD index for this panel
//=========================================================================
Rich::DAQ::PDPanelIndex DeRichPMTPanel::maxPdNumber() const
{
  return m_maxPDCopyN;
}

//=========================================================================
//  getFirstDeRich
//=========================================================================
const DetectorElement * DeRichPMTPanel::getFirstDeRich() const
{
  SmartDataPtr<DetectorElement> afterMag( dataSvc(),"/dd/Structure/LHCb/AfterMagnetRegion" );
  if ( !afterMag )
  {
    error() << "Could not load AfterMagnetRegion det elem" << endmsg;
  }

  const std::string firstRichLoc = ( afterMag->exists("RichDetectorLocations") ?
                                     afterMag->paramVect<std::string>("RichDetectorLocations")[0] :
                                     DeRichLocations::Rich1 );

  SmartDataPtr<DetectorElement> deRich(dataSvc(), firstRichLoc );
  if ( !deRich )
  {
    error() << "Could not load DeRich for DeRichPMTPanel" << endmsg;
  }

  return deRich;
}

//=========================================================================
// generate the transforms for global <-> local frames
//=========================================================================
StatusCode DeRichPMTPanel::geometryUpdate()
{
  MsgStream msg ( msgSvc(), "DeRichPMTPanel" );

  auto * firstRich = getFirstDeRich();
  if ( !firstRich ) return StatusCode::FAILURE;

  // geom update
  StatusCode sc = getPanelGeometryInfo();

  // get the pmtmodule and pmt detector elements
  m_DePMTs.clear();
  m_DePMTModules.clear();
  m_DePMTAnodes.clear();

  const auto numCurModules = getNumModulesInThisPanel();

  m_DePMTModules.reserve(numCurModules);
  m_DePMTs.reserve(numCurModules);
  m_DePMTAnodes.reserve(numCurModules);

  // reset copy number to PMT vector
  //m_copyNumToPMT.assign( m_totNumPMTs, nullptr );

  const auto & detelems = childIDetectorElements();

  Int iModNum(0);
  for ( auto det_it = detelems.begin(); det_it != detelems.end(); ++det_it, ++iModNum )
  {
    if ( std::string::npos != (*det_it)->name().find("MAPMT_MODULE:") )
    {

      // get PMT Module
      SmartDataPtr<IDetectorElement> dePMTModule( dataSvc(), (*det_it)->name() );
      if ( !dePMTModule )
      {
        msg << MSG::FATAL << "Non DeRichPMT Module detector element "
            << (*det_it)->name() << endmsg;
        return StatusCode::FAILURE;
      }

      // save to list of PMT Modules
      m_DePMTModules.push_back( dePMTModule->geometry() );
      const auto aCurrentModuleCopyNumber = getModuleCopyNumber( dePMTModule->name() );

      const auto aNumPmtInCurrentRichModule = (Int) dePMTModule->childIDetectorElements().size();
      std::vector<DeRichPMT*>           DePmtsInCurModule     (aNumPmtInCurrentRichModule,nullptr);
      std::vector<const IGeometryInfo*> DePmtAnodesInCurModule(aNumPmtInCurrentRichModule,nullptr);

      // register UMS dependency.
      updMgrSvc()->registerCondition( this, dePMTModule->geometry(), &DeRichPMTPanel::geometryUpdate );

      // get PMT
      // for now assume the only det elements inside a module is just the pmts and
      // that inside the pmt is the anode. This may be changed in the future if needed with
      // some modif to the following lines.

      if ( !dePMTModule->childIDetectorElements().empty() )
      {
        for ( auto det_it_pm= dePMTModule->childIDetectorElements().begin();
              det_it_pm !=  dePMTModule->childIDetectorElements().end(); ++det_it_pm )
        {
          if ( std::string::npos != (*det_it_pm)->name().find("MAPMT:") )
          {
            // get PMT
            //info() << (*det_it_pm)->name() << endmsg;
            SmartDataPtr<DeRichPMT> dePMT( dataSvc(), (*det_it_pm)->name() );

            if ( dePMT )
            {

              // register UMS dependency
              updMgrSvc()->registerCondition( this, dePMT->geometry(),
                                              &DeRichPMTPanel::geometryUpdate );
              // get the current pmt and save.
              const auto curPmtNum     = det_it_pm - dePMTModule->childIDetectorElements().begin();
              const auto curPmtCopyNum = dePMT->pmtCopyNumber();

              // CRJ - These should be set by the DePMT class itself....
              dePMT->setPmtLensFlag   ( isCurrentPmtWithLens(curPmtCopyNum) );
              dePMT->setPmtIsGrandFlag( ModuleIsWithGrandPMT(aCurrentModuleCopyNumber)  );
              auto id = m_panelID;
              id.setPD_PMT(iModNum,curPmtNum);
              dePMT->setPDSmartID(id);
              // curPmtNum is SmartID pdInCol
              // iModNum is pdCol

              DePmtsInCurModule[curPmtNum] = dePMT;

              if ( ! dePMT->childIDetectorElements().empty() )
              {
                for ( auto det_it_pm_an = dePMT->childIDetectorElements().begin();
                      det_it_pm_an != dePMT->childIDetectorElements().end(); ++det_it_pm_an )
                {
                  if ( std::string::npos != (*det_it_pm_an)->name().find("MAPMTAnode:") )
                  {
                    SmartDataPtr<IDetectorElement> dePmtAnode( dataSvc(), (*det_it_pm_an)->name() );

                    if ( dePmtAnode )
                    {
                      // register UMS dependency
                      updMgrSvc()->registerCondition( this, dePmtAnode->geometry(),
                                                      &DeRichPMTPanel::geometryUpdate );
                      DePmtAnodesInCurModule[curPmtNum] = dePmtAnode->geometry();
                    }
                    else
                    {
                      msg << MSG::FATAL << "Non DeRichPMT Anode detector element "
                          << (*det_it_pm_an)->name() << endmsg;
                      return StatusCode::FAILURE;
                    }
                  } // end test anode name

                } // end loop over anodes in a pmt. We assume there is only 1 det elem inside a pmt
                  // which is the anode.
              } // end test on pt child det elem

            }
            else
            {      // end test existence of pmt
              msg << MSG::FATAL << "No DeRichPMT  detector element "
                  << (*det_it_pm)->name() << endmsg;
              return StatusCode::FAILURE;
            }

          } // end test pmt name

        } // end loop over pmts in a module

        // move here as we are done with them afterwards
        m_DePMTs.push_back     ( std::move(DePmtsInCurModule)      );
        m_DePMTAnodes.push_back( std::move(DePmtAnodesInCurModule) );

      }
      else
      {
        msg << MSG::FATAL << "Problem getting PMT Det elem from " << (*det_it)->name()
            << endmsg;
        return StatusCode::FAILURE;
      }
    } // end check on module

  } // end loop over modules

  m_PDColumns  = param<int>("PMTColumns");   // pd colums in a panel, not used, may be phased out
  m_PDNumInCol = param<int>("PMTNumberInColumn"); //pd rows in  a panel. not used, may be phased out.
  //  m_PDMax = nPDColumns() * nPDsPerCol(); // total in a  panel.
  m_PDMax = getNumModulesInThisPanel() * m_NumPmtInRichModule;

  double aOffset = 0.0;
  Gaudi::XYZVector aDir(0,0,0);
  Gaudi::XYZPoint  aPon(0,0,0);
  Int sign = 0;

  using GP = Gaudi::XYZPoint;

  if ( rich() == Rich::Rich1 )
  {

    sign = ( side() == Rich::top ? 1 : -1 );

    if ( side() == Rich::top )
    {
      const auto aPmtR1P0DetPlaneDirCos = firstRich->param<std::vector<double> >("Rich1TopPmtDetectorPlaneDirCos");
      const auto aPmtR1P0DetPlanePoint  = firstRich->param<std::vector<double> >("Rich1TopPmtDetPlanePoint");
      aOffset = fabs( m_PmtModulePlaneHalfSizeR1SIMD[1][0] );
      aDir = { aPmtR1P0DetPlaneDirCos[0], aPmtR1P0DetPlaneDirCos[1], aPmtR1P0DetPlaneDirCos[2] };
      aPon = { aPmtR1P0DetPlanePoint[0],  aPmtR1P0DetPlanePoint[1],  aPmtR1P0DetPlanePoint[2]  };
      m_xyHalfSizeSIMD[0] = fabs( m_PmtModulePlaneHalfSizeR1SIMD[0][0] );
      m_xyHalfSizeSIMD[1] = fabs( m_PmtModulePlaneHalfSizeR1SIMD[1][0] );
    }
    else if ( side () == Rich::bottom )
    {
      const auto aPmtR1P1DetPlaneDirCos = firstRich->param<std::vector<double> >("Rich1BotPmtDetectorPlaneDirCos");
      const auto aPmtR1P1DetPlanePoint  = firstRich->param<std::vector<double> >("Rich1BotPmtDetPlanePoint");
      aOffset = fabs( m_PmtModulePlaneHalfSizeR1SIMD[3][0] );
      aDir = { aPmtR1P1DetPlaneDirCos[0], aPmtR1P1DetPlaneDirCos[1], aPmtR1P1DetPlaneDirCos[2] };
      aPon = { aPmtR1P1DetPlanePoint[0],  aPmtR1P1DetPlanePoint[1],  aPmtR1P1DetPlanePoint[2]  };
      m_xyHalfSizeSIMD[0] = fabs( m_PmtModulePlaneHalfSizeR1SIMD[2][0] );
      m_xyHalfSizeSIMD[1] = fabs( m_PmtModulePlaneHalfSizeR1SIMD[3][0] );
    }

    m_detectionPlane = Gaudi::Plane3D(aDir, aPon);

    // Also create a plane at the exterior surface of the pmt quartz window.
    const auto z = firstRich->param<double> ("Rich1PmtDetPlaneZInPmtPanel") - m_RichPmtQuartzThickness;
    m_detectionPlane_exterior = Gaudi::Plane3D( geometry()->toGlobal( GP( 0,   0, z ) ),
                                                geometry()->toGlobal( GP( 0, 100, z ) ),
                                                geometry()->toGlobal( GP( 50, 50, z ) ) );

  }
  else // if ( rich() == Rich::Rich2 || rich() == Rich::Rich )
  {

    //It does not seem to be straightforward,  to get the detection plane param
    //directly from the xmldb for RICH2. So create
    // the actual detection plane using the Z Shift wrt the current panel.
    const auto aZShiftR2 = firstRich->param<double> ("Rich2PmtDetPlaneZInPmtPanel");
    // create three random points on the detection plane, which can define the plane.
    m_detectionPlane = Gaudi::Plane3D( geometry()->toGlobal( GP(  0,   0, aZShiftR2 ) ),
                                       geometry()->toGlobal( GP(  0, 100, aZShiftR2 ) ),
                                       geometry()->toGlobal( GP( 50,  50, aZShiftR2 ) ) );

    // Also create  a  second plane at the exterior surface of the PmtQuartzWindow.
    const auto z = aZShiftR2 - m_RichPmtQuartzThickness;
    m_detectionPlane_exterior = Gaudi::Plane3D( geometry()->toGlobal( GP(  0,   0, z ) ),
                                                geometry()->toGlobal( GP(  0, 100, z ) ),
                                                geometry()->toGlobal( GP( 50,  50, z ) ) );

    aPon = geometry()->toGlobal( GP( 0, 0, aZShiftR2 ) );

    // determine which panel
    const RowCol rci = ( side() == Rich::left ? RowCol{0,1} : RowCol{2,3} );

    sign = ( side() == Rich::left ? 1 : -1 );

    auto aOffsetX = ( !m_Rich2UseGrandModule ? fabs( m_PmtModulePlaneHalfSizeR2SIMD[rci[0]][0] ) :
                      !m_Rich2UseMixedModule ? fabs( m_GrandPmtModulePlaneHalfSizeR2SIMD[rci[0]][0] ) :
                                               fabs( m_MixedPmtModulePlaneHalfSizeR2SIMD[rci[0]][0] ) );
    auto aOffsetY = ( !m_Rich2UseGrandModule ? fabs( m_PmtModulePlaneHalfSizeR2SIMD[rci[1]][0] ) :
                      !m_Rich2UseMixedModule ? fabs( m_GrandPmtModulePlaneHalfSizeR2SIMD[rci[1]][0] ) :
                                               fabs( m_MixedPmtModulePlaneHalfSizeR2SIMD[rci[1]][0] ) );
    aOffset = aOffsetX;
    m_xyHalfSizeSIMD[0] = aOffsetX;
    m_xyHalfSizeSIMD[1] = aOffsetY;

  }
  m_localOffset = aOffset;

  m_detPlaneZ = aPon.z();
  // m_detPlaneZ = geometry()->toLocal(aPon.z()); // it seems the local Z coord is to be saved,
  // looking at the hpd version  ?
  //  if so, uncomment this line and comment out the previous line.

  //  const ROOT::Math::Translation3D localTranslation = ROOT::Math::Translation3D(aPon.x(),sign*aOffset,aPon.z());
  const ROOT::Math::Translation3D localTranslation =
    ( rich() == Rich::Rich1 ?
      ROOT::Math::Translation3D(aPon.x(),sign*aOffset,aPon.z()) :
      ROOT::Math::Translation3D(sign*aOffset,aPon.y(),aPon.z()) );

  m_globalToPDPanelTransform = localTranslation * geometry()->toLocalMatrix();
  m_PDPanelToGlobalTransform = m_globalToPDPanelTransform.Inverse();

  m_localPlane = geometry()->toLocalMatrix() * m_detectionPlane;

  m_localPlaneNormal = m_localPlane.Normal();

  // loop over all PD smartIDs to work out the largest copy number for this panel
  m_maxPDCopyN = Rich::DAQ::PDPanelIndex(0);
  for ( const auto pd : deRichSys()->allPDRichSmartIDs() )
  {
    if ( pd.rich() == rich() && pd.panel() == side() )
    {
      const auto copyN =  _pdNumber( pd );
      if ( copyN > m_maxPDCopyN ) { m_maxPDCopyN = copyN; }
    }
  }
  _ri_debug << "Max PD Copy Number = " <<  m_maxPDCopyN << endmsg;

  // Define function pointers

  if ( Rich::Rich1 == rich() )
  {
    if ( m_Rich1PmtLensPresence )
    {
      m_getModuleNumsSIMD = ( side() == Rich::top ?
                              &DeRichPMTPanel::getModuleNums_R1Up_Lens_SIMD :
                              &DeRichPMTPanel::getModuleNums_R1Dn_Lens_SIMD );
    }
    else
    {
      m_getModuleNumsSIMD = ( side() == Rich::top ?
                              &DeRichPMTPanel::getModuleNums_R1Up_NoLens_SIMD :
                              &DeRichPMTPanel::getModuleNums_R1Dn_NoLens_SIMD );
    }
  }
  else
  {
    if ( !m_Rich2UseGrandModule )
    {
      m_getModuleNumsSIMD = ( side() == Rich::left ?
                              &DeRichPMTPanel::getModuleNums_R2Le_Small_SIMD :
                              &DeRichPMTPanel::getModuleNums_R2Ri_Small_SIMD );
    }
    else
    {
      if ( !m_Rich2UseMixedModule )
      {
        m_getModuleNumsSIMD = ( side() == Rich::left ?
                                &DeRichPMTPanel::getModuleNums_R2Le_Grand_SIMD :
                                &DeRichPMTPanel::getModuleNums_R2Ri_Grand_SIMD);
      }
      else
      {
        m_getModuleNumsSIMD = ( side() == Rich::left ?
                                &DeRichPMTPanel::getModuleNums_R2Le_Mixed_SIMD :
                                &DeRichPMTPanel::getModuleNums_R2Ri_Mixed_SIMD );
      }
    }
  }

  // make SIMD caches for various quantities

  m_Rich1LensDemagnificationFactorSIMD = m_Rich1LensDemagnificationFactor;
  m_GrandPmtAnodeXEdgeSIMD             = m_GrandPmtAnodeXEdge;
  m_GrandPmtAnodeYEdgeSIMD             = m_GrandPmtAnodeYEdge;
  m_PmtAnodeXEdgeSIMD                  = m_PmtAnodeXEdge;
  m_PmtAnodeYEdgeSIMD                  = m_PmtAnodeYEdge;

  for ( const auto i : {0,1} )
  {
    m_NumGrandPmtInRowColSIMD[i] = m_NumGrandPmtInRowCol[i];
    m_NumPmtInRowColSIMD[i]      = m_NumPmtInRowCol[i];
  }
  for ( const auto i : {0,1,2,3} )
  {
    m_RichPmtNumModulesInRowColSIMD[i]      = m_RichPmtNumModulesInRowCol[i];
    m_RichPmtModuleCopyNumBeginPanelSIMD[i] = m_RichPmtModuleCopyNumBeginPanel[i];
    m_RichPmtModuleCopyNumEndPanelSIMD[i]   = m_RichPmtModuleCopyNumEndPanel[i];
  }
  for ( const auto i : m_Rich2MixedModuleArrayColumnSize )
  {
    m_Rich2MixedModuleArrayColumnSizeSIMD.emplace_back( i );
  }

  // transform does not like direct assignent from scalar version :(
  {
    double xx{0}, xy{0}, xz{0}, dx{0}, yx{0}, yy{0};
    double yz{0}, dy{0}, zx{0}, zy{0}, zz{0}, dz{0};

    // Set to local matrix
    geometry()->toLocalMatrix().GetComponents( xx, xy, xz,
                                               dx, yx, yy,
                                               yz, dy, zx,
                                               zy, zz, dz );
    m_toLocalMatrixSIMD.SetComponents( xx, xy, xz,
                                       dx, yx, yy,
                                       yz, dy, zx,
                                       zy, zz, dz );
    // and to global
    geometry()->toGlobalMatrix().GetComponents( xx, xy, xz,
                                                dx, yx, yy,
                                                yz, dy, zx,
                                                zy, zz, dz );
    m_toGlobalMatrixSIMD.SetComponents( xx, xy, xz,
                                        dx, yx, yy,
                                        yz, dy, zx,
                                        zy, zz, dz );

  }

  m_localPlaneNormalSIMD = m_localPlaneNormal;

  {
    const auto & p = m_localPlane;
    m_localPlaneSIMD = Rich::SIMD::Plane<Rich::SIMD::DefaultScalarFP>( p.A(), p.B(), p.C(), p.D() );
  }

  return sc;
}

bool DeRichPMTPanel::smartID( const Gaudi::XYZPoint& globalPoint,
                              LHCb::RichSmartID& id ) const
{
  id = m_panelID; // sets RICH, panel and type
  const Rich::SIMD::Point<FP> simdp( globalPoint );
  const auto a = findPMTArraySetupSIMD(simdp);
  // get PMT module number in panel
  setRichPmtSmartID( PmtModuleNumInPanelFromModuleNumAlone((a[0])[0]), (a[1])[0], (a[2])[0], (a[3])[0], id );
  //setRichPmtSmartID( a[0], a[1], a[2], a[3], id );
  return true;
}

StatusCode DeRichPMTPanel::getPanelGeometryInfo()
{
  StatusCode sc = StatusCode::SUCCESS;

  auto * firstRich = getFirstDeRich();
  if ( !firstRich ) { return Error("Cannot locate first RICH detector"); }

  m_RichPmtNumModulesInRowCol[0]=1;
  m_RichPmtNumModulesInRowCol[1]=1;

  // keep some the RICH1 papameters inside the if block
  if ( rich() == Rich::Rich1 )
  {
    m_PmtModulePlaneHalfSizeR1SIMD =
      toarray<SIMDFP,4>( firstRich->param<std::vector<double> >("Rich1PMTModulePlaneHalfSize") );
    const auto aRich1PmtNumModulesInRow = firstRich->param<int> ("Rich1NumberOfModulesInRow" );
    const auto aRich1PmtNumModulesInCol = firstRich->param<int> ("Rich1NumberOfModulesInCol" );
    m_RichPmtNumModulesInRowCol[0] = aRich1PmtNumModulesInRow;
    m_RichPmtNumModulesInRowCol[1] = aRich1PmtNumModulesInCol;
  }
  m_Rich2UseGrandModule = false;
  m_Rich2UseMixedModule = false;
  m_Rich2ArrayConfig    = 0;
  if ( firstRich->exists("Rich2PMTArrayConfig") )
  {
    m_Rich2ArrayConfig = firstRich->param<int>("Rich2PMTArrayConfig");
    if ( m_Rich2ArrayConfig >= 1 )
    {
      m_Rich2UseGrandModule = true;
      if ( m_Rich2ArrayConfig == 2 )
      {
        m_Rich2UseMixedModule = true;
      }
    }
  }

  m_PmtModulePlaneHalfSizeR2SIMD =
    toarray<SIMDFP,4>( firstRich->param<std::vector<double> >("Rich2PMTModulePlaneHalfSize") );
  const auto PmtModulePitch = firstRich->param<double>("RichPmtModulePitch");
  m_PmtModulePitchInvSIMD = ( fabs(PmtModulePitch)>0 ? 1.0/PmtModulePitch : 0.0 );
  const auto aRich2PmtNumModulesInRow = firstRich->param<int> ( "Rich2NumberOfModulesInRow" );
  const auto aRich2PmtNumModulesInCol = firstRich->param<int> ( "Rich2NumberOfModulesInCol" );
  m_RichPmtNumModulesInRowCol[2] = aRich2PmtNumModulesInRow;
  m_RichPmtNumModulesInRowCol[3] = aRich2PmtNumModulesInCol;

  // info()<< "DeRichPmtplane  getPanelGeometryInfo  m_RichPmtNumModulesInRowCol "
  //       <<  m_RichPmtNumModulesInRowCol <<endmsg;

  m_RichPmtModuleCopyNumBeginPanel =
    toarray<Int,4>( firstRich->param<std::vector<int> > ("RichPmtModuleNumBeginInPanels") );
  m_RichPmtModuleCopyNumEndPanel =
    toarray<Int,4>( firstRich->param<std::vector<int> > ("RichPmtModuleNumEndInPanels") );
  m_RichPmtModuleActiveAreaHalfSizeSIMD =
    toarray<SIMDFP,2>( firstRich->param<std::vector<double> >("RichPMTModuleActiveAreaHalfSize") );

  const auto PmtPitch = firstRich->param<double>("RichPmtPitch");
  m_PmtPitchInvSIMD = ( fabs(PmtPitch)>0 ? 1.0/PmtPitch : 0.0 );

  m_NumPmtInRowCol[0]  = firstRich->param<int> ("RichPmtNumInModuleRow");
  m_NumPmtInRowCol[1]  = firstRich->param<int> ("RichPmtNumInModuleCol");
  m_NumPmtInRichModule = firstRich->param<int> ("RichTotNumPmtInModule");

  const auto aRich1NumModules = firstRich->param<int> ("Rich1TotNumModules" );
  const auto aRich2NumModules = firstRich->param<int> ("Rich2TotNumModules" );
  m_Rich2TotNumStdModules = aRich2NumModules;
  m_totNumPmtModuleInRich = aRich1NumModules + aRich2NumModules;

  m_NumPmtModuleInRich[0]=aRich1NumModules/2; //rich1top
  m_NumPmtModuleInRich[1]=aRich1NumModules/2; //rich1bottom
  m_NumPmtModuleInRich[2]=aRich2NumModules/2; //rich2left
  m_NumPmtModuleInRich[3]=aRich2NumModules/2; //rich2right

  m_PmtAnodeXSize = firstRich->param<double>("RichPmtAnodeXSize");
  m_PmtAnodeYSize = firstRich->param<double>("RichPmtAnodeYSize");

  m_PmtPixelGap = firstRich->param<double>("RichPmtPixelGap");
  m_PmtPixelsInRowSIMD = firstRich->param<int>("RichPmtNumPixelCol");
  m_PmtPixelsInColSIMD = firstRich->param<int>("RichPmtNumPixelRow");
  m_PmtAnodeXEdge = -0.5*(m_PmtAnodeXSize+m_PmtPixelGap);
  m_PmtAnodeYEdge = -0.5*(m_PmtAnodeYSize+m_PmtPixelGap);
  m_AnodeXPixelSize = firstRich->param<double> ("RichPmtPixelXSize");
  m_AnodeYPixelSize = firstRich->param<double> ("RichPmtPixelYSize");
  const auto PmtAnodeEffectiveXPixelSize = m_AnodeXPixelSize+m_PmtPixelGap;
  const auto PmtAnodeEffectiveYPixelSize = m_AnodeYPixelSize+m_PmtPixelGap;
  m_PmtAnodeEffectiveXPixelSizeInvSIMD = 1.0/PmtAnodeEffectiveXPixelSize;
  m_PmtAnodeEffectiveYPixelSizeInvSIMD = 1.0/PmtAnodeEffectiveYPixelSize;
  m_PmtMasterLateralSize = firstRich->param<double>("RichPmtMasterLateralSize");
  const auto Rich1TotNumPmts = firstRich->param<int>("Rich1TotNumPmt");
  const auto Rich2TotNumPmts = firstRich->param<int>("Rich2TotNumPmt");
  m_totNumPMTs = Rich1TotNumPmts + Rich2TotNumPmts;
  m_Rich2TotNumStdPmts = Rich2TotNumPmts ;
  m_RichPmtQuartzThickness = firstRich->param<double>("RichPmtQuartzZSize" );
  m_RichPmtQuartzLocalZInPmt= firstRich->param<double>("RichPmtQuartzZPosInPmt");

  if ( m_Rich2UseGrandModule )
  {
    if ( firstRich->exists("Rich2GrandPMTModulePlaneHalfSize") )
    {

      m_GrandPmtModulePlaneHalfSizeR2SIMD = toarray<SIMDFP,4>( firstRich->param<std::vector<double> >("Rich2GrandPMTModulePlaneHalfSize") );
      const auto GrandPmtModulePitch = firstRich->param<double>("RichGrandPmtModulePitch");
      m_GrandPmtModulePitchInvSIMD = ( fabs(GrandPmtModulePitch)>0 ? 1.0/GrandPmtModulePitch : 0.0 );
      m_RichGrandPmtModuleActiveAreaHalfSizeSIMD =
        toarray<SIMDFP,2>( firstRich->param<std::vector<double> >("RichGrandPMTModuleActiveAreaHalfSize") );
      const auto GrandPmtPitch = firstRich->param<double>("RichGrandPmtPitch");
      m_GrandPmtPitchInvSIMD = ( fabs(GrandPmtPitch)>0 ? 1.0/GrandPmtPitch : 0.0 );

      m_Rich2TotNumGrandModules=  firstRich->param<int> ("Rich2TotNumGrandModules" );
      m_Rich2TotNumStdModules = 0;
      m_totNumPmtModuleInRich = aRich1NumModules + m_Rich2TotNumGrandModules;
      m_NumPmtModuleInRich[2] = m_Rich2TotNumGrandModules/2; //rich2left
      m_NumPmtModuleInRich[3] = m_Rich2TotNumGrandModules/2; //rich2right


      m_GrandPmtAnodeXSize = firstRich->param<double>("RichGrandPmtAnodeXSize");
      m_GrandPmtAnodeYSize = firstRich->param<double>("RichGrandPmtAnodeYSize");

      m_GrandPmtPixelGap = firstRich->param<double>("RichGrandPmtPixelGap" );
      m_GrandPmtPixelsInRowSIMD = firstRich->param<int>("RichGrandPmtNumPixelCol" );
      m_GrandPmtPixelsInColSIMD = firstRich->param<int>("RichGrandPmtNumPixelRow" );
      m_GrandPmtAnodeXEdge = -0.5*(m_GrandPmtAnodeXSize+m_GrandPmtPixelGap);
      m_GrandPmtAnodeYEdge = -0.5*(m_GrandPmtAnodeYSize+m_GrandPmtPixelGap);
      m_GrandAnodeXPixelSize = firstRich->param<double> ("RichGrandPmtPixelXSize" );
      m_GrandAnodeYPixelSize = firstRich->param<double> ("RichGrandPmtPixelYSize" );
      const auto GrandPmtAnodeEffectiveXPixelSize = m_GrandAnodeXPixelSize+m_GrandPmtPixelGap;
      const auto GrandPmtAnodeEffectiveYPixelSize = m_GrandAnodeYPixelSize+m_GrandPmtPixelGap;
      m_GrandPmtAnodeEffectiveXPixelSizeInvSIMD = 1.0/GrandPmtAnodeEffectiveXPixelSize;
      m_GrandPmtAnodeEffectiveYPixelSizeInvSIMD = 1.0/GrandPmtAnodeEffectiveYPixelSize;
      m_GrandPmtMasterLateralSize = firstRich->param<double>("RichGrandPmtMasterLateralSize" );
      m_GrandNumPmtInRichModule = firstRich->param<int> ("RichTotNumGrandPmtInModule");

      m_NumGrandPmtInRowCol[0] = firstRich->param<int> ("RichGrandPmtNumInModuleRow");
      m_NumGrandPmtInRowCol[1] = firstRich->param<int> ("RichGrandPmtNumInModuleCol");
      m_Rich2TotNumGrandPmts = firstRich->param<int>  ( "Rich2TotNumGrandPmt"    );
      m_Rich2TotNumStdPmts=0;
      if ( m_Rich2UseMixedModule )
      {
        if ( firstRich->exists("Rich2MixedPMTModulePlaneHalfSize") )
        {
          m_MixedPmtModulePlaneHalfSizeR2SIMD =
            toarray<SIMDFP,4>( firstRich->param<std::vector<double> >("Rich2MixedPMTModulePlaneHalfSize") );
          m_MixedStdPmtModulePlaneHalfSizeR2SIMD =
            toarray<SIMDFP,4>( firstRich->param<std::vector<double> >("Rich2MixedStdPMTModulePlaneHalfSize") );
          m_Rich2TotNumStdModules = firstRich->param<int> ("Rich2TotNumStdModules" );
          m_totNumPmtModuleInRich = aRich1NumModules + m_Rich2TotNumGrandModules+m_Rich2TotNumStdModules;
          m_NumPmtModuleInRich[2] = (m_Rich2TotNumGrandModules+m_Rich2TotNumStdModules)/2; //rich2left
          m_NumPmtModuleInRich[3] = (m_Rich2TotNumGrandModules+m_Rich2TotNumStdModules)/2; //rich2right
          m_Rich2TotNumStdPmts = firstRich->param<int>( "Rich2TotNumStdPmt" );
          if ( firstRich->exists("Rich2MixedNumModulesArraySetup") )
          {
            m_Rich2MixedModuleArrayColumnSize = firstRich ->param<std::vector<int> >("Rich2MixedNumModulesArraySetup");
          }
        }

      }

    }

  }

  // Setup flags for large PMTs
  RichSetupMixedSizePmtModules();

  // setup the Lens Flag.
  m_totNumPmtModuleInRich1 = firstRich->param<int> ("Rich1TotNumModules" );

  m_Rich1PmtLensPresence = false;
  m_Rich1PmtLensModuleCol.clear();

  if ( exists("Rich1PmtLensPresence") )
  {
    m_Rich1PmtLensPresence = ( 0 < firstRich->param<int>("Rich1PmtLensPresence") );
  }

  if ( m_Rich1PmtLensPresence )
  {

    m_Rich1PmtLensModuleCol = firstRich->param<std::vector<int> >("Rich1PmtLensModuleColumns");
    m_PmtMasterWithLensLateralSizeSIMD = firstRich->param<double>("RichLensPmtMasterLateralSize" );
    const auto PmtModuleWithLensPitch = firstRich->param<double>("RichLensPmtModulePitch");
    m_PmtModuleWithLensPitchInvSIMD = ( fabs(PmtModuleWithLensPitch)>0 ? 1.0/PmtModuleWithLensPitch : 0.0 );
    m_PmtLensPitch=firstRich->param<double>("RichLensPmtPitch");
    m_Rich1PmtPanelWithLensXSizeSIMD = toarray<SIMDFP,2>( firstRich->param<std::vector<double> > ("Rich1PMTModuleLensPlaneXEdge") );
    m_Rich1PmtPanelWithLensYSizeSIMD = toarray<SIMDFP,2>( firstRich->param<std::vector<double> > ("Rich1PMTModuleLensPlaneYEdge") );

    m_Rich1PmtPanelWithLensColSizeSIMD = toarray<SIMDINT32,2>( firstRich->param<std::vector<int> >("Rich1PmtLensModuleColumnsSize")  );

    m_RichNumLensPmtinModuleRowColSIMD[0] = firstRich->param<int>( "RichLensPmtNumInModuleRow" );
    m_RichNumLensPmtinModuleRowColSIMD[1] = firstRich->param<int>( "RichLensPmtNumInModuleCol" );
    const auto Rich1LensMagnificationFactor = firstRich->param<double> ("RichPmtLensMagnficationFactor" );
    m_Rich1LensDemagnificationFactor = ( fabs(Rich1LensMagnificationFactor) > 0 ?
                                         1.0/Rich1LensMagnificationFactor : 0.0 );

    Rich1SetupPMTModulesWithLens();

  }
  else
  {
    std::fill( m_RichPmtModuleLensFlag.begin(),
               m_RichPmtModuleLensFlag.end(), false );
  }

  return sc;
}

void DeRichPMTPanel::Rich1SetupPMTModulesWithLens()
{

  const auto aRowR1 = 2*m_RichPmtNumModulesInRowCol[0];
  const auto aColR1 = m_RichPmtNumModulesInRowCol[1];
  m_RichPmtModuleLensFlag.clear();
  m_RichPmtModuleLensFlag.reserve(aRowR1*aColR1);
  m_RichPmtModuleLensFlag.resize(aRowR1*aColR1);

  //m_RichPmtModuleLensFlag.assign(aRowR1*aColR1, false);

  for ( Int r = 0; r < aRowR1; ++r )
  {
    for ( Int c = 0; c < aColR1; ++c )
    {
      const auto m=(r*aColR1)+c;
      m_RichPmtModuleLensFlag[m] = false;
      if ( !m_Rich1PmtLensModuleCol.empty() )
      {
        const auto location = std::find(m_Rich1PmtLensModuleCol.begin(),
                                        m_Rich1PmtLensModuleCol.end(), c);
        if ( location != m_Rich1PmtLensModuleCol.end() ) m_RichPmtModuleLensFlag[m] = true;
      }

    }

  }

}

void DeRichPMTPanel::RichSetupMixedSizePmtModules()
{
  m_ModuleIsWithGrandPMT.assign(m_totNumPmtModuleInRich,false);

  // Now set the flags for module according to the geometry setup

  if ( m_Rich2UseGrandModule )
  {

    const auto aTotNumModInRich1 = m_NumPmtModuleInRich[0] + m_NumPmtModuleInRich[1];

    for ( Int im = aTotNumModInRich1; im < m_totNumPmtModuleInRich; ++im )
    {
      if ( m_Rich2ArrayConfig == 1 )
      {
        m_ModuleIsWithGrandPMT[im] = true;
      }
      else if ( m_Rich2ArrayConfig == 2 )
      {
        Int imLocal = im - aTotNumModInRich1;
        if (imLocal > m_NumPmtModuleInRich[2] ) imLocal -= m_NumPmtModuleInRich[2];
        const Int aColNum = imLocal/(m_RichPmtNumModulesInRowCol[3]);
        const Int aRowNum = imLocal - aColNum*(m_RichPmtNumModulesInRowCol[3]);
        if ( aRowNum <  m_Rich2MixedModuleArrayColumnSize[0] ||
             aRowNum >= ( m_Rich2MixedModuleArrayColumnSize[0] + m_Rich2MixedModuleArrayColumnSize[1] ) )
        {
          m_ModuleIsWithGrandPMT[im] = true;
        }
      }

    } // end loop over modules in rich2

  } // end if rich2useGrandModule

}

//=========================================================================
// Gets the PMT information (SIMD)
//=========================================================================
DeRichPMTPanel::ArraySetupSIMD
DeRichPMTPanel::findPMTArraySetupSIMD( const Rich::SIMD::Point<FP>& aGlobalPoint,
                                       const Rich::SIMD::Point<FP>& aLocalPoint ) const
{
  using GP = Gaudi::XYZPoint;

  ArraySetupSIMD aCh{{}};

  const auto x = aLocalPoint.x();
  const auto y = aLocalPoint.y();

  ModuleNumbersSIMD nums;
  getModuleNumsSIMD( x, y, nums );

  if ( UNLIKELY( any_of( nums.aModuleNum < SIMDINT32::Zero() ) ) )
  {
    error() << "DeRichPmtPanel : findPMTArraySetupSIMD : Problem getting PMT numbers" << endmsg;
    return aCh;
  }

  // Scalar loop
  SIMDFP xp,yp;
  for ( std::size_t i = 0; i < SIMDINT32::Size; ++i )
  {
    const auto sc_inModule =
      m_DePMTModules[nums.aModuleNumInPanel[i]]->toLocalMatrix() * GP{ aGlobalPoint.x()[i],
                                                                       aGlobalPoint.y()[i],
                                                                       aGlobalPoint.z()[i] };
    xp[i] = sc_inModule.x();
    yp[i] = sc_inModule.y();
  }

  SIMDINT32 aPmtCol = -SIMDINT32::One();
  SIMDINT32 aPmtRow = -SIMDINT32::One();
  auto &    aPmtNum = aCh[1];

  if ( any_of(nums.aModuleWithLens) )
  {
    aPmtCol = LHCb::SIMD::simd_cast<SIMDINT32>( abs( (xp-(SIMDFP(0.5)*m_PmtMasterWithLensLateralSizeSIMD))*m_PmtModuleWithLensPitchInvSIMD ) );
    aPmtRow = LHCb::SIMD::simd_cast<SIMDINT32>( abs( (yp-(SIMDFP(0.5)*m_PmtMasterWithLensLateralSizeSIMD))*m_PmtModuleWithLensPitchInvSIMD ) );
    aPmtNum = getLensPmtNumFromRowCol(aPmtRow,aPmtCol);
  }

  const auto antim = !nums.aModuleWithLens;
  const SIMDINT32::MaskType r2mask( rich() == Rich::Rich2 );
  const auto gmask = r2mask && ModuleIsWithGrandPMT(nums.aModuleNum);

  const auto am = antim && gmask;
  if ( any_of(am) )
  {
    aPmtCol(am) = LHCb::SIMD::simd_cast<SIMDINT32>( abs( (xp-m_RichGrandPmtModuleActiveAreaHalfSizeSIMD[0])*m_GrandPmtPitchInvSIMD ) );
    aPmtRow(am) = LHCb::SIMD::simd_cast<SIMDINT32>( abs( (yp-m_RichGrandPmtModuleActiveAreaHalfSizeSIMD[1])*m_GrandPmtPitchInvSIMD ) );
    aPmtNum(am) = getGrandPmtNumFromRowCol(aPmtRow,aPmtCol);
  }
  const auto bm = antim && !gmask;
  if ( any_of(bm) )
  {
    aPmtCol(bm) = LHCb::SIMD::simd_cast<SIMDINT32>( abs( (xp-m_RichPmtModuleActiveAreaHalfSizeSIMD[0])*m_PmtPitchInvSIMD ) );
    aPmtRow(bm) = LHCb::SIMD::simd_cast<SIMDINT32>( abs( (yp-m_RichPmtModuleActiveAreaHalfSizeSIMD[1])*m_PmtPitchInvSIMD ) );
    aPmtNum(bm) = getPmtNumFromRowCol(aPmtRow,aPmtCol);
  }

  if ( UNLIKELY( any_of( aPmtNum < SIMDINT32::Zero() ) ) )
  {
    error() << "DeRichPmtPanel : findPMTArraySetupSIMD : Problem getting PMT numbers" << endmsg;
    return aCh;
  }

  aCh[0] = nums.aModuleNum;

  // Scalar loop
  SIMDFP xpi,ypi;
  for ( std::size_t i = 0; i < SIMDINT32::Size; ++i )
  {
    const auto sc_inPmtAnode =
      (m_DePMTAnodes[nums.aModuleNumInPanel[i]][aPmtNum[i]])->toLocalMatrix() * GP{ aGlobalPoint.x()[i],
                                                                                    aGlobalPoint.y()[i],
                                                                                    aGlobalPoint.z()[i] };
    xpi[i] = sc_inPmtAnode.x();
    ypi[i] = sc_inPmtAnode.y();
  }

  const auto mm = LHCb::SIMD::simd_cast<SIMDFP::MaskType>(nums.aModuleWithLens);
  if ( any_of(mm) )
  {
    xpi(mm) *= m_Rich1LensDemagnificationFactorSIMD;
    ypi(mm) *= m_Rich1LensDemagnificationFactorSIMD;
  }

  auto & aPmtPixelCol = aCh[2];
  auto & aPmtPixelRow = aCh[3];

  if ( any_of(gmask) )
  {
    aPmtPixelCol(gmask) = LHCb::SIMD::simd_cast<SIMDINT32>( abs((xpi - m_GrandPmtAnodeXEdgeSIMD)*m_GrandPmtAnodeEffectiveXPixelSizeInvSIMD));
    aPmtPixelRow(gmask) = LHCb::SIMD::simd_cast<SIMDINT32>( abs((ypi - m_GrandPmtAnodeYEdgeSIMD)*m_GrandPmtAnodeEffectiveYPixelSizeInvSIMD));
    aPmtPixelCol( gmask && aPmtPixelCol >=  m_GrandPmtPixelsInColSIMD ) = m_GrandPmtPixelsInColSIMD - SIMDINT32::One();
    aPmtPixelRow( gmask && aPmtPixelRow >=  m_GrandPmtPixelsInRowSIMD ) = m_GrandPmtPixelsInRowSIMD - SIMDINT32::One();
  }
  if ( any_of(!gmask) )
  {
    aPmtPixelCol(!gmask) = LHCb::SIMD::simd_cast<SIMDINT32>( abs((xpi - m_PmtAnodeXEdgeSIMD)*m_PmtAnodeEffectiveXPixelSizeInvSIMD));
    aPmtPixelRow(!gmask) = LHCb::SIMD::simd_cast<SIMDINT32>( abs((ypi - m_PmtAnodeYEdgeSIMD)*m_PmtAnodeEffectiveYPixelSizeInvSIMD));
    aPmtPixelCol( !gmask && aPmtPixelCol >= m_PmtPixelsInColSIMD ) = m_PmtPixelsInColSIMD - SIMDINT32::One();
    aPmtPixelRow( !gmask && aPmtPixelRow >= m_PmtPixelsInRowSIMD ) = m_PmtPixelsInRowSIMD - SIMDINT32::One();
  }

  aPmtPixelCol.setZero( aPmtPixelCol < SIMDINT32::Zero() );
  aPmtPixelRow.setZero( aPmtPixelRow < SIMDINT32::Zero() );

  return aCh;
}

//=========================================================================
// returns the (SIMD) intersection point with the detection plane
//=========================================================================
DeRichPMTPanel::SIMDRayTResult::Results
DeRichPMTPanel::detPlanePointSIMD( const Rich::SIMD::Point<FP>& pGlobal,
                                   const Rich::SIMD::Vector<FP>& vGlobal,
                                   Rich::SIMD::Point<FP>& hitPosition,
                                   SIMDRayTResult::SmartIDs& smartID,
                                   SIMDRayTResult::PDs& PDs,
                                   const LHCb::RichTraceMode mode ) const
{
  // results to return
  SIMDRayTResult::Results res;

  // define a dummy point and fill correctly later.
  Rich::SIMD::Point<FP> panelIntersection(0,0,0);

  // panel intersection
  const auto mask = getPanelInterSection( pGlobal, vGlobal, panelIntersection );
  if ( UNLIKELY( none_of(mask) ) ) { return res; }

  // set hit position to plane intersection in global frame
  hitPosition = m_toGlobalMatrixSIMD * panelIntersection;

  // get the PMT info (SIMD)
  const auto aC = findPMTArraySetupSIMD(hitPosition,panelIntersection);

  // get PMT module number in panel
  const auto pdNumInPanel = PmtModuleNumInPanelFromModuleNumAlone(aC[0]);

  // in panel mask
  const auto pmask = isInPmtPanel(panelIntersection);

  // Resort to a scalar loop at this point. To be improved...
  for ( std::size_t i = 0; i < SIMDFP::Size; ++i )
  {
    if ( mask[i] )
    {
      // get the DePMT object
      const auto pmt = m_DePMTs[ pdNumInPanel[i] ][ aC[1][i] ];
      PDs[i] = pmt;

      // Set the SmartID to the PD ID
      smartID[i] = pmt->pdSmartID();
      // set the pixel parts
      setRichPmtSmartIDPix( aC[2][i], aC[3][i], smartID[i] );

      // set final status
      res[i] = ( mode.detPlaneBound() == LHCb::RichTraceMode::DetectorPlaneBoundary::RespectPDPanel ?
                 pmask[i] ? LHCb::RichTraceMode::RayTraceResult::InPDPanel : LHCb::RichTraceMode::RayTraceResult::OutsidePDPanel :
                 LHCb::RichTraceMode::RayTraceResult::InPDPanel );
    }
  }

  return res;
}

//=========================================================================
// Returns the intersection point with the detector plane given a vector and a point.
//=========================================================================
LHCb::RichTraceMode::RayTraceResult
DeRichPMTPanel::detPlanePoint( const Gaudi::XYZPoint& pGlobal,
                               const Gaudi::XYZVector& vGlobal,
                               Gaudi::XYZPoint& hitPosition,
                               LHCb::RichSmartID& smartID,
                               const DeRichPD*& pd,
                               const LHCb::RichTraceMode mode ) const
{
  // Use the SIMD method
  // Note this will not be as efficient as properly using the SIMD methods,
  // but eventually the scalar calls should be fully depreciated anyway so this
  // is just a short term placeholder.

  using VcP = Rich::SIMD::Point<FP>;
  using VcV = Rich::SIMD::Vector<FP>;

  // form some temporary objects
  SIMDRayTResult::SmartIDs simdIDs;
  simdIDs.fill(smartID);
  VcP simdHitPoint(hitPosition);
  SIMDRayTResult::PDs PDs;
  PDs.fill(pd);

  // Call the SIMD method
  const auto simdRes = detPlanePointSIMD( VcP(pGlobal), VcV(vGlobal),
                                          simdHitPoint, simdIDs, PDs, mode );

  // copy results back to scalars. All entries are the same so use [0]
  hitPosition = { simdHitPoint.x()[0], simdHitPoint.y()[0], simdHitPoint.z()[0] };
  pd = PDs[0];
  smartID = simdIDs[0];

  // return
  return LHCb::RichTraceMode::RayTraceResult(simdRes[0]);
}

//=========================================================================
// find an intersection with the PMT window (SIMD)
//=========================================================================
DeRichPMTPanel::SIMDRayTResult::Results
DeRichPMTPanel::PDWindowPointSIMD( const Rich::SIMD::Point<FP>& pGlobal,
                                   const Rich::SIMD::Vector<FP>& vGlobal,
                                   Rich::SIMD::Point<FP>& hitPosition,
                                   SIMDRayTResult::SmartIDs& smartID,
                                   SIMDRayTResult::PDs& PDs,
                                   const LHCb::RichTraceMode mode ) const
{
  using GP = Gaudi::XYZPoint;

  // results to return
  SIMDRayTResult::Results res;

  // define a dummy point and fill correctly later.
  Rich::SIMD::Point<FP> panelIntersection(0,0,0);

  // panel intersection
  auto mask = getPanelInterSection( pGlobal, vGlobal, panelIntersection );
  if ( UNLIKELY( none_of(mask) ) ) { return res; }

  // sets RICH, panel and type
  smartID.fill( m_panelID );

  // set results to outside panel
  res( LHCb::SIMD::simd_cast<SIMDRayTResult::Results::MaskType>(mask) ) =
    SIMDRayTResult::Results(LHCb::RichTraceMode::RayTraceResult::OutsidePDPanel);

  // are we in the panel ?
  mask &= isInPmtPanel(panelIntersection);
  if ( any_of(mask) )
  {
    // Set res flag for those in mask to InPDPanel
    res( LHCb::SIMD::simd_cast<SIMDRayTResult::Results::MaskType>(mask) ) =
      SIMDRayTResult::Results(LHCb::RichTraceMode::RayTraceResult::InPDPanel);

    // set hit position to plane intersection in global frame
    hitPosition = m_toGlobalMatrixSIMD * panelIntersection;

    // check PD acceptance ?
    if ( mode.detPlaneBound() != LHCb::RichTraceMode::DetectorPlaneBoundary::IgnorePDAcceptance )
    {

      // get the PMT info (SIMD)
      const auto aC = findPMTArraySetupSIMD(hitPosition,panelIntersection);

        // get module in panel number
      const auto aModuleNumInPanel = PmtModuleNumInPanelFromModuleNumAlone(aC[0]);

      // large PMT flag
      const auto flagGrandPmt = ModuleIsWithGrandPMT(aC[0]);

      // Resort to a scalar loop at this point. To be improved...
      for ( std::size_t i = 0; i < SIMDFP::Size; ++i )
      {
        // skip those already failed
        if ( mask[i] )
        {

          // get the DePMT object
          const auto pmt = m_DePMTs[ aModuleNumInPanel[i] ][ aC[1][i] ];
          PDs[i] = pmt;

          // Set the SmartID to the PD ID
          smartID[i] = pmt->pdSmartID();
          // set the pixel parts
          setRichPmtSmartIDPix( aC[2][i], aC[3][i], smartID[i] );

          // coordinate in the PMT
          const auto coordinPmt = pmt->toLocalMatrix() * GP{ hitPosition.x()[i],
                                                             hitPosition.y()[i],
                                                             hitPosition.z()[i] };

          // check PMT acceptance
          if ( isInPmt( ( pmt->PmtLensFlag() ?
                          DemagnifyFromLens(coordinPmt) : coordinPmt ), flagGrandPmt[i] ) &&
               isInPmtAnodeLateralAcc(coordinPmt,flagGrandPmt[i]) )
          { res[i] = LHCb::RichTraceMode::RayTraceResult::InPDTube; }

        } // mask OK

      } // scalar loop

    }

  }

  // return
  return res;
}

//=========================================================================
// Returns the intersection with the PD window
//=========================================================================
LHCb::RichTraceMode::RayTraceResult
DeRichPMTPanel::PDWindowPoint( const Gaudi::XYZPoint& pGlobal,
                               const Gaudi::XYZVector& vGlobal,
                               Gaudi::XYZPoint& windowPointGlobal,
                               LHCb::RichSmartID& smartID,
                               const DeRichPD*& pd,
                               const LHCb::RichTraceMode mode ) const
{

  // Use the SIMD method
  // Note this will not be as efficient as properly using the SIMD methods,
  // but eventually the scalar calls should be fully depreciated anyway so this
  // is just a short term placeholder.

  using VcP = Rich::SIMD::Point<FP>;
  using VcV = Rich::SIMD::Vector<FP>;

  // form some temporary objects
  SIMDRayTResult::SmartIDs simdIDs;
  simdIDs.fill(smartID);
  VcP simdHitPoint(windowPointGlobal);
  SIMDRayTResult::PDs PDs;
  PDs.fill(pd);

  // Call the SIMD method
  const auto simdRes = PDWindowPointSIMD( VcP(pGlobal), VcV(vGlobal),
                                          simdHitPoint, simdIDs, PDs, mode );

  // copy results back to scalars. All entries are the same so use [0]
  windowPointGlobal = { simdHitPoint.x()[0], simdHitPoint.y()[0], simdHitPoint.z()[0] };
  pd = PDs[0];
  smartID = simdIDs[0];

  // return
  return LHCb::RichTraceMode::RayTraceResult(simdRes[0]);
}

Rich::DAQ::PDPanelIndex DeRichPMTPanel::pdNumber( const LHCb::RichSmartID& smartID ) const
{
  return _pdNumber( smartID );
}

const DeRichPD* DeRichPMTPanel::dePD( const LHCb::RichSmartID pdID ) const
{
  return dePMT(pdID);
}

const DeRichPD* DeRichPMTPanel::dePD( const Rich::DAQ::PDPanelIndex PmtNumber ) const
{
  return dePMT( PmtNumber );
}

const DeRichPMT* DeRichPMTPanel::dePMT( const Rich::DAQ::PDPanelIndex PmtNumber ) const
{
  const DeRichPMT * dePmt = nullptr;

  if ( PmtNumber.data() < m_totNumPMTs )
  {
    const auto Mnum = (unsigned int) (PmtNumber.data()/m_NumPmtInRichModule);
    const auto MNumInCurPanel = PmtModuleNumInPanelFromModuleNumAlone(Mnum);
    const auto Pnum = PmtNumber.data() - ( Mnum * m_NumPmtInRichModule );

    if ( UNLIKELY( MNumInCurPanel >= (Int)m_DePMTs.size() ||
                   Pnum >= m_DePMTs[MNumInCurPanel].size() ) )
    {
      std::ostringstream mess;
      mess << "DeRichPMTPanel::dePMT : Inappropriate PMT module and pmt numbers "
           << MNumInCurPanel << " " << Pnum;

      throw GaudiException( mess.str(), "*DeRichPMTPanel*", StatusCode::FAILURE );
    }
    else
    {
      dePmt = m_DePMTs [ MNumInCurPanel ] [ Pnum ];
    }

  }
  else
  {
    std::ostringstream mess;
    mess << "DeRichPMTPanel: Inappropriate PmtcopyNumber : " << PmtNumber;
    throw GaudiException( mess.str(), "*DeRichPMTPanel*", StatusCode::FAILURE );
  }

  return dePmt;
}

//  return a list with all the valid readout channels (smartIDs)
//=========================================================================
bool
DeRichPMTPanel::readoutChannelList ( LHCb::RichSmartID::Vector& readoutChannels ) const
{
  const auto aBeginM = m_RichPmtModuleCopyNumBeginPanel[m_CurPanelNum];
  const auto aEndM   = m_RichPmtModuleCopyNumEndPanel[m_CurPanelNum];

  for ( Int iM = aBeginM ; iM <= aEndM; ++iM )
  {
    for ( Int iP = 0; iP < m_NumPmtInRichModule; ++iP )
    {
      for ( Int iPx = 0; iPx < m_PmtPixelsInRowSIMD[0]; ++iPx )
      {
        for ( Int iPy = 0; iPy < m_PmtPixelsInColSIMD[0]; ++iPy )
        {
          readoutChannels.emplace_back( rich(), side(), iP, iM, iPy, iPx, m_pdType );
        }
      }
    }
  }

  return true;
}

int DeRichPMTPanel::sensitiveVolumeID(const Gaudi::XYZPoint& globalPoint) const
{
  // Create a RichSmartID for this RICH and panel
  const LHCb::RichSmartID def_id( rich(), side(), 0, 0, 0, 0, m_pdType );
  // set the remaining fields from the position
  auto id = def_id;
  return ( smartID(globalPoint,id) ? id : def_id );
  // From FTTU branch - return pdNumber
  //return pdNumber( smartID(globalPoint,id) ? id : def_id ).data();
}

//=========================================================================
//  setRichAndSide
//=========================================================================
bool DeRichPMTPanel::setRichAndSide()
{
  bool OK = true;

  const auto centreGlobal = geometry()->toGlobal(Gaudi::XYZPoint{0,0,0});

  // Work out what Rich/panel I am
  if ( name().find("Rich1") != std::string::npos )
  {
    setRich( Rich::Rich1 );
    setSide( centreGlobal.y() > 0.0 ? Rich::top : Rich::bottom );
  }
  // Rich2
  else if ( name().find("Rich2") != std::string::npos )
  {
    setRich( Rich::Rich2 );
    setSide( centreGlobal.x() > 0.0 ? Rich::left : Rich::right );
  }
  // Single Rich
  else if ( name().find("Rich/") != std::string::npos )
  {
    setRich( Rich::Rich );
    setSide( centreGlobal.x() > 0.0 ? Rich::left : Rich::right );
  }
  // problem
  else
  {
    error() << "Cannot identify Rich/side for " << name() << endmsg;
    OK = false;
  }

  if ( rich() == Rich::InvalidDetector ||
       side() == Rich::InvalidSide )
  {
    OK = false;
  }
  else
  {
    // cache the panel ID
    m_panelID = LHCb::RichSmartID( rich(), side(), m_pdType );
    // cache the panel idex
    m_CurPanelNum = ( rich() == Rich::Rich1 ?
                      ( side() == Rich::top  ? 0 : 1 ) :
                      ( side() == Rich::left ? 2 : 3 ) );
  }

  return OK;
}

DeRichPMTPanel::Int
DeRichPMTPanel::getModuleCopyNumber( const std::string& aModuleName )
{
  Int anumber = -1;
  const auto pos2 = aModuleName.find(":");
  if ( std::string::npos == pos2 )
  {
    error() << "A PMTModule without a number!   " << aModuleName << endmsg;
  }
  else
  {
    anumber = atoi( aModuleName.substr(pos2+1).c_str() );
  }

  return anumber;
}
