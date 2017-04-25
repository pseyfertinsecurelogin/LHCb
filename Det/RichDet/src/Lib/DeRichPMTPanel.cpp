
// STL
#include <sstream>
#include <time.h>
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
  StatusCode sc = StatusCode::SUCCESS;

  // store the name of the panel, without the /dd/Structure part
  const auto pos = name().find("Rich");
  setMyName( std::string::npos != pos ? name().substr(pos) : "DeRichPMTPanel_NO_NAME" );

  MsgStream msg ( msgSvc(), "DeRichPMTPanel" );

  if ( msgLevel(MSG::DEBUG,msg) )
    msg << MSG::DEBUG << "Initialize " << name() << endmsg;

  // set rich and side
  if ( !setRichPanelAndSide() ) return StatusCode::FAILURE;
  if ( rich() == Rich::InvalidDetector || side() == Rich::InvalidSide )
  {
    error() << "Error initializing PMT panel " << name() << " " << rich() << " " << side() << endmsg;
    return StatusCode::FAILURE;
  }

  sc = sc && getPanelGeometryInfo();
  if ( !sc ) return sc;

  // register UMS dependency on local geometry
  updMgrSvc()->registerCondition( this, geometry(), &DeRichPMTPanel::geometryUpdate );

  // get the pmtmodule and pmt  detector elements
  m_DePMTs.clear();
  m_DePMTModules.clear();
  m_DePMTAnodes.clear();

  const auto numCurModules = getNumModulesInThisPanel();

  m_DePMTModules.reserve(numCurModules);
  m_DePMTs.reserve(numCurModules);
  m_DePMTAnodes.reserve(numCurModules);

  const auto & detelems = childIDetectorElements();

  for ( auto det_it = detelems.begin(); det_it != detelems.end(); ++det_it )
  {
    //info()<<"  det it names  "<<( det_it - detelems.begin() ) <<"  "<<(*det_it)->name() <<endmsg;
    // info() << (*det_it)->name() << endmsg;

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
      m_DePMTModules.push_back( dePMTModule );
      const auto aCurrentModuleCopyNumber = getModuleCopyNumber( dePMTModule->name() );

      int aNumPmtInCurrentRichModule = (int) dePMTModule->childIDetectorElements().size();
      std::vector<DeRichPMT*>        DePmtsInCurModule(aNumPmtInCurrentRichModule,nullptr);
      std::vector<IDetectorElement*> DePmtAnodesInCurModule(aNumPmtInCurrentRichModule,nullptr);

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
                      //  IDetectorElement* dePmtAnode = (IDetectorElement*) (*det_it_pm_an);
                      // register UMS dependency
                      updMgrSvc()->registerCondition( this, dePmtAnode->geometry(),
                                                      &DeRichPMTPanel::geometryUpdate );
                      DePmtAnodesInCurModule[curPmtNum] = dePmtAnode;
                    }
                    else
                    {
                      msg << MSG::FATAL << "Non DeRichPMT Anode detector element "
                          << (*det_it_pm_an)->name() << endmsg;
                      return StatusCode::FAILURE;
                    }
                  }// end test anode name

                } // end loop over  anodes in a pmt. We assume there is only 1 det elem inside a pmt
                // which is the anode.
              }// end test on pt child det elem

            }
            else
            {      // end test existence of pmt
              msg << MSG::FATAL << "No DeRichPMT  detector element "
                  << (*det_it_pm)->name() << endmsg;
              return StatusCode::FAILURE;
            }

          }// end test pmt name

        } // end loop over pmts in  a module

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

  // trigger first UMS update
  const auto update = updMgrSvc()->update(this);

  //msg << MSG::DEBUG << "Panel Initialised" << endmsg;

  return update;
}

Rich::DAQ::HPDCopyNumber DeRichPMTPanel::maxPdNumber() const
{
  return m_maxPDCopyN;
}

//=========================================================================
//  getFirstDeRich
//=========================================================================
DetectorElement * DeRichPMTPanel::getFirstDeRich() const
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
  StatusCode sc =  StatusCode::SUCCESS;

  auto * firstRich = getFirstDeRich();
  if ( !firstRich ) return StatusCode::FAILURE;

  sc = sc && getPanelGeometryInfo();

  m_PDColumns  = param<int>("PMTColumns");   // pd colums in a panel, not used, may be phased out
  m_PDNumInCol = param<int>("PMTNumberInColumn"); //pd rows in  a panel. not used, may be phased out.
  //  m_PDMax = nPDColumns() * nPDsPerCol(); // total in a  panel.
  m_PDMax = getNumModulesInThisPanel() * m_NumPmtInRichModule;

  double aOffset = 0.0;
  Gaudi::XYZVector aDir(0.0,0.0,0.0);
  Gaudi::XYZPoint aPon(0.0,0.0,0.0);
  int sign = 0;

  if ( rich() == Rich::Rich1 )
  {

    sign = ( side() == Rich::top ? 1 : -1 );

    if ( side() == Rich::top )
    {
      const auto aPmtR1P0DetPlaneDirCos = firstRich->param<std::vector<double> >("Rich1TopPmtDetectorPlaneDirCos");
      const auto aPmtR1P0DetPlanePoint  = firstRich->param<std::vector<double> >("Rich1TopPmtDetPlanePoint");
      aOffset = fabs( m_PmtModulePlaneHalfSizeR1[1] );
      aDir = { aPmtR1P0DetPlaneDirCos[0], aPmtR1P0DetPlaneDirCos[1], aPmtR1P0DetPlaneDirCos[2] };
      aPon = { aPmtR1P0DetPlanePoint[0],  aPmtR1P0DetPlanePoint[1],  aPmtR1P0DetPlanePoint[2]  };
      m_xyHalfSize[0] = fabs( m_PmtModulePlaneHalfSizeR1[0] );
      m_xyHalfSize[1] = fabs( m_PmtModulePlaneHalfSizeR1[1] );
    }
    else if ( side () == Rich::bottom )
    {
      const auto aPmtR1P1DetPlaneDirCos = firstRich->param<std::vector<double> >("Rich1BotPmtDetectorPlaneDirCos");
      const auto aPmtR1P1DetPlanePoint  = firstRich->param<std::vector<double> >("Rich1BotPmtDetPlanePoint");
      aOffset = fabs( m_PmtModulePlaneHalfSizeR1[3] );
      aDir = { aPmtR1P1DetPlaneDirCos[0], aPmtR1P1DetPlaneDirCos[1], aPmtR1P1DetPlaneDirCos[2] };
      aPon = { aPmtR1P1DetPlanePoint[0],  aPmtR1P1DetPlanePoint[1],  aPmtR1P1DetPlanePoint[2]  };
      m_xyHalfSize[0] = fabs( m_PmtModulePlaneHalfSizeR1[2] );
      m_xyHalfSize[1] = fabs( m_PmtModulePlaneHalfSizeR1[3] );
    }

    m_detectionPlane = Gaudi::Plane3D(aDir, aPon);

    // Also create a plane at the exterior surface of the pmt quartz window.
    const auto aZShiftR1 = firstRich->param<double> ("Rich1PmtDetPlaneZInPmtPanel");
    const Gaudi::XYZPoint localpointP(0.0,0.0,aZShiftR1-m_RichPmtQuartzThickness);
    const Gaudi::XYZPoint localpointQ(0.0,100.0,aZShiftR1-m_RichPmtQuartzThickness);
    const Gaudi::XYZPoint localpointR(50.0,50.0,aZShiftR1-m_RichPmtQuartzThickness);
    const Gaudi::XYZPoint globalpointP(geometry()->toGlobal(localpointP));
    const Gaudi::XYZPoint globalpointQ(geometry()->toGlobal(localpointQ));
    const Gaudi::XYZPoint globalpointR(geometry()->toGlobal(localpointR));
    m_detectionPlane_exterior = Gaudi::Plane3D(globalpointP,globalpointQ,globalpointR);

  }
  else // if ( rich() == Rich::Rich2 || rich() == Rich::Rich )
  {

    //It does not seem to be straightforward,  to get the detection plane param
    //directly from the xmldb for RICH2. So create
    // the actual detection plane using the Z Shift wrt the current panel.
    const auto aZShiftR2 =  firstRich->param<double> ("Rich2PmtDetPlaneZInPmtPanel");
    // create three random points on the detection plane, which can define the plane.
    const Gaudi::XYZPoint localpointA(0.0,0.0,aZShiftR2);
    const Gaudi::XYZPoint localpointB(0.0,100.0,aZShiftR2);
    const Gaudi::XYZPoint localpointC(50.0,50.0,aZShiftR2);
    const Gaudi::XYZPoint globalpointA(geometry()->toGlobal(localpointA));
    const Gaudi::XYZPoint globalpointB(geometry()->toGlobal(localpointB));
    const Gaudi::XYZPoint globalpointC(geometry()->toGlobal(localpointC));
    m_detectionPlane = Gaudi::Plane3D(globalpointA,globalpointB,globalpointC);

    // Also create  a  second plane at the exterior surface of the PmtQuartzWindow.
    const Gaudi::XYZPoint localpointP(0.0,0.0,aZShiftR2-m_RichPmtQuartzThickness);
    const Gaudi::XYZPoint localpointQ(0.0,100.0,aZShiftR2-m_RichPmtQuartzThickness);
    const Gaudi::XYZPoint localpointR(50.0,50.0,aZShiftR2-m_RichPmtQuartzThickness);
    const Gaudi::XYZPoint globalpointP(geometry()->toGlobal(localpointP));
    const Gaudi::XYZPoint globalpointQ(geometry()->toGlobal(localpointQ));
    const Gaudi::XYZPoint globalpointR(geometry()->toGlobal(localpointR));
    m_detectionPlane_exterior = Gaudi::Plane3D(globalpointP,globalpointQ,globalpointR);

    aPon = globalpointA;

    // determine which panel
    const RowCol rci = ( side() == Rich::left ? RowCol{0,1} : RowCol{2,3} );

    sign = ( side() == Rich::left ? 1 : -1 );

    auto aOffsetX = ( !m_Rich2UseGrandModule ? fabs( m_PmtModulePlaneHalfSizeR2[rci[0]]      ) :
                      !m_Rich2UseMixedModule ? fabs( m_GrandPmtModulePlaneHalfSizeR2[rci[0]] ) :
                                               fabs( m_MixedPmtModulePlaneHalfSizeR2[rci[0]] ) );
    auto aOffsetY = ( !m_Rich2UseGrandModule ? fabs( m_PmtModulePlaneHalfSizeR2[rci[1]]      ) :
                      !m_Rich2UseMixedModule ? fabs( m_GrandPmtModulePlaneHalfSizeR2[rci[1]] ) :
                                               fabs( m_MixedPmtModulePlaneHalfSizeR2[rci[1]] ) );
    aOffset = aOffsetX;
    m_xyHalfSize[0] = aOffsetX;
    m_xyHalfSize[1] = aOffsetY;

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
  for ( const auto pd : deRichSys()->allPDRichSmartIDs() )
  {
    if ( pd.rich() == rich() && pd.panel() == side() )
    {
      const auto copyN =  _pdNumber( pd );
      if ( copyN > m_maxPDCopyN ) { m_maxPDCopyN = copyN; }
    }
  }

  return sc;
}

bool DeRichPMTPanel::smartID( const Gaudi::XYZPoint& globalPoint,
                              LHCb::RichSmartID& id ) const
{
  id = m_panelID; // sets RICH, panel and type
  setRichPmtSmartID( findPMTArraySetup(globalPoint), id ) ;
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
    m_PmtModulePlaneHalfSizeR1 = 
      toarray<double,4>( firstRich->param<std::vector<double> >("Rich1PMTModulePlaneHalfSize") );
    const auto aRich1PmtNumModulesInRow = firstRich->param<int> ("Rich1NumberOfModulesInRow" );
    const auto aRich1PmtNumModulesInCol = firstRich->param<int> ("Rich1NumberOfModulesInCol" );
    m_RichPmtNumModulesInRowCol[0] = aRich1PmtNumModulesInRow;
    m_RichPmtNumModulesInRowCol[1] = aRich1PmtNumModulesInCol;
  }
  m_Rich2UseGrandModule=false;
  m_Rich2UseMixedModule=false;
  m_Rich2ArrayConfig=0;
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
  
  m_PmtModulePlaneHalfSizeR2 = 
    toarray<double,4>( firstRich->param<std::vector<double> >("Rich2PMTModulePlaneHalfSize") );
  const auto PmtModulePitch = firstRich->param<double>("RichPmtModulePitch");
  m_PmtModulePitchInv = ( fabs(PmtModulePitch)>0 ? 1.0/PmtModulePitch : 0.0 );
  const auto aRich2PmtNumModulesInRow = firstRich->param<int> ( "Rich2NumberOfModulesInRow" );
  const auto aRich2PmtNumModulesInCol = firstRich->param<int> ( "Rich2NumberOfModulesInCol" );
  m_RichPmtNumModulesInRowCol[2]=aRich2PmtNumModulesInRow;
  m_RichPmtNumModulesInRowCol[3]=aRich2PmtNumModulesInCol;
  
  // info()<< "DeRichPmtplane  getPanelGeometryInfo  m_RichPmtNumModulesInRowCol "
  //       <<  m_RichPmtNumModulesInRowCol <<endmsg;
  
  m_RichPmtModuleCopyNumBeginPanel =
    toarray<int,4>( firstRich->param<std::vector<int> > ("RichPmtModuleNumBeginInPanels") );
  m_RichPmtModuleCopyNumEndPanel =
    toarray<int,4>( firstRich->param<std::vector<int> > ("RichPmtModuleNumEndInPanels") );
  m_RichPmtModuleActiveAreaHalfSize=
    toarray<double,2>( firstRich->param<std::vector<double> >("RichPMTModuleActiveAreaHalfSize") );
  
  const auto PmtPitch = firstRich->param<double>("RichPmtPitch");
  m_PmtPitchInv = ( fabs(PmtPitch)>0 ? 1.0/PmtPitch : 0.0 );
  
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
  
  m_PmtPixelGap = firstRich->param<double>("RichPmtPixelGap" );
  m_PmtPixelsInRow = firstRich->param<int>("RichPmtNumPixelCol" );
  m_PmtPixelsInCol = firstRich->param<int>("RichPmtNumPixelRow" );
  m_PmtAnodeXEdge = -0.5*(m_PmtAnodeXSize+m_PmtPixelGap);
  m_PmtAnodeYEdge = -0.5*(m_PmtAnodeYSize+m_PmtPixelGap);
  m_AnodeXPixelSize = firstRich->param<double> ("RichPmtPixelXSize" );
  m_AnodeYPixelSize = firstRich->param<double> ("RichPmtPixelYSize" );
  const auto PmtAnodeEffectiveXPixelSize = m_AnodeXPixelSize+m_PmtPixelGap;
  const auto PmtAnodeEffectiveYPixelSize = m_AnodeYPixelSize+m_PmtPixelGap;
  m_PmtAnodeEffectiveXPixelSizeInv = 1.0/PmtAnodeEffectiveXPixelSize;
  m_PmtAnodeEffectiveYPixelSizeInv = 1.0/PmtAnodeEffectiveYPixelSize;
  m_PmtMasterLateralSize = firstRich->param<double>("RichPmtMasterLateralSize" );
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
      
      m_GrandPmtModulePlaneHalfSizeR2 = toarray<double,4>( firstRich->param<std::vector<double> >("Rich2GrandPMTModulePlaneHalfSize") );
      const auto GrandPmtModulePitch = firstRich->param<double>("RichGrandPmtModulePitch");
      m_GrandPmtModulePitchInv = ( fabs(GrandPmtModulePitch)>0 ? 1.0/GrandPmtModulePitch : 0.0 );
      m_RichGrandPmtModuleActiveAreaHalfSize =
        toarray<double,2>( firstRich->param<std::vector<double> >("RichGrandPMTModuleActiveAreaHalfSize") );
      const auto GrandPmtPitch = firstRich->param<double>("RichGrandPmtPitch");
      m_GrandPmtPitchInv = ( fabs(GrandPmtPitch)>0 ? 1.0/GrandPmtPitch : 0.0 );
      
      m_Rich2TotNumGrandModules=  firstRich->param<int> ("Rich2TotNumGrandModules" );
      m_Rich2TotNumStdModules=0;
      m_totNumPmtModuleInRich = aRich1NumModules + m_Rich2TotNumGrandModules;
      m_NumPmtModuleInRich[2] = m_Rich2TotNumGrandModules/2; //rich2left
      m_NumPmtModuleInRich[3] = m_Rich2TotNumGrandModules/2; //rich2right
      
      
      m_GrandPmtAnodeXSize = firstRich->param<double>("RichGrandPmtAnodeXSize");
      m_GrandPmtAnodeYSize = firstRich->param<double>("RichGrandPmtAnodeYSize");
      
      m_GrandPmtPixelGap = firstRich->param<double>("RichGrandPmtPixelGap" );
      m_GrandPmtPixelsInRow = firstRich->param<int>("RichGrandPmtNumPixelCol" );
      m_GrandPmtPixelsInCol = firstRich->param<int>("RichGrandPmtNumPixelRow" );
      m_GrandPmtAnodeXEdge = -0.5*(m_GrandPmtAnodeXSize+m_GrandPmtPixelGap);
      m_GrandPmtAnodeYEdge = -0.5*(m_GrandPmtAnodeYSize+m_GrandPmtPixelGap);
      m_GrandAnodeXPixelSize = firstRich->param<double> ("RichGrandPmtPixelXSize" );
      m_GrandAnodeYPixelSize = firstRich->param<double> ("RichGrandPmtPixelYSize" );
      const auto GrandPmtAnodeEffectiveXPixelSize = m_GrandAnodeXPixelSize+m_GrandPmtPixelGap;
      const auto GrandPmtAnodeEffectiveYPixelSize = m_GrandAnodeYPixelSize+m_GrandPmtPixelGap;
      m_GrandPmtAnodeEffectiveXPixelSizeInv = 1.0/GrandPmtAnodeEffectiveXPixelSize;
      m_GrandPmtAnodeEffectiveYPixelSizeInv = 1.0/GrandPmtAnodeEffectiveYPixelSize;
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
          m_MixedPmtModulePlaneHalfSizeR2 = 
            toarray<double,4>( firstRich->param<std::vector<double> >("Rich2MixedPMTModulePlaneHalfSize") );
          m_MixedStdPmtModulePlaneHalfSizeR2 = 
            toarray<double,4>( firstRich->param<std::vector<double> >("Rich2MixedStdPMTModulePlaneHalfSize") );
          m_Rich2TotNumStdModules = firstRich->param<int> ("Rich2TotNumStdModules" );
          m_totNumPmtModuleInRich = aRich1NumModules + m_Rich2TotNumGrandModules+m_Rich2TotNumStdModules;
          m_NumPmtModuleInRich[2] = (m_Rich2TotNumGrandModules+m_Rich2TotNumStdModules)/2; //rich2left
          m_NumPmtModuleInRich[3] = (m_Rich2TotNumGrandModules+m_Rich2TotNumStdModules)/2; //rich2right
          m_Rich2TotNumStdPmts = firstRich->param<int>  ( "Rich2TotNumStdPmt"    );
          if ( firstRich->exists("Rich2MixedNumModulesArraySetup") )
          {
            m_Rich2MixedModuleArrayColumnSize =firstRich ->param<std::vector<int> >("Rich2MixedNumModulesArraySetup");
          }
        }
        
      }
      
    }
    
  }
  
  // Setup flags for large PMTs
  RichSetupMixedSizePmtModules();
  
  // setup the Lens Flag.
  m_totNumPmtModuleInRich1 = firstRich->param<int> ("Rich1TotNumModules" );
  
  m_Rich1PmtLensPresence = 0;
  m_Rich1PmtLensModuleCol.clear();
  
  if ( exists("Rich1PmtLensPresence") )
  {
    m_Rich1PmtLensPresence = firstRich->param<int>("Rich1PmtLensPresence");
  }
  
  if ( m_Rich1PmtLensPresence >= 1 ) {
    
    m_Rich1PmtLensModuleCol = firstRich->param<std::vector<int> >("Rich1PmtLensModuleColumns");
    m_PmtMasterWithLensLateralSize = firstRich->param<double>("RichLensPmtMasterLateralSize" );
    const auto PmtModuleWithLensPitch = firstRich->param<double>("RichLensPmtModulePitch");
    m_PmtModuleWithLensPitchInv = ( fabs(PmtModuleWithLensPitch)>0 ? 1.0/PmtModuleWithLensPitch : 0.0 );
    m_PmtLensPitch=firstRich->param<double>("RichLensPmtPitch");
    m_Rich1PmtPanelWithLensXSize = toarray<double,2>( firstRich->param<std::vector<double> > ("Rich1PMTModuleLensPlaneXEdge") );
    m_Rich1PmtPanelWithLensYSize = toarray<double,2>( firstRich->param<std::vector<double> > ("Rich1PMTModuleLensPlaneYEdge") );
    
    m_Rich1PmtPanelWithLensColSize = toarray<int,2>( firstRich->param<std::vector<int> >("Rich1PmtLensModuleColumnsSize")  );
    
    m_RichNumLensPmtinModuleRowCol[0] =  firstRich->param<int> ( "RichLensPmtNumInModuleRow" );
    m_RichNumLensPmtinModuleRowCol[1] =  firstRich->param<int> ( "RichLensPmtNumInModuleCol" );
    const auto Rich1LensMagnificationFactor = firstRich->param<double> ("RichPmtLensMagnficationFactor" );
    m_Rich1LensDemagnificationFactor = ( fabs(Rich1LensMagnificationFactor)>0 ?
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

  const auto aRowR1 =  2*m_RichPmtNumModulesInRowCol[0];
  const auto aColR1 =  m_RichPmtNumModulesInRowCol[1];
  m_RichPmtModuleLensFlag.clear();
  m_RichPmtModuleLensFlag.reserve(aRowR1*aColR1);
  m_RichPmtModuleLensFlag.resize(aRowR1*aColR1);

  //m_RichPmtModuleLensFlag.assign(aRowR1*aColR1, false);

  for ( int r = 0; r < aRowR1  ; ++r )
  {
    for ( int c = 0; c < aColR1 ; ++c )
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

    const int aTotNumModInRich1 = m_NumPmtModuleInRich[0] + m_NumPmtModuleInRich[1];

    for (int im=aTotNumModInRich1; im<m_totNumPmtModuleInRich; ++im)
    {
      if ( m_Rich2ArrayConfig == 1 )
      {
        m_ModuleIsWithGrandPMT[im] = true;
      }
      else if ( m_Rich2ArrayConfig == 2 )
      {
        int imLocal = im - aTotNumModInRich1;
        if (imLocal > m_NumPmtModuleInRich[2] ) imLocal -= m_NumPmtModuleInRich[2];
        const int aColNum = imLocal/(m_RichPmtNumModulesInRowCol[3]);
        const int aRowNum = imLocal - aColNum*(m_RichPmtNumModulesInRowCol[3]);
        if( (aRowNum < m_Rich2MixedModuleArrayColumnSize[0] ) ||
            (aRowNum >= ( m_Rich2MixedModuleArrayColumnSize[0]+ m_Rich2MixedModuleArrayColumnSize[1] ) ) ) {
          m_ModuleIsWithGrandPMT[im] = true;
        }
      }

    }// end loop over modules in rich2

  } // end if rich2useGrandModule

}

int DeRichPMTPanel::getPmtModuleNumFromRowCol( int MRow, int MCol ) const
{
  int aMNum = -1;

  // set the closest Row Col.
  // This means if the row col exceeds the edges set them to those at the closest edge.

  if ( MRow < 0 ) MRow = 0;
  if ( MCol < 0 ) MCol = 0;

  if ( Rich::Rich1 == rich() )
  {
    if ( MRow >= m_RichPmtNumModulesInRowCol[1] ) MRow = m_RichPmtNumModulesInRowCol[1]-1;
    if ( MCol >= m_RichPmtNumModulesInRowCol[0] ) MCol = m_RichPmtNumModulesInRowCol[0]-1;

    aMNum = MCol + ( MRow*m_RichPmtNumModulesInRowCol[0] );

    if ( side() == Rich::top )
    {
      aMNum += m_RichPmtModuleCopyNumBeginPanel[0] ;
    }
    else // if (side() == Rich::bottom)
    {
      aMNum += m_RichPmtModuleCopyNumBeginPanel[1] ;
    }

  }
  else //if ( rich() == Rich::Rich2 || rich() == Rich::Rich )
  {
    if ( MRow >= m_RichPmtNumModulesInRowCol[3]  ) MRow = m_RichPmtNumModulesInRowCol[3]-1;
    if ( MCol >=  m_RichPmtNumModulesInRowCol[2] ) MCol = m_RichPmtNumModulesInRowCol[2]-1;

    aMNum = MRow + ( MCol*m_RichPmtNumModulesInRowCol[3] );
    if ( side() == Rich::left)
    {
      aMNum += m_RichPmtModuleCopyNumBeginPanel[2];
    }
    else //if ( side() == Rich::right )
    {
      aMNum += m_RichPmtModuleCopyNumBeginPanel[3];
    }

  }

  return aMNum;
}

DeRichPMTPanel::ArraySetup 
DeRichPMTPanel::findPMTArraySetup( const Gaudi::XYZPoint& aGlobalPoint ) const
{
  ArraySetup aCh = {};

  const auto inPanel = geometry()->toLocal(aGlobalPoint);
  const auto x =  inPanel.x();
  const auto y =  inPanel.y();

  int aModuleCol        = -1;
  int aModuleRow        = -1;
  int aModuleNum        = -1;
  int aModuleNumInPanel = -1;
  int aModuleWithLens   = 0;

  if ( Rich::Rich1 == rich() )
  {
    if ( side() == Rich::top )
    {
      if ( m_Rich1PmtLensPresence )
      {
        if ( x > m_Rich1PmtPanelWithLensXSize[0] &&
             x < m_Rich1PmtPanelWithLensXSize[1] )
        {
          aModuleCol = ((int) (fabs( (x-m_Rich1PmtPanelWithLensXSize[0])*m_PmtModulePitchInv ))) + m_Rich1PmtPanelWithLensColSize[2];
          aModuleRow = ((int) (fabs( (y-m_PmtModulePlaneHalfSizeR1[1])  *m_PmtModulePitchInv )));
        }
        else if ( x < m_Rich1PmtPanelWithLensXSize[0] )
        {
          aModuleCol = (int) (fabs( (x-m_Rich1PmtPanelWithLensXSize[2])*m_PmtModuleWithLensPitchInv ));
          aModuleRow = (int) (fabs( (y-m_Rich1PmtPanelWithLensYSize[0])*m_PmtModuleWithLensPitchInv ));
          aModuleWithLens=1;
        }
        else if ( x > m_Rich1PmtPanelWithLensXSize[1] )
        {
          aModuleCol = ( (int) (fabs( (x-m_Rich1PmtPanelWithLensXSize[4])*m_PmtModuleWithLensPitchInv )))
            + m_Rich1PmtPanelWithLensColSize[0] + m_Rich1PmtPanelWithLensColSize[2] ;
          aModuleRow = (int) (fabs( (y-m_Rich1PmtPanelWithLensYSize[0])*m_PmtModuleWithLensPitchInv ));
          aModuleWithLens=1;
        }
      }
      else
      {
        aModuleCol = (int) (fabs( (x-m_PmtModulePlaneHalfSizeR1[0])*m_PmtModulePitchInv));
        aModuleRow = (int) (fabs( (y-m_PmtModulePlaneHalfSizeR1[1])*m_PmtModulePitchInv));
      }

      aModuleNum        = getPmtModuleNumFromRowCol(aModuleRow,aModuleCol);
      aModuleNumInPanel = aModuleNum-m_RichPmtModuleCopyNumBeginPanel[0];

    }
    else //if ( side() == Rich::bottom)
    {
      if ( m_Rich1PmtLensPresence )
      {

        if ( x > m_Rich1PmtPanelWithLensXSize[0] && x < m_Rich1PmtPanelWithLensXSize[1] )
        {
          aModuleCol = ((int)(fabs( (x-m_Rich1PmtPanelWithLensXSize[1])*m_PmtModulePitchInv ))) + m_Rich1PmtPanelWithLensColSize[1];
          aModuleRow = ((int)(fabs( (y-m_PmtModulePlaneHalfSizeR1[3])*m_PmtModulePitchInv)));
        }
        else if ( x > m_Rich1PmtPanelWithLensXSize[1] )
        {
          aModuleCol = (int) (fabs( (x-m_Rich1PmtPanelWithLensXSize[5])*m_PmtModuleWithLensPitchInv ));
          aModuleRow = (int) (fabs( (y-m_Rich1PmtPanelWithLensYSize[0])*m_PmtModuleWithLensPitchInv ));
          aModuleWithLens=1;
        }
        else if ( x <  m_Rich1PmtPanelWithLensXSize[0] )
        {
          aModuleCol = ( (int) (fabs( (x-m_Rich1PmtPanelWithLensXSize[3])*m_PmtModuleWithLensPitchInv )))
            + m_Rich1PmtPanelWithLensColSize[0]+ m_Rich1PmtPanelWithLensColSize[1] ;
          aModuleRow = (int) (fabs( (y-m_Rich1PmtPanelWithLensYSize[0])*m_PmtModuleWithLensPitchInv ));
          aModuleWithLens=1;
        }

      }
      else
      {
        aModuleCol = (int) ( fabs( (x-m_PmtModulePlaneHalfSizeR1[2])*m_PmtModulePitchInv));
        aModuleRow = (int) ( fabs( (y-m_PmtModulePlaneHalfSizeR1[3])*m_PmtModulePitchInv));
      }

      aModuleNum        = getPmtModuleNumFromRowCol(aModuleRow,aModuleCol);
      aModuleNumInPanel = aModuleNum-m_RichPmtModuleCopyNumBeginPanel[1];
    }
  }
  else //if ( rich() == Rich::Rich2 || rich() == Rich::Rich )
  {
    if ( side() == Rich::left )
    {
      if ( ( !m_Rich2UseGrandModule ) )
      {
        aModuleCol = (int) (fabs( (x-m_PmtModulePlaneHalfSizeR2[0])*m_PmtModulePitchInv));
        aModuleRow = (int) (fabs( (y-m_PmtModulePlaneHalfSizeR2[1])*m_PmtModulePitchInv));
      }
      else
      {
        if ( !m_Rich2UseMixedModule )
        {
          aModuleCol = (int) (fabs( (x-m_GrandPmtModulePlaneHalfSizeR2[0])*m_GrandPmtModulePitchInv));
          aModuleRow = (int) (fabs( (y-m_GrandPmtModulePlaneHalfSizeR2[1])*m_GrandPmtModulePitchInv));
        }
        else
        {
          if ( fabs(y) <  fabs(m_MixedStdPmtModulePlaneHalfSizeR2[1]) )
          {
            aModuleCol = (int) (fabs( (x-m_MixedStdPmtModulePlaneHalfSizeR2[0])*m_PmtModulePitchInv));
            aModuleRow = (int) (fabs( (y-m_MixedStdPmtModulePlaneHalfSizeR2[1])*m_PmtModulePitchInv));
            aModuleRow += m_Rich2MixedModuleArrayColumnSize[0];
          }
          else
          {
            aModuleCol = (int) (fabs( (x-m_MixedPmtModulePlaneHalfSizeR2[0])*m_GrandPmtModulePitchInv ));
            if ( y < m_MixedStdPmtModulePlaneHalfSizeR2[1] )
            {
              aModuleRow = (int) (fabs( (y-m_MixedPmtModulePlaneHalfSizeR2[1])*m_GrandPmtModulePitchInv));
            }
            else if (y >= fabs(m_MixedStdPmtModulePlaneHalfSizeR2[1]))
            {
              aModuleRow = (int) (fabs( (y- (fabs(m_MixedStdPmtModulePlaneHalfSizeR2 [1])))*m_GrandPmtModulePitchInv));
              aModuleRow +=  ( m_Rich2MixedModuleArrayColumnSize[0] + m_Rich2MixedModuleArrayColumnSize[1]);
            }

          }

        } // end check on UseMixed module.

      } // end check on any GrandModule use

      aModuleNum        = getPmtModuleNumFromRowCol(aModuleRow,aModuleCol);
      aModuleNumInPanel = aModuleNum-m_RichPmtModuleCopyNumBeginPanel[2];
    }  // end of rich2 side left

    else //if ( side() == Rich::right )
    {
      if ( !m_Rich2UseGrandModule )
      {
        aModuleCol = (int) (fabs( (x-m_PmtModulePlaneHalfSizeR2[2])*m_PmtModulePitchInv ) );
        aModuleRow = (int) (fabs( (y-m_PmtModulePlaneHalfSizeR2[3])*m_PmtModulePitchInv ) );
      }
      else
      {
        if ( !m_Rich2UseMixedModule )
        {
          aModuleCol = (int) (fabs( (x-m_GrandPmtModulePlaneHalfSizeR2[2])*m_GrandPmtModulePitchInv));
          aModuleRow = (int) (fabs( (y-m_GrandPmtModulePlaneHalfSizeR2[3])*m_GrandPmtModulePitchInv));
        }
        else
        {
          if ( fabs(y) < fabs( m_MixedStdPmtModulePlaneHalfSizeR2[3]) )
          {
            aModuleCol = (int) (fabs( (x-m_MixedStdPmtModulePlaneHalfSizeR2[2])*m_PmtModulePitchInv));
            aModuleRow = (int) (fabs( (y-m_MixedStdPmtModulePlaneHalfSizeR2[3])*m_PmtModulePitchInv));
            aModuleRow += m_Rich2MixedModuleArrayColumnSize[0];
          }
          else
          {
            aModuleCol = (int) (fabs( (x-m_MixedPmtModulePlaneHalfSizeR2[2])*m_GrandPmtModulePitchInv ));
            if ( y > m_MixedStdPmtModulePlaneHalfSizeR2 [3] )
            {
              aModuleRow = (int) (fabs( (y-m_MixedPmtModulePlaneHalfSizeR2[3])*m_GrandPmtModulePitchInv));
            }
            else if ( y <= (-1.0*m_MixedStdPmtModulePlaneHalfSizeR2[3] ) )
            {
              aModuleRow = (int) ( fabs( (y+m_MixedStdPmtModulePlaneHalfSizeR2[3]) * m_GrandPmtModulePitchInv ) );
              aModuleRow += ( m_Rich2MixedModuleArrayColumnSize[0] + m_Rich2MixedModuleArrayColumnSize[1] );
            }

          }

        } // end check on mixed module

      } // end check on any GrandModule use

      aModuleNum        = getPmtModuleNumFromRowCol(aModuleRow,aModuleCol);
      aModuleNumInPanel = aModuleNum-m_RichPmtModuleCopyNumBeginPanel[3];
    } // end rich2 side right
  }


  if ( aModuleNum >-1 )
  {
    const auto inModule = ( ((m_DePMTModules[aModuleNumInPanel])->
                             geometry()->toLocalMatrix())*aGlobalPoint );

    const auto xp = inModule.x();
    const auto yp = inModule.y();
    int aPmtCol = -1;
    int aPmtRow = -1;
    int aPmtNum = -1;

    if ( aModuleWithLens == 0 )
    {
      if ( ( rich() == Rich::Rich2 ) && ( ModuleIsWithGrandPMT(aModuleNum)) )
      {
        aPmtCol = (int) (fabs ((xp-m_RichGrandPmtModuleActiveAreaHalfSize[0])*m_GrandPmtPitchInv));
        aPmtRow = (int) (fabs ((yp-m_RichGrandPmtModuleActiveAreaHalfSize[1])*m_GrandPmtPitchInv));
        aPmtNum = getGrandPmtNumFromRowCol(aPmtRow,aPmtCol);
      }
      else
      {
        aPmtCol = (int) (fabs ((xp-m_RichPmtModuleActiveAreaHalfSize[0])*m_PmtPitchInv));
        aPmtRow = (int) (fabs ((yp-m_RichPmtModuleActiveAreaHalfSize[1])*m_PmtPitchInv));
        aPmtNum = getPmtNumFromRowCol(aPmtRow,aPmtCol);
      }
    }
    else
    {
      aPmtCol = (int) (fabs ((xp-(0.5*m_PmtMasterWithLensLateralSize))*m_PmtModuleWithLensPitchInv));
      aPmtRow = (int) (fabs ((yp-(0.5*m_PmtMasterWithLensLateralSize))*m_PmtModuleWithLensPitchInv));
      aPmtNum = getLensPmtNumFromRowCol(aPmtRow,aPmtCol);
    }


    if ( aPmtNum > -1 )
    {
      aCh[0] = aModuleNum;
      aCh[1] = aPmtNum;

      const auto inPmtAnode = ( (m_DePMTAnodes [aModuleNumInPanel] [aPmtNum ]) ->
                                geometry() ->toLocalMatrix() *aGlobalPoint );

      auto xpi = inPmtAnode.x();
      auto ypi = inPmtAnode.y();

      if ( aModuleWithLens > 0 )
      {
        xpi = xpi*m_Rich1LensDemagnificationFactor;
        ypi = ypi*m_Rich1LensDemagnificationFactor;
      }
      int aPmtPixelCol = 0;
      int aPmtPixelRow = 0;

      if ( rich() == Rich::Rich2 && ModuleIsWithGrandPMT(aModuleNum) ) 
      {
        aPmtPixelCol = (int) (fabs ((xpi - m_GrandPmtAnodeXEdge)*m_GrandPmtAnodeEffectiveXPixelSizeInv));
        aPmtPixelRow = (int) (fabs ((ypi - m_GrandPmtAnodeYEdge)*m_GrandPmtAnodeEffectiveYPixelSizeInv));

        if ( aPmtPixelCol >=  m_GrandPmtPixelsInCol ) { aPmtPixelCol = m_GrandPmtPixelsInCol-1; }
        if ( aPmtPixelRow >=  m_GrandPmtPixelsInRow ) { aPmtPixelRow = m_GrandPmtPixelsInRow-1; }
      }
      else 
      {
        aPmtPixelCol = (int) (fabs ((xpi - m_PmtAnodeXEdge)*m_PmtAnodeEffectiveXPixelSizeInv));
        aPmtPixelRow = (int) (fabs ((ypi - m_PmtAnodeYEdge)*m_PmtAnodeEffectiveYPixelSizeInv));
        if ( aPmtPixelCol >=  m_PmtPixelsInCol ) { aPmtPixelCol = m_PmtPixelsInCol-1; }
        if ( aPmtPixelRow >=  m_PmtPixelsInRow ) { aPmtPixelRow = m_PmtPixelsInRow-1; }
      }

      if ( aPmtPixelCol < 0 ) { aPmtPixelCol = 0; }
      if ( aPmtPixelRow < 0 ) { aPmtPixelRow = 0; }

      aCh[2] = aPmtPixelCol;
      aCh[3] = aPmtPixelRow;

    }
    else
    {
      error() << "DeRichPmtPanel : Inadmissible PMT number. Rich side Module Col Row pmt col row  "
              <<rich()<<"   "<< side() << "   "<< aModuleCol <<"  "
              <<aModuleRow<<"  "<<aPmtCol<<"   "<<aPmtRow<< endmsg;
    }

  }
  else
  {
    error() << "DeRichPmtPanel : Inadmissible PMT Module number. Rich side Module Col Row   "
            << rich() <<"   "<< side() << "   "<< aModuleCol <<"  "<<aModuleRow<<endmsg;
  }

  return aCh;
}

/// Returns the intersection point with the detector plane given a vector and a point.
LHCb::RichTraceMode::RayTraceResult
DeRichPMTPanel::detPlanePoint( const Gaudi::XYZPoint& pGlobal,
                               const Gaudi::XYZVector& vGlobal,
                               Gaudi::XYZPoint& hitPosition,
                               LHCb::RichSmartID& smartID,
                               const LHCb::RichTraceMode mode ) const
{
  Gaudi::XYZPoint panelIntersection = Gaudi::XYZPoint(0.0,0.0,0.0);  // define a dummy point and fill correctly later.

  auto sc = getPanelInterSection(pGlobal,vGlobal,panelIntersection);
  if ( !sc ) { return LHCb::RichTraceMode::RayTraceFailed; }

  // set hit position to plane intersection in global frame
  hitPosition = geometry()->toGlobal(panelIntersection);

  // sets RICH, panel and type
  smartID = m_panelID;

  // get the PMT info and set in the smartID
  setRichPmtSmartID( findPMTArraySetup(hitPosition), smartID );
 
  return ( mode.detPlaneBound() == LHCb::RichTraceMode::RespectPDPanel ? 
           isInPmtPanel(panelIntersection) ? LHCb::RichTraceMode::InPDPanel : LHCb::RichTraceMode::OutsidePDPanel :
           LHCb::RichTraceMode::InPDPanel );
}

LHCb::RichTraceMode::RayTraceResult
DeRichPMTPanel::PDWindowPoint( const Gaudi::XYZVector& vGlobal,
                               const Gaudi::XYZPoint& pGlobal,
                               Gaudi::XYZPoint& windowPointGlobal,
                               LHCb::RichSmartID& smartID,
                               const LHCb::RichTraceMode mode ) const
{
  Gaudi::XYZPoint panelIntersection(0,0,0);  // define a dummy point and fill correctly later.

  auto sc = getPanelInterSection(pGlobal,vGlobal,panelIntersection);
  if ( !sc ) { return LHCb::RichTraceMode::RayTraceFailed; }

  // sets RICH, panel and type
  smartID = m_panelID;

  // the return status. Default to outside panel.
  LHCb::RichTraceMode::RayTraceResult res = LHCb::RichTraceMode::OutsidePDPanel;

  // are we in the panel ?
  if ( isInPmtPanel(panelIntersection) )
  {
    // set acceptance to in panel
    res = LHCb::RichTraceMode::InPDPanel;

    // set hit position to plane intersection in global frame
    windowPointGlobal = geometry()->toGlobal(panelIntersection);

    // check PD acceptance ?
    if ( mode.detPlaneBound() != LHCb::RichTraceMode::IgnorePDAcceptance )
    {

      // get the PMT info and set in the smartID
      const auto aC = findPMTArraySetup(windowPointGlobal);
      setRichPmtSmartID( aC, smartID );

      // get module in panel number
      const auto aModuleNumInPanel = PmtModuleNumInPanelFromModuleNumAlone(aC[0]);
      
      // get the DePMT object
      const auto pmt = m_DePMTs[aModuleNumInPanel][aC[1]];
      
      // coordinate in the PMT
      const auto coordinPmt = ( pmt->geometry()->toLocalMatrix() * windowPointGlobal );

      // large PMT flag
      const auto flagGrandPmt = ModuleIsWithGrandPMT(aC[0]);

      // check PMT acceptance
      if ( isInPmt( ( pmt->PmtLensFlag() ? DemagnifyFromLens(coordinPmt) : coordinPmt ), flagGrandPmt ) &&
           isInPmtAnodeLateralAcc(coordinPmt,flagGrandPmt) )
      { res = LHCb::RichTraceMode::InPDTube; }

    }

  }

  return res;
}

bool DeRichPMTPanel::getPanelInterSection ( const Gaudi::XYZPoint& pGlobal,
                                            const Gaudi::XYZVector& vGlobal ,
                                            Gaudi::XYZPoint& panelIntersection ) const
{
  bool sc = true;

  const auto vInPanel = geometry()->toLocalMatrix() * vGlobal;

  // find the intersection with the detection plane
  const auto scalar = vInPanel.Dot(m_localPlaneNormal);

  if ( UNLIKELY( fabs(scalar) < 1e-5 ) ) { sc = false; }
  else
  {
    // transform point to the PMTPanel coordsystem.
    const auto pInPanel = geometry()->toLocal(pGlobal);

    // get panel intersection point
    const auto distance = -m_localPlane.Distance(pInPanel) / scalar;
    panelIntersection = pInPanel + (distance*vInPanel);
  }

  return sc;
}

Rich::DAQ::HPDCopyNumber DeRichPMTPanel::pdNumber( const LHCb::RichSmartID& smartID ) const
{
  return _pdNumber( smartID );
}

const DeRichPD* DeRichPMTPanel::dePD( const LHCb::RichSmartID pdID ) const
{
  return dePMT( _pdNumber( pdID ) );
}

const DeRichPD* DeRichPMTPanel::dePD( const Rich::DAQ::HPDCopyNumber PmtCopyNumber ) const
{
  return dePMT( PmtCopyNumber );
}

const DeRichPMT* DeRichPMTPanel::dePMT( const Rich::DAQ::HPDCopyNumber PmtCopyNumber ) const
{
  const DeRichPMT * dePmt = nullptr;

  if ( PmtCopyNumber.data() < m_totNumPMTs )
  {
    const auto Mnum = (unsigned int) (PmtCopyNumber.data()/m_NumPmtInRichModule);
    const auto MNumInCurPanel = PmtModuleNumInPanelFromModuleNumAlone(Mnum);
    const auto Pnum = PmtCopyNumber.data() - ( Mnum * m_NumPmtInRichModule );

    if ( UNLIKELY( MNumInCurPanel >= (int)m_DePMTs.size() ||
                   Pnum >= m_DePMTs[MNumInCurPanel].size() ) )
    {
      std::ostringstream mess;
      mess << "DeRichPMTPanel: Inappropriate PMT module and pmt numbers "
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
    mess << "DeRichPMTPanel: Inappropriate PmtcopyNumber : " << PmtCopyNumber;
    throw GaudiException( mess.str(), "*DeRichPMTPanel*", StatusCode::FAILURE );
  }

  return dePmt;
}

//  return a list with all the valid readout channels (smartIDs)
//=========================================================================
bool
DeRichPMTPanel::readoutChannelList ( LHCb::RichSmartID::Vector& readoutChannels ) const
{
  const int CurPanelNum = ( rich() == Rich::Rich1 ?
                            ( side() == Rich::top  ? 0 : 1 ) :
                            ( side() == Rich::left ? 2 : 3 ) );

  const auto aBeginM = m_RichPmtModuleCopyNumBeginPanel[CurPanelNum];
  const auto aEndM   = m_RichPmtModuleCopyNumEndPanel[CurPanelNum];
 
  for ( int iM = aBeginM ; iM <= aEndM; ++iM )
  {
    for ( int iP = 0; iP < m_NumPmtInRichModule; ++iP )
    {
      for ( int iPx = 0; iPx < m_PmtPixelsInRow; ++iPx )
      {
        for ( int iPy = 0; iPy < m_PmtPixelsInCol; ++iPy )
        {
          readoutChannels.emplace_back( rich(), side(), iP, iM, iPy, iPx, LHCb::RichSmartID::MaPMTID );
        }
      }
    }
  }

  return true;
}

int DeRichPMTPanel::sensitiveVolumeID(const Gaudi::XYZPoint& globalPoint) const
{
  // Create a RichSmartID for this RICH and panel
  const LHCb::RichSmartID def_id( rich(), side(), 0, 0, 0, 0, LHCb::RichSmartID::MaPMTID );
  // set the remaining fields from the position
  auto id = def_id;
  return ( smartID(globalPoint,id) ? id : def_id );
  // From FTTU branch - return pdNumber
  //return pdNumber( smartID(globalPoint,id) ? id : def_id ).data();
}

//=========================================================================
//  setRichPanelAndSide
//=========================================================================
bool DeRichPMTPanel::setRichPanelAndSide()
{
  bool OK = true;

  const Gaudi::XYZPoint zero(0.0, 0.0, 0.0);
  const Gaudi::XYZPoint centreGlobal(geometry()->toGlobal(zero));

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
    error() << "Cannot identify Rich/side" << endmsg;
    OK = false;
  }

  // set the panel ID
  m_panelID = LHCb::RichSmartID( rich(), side(), LHCb::RichSmartID::MaPMTID );

  return OK;
}

int DeRichPMTPanel::getModuleCopyNumber( const std::string& aModuleName )
{
  int anumber = -1;
  const auto pos2 = aModuleName.find(":");
  if ( std::string::npos == pos2 )
  {
    error() << "A PMTModule  without a number!   " <<aModuleName<< endmsg;
  }
  else
  {
    anumber = atoi( aModuleName.substr(pos2+1).c_str() );
  }

  return anumber;
}
