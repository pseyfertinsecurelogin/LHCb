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
/** @file RichSIMDGeomPhoton.h
 *
 *  Header file for RICH utility class : Rich::Future::SIMDRecoPhoton
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2017-02-06
 */
//-----------------------------------------------------------------------------

#pragma once

// std include
#include <array>
#include <cmath>
#include <iostream>
#include <vector>

// Utils
#include "RichFutureUtils/RichGeomPhoton.h"
#include "RichUtils/RichSIMDTypes.h"

// geometry
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// Kernel
#include "Kernel/RichSmartID.h"

// Vc
#include <Vc/common/alignedbase.h>

// Forward declarations
class DeRichSphMirror;
class DeRichPD;

namespace Rich::SIMD::Future {

  /** @class RecoPhoton RichFutureUtils/RichSIMDGeomPhoton.h
   *
   *  Basic SIMD representation of a Cherenkov photon
   *
   *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
   *  @date   2017-10-13
   */
  class RecoPhoton : public Vc::AlignedBase<Vc::VectorAlignment> {

  public:
    // definitions

    /// The scalar floating point precision
    using FP = Rich::SIMD::DefaultScalarFP;
    /// The SIMD floating point type
    using SIMDFP = Rich::SIMD::FP<FP>;
    /// Mask type
    using MASK = SIMDFP::mask_type;
    /// Type for SmartIDs container.
    using SmartIDs = Rich::SIMD::STDArray<LHCb::RichSmartID, SIMDFP>;
    /// Container of photons.
    using Vector = Rich::SIMD::STDVector<RecoPhoton>;
    /// Type for scalar photon
    using ScalarPhoton = Rich::Future::RecoPhoton;

  public:
    // constructors

    /// Default Constructor
    RecoPhoton() = default;

    /** Constructor from RICH and SmartIDs
     *
     *  @param rich      The RICH detector
     *  @param smartID   The RCH PD channel identifiers associated to the photons
     *  @param validMask Starting validity mask
     */
    RecoPhoton( const Rich::DetectorType rich, //
                const SmartIDs&          smartIDs )
        : m_smartIDs( smartIDs ), m_rich( rich ) {}

    /** Constructor from full data
     *
     *  @param rich         The RICH detector
     *  @param theta        Cherenkov angles theta
     *  @param phi          Cherenkov angles phi
     *  @param smartID      The RICH PD channel identifiers associated to the photons
     *  @param activeFrac   Fraction of the track segment that these photons could have been radiated from
     *  @param unambigPhots Unambiguous photon mask
     *  @param validMask    Photon validity mask
     */
    RecoPhoton( const Rich::DetectorType rich,         //
                const SIMDFP&            theta,        //
                const SIMDFP&            phi,          //
                const SmartIDs&          smartIDs,     //
                const SIMDFP&            activeFrac,   //
                const MASK&              unambigPhots, //
                const MASK&              validMask )
        : m_ckTheta( theta )
        , m_ckPhi( phi )
        , m_smartIDs( smartIDs )
        , m_activeFraction( activeFrac )
        , m_unambigPhot( unambigPhots )
        , m_valid( validMask )
        , m_rich( rich ) {}

  public:
    // Setters

    /** Set accessor for the Cherenkov theta angle
     *  @param theta the new value for the Cherenkov theta angle */
    void setCherenkovTheta( const SIMDFP& theta ) noexcept { m_ckTheta = theta; }

    /** Set accessor for Cherenkov phi angle
     *  @param phi the new value for the Cherenkov phi angle */
    void setCherenkovPhi( const SIMDFP& phi ) noexcept { m_ckPhi = phi; }

    /** Set accessor for the current active segment fraction.
     *  The fraction of the RichTrackSegment trajectory this photon is associated
     *  with for which it is geometrically possible this photon was produced. */
    void setActiveSegmentFraction( const SIMDFP& fraction ) noexcept { m_activeFraction = fraction; }

    /** Set accessor for the RichSmartID
     *  @param id The new RichSmartID */
    void setSmartID( const SmartIDs& id ) noexcept { m_smartIDs = id; }

    /// Set the unambiguous photon mask
    void setUnambiguousPhoton( const MASK& unambig ) noexcept { m_unambigPhot = unambig; }

    /// Set the photon validity mask
    void setValidityMask( const MASK& valid ) noexcept { m_valid = valid; }

  public:
    // getters

    /** Get accessor for the Cherenkov theta angle
     *  @return the current value of the Cherenkov theta angle */
    [[nodiscard]] const SIMDFP& CherenkovTheta() const noexcept { return m_ckTheta; }

    /** Get accessor for Cherenkov phi angle
     *  @return the current value of the Cherenkov phi angle */
    [[nodiscard]] const SIMDFP& CherenkovPhi() const noexcept { return m_ckPhi; }

    /** Get accessor to the current active segment fraction.
     *  The fraction of the RichTrackSegment trajectory this photon is associated
     *  with for which it is geometrically possible this photon was produced.
     *  @return the current value of the current active segment fraction. */
    [[nodiscard]] const SIMDFP& activeSegmentFraction() const noexcept { return m_activeFraction; }

    /** Get accessor to RichSmartID
     *  @return the current value of RichSmartID */
    [[nodiscard]] const SmartIDs& smartID() const noexcept { return m_smartIDs; }

    /// Access the unambiguous photon mask
    [[nodiscard]] const MASK& unambiguousPhoton() const noexcept { return m_unambigPhot; }

    /// Access the unambiguous photon flag
    [[nodiscard]] const MASK& validityMask() const noexcept { return m_valid; }

    /// Access the RICH
    [[nodiscard]] Rich::DetectorType rich() const noexcept { return m_rich; }

  public:
    // scalar helpers

    /// Create a scalar photon object for the given SIMD entry
    [[nodiscard]] decltype( auto ) scalarPhoton( const std::size_t simdEntry ) const noexcept {
      // if SIMD entry is valid, return fully constructed object, otherwise defaults
      return ( simdEntry < SIMDFP::Size && validityMask()[simdEntry]  //
                   ? ScalarPhoton{CherenkovTheta()[simdEntry],        //
                                  CherenkovPhi()[simdEntry],          //
                                  smartID()[simdEntry],               //
                                  activeSegmentFraction()[simdEntry], //
                                  unambiguousPhoton()[simdEntry]}     //
                   : ScalarPhoton{} );
    }

    /// Create a vector of valid scalar photons
    [[nodiscard]] Rich::Future::RecoPhoton::Vector scalarPhotons() const;

  protected:
    // messaging

    /// Printout method
    std::ostream& fillStream( std::ostream& s ) const;

  public:
    // messaging

    /// Implement ostream << method for GeomPhoton
    friend std::ostream& operator<<( std::ostream& s, const RecoPhoton& photon ) { return photon.fillStream( s ); }

  private:
    // data

    /// Cherenkov angles theta
    SIMDFP m_ckTheta{SIMDFP::Zero()};

    /// Cherenkov angles phi
    SIMDFP m_ckPhi{SIMDFP::Zero()};

    /// The channel IDs for the photon detection points
    SmartIDs m_smartIDs;

    /** The fraction of the RichTrackSegment trajectory these photons are associated
     *  with for which it is geometrically possible these photon were produced */
    SIMDFP m_activeFraction{SIMDFP::One()};

    /// Flag to indicate if unambiguous photons or not
    MASK m_unambigPhot{MASK( false )};

    /// Validity mask
    MASK m_valid{MASK( false )};

    /// RICH
    Rich::DetectorType m_rich{Rich::InvalidDetector};
  };

} // namespace Rich::SIMD::Future
