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
StatusCode LAssembly::belongsTo( const Gaudi::XYZPoint&   LocalPoint  ,
                                 const int                Level       ,
                                 ILVolume::PVolumePath&   pVolumePath ) const
{
  /// check the depth
  if( 0 == Level ) { return StatusCode::SUCCESS; }
  /// check the point
  const IPVolume* pv = insideDaughter( LocalPoint ).first ;
  if( !pv ) { return StatusCode::FAILURE; }
  /// look for daughters
  if( !pv->lvolume() )
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
StatusCode LAssembly::belongsTo( const Gaudi::XYZPoint&   LocalPoint  ,
                                 const int                Level       ,
                                 ILVolume::ReplicaPath&   replicaPath ) const
{
  /// check the depth
  if( 0 == Level     ) { return StatusCode::SUCCESS; }
  /// check the point
  auto ppi = insideDaughter( LocalPoint );
  const auto* pv = ppi.first;
  if( !pv ) { return StatusCode::FAILURE; }
  /// look for daughters
  if( 0 == pv->lvolume() )
  { replicaPath.clear() ; return StatusCode::FAILURE; }
  /// get replica number
  ILVolume::ReplicaType replica = ppi.second;
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
unsigned int LAssembly::intersectLine( const Gaudi::XYZPoint   & Point   ,
                                       const Gaudi::XYZVector  & Vector  ,
                                       ILVolume::Intersections & intersections,
                                       const double              threshold    ) const
{
  /// clear the container
  intersections.clear();
  /// line with null direction vector is not able to intersect any volume
  if( Vector.mag2() <= 0 ) { return 0 ; }       // RETURN !!!
  /// intersections with childrens
  intersectDaughters( Point     , Vector    , intersections  , threshold );
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
unsigned int LAssembly::intersectLine( const Gaudi::XYZPoint&     Point        ,
                                       const Gaudi::XYZVector&    Vector       ,
                                       ILVolume::Intersections  & intersections,
                                       const ISolid::Tick         tickMin      ,
                                       const ISolid::Tick         tickMax      ,
                                       const double               Threshold    ) const
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
  if ( zMin() > p1.z() && zMin() > p2.z() ) return 0 ;
  if ( zMax() < p1.z() && zMax() < p2.z() ) return 0 ;
  if ( xMin() > p1.x() && xMin() > p2.x() ) return 0 ;
  if ( xMax() < p1.x() && xMax() < p2.x() ) return 0 ;
  if ( yMin() > p1.y() && yMin() > p2.y() ) return 0 ;
  if ( yMax() < p1.y() && yMax() < p2.y() ) return 0 ;

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
void LAssembly::updateCover(const IPVolume& pv) {
  const ISolid* mySolid = pv.lvolume()->solid();
  //== if it is a Solid, it has a cover -- otherwise, it is an assembly and thus an ISolid and an IBoxCover
  const IBoxCover* boxCover = (mySolid ? mySolid->cover() : dynamic_cast<const IBoxCover*>(pv.lvolume()));
  if (!boxCover) {
    MsgStream log ( msgSvc() , "TransportSvc" );
    log << MSG::ERROR << " === No cover for assembly " << name()
        << " pv " << pv.name() << endmsg;
  } else {
    //== Compute the 8 corners, transform to mother frame and build the
    //== envelop as a box (x,y,z Min/Max)
    for (auto pointX : { boxCover->xMin(), boxCover->xMax() } ) {
      for (auto pointY : { boxCover->yMin(), boxCover->yMax() } ) {
        for (auto pointZ : { boxCover->zMin(), boxCover->zMax() } ) {
          auto motherPt = pv.toMother(Gaudi::XYZPoint(pointX,pointY,pointZ));
          if ( xMin() > motherPt.x() ) setXMin(motherPt.x());
          if ( xMax() < motherPt.x() ) setXMax(motherPt.x());
          if ( yMin() > motherPt.y() ) setYMin(motherPt.y());
          if ( yMax() < motherPt.y() ) setYMax(motherPt.y());
          if ( zMin() > motherPt.z() ) setZMin(motherPt.z());
          if ( zMax() < motherPt.z() ) setZMax(motherPt.z());
        }
      }
    }
  }
}
// ============================================================================
// The End
// ============================================================================
