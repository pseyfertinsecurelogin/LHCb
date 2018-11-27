/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef DETDESC_INTERSECTIONERRORS_H
#define DETDESC_INTERSECTIONERRORS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <map>
#include <atomic>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/StatusCode.h"
// ============================================================================
// DetDesc
// ============================================================================
#include "DetDesc/ILVolume.h"
// ============================================================================
namespace DetDesc
{
  // forward declarations
  struct IGeometryErrorSvc ;
  // ==========================================================================
  /** @class IntersectionErrors DetDesc/IntersectionErrors.h
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2007-12-12
   */
  class IntersectionErrors final
  {
  public:
    /// get the total number of uncatched errors
    static unsigned long errors () ;
  public:
    /// set the static error code
    static void              setCode
    ( const StatusCode& sc , const ILVolume*   volume ) ;
    static inline void setCode( unsigned long sc, const ILVolume* volume ) { setCode( StatusCode{sc}, volume ); }
    /// get the static error code
    // [[deprecated("can not be guaranteed to give the expected results in a multithreaded environment")]]
    static StatusCode code    ()  ;
  public:
    /// set the static pointer to Geoemtry Error Service
    static void setService ( DetDesc::IGeometryErrorSvc* svc) ;
    /// get the static pointer to Geoemtry Error Service
    static DetDesc::IGeometryErrorSvc* service() ;
  public:
    /// set the static flag for recovery
    static void setRecovery ( const bool value ) ;
    /// get the static flag for recovery
    // [[deprecated("can not be guaranteed to give the expected results in a multithreaded environment")]]
    static bool    recovery () ;
  public:
    // ========================================================================
    /** report the recovered action in intersections
     *  @param  volume    the problematic volume
     *  @param  material1 the affected material
     *  @param  material2 the affected material
     *  @param  delta    the problematic delta  (non-negative!)
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-12-14
     */
    static void recovered
    ( const ILVolume* volume    ,
      const Material* material1 ,
      const Material* material2 ,
      const double    delta     ) ;
    // ========================================================================
    /** report the skipped intersection
     *  @param  volume   the problematic volume
     *  @param  material the affected material
     *  @param  delta    the problematic delta  (non-negative!)
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-12-14
     */
    static void skip
    ( const ILVolume* volume   ,
      const Material* material ,
      const double    delta    ) ;
    // ========================================================================
  public:
    /// inspect the potential error in intersections
    static void  inspect
    ( const ILVolume*                volume ,
      const Gaudi::XYZPoint&         pnt    ,
      const Gaudi::XYZVector&        vect   ,
      const ILVolume::Intersections& cnt    ) ;
  private:
    // static code
    static std::atomic<unsigned long> s_code     ; /// the static error code
    // static pointer to IGeometryErrorSvc
    static std::atomic<DetDesc::IGeometryErrorSvc*> s_service  ; /// pointer to IGeometryErrorSvc
    // static counter of unreported errors
    static std::atomic<unsigned long>  s_errors   ;
    // static flag to perform the error ecovery
    static std::atomic<bool>           s_recovery ;
  } ;
} // end of namespace DetDesc
// ============================================================================
// The END
// ============================================================================
#endif // DETDESC_INTERSECTIONERRORS_H
// ============================================================================
