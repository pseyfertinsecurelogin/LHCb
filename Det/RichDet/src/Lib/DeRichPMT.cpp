
// STL
#include <sstream>

// Gaudi
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/PhysicalConstants.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/Bootstrap.h"

// DetDesc
#include "DetDesc/Condition.h"
#include "DetDesc/SolidBox.h"

// local
#include "RichDet/DeRichPMT.h"
#include "RichDet/DeRich.h"
#include "RichDet/DeRichSystem.h"

// VDT
#include "vdt/sincos.h"
#include "vdt/atan2.h"

//-----------------------------------------------------------------------------
// Implementation file for class : DeRichPMT
//
// 2011-10-11 : Sajan Easo
//-----------------------------------------------------------------------------
const CLID CLID_DeRichPMT = 12025;  // User defined

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
DeRichPMT::DeRichPMT( const std::string & name )
  : DeRichPD ( name ) { }

//=============================================================================

const CLID& DeRichPMT::classID()
{
  return CLID_DeRichPMT;
}

//=============================================================================

StatusCode DeRichPMT::initialize()
{
  StatusCode sc = StatusCode::SUCCESS;
  MsgStream msg( msgSvc(), "DeRichPMT" );

  // store the name of the PMT, without the /dd/Structure part
  const auto pos = name().find("MAPMT:");
  setMyName( std::string::npos != pos ? name().substr(pos) : "DeRichPMT_NO_NAME" );
  if ( msgLevel(MSG::DEBUG,msg) )
    msg << MSG::DEBUG << "Initialize " << myName() << endmsg;

  // extract PMT overall module and copy number from detector element name
  const auto pos2 = name().find(":");
  if ( std::string::npos == pos2 )
  {
    msg << MSG::FATAL << "A PMT without a module number" << endmsg;
    return StatusCode::FAILURE;
  }
  const auto s1   = name().substr(pos2+1);
  const auto pos3 = s1.find("/MAPMT:");
  if ( std::string::npos == pos3 )
  {
    msg << MSG::FATAL << "A PMT without a copy number" << endmsg;
    return StatusCode::FAILURE;
  }
  m_moduleNum = atoi( s1.substr(0,pos3).c_str() );
  m_copyNum   = atoi( s1.substr(pos3+7).c_str() );

  m_dePmtAnode = ( !childIDetectorElements().empty() ?
                   childIDetectorElements().front() : nullptr );
  if ( !m_dePmtAnode )
  {
    fatal() << "DePMT : Cannot find PmtAnode detector element" << endmsg;
    return StatusCode::FAILURE;
  }

  updMgrSvc()->registerCondition( this, geometry(),
                                  &DeRichPMT::updateGeometry );
  updMgrSvc()->registerCondition( this, m_dePmtAnode->geometry(),
                                  &DeRichPMT::updateGeometry );

  // Update PMT QE values whenever DeRichSystem updates
  updMgrSvc()->registerCondition( this, deRichSys(), &DeRichPMT::initPMTQuantumEff );

  // Trigger first update
  sc = sc && ( updMgrSvc()->update(this) );
  if ( sc.isFailure() ) { fatal() << "UMS updates failed" << endmsg; }

  return sc;
}

//=============================================================================

DetectorElement * DeRichPMT::getFirstRich()
{
  DetectorElement * de{nullptr};
  SmartDataPtr<DetectorElement> afterMag(dataSvc(),"/dd/Structure/LHCb/AfterMagnetRegion");
  if ( !afterMag )
  {
    error() << "Could not load AfterMagnetRegion det elem" << endmsg;
  }
  else
  {
    const auto firstRichLoc = ( afterMag->exists("RichDetectorLocations") ?
                                afterMag->paramVect<std::string>("RichDetectorLocations")[0] :
                                DeRichLocations::Rich1 );
    SmartDataPtr<DetectorElement> deRich( dataSvc(), firstRichLoc );
    if ( deRich ) { de = deRich; }
  }
  if ( !de ) { error() << "Could not load DeRich for DeRichPMTPanel" << endmsg; }
  return de;
}

//=============================================================================

StatusCode DeRichPMT::getPMTParameters()
{
  StatusCode sc = StatusCode::SUCCESS;

  const auto * deRich = getFirstRich();
  if ( !deRich ) { return StatusCode::FAILURE; }

  const std::string effnumPixCond = "RichPmtTotNumPixel";
  m_effNumActivePixs = ( deRich->exists(effnumPixCond) ? 
                         (FType)deRich->param<int>(effnumPixCond) : 64.0 );

  m_PmtAnodeXSize = deRich->param<double> ("RichPmtAnodeXSize" );
  m_PmtAnodeYSize = deRich->param<double> ("RichPmtAnodeYSize" );
  m_PmtAnodeZSize = deRich->param<double> ("RichPmtAnodeZSize" );
  m_PmtAnodeLocationInPmt = deRich->param<double>("RichPmtSiliconDetectorLocalZlocation" );
  m_PmtPixelXSize = deRich->param<double>( "RichPmtPixelXSize");
  m_PmtPixelYSize = deRich->param<double>( "RichPmtPixelYSize");
  m_PmtPixelGap = deRich->param<double> ( "RichPmtPixelGap" );
  m_PmtEffectivePixelXSize = m_PmtPixelXSize + m_PmtPixelGap;
  m_PmtEffectivePixelYSize = m_PmtPixelYSize + m_PmtPixelGap;
  m_PmtAnodeHalfThickness = m_PmtAnodeZSize/2.0;
  m_PmtNumPixCol = deRich->param<int> ("RichPmtNumPixelCol");

  m_PmtNumPixRow   = deRich->param<int>    ( "RichPmtNumPixelRow" );
  m_PmtQwZSize     = deRich->param<double> ( "RichPmtQuartzZSize" );
  m_QwToAnodeZDist = deRich->param<double> ( "RichPmtQWToSiMaxDist" );

  m_Rich2PmtArrayConfig = 0;

  if ( deRich->exists ("Rich2PMTArrayConfig") )
  {

    m_Rich2PmtArrayConfig = deRich->param<int>("Rich2PMTArrayConfig");
    if ( deRich->exists ("RichGrandPmtAnodeXSize" ) )
    {
      m_GrandPmtAnodeXSize     = deRich->param<double>( "RichGrandPmtAnodeXSize" );
      m_GrandPmtAnodeYSize     = deRich->param<double>( "RichGrandPmtAnodeYSize" );
      m_GrandPmtAnodeZSize     = deRich->param<double>( "RichGrandPmtAnodeZSize" );
      m_GrandPmtPixelXSize     = deRich->param<double>( "RichGrandPmtPixelXSize" );
      m_GrandPmtPixelYSize     = deRich->param<double>( "RichGrandPmtPixelYSize" );
      m_GrandPmtPixelGap       = deRich->param<double>( "RichGrandPmtPixelGap" );
      m_GrandPmtEdgePixelXSize = deRich->param<double>( "RichGrandPmtEdgePixelXSize");
      m_GrandPmtEdgePixelYSize = deRich->param<double>( "RichGrandPmtEdgePixelYSize");
      m_GrandPmtEffectivePixelXSize = m_GrandPmtPixelXSize + m_GrandPmtPixelGap;
      m_GrandPmtEffectivePixelYSize = m_GrandPmtPixelYSize + m_GrandPmtPixelGap;
      m_GrandPmtEdgePixelXDiff = m_GrandPmtEdgePixelXSize - m_GrandPmtPixelXSize;
      m_GrandPmtEdgePixelYDiff = m_GrandPmtEdgePixelYSize - m_GrandPmtPixelYSize;
      m_GrandPmtAnodeHalfThickness = m_GrandPmtAnodeZSize/2.0;
    }

  }
  m_Rich2UseGrandPmt = false;
  if ( m_Rich2PmtArrayConfig >= 1 ) 
  {
    m_Rich2UseGrandPmt = true;
    if ( m_Rich2PmtArrayConfig == 2 ) 
    {
      m_Rich2UseMixedPmt = true;
    }
  }

  const FType Rich1Rich2ZDivideLimit = 6000.0;
  
  // Which RICH are we in ?
  const Gaudi::XYZPoint atestPoint(0.0,0.0,0.0);
  const auto atestGlobalPoint = geometry()->toGlobalMatrix() * atestPoint ;
  m_rich = ( atestGlobalPoint.z() > Rich1Rich2ZDivideLimit ? Rich::Rich2 : Rich::Rich1 );

  if ( exists("RichPmtLensMagnficationFactor") ) 
  {
    m_PmtLensMagnificationRatio = deRich->param<double>("RichPmtLensMagnficationFactor"  );
  }
  else
  {
    m_PmtLensMagnificationRatio = 1.0;
  }

  if ( exists("RichPmtLensRadiusofCurvature") )
  {
    m_PmtLensRoc2 = std::pow( deRich->param<double>("RichPmtLensRadiusofCurvature"), 2 );
  }
  else 
  {
    m_PmtLensRoc2 = std::pow( 100000.0, 2 );
  }

  // Default initialise some DePD base parameters
  setPmtIsGrandFlag( PmtIsGrand() );

  return sc;
}

//=============================================================================

void DeRichPMT::setPmtIsGrandFlag( const bool isGrand )
{
  m_PmtIsGrand = isGrand;
  // Update cached size parameters
  m_pixelArea = ( isGrand ?
                  m_GrandPmtPixelXSize * m_GrandPmtPixelYSize :
                  m_PmtPixelXSize      * m_PmtPixelYSize );
  m_effPixelArea = m_pixelArea; // PMTs have no demagnification
}

//=============================================================================

StatusCode DeRichPMT::initPMTQuantumEff()
{
  const auto * deRich = getFirstRich();
  if ( !deRich ) { return StatusCode::FAILURE; }
  const auto PmtQELocation = deRich->param<std::string>("RichPmtQETableName");
  SmartDataPtr<TabulatedProperty> pmtQuantumEffTabProp( dataSvc(), PmtQELocation );
  m_pdQuantumEffFunc = std::make_shared<Rich::TabulatedProperty1D>( pmtQuantumEffTabProp );
  return StatusCode::SUCCESS;
}

//=============================================================================

StatusCode DeRichPMT::updateGeometry()
{
  StatusCode sc = getPMTParameters();
  return sc;
}

//=============================================================================

bool DeRichPMT::detectionPoint ( const double fracPixelCol,
                                 const double fracPixelRow,
                                 Gaudi::XYZPoint& detectPoint,
                                 const bool photoCathodeSide ) const
{
  const auto aLocalHit = ( Rich::Rich2 == m_rich && PmtIsGrand() ? 
                           getAnodeHitCoordFromGrandPixelNum( fracPixelCol,fracPixelRow ):
                           getAnodeHitCoordFromPixelNum     ( fracPixelCol,fracPixelRow ) );

  // detectPoint  =  geometry()->toGlobalMatrix() * aPhCathHit ; // not used since the
  // output is expected to be in the PMT coord system.
  // for now assume negligible refraction effect at the QW.
 
  const auto zPh = aLocalHit.z() + m_QwToAnodeZDist +  m_PmtAnodeLocationInPmt ;

  if ( photoCathodeSide )
  {
    const Gaudi::XYZPoint aPhCathHit ( aLocalHit.x(), aLocalHit.y(), zPh    );
    detectPoint = ( m_PmtLensFlag ? RichPmtLensReconFromPhCath(aPhCathHit) : aPhCathHit );
  }
  else
  {
    const auto zQwExt = zPh + m_PmtQwZSize;
    const Gaudi::XYZPoint aQWExtHit  ( aLocalHit.x(), aLocalHit.y(), zQwExt );
    detectPoint = ( m_PmtLensFlag ? RichPmtLensReconFromPhCath(aQWExtHit)  : aQWExtHit );
  }

  return true;
}

//=============================================================================

Gaudi::XYZPoint 
DeRichPMT::RichPmtLensReconFromPhCath( const Gaudi::XYZPoint & aPhCathCoord ) const
{

  const auto aPhCaRsq_Coord = ( std::pow(aPhCathCoord.x(),2) + std::pow(aPhCathCoord.y(),2) );
  const auto aPhCaR_Coord =  ( aPhCaRsq_Coord > 0.0 ? std::sqrt(aPhCaRsq_Coord) : 0.0 );
  const auto aPhCaRsq_Phi = vdt::fast_atan2( aPhCathCoord.y(), aPhCathCoord.x() );
  const auto aXSignLocal = ( aPhCathCoord.x() > 0 ? 1 : -1 );
  const auto aYSignLocal = ( aPhCathCoord.y() > 0 ? 1 : -1 );

  double sinphi(0), cosphi(0);
  vdt::fast_sincos( aPhCaRsq_Phi, sinphi, cosphi );
  const auto aLensRecXLocal = fabs((aPhCaR_Coord*m_PmtLensMagnificationRatio)*cosphi)* aXSignLocal;
  const auto aLensRecYLocal = fabs((aPhCaR_Coord*m_PmtLensMagnificationRatio)*sinphi)* aYSignLocal;

  const auto Rsq = std::pow(aPhCaR_Coord*m_PmtLensMagnificationRatio,2);

  const auto aLensRecZStd = aPhCathCoord.z() + std::sqrt( m_PmtLensRoc2 - Rsq );

  return { aLensRecXLocal, aLensRecYLocal, aLensRecZStd };
}

//=============================================================================

bool DeRichPMT::detectionPoint( const LHCb::RichSmartID smartID,
                                Gaudi::XYZPoint& detectPoint,
                                bool photoCathodeSide ) const
{
  const auto aPixCol  = (FType) ( smartID.pixelCol() );
  const auto aPixRow  = (FType) ( smartID.pixelRow() );
  const auto aLocalHit = getAnodeHitCoordFromMultTypePixelNum( aPixCol, aPixRow, smartID );

  const auto zPh = aLocalHit.z() + m_QwToAnodeZDist + m_PmtAnodeLocationInPmt ;

  // for now assume negligible refraction effect at the QW.

  if ( photoCathodeSide )
  {
    const Gaudi::XYZPoint aPhCathHit( aLocalHit.x(), aLocalHit.y(), zPh );
    detectPoint = ( geometry()->toGlobalMatrix() * 
                    ( m_PmtLensFlag ? RichPmtLensReconFromPhCath(aPhCathHit) : aPhCathHit ) );
  }
  else
  {
    const auto zQwExt = zPh + m_PmtQwZSize;
    const Gaudi::XYZPoint aQWExtHit ( aLocalHit.x(), aLocalHit.y(), zQwExt );
    detectPoint = ( geometry()->toGlobalMatrix() * 
                    ( m_PmtLensFlag ? RichPmtLensReconFromPhCath(aQWExtHit) : aQWExtHit ) );       
  }

  return true;
}

//============================================================================================

Gaudi::XYZPoint
DeRichPMT::getAnodeHitCoordFromGrandPixelNum( const FType fracPixelCol,
                                              const FType fracPixelRow ) const
{
  const auto aXEffPixel =
    ((fracPixelCol==0) || (fracPixelCol==( m_PmtNumPixCol-1)) ) ?  
    m_GrandPmtEdgePixelXSize : m_GrandPmtEffectivePixelXSize;
  const auto aYEffPixel =
    ((fracPixelRow==0) || (fracPixelRow==( m_PmtNumPixRow-1)) ) ? 
    m_GrandPmtEdgePixelYSize : m_GrandPmtEffectivePixelYSize;

  const auto xh = ( fracPixelCol - (m_PmtNumPixCol-1) * 0.5 ) * aXEffPixel;
  const auto yh = ( fracPixelRow - (m_PmtNumPixRow-1) * 0.5 ) * aYEffPixel;
  const auto zh = m_GrandPmtAnodeHalfThickness;

  return { xh, yh, zh };
}

//===============================================================================================

Gaudi::XYZPoint DeRichPMT::detPointOnAnode ( const LHCb::RichSmartID& smartID ) const
{
  const auto aPixCol = (FType) (smartID.pixelCol());
  const auto aPixRow = (FType) (smartID.pixelRow());
  const auto aLocalAnodeCoord = getAnodeHitCoordFromMultTypePixelNum(aPixCol,aPixRow,smartID );
  return ( m_dePmtAnode->geometry()->toGlobal(aLocalAnodeCoord) );
}

//=============================================================================
