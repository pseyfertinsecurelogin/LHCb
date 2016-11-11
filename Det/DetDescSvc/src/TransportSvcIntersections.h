// $Id: TransportSvcIntersections.h,v 1.5 2007-09-13 14:19:16 wouter Exp $ 
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $
// ============================================================================
#ifndef     __DETDESC_TRANSPORTSVC_TRANSPORTSVCINTERSECTIONS_H__ 
#define     __DETDESC_TRANSPORTSVC_TRANSPORTSVCINTERSECTIONS_H__  1

#include "TransportSvc.h" 

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
  return this->intersections_r( point, vect, tickMin, tickMax,
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
  AccelCache&              accelCache          ,
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
 
    //
    Gaudi::XYZPoint point1( point + vect * tickMin ) ; 
    Gaudi::XYZPoint point2( point + vect * tickMax ) ; 
    // check - if the previous paramaters are the same
    if ( point1              == accelCache.prevPoint1          && 
         point2              == accelCache.prevPoint2          &&
         threshold           == accelCache.previousThreshold   &&
         topGeometry         == accelCache.previousTopGeometry && 
         guessGeometry       == accelCache.previousGuess        ) 
    {
      /// use cached container!!!
      intersept.reserve( accelCache.localIntersections.size() ); 
      intersept.insert( intersept.begin(),
                        accelCache.localIntersections.begin() , 
                        accelCache.localIntersections.end() ); 
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
      ( accelCache.previousGeometry && topGeometry == accelCache.previousTopGeometry &&
	( giLocal = findLocalGI( point1 , 
                                 point2 , 
                                 accelCache.previousGeometry ,
                                 topGeometry ) ) ) ? 
      // just take the top geometry
      accelCache.previousGeometry :
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
    accelCache.previousGeometry = giLocal;
    
    /// make local copy of all parameters:    
    accelCache.prevPoint1           = point1              ;
    accelCache.prevPoint2           = point2              ; 
    accelCache.previousThreshold    = threshold           ;
    accelCache.previousGuess        = guessGeometry       ;
    accelCache.previousTopGeometry  = topGeometry ;
    
    /// intersections 
    accelCache.localIntersections.clear();
    accelCache.localIntersections.reserve( intersept.size() ); 
    accelCache.localIntersections.insert( accelCache.localIntersections.begin(),
                                          intersept.begin() , 
                                          intersept.end() );
    
  }
  
  catch ( const GaudiException& Exception ) 
  {
    /// 1) reset cache: 
    accelCache.prevPoint1           = Gaudi::XYZPoint() ;
    accelCache.prevPoint2           = Gaudi::XYZPoint() ; 
    accelCache.previousThreshold    = -1000.0      ; 
    accelCache.previousGuess        = 0            ; 
    accelCache.previousTopGeometry  = 0            ;
    accelCache.localIntersections.clear()          ; 

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
    accelCache.prevPoint1           = Gaudi::XYZPoint() ;
    accelCache.prevPoint2           = Gaudi::XYZPoint() ; 
    accelCache.previousThreshold    = -1000.0      ; 
    accelCache.previousGuess        = 0            ; 
    accelCache.previousTopGeometry  = 0            ;
    accelCache.localIntersections.clear()          ; 

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
