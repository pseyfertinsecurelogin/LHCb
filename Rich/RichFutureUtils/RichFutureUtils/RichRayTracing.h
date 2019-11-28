/*****************************************************************************\
* (c) Copyright 2000-2020 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

#pragma once

// STL
#include <array>
#include <cmath>
#include <iomanip>
#include <vector>

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
#include "RichFutureUtils/RichGeomPhoton.h"
#include "RichFutureUtils/RichMirrorFinder.h"
#include "RichFutureUtils/RichSIMDMirrorData.h"
#include "RichUtils/RichRayTracingUtils.h"
#include "RichUtils/RichSIMDRayTracing.h"
#include "RichUtils/RichSIMDTypes.h"
#include "RichUtils/RichTrackSegment.h"
#include "RichUtils/ZipRange.h"

// Kernel
#include "Kernel/RichSide.h"
#include "Kernel/RichSmartID.h"
#include "Kernel/RichTraceMode.h"

// RichDet
#include "RichDet/DeRich1.h"
#include "RichDet/DeRich2.h"
#include "RichDet/DeRichBeamPipe.h"
#include "RichDet/DeRichPDPanel.h"
#include "RichDet/DeRichSphMirror.h"

// Units
#include "GaudiKernel/SystemOfUnits.h"

// Det Desc
#include "DetDesc/ConditionKey.h"
#include "DetDesc/IConditionDerivationMgr.h"

namespace Rich::Utils {

  //-----------------------------------------------------------------------------
  /** @class RayTracing RichRayTracing.h
   *
   *  Utility to ray trace photons through the RICH system.
   *
   *  @author Chris Jones
   *  @date   2019-11-27
   */
  //-----------------------------------------------------------------------------

  class RayTracing final {

  public:
    // types

    // SIMD types
    using FP         = SIMD::DefaultScalarFP; ///< Default scalar floating point type
    using SIMDFP     = SIMD::FP<FP>;          ///< Default vector floating point type
    using SIMDVector = SIMD::Vector<FP>;      ///< Default vector Vector class
    using SIMDPoint  = SIMD::Point<FP>;       ///< Default vector Point class

    /// SIMD Result class
    using Result = RayTracingUtils::SIMDResult;

  public:
    // constructors

    // no default constructor
    RayTracing() = delete;

    /// Constructor from dependent detector elements and options
    RayTracing( const DeRich1&             rich1,                     //
                const DeRich2&             rich2,                     //
                const Utils::MirrorFinder& mirrFinder,                //
                const bool                 ignoreSecMirrs    = false, //
                const DetectorArray<bool>  treatSecMirrsFlat = {true, false} );

  public:
    // vector methods

    /// For a given detector, ray-traces a given set of directions from a given point to
    /// the photo detectors.
    Result::Vector traceToDetector( const Gaudi::XYZPoint&        startPoint,                  //
                                    SIMD::STDVector<SIMDVector>&& startDirs,                   //
                                    const LHCb::RichTrackSegment& trSeg,                       //
                                    const LHCb::RichTraceMode     mode = LHCb::RichTraceMode() //
                                    ) const;

  public:
    // scalar methods

    /// For a given detector, ray-traces a given direction from a given point to
    /// the photo detectors. Returns the result in the form of a RichGeomPhoton
    /// which contains the full ray tracing information.
    LHCb::RichTraceMode::RayTraceResult traceToDetector( const Rich::DetectorType      rich,                         //
                                                         const Gaudi::XYZPoint&        startPoint,                   //
                                                         const Gaudi::XYZVector&       startDir,                     //
                                                         Future::GeomPhoton&           photon,                       //
                                                         const LHCb::RichTrackSegment& trSeg,                        //
                                                         const LHCb::RichTraceMode     mode = LHCb::RichTraceMode(), //
                                                         const Rich::Side              forcedSide = Rich::top        //
                                                         ) const;

    /// For a given detector, raytraces a given direction from a given point to
    /// the photo detectors. Returns the result in the form of a RichGeomPhoton.
    LHCb::RichTraceMode::RayTraceResult traceToDetector( const Rich::DetectorType      rich,                         //
                                                         const Gaudi::XYZPoint&        startPoint,                   //
                                                         const Gaudi::XYZVector&       startDir,                     //
                                                         Gaudi::XYZPoint&              hitPosition,                  //
                                                         const LHCb::RichTrackSegment& trSeg,                        //
                                                         const LHCb::RichTraceMode     mode = LHCb::RichTraceMode(), //
                                                         const Rich::Side              forcedSide = Rich::top        //
                                                         ) const;

    /// For a given detector, raytraces a given direction from a given point to
    /// the photo detectors. Returns the result in the form of a RichGeomPhoton
    LHCb::RichTraceMode::RayTraceResult traceToDetector( const Rich::DetectorType  rich,                         //
                                                         const Gaudi::XYZPoint&    startPoint,                   //
                                                         const Gaudi::XYZVector&   startDir,                     //
                                                         Future::GeomPhoton&       photon,                       //
                                                         const LHCb::RichTraceMode mode = LHCb::RichTraceMode(), //
                                                         const Rich::Side          forcedSide   = Rich::top,     //
                                                         const double              photonEnergy = 0              //
                                                         ) const;

    /// For a given detector, raytraces a given direction from a given point to
    /// the photo detectors.
    LHCb::RichTraceMode::RayTraceResult traceToDetector( const Rich::DetectorType  rich,                         //
                                                         const Gaudi::XYZPoint&    startPoint,                   //
                                                         const Gaudi::XYZVector&   startDir,                     //
                                                         Gaudi::XYZPoint&          hitPosition,                  //
                                                         const LHCb::RichTraceMode mode = LHCb::RichTraceMode(), //
                                                         const Rich::Side          forcedSide   = Rich::top,     //
                                                         const double              photonEnergy = 0              //
                                                         ) const;

    /// Raytraces from a point in the detector panel back to the spherical mirror
    /// returning the mirror intersection point and the direction a track would
    /// have in order to hit that point in the detector panel.
    bool traceBackFromDetector( const Gaudi::XYZPoint&  startPoint, //
                                const Gaudi::XYZVector& startDir,   //
                                Gaudi::XYZPoint&        endPoint,   //
                                Gaudi::XYZVector&       endDir ) const;

  private:
    // methods

    /// Do the ray tracing
    LHCb::RichTraceMode::RayTraceResult _traceToDetector( const Rich::DetectorType  rich,       //
                                                          const Gaudi::XYZPoint&    startPoint, //
                                                          Gaudi::XYZPoint&          tmpPos,     //
                                                          Gaudi::XYZVector&         tmpDir,     //
                                                          Future::GeomPhoton&       photon,     //
                                                          const LHCb::RichTraceMode mode,       //
                                                          const Rich::Side          forcedSide ) const;

    /// Ray trace from given position in given direction off both mirrors
    bool reflectBothMirrors( const Rich::DetectorType  rich,      //
                             Gaudi::XYZPoint&          position,  //
                             Gaudi::XYZVector&         direction, //
                             Future::GeomPhoton&       photon,    //
                             const LHCb::RichTraceMode mode,      //
                             const Rich::Side          fSide ) const;

    /// Truncate the give value to a given d.p.
    template <typename TYPE>
    inline TYPE truncate( const TYPE x ) const noexcept {
      // constexpr int scale = 100; // 2 d.p.
      // const long long int i = std::round( x * scale );
      // const long long int i = ( x * scale );
      // return (TYPE)(i)/scale;
      // no truncation ...
      return x;
    }

    /// Access PD panel for RICH and side
    const DeRichPDPanel* pdPanel( const Rich::DetectorType rich, //
                                  const Rich::Side         side ) const noexcept {
      return m_rich[rich]->pdPanel( side );
    }

    /// Access the associated mirror finder
    const Utils::MirrorFinder& mirrorFinder() const noexcept { return *m_mirrorFinder; }

  public:
    // conditions handling

    /// Creates a condition derivation for the given key
    template <typename PARENT>
    static inline LHCb::DetDesc::IConditionDerivationMgr::DerivationId   //
    addConditionDerivation( PARENT*                     parent,          //
                            LHCb::DetDesc::ConditionKey ray_tracing_key, //
                            LHCb::DetDesc::ConditionKey mirror_finder_key = Utils::MirrorFinder::DefaultConditionKey ) {

      // First, make sure mirror finder is instanciated
      Utils::MirrorFinder::addConditionDerivation( parent, mirror_finder_key );

      // Now the ray tracing ...
      if ( parent->msgLevel( MSG::DEBUG ) ) {
        parent->debug() << "RayTracing::addConditionDerivation : Key=" << ray_tracing_key << endmsg;
      }
      using SA = std::array<std::string, 3>; // Gerhard, when you read this, can it be avoided ;) ?
      return LHCb::DetDesc::                 //
          addConditionDerivation( parent->conditionDerivationMgr(),
                                  SA{DeRichLocations::Rich1,            // input conditions locations
                                     DeRichLocations::Rich2,            //
                                     std::move( mirror_finder_key )},   //
                                  std::move( ray_tracing_key ),         // output derived condition location
                                  []( const DeRich1&             rich1, //
                                      const DeRich2&             rich2, //
                                      const Utils::MirrorFinder& mirrorFinder ) {
                                    return RayTracing{rich1, rich2, mirrorFinder};
                                  } );
    }

    /// Default conditions name
    static constexpr const char* DefaultConditionKey = "RichRayTracing-Handler";

  private:
    // data

    /// Rich1 and Rich2 pointers
    DetectorArray<const DeRich*> m_rich{{nullptr, nullptr}};

    /// Mirror finder
    const Utils::MirrorFinder* m_mirrorFinder = nullptr;

    /// Number of primary mirror rows in each RICH
    DetectorArray<int> m_sphMirrorSegRows{{0, 0}};
    /// Number of primary mirror columns in each RICH
    DetectorArray<int> m_sphMirrorSegCols{{0, 0}};
    /// Number of secondary mirror rows in each RICH
    DetectorArray<int> m_secMirrorSegRows{{0, 0}};
    /// Number of secondary mirror columns in each RICH
    DetectorArray<int> m_secMirrorSegCols{{0, 0}};

    /// Flag to to ignore secondary mirrors (useful for test beam work)
    bool m_ignoreSecMirrs = false;

    /// Flag to control if the secondary mirrors are treated as if they are completely flat
    DetectorArray<bool> m_treatSecMirrsFlat = {true, false};
  };

} // namespace Rich::Utils
