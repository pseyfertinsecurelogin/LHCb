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

//---------------------------------------------------------------------------------
/** @file IRichRadiatorTool.h
 *
 *  Header file for RICH tool interface : Rich::IRadiatorTool
 *
 *  @author Antonis Papanestis
 *  @date   2006-03-01
 */
//---------------------------------------------------------------------------------

#pragma once

// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// from MathCore
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// from LHCbKernel
#include "Kernel/RichRadIntersection.h"
#include "Kernel/RichRadiatorType.h"

namespace Rich {

  /** @class IRadiatorTool IRichRadiatorTool.h RichKernel/IRichRadiatorTool.h
   *
   *  Interface to RICH radiator tool
   *
   *  @author Antonis Papanestis
   *  @date   2006-03-01
   */

  class IRadiatorTool : virtual public IAlgTool {

  public:
    /// Interface ID
    DeclareInterfaceID( IRadiatorTool, 1, 0 );

    /** @brief Finds the intersections (entry/exit) with radiator.
     *
     * For multiple radiators (e.g. the aerogel tiles) there can be more than one intersections
     *
     * The intersections are sorted into order of increasing z
     *
     * @param globalPoint   The start point for the intersection extraplotion
     * @param globalVector  The direction vector for the intersection extraplotion
     * @param radiator      The radiator to find the intersections in
     * @param intersections The found intersections
     *
     * @return The number of intersections
     */
    virtual unsigned int intersections( const Gaudi::XYZPoint& globalPoint, const Gaudi::XYZVector& globalVector,
                                        const Rich::RadiatorType       radiator,
                                        Rich::RadIntersection::Vector& intersections ) const = 0;
  };

} // namespace Rich
