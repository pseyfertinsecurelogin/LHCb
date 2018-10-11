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
#ifndef     _DETDESCSVC_TRANSPORTSVC_H
#define     _DETDESCSVC_TRANSPORTSVC_H
// ============================================================================
// Include Files
// ============================================================================
// STD & STL
// ============================================================================
#include <iostream>
#include <string>
#include <map>
#include <mutex>
// ============================================================================
// GaudiKErnel
// ============================================================================
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/StatEntity.h"
// ============================================================================
// DetDesc
// ============================================================================
#include "DetDesc/ITransportSvc.h"
#include "DetDesc/IGeometryErrorSvc.h"
#include "DetDesc/TransportSvcException.h"
// ============================================================================
// forward decalrations
// ============================================================================
class IDataProviderSvc;
class IMessageSvc;
struct IDetectorElement;
class ISvcLocator;
class GaudiException;
// ============================================================================
/** @class TransportSvc TransportSvc.h DetDescSvc/TransportSvc.h
 *
 *  Implementation of abstract interface ITransportSvc
 *  and abstract interface DetDesc::IGometryErrorSvc
 *
 *  @author Vanya Belyaev ibelyaev@physics.syr.edu
 */
class TransportSvc : public extends<Service,ITransportSvc,DetDesc::IGeometryErrorSvc>
{
  /// typedefs: (internal only!)
  typedef std::vector<IGeometryInfo*>     GeoContainer;
  typedef GeoContainer::reverse_iterator  rGeoIt      ;
  ///
public:
  /// constructor
  using base_class::base_class;
  ///
public:
  // ==========================================================================
  // Methods from (I)Service/IInterface
  // ==========================================================================
  ///     Initialise the service.
  StatusCode initialize    () override;
  ///     Finalise the service.
  StatusCode finalize      () override;
  // ==========================================================================
public:
  // ==========================================================================
  // Methods from ITransportSvc
  // ==========================================================================
  /// Create an instance of the accelerator cache
  ranges::v3::any createCache() const override;
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
  double distanceInRadUnits
  ( const Gaudi::XYZPoint& Point1         ,
    const Gaudi::XYZPoint& Point2         ,
    double            Threshold           ,
    IGeometryInfo*    AlternativeGeometry ,
    IGeometryInfo*    GeometryGuess       ) const override;
  // ==========================================================================
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
    ranges::v3::any&       accelCache,
    double                 threshold           = 0,
    IGeometryInfo*         alternativeGeometry = nullptr,
    IGeometryInfo*         geometryGuess       = nullptr  ) const override;
  // ==========================================================================
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
  unsigned long intersections
  ( const Gaudi::XYZPoint&   point               ,
    const Gaudi::XYZVector&  vect                ,
    const ISolid::Tick&      tickMin             ,
    const ISolid::Tick&      tickMax             ,
    ILVolume::Intersections& intersept           ,
    double                   threshold           ,
    IGeometryInfo*           alternativeGeometry ,
    IGeometryInfo*           geometryGuess       )  const override;
  // ==========================================================================
  /** general method ( returns the "full history" of the volume
   *  boundary intersections
   *  with different material properties between 2 points )
   *  Similar to intersections but with an additional accelerator
   *  cache for local client storage. This method, unlike the above
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
  unsigned long intersections_r
  ( const Gaudi::XYZPoint&   point,
    const Gaudi::XYZVector&  vector,
    const ISolid::Tick&      tickMin,
    const ISolid::Tick&      tickMax,
    ILVolume::Intersections& intersept,
    ranges::v3::any&         accelCache,
    double                   threshold           = 0,
    IGeometryInfo*           alternativeGeometry = nullptr,
    IGeometryInfo*           geometryGuess       = nullptr ) const override;
// ==========================================================================
public:
  // ==========================================================================
  // The methods from DetDesc::IGeometryErrorSvc
  // ==========================================================================
  /** set/reset the current "status" of geometry erorrs
   *  @param sc the status code
   *  @param volume the volume
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-12-14
   */
  void setCode
  ( const StatusCode& sc     ,
    const ILVolume*   volume ) override ;
  // ==========================================================================
  /** inspect the potential error in intersections
   *  @param  volume   the problematic volume
   *  @param  pnt      3D point
   *  @param  vect     3D direction vector
   *  @param  cnt  the problematic container of intersections
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-12-14
   */
  void inspect
  ( const ILVolume*                volume ,
    const Gaudi::XYZPoint&         pnt    ,
    const Gaudi::XYZVector&        vect   ,
    const ILVolume::Intersections& cnt    ) override;
  // ========================================================================
  /** report the recovered action in intersections
   *  @param  volume    the problematic volume
   *  @param  material1 the affected material
   *  @param  material2 the affected material
   *  @param  delta     the problematic delta  (non-negative!)
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-12-14
   */
  void recovered
  ( const ILVolume* volume    ,
    const Material* material1 ,
    const Material* material2 ,
    const double    delta     ) override ;
  // ========================================================================
  /** report the skipped intersection
   *  @param  volume   the problematic volume
   *  @param  material the affected material
   *  @param  delta    the problematic delta  (non-negative!)
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-12-14
   */
  void skip
  ( const ILVolume* volume   ,
    const Material* material ,
    const double    delta    ) override;
  // ========================================================================
private:
  // ==========================================================================
  // own private methods
  // ==========================================================================
  /// access to Detector Data Service
  inline IDataProviderSvc*        detSvc           () const noexcept;
  /**  source of "standard" geometry information -
   *  "top of Detector Description tree"
   */
  inline IGeometryInfo*           standardGeometry () const noexcept;
  /// assertion!
  inline void Assert
  ( bool                  assertion                        ,
    const std::string&    message                          ,
    const StatusCode&     statusCode = StatusCode::FAILURE ) const;
  ///
  inline void Assert
  ( bool                  assertion                        ,
    const std::string&    message                          ,
    const GaudiException& Exception                        ,
    const StatusCode&     statusCode = StatusCode::FAILURE ) const;
  /// get the geometry info by name
  IGeometryInfo* findGeometry( const std::string& address ) const;
  /// check for "good" geometry info
  bool           goodLocalGI ( const Gaudi::XYZPoint& point1,
                               const Gaudi::XYZPoint& point2,
                               IGeometryInfo*    gi     ) const;
  ///  find good local geometry element
  IGeometryInfo* findLocalGI ( const Gaudi::XYZPoint& point1,
                               const Gaudi::XYZPoint& point2,
                               IGeometryInfo*    gi     ,
                               IGeometryInfo*    topGi  ) const;
  // ==========================================================================
private:
  // ==========================================================================
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
  // ==========================================================================
private:
  // ==========================================================================
  /// Own private data members:
  /// names of used services:
  Gaudi::Property<std::string>     m_detDataSvc_name
  { this, "DetectorDataService",  "DetectorDataSvc" };
  ///  Detector Data Service
  SmartIF<IDataProviderSvc>        m_detDataSvc;
  /** Name (address in Transient Store) for the top element
   *  of "standard" geometry source */
  Gaudi::Property<std::string>     m_standardGeometry_address
  { this, "StandardGeometryTop", "/dd/Structure/LHCb" };
  /// Pointer to the "standard" geometry 
  IGeometryInfo *                  m_standardGeometry = nullptr;
private:
  /** Local accelerator cache. Should eventually be removed so
   *  only the re-entrant versions are available */
  mutable ranges::v3::any m_accelCache { AccelCache{} };
  //
private:
  /// the actual type of the Map
  typedef std::map<std::string, std::pair<StatEntity,StatEntity> > Map ;
  typedef std::map<std::string,unsigned long>                      Map1 ;
  Map  m_skip        ; /// the map of the skip-intervals
  Map  m_recover     ; /// the map of the recovered-intervals
  Map1 m_codes       ; /// the map of various error-codes
  /// property to allow the recovery
  Gaudi::Property<bool> m_recovery 
  { this, "Recovery", true,
      "The flag to allow the recovery of geometry errors" } ;
  /// property to allow the protocol
  Gaudi::Property<bool> m_protocol 
  { this, "Protocol", true,
      "The flag to allow protocol for the geometry problems" };
};
// ============================================================================
/// access to Detector Data  Service
inline IDataProviderSvc*    TransportSvc::detSvc            () const noexcept
{ return m_detDataSvc; }
// ============================================================================
/// access to the top of standard detector geometry information
inline IGeometryInfo*       TransportSvc::standardGeometry  () const noexcept
{ return m_standardGeometry; }
// ============================================================================
/// Assertion !!!
inline void TransportSvc::Assert
( bool                assertion  ,
  const std::string&  Message    ,
  const StatusCode&   statusCode ) const
{ if( !assertion ) { throw TransportSvcException( Message , statusCode ); } }
// ============================================================================
/// Assertion !!!
inline void TransportSvc::Assert
( bool                  assertion  ,
  const std::string&    Message    ,
  const GaudiException& Exception  ,
  const StatusCode&     statusCode ) const
{ if( !assertion )
 { throw TransportSvcException( Message , Exception , statusCode ); } }
// ============================================================================
// The END
// ============================================================================
#endif  //   DETDESCSVC__TRANSPORTSVC_H
// ============================================================================
