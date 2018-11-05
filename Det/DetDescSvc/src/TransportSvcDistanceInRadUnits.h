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
#ifndef    __DETDESC_TRANSPORTSVC_TRASNPORTSVCDISTANCEINRADUNITS_H__
#define    __DETDESC_TRANSPORTSVC_TRASNPORTSVCDISTANCEINRADUNITS_H__ 1

// DetDesc
#include "DetDesc/VolumeIntersectionIntervals.h"

// local
#include "TransportSvc.h"

// ============================================================================
/** @file
 *
 * simple implementation of  TrnasportSvc::distanceInRadUnits method
 * @see TransportSvc
 * @see ITransportSvc
 *
 * @author: Vanya Belyaev
 */
// ============================================================================

/** Estimate the distance between 2 points in
 *  units of radiation length units
 *  @see ITransportSvc
 *  @param Point1 first point
 *  @param Point2 second point
 *  @param Threshold threshold value
 *  @param AlternativeGeometry  source ot alternative geomtry
 *  @param GeometryGuess guess foe geometry
 *  @return distance in rad length units
 */
double TransportSvc::distanceInRadUnits
( const Gaudi::XYZPoint& Point1         ,
  const Gaudi::XYZPoint& Point2         ,
  double            Threshold           ,
  IGeometryInfo*    AlternativeGeometry ,
  IGeometryInfo*    GeometryGuess       ) const
{
  return distanceInRadUnits_r( Point1, Point2, m_accelCache, Threshold,
                               AlternativeGeometry, GeometryGuess );
}
// ============================================================================
/** Estimate the distance between 2 points in
 *  units of radiation length units
 *  Similar to distanceInRadUnits but with an additional accelerator
 *  cache for local client storage. This method, unlike distanceInRadUnits
 *  is re-entrant and thus thread safe.
 *  @see ITransportSvc
 *  @param point1 first point
 *  @param point2 second point
 *  @param threshold threshold value
 *  @param alternativeGeometry  source ot alternative geomtry
 *  @param geometryGuess guess for geometry
 *  @return distance in rad length units
 */
// ============================================================================
double TransportSvc::distanceInRadUnits_r
( const Gaudi::XYZPoint& point1         ,
  const Gaudi::XYZPoint& point2         ,
  std::any&       accelCache     ,
  double            threshold           ,
  IGeometryInfo*    alternativeGeometry ,
  IGeometryInfo*    geometryGuess       )  const
{
  // check for the  distance
  if ( point1 == point2 ) { return 0; }

  // retrieve the history
  const Gaudi::XYZVector Vector( point2 - point1 ) ;

  // initial point on the line
  // direction vector of the line
  // minimal value of the parameter of the line
  // maximal value of the parameter of the line
  // (output) container of intersections
  // threshold value
  // source of the alternative geometry information
  // a guess for navigation
  ILVolume::Intersections local_intersects;
  intersections_r( point1              ,
                   Vector              ,
                   0.0                 ,
                   1.0                 ,
                   local_intersects    ,
                   accelCache          ,
                   threshold           ,
                   alternativeGeometry ,
                   geometryGuess       );

  //  radiation length in tick units
  const auto RadLength =
    std::accumulate
    (  local_intersects.begin()                               ,
       local_intersects.end  ()                               ,
       0.0                                                    ,
       VolumeIntersectionIntervals::AccumulateIntersections() );

  // scale
  const auto TickLength = std::sqrt( Vector.mag2() );

  return RadLength * TickLength ;
}

// ============================================================================
// The End
// ============================================================================
#endif  // __DETDESC_TRANSPORTSVC_TRASNPORTSVCDISTANCEINRADUNITS_H__
// ============================================================================
