
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
#include <iostream>
#include <array>
#include <cmath>

// geometry
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// Kernel
#include "Kernel/RichSmartID.h"
#include "Kernel/FastAllocVector.h"

// Vc
#include <Vc/Vc>

// Forward declarations
class DeRichSphMirror;
class DeRichPD;

namespace Rich
{
  namespace Future
  {

    /** @class SIMDRecoPhoton RichFutureUtils/RichSIMDGeomPhoton.h
     *
     *  Basic SIMD representation of a Cherenkov photon
     *
     *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
     *  @date   2017-10-13
     */
    class SIMDRecoPhoton 
    {

    public:

      /// The floating point precision to use
      using Scalar = Vc::Vector<float>;

      /// Type for SmartIDs.
      using SmartIDs = std::array<LHCb::RichSmartID,Scalar::Size>;

      /// Container of photons
      using Vector = LHCb::STL::Vector<SIMDRecoPhoton>;

    public:

      /// Default Constructor
      SIMDRecoPhoton() = default;

    public:

      /** Constructor with parameters
       *
       *  @param theta Cherenkov angles theta
       *  @param phi   Cherenkov angles phi
       *  @param smartID The RCH PD channel identifiers associated to the photons
       *  @param activeFrac The fraction of the associated segment that these photons could have been radiated from
       */
      SIMDRecoPhoton( const Scalar theta,
                      const Scalar phi,
                      const SmartIDs smartID = SmartIDs(),
                      const Scalar activeFrac = Scalar(1) ) : 
        m_CherenkovTheta        ( theta      ),
        m_CherenkovPhi          ( phi        ),
        m_smartID               ( smartID    ),
        m_activeSegmentFraction ( activeFrac ) { }

    public:

      /** Set accessor for the Cherenkov theta angle
       *  @param theta the new value for the Cherenkov theta angle */
      inline void setCherenkovTheta ( const Scalar theta ) noexcept { m_CherenkovTheta = theta; }

      /** Get accessor for the Cherenkov theta angle
       *  @return the current value of the Cherenkov theta angle */
      inline Scalar CherenkovTheta () const noexcept { return m_CherenkovTheta; }

      /** Set accessor for Cherenkov phi angle
       *  @param phi the new value for the Cherenkov phi angle */
      inline void setCherenkovPhi ( const Scalar phi ) noexcept { m_CherenkovPhi = phi; }

      /** Get accessor for Cherenkov phi angle
       *  @return the current value of the Cherenkov phi angle */
      inline Scalar CherenkovPhi () const noexcept { return m_CherenkovPhi; }

      /**
       * Set accessor for the current active segment fraction.
       * The fraction of the RichTrackSegment trajectory this photon is associated
       * with for which it is geometrically possible this photon was produced
       *
       * @param fraction the new value for the active fraction
       */
      inline void setActiveSegmentFraction ( const Scalar fraction ) noexcept
      {
        m_activeSegmentFraction = fraction;
      }

      /**
       * Get accessor to the current active segment fraction.
       * The fraction of the RichTrackSegment trajectory this photon is associated
       * with for which it is geometrically possible this photon was produced
       *
       * @return the current value of the current active segment fraction.
       */
      inline Scalar activeSegmentFraction() const noexcept
      {
        return m_activeSegmentFraction;
      }

      /** Set accessor for the RichSmartID
       *  @param id The new RichSmartID */
      inline void setSmartID ( const SmartIDs id ) noexcept { m_smartID = id; }

      /** Get accessor to RichSmartID
       *  @return the current value of RichSmartID */
      inline const SmartIDs& smartID() const noexcept { return m_smartID; }

      /// Set the unambiguous photon flag
      inline void setUnambiguousPhoton( const Scalar::mask_type unambig ) noexcept 
      {
        m_unambigPhot = unambig;
      }

      /// Access the unambiguous photon flag
      inline Scalar::mask_type unambiguousPhoton() const noexcept { return m_unambigPhot; }

    private:

      /// Cherenkov angles theta
      Scalar m_CherenkovTheta;

      /// Cherenkov angles phi
      Scalar m_CherenkovPhi;     

      /// The channel IDs for the photon detection points
      SmartIDs m_smartID; 

      /** The fraction of the RichTrackSegment trajectory these photons are associated
       *  with for which it is geometrically possible these photon were produced */
      Scalar m_activeSegmentFraction;

      /// Flag to indicate if unambiguous photons or not
      Scalar::mask_type m_unambigPhot;

    };

  }
}
