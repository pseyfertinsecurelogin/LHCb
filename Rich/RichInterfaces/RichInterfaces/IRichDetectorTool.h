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

//---------------------------------------------------------------------------------
/** @file IRichDetectorTool.h
 *
 *  Header file for tool interface : Rich::IDetectorTool
 *
 *  @author Antonis Papanestis  antonis.papanestis@stfc.ac.uk
 *  @date   2012-10-26
 */
//---------------------------------------------------------------------------------

#pragma once

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// forward decs
class DeRich;

namespace Rich
{

  //---------------------------------------------------------------------------------
  /** @class IDetectorTool IRichDetectorTool.h RichKernel/IRichDetectorTool.h
   *
   *  Interface to tool for the location of the RICH DeRich detector objects
   *
   *  @author Antonis Papanestis  antonis.papanestis@stfc.ac.uk
   *  @date   2012-10-26
   */
  //---------------------------------------------------------------------------------

  class IDetectorTool : virtual public IAlgTool
  {

  public:

    /// Interface ID
    DeclareInterfaceID( IDetectorTool, 1, 0 );

    /// Vector of pointers to the Rich Detectors
    virtual std::vector< DeRich * > deRichDetectors() const = 0;
  };

} // namespace Rich
