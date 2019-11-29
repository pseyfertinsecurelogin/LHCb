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
/** @file IRichParticleProperties.h
 *
 *  Header file for RICH reconstruction tool interface : Rich::IParticleProperties
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   15/03/2002
 */
//-----------------------------------------------------------------------------

#pragma once

// Kernel
#include "GaudiKernel/IAlgTool.h"
#include "Kernel/RichParticleIDType.h"
#include "Kernel/RichRadiatorType.h"

// Event Model
namespace LHCb {
  class RichTrackSegment;
}

namespace Rich {

  /** @class IParticleProperties IRichParticleProperties.h
   *
   *  Interface for tool to calculate various physical properties
   *  for the different mass hypotheses.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   15/03/2002
   */

  class IParticleProperties : public virtual IAlgTool {

  public:
    /// Interface ID
    DeclareInterfaceID( IParticleProperties, 1, 0 );

  public:
    /// Scalar type to use
    using ScType = float;

  public:
    /** Calculates 'beta' for given particle hypothesis and track momentum
     *
     *  @param ptot    Track segment momentum
     *  @param id      The mass hypothesis to assume whilst calculating beta
     *
     *  @return The value of beta
     */
    virtual ScType beta( const ScType ptot, const Rich::ParticleIDType id ) const = 0;

    /// Access the array of all particle masses
    virtual const Rich::ParticleArray<ScType>& masses() const = 0;

    /** Calculates the nominal mass for a given particle type
     *
     *  @param id  The mass hypothesis to assume whilst calculating beta
     *
     *  @return the nominal mass for the given particle hypothesis
     */
    virtual ScType mass( const Rich::ParticleIDType id ) const = 0;

    /** calculates the nominal mass squared for a given particle hypothesis
     *
     *  @param id  The mass hypothesis to assume whilst calculating beta
     *
     *  @return the nominal mass squared for the given particle hypothesis
     */
    virtual ScType massSq( const Rich::ParticleIDType id ) const = 0;

    /// Vector of the mass hypotheses to be considered
    virtual const Rich::Particles& particleTypes() const = 0;

    /// Access the minimum cherenkov photon energies (Aero/R1Gas/R2Gas)
    virtual const RadiatorArray<float>& minPhotonEnergy() const = 0;

    /// Access the minimum cherenkov photon energies (Aero/R1Gas/R2Gas)
    virtual const RadiatorArray<float>& maxPhotonEnergy() const = 0;

    /// Access the minimum cherenkov photon energy for given radiator
    virtual float minPhotonEnergy( const Rich::RadiatorType rad ) const = 0;

    /// Access the minimum cherenkov photon energy for given radiator
    virtual float maxPhotonEnergy( const Rich::RadiatorType rad ) const = 0;

    /// Access the mean cherenkov photon energy for given radiator
    virtual float meanPhotonEnergy( const Rich::RadiatorType rad ) const = 0;
  };

} // namespace Rich
