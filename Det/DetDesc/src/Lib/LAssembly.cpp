// $Id: LAssembly.cpp,v 1.19 2009-04-17 08:54:24 cattanem Exp $
// ============================================================================
// Include files
// ============================================================================
// DetDesc
// ============================================================================
#include "DetDesc/LAssembly.h"
#include "DetDesc/SolidBase.h"
#include "DetDesc/VolumeIntersectionIntervals.h"
// ============================================================================
/** @file LAssembly.cpp
 *
 *  Implementation file for class : LAssembly
 * 
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @author Sebastien Ponce
 */
// ============================================================================
/* class/object identifier (static method)
 *  @return unique class identifier 
 */
// ============================================================================
const CLID& LAssembly::classID ()           { return CLID_LAssembly        ; }
// ============================================================================
/*  class/object identification (virtual method)
 *  @return unique class identifier 
 */      
// ============================================================================
const CLID& LAssembly::clID    ()     const { return LAssembly::classID()  ; }
// ============================================================================
/*  calculate the daughter path containing the Point in Local frame , 
 *  can be VERY slow for complex geometry, 
 *  therefore use the appropriate Level for usage 
 *  @see ILVolume 
 *  @param  localPoint point in local reference system of logical volume 
 *  @param  level depth level
 *  @param  volumePath  vector of physical volumes
 *  @return status code 
 */
// ============================================================================
StatusCode LAssembly::belongsTo
( const Gaudi::XYZPoint&        LocalPoint  ,
  const int                Level       , 
  ILVolume::PVolumePath&   pVolumePath ) const 
{    
  /// check the depth 
  if( 0 == Level ) { return StatusCode::SUCCESS; } 
  /// check the point 
  ILVolume::PVolumes::const_iterator ppi = 
    insideDaughter( LocalPoint ) ;
  if( pvEnd() == ppi ) { return StatusCode::FAILURE; } 
  /// look for daughters 
  const IPVolume* pv = *ppi;
  if( 0 == pv || 0 == pv->lvolume() ) 
  { pVolumePath.clear() ; return StatusCode::FAILURE; } 
  /// add volume to the path 
  pVolumePath.push_back( pv ) ;
  /// recursion 
  return pv->lvolume()->belongsTo( pv->matrix() * LocalPoint , 
                                   Level - 1                 , 
                                   pVolumePath               );
}
// ============================================================================
/*  calculate the daughter path containing the Point in Local frame , 
 *  can be VERY slow for complex geometry, 
 *  therefore use the appropriate Level for usage 
 *  @see ILVolume 
 *  @param  localPoint point in local reference system of logical volume 
 *  @param  level depth level
 *  @param  volumePath  vector of physical volumes
 *  @return status code 
 */
// ============================================================================
StatusCode LAssembly::belongsTo
( const Gaudi::XYZPoint&        LocalPoint  ,
  const int                Level       , 
  ILVolume::ReplicaPath&   replicaPath ) const  
{    
  /// check the depth 
  if( 0 == Level     ) { return StatusCode::SUCCESS; } 
  /// check the point 
  ILVolume::PVolumes::const_iterator ppi = 
    insideDaughter( LocalPoint ) ;
  if( pvEnd() == ppi ) { return StatusCode::FAILURE; } 
  /// look for daughters 
  const IPVolume* pv = *ppi;
  if( 0 == pv || 0 == pv->lvolume() ) 
  { replicaPath.clear() ; return StatusCode::FAILURE; } 
  /// get replica number 
  ILVolume::ReplicaType replica = ppi - pvBegin();
  /// add volume to the path 
  replicaPath.push_back( replica ) ;
  /// recursion 
  return pv->lvolume()->belongsTo( pv->matrix() * LocalPoint , 
                                   Level - 1                 , 
                                   replicaPath               );
}
// ============================================================================
/*  intersection of the logical volume with with the line \n
 *  The line is parametrized in the local reference system 
 *  of the logical volume by initial Point and direction Vector \n 
 *  @f$ \vec{x}(t) = \vec{p} + t\times \vec{v} @f$  \n 
 * 
 *   Method returns the number of intersection points("ticks") and 
 *   the container of pairs - ticks and pointer to the corresponding 
 *   material. The simplification is determined by value of threshold
 *   (in units of radiation length) 
 *   Method throws LVolumeException in the case, then 
 *   solid is not defined or material is not accessible.
 *
 *  @see ILVolume 
 *  @exception LogVolumeException solid or/and matherial problems 
 *  @param Point initial point at the line
 *  @param Vector direction vector of the line
 *  @param intersections output container 
 *  @param threshold threshold value 
 *  @return number of intersections  
 */
// ============================================================================
unsigned int LAssembly::intersectLine
( const Gaudi::XYZPoint        & Point         , 
  const Gaudi::XYZVector       & Vector        , 
  ILVolume::Intersections & intersections , 
  const double              threshold     ) const 
{
  /// clear the container 
  intersections.clear();
  /// line with null direction vector is not able to intersect any volume 
  if( Vector.mag2() <= 0 ) { return 0 ; }       // RETURN !!!
  /// intersections with childrens 
  intersectDaughters
    ( Point     , Vector    , intersections  , threshold );
  ///
  return intersections.size();  ///< RETURN!!!
}
// ============================================================================
/*  intersection of the logical volume with with the line \n
 *  Theine is parametrized in the local reference system 
 *  of the logical volume by initial Point and direction Vector \n 
 *  @f$ \vec{x}(t) = \vec{p} + t\times \vec{v} @f$  \n 
 * 
 *   Method returns the number of intersection points("ticks") and 
 *   the container of pairs - ticks and pointer to the corresponding 
 *   material. The simplification is determined by value of threshold
 *   (in units of radiation length) 
 *   Method throws LVolumeException in the case, then 
 *   solid is not defined or material is not accessible.
 *
 *  @see ILVolume 
 *  @exception LogVolumeException solid or/and matherial problems 
 *  @param Point initial point at the line
 *  @param Vector direction vector of the line
 *  @param intersections output container
 *  @param tickMin minimum value of possible Tick
 *  @param tickMax maximum value of possible Tick
 *  @param threshold threshold value 
 *  @return number of intersections  
 */
// ============================================================================
unsigned int LAssembly::intersectLine
( const Gaudi::XYZPoint         & Point         , 
  const Gaudi::XYZVector        & Vector        , 
  ILVolume::Intersections  & intersections , 
  const ISolid::Tick         tickMin       , 
  const ISolid::Tick         tickMax       , 
  const double               Threshold     ) const
{
  /// clear the output container 
  intersections.clear();
  /// check the valid tick values 
  if( tickMin >= tickMax ) { return 0 ;} 
  /* line with null direction vector 
   * is not able to intersect any volume
   */
  if( Vector.mag2() <= 0 ) { return 0 ; }       // RETURN !!!  
  
  Gaudi::XYZPoint p1 = Point + tickMin * Vector;
  Gaudi::XYZPoint p2 = Point + tickMax * Vector;
  if ( (m_zMin > p1.z()) && (m_zMin > p2.z()) ) return 0 ;
  if ( (m_zMax < p1.z()) && (m_zMax < p2.z()) ) return 0 ;
  if ( (m_xMin > p1.x()) && (m_xMin > p2.x()) ) return 0 ;
  if ( (m_xMax < p1.x()) && (m_xMax < p2.x()) ) return 0 ;
  if ( (m_yMin > p1.y()) && (m_yMin > p2.y()) ) return 0 ;
  if ( (m_yMax < p1.y()) && (m_yMax < p2.y()) ) return 0 ;
  
  /*  look for the intersections of the given 
   *  line with daughter elements construct the 
   *  intersections container for daughter volumes
   */
  intersectDaughters
    ( Point , Vector , intersections , tickMin , tickMax , Threshold  );
  
  return intersections.size();    ///< RETURN!!!
}
// ============================================================================
/*  printout to STD/STL stream
 *  @see ILVolume 
 *  @param os STD/STL stream
 *  @return reference to the stream
 */
// ============================================================================
std::ostream& LAssembly::printOut 
( std::ostream & os             ) const 
{ return LogVolBase::printOut( os ) ; }
// ============================================================================
/*  printout to Gaudi MsgStream stream
 *  @see ILVolume 
 *  @param os Gaudi MsgStream  stream
 *  @return reference to the stream
 */
// ============================================================================
MsgStream&    LAssembly::printOut
( MsgStream    & os             ) const
{ return LogVolBase::printOut( os ) ; }
// ============================================================================
//  
// =============================================================================
void LAssembly::updateCover(const IPVolume* const pv) {
  const IBoxCover* boxCover = 0;
  const ISolid* mySolid = pv->lvolume()->solid();
  if ( 0 != mySolid ) {  //== Solid => has a cover
    boxCover = mySolid->cover();
  } else {  //== No solid : This is an assembly
    boxCover = dynamic_cast<const LAssembly*>(pv->lvolume());
  }
  if (!boxCover) {
    MsgStream log ( msgSvc() , "TransportSvc" );
    log << MSG::ERROR << " === No cover for assembly " << name() 
        << " pv " << pv->name() << endmsg;
  } else {
    //== Compute the 8 corners, transform to mother frame and build the 
    //== envelop as a box (x,y,z Min/Max)
    double pointX=boxCover->xMin();
    for (int i = 0 ; 2 > i ; ++i ) {
      double pointY=boxCover->yMin();
      for (int j = 0 ; 2 > j ; ++j ) {
        double pointZ=boxCover->zMin();
        for (int k = 0 ; 2 > k ; ++k ) {
          auto motherPt = pv->toMother(Gaudi::XYZPoint(pointX,pointY,pointZ));
          if ( m_xMin > motherPt.x() ) m_xMin = motherPt.x();
          if ( m_xMax < motherPt.x() ) m_xMax = motherPt.x();
          if ( m_yMin > motherPt.y() ) m_yMin = motherPt.y();
          if ( m_yMax < motherPt.y() ) m_yMax = motherPt.y();
          if ( m_zMin > motherPt.z() ) m_zMin = motherPt.z();
          if ( m_zMax < motherPt.z() ) m_zMax = motherPt.z();
          pointZ=boxCover->zMax();
        }
        pointY = boxCover->yMax();
      }
      pointX = boxCover->xMax();
    }
  }
}
// ============================================================================
// The End 
// ============================================================================
