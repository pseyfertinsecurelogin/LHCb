/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

//-----------------------------------------------------------------------------
/** @file IRichSnellsLawRefraction.h
 *
 *  Header file for tool interface : Rich::ISnellsLawRefraction
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   03/01/2008
 */
//-----------------------------------------------------------------------------

#pragma once

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// from MathCore
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// Event Model
namespace LHCb
{
  class RichTrackSegment;
}

namespace Rich
{

  //-----------------------------------------------------------------------------
  /** @class ISnellsLawRefraction RichKernel/IRichSnellsLawRefraction.h
   *
   *  Interface to a tool which implements snells law refraction at Aerogel / Rich1Gas
   *  boundary.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   03/01/2008
   */
  //-----------------------------------------------------------------------------

  class ISnellsLawRefraction : public virtual IAlgTool
  {

  public:

    /// Interface ID
    DeclareInterfaceID( ISnellsLawRefraction, 1, 0 );

    /** Correct the direction vector and start point for refraction in going from Aerogel to
     * Rich1Gas.
     *
     *  Uses the effective refractive index seen by the track segment in question.
     *
     *  @param[in]     startPoint The starting point
     *  @param[in,out] dir   The direction to update (WARNING Must be a unit vector)
     *  @param[in]     trSeg The aerogel track segment associated to the photon direction to update
     */
    virtual void aerogelToGas( Gaudi::XYZPoint &             startPoint,
                               Gaudi::XYZVector &            dir,
                               const LHCb::RichTrackSegment &trSeg ) const = 0;

    /** Correct the direction vector and start point for refraction in going from Aerogel to
     * Rich1Gas
     *
     *  Uses a simple average of the refractive index for the radiator volumes
     *
     *  @param[in]     startPoint The starting point
     *  @param[in,out] dir        The direction to update (WARNING Must be a unit vector)
     *  @param[in]     photonEnergy The energy of the photon (used to get the correct refractive
     * indices)
     */
    virtual void aerogelToGas( Gaudi::XYZPoint & startPoint,
                               Gaudi::XYZVector &dir,
                               const double      photonEnergy ) const = 0;

    /** Correct the direction vector for refraction in going from Rich1Gas to aerogel
     *
     *  Uses the effective refractive index seen by the track segment in question.
     *
     *  @param[in,out] dir   The direction to update (WARNING Must be a unit vector)
     *  @param[in]     trSeg The aerogel track segment associated to the photon direction to update
     */
    virtual void gasToAerogel( Gaudi::XYZVector &            dir,
                               const LHCb::RichTrackSegment &trSeg ) const = 0;

    /** Correct the direction vector for refraction in going from Rich1Gas to aerogel
     *
     *  Uses a simple average of the refractive index for the radiator volumes
     *
     *  @param[in,out] dir        The direction to update (WARNING Must be a unit vector)
     *  @param[in]     photonEnergy The energy of the photon (used to get the correct refractive
     * indices)
     */
    virtual void gasToAerogel( Gaudi::XYZVector &dir, const double photonEnergy ) const = 0;
  };

} // namespace Rich
