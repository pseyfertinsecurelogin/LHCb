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

//--------------------------------------------------------------------------
/** @file RichParticleIDType.h
 *
 *  Header file for RICH particle ID enumeration : RichParticleIDType
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   08/07/2004
 */
//--------------------------------------------------------------------------

#pragma once

// std include
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// General namespace for RICH specific definitions documented in RichSide.h
namespace Rich {

  /// Number of RICH particle types (Electron, Muon, Pion, Kaon, Proton, Deuteron
  /// and 'Below Threshold')
  inline constexpr std::uint16_t NParticleTypes = 7;

  /// Number of 'real' RICH particle types (Electron, Muon, Pion, Kaon, Proton and Deuteron)
  inline constexpr std::uint16_t NRealParticleTypes = 6;

  //--------------------------------------------------------------------------
  /** @enum ParticleIDType
   *  Enumeration for the RICH particle ID mass hypotheses.
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   08/07/2004
   */
  //--------------------------------------------------------------------------
  enum ParticleIDType : std::int8_t {
    Unknown = -1,  ///< Unknown particle type
    Electron,      ///< Represents e+ or e-
    Muon,          ///< Represents mu+ or mu-
    Pion,          ///< Represents pi+ or pi-
    Kaon,          ///< Represents K+ or K-
    Proton,        ///< Represents Pr+ or Pr-
    Deuteron,      ///< Represents d+ or d-
    BelowThreshold ///< Particle type is below threshold
  };

  /// Text conversion for Rich::ParticleIDType enumeration
  std::string text( const Rich::ParticleIDType particle );

  /// Type for container of particle types
  using Particles = std::vector<ParticleIDType>;

  /// Access a vector of all valid particle ID types
  const Particles& particles() noexcept;

  /// Implement textual ostream << method for Rich::ParticleIDType enumeration
  inline std::ostream& operator<<( std::ostream& s, const Rich::ParticleIDType particle ) {
    return s << Rich::text( particle );
  }

  /// Type for fixed size arrays with mass hypothesis information
  template <typename TYPE>
  using ParticleArray = std::array<TYPE, NParticleTypes>;

} // namespace Rich
