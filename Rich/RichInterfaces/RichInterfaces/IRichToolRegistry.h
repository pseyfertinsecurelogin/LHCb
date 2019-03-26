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
/** @file IRichToolRegistry.h
 *
 *  Header file for utility class : Rich::IToolRegistry
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   15/03/2002
 */
//-----------------------------------------------------------------------------

#pragma once

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

namespace Rich {

  //-----------------------------------------------------------------------------
  /** @class IToolRegistry IRichToolRegistry.h
   *
   *  Interface to the tool providing a mapping between tool "nicknames" and types
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   15/03/2002
   */
  //-----------------------------------------------------------------------------

  class IToolRegistry : public virtual IAlgTool {

  public:
    /// Interface ID
    DeclareInterfaceID( IToolRegistry, 1, 0 );

    /** Converts a tool "nickname" into a particular class name
     *
     *  @param nickname The tool nickname for which the class name is requested
     *
     *  @return The class name of the tool associated to the given "nickname"
     */
    virtual const std::string& toolType( const std::string& nickname ) const = 0;

    /** Converts a tool "nickname" into a particular instance name
     *
     *  Takes into account the Context of the tool.
     *
     *  @param nickname The tool nickname for which the class name is requested
     *
     *  @return The instance name of the tool associated to the given "nickname"
     */
    virtual const std::string toolName( const std::string& nickname ) const = 0;
  };

} // namespace Rich
