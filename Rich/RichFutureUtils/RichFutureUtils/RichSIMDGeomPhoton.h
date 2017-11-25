
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
#include <vector>

// Utils
#include "RichUtils/RichSIMDTypes.h"
#include "RichFutureUtils/RichGeomPhoton.h"

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

namespace Rich
{
  namespace SIMD
  {
    namespace Future
    {

      /** @class RecoPhoton RichFutureUtils/RichSIMDGeomPhoton.h
       *
       *  Basic SIMD representation of a Cherenkov photon
       *
       *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
       *  @date   2017-10-13
       */
      class RecoPhoton : public Vc::AlignedBase<Vc::VectorAlignment>
      {
        
      public:
        
        /// The scalar floating point precision
        using FP        = Rich::SIMD::DefaultScalarFP;
        /// The SIMD floating point type
        using SIMDFP    = Rich::SIMD::FP<FP>; 
        /// Type for SmartIDs container.
        using SmartIDs  = Rich::SIMD::STDArray<LHCb::RichSmartID,SIMDFP>;
        /// Container of photons.
        using Vector    = Rich::SIMD::STDVector<RecoPhoton>;
        
      public:
        
        /// Default Constructor
        RecoPhoton() = default;
        
      public:

        /** Constructor from RICH and SmartIDs
         *
         *  @param rich  The RICH detector
         *  @param smartID The RCH PD channel identifiers associated to the photons
         */
        RecoPhoton( const Rich::DetectorType    rich,
                    const SmartIDs&          smartID ) : 
          m_smartID( smartID ),
          m_rich   ( rich    ) { }
        
        /** Constructor with full parameters
         *
         *  @param rich  The RICH detector
         *  @param theta Cherenkov angles theta
         *  @param phi   Cherenkov angles phi
         *  @param smartID The RCH PD channel identifiers associated to the photons
         *  @param activeFrac The fraction of the associated segment that these photons could have been radiated from
         */
        RecoPhoton( const Rich::DetectorType   rich,
                    const SIMDFP&             theta,
                    const SIMDFP&               phi,
                    const SmartIDs& smartID  = SmartIDs(),
                    const SIMDFP& activeFrac = SIMDFP::One() ) : 
          m_CherenkovTheta        ( theta      ),
          m_CherenkovPhi          ( phi        ),
          m_smartID               ( smartID    ),
          m_activeSegmentFraction ( activeFrac ),
          m_rich                  ( rich       ) { }
        
      public:
        
        /** Set accessor for the Cherenkov theta angle
         *  @param theta the new value for the Cherenkov theta angle */
        inline void setCherenkovTheta ( const SIMDFP& theta ) noexcept { m_CherenkovTheta = theta; }
        
        /** Get accessor for the Cherenkov theta angle
         *  @return the current value of the Cherenkov theta angle */
        inline const SIMDFP& CherenkovTheta () const noexcept { return m_CherenkovTheta; }
        
        /** Set accessor for Cherenkov phi angle
         *  @param phi the new value for the Cherenkov phi angle */
        inline void setCherenkovPhi ( const SIMDFP& phi ) noexcept { m_CherenkovPhi = phi; }
        
        /** Get accessor for Cherenkov phi angle
         *  @return the current value of the Cherenkov phi angle */
        inline const SIMDFP& CherenkovPhi () const noexcept { return m_CherenkovPhi; }
        
        /**
         * Set accessor for the current active segment fraction.
         * The fraction of the RichTrackSegment trajectory this photon is associated
         * with for which it is geometrically possible this photon was produced
         *
         * @param fraction the new value for the active fraction
         */
        inline void setActiveSegmentFraction ( const SIMDFP& fraction ) noexcept
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
        inline const SIMDFP& activeSegmentFraction() const noexcept
        {
          return m_activeSegmentFraction;
        }
        
        /** Set accessor for the RichSmartID
         *  @param id The new RichSmartID */
        inline void setSmartID ( const SmartIDs& id ) noexcept { m_smartID = id; }
        
        /** Get accessor to RichSmartID
         *  @return the current value of RichSmartID */
        inline const SmartIDs& smartID() const noexcept { return m_smartID; }
        
        /// Set the unambiguous photon mask
        inline void setUnambiguousPhoton( const SIMDFP::mask_type& unambig ) noexcept 
        {
          m_unambigPhot = unambig;
        }
        
        /// Access the unambiguous photon mask
        inline const SIMDFP::mask_type& unambiguousPhoton() const noexcept { return m_unambigPhot; }
        
        /// Set the photon validity mask
        inline void setValidityMask( const SIMDFP::mask_type& valid ) noexcept 
        {
          m_valid = valid;
        }
        
        /// Access the unambiguous photon flag
        inline const SIMDFP::mask_type& validityMask() const noexcept { return m_valid; }

        /// Access the RICH
        inline Rich::DetectorType rich() const noexcept { return m_rich; }

      public:

        /// Create a scalar photon object for the given SIMD entry
        inline Rich::Future::RecoPhoton 
        scalarPhoton( const std::size_t simdEntry ) const noexcept
        {
          return { CherenkovTheta()[simdEntry],
                   CherenkovPhi()[simdEntry],
                   smartID()[simdEntry],
                   activeSegmentFraction()[simdEntry] };
        }

        /// Create a vector of valid scalar photons
        Rich::Future::RecoPhoton::Vector scalarPhotons() const;

      protected:
        
        /// Printout method
        std::ostream & fillStream ( std::ostream& s ) const;
        
      public:
        
        /// Implement ostream << method for GeomPhoton
        friend inline std::ostream& operator << ( std::ostream& s,
                                                  const RecoPhoton& photon )
        {
          return photon.fillStream(s);
        }
        
      private:
        
        /// Cherenkov angles theta
        SIMDFP m_CherenkovTheta { SIMDFP::Zero() };
        
        /// Cherenkov angles phi
        SIMDFP m_CherenkovPhi { SIMDFP::Zero() };  
        
        /// The channel IDs for the photon detection points
        SmartIDs m_smartID; 
        
        /** The fraction of the RichTrackSegment trajectory these photons are associated
         *  with for which it is geometrically possible these photon were produced */
        SIMDFP m_activeSegmentFraction { SIMDFP::One() };
        
        /// Flag to indicate if unambiguous photons or not
        SIMDFP::mask_type m_unambigPhot { false };
        
        /// Validity mask
        SIMDFP::mask_type m_valid { false };

        /// RICH
        Rich::DetectorType m_rich { Rich::InvalidDetector };
        
      };
      
    }
  }
}

