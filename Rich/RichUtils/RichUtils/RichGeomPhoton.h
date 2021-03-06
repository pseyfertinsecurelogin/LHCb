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
/** @file RichGeomPhoton.h
 *
 *  Header file for RICH utility class : LHCb::RichGeomPhoton
 *
 *  @author Antonis Papanestis
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2002-05-30
 */
//-----------------------------------------------------------------------------

#pragma once

// std include
#include <iostream>

// geometry
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// RichUtils
#include "RichUtils/RichPixelCluster.h"

// Kernel
#include "Kernel/MemPoolAlloc.h"

// Forward declarations
class DeRichSphMirror;

// General LHCb namespace
namespace LHCb {

  /** @class RichGeomPhoton RichGeomPhoton.h RichUtils/RichGeomPhoton.h
   *
   *  A geometrical representation of a reconstructed Cherenkov photon.
   *
   *  @author Antonis Papanestis
   *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
   *  @date   2002-05-30
   */

  class RichGeomPhoton final : public LHCb::MemPoolAlloc<RichGeomPhoton> {

  public:
    /// Container of photons
    using Vector = LHCb::STL::Vector<RichGeomPhoton>;

  public:
    /// Default Constructor
    RichGeomPhoton() = default;

  public:
    /** Constructor with parameters
     *
     *  @param theta Cherenkov angle theta
     *  @param phi   Cherenkov angle phi
     *  @param activeFrac The fraction of the associated segment that this photon could have been
     * radiated from
     */
    RichGeomPhoton( const float theta, const float phi, const float activeFrac )
        : m_CherenkovTheta( theta ), m_CherenkovPhi( phi ), m_activeSegmentFraction( activeFrac ) {}

    /** Constructor from full photon information
     *
     *  @param theta Cherenkov angle theta
     *  @param phi   Cherenkov angle phi
     *  @param emissionPoint The assumed photon emission point
     *  @param emissionDir   The photon direction at the emission point
     *  @param detectionPoint The photon detection point on the HPDs
     *  @param sphMirrReflPoint The reflection point on the spherical mirrors
     *  @param flatMirrReflPoint The reflection point on the flat mirrors
     *  @param smartID The RichSmartID channel cluster
     *  @param activeFrac The fraction of the associate segment that this photon could have been
     * radiated from
     */
    RichGeomPhoton( const float theta, const float phi, const Gaudi::XYZPoint& emissionPoint,
                    const Gaudi::XYZVector& emissionDir, const Gaudi::XYZPoint& detectionPoint,
                    const Gaudi::XYZPoint& sphMirrReflPoint, const Gaudi::XYZPoint& flatMirrReflPoint,
                    const LHCb::RichSmartID smartID = LHCb::RichSmartID(), const float activeFrac = 0 )
        : m_CherenkovTheta( theta )
        , m_CherenkovPhi( phi )
        , m_emissionPoint( emissionPoint )
        , m_emissionDir( emissionDir )
        , m_detectionPoint( detectionPoint )
        , m_sphMirReflectionPoint( sphMirrReflPoint )
        , m_flatMirReflectionPoint( flatMirrReflPoint )
        , m_smartID( smartID )
        , m_activeSegmentFraction( activeFrac ) {}

    /** Constructor from full photon information (without photon direction at emission).
     *
     *  @param theta Cherenkov angle theta
     *  @param phi   Cherenkov angle phi
     *  @param emissionPoint The assumed photon emission point
     *  @param detectionPoint The photon detection point on the HPDs
     *  @param sphMirrReflPoint The reflection point on the spherical mirrors
     *  @param flatMirrReflPoint The reflection point on the flat mirrors
     *  @param smartID The RichSmartID channel cluster
     *  @param activeFrac The fraction of the associate segment that this photon could have been
     * radiated from
     */
    RichGeomPhoton( const float theta, const float phi, const Gaudi::XYZPoint& emissionPoint,
                    const Gaudi::XYZPoint& detectionPoint, const Gaudi::XYZPoint& sphMirrReflPoint,
                    const Gaudi::XYZPoint& flatMirrReflPoint, const LHCb::RichSmartID smartID = LHCb::RichSmartID(),
                    const float activeFrac = 0 )
        : m_CherenkovTheta( theta )
        , m_CherenkovPhi( phi )
        , m_emissionPoint( emissionPoint )
        , m_detectionPoint( detectionPoint )
        , m_sphMirReflectionPoint( sphMirrReflPoint )
        , m_flatMirReflectionPoint( flatMirrReflPoint )
        , m_smartID( smartID )
        , m_activeSegmentFraction( activeFrac ) {}

  public:
    /**
     * Set accessor for the Cherenkov theta angle
     * @param theta the new value for the Cherenkov theta angle
     */
    inline void setCherenkovTheta( const float theta ) noexcept { m_CherenkovTheta = theta; }

    /**
     * Get accessor for the Cherenkov theta angle
     * @return the current value of the Cherenkov theta angle
     */
    inline float CherenkovTheta() const noexcept { return m_CherenkovTheta; }

    /**
     * Set accessor for Cherenkov phi angle
     * @param phi the new value for the Cherenkov phi angle
     */
    inline void setCherenkovPhi( const float phi ) noexcept { m_CherenkovPhi = phi; }

    /**
     * Get accessor for Cherenkov phi angle
     * @return the current value of the Cherenkov phi angle
     */
    inline float CherenkovPhi() const noexcept { return m_CherenkovPhi; }

    /**
     * Set accessor for the estimated emission point
     * @param emissionPoint the new value for the estimated emission point
     */
    inline void setEmissionPoint( const Gaudi::XYZPoint& emissionPoint ) noexcept { m_emissionPoint = emissionPoint; }

    /**
     * Const Get accessor for the estimated emission point
     * @return the current value of the estimated emission point
     */
    inline const Gaudi::XYZPoint& emissionPoint() const noexcept { return m_emissionPoint; }

    /**
     * Get accessor for the estimated emission point
     * @return the current value of the estimated emission point
     */
    inline Gaudi::XYZPoint& emissionPoint() noexcept { return m_emissionPoint; }

    /**
     * Set accessor for the estimated photon direction at the emission point
     * @param emissionDir the new value for the estimated emission direction
     */
    inline void setEmissionDir( const Gaudi::XYZVector& emissionDir ) noexcept { m_emissionDir = emissionDir; }

    /**
     * Const Get accessor for the estimated photon direction at the emission point
     * @return the current value of the estimated emission direction
     */
    inline const Gaudi::XYZVector& emissionDir() const noexcept { return m_emissionDir; }

    /**
     * Get accessor for the estimated photon direction at the emission point
     * @return the current value of the estimated emission direction
     */
    inline Gaudi::XYZVector& emissionDir() noexcept { return m_emissionDir; }

    /**
     * Set accessor for the detection point
     * @param detectionPoint the new value for the detection point
     */
    inline void setDetectionPoint( const Gaudi::XYZPoint& detectionPoint ) noexcept {
      m_detectionPoint = detectionPoint;
    }

    /**
     * Const Get accessor for the detection point
     * @return the current value of the detection point
     */
    inline const Gaudi::XYZPoint& detectionPoint() const noexcept { return m_detectionPoint; }

    /**
     * Get accessor for the detection point
     * @return the current value of the detection point
     */
    inline Gaudi::XYZPoint& detectionPoint() noexcept { return m_detectionPoint; }

    /// Set the unambiguous photon flag
    void setUnambiguousPhoton( const bool unambig ) noexcept { m_unambiguousPhoton = unambig; }

    /// Access the unambiguous photon flag
    bool unambiguousPhoton() const noexcept { return m_unambiguousPhoton; }

    /// Set the associated primary mirror detector element
    inline void setPrimaryMirror( const DeRichSphMirror* mirror ) noexcept { m_primaryMirror = mirror; }

    /// Access the associated primary mirror detector element
    inline const DeRichSphMirror* primaryMirror() const noexcept { return m_primaryMirror; }

    /// Set the associated secondary mirror detector element
    inline void setSecondaryMirror( const DeRichSphMirror* mirror ) noexcept { m_secondaryMirror = mirror; }

    /// Access the associated secondary mirror detector element
    inline const DeRichSphMirror* secondaryMirror() const noexcept { return m_secondaryMirror; }

    /**
     * Set accessor for the RichSmartID
     * @param id The new RichSmartID
     */
    inline void setSmartID( const LHCb::RichSmartID& id ) noexcept { m_smartID = id; }

    /**
     * Const Get accessor to RichSmartID
     * @return the current value of RichSmartID
     */
    inline const LHCb::RichSmartID& smartID() const noexcept { return m_smartID; }

    /**
     * Set accessor for the spherical mirror reflection point
     * @param sphMirReflectionPoint the new value for the spherical mirror reflection point
     */
    inline void setSphMirReflectionPoint( const Gaudi::XYZPoint& sphMirReflectionPoint ) noexcept {
      m_sphMirReflectionPoint = sphMirReflectionPoint;
    }

    /**
     * Const Get accessor for the spherical mirror reflection point
     * @return the current value of the spherical mirror reflection point
     */
    inline const Gaudi::XYZPoint& sphMirReflectionPoint() const noexcept { return m_sphMirReflectionPoint; }

    /**
     * Get accessor for the spherical mirror reflection point
     * @return the current value of the spherical mirror reflection point
     */
    inline Gaudi::XYZPoint& sphMirReflectionPoint() noexcept { return m_sphMirReflectionPoint; }

    /**
     * Set accessor for the flat mirror reflection point
     * @param flatMirReflectionPoint the new value for the flat mirror reflection point
     */
    inline void setFlatMirReflectionPoint( const Gaudi::XYZPoint& flatMirReflectionPoint ) noexcept {
      m_flatMirReflectionPoint = flatMirReflectionPoint;
    }

    /**
     * Const Get accessor for the flat mirror reflection point
     * @return the current value of the flat mirror reflection point
     */
    inline const Gaudi::XYZPoint& flatMirReflectionPoint() const noexcept { return m_flatMirReflectionPoint; }

    /**
     * Get accessor for the flat mirror reflection point
     * @return the current value of the flat mirror reflection point
     */
    inline Gaudi::XYZPoint& flatMirReflectionPoint() noexcept { return m_flatMirReflectionPoint; }

    /**
     * Set accessor for the current active segment fraction.
     * The fraction of the RichTrackSegment trajectory this photon is associated
     * with for which it is geometrically possible this photon was produced
     *
     * @param fraction the new value for the active fraction
     */
    inline void setActiveSegmentFraction( const float fraction ) noexcept { m_activeSegmentFraction = fraction; }

    /**
     * Get accessor to the current active segment fraction.
     * The fraction of the RichTrackSegment trajectory this photon is associated
     * with for which it is geometrically possible this photon was produced
     *
     * @return the current value of the current active segment fraction.
     */
    inline float activeSegmentFraction() const noexcept { return m_activeSegmentFraction; }

  public:
    /// Printout method
    std::ostream& fillStream( std::ostream& s ) const;

    /// Implement ostream << method for RichGeomPhoton
    friend inline std::ostream& operator<<( std::ostream& s, const LHCb::RichGeomPhoton& photon ) {
      return photon.fillStream( s );
    }

  private:                                      // data
    float             m_CherenkovTheta{0};      ///< Cherenkov angle theta
    float             m_CherenkovPhi{0};        ///< Cherenkov angle phi
    Gaudi::XYZPoint   m_emissionPoint;          ///< The photon emission point
    Gaudi::XYZVector  m_emissionDir;            ///< The photon direction at the emission point
    Gaudi::XYZPoint   m_detectionPoint;         ///< The photon detection point on the HPD entrance window
    Gaudi::XYZPoint   m_sphMirReflectionPoint;  ///< The primary mirror reflection point
    Gaudi::XYZPoint   m_flatMirReflectionPoint; ///< The secondary mirror reflection point
    LHCb::RichSmartID m_smartID;                ///< The channel ID for the photon detection point

    /// Pointer to the associated primary mirror detector element
    const DeRichSphMirror* m_primaryMirror = nullptr;

    /// Pointer to the associated secondary mirror detector element
    const DeRichSphMirror* m_secondaryMirror = nullptr;

    /// Flag to indicate if an unambiguous photon or not
    bool m_unambiguousPhoton = false;

    /** The fraction of the RichTrackSegment trajectory this photon is associated
     *  with for which it is geometrically possible this photon was produced
     */
    float m_activeSegmentFraction{0};
  };

} // namespace LHCb
