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
/** @file RichToolBase.h
 *
 *  Header file for tool base class : Rich::ToolBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   05/04/2002
 */
//-----------------------------------------------------------------------------

#pragma once

// GaudiAlg
#include "GaudiAlg/GaudiTool.h"

// local
#include "RichKernel/RichCommonBase.h"

namespace Rich {

  //-----------------------------------------------------------------------------
  /** @class ToolBase RichToolBase.h RichKernel/RichToolBase.h
   *
   *  Abstract base class for RICH tools providing
   *  some basic functionality.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   05/04/2002
   */
  //-----------------------------------------------------------------------------

  class ToolBase : public Rich::CommonBase<GaudiTool> {

  public:
    /// Standard constructor
    ToolBase( const std::string& type, const std::string& name, const IInterface* parent )
        : Rich::CommonBase<GaudiTool>( type, name, parent ) {}
  };

} // namespace Rich
