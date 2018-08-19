
/** @file DeRich1.cpp
 *
 *  Implementation file for detector description class : DeRich1
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */

// Include files
#include "RichDet/DeRich1.h"
#include "RichDet/DeRichHPDPanel.h"
#include "RichDet/DeRichSphMirror.h"

// Gaudi
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/SmartDataPtr.h"

// DetDesc
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/Material.h"

//-----------------------------------------------------------------------------

const CLID CLID_DERich1 = 12001; // User defined

// Standard Constructors
DeRich1::DeRich1( const std::string &name ) : DeRich( name )
{
  m_rich = Rich::Rich1;
  setMyName( "DeRich1" );
}

// Retrieve Pointer to class defininition structure
const CLID &
DeRich1::classID()
{
  return CLID_DERich1;
}

//===========================================================================

StatusCode
DeRich1::initialize()
{
  _ri_debug << "Initialize " << name() << endmsg;

  if ( !DeRich::initialize() ) return StatusCode::FAILURE;

  // Declare dependency on mirrors
  for ( const auto mirrType :
        { "SphericalMirrorDetElemLocations", "SecondaryMirrorDetElemLocations" } )
  {
    for ( const auto &loc : paramVect< std::string >( mirrType ) )
    {
      SmartDataPtr< DeRichSphMirror > mirror( dataSvc(), loc );
      updMgrSvc()->registerCondition( this, &*mirror, &DeRich1::updateMirrorParams );
    }
  }

  const IPVolume *pvGasWindow( nullptr );
  const IPVolume *pvRich1SubMaster = geometry()->lvolume()->pvolume( "pvRich1SubMaster" );
  if ( pvRich1SubMaster )
  {
    pvGasWindow = pvRich1SubMaster->lvolume()
                    ->pvolume( "pvRich1MagShH0:0" )
                    ->lvolume()
                    ->pvolume( "pvRich1GQuartzWH0:0" );
  }
  if ( pvGasWindow )
  {
    for ( const auto &mat : pvGasWindow->lvolume()->material()->tabulatedProperties() )
    {
      if ( mat )
      {
        if ( mat->type() == "RINDEX" )
        {
          _ri_debug << "Loaded gas window refIndex from: " << mat->name() << endmsg;
          m_gasWinRefIndex.reset( new Rich::TabulatedProperty1D( mat ) );
          if ( !m_gasWinRefIndex->valid() )
          { return Error( "Invalid RINDEX Rich::TabulatedProperty1D for " + mat->name() ); }
        }
        else if ( mat->type() == "ABSLENGTH" )
        {
          m_gasWinAbsLength.reset( new Rich::TabulatedProperty1D( mat ) );
          if ( !m_gasWinAbsLength->valid() )
          { return Error( "Invalid ABSLENGTH Rich::TabulatedProperty1D for " + mat->name() ); }
        }
      }
    }
  }
  else
  {
    return Error( "Could not find gas window properties" );
  }

  // get the nominal reflectivity of the spherical mirror
  const std::string sphCondname( "NominalSphericalMirrorReflectivityLoc" );
  const std::string sphMirrorReflLoc =
    ( exists( sphCondname ) ? param< std::string >( sphCondname ) :
                              "/dd/Geometry/BeforeMagnetRegion/Rich1/Rich1SurfaceTabProperties/"
                              "Rich1Mirror1SurfaceIdealReflectivityPT" );

  SmartDataPtr< TabulatedProperty > sphMirrorRefl( dataSvc(), sphMirrorReflLoc );
  if ( !sphMirrorRefl )
  { return Error( "No info on spherical mirror reflectivity at " + sphMirrorReflLoc ); }
  else
  {
    _ri_debug << "Loaded spherical mirror reflectivity from: " << sphMirrorReflLoc << endmsg;
    m_nominalSphMirrorRefl.reset( new Rich::TabulatedProperty1D( sphMirrorRefl ) );
    if ( !m_nominalSphMirrorRefl->valid() )
    { return Error( "Invalid Rich::TabulatedProperty1D for " + sphMirrorRefl->name() ); }
  }

  // get the nominal reflectivity of the secondary mirror
  const std::string secCondName( "NominalSecondaryMirrorReflectivityLoc" );
  const std::string secMirrorReflLoc =
    ( exists( secCondName ) ? param< std::string >( secCondName ) :
                              "/dd/Geometry/BeforeMagnetRegion/Rich1/Rich1SurfaceTabProperties/"
                              "Rich1Mirror2SurfaceIdealReflectivityPT" );

  SmartDataPtr< TabulatedProperty > secMirrorRefl( dataSvc(), secMirrorReflLoc );
  if ( !secMirrorRefl )
  { return Error( "No info on secondary mirror reflectivity at " + secMirrorReflLoc ); }
  else
  {
    _ri_debug << "Loaded secondary mirror reflectivity from: " << secMirrorReflLoc << endmsg;
    m_nominalSecMirrorRefl.reset( new Rich::TabulatedProperty1D( secMirrorRefl ) );
    if ( !m_nominalSecMirrorRefl->valid() )
    { return Error( "Invalid Rich::TabulatedProperty1D for " + secMirrorRefl->name() ); }
  }

  // initialize all child detector elements. This triggers the update
  // of the radiator properties
  childIDetectorElements();

  // initialise various local cached data
  loadPDPanels();
  loadNominalQuantumEff();

  // Trigger first update
  const auto sc = updMgrSvc()->update( this );
  if ( sc.isFailure() ) { fatal() << "UMS updates failed" << endmsg; }
  return sc;
}

//===========================================================================

StatusCode
DeRich1::updateMirrorParams()
{

  if ( msgLevel( MSG::DEBUG ) )
  {
    debug() << "CoC Top      " << param< std::vector< double > >( "NominalSphMirrorTopCoC" )
            << endmsg;
    debug() << "CoC Bot      " << param< std::vector< double > >( "NominalSphMirrorBotCoC" )
            << endmsg;
    debug() << "CoC          " << param< std::vector< double > >( "NominalSphMirrorCoC" ) << endmsg;
    debug() << "SecPlane Top " << param< std::vector< double > >( "NominalSecMirrorPlaneTop" )
            << endmsg;
    debug() << "SecPlane Bot " << param< std::vector< double > >( "NominalSecMirrorPlaneBot" )
            << endmsg;
    debug() << "SecPlane     " << param< std::vector< double > >( "NominalSecMirrorPlane" )
            << endmsg;
    debug() << "RoC          " << param< double >( "SphMirrorRadius" ) << endmsg;
  }

  // Load the primary mirror segments to compute the 'nominal' settings
  {
    m_nominalCentresOfCurvature[ Rich::top ]    = Gaudi::XYZPoint( 0, 0, 0 );
    m_nominalCentresOfCurvature[ Rich::bottom ] = Gaudi::XYZPoint( 0, 0, 0 );
    unsigned int nTop( 0 ), nBot( 0 );
    double       avroc{ 0 };
    for ( const auto &loc : paramVect< std::string >( "SphericalMirrorDetElemLocations" ) )
    {
      SmartDataPtr< DeRichSphMirror > mirror( dataSvc(), loc );
      const auto                      plane = mirror->centreNormalPlane();
      const auto                      coc   = mirror->centreOfCurvature();
      const auto                      cen   = mirror->mirrorCentre();
      const auto                      roc   = mirror->radius();
      _ri_debug << loc << " Centre = " << cen << " Plane " << plane.Normal() << " "
                << plane.HesseDistance() << " RoC = " << roc << " CoC = " << coc << endmsg;
      if ( cen.y() > 0 )
      {
        ++nTop;
        m_nominalCentresOfCurvature[ Rich::top ] += Gaudi::XYZVector( coc );
      }
      else
      {
        ++nBot;
        m_nominalCentresOfCurvature[ Rich::bottom ] += Gaudi::XYZVector( coc );
      }
      avroc += roc;
    }
    m_nominalCentresOfCurvature[ Rich::top ] /= (double)nTop;
    m_nominalCentresOfCurvature[ Rich::bottom ] /= (double)nBot;
    m_sphMirrorRadius = avroc / (double)( nTop + nBot );
  }

  // m_sphMirrorRadius = param<double>("SphMirrorRadius");

  _ri_debug << "Nominal centre of curvature " << m_nominalCentresOfCurvature << endmsg;

  _ri_debug << "Nominal Radius of Curvature = " << m_sphMirrorRadius << endmsg;

  // get the parameters of the nominal flat mirror plane in the form
  // Ax+By+Cz+D=0

  // Load the secondary mirrors to compute the 'nominal' settings
  {
    std::vector< double > nominalFMirrorPlaneTop{ 0, 0, 0, 0 };
    std::vector< double > nominalFMirrorPlaneBot{ 0, 0, 0, 0 };
    unsigned int          nTop( 0 ), nBot( 0 );
    for ( const auto &loc : paramVect< std::string >( "SecondaryMirrorDetElemLocations" ) )
    {
      SmartDataPtr< DeRichSphMirror > mirror( dataSvc(), loc );
      auto                            plane = mirror->centreNormalPlane();
      const auto                      cen   = mirror->mirrorCentre();
      _ri_debug << loc << " Centre = " << mirror->mirrorCentre() << " Plane = " << plane.Normal()
                << " " << plane.HesseDistance() << " RoC = " << mirror->radius()
                << " CoC = " << mirror->centreOfCurvature() << endmsg;
      auto &params = ( cen.y() > 0 ? nominalFMirrorPlaneTop : nominalFMirrorPlaneBot );
      params[ 0 ] += plane.A();
      params[ 1 ] += plane.B();
      params[ 2 ] += plane.C();
      params[ 3 ] += plane.D();
      ++( cen.y() > 0 ? nTop : nBot );
    }

    m_nominalPlanes[ Rich::top ]    = Gaudi::Plane3D( nominalFMirrorPlaneTop[ 0 ] / (double)nTop,
                                                   nominalFMirrorPlaneTop[ 1 ] / (double)nTop,
                                                   nominalFMirrorPlaneTop[ 2 ] / (double)nTop,
                                                   nominalFMirrorPlaneTop[ 3 ] / (double)nTop );
    m_nominalPlanes[ Rich::bottom ] = Gaudi::Plane3D( nominalFMirrorPlaneBot[ 0 ] / (double)nBot,
                                                      nominalFMirrorPlaneBot[ 1 ] / (double)nBot,
                                                      nominalFMirrorPlaneBot[ 2 ] / (double)nBot,
                                                      nominalFMirrorPlaneBot[ 3 ] / (double)nBot );
  }

  _ri_debug << "Nominal Plane Top " << m_nominalPlanes[ Rich::top ].Normal() << " "
            << m_nominalPlanes[ Rich::top ].HesseDistance() << endmsg;
  _ri_debug << "Nominal Plane Bot " << m_nominalPlanes[ Rich::bottom ].Normal() << " "
            << m_nominalPlanes[ Rich::bottom ].HesseDistance() << endmsg;

  m_nominalNormals[ Rich::top ]    = m_nominalPlanes[ Rich::top ].Normal();
  m_nominalNormals[ Rich::bottom ] = m_nominalPlanes[ Rich::bottom ].Normal();

  _ri_debug << "Nominal normal " << m_nominalNormals << endmsg;

  // Fill SIMD caches
  fillSIMDTypes();

  return StatusCode::SUCCESS;
}

//=========================================================================
// Access the name for a given panel
//=========================================================================
const std::string
DeRich1::panelName( const Rich::Side panel ) const
{

  std::string pname =
    ( Rich::top == panel ? DeRichLocations::Rich1Panel0 : DeRichLocations::Rich1Panel1 );

  if ( RichPhotoDetConfig() == Rich::HPDConfig )
  {

    if ( exists( "Rich1HPDPanelDetElemLocations" ) )
    {
      const auto &panelLoc = paramVect< std::string >( "Rich1HPDPanelDetElemLocations" );
      pname                = panelLoc[ panel ];
    }
    else if ( exists( "HPDPanelDetElemLocations" ) )
    { // kept for backward compatibility
      const auto &panelLoc = paramVect< std::string >( "HPDPanelDetElemLocations" );
      pname                = panelLoc[ panel ];
    }
  }
  else if ( RichPhotoDetConfig() == Rich::PMTConfig )
  {

    if ( exists( "Rich1PMTPanelDetElemLocations" ) )
    {
      const auto &panelLoc = paramVect< std::string >( "Rich1PMTPanelDetElemLocations" );
      pname                = panelLoc[ panel ];
    }
  }

  return pname;
}
