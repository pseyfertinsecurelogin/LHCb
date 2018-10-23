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
#ifndef DETDESC_IGEOMETRYERRORSVC_H
#define DETDESC_IGEOMETRYERRORSVC_H 1
// ============================================================================
// Include files
// ============================================================================
// Gaudi Kernel
// ============================================================================
#include "GaudiKernel/IService.h"
// ============================================================================
// DetDesc
// ============================================================================
#include "DetDesc/ITransportSvc.h"
// ============================================================================
namespace DetDesc
{
  // ==========================================================================
  /** @class IGeometryErrorSvc DetDesc/IGeometryErrorSvc.h
   *
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2007-12-14
   */
  struct IGeometryErrorSvc : virtual IService
  {
    // ========================================================================
    /** set/reset the current "status" of geometry erorrs
     *  @param sc the status code
     *  @param volume the problematic logicla volume
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-12-14
     */
    virtual void setCode
    ( const StatusCode& sc , const ILVolume*   volume ) = 0 ;
    // ========================================================================
    /** inspect the potential error in intersections
     *  @param  volume   the problematic volume
     *  @param  pnt      3D point
     *  @param  vect     3D direction vector
     *  @param  cnt  the problematic container of intersections
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-12-14
     */
    virtual void inspect
    ( const ILVolume*                volume ,
      const Gaudi::XYZPoint&         pnt    ,
      const Gaudi::XYZVector&        vect   ,
      const ILVolume::Intersections& cnt    ) = 0 ;
    // ========================================================================
    /** report the recovered action in intersections
     *  @param  volume    the problematic volume
     *  @param  material1 the affected material
     *  @param  material2 the affected material
     *  @param  delta     the problematic delta  (non-negative!)
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-12-14
     */
    virtual void recovered
    ( const ILVolume* volume    ,
      const Material* material1 ,
      const Material* material2 ,
      const double    delta     ) = 0 ;
    // ========================================================================
    /** report the skipped intersection
     *  @param  volume   the problematic volume
     *  @param  material the affected material
     *  @param  delta    the problematic delta  (non-negative!)
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-12-14
     */
    virtual void skip
    ( const ILVolume* volume   ,
      const Material* material ,
      const double    delta    ) = 0 ;
    // ========================================================================
    /// Retrieve the unique interface ID
    static const InterfaceID& interfaceID() ;
  };
  // ==========================================================================
} // end of namespace DetDesc
// ============================================================================
// The END
// ============================================================================
#endif // DETDESC_IGEOMETRYERRORSVC_H
// ============================================================================
