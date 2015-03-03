
//-----------------------------------------------------------------------------
/** @file RichTrackSegment.h
 *
 *  Header file for tool interface : RichTrackSegment
 *
 *  CVS Log :-
 *  $Id: RichTrackSegment.h,v 1.10 2009-07-09 11:14:01 jonrob Exp $
 *
 *  @author Antonis Papanestis   Antonis.Papanestis@cern.ch
 *  @author Chris Jones          Christopher.Rob.Jones@cern.ch
 *  @date   2002-05-31
 */
//-----------------------------------------------------------------------------

#ifndef RICHKERNEL_RichTrackSegment_H
#define RICHKERNEL_RichTrackSegment_H 1

// std include
#include <iostream>

// LHCbKernel
#include "Kernel/RichRadiatorType.h"
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichRadIntersection.h"

// geometry
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "GaudiKernel/Transform3DTypes.h"

// Kernel
#include "Kernel/MemPoolAlloc.h"
#include "RichKernel/RichException.h"

// General LHCb namespace
namespace LHCb
{

  //-----------------------------------------------------------------------------
  /** @class RichTrackSegment RichTrackSegment.h RichKernel/RichTrackSegment.h
   *
   *  RichTrackSegment represents the trajectory of a Track through a radiator volume.
   *
   *  Implements the functionaility needed by the RICH reconstruction to interact with
   *  this trajectory in order to calculate the Cherenjkov angles etc.
   *
   *  @author Antonis Papanestis   Antonis.Papanestis@cern.ch
   *  @author Chris Jones          Christopher.Rob.Jones@cern.ch
   *  @date   2002-05-31
   *
   *  @todo Check if normalisation of axis vectors is really needed
   */
  //-----------------------------------------------------------------------------

  class RichTrackSegment : public LHCb::MemPoolAlloc<LHCb::RichTrackSegment>
  {

  private:

    /** Add two Gaudi::XYZPoint points together
     *  @param p1 The first point
     *  @param p2 The second point
     *  @return The point represented by "p1+p2"
     */
    inline Gaudi::XYZPoint add_points ( const Gaudi::XYZPoint & p1, const Gaudi::XYZPoint & p2 )
    {
      return Gaudi::XYZPoint ( p1.x()+p2.x(), p1.y()+p2.y(), p1.z()+p2.z() );
    }

    /** Returns the average photon energy for the given RICH radiator
     *  @param[in] rad The radiator type
     *  @return The average photon energy */
    inline double avPhotEn( const Rich::RadiatorType rad ) const
    {
      return ( Rich::Aerogel  == rad ? 3.00 :   // Aerogel
               Rich::Rich1Gas == rad ? 4.25 :   // C4F10
               4.4                              // CF4
               );
    }

  public: // helper classes

    //-----------------------------------------------------------------------------
    /** @class StateErrors RichTrackSegment.h RichKernel/RichTrackSegment.h
     *
     *  Helper class for RichTrackSegment to describe the segment errors.
     *
     *  @author Chris Jones          Christopher.Rob.Jones@cern.ch
     *  @date   2004-05-31
     */
    //-----------------------------------------------------------------------------
    class StateErrors
    {

    public:

      /// Copy constructor
      StateErrors( const StateErrors & errors ) :
        m_errX2  ( errors.errX2()  ),
        m_errY2  ( errors.errY2()  ),
        m_errTX2 ( errors.errTX2() ),
        m_errTY2 ( errors.errTY2() ),
        m_errP2  ( errors.errP2()  ) { }

      /// Constructor with explicit float values
      StateErrors( const float errX2  = 0, ///< error on x squared
                   const float errY2  = 0, ///< error on y squared
                   const float errTX2 = 0, ///< error on x slope squared
                   const float errTY2 = 0, ///< error on y slope squared
                   const float errP2  = 0  ///< error on momentum squared
                   )
        : m_errX2  ( fabs(errX2)  ),
          m_errY2  ( fabs(errY2)  ),
          m_errTX2 ( fabs(errTX2) ),
          m_errTY2 ( fabs(errTY2) ),
          m_errP2  ( fabs(errP2)  ) { }

      /// Constructor with explicit double values
      StateErrors( const double errX2,  ///< error on x squared
                   const double errY2,  ///< error on y squared
                   const double errTX2, ///< error on x slope squared
                   const double errTY2, ///< error on y slope squared
                   const double errP2   ///< error on momentum squared
                   )
        : m_errX2  ( fabs(static_cast<float>(errX2))  ),
          m_errY2  ( fabs(static_cast<float>(errY2))  ),
          m_errTX2 ( fabs(static_cast<float>(errTX2)) ),
          m_errTY2 ( fabs(static_cast<float>(errTY2)) ),
          m_errP2  ( fabs(static_cast<float>(errP2))  ) { }

      inline float errX2()  const { return m_errX2;  }  ///< Access the x error squared
      inline float errY2()  const { return m_errY2;  }  ///< Access the y error squared
      inline float errTX2() const { return m_errTX2; }  ///< Access the tx error squared
      inline float errTY2() const { return m_errTY2; }  ///< Access the ty error squared
      inline float errP2()  const { return m_errP2;  }  ///< Access the P error squared

      inline float errX()  const { return std::sqrt(errX2());  }  ///< Access the x error
      inline float errY()  const { return std::sqrt(errY2());  }  ///< Access the y error
      inline float errTX() const { return std::sqrt(errTX2()); }  ///< Access the tx error
      inline float errTY() const { return std::sqrt(errTY2()); }  ///< Access the ty error
      inline float errP()  const { return std::sqrt(errP2());  }  ///< Access the P error

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

      float m_errX2;  ///< The x error squared
      float m_errY2;  ///< The y error squared
      float m_errTX2; ///< The tx error squared
      float m_errTY2; ///< The ty error squared
      float m_errP2;  ///< The P error squared

    };

    /// Enum to flag to determine how to create the RichTrackSegment
    enum SegmentType
      {
        UnDefined             = 0,  ///< Undefined segment type
        UseChordBetweenStates,      ///< Uses full state information to define the segment direction
        UseAllStateVectors          ///< Uses chord between the entry and exit points. NB : Under development - Do not use yet
      };

  public:

    /// Delete and reset the current rotations
    inline void cleanUpRotations() const
    {
      if ( m_rotation  ) { delete m_rotation;  m_rotation  = 0; }
      if ( m_rotation2 ) { delete m_rotation2; m_rotation2 = 0; }
    }

    /// The segment type
    inline LHCb::RichTrackSegment::SegmentType type() const
    {
      return m_type;
    }

    // ------------------------------------------------------------------------------------------------------

  private:

    /// Helper method to initialise the based chord constructor without a middle point
    void chordConstructorInit2();

    /// Helper method to initialise the based chord constructor with a middle point
    void chordConstructorInit3();

    // ------------------------------------------------------------------------------------------------------

  public:

    /** Constructor that uses the supplied radiator intersections for the entry and exit points.
     *
     *  The middle point and the momentum at the middle point are derived from the average
     *  of the entry and exit states.
     */
    RichTrackSegment( const SegmentType t,           ///< The segment type
                      const RichRadIntersection::Vector & intersects, ///< The radiator intersections
                      const Rich::RadiatorType rad,  ///< The radiator type
                      const Rich::DetectorType rich, ///< The detector type
                      const StateErrors entryErrors  = StateErrors(), ///< The segment errors at the entry point
                      const StateErrors exitErrors   = StateErrors()  ///< The segment errors at the exit point
                      )
      : m_type             ( t             ),
        m_radIntersections ( intersects    ),
        m_radiator         ( rad           ),
        m_rich             ( rich          ),
        m_errorsEntry      ( entryErrors   ),
        m_errorsMiddle     ( Rich::Rich1Gas == rad ? exitErrors : entryErrors ), // CRJ : Is this best ?
        m_errorsExit       ( exitErrors    ),
        m_avPhotonEnergy   ( avPhotEn(rad) ),
        m_rotation         ( NULL          ),
        m_rotation2        ( NULL          )
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
        throw RichException( "Unknown RichTrackSegment::SegmentType" );
      }
    }

    /** Constructor that uses the supplied radiator intersections for the entry and exit points.
     *
     *  In addition the middle point and the momentum vector at that point are given explicitly.
     */
    RichTrackSegment( const SegmentType t,           ///< The segment type
                      const RichRadIntersection::Vector & intersects, ///< The radiator intersections
                      const Gaudi::XYZPoint& middP,  ///< The mid point in the radiator
                      const Gaudi::XYZVector& middV, ///< The momentum vector at the radiator mid point
                      const Rich::RadiatorType rad,  ///< The radiator type
                      const Rich::DetectorType rich, ///< The detector type
                      const StateErrors entryErrors  = StateErrors(), ///< The segment errors at the entry point
                      const StateErrors middleErrors = StateErrors(), ///< The segment errors at the mid point
                      const StateErrors exitErrors   = StateErrors()  ///< The segment errors at the exit point
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
        m_avPhotonEnergy   ( avPhotEn(rad) ),
        m_rotation         ( NULL          ),
        m_rotation2        ( NULL          )
    {
      if      ( RichTrackSegment::UseAllStateVectors == type() )
      {
        // nothing to do yet
      }
      else if ( RichTrackSegment::UseChordBetweenStates == type() )
      {
        chordConstructorInit3();
      }
      else
      {
        throw RichException( "Unknown RichTrackSegment::SegmentType" );
      }
    }

    // ------------------------------------------------------------------------------------------------------

  public:

    /// Standard constructor
    RichTrackSegment()
      : m_type             ( RichTrackSegment::UnDefined ),
        m_radIntersections ( 1 ),
        m_radiator         ( Rich::InvalidRadiator ),
        m_rich             ( Rich::InvalidDetector ),
        m_avPhotonEnergy   ( 0                     ),
        m_rotation         ( NULL                  ),
        m_rotation2        ( NULL                  ) { }

    /// Destructor
    ~RichTrackSegment( ) { cleanUpRotations(); }

    // ------------------------------------------------------------------------------------------------------

  public:

    /// Provides read-only access to the radiator intersections
    inline const RichRadIntersection::Vector & radIntersections() const
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
    void angleToDirection ( const Gaudi::XYZVector& direction,
                            double& theta,
                            double& phi ) const;

    /** Creates a vector at an given angle and azimuth to the track segment
     *
     *  @param theta The angle between this track segment and the created vector
     *  @param phi   The azimuthal angle of the vector to this track segment
     *
     *  @return The vector at the given theta and phi angles to this track segment
     */
    Gaudi::XYZVector vectorAtThetaPhi ( const double theta,
                                        const double phi ) const;

    /** Creates a vector at an given angle and azimuth to the track segment
     *
     *  @param cosTheta Cosine of the angle between this track segment and the created vector
     *  @param sinTheta Sine   of the angle between this track segment and the created vector
     *  @param cosPhi   Cosine of the azimuthal angle of the vector to this track segment
     *  @param sinPhi   Sine   of the azimuthal angle of the vector to this track segment
     *
     *  @return The vector at the given theta and phi angles to this track segment
     */
    Gaudi::XYZVector vectorAtCosSinThetaPhi ( const double cosTheta,
                                              const double sinTheta,
                                              const double cosPhi,
                                              const double sinPhi ) const;

    /** Calculates the path lenth of a track segment.
     *  @returns The total length of the track inside the radiator
     */
    inline double pathLength() const
    {
      return ( std::sqrt((entryPoint()-middlePoint()).mag2()) +
               std::sqrt((middlePoint()-exitPoint()).mag2())  );
    }

    /// Returns the segment entry point to the radiator
    inline const Gaudi::XYZPoint& entryPoint() const
    {
      return radIntersections().front().entryPoint();
    }

    /// Returns the segment mid-point in the radiator
    inline const Gaudi::XYZPoint& middlePoint() const
    {
      return m_middlePoint;
    }

    /// Returns the segment exit point from the radiator
    inline const Gaudi::XYZPoint& exitPoint() const
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
    Gaudi::XYZPoint bestPoint( const double fractDist ) const;

    /// Returns the best estimate of the average point in the radiator
    /// Equivalent to RichTrackSegment::bestPoint(0.5), but more efficient
    inline const Gaudi::XYZPoint& bestPoint() const
    {
      return m_middlePoint;
    }

    /// Returns the momentum vector at entry
    inline const Gaudi::XYZVector& entryMomentum() const
    {
      return radIntersections().front().entryMomentum();
    }

    /// Returns the momentum vector at the mid point
    inline const Gaudi::XYZVector& middleMomentum() const
    {
      return m_middleMomentum;
    }

    /// Returns the momentum vector at exit
    inline const Gaudi::XYZVector& exitMomentum() const
    {
      return radIntersections().back().exitMomentum();
    }

    /// Returns the best direction for segment at a given fractional distance along segment
    Gaudi::XYZVector bestMomentum( const double fractDist ) const;

    /// Returns the best estimate of the average momentum vector for the entire segment
    /// Equivalent to RichTrackSegment::bestMomentum(0.5) but more efficient
    inline const Gaudi::XYZVector& bestMomentum() const
    {
      return m_middleMomentum;
    }

    /// Returns the radiator type
    inline Rich::RadiatorType radiator() const
    {
      return m_radiator;
    }

    /// Returns the detector type
    inline Rich::DetectorType rich() const
    {
      return m_rich;
    }

    /// Set the entry state
    inline void setEntryState( const Gaudi::XYZPoint& point,
                               const Gaudi::XYZVector& dir )
    {
      radIntersections().front().setEntryPoint    ( point );
      radIntersections().front().setEntryMomentum ( dir   );
    }

    /// Set the Middle state
    inline void setMiddleState( const Gaudi::XYZPoint& point,
                                const Gaudi::XYZVector& dir )
    {
      m_middlePoint    = point;
      m_middleMomentum = dir;
    }

    /// Set the exit state
    inline void setExitState( const Gaudi::XYZPoint& point,
                              const Gaudi::XYZVector& dir )
    {
      radIntersections().back().setExitPoint    ( point );
      radIntersections().back().setExitMomentum ( dir   );
    }

    /// Set the radiator type
    inline void setRadiator(const Rich::RadiatorType rad)
    {
      m_radiator = rad;
    }

    /// Set the rich detector
    inline void setRich( const Rich::DetectorType det )
    {
      m_rich = det;
    }

    /// Returns the entry errors
    inline const StateErrors & entryErrors() const
    {
      return m_errorsEntry;
    }

    /// Returns the middle point errors
    inline const StateErrors & middleErrors() const
    {
      return m_errorsMiddle;
    }

    /// Returns the exit errors
    inline const StateErrors & exitErrors() const
    {
      return m_errorsExit;
    }

    /// Returns the average observable photon energy
    inline double avPhotonEnergy() const
    {
      return m_avPhotonEnergy;
    }

    /// Sets the average observable photon energy
    inline void setAvPhotonEnergy( const double energy )
    {
      m_avPhotonEnergy = energy;
    }

    /// Reset segment after information update
    void reset() const;

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
    inline RichRadIntersection::Vector & radIntersections()
    {
      return m_radIntersections;
    }

    /// Access the rotation matrix 1
    inline const Gaudi::Rotation3D & rotationMatrix() const
    {
      if ( !m_rotation ) computeRotationMatrix();
      return *m_rotation;
    }

    /// Access the rotation matrix 2
    inline const Gaudi::Rotation3D & rotationMatrix2() const
    {
      if ( !m_rotation2 ) computeRotationMatrix2();
      return *m_rotation2;
    }

    /// Compute the rotation matrix
    void computeRotationMatrix() const;

    /// Compute the rotation matrix
    void computeRotationMatrix2() const;

  private:  // private data

    /// The segment type
    SegmentType m_type;

    /// The raw intersections with the radiator volumes
    RichRadIntersection::Vector m_radIntersections;

    /// The middle point of the segment in the radiator volume
    Gaudi::XYZPoint m_middlePoint;

    /// The momentum vector at the segment middle point in the radiator volume
    Gaudi::XYZVector m_middleMomentum;

    Rich::RadiatorType m_radiator; ///< Rich radiator
    Rich::DetectorType m_rich;     ///< Rich detector

    StateErrors m_errorsEntry;     ///< Errors for the entry state
    StateErrors m_errorsMiddle;    ///< Errors for the middle state
    StateErrors m_errorsExit;      ///< Errors for the exit state

    /** The average observable photon energy for this segment
     *  @todo Quick fix. Need to review to if this can be done in a better way
     *        without the need for this variable.
     */
    double m_avPhotonEnergy;

    // Some variables for internal caching of information for speed

    /** Rotation matrix used to calculate the theta and phi angles between
     *  this track segment and a given direction.
     *  Created on demand as required.
     */
    mutable Gaudi::Rotation3D * m_rotation;

    /** Rotation matrix used to create vectors at a given theta and phi angle
     *  to this track segment.
     *  Created on demand as required
     */
    mutable Gaudi::Rotation3D * m_rotation2;

  };

} // end LHCb namespace

inline void LHCb::RichTrackSegment::computeRotationMatrix() const
{
  m_rotation = new Gaudi::Rotation3D( rotationMatrix2().Inverse() );
}

inline void
LHCb::RichTrackSegment::angleToDirection( const Gaudi::XYZVector & direction,
                                          double & theta,
                                          double & phi ) const
{
  // create vector in track reference frame
  const Gaudi::XYZVector rotDirection = rotationMatrix() * direction;
  // get the angles
  theta = rotDirection.theta();
  phi   = rotDirection.phi();
  // correct phi
  if ( phi < 0 ) phi += 2.0*M_PI;
}

inline Gaudi::XYZVector
LHCb::RichTrackSegment::vectorAtThetaPhi( const double theta,
                                          const double phi ) const
{
  const double sinTheta = std::sin(theta);
  return rotationMatrix2() * Gaudi::XYZVector( sinTheta*std::cos(phi),
                                               sinTheta*std::sin(phi),
                                               std::cos(theta) );
}

inline Gaudi::XYZVector
LHCb::RichTrackSegment::vectorAtCosSinThetaPhi ( const double cosTheta,
                                                 const double sinTheta,
                                                 const double cosPhi,
                                                 const double sinPhi ) const
{
  return rotationMatrix2() * Gaudi::XYZVector( sinTheta*cosPhi,
                                               sinTheta*sinPhi,
                                               cosTheta );
}

inline void LHCb::RichTrackSegment::reset() const
{
  cleanUpRotations();
}

#endif // RICHKERNEL_RichTrackSegment_H
