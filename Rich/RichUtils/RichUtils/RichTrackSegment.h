
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

#ifndef RICHUTILS_RichTrackSegment_H
#define RICHUTILS_RichTrackSegment_H 1

// std include
#include <iostream>
#include <memory>
#include <cmath>

// LHCbKernel
#include "Kernel/RichRadiatorType.h"
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichRadIntersection.h"
#include "Kernel/FastAllocVector.h"

// geometry
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "GaudiKernel/Transform3DTypes.h"

// Kernel
#include "Kernel/MemPoolAlloc.h"

// local
#include "RichUtils/RichException.h"

// VDT
#include "vdt/sincos.h"
#include "vdt/sqrt.h"
#include "vdt/atan2.h"

// General LHCb namespace
namespace LHCb
{

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

  class RichTrackSegment final : public LHCb::MemPoolAlloc<LHCb::RichTrackSegment>
  {

  public:
    
    /// Vector of track segments
    using Vector = LHCb::STL::Vector<RichTrackSegment>;
    
  private:

    /** Add two Gaudi::XYZPoint points together
     *  @param p1 The first point
     *  @param p2 The second point
     *  @return The point represented by "p1+p2"
     */
    inline Gaudi::XYZPoint add_points ( const Gaudi::XYZPoint & p1,
                                        const Gaudi::XYZPoint & p2 ) const noexcept
    {
      return Gaudi::XYZPoint ( p1.x()+p2.x(), p1.y()+p2.y(), p1.z()+p2.z() );
    }

    /** Returns the average photon energy for the given RICH radiator
     *  @param[in] rad The radiator type
     *  @return The average photon energy */
    inline double avPhotEn( const Rich::RadiatorType rad ) const
    {
      return ( Rich::Rich1Gas == rad ? 4.25 :  // C4F10
               Rich::Rich2Gas == rad ? 4.40 :  // CF4
               3.00                        );  // Aerogel 
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
    class StateErrors final
    {

    public:

      /// Default Constructor
      StateErrors() { }

    public:

      /// Constructor with explicit float values
      StateErrors( const float errX2  , ///< error on x squared
                   const float errY2  , ///< error on y squared
                   const float errTX2 , ///< error on x slope squared
                   const float errTY2 , ///< error on y slope squared
                   const float errP2    ///< error on momentum squared
                   )
        : m_errX2  ( fabsf(errX2)  ),
          m_errY2  ( fabsf(errY2)  ),
          m_errTX2 ( fabsf(errTX2) ),
          m_errTY2 ( fabsf(errTY2) ),
          m_errP2  ( fabsf(errP2)  ) { }

      /// Constructor with explicit double values
      StateErrors( const double errX2,  ///< error on x squared
                   const double errY2,  ///< error on y squared
                   const double errTX2, ///< error on x slope squared
                   const double errTY2, ///< error on y slope squared
                   const double errP2   ///< error on momentum squared
                   )
        : m_errX2  ( fabsf((float)(errX2))  ),
          m_errY2  ( fabsf((float)(errY2))  ),
          m_errTX2 ( fabsf((float)(errTX2)) ),
          m_errTY2 ( fabsf((float)(errTY2)) ),
          m_errP2  ( fabsf((float)(errP2))  ) { }

      inline float errX2()  const noexcept { return m_errX2;  }  ///< Access the x error squared
      inline float errY2()  const noexcept { return m_errY2;  }  ///< Access the y error squared
      inline float errTX2() const noexcept { return m_errTX2; }  ///< Access the tx error squared
      inline float errTY2() const noexcept { return m_errTY2; }  ///< Access the ty error squared
      inline float errP2()  const noexcept { return m_errP2;  }  ///< Access the P error squared

      inline float errX()  const noexcept { return std::sqrt(errX2());  }  ///< Access the x error
      inline float errY()  const noexcept { return std::sqrt(errY2());  }  ///< Access the y error
      inline float errTX() const noexcept { return std::sqrt(errTX2()); }  ///< Access the tx error
      inline float errTY() const noexcept { return std::sqrt(errTY2()); }  ///< Access the ty error
      inline float errP()  const noexcept { return std::sqrt(errP2());  }  ///< Access the P error

      ///< send to std::ostream
      inline friend std::ostream& operator << ( std::ostream& s,
                                                const StateErrors& errs )
      {
        s << "{ errX2=" << errs.errX2() << " errY2=" << errs.errY2()
          << " errTX2=" << errs.errTX2() << " errTY2=" << errs.errTY2()
          << " errP2=" << errs.errP2() << " }";
        return s;
      }

    private:

      float m_errX2  {0};  ///< The x error squared
      float m_errY2  {0};  ///< The y error squared
      float m_errTX2 {0};  ///< The tx error squared
      float m_errTY2 {0};  ///< The ty error squared
      float m_errP2  {0};  ///< The P error squared

    };

    /// Enum to flag to determine how to create the RichTrackSegment
    enum SegmentType : int8_t
      {
        UnDefined         = 0, ///< Undefined segment type
        UseChordBetweenStates, ///< Uses full state information to define the segment direction
        UseAllStateVectors     ///< Uses chord between the entry and exit points. NB : Under development - Do not use yet
      };

  public:

    /// The segment type
    inline LHCb::RichTrackSegment::SegmentType type() const noexcept
    {
      return m_type;
    }

    // ------------------------------------------------------------------------------------------------------

  private:

    /// Helper method to initialise the based chord constructor without a middle point
    void chordConstructorInit2();

    /// Helper method to initialise the based chord constructor with a middle point
    void chordConstructorInit3();

    /// Helper method for two point constructor
    inline void initTwoPoints()
    {
      if      ( RichTrackSegment::UseAllStateVectors    == type() )
      {
        setMiddleState ( add_points(entryPoint(),exitPoint())/2, (entryMomentum()+exitMomentum())/2 );
      }
      else if ( RichTrackSegment::UseChordBetweenStates == type() )
      {
        chordConstructorInit2();
      }
      else
      {
        throw Rich::Exception( "Unknown RichTrackSegment::SegmentType" );
      }
      // fill the cached information.
      updateCachedInfo();
    }

    /// Helper method for three point constructor
    inline void initThreePoints()
    {
      if      ( RichTrackSegment::UseAllStateVectors    == type() )
      {
        // nothing to do yet
      }
      else if ( RichTrackSegment::UseChordBetweenStates == type() )
      {
        chordConstructorInit3();
      }
      else
      {
        throw Rich::Exception( "Unknown RichTrackSegment::SegmentType" );
      }
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
     *  Information is copied in this implementation.
     */
    RichTrackSegment( const SegmentType t,           ///< The segment type
                      const Rich::RadIntersection::Vector & intersects, ///< The radiator intersections
                      const Rich::RadiatorType rad,  ///< The radiator type
                      const Rich::DetectorType rich, ///< The detector type
                      const StateErrors& entryErrors = StateErrors{}, ///< The segment errors at the entry point
                      const StateErrors& exitErrors  = StateErrors{}  ///< The segment errors at the exit point
                      )
      : m_type             ( t             ),
        m_radIntersections ( intersects    ),
        m_radiator         ( rad           ),
        m_rich             ( rich          ),
        m_errorsEntry      ( entryErrors   ),
        m_errorsMiddle     ( Rich::Rich1Gas == rad ? exitErrors : entryErrors ), // CRJ : Is this best ?
        m_errorsExit       ( exitErrors    ),
        m_avPhotonEnergy   ( avPhotEn(rad) )
    {
      initTwoPoints();
    }

    /** Constructor that uses the supplied radiator intersections for the entry and exit points.
     *
     *  The middle point and the momentum at the middle point are derived from the average
     *  of the entry and exit states.
     *
     *  Information is moved in this implementation.
     */
    RichTrackSegment( const SegmentType t,           ///< The segment type
                      Rich::RadIntersection::Vector && intersects, ///< The radiator intersections
                      const Rich::RadiatorType rad,  ///< The radiator type
                      const Rich::DetectorType rich, ///< The detector type
                      StateErrors&& entryErrs = StateErrors{}, ///< The segment errors at the entry point
                      StateErrors&& exitErrs  = StateErrors{}  ///< The segment errors at the exit point
                      )
      : m_type             ( t                     ),
        m_radIntersections ( std::move(intersects) ),
        m_radiator         ( rad                   ),
        m_rich             ( rich                  ),
        m_errorsEntry      ( std::move(entryErrs)  ),
        m_errorsExit       ( std::move(exitErrs)   ),
        m_avPhotonEnergy   ( avPhotEn(rad)         )
    {
      m_errorsMiddle = ( Rich::Rich1Gas == rad ? exitErrors() : entryErrors() ); // CRJ : Is this best ?
      initTwoPoints();
    }

    /** Constructor that uses the supplied radiator intersections for the entry and exit points.
     *
     *  In addition the middle point and the momentum vector at that point are given explicitly.
     *
     *  Information is copied in this implementation.
     */
    RichTrackSegment( const SegmentType t,           ///< The segment type
                      const Rich::RadIntersection::Vector & intersects, ///< The radiator intersections
                      const Gaudi::XYZPoint& middP,  ///< The mid point in the radiator
                      const Gaudi::XYZVector& middV, ///< The momentum vector at the radiator mid point
                      const Rich::RadiatorType rad,  ///< The radiator type
                      const Rich::DetectorType rich, ///< The detector type
                      const StateErrors& entryErrors  = StateErrors{}, ///< The segment errors at the entry point
                      const StateErrors& middleErrors = StateErrors{}, ///< The segment errors at the mid point
                      const StateErrors& exitErrors   = StateErrors{}  ///< The segment errors at the exit point
                      )
      : m_type             ( t             ),
        m_radIntersections ( intersects    ),
        m_middlePoint      ( middP         ),
        m_middleMomentum   ( middV         ),
        m_radiator         ( rad           ),
        m_rich             ( rich          ),
        m_errorsEntry      ( entryErrors   ),
        m_errorsMiddle     ( middleErrors  ),
        m_errorsExit       ( exitErrors    ),
        m_avPhotonEnergy   ( avPhotEn(rad) )
    {
      initThreePoints();
    }

    /** Constructor that uses the supplied radiator intersections for the entry and exit points.
     *
     *  In addition the middle point and the momentum vector at that point are given explicitly.
     *
     *  Information is noved in this implementation.
     */
    RichTrackSegment( const SegmentType t,           ///< The segment type
                      Rich::RadIntersection::Vector && intersects, ///< The radiator intersections
                      Gaudi::XYZPoint  && middP,     ///< The mid point in the radiator
                      Gaudi::XYZVector && middV,     ///< The momentum vector at the radiator mid point
                      const Rich::RadiatorType rad,  ///< The radiator type
                      const Rich::DetectorType rich, ///< The detector type
                      StateErrors&& entryErrors  = StateErrors{}, ///< The segment errors at the entry point
                      StateErrors&& middleErrors = StateErrors{}, ///< The segment errors at the mid point
                      StateErrors&& exitErrors   = StateErrors{}  ///< The segment errors at the exit point
                      )
      : m_type             ( t                       ),
        m_radIntersections ( std::move(intersects)   ),
        m_middlePoint      ( std::move(middP)        ),
        m_middleMomentum   ( std::move(middV)        ),
        m_radiator         ( rad                     ),
        m_rich             ( rich                    ),
        m_errorsEntry      ( std::move(entryErrors)  ),
        m_errorsMiddle     ( std::move(middleErrors) ),
        m_errorsExit       ( std::move(exitErrors)   ),
        m_avPhotonEnergy   ( avPhotEn(rad)           )
    {
      initThreePoints();
    }

    // ------------------------------------------------------------------------------------------------------

  public:

    /// Standard constructor
    RichTrackSegment() = default;

    // ------------------------------------------------------------------------------------------------------

  private:

    /// atan2 for double
    inline double myatan2( const double x, const double y ) const noexcept { return vdt::fast_atan2(x,y);  }

    /// atan2 for float
    inline float  myatan2( const float x,  const float y  ) const noexcept { return vdt::fast_atan2f(x,y); }

    /// sincos for double
    inline void mysincos( const double x, double & s, double & c ) const noexcept { vdt::fast_sincos(x,s,c); }

    /// sincos for float
    inline void mysincos( const float  x, float  & s, float  & c ) const noexcept { vdt::fast_sincosf(x,s,c); }

    // ------------------------------------------------------------------------------------------------------

  public:

    /// Provides read-only access to the radiator intersections
    inline const Rich::RadIntersection::Vector & radIntersections() const noexcept
    {
      return m_radIntersections;
    }

    /** Update the track using a given transformation matrix and a fixed point
     *
     *  @param rotPnt Fixed point for transformation
     *  @param trans  Transformation matrix to apply to the segment
     */
    void updateState( const Gaudi::XYZPoint & rotPnt,
                      const Gaudi::Transform3D & trans );

    /** Calculates the theta and phi angles of a direction with respect to
     *  the segment direction
     *
     *  @param direction Direction to which to calculate the angles for this segment
     *  @param theta The angle between input direction and the segment
     *  @param phi   The azimuthal angle of the direction around the segment
     */
    template < typename VECTOR, typename TYPE >
    inline void __attribute__((always_inline))
      angleToDirection ( const VECTOR & direction,
                         TYPE & theta,
                         TYPE & phi ) const
    {
      // create vector in track reference frame
      const auto rotDir = rotationMatrix() * direction;
      // compute theta and phi directly from the vector components
      phi   = myatan2( (TYPE)rotDir.y(), (TYPE)rotDir.x() );
      theta = myatan2( (TYPE)std::sqrt( std::pow((TYPE)rotDir.x(),2) +
                                        std::pow((TYPE)rotDir.y(),2) ),
                       (TYPE)rotDir.z() );
      // correct phi to range 0 - 2PI
      constexpr TYPE twopi = (TYPE)(2.0*M_PI);
      if ( phi < 0 ) { phi += twopi; }
    }

    /** Creates a vector at an given angle and azimuth to the track segment
     *
     *  @param theta The angle between this track segment and the created vector
     *  @param phi   The azimuthal angle of the vector to this track segment
     *
     *  @return The vector at the given theta and phi angles to this track segment
     */
    template < typename TYPE >
    inline Gaudi::XYZVector __attribute__((always_inline))
      vectorAtThetaPhi ( const TYPE theta,
                         const TYPE phi ) const
    {
      TYPE sinTheta(0), cosTheta(0), sinPhi(0), cosPhi(0);
      mysincos( theta, sinTheta, cosTheta );
      mysincos( phi,   sinPhi,   cosPhi   );
      return vectorAtCosSinThetaPhi( cosTheta, sinTheta, cosPhi, sinPhi );
    }

    /** Creates a vector at an given angle and azimuth to the track segment
     *
     *  @param cosTheta Cosine of the angle between this track segment and the created vector
     *  @param sinTheta Sine   of the angle between this track segment and the created vector
     *  @param cosPhi   Cosine of the azimuthal angle of the vector to this track segment
     *  @param sinPhi   Sine   of the azimuthal angle of the vector to this track segment
     *
     *  @return The vector at the given theta and phi angles to this track segment
     */
    template< typename THETA, typename PHI >
    inline Gaudi::XYZVector vectorAtCosSinThetaPhi ( const THETA cosTheta,
                                                     const THETA sinTheta,
                                                     const PHI cosPhi,
                                                     const PHI sinPhi ) const
    {
      return rotationMatrix2() * Gaudi::XYZVector( sinTheta*cosPhi,
                                                   sinTheta*sinPhi,
                                                   cosTheta );
    }

    /** Calculates the path lenth of a track segment.
     *  @returns The total length of the track inside the radiator
     */
    inline double pathLength() const noexcept { return m_pathLength; }

    /// Returns the segment entry point to the radiator
    inline const Gaudi::XYZPoint& entryPoint() const noexcept
    {
      return radIntersections().front().entryPoint();
    }

    /// Returns the segment mid-point in the radiator
    inline const Gaudi::XYZPoint& middlePoint() const noexcept
    {
      return m_middlePoint;
    }

    /// Returns the segment exit point from the radiator
    inline const Gaudi::XYZPoint& exitPoint() const noexcept
    {
      return radIntersections().back().exitPoint();
    }

    // need to double check this is correct...
    /// Returns the z coordinate at a given fractional distance along segment
    inline double zCoordAt( const double fraction ) const
    {
      return fraction*exitPoint().z() + (1-fraction)*entryPoint().z();
    }

    /// Returns the best space point for segment at a given fractional distance along segment.
    /// Zero gives the entry point, one gives the exit point
    inline Gaudi::XYZPoint bestPoint( const double fractDist ) const
    {
      // return the best point
      return ( zCoordAt(fractDist) < middlePoint().z() ?
               entryPoint()  + (fractDist*m_invMidFrac1*m_midEntryV) :
               middlePoint() + (m_exitMidV*((fractDist-m_midFrac2)/m_midFrac2)) );
    }
    
    /// Returns the best estimate of the average point in the radiator
    /// Equivalent to RichTrackSegment::bestPoint(0.5), but more efficient
    inline const Gaudi::XYZPoint& bestPoint() const noexcept
    {
      return m_middlePoint;
    }

    /// Returns the momentum vector at entry
    inline const Gaudi::XYZVector& entryMomentum() const noexcept
    {
      return radIntersections().front().entryMomentum();
    }

    /// Returns the momentum vector at the mid point
    inline const Gaudi::XYZVector& middleMomentum() const noexcept
    {
      return m_middleMomentum;
    }

    /// Returns the momentum vector at exit
    inline const Gaudi::XYZVector& exitMomentum() const noexcept
    {
      return radIntersections().back().exitMomentum();
    }

    /// Returns the best direction for segment at a given fractional distance along segment
    Gaudi::XYZVector bestMomentum( const double fractDist ) const;

    /// Returns the best estimate of the average momentum vector for the entire segment
    /// Equivalent to RichTrackSegment::bestMomentum(0.5) but more efficient
    inline const Gaudi::XYZVector& bestMomentum() const noexcept
    {
      return m_middleMomentum;
    }

    /// Returns the magnitude of the best momentum vector
    inline double bestMomentumMag() const
    {
      return std::sqrt( m_middleMomentum.Mag2() );
    }

    /// Returns the radiator type
    inline Rich::RadiatorType radiator() const noexcept
    {
      return m_radiator;
    }

    /// Returns the detector type
    inline Rich::DetectorType rich() const noexcept
    {
      return m_rich;
    }

    /// Set the entry state
    inline void setEntryState( const Gaudi::XYZPoint& point,
                               const Gaudi::XYZVector& dir )
    {
      radIntersections().front().setEntryPoint    ( point );
      radIntersections().front().setEntryMomentum ( dir   );
      updateCachedInfo();
    }

    /// Set the Middle state
    inline void setMiddleState( const Gaudi::XYZPoint& point,
                                const Gaudi::XYZVector& dir )
    {
      m_middlePoint    = point;
      m_middleMomentum = dir;
      updateCachedInfo();
    }

    /// Set the exit state
    inline void setExitState( const Gaudi::XYZPoint& point,
                              const Gaudi::XYZVector& dir )
    {
      radIntersections().back().setExitPoint    ( point );
      radIntersections().back().setExitMomentum ( dir   );
      updateCachedInfo();
    }

    /// Set all states
    inline void setStates( const Gaudi::XYZPoint&  entry_point,
                           const Gaudi::XYZVector& entry_dir,
                           const Gaudi::XYZPoint&  mid_point,
                           const Gaudi::XYZVector& mid_dir,
                           const Gaudi::XYZPoint&  exit_point,
                           const Gaudi::XYZVector& exit_dir )
    {
      radIntersections().front().setEntryPoint    ( entry_point );
      radIntersections().front().setEntryMomentum ( entry_dir   );
      m_middlePoint    = mid_point;
      m_middleMomentum = mid_dir;
      radIntersections().back().setExitPoint    ( exit_point );
      radIntersections().back().setExitMomentum ( exit_dir   );
      updateCachedInfo();
    }

    /// Set the radiator type
    inline void setRadiator(const Rich::RadiatorType rad) noexcept
    {
      m_radiator = rad;
    }

    /// Set the rich detector
    inline void setRich( const Rich::DetectorType det ) noexcept
    {
      m_rich = det;
    }

    /// Returns the entry errors
    inline const StateErrors & entryErrors() const noexcept
    {
      return m_errorsEntry;
    }

    /// Returns the middle point errors
    inline const StateErrors & middleErrors() const noexcept
    {
      return m_errorsMiddle;
    }

    /// Returns the exit errors
    inline const StateErrors & exitErrors() const noexcept
    {
      return m_errorsExit;
    }

    /// Returns the average observable photon energy
    inline double avPhotonEnergy() const noexcept
    {
      return m_avPhotonEnergy;
    }

    /// Sets the average observable photon energy
    inline void setAvPhotonEnergy( const double energy ) noexcept
    {
      m_avPhotonEnergy = energy;
    }

    /// Reset the segment
    inline void reset() { updateCachedInfo(); }

  public:

    /// Printout method
    std::ostream & fillStream ( std::ostream& s ) const;

    /// Implement ostream << method for RichTrackSegment
    friend inline std::ostream& operator << ( std::ostream& s,
                                              const LHCb::RichTrackSegment& segment )
    {
      return segment.fillStream(s);
    }

  private:  // methods

    /// Provides write access to the radiator intersections
    inline Rich::RadIntersection::Vector & radIntersections() noexcept
    {
      return m_radIntersections;
    }

    /// Access the rotation matrix 1
    inline const Gaudi::Rotation3D & rotationMatrix() const noexcept
    {
      return m_rotation;
    }

    /// Access the rotation matrix 2
    inline const Gaudi::Rotation3D & rotationMatrix2() const noexcept
    {
      return m_rotation2;
    }

    /// Updates the cached information
    void updateCachedInfo();

  private:  // private data

    /// The segment type
    SegmentType m_type = RichTrackSegment::UnDefined;

    /// The raw intersections with the radiator volumes
    Rich::RadIntersection::Vector m_radIntersections{1};

    /// The middle point of the segment in the radiator volume
    Gaudi::XYZPoint m_middlePoint;

    /// The momentum vector at the segment middle point in the radiator volume
    Gaudi::XYZVector m_middleMomentum;

    Rich::RadiatorType m_radiator = Rich::InvalidRadiator; ///< Rich radiator
    Rich::DetectorType m_rich     = Rich::InvalidDetector; ///< Rich detector

    StateErrors m_errorsEntry;     ///< Errors for the entry state
    StateErrors m_errorsMiddle;    ///< Errors for the middle state
    StateErrors m_errorsExit;      ///< Errors for the exit state

    /** The average observable photon energy for this segment.
     *  Set to the average for Rich1 and Rich2 gas radiators...
     *  @todo Quick fix. Need to review to if this can be done in a better way
     *        without the need for this variable.
     */
    double m_avPhotonEnergy = 4.325;

  private: // Some variables for internal caching of information for speed

    /** Rotation matrix used to calculate the theta and phi angles between
     *  this track segment and a given direction.
     *  Created on demand as required. */
    Gaudi::Rotation3D m_rotation;

    /** Rotation matrix used to create vectors at a given theta and phi angle
     *  to this track segment. Created on demand as required */
    Gaudi::Rotation3D m_rotation2;

    Gaudi::XYZVector m_midEntryV; ///< Entry to middle point vector
    Gaudi::XYZVector m_exitMidV;  ///< Middle to exit point vector
    double m_invMidFrac1{0};      ///< Cached fraction 1
    double m_midFrac2{0};         ///< Cached fraction 2
    double m_pathLength{0};       ///< Segment path length

  };

  /// TES locations
  namespace RichTrackSegmentLocation
  {
    /// Default Location in TES for the track segments
    static const std::string Default = "Rec/Rich/TrackSegments/Default";
  }
  
} // end LHCb namespace

#endif // RICHUTILS_RichTrackSegment_H
