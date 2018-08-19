
//-----------------------------------------------------------------------------
/** @file IRichRefractiveIndex.h
 *
 *  Header file for tool interface : Rich::IRefractiveIndex
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   15/03/2002
 */
//-----------------------------------------------------------------------------

#pragma once

// Gaudi
#include "GaudiKernel/IAlgTool.h"

// LHCbKernel
#include "Kernel/RichRadIntersection.h"
#include "Kernel/RichRadiatorType.h"

namespace Rich
{

  /** @class IRefractiveIndex IRichRefractiveIndex.h
   *
   *  Interface for tool to calculate effective refractive indices for
   *  each radiator
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   15/03/2002
   */

  class IRefractiveIndex : public virtual IAlgTool
  {

  public:
    /// Interface ID
    DeclareInterfaceID( IRefractiveIndex, 1, 0 );

  public:
    /// Scalar type to use
    using ScType = float;

  public:
    /** Calculates the refractive index for a given radiator type at a
     *  given photon energy.
     *
     *  @param rad    The radiator type
     *  @param energy The photon energy at which the refractive index is to be calculated
     *
     *  @return The refractive index at the requested energy
     */
    virtual ScType refractiveIndex( const Rich::RadiatorType rad, const ScType energy ) const = 0;

    /** Calculates the average refractive index for a given radiator type
     *  for a given range of photon energies.
     *
     *  @param rad       The radiator type
     *  @param energyBot The lower bound of photon energies
     *  @param energyTop The upper bound of photon energies
     *
     *  @return The average refractive index between the two energy bounds
     */
    virtual ScType refractiveIndex( const Rich::RadiatorType rad,
                                    const ScType             energyBot,
                                    const ScType             energyTop ) const = 0;

    /** Calculates the average refractive index for a given radiator type
     *  for all visable photon energies.
     *
     *  @param rad       The radiator type
     *
     *  @return The overall average refractive index
     */
    virtual ScType refractiveIndex( const Rich::RadiatorType rad ) const = 0;

    /** Calculates the average refractive index for a given set of radiator intersections
     *  for all visable photon energies.
     *
     *  @param intersections The radiator intersections
     *
     *  @return The overall average refractive index
     */
    virtual ScType refractiveIndex( const Rich::RadIntersection::Vector &intersections ) const = 0;

    /** Calculates the refractive index R.M.S. for a given set of radiator intersections
     *  for all visable photon energies.
     *
     *  @param intersections The radiator intersections
     *
     *  @return The refractive index R.M.S.
     */
    virtual ScType
    refractiveIndexRMS( const Rich::RadIntersection::Vector &intersections ) const = 0;

    /** Calculates the refractive index standard deviation  for a given set of radiator
     * intersections for all visable photon energies.
     *
     *  @param intersections The radiator intersections
     *
     *  @return The refractive index S.D.
     */
    virtual ScType
    refractiveIndexSD( const Rich::RadIntersection::Vector &intersections ) const = 0;

    /** Calculates the average refractive index for a given set of radiator intersections
     *  for all visable photon energies.
     *
     *  @param intersections The radiator intersections
     *  @param energy The photon energy at which the refractive index is to be calculated
     *
     *  @return The overall average refractive index
     */
    virtual ScType refractiveIndex( const Rich::RadIntersection::Vector &intersections,
                                    const ScType                         energy ) const = 0;
  };

} // namespace Rich
