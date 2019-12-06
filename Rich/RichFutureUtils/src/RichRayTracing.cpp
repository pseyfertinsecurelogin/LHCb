
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

#include "RichFutureUtils/RichRayTracing.h"

using namespace Rich::Utils;

namespace {
  /// A z point that separates Rich1 from Rich2 (anything between 3000-9000mm)
  static constexpr double s_RichDetSeparationPointZ = 8000.0;
} // namespace

// constructor from dependent detector elements
RayTracing::RayTracing( const DeRich1&             rich1,          //
                        const DeRich2&             rich2,          //
                        const Utils::MirrorFinder& mirrFinder,     //
                        const bool                 ignoreSecMirrs, //
                        const DetectorArray<bool>  treatSecMirrsFlat )
    : m_mirrorFinder( &mirrFinder )      //
    , m_ignoreSecMirrs( ignoreSecMirrs ) //
    , m_treatSecMirrsFlat( treatSecMirrsFlat ) {

  // save RICH detector elements
  m_rich[Rich::Rich1] = &rich1;
  m_rich[Rich::Rich2] = &rich2;

  // Rich1 mirrors
  if ( m_rich[Rich::Rich1]->exists( "SphMirrorSegRows" ) ) {
    m_sphMirrorSegRows[Rich::Rich1] = m_rich[Rich::Rich1]->param<int>( "SphMirrorSegRows" );
    m_sphMirrorSegCols[Rich::Rich1] = m_rich[Rich::Rich1]->param<int>( "SphMirrorSegColumns" );
  } else {
    throw Rich::Exception( "No primary mirrors for RICH1 found !" );
  }
  if ( m_rich[Rich::Rich1]->exists( "SecMirrorSegRows" ) ) {
    m_secMirrorSegRows[Rich::Rich1] = m_rich[Rich::Rich1]->param<int>( "SecMirrorSegRows" );
    m_secMirrorSegCols[Rich::Rich1] = m_rich[Rich::Rich1]->param<int>( "SecMirrorSegColumns" );
  } else {
    throw Rich::Exception( "No secondary mirrors for RICH1 found !" );
  }

  // Rich2 mirrors
  if ( m_rich[Rich::Rich2]->exists( "SphMirrorSegRows" ) ) {
    m_sphMirrorSegRows[Rich::Rich2] = m_rich[Rich::Rich2]->param<int>( "SphMirrorSegRows" );
    m_sphMirrorSegCols[Rich::Rich2] = m_rich[Rich::Rich2]->param<int>( "SphMirrorSegColumns" );
  } else {
    throw Rich::Exception( "No primary mirrors for RICH2 found !" );
  }
  if ( m_rich[Rich::Rich2]->exists( "SecMirrorSegRows" ) ) {
    m_secMirrorSegRows[Rich::Rich2] = m_rich[Rich::Rich2]->param<int>( "SecMirrorSegRows" );
    m_secMirrorSegCols[Rich::Rich2] = m_rich[Rich::Rich2]->param<int>( "SecMirrorSegColumns" );
  } else {
    throw Rich::Exception( "No secondary mirrors for RICH2 found !" );
  }
}

// For a given detector, ray-traces a given set of directions from a given point to
// the photo detectors.
RayTracing::Result::Vector RayTracing::traceToDetector( const Gaudi::XYZPoint&        startPoint, //
                                                        SIMD::STDVector<SIMDVector>&& startDirs,  //
                                                        const LHCb::RichTrackSegment& trSeg,      //
                                                        const LHCb::RichTraceMode     mode        //
                                                        ) const {

  // types
  using FP     = SIMD::DefaultScalarFP;
  using SIMDFP = SIMD::FP<FP>;
  using Point  = SIMD::Point<FP>;

  // Ray tracing utils
  using namespace Rich::RayTracingUtils;
  // SIMD utils
  using namespace LHCb::SIMD;

  // which rich
  const auto rich = trSeg.rich();

  // Start detector side
  const auto tkside = m_rich[rich]->side( startPoint );

  // cache the PD type. Note we are assuming its always the same
  // const auto pdType = pdPanel(rich,tkside)->pdType();

  // Number of Vector objects.
  const auto NVC = startDirs.size();

  // cache the SIMD start point
  const Point simdStartPoint( startPoint );

  // Starting CoC value
  const Point startCoC( m_rich[rich]->nominalCentreOfCurvature( tkside ) );

  // Starting RoC value
  const SIMDFP startRoC( (FP)m_rich[rich]->sphMirrorRadius() );

  // The results vector to return
  Result::Vector results( NVC );

  // Form the SIMD result mask to test
  const SIMDResult::Results testMask( (int)mode.detPlaneBound() );

  // Mirror data caches
  SIMD::MirrorData sphMirData, secMirData;

  // Intersect with the spherical mirrors to find the reflection points and use
  // these to find the mirror segments to use for the primary mirrors
  for ( auto&& data : Ranges::Zip( startDirs, results ) ) {
    // start direction
    auto& dir = std::get<0>( data );

    // the result to fill
    auto& result = std::get<1>( data );

    // Start Point
    auto point = simdStartPoint;

    // selection mask
    auto& mask = result.valid;

    // mirrors pointer arrays
    auto& sphMirP = result.primaryMirror;
    auto& secMirP = result.secondaryMirror;

    // PD smart IDs
    auto& smartID = result.smartID;

    // Detection point
    auto& hitPosition = result.detectionPoint;

    // PD pointers
    auto& pds = result.photonDetector;

    // detector sides
    SIMD::Sides sides( (int)tkside );

    // interset with nominal primary
    Point nomSphInter;
    mask = intersectSpherical( point, dir, startCoC, startRoC, nomSphInter );

    // Check the detector side for the intersection point sides
    const auto new_sides = m_rich[rich]->side( nomSphInter );
    // changed ?
    const auto side_change = any_of( new_sides != sides );
    if ( UNLIKELY( side_change ) ) {
      // update sides
      sides = new_sides;
      // New CoCs
      const auto CoC = m_rich[rich]->nominalCentreOfCurvature( sides );
      // rerun the intersection
      mask &= intersectSpherical( point, dir, CoC, startRoC, nomSphInter );
    }

    // Find the primary mirror segment for this point
    sphMirP = mirrorFinder().findSphMirror( rich, sides, nomSphInter );
    // Update mirror cache
    sphMirData.update( sphMirP );

    // perform the final reflection on the primaries
    mask &= reflectSpherical( point, dir, sphMirData.getCoCs(), sphMirData.getRoCs() );
    // Spherical mirror reflection point
    Point sphMir = point;

    // move on to the secondary mirrors

    // nominal intersection point
    Point planeInt;
    if ( UNLIKELY( side_change ) ) {
      // slow method to construct a plane for the mixture of sides
      const auto nomPlane = m_rich[rich]->nominalPlane( sides );
      intersectPlane( point, dir, nomPlane, planeInt );
    } else {
      // shortcut using faster single side plane
      intersectPlane( point, dir, m_rich[rich]->nominalPlaneSIMD( tkside ), planeInt );
    }

    // find secondary mirror segments
    secMirP = mirrorFinder().findSecMirror( rich, sides, planeInt );
    // update mirror cache
    secMirData.update( secMirP );

    // Treat secondaries as flat or not ?
    if ( m_treatSecMirrsFlat[rich] ) {
      // reflect off the planes
      reflectPlane( point, dir, secMirData.getNormalPlane() );
    } else {
      // reflect off the spherical mirrors
      mask &= reflectSpherical( point, dir, secMirData.getCoCs(), secMirData.getRoCs() );
    }

    // Intersect with the PD plane

    // Update sides for the end point
    sides = m_rich[rich]->side( point );

    // do ray tracing to PD panels
    result.result = m_rich[rich]->rayTrace( sides, point, dir, hitPosition, smartID, pds, mode );
    mask &= LHCb::SIMD::simd_cast<SIMDFP::mask_type>( result.result >= testMask );

    // test for beam pipe intersections ?
    if ( UNLIKELY( mode.beamPipeIntersects() && any_of( mask ) ) ) {
      const auto beam_mask = m_rich[rich]->beampipe()->testForIntersection( simdStartPoint, sphMir, mask );
      if ( any_of( beam_mask ) ) {
        // set flag to outside panel...
        // CRJ : Do we need a special result flag ?
        result.result( LHCb::SIMD::simd_cast<Result::Results::mask_type>( beam_mask ) ) =
            Result::Results( (int)LHCb::RichTraceMode::RayTraceResult::OutsidePDPanel );
      }
    }

    // if ( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
    // {
    //   for ( std::size_t i = 0; i < SIMDFP::Size; ++i )
    //   {
    //     using GP = Gaudi::XYZPoint;
    //     using GV = Gaudi::XYZVector;
    //     const auto & p = result.detectionPoint;
    //     verbose() << std::setprecision(3)
    //               << "Hit Point " << GP(p.x()[i],p.y()[i],p.z()[i])
    //               << " " << LHCb::RichTraceMode::RayTraceResult((int)result.result[i])
    //               << endmsg;
    //   }
    // }

  } // loop over photon data

  return results;
}

//=============================================================================
// reflect the trajectory on the mirror, and determine the position where
// it hits the detector plane,
// take into account the geometrical boundaries of mirrors and detector
//=============================================================================
LHCb::RichTraceMode::RayTraceResult RayTracing::traceToDetector( const Rich::DetectorType  rich,        //
                                                                 const Gaudi::XYZPoint&    startPoint,  //
                                                                 const Gaudi::XYZVector&   startDir,    //
                                                                 Gaudi::XYZPoint&          hitPosition, //
                                                                 const LHCb::RichTraceMode mode,        //
                                                                 const Rich::Side          forcedSide,  //
                                                                 const double              photonEnergy //
                                                                 ) const {
  // need to think if this can be done without creating a temp RichGeomPhoton ?
  Future::GeomPhoton photon;
  const auto         sc = traceToDetector( rich, startPoint, startDir, photon, mode, forcedSide, photonEnergy );
  hitPosition           = photon.detectionPoint();
  return sc;
}

//=============================================================================
// reflect the trajectory on the mirror, and determine the position where
// it hits the detector plane,
// take into account the geometrical boundaries of mirrors and detector
//=============================================================================
LHCb::RichTraceMode::RayTraceResult RayTracing::traceToDetector( const Rich::DetectorType      rich,        //
                                                                 const Gaudi::XYZPoint&        startPoint,  //
                                                                 const Gaudi::XYZVector&       startDir,    //
                                                                 Gaudi::XYZPoint&              hitPosition, //
                                                                 const LHCb::RichTrackSegment& trSeg,       //
                                                                 const LHCb::RichTraceMode     mode,        //
                                                                 const Rich::Side              forcedSide   //
                                                                 ) const {
  // need to think if this can be done without creating a temp GeomPhoton ?
  Future::GeomPhoton photon;
  const auto         sc = traceToDetector( rich, startPoint, startDir, photon, trSeg, mode, forcedSide );
  hitPosition           = photon.detectionPoint();
  return sc;
}

//=============================================================================
// reflect the trajectory on the mirror, and determine the position where
// it hits the detector plane,
// take into account the geometrical boundaries of mirrors and detector
//=============================================================================
LHCb::RichTraceMode::RayTraceResult RayTracing::traceToDetector( const Rich::DetectorType  rich,       //
                                                                 const Gaudi::XYZPoint&    startPoint, //
                                                                 const Gaudi::XYZVector&   startDir,   //
                                                                 Future::GeomPhoton&       photon,     //
                                                                 const LHCb::RichTraceMode mode,       //
                                                                 const Rich::Side          forcedSide, //
                                                                 const double /* photonEnergy */       //
                                                                 ) const {

  // temporary working objects
  Gaudi::XYZPoint  tmpPos( startPoint );
  Gaudi::XYZVector tmpDir( startDir );

  // Correct start point/direction for aerogel refraction, if appropriate
  if ( UNLIKELY( mode.aeroRefraction() && Rich::Rich1 == rich ) ) {
    Exception( "Aerogel processing is not supported" );
  } // Do the ray tracing
  return _traceToDetector( rich, startPoint, tmpPos, tmpDir, photon, mode, forcedSide );
}

//=============================================================================
// reflect the trajectory on the mirror, and determine the position where
// it hits the detector plane,
// take into account the geometrical boundaries of mirrors and detector
//=============================================================================
LHCb::RichTraceMode::RayTraceResult RayTracing::traceToDetector( const Rich::DetectorType      rich,       //
                                                                 const Gaudi::XYZPoint&        startPoint, //
                                                                 const Gaudi::XYZVector&       startDir,   //
                                                                 Future::GeomPhoton&           photon,     //
                                                                 const LHCb::RichTrackSegment& trSeg,      //
                                                                 const LHCb::RichTraceMode     mode,       //
                                                                 const Rich::Side              forcedSide  //
                                                                 ) const {

  // temporary working objects
  Gaudi::XYZPoint  tmpPos( startPoint );
  Gaudi::XYZVector tmpDir( startDir );

  // Correct start point/direction for aerogel refraction, if appropriate
  if ( UNLIKELY( mode.aeroRefraction() && rich == Rich::Rich1 && trSeg.radiator() == Rich::Aerogel ) ) {
    Exception( "Aerogel processing is not supported" );
  } // Do the ray tracing
  return _traceToDetector( rich, startPoint, tmpPos, tmpDir, photon, mode, forcedSide );
}

//=============================================================================
// Does the actual ray tracing
//=============================================================================
LHCb::RichTraceMode::RayTraceResult RayTracing::_traceToDetector( const Rich::DetectorType  rich,       //
                                                                  const Gaudi::XYZPoint&    startPoint, //
                                                                  Gaudi::XYZPoint&          tmpPos,     //
                                                                  Gaudi::XYZVector&         tmpDir,     //
                                                                  Future::GeomPhoton&       photon,     //
                                                                  const LHCb::RichTraceMode mode,       //
                                                                  const Rich::Side          forcedSide  //
                                                                  ) const {

  // default result is failure
  LHCb::RichTraceMode::RayTraceResult result = LHCb::RichTraceMode::RayTraceResult::RayTraceFailed;

  //_ri_verbo << "Ray Tracing : " << rich << " Ptn=" << startPoint
  //          << " Dir=" << tmpDir << endmsg;

  // first, try and reflect of both mirrors
  const auto sc = reflectBothMirrors( rich, tmpPos, tmpDir, photon, mode, forcedSide );

  //_ri_verbo << "  -> After reflectBothMirrors OK=" << sc << " : Ptn=" << tmpPos
  //          << " Dir=" << tmpDir << endmsg;

  if ( sc ) {

    // for hit point use photon data directly
    auto& hitPosition = photon.detectionPoint();

    // the detector side
    const auto side = m_rich[rich]->side( tmpPos );

    // smart ID for RICH and panel (to be filled further when possible in following methods)
    LHCb::RichSmartID smartID( rich, side, pdPanel( rich, side )->pdType() );

    // pointer to the deRichPD object
    const DeRichPD* dePD{nullptr};

    // do ray tracing, depending on mode
    result = m_rich[rich]->rayTrace( side, tmpPos, tmpDir, hitPosition, smartID, dePD, mode );

    // Set remaining GeomPhoton data
    photon.setSmartID( smartID );
    photon.setEmissionPoint( startPoint );
    photon.setPhotonDetector( dePD );

    // test for beam pipe intersections ?
    if ( mode.traceWasOK( result ) && mode.beamPipeIntersects() ) {
      // test for intersections between emission point and spherical reflection point
      if ( m_rich[rich]->beampipe()->testForIntersection( startPoint, photon.sphMirReflectionPoint() ) ) {
        result = LHCb::RichTraceMode::RayTraceResult::OutsidePDPanel; // CRJ : Do we need a special
                                                                      // result flag ?
      }
      // Probably not needed to check for other intersections ?
    }

  } // mirrors reflection OK

  // return the result
  return result;
}

//=========================================================================
// Reflect a photon on both mirrors and return the position and direction
// on the secondary mirror.
//=========================================================================
bool RayTracing::reflectBothMirrors( const Rich::DetectorType  rich,      //
                                     Gaudi::XYZPoint&          position,  //
                                     Gaudi::XYZVector&         direction, //
                                     Future::GeomPhoton&       photon,    //
                                     const LHCb::RichTraceMode mode,      //
                                     const Rich::Side          forcedSide ) const {

  using namespace Rich::RayTracingUtils;

  Gaudi::XYZPoint  tmpPos( position );
  Gaudi::XYZVector tmpDir( direction );

  // which side are we on ?
  auto side = ( mode.forcedSide() ? forcedSide : m_rich[rich]->side( tmpPos ) );

  // Spherical mirror reflection with nominal parameters
  if ( !reflectSpherical( tmpPos, tmpDir,                                 //
                          m_rich[rich]->nominalCentreOfCurvature( side ), //
                          m_rich[rich]->sphMirrorRadius() ) ) {
    return false;
  } // if not forced, check if still same side, if not change sides
  if ( !mode.forcedSide() ) {
    const auto tmpSide = m_rich[rich]->side( tmpPos );
    if ( side != tmpSide ) {
      side   = tmpSide;
      tmpPos = position;
      tmpDir = direction;
      if ( !reflectSpherical( tmpPos, tmpDir, m_rich[rich]->nominalCentreOfCurvature( side ),
                              m_rich[rich]->sphMirrorRadius() ) ) {
        return false;
      }
    }
  }

  // find segment
  const auto* sphSegment = mirrorFinder().findSphMirror( rich, side, tmpPos );

  // depending on the tracing flag
  if ( mode.mirrorSegBoundary() ) {
    // if reflection from a mirror segment is required
    if ( !sphSegment->intersects( position, direction ) ) { return false; }
  } else if ( mode.outMirrorBoundary() ) {
    // check the outside boundaries of the (whole) mirror
    if ( !sphSegment->intersects( position, direction ) ) {
      const auto  pos       = m_rich[rich]->sphMirrorSegPos( sphSegment->mirrorNumber() );
      const auto& mirCentre = sphSegment->mirrorCentre();
      // check for intersection failure
      if ( ( pos.row() == 0 && tmpPos.y() < mirCentre.y() ) ||
           ( pos.row() == m_sphMirrorSegRows[rich] - 1 && tmpPos.y() > mirCentre.y() ) ||
           ( pos.column() == 0 && tmpPos.x() < mirCentre.x() ) ||
           ( pos.column() == m_sphMirrorSegCols[rich] - 1 && tmpPos.x() > mirCentre.x() ) ) {
        return false;
      }
    }
  }

  // reset position, direction before trying again
  tmpPos = position;
  tmpDir = direction;

  // Spherical mirror reflection with exact parameters
  if ( !reflectSpherical( tmpPos, tmpDir, sphSegment->centreOfCurvature(), sphSegment->radius() ) ) {
    return false;
  } // set primary mirror data photon.setSphMirReflectionPoint( tmpPos );
  photon.setPrimaryMirror( sphSegment );

  // Are we ignoring the secondary mirrors ?
  if ( !m_ignoreSecMirrs ) {

    Gaudi::XYZPoint planeInt;
    // sec mirror reflection with nominal parameters
    intersectPlane( tmpPos, tmpDir, m_rich[rich]->nominalPlane( side ), planeInt );
    // find secondary mirror segment
    const auto* secSegment = mirrorFinder().findSecMirror( rich, side, planeInt );

    // depending on the tracing flag:
    if ( mode.mirrorSegBoundary() ) {
      // if reflection from a mirror segment is required
      if ( !secSegment->intersects( tmpPos, tmpDir ) ) { return false; }
    } else if ( mode.outMirrorBoundary() ) {
      // check the outside boundaries of the (whole) mirror
      if ( !secSegment->intersects( tmpPos, tmpDir ) ) {
        const auto  pos       = m_rich[rich]->secMirrorSegPos( secSegment->mirrorNumber() );
        const auto& mirCentre = secSegment->mirrorCentre();
        if ( ( pos.row() == 0 && planeInt.y() < mirCentre.y() ) ||
             ( pos.row() == m_secMirrorSegRows[rich] - 1 && planeInt.y() > mirCentre.y() ) ||
             ( pos.column() == 0 && planeInt.x() < mirCentre.x() ) ||
             ( pos.column() == m_secMirrorSegCols[rich] - 1 && planeInt.x() > mirCentre.x() ) ) {
          return false;
        }
      }
    }

    // Secondary mirror reflection with actual parameters
    if ( m_treatSecMirrsFlat[rich] ) {
      reflectPlane( tmpPos, tmpDir, secSegment->centreNormalPlane() );
    } else {
      const auto sc = reflectSpherical( tmpPos, tmpDir, secSegment->centreOfCurvature(), secSegment->radius() );
      if ( UNLIKELY( !sc ) ) { return false; }
    }

    // set secondary ("flat") mirror data
    photon.setFlatMirReflectionPoint( tmpPos );
    photon.setSecondaryMirror( secSegment );

  } // ignore secondary mirrors

  // Set final direction and position data
  position  = tmpPos;
  direction = tmpDir;

  return true;
}

//==========================================================================
// Raytraces from a point in the detector panel back to the spherical mirror
// returning the mirror intersection point and the direction a track would
// have in order to hit that point in the detector panel.
//==========================================================================
bool RayTracing::traceBackFromDetector( const Gaudi::XYZPoint&  startPoint, //
                                        const Gaudi::XYZVector& startDir,   //
                                        Gaudi::XYZPoint&        endPoint,   //
                                        Gaudi::XYZVector&       endDir ) const {

  using namespace Rich::RayTracingUtils;

  Gaudi::XYZPoint  tmpStartPoint( startPoint );
  Gaudi::XYZVector tmpStartDir( startDir );

  // which RICH ?
  const auto rich = ( startPoint.z() / Gaudi::Units::mm < s_RichDetSeparationPointZ ? Rich::Rich1 : Rich::Rich2 );
  // which side ?
  const Rich::Side side = m_rich[rich]->side( startPoint );

  // are we using the secondary mirrors ?
  if ( !m_ignoreSecMirrs ) {

    Gaudi::XYZPoint planeInt;
    // sec mirror reflection with nominal parameters
    intersectPlane( tmpStartPoint, tmpStartDir, m_rich[rich]->nominalPlane( side ), planeInt );
    // find secondary mirror segment
    const auto* secSegment = mirrorFinder().findSecMirror( rich, side, planeInt );

    // secondary mirror reflection
    if ( m_treatSecMirrsFlat[rich] ) {
      reflectPlane( tmpStartPoint, tmpStartDir, secSegment->centreNormalPlane() );
    } else {
      const auto sc =
          reflectSpherical( tmpStartPoint, tmpStartDir, secSegment->centreOfCurvature(), secSegment->radius() );
      if ( UNLIKELY( !sc ) ) { return false; }
    }
  }

  // save points after first mirror reflection
  Gaudi::XYZPoint  storePoint( tmpStartPoint );
  Gaudi::XYZVector storeDir( tmpStartDir );

  // Primary mirror reflection with nominal parameters
  if ( !reflectSpherical( tmpStartPoint, tmpStartDir, m_rich[rich]->nominalCentreOfCurvature( side ),
                          m_rich[rich]->sphMirrorRadius() ) ) {
    return false;
  } // find primary mirror segment
  const auto* sphSegment = mirrorFinder().findSphMirror( rich, side, tmpStartPoint );

  // Primary mirror reflection with exact parameters
  if ( !reflectSpherical( storePoint, storeDir, sphSegment->centreOfCurvature(), sphSegment->radius() ) ) {
    return false;
  }
  endPoint = storePoint;
  endDir   = storeDir;

  return true;
}

//=========================================================================
