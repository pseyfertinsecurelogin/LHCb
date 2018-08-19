
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
namespace LHCb
{
  class RichTrackSegment;
}

namespace Rich
{

  /** @class IParticleProperties IRichParticleProperties.h
   *
   *  Interface for tool to calculate various physical properties
   *  for the different mass hypotheses.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   15/03/2002
   */

  class IParticleProperties : public virtual IAlgTool
  {

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

    /** Calculates the threshold momentum for a given mass hypothesis
     *  in a given radiator
     *
     *  @param id The mass hypothesis
     *  @param rad The radiator medium
     *
     *  @return The momentum threshold value, below which no Cherenkov radiation
     *          can be emitted
     */
    virtual ScType thresholdMomentum( const Rich::ParticleIDType id,
                                      const Rich::RadiatorType   rad ) const = 0;

    /** Calculates the threshold momentum for a given mass hypothesis
     *  for the given RichTrackSegment
     *
     *  @param id    The mass hypothesis
     *  @param trSeg The track segment
     *
     *  @return The momentum threshold value, below which no Cherenkov radiation
     *          can be emitted
     */
    virtual ScType thresholdMomentum( const Rich::ParticleIDType    id,
                                      const LHCb::RichTrackSegment &trSeg ) const = 0;

    /// Vector of the mass hypotheses to be considered
    virtual const Rich::Particles &particleTypes() const = 0;
  };

} // namespace Rich
