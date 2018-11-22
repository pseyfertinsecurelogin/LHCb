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

#ifndef     __DETDESC_TRANSPORTSVC_TRANSPORTSVCINTERSECTIONS_H__
#define     __DETDESC_TRANSPORTSVC_TRANSPORTSVCINTERSECTIONS_H__  1

#include "TransportSvc.h"
#include "DetDesc/IGeometryInfo.h"

// ============================================================================
/** @file TransportSvcIntersections.h
 *
 *  a simple implementation of TransportSvc::intersection method
 *  @see TransportSvc
 *  @see ITransportSvc
 *  @see ILVolume
 *  @see IPVolume
 *  @see ISolid
 *
 *  @author: Vanya Belyaev
 */
// ============================================================================

/** general method ( returns the "full history" of the volume
   *  boundary intersections
   * with different material properties between 2 points )
   *  @see ITransportSvc
   *  @see IGeometryInfo
   *  @see ILVolume
   *  @param point               initial point on the line
   *  @param vect                direction vector of the line
   *  @param tickMin             minimal value of line paramater
   *  @param tickMax             maximal value of line parameter
   *  @param intersept           (output) container of intersections
   *  @param threshold           threshold value
   *  @param alternativeGeometry source of alternative geometry information
   *  @param geometryGuess       a guess for navigation
   */
unsigned long TransportSvc::intersections
( const Gaudi::XYZPoint&   point               ,
  const Gaudi::XYZVector&  vect                ,
  const ISolid::Tick&      tickMin             ,
  const ISolid::Tick&      tickMax             ,
  ILVolume::Intersections& intersept           ,
  double                   threshold           ,
  IGeometryInfo*           alternativeGeometry ,
  IGeometryInfo*           geometryGuess       )  const
{
  return intersections_r( point, vect, tickMin, tickMax,
                          intersept, m_accelCache, threshold,
                          alternativeGeometry, geometryGuess );
}

// ============================================================================
/** general method ( returns the "full history" of the volume
 *  boundary intersections
 *  with different material properties between 2 points )
 *  Similar to intersections but with an additional accelerator
 *  cache for local client storage. This method, unlike the above
 *  @see ITransportSvc
 *  @see IGeometryInfo
 *  @see ILVolume
 *  @param Point               initial point on the line
 *  @param Vector              direction vector of the line
 *  @param TickMin             minimal value of line paramater
 *  @param TickMax             maximal value of line parameter
 *  @param Intersept           (output) container of intersections
 *  @param Threshold           threshold value
 *  @param AlternativeGeometry source of alternative geometry information
 *  @param GeometryGuess       a guess for navigation
 */
// ============================================================================
unsigned long TransportSvc::intersections_r
( const Gaudi::XYZPoint&   point               ,
  const Gaudi::XYZVector&  vect                ,
  const ISolid::Tick&      tickMin             ,
  const ISolid::Tick&      tickMax             ,
  ILVolume::Intersections& intersept           ,
  std::any&         accelCache          ,
  double                   threshold           ,
  IGeometryInfo*           alternativeGeometry ,
  IGeometryInfo*           guessGeometry       ) const
{

  try {

    intersept.clear();

    /// check the input parameters of the line
    if ( tickMin >= tickMax && vect.mag2() <= 0 ) { return 0;}

    // Set the top level geometry, because this is what is in the cache
    auto * topGeometry = alternativeGeometry ? alternativeGeometry : standardGeometry() ;

    // get the cache
    auto & cache = std::any_cast<AccelCache&>(accelCache);

    Gaudi::XYZPoint point1( point + vect * tickMin ) ;
    Gaudi::XYZPoint point2( point + vect * tickMax ) ;
    // check - if the previous paramaters are the same
    if ( point1              == cache.prevPoint1          &&
         point2              == cache.prevPoint2          &&
         threshold           == cache.previousThreshold   &&
         topGeometry         == cache.previousTopGeometry &&
         guessGeometry       == cache.previousGuess        )
    {
      /// use cached container!!!
      intersept.reserve( cache.localIntersections.size() );
      intersept.insert( intersept.begin(),
                        cache.localIntersections.begin() ,
                        cache.localIntersections.end() );
      ///
      return intersept.size();
    }

    /** locate the both points inside one "good"
     *  DetectorElement/GeometryInfo objects
     *
     * "Good" in this context means that
     *
     *  - 1) both points should be "inside"
     *  - 2) it has the associated Logical Volume (it is not "ghost")
     *  - 3) Logical Volume is not an assembly!
     *  - 4) the line between tickMin and tickMax do not cross
     *    the Logical Volume ("no holes between") requirement
     */

    IGeometryInfo* giLocal = nullptr ;
    IGeometryInfo* gi      =
      // try the guess geometry
     ( guessGeometry       &&
       ( giLocal = findLocalGI( point1 ,
                                point2 ,
                                guessGeometry ,
                                alternativeGeometry ) ) ) ?
      guessGeometry       :
      // try the previous geometry (only if top-geometry matches)
      ( cache.previousGeometry && topGeometry == cache.previousTopGeometry &&
	( giLocal = findLocalGI( point1 ,
                                 point2 ,
                                 cache.previousGeometry ,
                                 topGeometry ) ) ) ?
      // just take the top geometry
      cache.previousGeometry :
      ( giLocal = findLocalGI( point1 ,
                               point2 ,
                               topGeometry,
                               topGeometry ) ) ?

      topGeometry : nullptr ;

    ///
    if ( !giLocal )
      throw TransportSvcException( "TransportSvc::(1) unable to locate points",
                                   StatusCode::FAILURE );
    if ( !gi )
      throw TransportSvcException( "TransportSvc::(2) unable to locate points",
                                   StatusCode::FAILURE );

    /// delegate the calculation to the logical volume
    const auto * lv = giLocal->lvolume();
    lv->intersectLine
      ( giLocal->toLocalMatrix() * point ,
        giLocal->toLocalMatrix() * vect  ,
        intersept                 ,
        tickMin                   ,
        tickMax                   ,
        threshold                 );

    /// redefine previous geometry
    cache.previousGeometry = giLocal;

    /// make local copy of all parameters:
    cache.prevPoint1           = point1              ;
    cache.prevPoint2           = point2              ;
    cache.previousThreshold    = threshold           ;
    cache.previousGuess        = guessGeometry       ;
    cache.previousTopGeometry  = topGeometry ;

    /// intersections
    cache.localIntersections.clear();
    cache.localIntersections.reserve( intersept.size() );
    cache.localIntersections.insert( cache.localIntersections.begin(),
                                     intersept.begin() ,
                                     intersept.end() );

  }

  catch ( const GaudiException& Exception )
  {
    /// 1) reset cache:
    auto & cache = std::any_cast<AccelCache&>(accelCache);
    cache.prevPoint1           = Gaudi::XYZPoint() ;
    cache.prevPoint2           = Gaudi::XYZPoint() ;
    cache.previousThreshold    = -1000.0      ;
    cache.previousGuess        = 0            ;
    cache.previousTopGeometry  = 0            ;
    cache.localIntersections.clear()          ;

    ///  2) throw new exception:
    std::string message
      ("TransportSvc::intersection(...), exception caught; Params: ");
    {
      std::ostringstream ost;
      ost << "Point=" << point
          << ",Vect=" << vect
          << ",Tick=" << tickMin << "/" << tickMax
          << "Thrsh=" << threshold;
      if ( alternativeGeometry ) { ost << "A.Geo!=NULL" ; }
      if ( guessGeometry ) { ost << "G.Geo!=NULL" ; }
      message += ost.str();
      Assert( false , message , Exception );
    }
  }
  catch( ... )
  {
    /// 1) reset cache:
    auto & cache = std::any_cast<AccelCache&>(accelCache);
    cache.prevPoint1           = Gaudi::XYZPoint() ;
    cache.prevPoint2           = Gaudi::XYZPoint() ;
    cache.previousThreshold    = -1000.0      ;
    cache.previousGuess        = 0            ;
    cache.previousTopGeometry  = 0            ;
    cache.localIntersections.clear()          ;

    /// 2) throw new exception:
    std::string message
      ("TransportSvc::intersection(...), unknown exception caught; Params: ");
    {
      std::ostringstream ost;
      ost << "Point=" << point
          << ",Vect=" << vect
          << ",Tick=" << tickMin << "/" << tickMax
          << "Thrsh=" << threshold;
      if ( alternativeGeometry ) { ost << "A.Geo!=NULL" ; }
      if ( guessGeometry ) { ost << "G.Geo!=NULL" ; }
      message += ost.str();
      Assert( false , message );
    }
  }

  return intersept.size() ;
  ///
}

// ============================================================================
// The End
// ============================================================================
#endif  //  __DETDESC_TRANSPORTSVC_TRANSPORTSVCINTERSECTIONS_H__
// ============================================================================
