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
/** @file IRichHPDOccupancyTool.h
 *
 *  Header file for tool interface : Rich::DAQ::IHPDOccupancyTool
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

namespace Rich
{

  //-----------------------------------------------------------------------------
  /** @class IHPDOccupancyTool RichKernel/IRichHPDOccupancyTool.h
   *
   *  Interface for tool which determines the occupancy in each HPD
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   15/03/2002
   */
  //-----------------------------------------------------------------------------

  class IHPDOccupancyTool : public virtual IAlgTool
  {

  public:

    /// Interface ID
    DeclareInterfaceID( IHPDOccupancyTool, 1, 0 );

    /** Returns the average occupancy for the given HPD
     *
     *  @param hpdID HPD identifier
     *  @param smartIDs RichSmartIDS for the given HPD for the current event
     *
     *  @return The average HPD occupancy
     */
    virtual double averageOccupancy( const LHCb::RichSmartID hpdID ) const = 0;
  };

} // namespace Rich
