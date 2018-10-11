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
/** @file IRichPixelSuppressionTool.h
 *
 *  Header file for tool interface : Rich::DAQ::IPixelSuppressionTool
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2003-07-31
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <vector>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Kernel
#include "Kernel/RichSmartID.h"

namespace Rich::DAQ
{

  //-----------------------------------------------------------------------------
  /** @class IPixelSuppressionTool IRichPixelSuppressionTool.h
   * RichKernel/IRichPixelSuppressionTool.h
   *
   *  Interface for tool for apply suppression to HPD pixels
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   15/03/2002
   */
  //-----------------------------------------------------------------------------

  class IPixelSuppressionTool : public virtual IAlgTool
  {

  public:

    /// Interface ID
    DeclareInterfaceID( IPixelSuppressionTool, 1, 0 );

    /** Applies pixel suppression to the given HPD
     *
     *  @param hpdID    HPD identifier
     *  @param smartIDs Reference to vector of pixel RichSmartIDs for the given HPD
     *
     *  @return Boolean indicating if pixels have been suppressed or not
     *  @retval true Some (or all) pixels have been suppressed. In the case the vector of
     *               pixel RichSmartIDs is changed
     *  @retval false No pixels are suppressed
     */
    virtual bool applyPixelSuppression( const LHCb::RichSmartID    hpdID,
                                        LHCb::RichSmartID::Vector &smartIDs ) const = 0;
  };

} // namespace Rich::DAQ
