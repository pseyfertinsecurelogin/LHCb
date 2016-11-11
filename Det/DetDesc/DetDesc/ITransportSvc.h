#ifndef DETDESC_ITRANSPORTSVC_H
#define DETDESC_ITRANSPORTSVC_H 1

// Gaudi
#include "GaudiKernel/IService.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// DetDesc
#include "DetDesc/ILVolume.h"

// Forward declarations
struct IGeometryInfo;


/** @class ITransportSvc ITransportSvc.h DetDesc/ITransportSvc.h
 *
 *  Definition of abstract interface for Transport Service
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */
struct ITransportSvc : extend_interfaces<IService>
{
  /** Declaration of the unique interface identifier
   *  ( interface id, major version, minor version)
   */
  DeclareInterfaceID( ITransportSvc , 4 , 0);

  /// Type for accelerator cache
  struct AccelCache
  {
    /// Pointer to last used geometry
    IGeometryInfo*           previousGeometry    = nullptr;
    // previous point parameters
    Gaudi::XYZPoint          prevPoint1;
    Gaudi::XYZPoint          prevPoint2;
    // "cache" parameters
    double                   previousThreshold   = -10000;
    IGeometryInfo*           previousGuess       = nullptr;
    IGeometryInfo*           previousTopGeometry = nullptr;
    ILVolume::Intersections  localIntersections;
  };

  /** Estimate the distance between 2 points in units
   *  of radiation length units
   *  @param point1 first  point
   *  @param point2 second point
   *  @param threshold threshold value
   *  @param alternativeGeometry source of alternative geometry information
   *  @param geometryGuess a guess for navigation
   */
  virtual double
  distanceInRadUnits
  ( const Gaudi::XYZPoint& point1,
    const Gaudi::XYZPoint& point2,
    double            threshold                 = 0,
    IGeometryInfo*    alternativeGeometry       = nullptr,
    IGeometryInfo*    geometryGuess             = nullptr  )  const = 0 ;

  /** Estimate the distance between 2 points in units
   *  of radiation length units
   *  Similar to distanceInRadUnits but with an additional accelerator
   *  cache for local client storage. This method, unlike distanceInRadUnits
   *  is re-entrant and thus thread safe.
   *  @param point1 first  point
   *  @param point2 second point
   *  @param threshold threshold value
   *  @param alternativeGeometry source of alternative geometry information
   *  @param geometryGuess a guess for navigation
   */
  virtual double
  distanceInRadUnits_r
  ( const Gaudi::XYZPoint& point1,
    const Gaudi::XYZPoint& point2,
    AccelCache&            accelCache,
    double            threshold                 = 0,
    IGeometryInfo*    alternativeGeometry       = nullptr,
    IGeometryInfo*    geometryGuess             = nullptr  )  const = 0 ;

  /** general method ( returns the "full history" of the volume
   *  boundary intersections
   *  with different material properties between 2 points )
   *  BEWARE : This method is not re-entrant as it internal uses
   *           data caching inside TransPortSvc. Please migrate to
   *           use intersections_r instead.
   *  @see ILVolume
   *  @see IPVolume
   *  @see ISolid
   *  @see IGeometryInfo
   *  @see Material
   *  @param point   initial point on the line
   *  @param vector  direction vector of the line
   *  @param tickMin minimal value of line paramater
   *  @param tickMax maximal value of line parameter
   *  @param intersept (output) container of intersections
   *  @param accelCache Accelerator cache
   *  @param threshold threshold value
   *  @param alternativeGeometry  source of alternative geometry information
   *  @param geometryGuess a guess for navigation
   */
  virtual unsigned long
  intersections
  ( const Gaudi::XYZPoint&   point,
    const Gaudi::XYZVector&  vector,
    const ISolid::Tick&      tickMin,
    const ISolid::Tick&      tickMax,
    ILVolume::Intersections& intersept,
    double                   threshold           = 0,
    IGeometryInfo*           alternativeGeometry = nullptr,
    IGeometryInfo*           geometryGuess       = nullptr ) const = 0;

  /** general method ( returns the "full history" of the volume
   *  boundary intersections
   *  with different material properties between 2 points )
   *  Similar to intersections but with an additional accelerator
   *  cache for local client storage. This method, unlike intersections
   *  is re-entrant and thus thread safe.
   *  @see ILVolume
   *  @see IPVolume
   *  @see ISolid
   *  @see IGeometryInfo
   *  @see Material
   *  @param point   initial point on the line
   *  @param vector  direction vector of the line
   *  @param tickMin minimal value of line paramater
   *  @param tickMax maximal value of line parameter
   *  @param intersept (output) container of intersections
   *  @param accelCache Accelerator cache
   *  @param threshold threshold value
   *  @param alternativeGeometry  source of alternative geometry information
   *  @param geometryGuess a guess for navigation
   */
  virtual unsigned long
  intersections_r
  ( const Gaudi::XYZPoint&   point,
    const Gaudi::XYZVector&  vector,
    const ISolid::Tick&      tickMin,
    const ISolid::Tick&      tickMax,
    ILVolume::Intersections& intersept,
    AccelCache&              accelCache,
    double                   threshold           = 0,
    IGeometryInfo*           alternativeGeometry = nullptr,
    IGeometryInfo*           geometryGuess       = nullptr ) const = 0;

};

// ============================================================================
// the End
// ============================================================================
#endif  //  GAUDIKERNEL_ITRANSPORTSVC_H
// ============================================================================
