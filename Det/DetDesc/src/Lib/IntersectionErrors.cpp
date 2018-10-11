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
// DetDesk
// ============================================================================
#include "DetDesc/ILVolume.h"
#include "DetDesc/IntersectionErrors.h"
#include "DetDesc/VolumeIntersectionIntervals.h"
#include "DetDesc/IGeometryErrorSvc.h"
// ============================================================================
/** @file
 *  Implementation file for class DetDesc::IntersectionErrors
 *  @date 2007-12-12
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 */
// ============================================================================
// initialize the static variableq
std::atomic<unsigned long> DetDesc::IntersectionErrors::s_code{static_cast<unsigned long>( StatusCode::SUCCESS )};
// ============================================================================
// initialize the static variable
std::atomic<DetDesc::IGeometryErrorSvc*> DetDesc::IntersectionErrors::s_service = {nullptr} ;
// ============================================================================
// initialize the static variable
std::atomic<unsigned long> DetDesc::IntersectionErrors::s_errors = {0} ;
// ============================================================================
// initialize the static variable
std::atomic<bool> DetDesc::IntersectionErrors::s_recovery = {false} ;
// ============================================================================
// set the static code
// ============================================================================
void  DetDesc::IntersectionErrors::setCode
( const StatusCode& sc     ,
  const ILVolume*   volume )
{
  s_code = sc.getCode() ;
  if ( sc.isFailure() ) {
    auto svc = s_service.load();
    if ( svc ) { svc -> setCode ( sc , volume ) ; }
    else       { ++s_errors ; } /// increment number of errors
  }
}
// ============================================================================
// get the total number of uncatched errors
// ============================================================================
unsigned long DetDesc::IntersectionErrors::errors() { return s_errors ; }
// ============================================================================
// get the static error code
// ============================================================================
StatusCode DetDesc::IntersectionErrors::code() { return StatusCode{s_code.load()}; }
// ============================================================================
// set the static pointer to Geoemtry Error Service
// ============================================================================
void DetDesc::IntersectionErrors::setService
( DetDesc::IGeometryErrorSvc* svc) { s_service = svc ; }
// ============================================================================
// get the static pointer to Geoemtry Error Service
// ============================================================================
DetDesc::IGeometryErrorSvc* DetDesc::IntersectionErrors::service()
{ return s_service ; }
// ============================================================================
// set the static flag for recovery
void DetDesc::IntersectionErrors::setRecovery ( const bool value )
{ s_recovery = value ; }
// ============================================================================
// get the static flag for recovery
// ============================================================================
bool DetDesc::IntersectionErrors::recovery() { return s_recovery ; }
// ============================================================================
// inspect the potential error in intersectionsa
// ============================================================================
void  DetDesc::IntersectionErrors::inspect
( const ILVolume*                volume ,
  const Gaudi::XYZPoint&         pnt    ,
  const Gaudi::XYZVector&        vect   ,
  const ILVolume::Intersections& cnt    )
{
  auto svc = s_service.load();
  if ( svc ) { svc -> inspect( volume , pnt , vect , cnt ) ; }
  else       { ++s_errors ; } // increment number of improcessed errors
}
// ============================================================================
/* report the recovered action in intersections
 *  @param  volume    the problematic volume
 *  @aram   material1 the affected material
 *  @aram   material2 the affected material
 *  @param  delta    the problematic delta  (non-negative!)
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-12-14
 */
// ============================================================================
void DetDesc::IntersectionErrors::recovered
( const ILVolume* volume    ,
  const Material* material1 ,
  const Material* material2 ,
  const double    delta     )
{
  auto svc = s_service.load();
  if ( svc ) { svc -> recovered ( volume , material1 , material2 , delta ) ; }
  else       { ++s_errors ; } // increment number of improcessed errors
}
// ============================================================================
/* report the skipped intersection
 *  @param  volume   the problematic volume
 *  @aram   material the affected material
 *  @param  delta    the problematic delta  (non-negative!)
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-12-14
 */
// ===========================================================================
void DetDesc::IntersectionErrors::skip
( const ILVolume* volume   ,
  const Material* material ,
  const double    delta    )
{
  auto svc = s_service.load();
  if ( svc ) { svc -> skip ( volume , material , delta ) ; }
  else       { ++s_errors ; } // increment number of improcessed errors
}
// ============================================================================
// The END
// ============================================================================
