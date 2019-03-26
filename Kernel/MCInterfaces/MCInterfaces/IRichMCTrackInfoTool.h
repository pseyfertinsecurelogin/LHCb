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
/** @file IRichMCTrackInfoTool.h
 *
 *  Header file for tool interface : Rich::IMCTrackInfoTool
 *
 *  CVS Log :-
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   15/03/2002
 */

#ifndef RICHKERNEL_IRICHMCTRACKINFOTOOL_H
#define RICHKERNEL_IRICHMCTRACKINFOTOOL_H 1

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/Point3DTypes.h"

// Event Model
namespace LHCb {
  class MCRichSegment;
}

namespace Rich {
  namespace MC {

    /** @class IMCTrackInfoTool IRichMCTrackInfoTool.h
     *
     *  Interface to tool providing tracking extrapolation information from
     *  RICH Monte Carlo data objects
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   15/03/2002
     */

    struct IMCTrackInfoTool : extend_interfaces<IAlgTool> {

      /** static interface identification
       *  @return unique interface identifier
       */
      DeclareInterfaceID( IMCTrackInfoTool, 2, 0 );

      /** Takes the direction information from a MCRichSegment and ray traces it through the
       *  appropriate RICH optical system and computes the intersect points with the HPD
       *  panel in LHCb global coordinates.
       *
       *  @param segment  Pointer to an MCRichSegment
       *  @param hitPoint Ray traced hit position on HPD panel in global LHCb coordinates
       *
       *  @return boolean indicating if the intersection was successful
       *  @retval true   intersection was successful, returned hitPoint is valid
       *  @retval false  intersection was unsuccessful, returned hitPoint is not valid
       */
      virtual bool panelIntersectGlobal( const LHCb::MCRichSegment* segment, Gaudi::XYZPoint& hitPoint ) const = 0;

      /** Takes the direction information from a MCRichSegment and ray traces it through the
       *  appropriate RICH optical system and computes the intersect points with the HPD
       *  panel in local HPD panel coordinates.
       *
       *  @param segment  Pointer to an MCRichSegment
       *  @param hitPoint Ray traced hit position on HPD panel in local HPD panel coordinates
       *
       *  @return boolean indicating if the intersection was successful
       *  @retval true   intersection was successful, returned hitPoint is valid
       *  @retval false  intersection was unsuccessful, returned hitPoint is not valid
       */
      virtual bool panelIntersectLocal( const LHCb::MCRichSegment* segment, Gaudi::XYZPoint& hitPoint ) const = 0;
    };

  } // namespace MC
} // namespace Rich

#endif // RICHKERNEL_IRICHMCTRACKINFOTOOL_H
