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

//-----------------------------------------------------------------------------
/** @file IRichRayTracing.h
 *
 *  Header file for tool interface : Rich::Future::IRayTracing
 *
 *  @author Antonis Papanestis
 *  @date   2003-10-28
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <vector>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// LHCbKernel
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichSide.h"
#include "Kernel/RichSmartID.h"
#include "Kernel/RichTraceMode.h"

// MathCore
#include "GaudiKernel/Plane3DTypes.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// RichUtils
#include "RichUtils/RichSIMDRayTracing.h"
#include "RichUtils/RichSIMDTypes.h"

// forward decs
namespace LHCb {
  class RichTrackSegment;
}

namespace Rich::Future {

  /// Forward declaration of photon class
  class GeomPhoton;

  //-----------------------------------------------------------------------------
  /** @class IRayTracing IRichRayTracing.h RichKernel/IRichRayTracing.h
   *
   *  A tool to trace photons (or similar) from a point all the way to
   *  the photo detectors.
   *
   *  @author Antonis Papanestis
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2003-10-28
   */
  //-----------------------------------------------------------------------------

  class IRayTracing : public virtual IAlgTool {

  public:
    /// Interface ID
    DeclareInterfaceID( IRayTracing, 1, 0 );

  public: // types
    // SIMD types
    using FP         = Rich::SIMD::DefaultScalarFP; ///< Default scalar floating point type
    using SIMDFP     = SIMD::FP<FP>;                ///< Default vector floating point type
    using SIMDVector = SIMD::Vector<FP>;            ///< Default vector Vector class
    using SIMDPoint  = SIMD::Point<FP>;             ///< Default vector Point class

    /// SIMD Result class
    using Result = Rich::RayTracingUtils::SIMDResult;

  public: // vector methods
    /** For a given detector, ray-traces a given set of directions from a given point to
     *  the photo detectors.
     *
     *  @param[in]  startPoint  The start point to use for the ray tracing
     *  @param[in]  startDirs   The directions to ray trace from the start point
     *  @param[in]  trSeg       The track segment associated to the photon direction to update
     *  @param[in]  mode        The ray tracing mode configuration
     *
     *  @return Vector of ray tracing result objects
     */
    virtual Result::Vector traceToDetector( const Gaudi::XYZPoint& startPoint, SIMD::STDVector<SIMDVector>&& startDirs,
                                            const LHCb::RichTrackSegment& trSeg,
                                            const LHCb::RichTraceMode     mode = LHCb::RichTraceMode() ) const = 0;

  public: // scalar methods
    /** For a given detector, ray-traces a given direction from a given point to
     *  the photo detectors. Returns the result in the form of a RichGeomPhoton
     *  which contains the full ray tracing information.
     *
     *  @param[in]  rich       The RICH detector
     *  @param[in]  startPoint The start point to use for the ray tracing
     *  @param[in]  startDir   The direction to ray trace from the start point
     *  @param[out] photon     The result of the raytracing, encapsulated as a RichGeomPhoton
     *  @param[in]  trSeg      The track segment associated to the photon direction to update
     *  @param[in]  mode       The ray tracing mode configuration
     *  @param[in]  forcedSide If configured to do so, the forced side to use
     *
     *  @return Status of the ray tracing
     */
    virtual LHCb::RichTraceMode::RayTraceResult traceToDetector( const Rich::DetectorType rich,
                                                                 const Gaudi::XYZPoint&   startPoint,
                                                                 const Gaudi::XYZVector& startDir, GeomPhoton& photon,
                                                                 const LHCb::RichTrackSegment& trSeg,
                                                                 const LHCb::RichTraceMode mode = LHCb::RichTraceMode(),
                                                                 const Rich::Side forcedSide    = Rich::top ) const = 0;

    /** For a given detector, raytraces a given direction from a given point to
     *  the photo detectors. Returns the result in the form of a RichGeomPhoton.
     *
     *  @param[in]  rich        The RICH detector
     *  @param[in]  startPoint  The start point to use for the ray tracing
     *  @param[in]  startDir    The direction to ray trace from the start point
     *  @param[out] hitPosition The result of the tracing, the hit point on the HPD panel
     *  @param[in]  trSeg       The track segment associated to the photon direction to update
     *  @param[in]  mode        The ray tracing mode configuration
     *  @param[in]  forcedSide  If configured to do so, the forced side to use
     *
     *  @return Status of the ray tracing
     */
    virtual LHCb::RichTraceMode::RayTraceResult
    traceToDetector( const Rich::DetectorType rich, const Gaudi::XYZPoint& startPoint, const Gaudi::XYZVector& startDir,
                     Gaudi::XYZPoint& hitPosition, const LHCb::RichTrackSegment& trSeg,
                     const LHCb::RichTraceMode mode       = LHCb::RichTraceMode(),
                     const Rich::Side          forcedSide = Rich::top ) const = 0;

    /** For a given detector, ray-traces a given direction from a given point to
     *  the photo detectors. Returns the result in the form of a RichGeomPhoton
     *  which contains the full ray tracing information.
     *
     *  @param[in]  rich       The RICH detector
     *  @param[in]  startPoint The start point to use for the ray tracing
     *  @param[in]  startDir   The direction to ray trace from the start point
     *  @param[out] photon     The result of the raytracing, encapsulated as a RichGeomPhoton
     *  @param[in]  mode       The ray tracing mode configuration
     *  @param[in]  forcedSide If configured to do so, the forced side to use
     *  @param[in]  photonEnergy If needed (for instance for refraction corrections) the photon
     *                           energy to assume.
     *
     *  @return Status of the ray tracing
     */
    virtual LHCb::RichTraceMode::RayTraceResult
    traceToDetector( const Rich::DetectorType rich, const Gaudi::XYZPoint& startPoint, const Gaudi::XYZVector& startDir,
                     GeomPhoton& photon, const LHCb::RichTraceMode mode = LHCb::RichTraceMode(),
                     const Rich::Side forcedSide = Rich::top, const double photonEnergy = 0 ) const = 0;

    /** For a given detector, raytraces a given direction from a given point to
     *  the photo detectors. Returns the result in the form of a RichGeomPhoton.
     *
     *  @param[in]  rich        The RICH detector
     *  @param[in]  startPoint  The start point to use for the ray tracing
     *  @param[in]  startDir    The direction to ray trace from the start point
     *  @param[out] hitPosition The result of the tracing, the hit point on the HPD panel
     *  @param[in]  mode        The ray tracing mode configuration
     *  @param[in]  forcedSide  If configured to do so, the forced side to use
     *  @param[in]  photonEnergy If needed (for instance for refraction corrections) the photon
     *                           energy to assume.
     *
     *  @return Status of the ray tracing
     */
    virtual LHCb::RichTraceMode::RayTraceResult
    traceToDetector( const Rich::DetectorType rich, const Gaudi::XYZPoint& startPoint, const Gaudi::XYZVector& startDir,
                     Gaudi::XYZPoint& hitPosition, const LHCb::RichTraceMode mode = LHCb::RichTraceMode(),
                     const Rich::Side forcedSide = Rich::top, const double photonEnergy = 0 ) const = 0;

    /** Raytraces from a point in the detector panel back to the spherical mirror
     *  returning the mirror intersection point and the direction a track would have
     *  in order to hit that point in the detector panel.
     *
     *  @param[in]  startPoint The start point to use for the ray tracing
     *  @param[in]  startDir   The direction to ray trace from the start point
     *  @param[out] endPoint   The required mirror intersection point
     *  @param[out] endDir     The required track direction
     *
     *  @return Boolean indicating the status of the ray tracing
     *  @retval true  Ray tracing was successful
     *  @retval false Ray tracing was unsuccessful
     */
    virtual bool traceBackFromDetector( const Gaudi::XYZPoint& startPoint, const Gaudi::XYZVector& startDir,
                                        Gaudi::XYZPoint& endPoint, Gaudi::XYZVector& endDir ) const = 0;
  };

} // namespace Rich::Future
