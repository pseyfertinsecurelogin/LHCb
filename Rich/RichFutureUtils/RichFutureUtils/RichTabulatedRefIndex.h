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
#include <memory>

// RichDet
#include "RichDet/DeRich1.h"
#include "RichDet/DeRich2.h"
#include "RichDet/DeRichRadiator.h"
#include "RichDet/Rich1DTabProperty.h"

// Kernel
#include "Kernel/RichParticleIDType.h"
#include "Kernel/RichRadiatorType.h"

// Utils
#include "RichUtils/RichTrackSegment.h"

// Det Desc
#include "DetDesc/ConditionKey.h"
#include "DetDesc/IConditionDerivationMgr.h"

namespace Rich::Utils {

  //-----------------------------------------------------------------------------
  /** @class TabulatedRefIndex RichTabulatedRefIndex.h
   *
   *  Tool to calculate the effective refractive index for
   *  a given radiator. An implementation that uses the tabulated
   *  information from the XML.
   *
   *  @author Chris Jones
   *  @date   2017-01-19
   */
  //-----------------------------------------------------------------------------

  class TabulatedRefIndex final {

  public:
    /// Disallow default constructor
    TabulatedRefIndex() = delete;

    /// Constructor from detector info
    TabulatedRefIndex( const DeRich1&              rich1,     //
                       const DeRich2&              rich2,     //
                       const DeRichRadiator&       r1gas,     //
                       const DeRichRadiator&       r2gas,     //
                       const RadiatorArray<float>& minPhotEn, //
                       const RadiatorArray<float>& maxPhotEn, //
                       const ParticleArray<float>& masses )
        : m_riches{&rich1, &rich2}
        , m_radiators{nullptr, &r1gas, &r2gas}
        , m_minPhotEn( minPhotEn )
        , m_maxPhotEn( maxPhotEn )
        , m_particleMass( masses ) {}

  public:
    // access methods

    // Calculates the refractive index for a given radiator type at a
    // given energy
    float refractiveIndex( const Rich::RadiatorType rad, //
                           const float              energy ) const {
      return deRad( rad )->refractiveIndex( energy, m_hltMode );
    }

    // Calculates the average refractive index for a given radiator type
    // for a given range of photon energies.
    float refractiveIndex( const Rich::RadiatorType rad,       //
                           const float              energyBot, //
                           const float              energyTop ) const {
      const auto rich = ( rad == Rich::Rich2Gas ? Rich::Rich2 : Rich::Rich1 );
      return refractiveIndex( rad,
                              m_riches[rich]->nominalPDQuantumEff()->meanX( energyBot, energyTop ) / Gaudi::Units::eV );
    }

    // Calculates the average refractive index for a given radiator type
    // for a all visable photon energies.
    float refractiveIndex( const Rich::RadiatorType rad ) const {
      return refractiveIndex( rad, meanPhotonEnergy( rad ) );
    }

    // Calculates the average refractive index for a given set of radiator intersections
    // for all visable photon energies.
    float refractiveIndex( const Rich::RadIntersection::Vector& intersections, const float energy ) const;

    // Calculates the average refractive index for a given set of radiator intersections
    // for all visable photon energies
    float refractiveIndex( const Rich::RadIntersection::Vector& intersections ) const;

    // Calculates the refractive index R.M.S. for a given set of radiator intersections
    // for all visable photon energies.
    float refractiveIndexRMS( const Rich::RadIntersection::Vector& intersections ) const;

    // Calculates the refractive index S.D. for a given set of radiator intersections
    // for all visable photon energies.
    float refractiveIndexSD( const Rich::RadIntersection::Vector& intersections ) const;

    // Returns the threshold momentum for a given hypothesis in a given radiator
    float thresholdMomentum( const Rich::ParticleIDType id, const Rich::RadiatorType rad ) const;

    // Calculates the threshold momentum for a given mass hypothesis
    float thresholdMomentum( const Rich::ParticleIDType id, const LHCb::RichTrackSegment& trSeg ) const;

  private:
    // methods

    /// get the average mean photon energy for given radiator
    inline float meanPhotonEnergy( const Rich::RadiatorType rad ) const noexcept {
      return 0.5f * ( m_maxPhotEn[rad] + m_minPhotEn[rad] );
    }

    /// Access on demand the Rich radiator detector elements
    inline const DeRichRadiator* deRad( const Rich::RadiatorType rad ) const { return m_radiators[rad]; }

  private:
    // data

    /// Pointers to RICHes
    Rich::DetectorArray<const DeRich*> m_riches = {{}};

    /// Pointers to RICH radiator detector elements
    Rich::RadiatorArray<const DeRichRadiator*> m_radiators = {{}};

    /// The minimum photon energies
    RadiatorArray<float> m_minPhotEn{1.75, 1.75, 1.75};

    /// The maximum photon energies
    RadiatorArray<float> m_maxPhotEn{4.0, 7.0, 7.0};

    /// Array containing particle masses
    ParticleArray<float> m_particleMass = {{}};

    /// Flag to say if we are in HLT mode or not ...
    /// Need to decide what to do with this ...
    const bool m_hltMode{true};

  public:
    /// Creates a condition derivation for the given key
    template <typename PARENT>
    static inline LHCb::DetDesc::IConditionDerivationMgr::DerivationId //
    addConditionDerivation( PARENT*                     parent,        //
                            LHCb::DetDesc::ConditionKey key ) {
      if ( parent->msgLevel( MSG::DEBUG ) ) {
        parent->debug() << "TabulatedRefIndex::addConditionDerivation : Key=" << key << endmsg;
      }
      using SA = std::array<std::string, 4>;
      return LHCb::DetDesc:: //
          addConditionDerivation( parent->conditionDerivationMgr(),
                                  SA{DeRichLocations::Rich1,                               // inputs
                                     DeRichLocations::Rich2,                               //
                                     DeRichLocations::Rich1Gas,                            //
                                     DeRichLocations::Rich2Gas},                           //
                                  std::move( key ),                                        // output
                                  [minPhotEn = parent->richPartProps()->minPhotonEnergy(), //
                                   maxPhotEn = parent->richPartProps()->maxPhotonEnergy(), //
                                   masses    = parent->richPartProps()->masses()]             //
                                  ( const DeRich1&        rich1,                           //
                                    const DeRich2&        rich2,                           //
                                    const DeRichRadiator& r1gas,                           //
                                    const DeRichRadiator& r2gas ) {
                                    return Utils::TabulatedRefIndex{rich1,     rich2,     r1gas, r2gas,
                                                                    minPhotEn, maxPhotEn, masses};
                                  } );
    }

    /// Default conditions name
    static constexpr const char* DefaultConditionKey = "RichRefIndex-Handler";
  };

} // namespace Rich::Utils
