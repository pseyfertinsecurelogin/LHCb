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
// ============================================================================
// Include files
// ============================================================================
/// GaudiKernel
// ============================================================================
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
// ============================================================================
// DetDesc
// ============================================================================
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/IDetectorElement.h"
#include "DetDesc/IntersectionErrors.h"
// ============================================================================
// local
// ============================================================================
#include "TransportSvc.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/format.hpp"
// ============================================================================
/** @file TransportSvc.cpp
 *
 *  implementation of class TransportSvc
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 */
// ============================================================================
namespace
{
  inline
  std::string
  logVol
  ( const std::string& name                     ,
    const unsigned int len                      ,
    const std::string& prefix = "/dd/Geometry/" )
  {
    auto pos = name.find ( prefix ) ;
    if ( 0 == pos )
    { return logVol( { name , pos + prefix.size() } , len , prefix ) ; }

    return len < name.size () ? std::string{ name , name.size() - len  } : name;
  }
}
// ============================================================================
//  Implementation of initialize() method
// ============================================================================
StatusCode TransportSvc::initialize()
{
  /// initialise the base class
  StatusCode statusCode = Service::initialize();
  if( statusCode.isFailure() ) return statusCode; // Propagate the error


  /// Locate Detector Data  Service:
  m_detDataSvc = serviceLocator()->service(m_detDataSvc_name, true);
  if ( !m_detDataSvc ) {
    fatal() << " Unable to locate  Detector Data Service="
            << m_detDataSvc_name << endmsg;
    return StatusCode::FAILURE ;
  }

  if ( msgLevel(MSG::DEBUG) && !m_protocol )
  {
    warning() << "Protocol property is always 'true' when in DEBUG" << endmsg ;
    m_protocol = true ;
  }

  if ( !DetDesc::IntersectionErrors::service() )
  {
    info() << "Initialize the static pointer to DetDesc::IGeometryErrorSvc" << endmsg ;
    DetDesc::IntersectionErrors::setService ( this ) ;
  }

  if ( !m_protocol )
  {
    warning() << "The protocol of geometry errors is DISABLED" << endmsg;
    DetDesc::IntersectionErrors::setService ( 0 ) ;
  }

  // recover errors ?
  DetDesc::IntersectionErrors::setRecovery ( m_recovery ) ;
  if (  DetDesc::IntersectionErrors::recovery() ) {
    info() << "Recovery of geometry errors is  ENABLED" << endmsg; }
  else {
    info() << "Recovery of geometry errors is DISABLED" << endmsg; }

  // Load geometry
  m_standardGeometry = findGeometry( m_standardGeometry_address ) ;

  return StatusCode::SUCCESS;
}
// ============================================================================
// Implementation of finalize() method
// ============================================================================
StatusCode TransportSvc::finalize()
{
  //

  { // skip map
    always() << " GEOMETRY ERRORS: 'Skip'     map has the size " << m_skip.size() << std::endl ;
    if ( !m_skip.empty() )
    {
      boost::format fmt1 ( "   | %1$=55.55s |    | %2$-65.65s |" ) ;
      fmt1 % " Logical Volume " ;
      fmt1 % " #          mean             RMS          min              max" ;
      msgStream() << fmt1.str() << std::endl ;
      for ( auto i = m_skip.begin() ; m_skip.end() != i ; ++i )
      {
        {
          std::ostringstream ost;
          i->second.first.printFormatted( ost , "%1$-7d %3$-14.8g %4$14.8g %5$-14.8g %6$14.8g" );
          boost::format fmt2 ( "   | %1$-55.55s | mm | %2$-65.65s |" ) ;
          fmt2 % logVol ( i->first , 55 ) % ost.str();
          msgStream() << fmt2.str() << std::endl ;
        }
        {
          std::ostringstream ost;
          i->second.second.printFormatted( ost , "%1$-7d %3$-14.8g %4$14.8g %5$-14.8g %6$14.8g" );
          boost::format fmt2 ( "   | %1$-55.55s | X0 | %2$-65.65s |" ) ;
          fmt2 % logVol ( i->first , 55 ) % ost.str();
          msgStream() << fmt2.str() << std::endl ;
        }
      }
    }
  }
  msgStream() << endmsg;

  // recovery map
  {
    always() << " GEOMETRY ERRORS: 'Recover'  map has the size " << m_recover.size() << std::endl ;
    if ( !m_recover.empty() )
    {
      boost::format fmt1 ( "   | %1$=55.55s |    | %2$-65.65s |" ) ;
      fmt1 % " Logical Volume " ;
      fmt1 % " #          mean             RMS          min              max" ;
      msgStream() << fmt1.str() << std::endl ;
      for ( Map::const_iterator i = m_recover.begin() ; m_recover.end() != i ; ++i )
      {
        {
          std::ostringstream ost;
          i->second.first.printFormatted( ost , "%1$-7d %3$-14.8g %4$14.8g %5$-14.8g %6$14.8g" );
          boost::format fmt2 ( "   | %1$-55.55s | mm | %2$-65.65s |" ) ;
          fmt2 % logVol ( i->first , 55 ) % ost.str() ;
          msgStream() << fmt2.str() << std::endl ;
        }
        {
          std::ostringstream ost;
          i->second.second.printFormatted( ost ,  "%1$-7d %3$-14.8g %4$14.8g %5$-14.8g %6$14.8g" ) ;
          boost::format fmt2 ( "   | %1$-55.55s | X0 | %2$-65.65s |" ) ;
          fmt2 % logVol ( i->first , 55 ) %ost.str() ;
          msgStream() << fmt2.str() << std::endl ;
        }
      }
    }
  }
  msgStream() << endmsg;

  // codes:
  {
    always() << " GEOMETRY ERRORS: 'Codes'    map has the size " << m_codes.size() << std::endl ;
    if ( !m_codes  .empty() )
    {
      boost::format fmt1 ( "   | %1$=55.55s | %2$=10.10s |" ) ;
      fmt1 % " Logical Volume " ;
      fmt1 % "#errors" ;
      msgStream() << fmt1.str() << std::endl ;
      for ( Map1::const_iterator i = m_codes.begin() ; m_codes.end() != i ; ++i )
      {
        {
          boost::format fmt2 ( "   | %1$-55.55s | %2$=10d |" ) ;
          fmt2 % logVol ( i->first , 55 ) ;
          fmt2 %              i->second ;
          msgStream() << fmt2.str() << std::endl ;
        }
      }
    }
  }
  msgStream() << endmsg;

  if ( this == DetDesc::IntersectionErrors::service() )
  {
    info() << "Reset the static pointer to DetDesc::IGeometyrErrorSvc" << endmsg ;
    DetDesc::IntersectionErrors::setService ( 0 ) ;
  }
  ///
  { // uncatched errors ?
    const unsigned long nErrors = DetDesc::IntersectionErrors::errors ()  ;
    if ( 0 != nErrors  )
    {
      error() << "DetDesc::IntersectionErrors has "
              << nErrors << " errors " << endmsg ;
      error() << "Rerun the job with activated error handling!" << endmsg ;
    }
  }
  ///
  {
    /// release all services
    /// release Detector Data  Service
    m_detDataSvc.reset();
  }
  ///
  if ( msgLevel(MSG::DEBUG) )
    debug() << "Service finalised successfully" << endmsg;
  ///
  return Service::finalize();
  ///
}
// ============================================================================
IGeometryInfo*       TransportSvc::findGeometry
( const std::string& address ) const
{
  ///
  IGeometryInfo* gi = nullptr ;
  ///
  try
    {
      SmartDataPtr<IDetectorElement> detelem( detSvc() , address );
      if( detelem ) { gi = detelem->geometry() ; }
    }
  catch(...)
    { Assert( false ,
              "TransportSvc::findGeometry(), unknown exception caught!" ); }
  ///
  if( !gi )
    throw TransportSvcException(
          "TransportSvc::unable to locate geometry address="+address,
          StatusCode::FAILURE );
  ///
  return gi;
}
// ============================================================================
// Implementations of additional methods
// ============================================================================
#include "TransportSvcDistanceInRadUnits.h"
#include "TransportSvcFindLocalGI.h"
#include "TransportSvcGoodLocalGI.h"
#include "TransportSvcIntersections.h"
// ============================================================================

// Create an instance of the accelerator cache
ranges::v3::any TransportSvc::createCache() const { return AccelCache{}; }

// ============================================================================
// Implementation of interface DetDesc::IGeometryErrorSvc
// ============================================================================
/*  set/reset the current "status" of geometry erorrs
 *  @param sc the status code
 *  @param volume the volume
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-12-14
 */
void TransportSvc::setCode
( const StatusCode& sc     ,
  const ILVolume*   volume )
{
  if ( sc.isSuccess()                            ) { return ; } // nothing to do
  if ( !m_protocol && MSG::DEBUG > outputLevel() ) { return ; } //

  if ( volume ) { ++m_codes [ volume->name() ] ; }
  else          { ++m_codes [ ""             ] ; }
}
// ===========================================================================
/*  inspect the potential error in intersections
 *  @param  volume   the problematic volume
 *  @param  point    3D point
 *  @param  vect     3D direction vector
 *  @param  cnt  the problematic container of intersections
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-12-14
 */
// ============================================================================
void TransportSvc::inspect
( const ILVolume*                volume ,
  const Gaudi::XYZPoint&         pnt    ,
  const Gaudi::XYZVector&        vect   ,
  const ILVolume::Intersections& cnt    )
{


  error() << " DetDesc::IntersectionError for LV='" << volume->name()  <<"'" << endmsg ;
  error() << " Local Frame: "
          << " point="       << pnt
          << "/vector="      << vect
          << " Tick Length=" << vect.R() << endmsg ;

  boost::format fmt1
    ( "| %1$1.1s |%2$=15.15s %3$=15.15s|%4$=27.27s %5$=27.27s|%6$=3s %7$=16.16s | %8$=16.16s |" ) ;
  fmt1 % "?"             ; // problems ?
  fmt1 % "tickMin"       ;
  fmt1 % "tickMax"       ;
  fmt1 % "point1"        ;
  fmt1 % "point2"        ;
  fmt1 % "#"             ;
  fmt1 % "phys.volume"   ;
  fmt1 % "material"      ;
  error() << fmt1.str() << endmsg ;

  for ( auto i = cnt.begin() ; cnt.end() != i ; ++i )
  {
    // check the intersections:
    bool ok = true ;
    for ( auto i2 = cnt.begin() ;
          ok && cnt.end() != i2 ; ++i2 )
    {
      if ( i2 == i ) { continue ; }
      ok = ( 0 != VolumeIntersectionIntervals::intersect ( *i , *i2 ) ) ;
    }
    boost::format fmt
      ( "| %1$1.1s |%2$15.9g %3$-15.9g|%4$27.27s %5$-27.27s|%6$=3d %7$16.16s | %8$16.16s |" ) ;
    fmt % ( ok ? " " : "*" )                 ; // (1) problems?
    fmt %         i -> first.first           ; // (2) tick min
    fmt %         i -> first.second          ; // (3) tick max
    fmt % ( pnt + i -> first.first  * vect ) ; // (4) the first point
    fmt % ( pnt + i -> first.second * vect ) ; // (5) the second point

    // get the central point:
    Gaudi::XYZPoint c = pnt + 0.5 * ( i -> first.first + i -> first.second ) * vect ;
    ILVolume::PVolumePath pvs ;
    volume -> belongsTo ( c , 1 , pvs ) ;
    if ( pvs.empty() || 0 == pvs.front() )
    {
      fmt % -1          ;
      fmt % "<mother>"  ;
    }
    else
    {
      const ILVolume::PVolumes& volumes = volume->pvolumes() ;
      ILVolume::PVolumes::const_iterator ipv =
        std::find ( volumes.begin() , volumes.end() , pvs.front() ) ;
      if ( volumes.end() == ipv ) { fmt % -1 ; }
      else { fmt % ( ipv - volumes.begin() ) ; }
      fmt % pvs.front()->name() ;
    }
    const std::string& n = i->second->name() ;
    if ( 16 >= n.size()  ) { fmt %  n ; } // (6) the material name
    else { fmt %  std::string ( n , n.size() - 16 ) ; } // (6) the material name

    //
    error() << fmt.str() << endmsg ;
  }
}
// ============================================================================
/* report the recovered action in intersections
 *  @param  volume    the problematic volume
 *  @param  material1 the affected material
 *  @param  material2 the affected material
 *  @param  delta    the problematic delta  (non-negative!)
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-12-14
 */
// ============================================================================
void TransportSvc::recovered
( const ILVolume* volume    ,
  const Material* material1 ,
  const Material* material2 ,
  const double    delta     )
{
  if ( !m_protocol && MSG::DEBUG > outputLevel() ) { return ; } // RETURN

  Map::mapped_type& p = m_recover [ volume->name() ] ;

  p.first  += delta ;
  p.second +=
    std::abs
    ( delta / material1 -> radiationLength () -
      delta / material2 -> radiationLength () ) ;
}
// ============================================================================
/* report the skipped intersection
 *  @param  volume   the problematic volume
 *  @aram   material the affected material
 *  @param  delta    the problematic delta  (non-negative!)
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-12-14
 */
// ============================================================================
void TransportSvc::skip
( const ILVolume* volume   ,
  const Material* material ,
  const double    delta    )
{
  if ( !m_protocol && MSG::DEBUG > outputLevel() ) { return ; } // RETURN

  Map::mapped_type& p = m_skip [ volume->name() ] ;

  p.first  += delta ;
  p.second += delta / material->radiationLength() ;
}
// ============================================================================


// ============================================================================
DECLARE_COMPONENT( TransportSvc )
// ============================================================================
// The END
// ============================================================================
