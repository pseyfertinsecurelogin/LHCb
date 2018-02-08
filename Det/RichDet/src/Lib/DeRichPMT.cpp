
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

// RichUtils
#include "RichUtils/FastMaths.h"

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

  // register for updates when geometry changes
  updMgrSvc()->registerCondition( this, geometry(), &DeRichPMT::updateGeometry );
  updMgrSvc()->registerCondition( this, m_dePmtAnode->geometry(), &DeRichPMT::updateGeometry );
  // Update PMT QE values whenever DeRichSystem updates
  updMgrSvc()->registerCondition( this, deRichSys(), &DeRichPMT::initPMTQuantumEff );

  // Trigger first update
  StatusCode sc = updMgrSvc()->update(this);
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

  // CRJ - To reduce PMT memory size do not load parameters not used yet ...

  const auto PmtAnodeZSize = deRich->param<double> ("RichPmtAnodeZSize" );
  const auto PmtAnodeLocationInPmt = deRich->param<double>("RichPmtSiliconDetectorLocalZlocation" );
  const auto PmtPixelXSize = deRich->param<double>( "RichPmtPixelXSize");
  const auto PmtPixelYSize = deRich->param<double>( "RichPmtPixelYSize");
  const auto PmtPixelGap = deRich->param<double> ( "RichPmtPixelGap" );
  m_PmtEffectivePixelXSize = PmtPixelXSize + PmtPixelGap;
  m_PmtEffectivePixelYSize = PmtPixelYSize + PmtPixelGap;
  m_PmtAnodeHalfThickness = PmtAnodeZSize/2.0;
  m_PmtNumPixCol = deRich->param<int> ("RichPmtNumPixelCol");
  m_PmtNumPixRow = deRich->param<int> ("RichPmtNumPixelRow");
  m_PmtNumPixColFrac = (m_PmtNumPixCol-1) * 0.5;
  m_PmtNumPixRowFrac = (m_PmtNumPixRow-1) * 0.5;

  m_PmtQwZSize   = deRich->param<double> ( "RichPmtQuartzZSize" );
  const auto QwToAnodeZDist = deRich->param<double> ( "RichPmtQWToSiMaxDist" );

  m_zShift = QwToAnodeZDist + PmtAnodeLocationInPmt;

  int Rich2PmtArrayConfig = 0;

  if ( deRich->exists ("Rich2PMTArrayConfig") )
  {

    Rich2PmtArrayConfig = deRich->param<int>("Rich2PMTArrayConfig");
    if ( deRich->exists ("RichGrandPmtAnodeXSize" ) )
    {
      const auto GrandPmtAnodeZSize  = deRich->param<double>( "RichGrandPmtAnodeZSize" );
      const auto GrandPmtPixelXSize  = deRich->param<double>( "RichGrandPmtPixelXSize" );
      const auto GrandPmtPixelYSize  = deRich->param<double>( "RichGrandPmtPixelYSize" );
      const auto GrandPmtPixelGap    = deRich->param<double>( "RichGrandPmtPixelGap" );
      m_GrandPmtEdgePixelXSize = deRich->param<double>( "RichGrandPmtEdgePixelXSize");
      m_GrandPmtEdgePixelYSize = deRich->param<double>( "RichGrandPmtEdgePixelYSize");
      m_GrandPmtEffectivePixelXSize = GrandPmtPixelXSize + GrandPmtPixelGap;
      m_GrandPmtEffectivePixelYSize = GrandPmtPixelYSize + GrandPmtPixelGap;
      m_GrandPmtAnodeHalfThickness = GrandPmtAnodeZSize/2.0;
    }

  }
  m_Rich2UseGrandPmt = false;
  if ( Rich2PmtArrayConfig >= 1 )
  {
    m_Rich2UseGrandPmt = true;
    if ( Rich2PmtArrayConfig == 2 )
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
    m_PmtLensMagRatio = deRich->param<double>("RichPmtLensMagnficationFactor"  );
  }
  else
  {
    m_PmtLensMagRatio = 1.0;
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

  // cache to local matrix
  m_toLocM = geometry()->toLocalMatrix();

  return sc;
}

//=============================================================================

void DeRichPMT::setPmtIsGrandFlag( const bool isGrand )
{
  m_PmtIsGrand = isGrand;
  // Update cached size parameters
  const auto * deRich = getFirstRich();
  if ( deRich )
  {
    if ( isGrand )
    {
      const auto GrandPmtPixelXSize = deRich->param<double>( "RichGrandPmtPixelXSize" );
      const auto GrandPmtPixelYSize = deRich->param<double>( "RichGrandPmtPixelYSize" );
      m_pixelArea = GrandPmtPixelXSize * GrandPmtPixelYSize;
    }
    else
    {
      const auto PmtPixelXSize = deRich->param<double>( "RichPmtPixelXSize");
      const auto PmtPixelYSize = deRich->param<double>( "RichPmtPixelYSize");
      m_pixelArea = PmtPixelXSize * PmtPixelYSize;
    }
    m_effPixelArea = m_pixelArea; // PMTs have no demagnification
  }
  else { error() << "Could not load First Rich" << endmsg; }
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
  return getPMTParameters();
}

//=============================================================================

Gaudi::XYZPoint
DeRichPMT::RichPmtLensReconFromPhCath( const Gaudi::XYZPoint & aPhCathCoord ) const
{

  const auto x = aPhCathCoord.x();
  const auto y = aPhCathCoord.y();
  const auto aPhCaRsq_Coord = ( (x*x) + (y*y) );
  const auto aPhCaR_Coord   = ( aPhCaRsq_Coord > 0.0 ? std::sqrt(aPhCaRsq_Coord) : 0.0 );
  const auto aPhCaRsq_Phi   = Rich::Maths::fast_atan2( aPhCathCoord.y(), aPhCathCoord.x() );
  const auto aXSignLocal    = ( aPhCathCoord.x() > 0 ? 1 : -1 );
  const auto aYSignLocal    = ( aPhCathCoord.y() > 0 ? 1 : -1 );

  double sinphi(0), cosphi(0);
  Rich::Maths::fast_sincos( aPhCaRsq_Phi, sinphi, cosphi );
  const auto aLensRecXLocal = fabs((aPhCaR_Coord*m_PmtLensMagRatio)*cosphi) * aXSignLocal;
  const auto aLensRecYLocal = fabs((aPhCaR_Coord*m_PmtLensMagRatio)*sinphi) * aYSignLocal;

  const auto Rsq = aPhCaRsq_Coord * m_PmtLensMagRatio * m_PmtLensMagRatio;

  const auto aLensRecZStd = aPhCathCoord.z() + std::sqrt( m_PmtLensRoc2 - Rsq );

  return { aLensRecXLocal, aLensRecYLocal, aLensRecZStd };
}

//=============================================================================

bool DeRichPMT::detectionPoint( const LHCb::RichSmartID smartID,
                                Gaudi::XYZPoint& detectPoint,
                                bool photoCathodeSide ) const
{
  const auto aLocalHit = getAnodeHitCoordFromMultTypePixelNum( smartID.pixelCol(),
                                                               smartID.pixelRow(),
                                                               smartID.rich() );

  const auto zPh = aLocalHit.z() + m_zShift;

  // for now assume negligible refraction effect at the QW.

  if ( UNLIKELY(photoCathodeSide) )
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

//===============================================================================================

DeRichPD::SIMDFP::MaskType
DeRichPMT::detectionPoint( const SmartIDs& smartID,
                           SIMDPoint& detectPoint,
                           bool photoCathodeSide ) const 
{
  SIMDFP::MaskType ok{{}};

  // Just use a scalar loop here...
  SIMDFP X(0), Y(0), Z(0);
  for ( std::size_t i = 0; i < SIMDFP::Size; ++i )
  {
    Gaudi::XYZPoint p{0,0,0};
    ok[i] = detectionPoint( smartID[i], p, photoCathodeSide );
    if ( ok[i] )
    {
      X[i] = p.X();
      Y[i] = p.Y();
      Z[i] = p.Z();
    }
  }
  detectPoint = { X, Y, Z };

  // To Do, implement a full SIMD version


  return ok;
}

//===============================================================================================

Gaudi::XYZPoint DeRichPMT::detPointOnAnode ( const LHCb::RichSmartID& smartID ) const
{
  return ( m_dePmtAnode->geometry()->toGlobal( getAnodeHitCoordFromMultTypePixelNum( smartID.pixelCol(),
                                                                                     smartID.pixelRow(),
                                                                                     smartID.rich() ) ) );
}

//=============================================================================
