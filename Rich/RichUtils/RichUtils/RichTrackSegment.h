
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
/** @file RichTrackSegment.h
 *
 *  Header file for tool interface : LHCb::RichTrackSegment
 *
 *  @author Antonis Papanestis   Antonis.Papanestis@cern.ch
 *  @author Chris Jones          Christopher.Rob.Jones@cern.ch
 *  @date   2002-05-31
 */
//-----------------------------------------------------------------------------

#pragma once

// std include
#include <array>
#include <cmath>
#include <iostream>
#include <memory>
#include <type_traits>

// local
#include "RichUtils/RichException.h"
#include "RichUtils/RichSIMDTypes.h"

// LHCb Maths
#include "LHCbMath/FastMaths.h"

// LHCbKernel
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichRadIntersection.h"
#include "Kernel/RichRadiatorType.h"

// geometry
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Transform3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// VDT
#include "vdt/sqrt.h"

// Vc
#include <Vc/common/alignedbase.h>

// General LHCb namespace
namespace LHCb {

  //-----------------------------------------------------------------------------
  /** @class RichTrackSegment RichUtils/RichTrackSegment.h
   *
   *  RichTrackSegment represents the trajectory of a Track through a radiator volume.
   *
   *  Implements the functionaility needed by the RICH reconstruction to interact with
   *  this trajectory in order to calculate the Cherenkov angles etc.
   *
   *  @author Antonis Papanestis   Antonis.Papanestis@cern.ch
   *  @author Chris Jones          Christopher.Rob.Jones@cern.ch
   *  @date   2002-05-31
   *
   *  @todo Check if normalisation of axis vectors is really needed
   */
  //-----------------------------------------------------------------------------

  class RichTrackSegment final : public Vc::AlignedBase<Vc::VectorAlignment> {

  public:
    /// Vector of track segments
    using Vector = Rich::SIMD::STDVector<RichTrackSegment>;

    /// Scalar float type
    using FP = Rich::SIMD::DefaultScalarFP;

    /// SIMD Float Type
    using SIMDFP = Rich::SIMD::FP<FP>;

    /// SIMD Point Type
    using SIMDPoint = Rich::SIMD::Point<FP>;

    /// SIMD Vector Type
    using SIMDVector = Rich::SIMD::Vector<FP>;

  private:
    /** Add two points together
     *  @param p1 The first point
     *  @param p2 The second point
     *  @return The point represented by "p1+p2"
     */
    template <typename POINT>
    inline POINT add_points( const POINT& p1, const POINT& p2 ) const noexcept {
      return {p1.x() + p2.x(), p1.y() + p2.y(), p1.z() + p2.z()};
    }

    /** Returns the average photon energy for the given RICH radiator
     *  @param[in] rad The radiator type
     *  @return The average photon energy */
    inline float avPhotEn( const Rich::RadiatorType rad ) const {
      return ( Rich::Rich1Gas == rad ? 4.25f :     // C4F10
                   Rich::Rich2Gas == rad ? 4.40f : // CF4
                       3.00f );                    // Aerogel
    }

  public: // helper classes
    //-----------------------------------------------------------------------------
    /** @class StateErrors RichTrackSegment.h RichUtils/RichTrackSegment.h
     *
     *  Helper class for RichTrackSegment to describe the segment errors.
     *
     *  @author Chris Jones          Christopher.Rob.Jones@cern.ch
     *  @date   2004-05-31
     */
    //-----------------------------------------------------------------------------
    class StateErrors final {

    public:
      /// Default Constructor
      StateErrors() {}

    public:
      /// Constructor with explicit float values
      StateErrors( const float errX2,  ///< error on x squared
                   const float errY2,  ///< error on y squared
                   const float errTX2, ///< error on x slope squared
                   const float errTY2, ///< error on y slope squared
                   const float errP2   ///< error on momentum squared
                   )
          : m_errX2( fabsf( errX2 ) )
          , m_errY2( fabsf( errY2 ) )
          , m_errTX2( fabsf( errTX2 ) )
          , m_errTY2( fabsf( errTY2 ) )
          , m_errP2( fabsf( errP2 ) ) {}

      /// Constructor with explicit double values
      StateErrors( const double errX2,  ///< error on x squared
                   const double errY2,  ///< error on y squared
                   const double errTX2, ///< error on x slope squared
                   const double errTY2, ///< error on y slope squared
                   const double errP2   ///< error on momentum squared
                   )
          : StateErrors( (float)errX2, (float)errY2, (float)errTX2, (float)errTY2, (float)errP2 ) {}

      /// Constructor from a State object
      template <typename STATE>
      StateErrors( const STATE& state )
          : StateErrors( (float)state.errX2(), (float)state.errY2(), (float)state.errTx2(), (float)state.errTy2(),
                         (float)state.errP2() ) {}

    public:
      inline float errX2() const noexcept { return m_errX2; }   ///< Access the x error squared
      inline float errY2() const noexcept { return m_errY2; }   ///< Access the y error squared
      inline float errTX2() const noexcept { return m_errTX2; } ///< Access the tx error squared
      inline float errTY2() const noexcept { return m_errTY2; } ///< Access the ty error squared
      inline float errP2() const noexcept { return m_errP2; }   ///< Access the P error squared

      inline float errX() const noexcept { return std::sqrt( errX2() ); }   ///< Access the x error
      inline float errY() const noexcept { return std::sqrt( errY2() ); }   ///< Access the y error
      inline float errTX() const noexcept { return std::sqrt( errTX2() ); } ///< Access the tx error
      inline float errTY() const noexcept { return std::sqrt( errTY2() ); } ///< Access the ty error
      inline float errP() const noexcept { return std::sqrt( errP2() ); }   ///< Access the P error

    public:
      ///< send to std::ostream
      inline friend std::ostream& operator<<( std::ostream& s, const StateErrors& errs ) {
        s << "{ errX2=" << errs.errX2() << " errY2=" << errs.errY2() << " errTX2=" << errs.errTX2()
          << " errTY2=" << errs.errTY2() << " errP2=" << errs.errP2() << " }";
        return s;
      }

    private:
      float m_errX2{0};  ///< The x error squared
      float m_errY2{0};  ///< The y error squared
      float m_errTX2{0}; ///< The tx error squared
      float m_errTY2{0}; ///< The ty error squared
      float m_errP2{0};  ///< The P error squared
    };

    // ------------------------------------------------------------------------------------------------------

    /// Private SIMD implementation of Rotation3D (Not yet in ROOT)
    class SIMDRotation3D {
    private:
      /// 9 elements (3x3 matrix) representing the rotation
      std::array<SIMDFP, 9> fM;

    public:
      /// Default Constructor
      SIMDRotation3D() {}
      /// Construct from Gaudi::Rotation3D
      SIMDRotation3D( const Gaudi::Rotation3D& rot ) {
        std::array<Gaudi::Rotation3D::Scalar, 9> m;
        rot.GetComponents( m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8] );
        for ( int i = 0; i < 9; ++i ) { fM[i] = FP( m[i] ); }
      }
      /// Vector rotation operator
      template <typename VECTOR>
      inline VECTOR operator*( const VECTOR& v ) const noexcept {
        return VECTOR( fM[0] * v.X() + fM[1] * v.Y() + fM[2] * v.Z(), // x
                       fM[3] * v.X() + fM[4] * v.Y() + fM[5] * v.Z(), // y
                       fM[6] * v.X() + fM[7] * v.Y() + fM[8] * v.Z()  // z
        );
      }
    };

  private:
    /// Helper method for two point constructor
    inline void initTwoPoints() noexcept {
      // create middle state
      setMiddleState( add_points( entryPoint(), exitPoint() ) * 0.5, ( entryMomentum() + exitMomentum() ) * 0.5 );
      // Note the above calls updateCachedInfo() so no need to do it here.
    }

    /// Helper method for three point constructor
    inline void initThreePoints() noexcept {
      // fill the cached information.
      updateCachedInfo();
    }

    // ------------------------------------------------------------------------------------------------------

  public:
    /** Constructor that uses the supplied radiator intersections for the entry and exit points.
     *
     *  The middle point and the momentum at the middle point are derived from the average
     *  of the entry and exit states.
     *
     *  Information is moved in this implementation.
     */
    template <class INTERS>
    RichTrackSegment( INTERS&&                 inters,                    ///< The radiator intersections
                      const Rich::RadiatorType rad,                       ///< The radiator type
                      const Rich::DetectorType rich,                      ///< The detector type
                      const StateErrors&       entryErrs = StateErrors{}, ///< The segment errors at the entry point
                      const StateErrors&       exitErrs  = StateErrors{}  ///< The segment errors at the exit point
                      )
        : m_radiator( rad )
        , m_rich( rich )
        , m_radIntersections( std::forward<INTERS>( inters ) )
        , m_errorsEntry( entryErrs )
        , m_errorsMiddle( Rich::Rich1Gas == rad ? exitErrs : entryErrs )
        , // CRJ : Is this best ?
        m_errorsExit( exitErrs )
        , m_avPhotonEnergy( avPhotEn( rad ) ) {
      initTwoPoints();
    }

    /** Constructor that uses the supplied radiator intersections for the entry and exit points.
     *
     *  In addition the middle point and the momentum vector at that point are given explicitly.
     *
     *  Information is moved in this implementation.
     */
    template <class INTERS>
    RichTrackSegment( INTERS&&                 inters, ///< The radiator intersections
                      const Gaudi::XYZPoint&   middP,  ///< The mid point in the radiator
                      const Gaudi::XYZVector&  middV,  ///< The momentum vector at the radiator mid point
                      const Rich::RadiatorType rad,    ///< The radiator type
                      const Rich::DetectorType rich,   ///< The detector type
                      const StateErrors&       entryErrors  = StateErrors{}, ///< The segment errors at the entry point
                      const StateErrors&       middleErrors = StateErrors{}, ///< The segment errors at the mid point
                      const StateErrors&       exitErrors   = StateErrors{}  ///< The segment errors at the exit point
                      )
        : m_radiator( rad )
        , m_rich( rich )
        , m_radIntersections( std::forward<INTERS>( inters ) )
        , m_middlePoint( middP )
        , m_middleMomentum( middV )
        , m_errorsEntry( entryErrors )
        , m_errorsMiddle( middleErrors )
        , m_errorsExit( exitErrors )
        , m_avPhotonEnergy( avPhotEn( rad ) ) {
      initThreePoints();
    }

    // ------------------------------------------------------------------------------------------------------

  public:
    /// Standard constructor
    RichTrackSegment() = default;

    // ------------------------------------------------------------------------------------------------------

  public:
    /// Provides read-only access to the radiator intersections
    inline const Rich::RadIntersection::Vector& radIntersections() const noexcept { return m_radIntersections; }

    /** Update the track using a given transformation matrix and a fixed point
     *
     *  @param rotPnt Fixed point for transformation
     *  @param trans  Transformation matrix to apply to the segment
     */
    void updateState( const Gaudi::XYZPoint& rotPnt, const Gaudi::Transform3D& trans );

    /** Calculates the theta and phi angles of a direction with respect to
     *  the segment direction.
     *
     *  The precision on the determination of Cherenkov theta and phi can be set
     *  individually via template parameters. In general, less precision is required in
     *  phi than theta, so by default use 0 for phi and 1 for theta.
     *
     *  @param direction Direction to which to calculate the angles for this segment
     *  @param theta The angle between input direction and the segment
     *  @param phi   The azimuthal angle of the direction around the segment
     */
    template <std::size_t THETA_PRECISION = 1, //
              std::size_t PHI_PRECISION   = 0, //
              typename VECTOR, typename TYPE>
    inline void __attribute__( ( always_inline ) ) //
    angleToDirection( const VECTOR& direction, TYPE& theta, TYPE& phi ) const {
      if constexpr ( std::is_arithmetic_v<TYPE> ) {
        // scalar implementation
        // create vector in track reference frame
        const auto r = m_rotation * direction;
        // compute theta and phi directly from the vector components
        const auto perp = std::sqrt( ( r.x() * r.x() ) + ( r.y() * r.y() ) );
        theta           = my_atan2<THETA_PRECISION>( perp, r.z() );
        phi             = my_atan2<PHI_PRECISION>( r.y(), r.x() );
        // correct phi to range 0 - 2PI
        constexpr TYPE twopi = TYPE( 2.0 * M_PI );
        if ( phi < 0 ) { phi += twopi; }
      } else if constexpr ( LHCb::SIMD::is_SIMD_v<TYPE> ) {
        // SIMD version
        // create vector in track reference frame
        const auto r = m_rotationSIMD * direction;
        // compute theta and phi directly from the vector components
        const auto perp = std::sqrt( ( r.x() * r.x() ) + ( r.y() * r.y() ) );
        theta           = my_atan2<THETA_PRECISION>( perp, r.z() );
        phi             = my_atan2<PHI_PRECISION>( r.y(), r.x() );
        // correct phi to range 0 - 2PI
        phi( phi < TYPE::Zero() ) += TYPE( 2.0 * M_PI );
      } else {
        // If get here unknown types so force compilation failure
        TYPE::WillFail();
      }
    }

    /** Creates a vector at an given angle and azimuth to the track segment
     *
     *  @param theta The angle between this track segment and the created vector
     *  @param phi   The azimuthal angle of the vector to this track segment
     *
     *  @return The vector at the given theta and phi angles to this track segment
     */
    template <typename TYPE>
    inline decltype( auto ) vectorAtThetaPhi( const TYPE& theta, const TYPE& phi ) const {
      using namespace LHCb::Math;
      TYPE sinTheta( 0 ), cosTheta( 0 ), sinPhi( 0 ), cosPhi( 0 );
      fast_sincos( theta, sinTheta, cosTheta );
      fast_sincos( phi, sinPhi, cosPhi );
      return vectorAtCosSinThetaPhi( cosTheta, sinTheta, cosPhi, sinPhi );
    }

    /** Creates a vector at an given angle and azimuth to the track segment (scalar)
     *
     *  @param cosTheta Cosine of the angle between this track segment and the created vector
     *  @param sinTheta Sine   of the angle between this track segment and the created vector
     *  @param cosPhi   Cosine of the azimuthal angle of the vector to this track segment
     *  @param sinPhi   Sine   of the azimuthal angle of the vector to this track segment
     *
     *  @return The vector at the given theta and phi angles to this track segment
     */
    template <
        typename THETA, typename PHI,
        typename std::enable_if<std::is_arithmetic<THETA>::value && std::is_arithmetic<PHI>::value>::type* = nullptr>
    inline decltype( auto ) vectorAtCosSinThetaPhi( const THETA cosTheta, //
                                                    const THETA sinTheta, //
                                                    const PHI   cosPhi,   //
                                                    const PHI   sinPhi    //
                                                    ) const noexcept {
      return m_rotation2 * Gaudi::XYZVector( sinTheta * cosPhi, sinTheta * sinPhi, cosTheta );
    }

    /** Creates a vector at an given angle and azimuth to the track segment (SIMD)
     *
     *  @param cosTheta Cosine of the angle between this track segment and the created vector
     *  @param sinTheta Sine   of the angle between this track segment and the created vector
     *  @param cosPhi   Cosine of the azimuthal angle of the vector to this track segment
     *  @param sinPhi   Sine   of the azimuthal angle of the vector to this track segment
     *
     *  @return The vector at the given theta and phi angles to this track segment
     */
    inline decltype( auto ) vectorAtCosSinThetaPhi( const SIMDFP cosTheta, //
                                                    const SIMDFP sinTheta, //
                                                    const SIMDFP cosPhi,   //
                                                    const SIMDFP sinPhi    //
                                                    ) const noexcept {
      return m_rotation2SIMD * SIMDVector( sinTheta * cosPhi, sinTheta * sinPhi, cosTheta );
    }

    /** Calculates the path lenth of a track segment.
     *  @returns The total length of the track inside the radiator
     */
    inline decltype( auto ) pathLength() const noexcept { return m_pathLength; }

    /// Returns the segment entry point to the radiator
    inline const Gaudi::XYZPoint& entryPoint() const noexcept { return radIntersections().front().entryPoint(); }

    /// Returns the segment entry point to the radiator
    inline const SIMDPoint& entryPointSIMD() const noexcept { return m_entryPointSIMD; }

    /// Returns the segment mid-point in the radiator
    inline const Gaudi::XYZPoint& middlePoint() const noexcept { return m_middlePoint; }

    /// Returns the segment mid-point in the radiator
    inline const SIMDPoint& middlePointSIMD() const noexcept { return m_middlePointSIMD; }

    /// Returns the segment exit point from the radiator (scalar)
    inline const Gaudi::XYZPoint& exitPoint() const noexcept { return radIntersections().back().exitPoint(); }

    /// Returns the segment exit point from the radiator (SIMD)
    inline const SIMDPoint& exitPointSIMD() const noexcept { return m_exitPointSIMD; }

    /// Returns the z coordinate at a given fractional distance along segment
    template <typename TYPE>
    inline TYPE zCoordAt( const TYPE fraction ) const {
      if constexpr ( std::is_arithmetic_v<TYPE> ) {
        // scalar implementation
        return ( fraction * exitPoint().z() ) + ( ( TYPE( 1 ) - fraction ) * entryPoint().z() );
      } else if constexpr ( LHCb::SIMD::is_SIMD_v<TYPE> ) {
        // SIMD version
        return ( fraction * exitPointSIMD().z() ) + ( ( TYPE::One() - fraction ) * entryPointSIMD().z() );
      }
    }

    /// Returns the best space point for segment at a given fractional distance along segment.
    /// Zero gives the entry point, one gives the exit point
    template <typename TYPE>
    inline decltype( auto ) bestPoint( const TYPE fractDist ) const {
      // return the best point
      if constexpr ( std::is_arithmetic_v<TYPE> ) {
        // scalar implementation
        return ( zCoordAt( fractDist ) < middlePoint().z()
                     ? entryPoint() + ( fractDist * m_invMidFrac1 * m_midEntryV )
                     : middlePoint() + ( m_exitMidV * ( ( fractDist - m_midFrac2 ) / m_midFrac2 ) ) );
      } else if constexpr ( LHCb::SIMD::is_SIMD_v<TYPE> ) {
        // SIMD version
        using namespace LHCb::SIMD;
        auto       p    = middlePointSIMD() + ( m_exitMidVSIMD * ( ( fractDist - m_midFrac2SIMD ) / m_midFrac2SIMD ) );
        const auto mask = zCoordAt( fractDist ) < middlePointSIMD().z();
        if ( any_of( mask ) ) // need to decide if it helps to do this
        {
          const auto p2 = entryPointSIMD() + ( fractDist * m_invMidFrac1SIMD * m_midEntryVSIMD );
          // Need to fix GenVector to allow direct blending of those types..
          SIMDFP x( p.x() ), y( p.y() ), z( p.z() );
          x( mask ) = p2.x();
          y( mask ) = p2.y();
          z( mask ) = p2.z();
          p         = SIMDPoint( x, y, z );
        }
        return p;
      }
    }

    /// Returns the best estimate of the average point in the radiator
    /// Equivalent to RichTrackSegment::bestPoint(0.5), but more efficient
    inline const Gaudi::XYZPoint& bestPoint() const noexcept { return middlePoint(); }

    /// Returns the best estimate of the average point in the radiator
    /// Equivalent to RichTrackSegment::bestPoint(0.5), but more efficient
    inline const SIMDPoint& bestPointSIMD() const noexcept { return middlePointSIMD(); }

    /// Returns the momentum vector at entry
    inline const Gaudi::XYZVector& entryMomentum() const noexcept { return radIntersections().front().entryMomentum(); }

    /// Returns the momentum vector at the mid point
    inline const Gaudi::XYZVector& middleMomentum() const noexcept { return m_middleMomentum; }

    /// Returns the momentum vector at exit
    inline const Gaudi::XYZVector& exitMomentum() const noexcept { return radIntersections().back().exitMomentum(); }

    /// Returns the best direction for segment at a given fractional distance along segment
    Gaudi::XYZVector bestMomentum( const double fractDist ) const;

    /// Returns the best estimate of the average momentum vector for the entire segment
    /// Equivalent to RichTrackSegment::bestMomentum(0.5) but more efficient
    inline const Gaudi::XYZVector& bestMomentum() const noexcept { return m_middleMomentum; }

    /// Returns the magnitude of the best momentum vector
    inline double bestMomentumMag() const { return std::sqrt( m_middleMomentum.Mag2() ); }

    /// Returns the radiator type
    inline Rich::RadiatorType radiator() const noexcept { return m_radiator; }

    /// Returns the detector type
    inline Rich::DetectorType rich() const noexcept { return m_rich; }

    /// Set the entry state
    inline void setEntryState( const Gaudi::XYZPoint& point, const Gaudi::XYZVector& dir ) {
      radIntersections().front().setEntryPoint( point );
      radIntersections().front().setEntryMomentum( dir );
      updateCachedInfo();
    }

    /// Set the Middle state
    inline void setMiddleState( const Gaudi::XYZPoint& point, const Gaudi::XYZVector& dir ) {
      m_middlePoint    = point;
      m_middleMomentum = dir;
      updateCachedInfo();
    }

    /// Set the exit state
    inline void setExitState( const Gaudi::XYZPoint& point, const Gaudi::XYZVector& dir ) {
      radIntersections().back().setExitPoint( point );
      radIntersections().back().setExitMomentum( dir );
      updateCachedInfo();
    }

    /// Set all states
    inline void setStates( const Gaudi::XYZPoint& entry_point, const Gaudi::XYZVector& entry_dir,
                           const Gaudi::XYZPoint& mid_point, const Gaudi::XYZVector& mid_dir,
                           const Gaudi::XYZPoint& exit_point, const Gaudi::XYZVector& exit_dir ) {
      radIntersections().front().setEntryPoint( entry_point );
      radIntersections().front().setEntryMomentum( entry_dir );
      m_middlePoint    = mid_point;
      m_middleMomentum = mid_dir;
      radIntersections().back().setExitPoint( exit_point );
      radIntersections().back().setExitMomentum( exit_dir );
      updateCachedInfo();
    }

    /// Set the radiator type
    inline void setRadiator( const Rich::RadiatorType rad ) noexcept { m_radiator = rad; }

    /// Set the rich detector
    inline void setRich( const Rich::DetectorType det ) noexcept { m_rich = det; }

    /// Returns the entry errors
    inline const StateErrors& entryErrors() const noexcept { return m_errorsEntry; }

    /// Returns the middle point errors
    inline const StateErrors& middleErrors() const noexcept { return m_errorsMiddle; }

    /// Returns the exit errors
    inline const StateErrors& exitErrors() const noexcept { return m_errorsExit; }

    /// Returns the average observable photon energy
    inline float avPhotonEnergy() const noexcept { return m_avPhotonEnergy; }

    /// Sets the average observable photon energy
    inline void setAvPhotonEnergy( const float energy ) noexcept { m_avPhotonEnergy = energy; }

    /// Reset the segment
    inline void reset() { updateCachedInfo(); }

  public:
    /// Printout method
    std::ostream& fillStream( std::ostream& s ) const;

    /// Implement ostream << method for RichTrackSegment
    friend inline std::ostream& operator<<( std::ostream& s, const LHCb::RichTrackSegment& segment ) {
      return segment.fillStream( s );
    }

  private:
    // methods

    /// Provides write access to the radiator intersections
    inline Rich::RadIntersection::Vector& radIntersections() noexcept { return m_radIntersections; }

    /// Updates the cached information
    void updateCachedInfo();

    /** atan2 for given precision level
     *
     *  PRECISION >= 2 : Use STL std::atan2. 'reference' precision.
     *  PRECISION == 1 : Use LHCbMath 'VDT inspired' implementation.
     *  PRECISION == 0 : Use LHCbMath approximate (factor 2 w.r.t. VDT) implementation.
     */
    template <std::size_t PRECISION, typename TYPE>
    inline TYPE __attribute__( ( always_inline ) ) //
    my_atan2( const TYPE y, const TYPE x ) const noexcept {
      using namespace LHCb::Math;
      if constexpr ( PRECISION >= 2 ) {
        return std::atan2( y, x );
      } else if constexpr ( PRECISION == 1 ) {
        return fast_atan2( y, x );
      } else {
        return Approx::vapprox_atan2( y, x );
      }
    }

  private:
    // private data

    Rich::RadiatorType m_radiator = Rich::InvalidRadiator; ///< Rich radiator
    Rich::DetectorType m_rich     = Rich::InvalidDetector; ///< Rich detector

    /// The raw intersections with the radiator volumes
    Rich::RadIntersection::Vector m_radIntersections{1};

    /// The middle point of the segment in the radiator volume (Scalar)
    Gaudi::XYZPoint m_middlePoint;

    /// The momentum vector at the segment middle point in the radiator volume
    Gaudi::XYZVector m_middleMomentum;

    StateErrors m_errorsEntry;  ///< Errors for the entry state
    StateErrors m_errorsMiddle; ///< Errors for the middle state
    StateErrors m_errorsExit;   ///< Errors for the exit state

    /** The average observable photon energy for this segment.
     *  Set to the average for Rich1 and Rich2 gas radiators...
     *  @todo Quick fix. Need to review to if this can be done in a better way
     *        without the need for this variable.
     */
    float m_avPhotonEnergy = 4.325;

  private: // SIMD data caches
    /// SIMD Entry Point
    SIMDPoint m_entryPointSIMD;

    /// SIMD Exit Point
    SIMDPoint m_exitPointSIMD;

    /// The middle point of the segment in the radiator volume (SIMD)
    SIMDPoint m_middlePointSIMD;

    /// Entry to middle point vector
    SIMDVector m_midEntryVSIMD;

    /// Middle to exit point vector
    SIMDVector m_exitMidVSIMD;

    SIMDFP m_invMidFrac1SIMD; ///< Cached fraction 1
    SIMDFP m_midFrac2SIMD;    ///< Cached fraction 2

    /// SIMD rotations
    SIMDRotation3D m_rotationSIMD, m_rotation2SIMD;

  private: // Some variables for internal caching of information for speed
    /** Rotation matrix used to calculate the theta and phi angles between
     *  this track segment and a given direction. */
    Gaudi::Rotation3D m_rotation;

    /** Rotation matrix used to create vectors at a given theta and phi angle
     *  to this track segment. */
    Gaudi::Rotation3D m_rotation2;

    Gaudi::XYZVector m_midEntryV;      ///< Entry to middle point vector
    Gaudi::XYZVector m_exitMidV;       ///< Middle to exit point vector
    double           m_invMidFrac1{0}; ///< Cached fraction 1
    double           m_midFrac2{0};    ///< Cached fraction 2
    double           m_pathLength{0};  ///< Segment path length
  };

  /// TES locations
  namespace RichTrackSegmentLocation {
    /// Default Location in TES for the track segments
    inline const std::string Default = "Rec/Rich/TrackSegments/Default";
  } // namespace RichTrackSegmentLocation

} // namespace LHCb
