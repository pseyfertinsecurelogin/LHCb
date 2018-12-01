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

#pragma once

// Gaudi
#include "GaudiAlg/FunctionalDetails.h"

// Range V3
#include <range/v3/all.hpp>

namespace Rich
{

  //-----------------------------------------------------------------------------
  /** @namespace Ranges
   *
   *  Namespace for RICH Range utities
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   16/10/2016
   */
  //-----------------------------------------------------------------------------

  namespace Ranges
  {

    /// Zips multiple containers together to form a single range
    template < typename... Args >
    inline decltype( auto ) Zip( Args &&... args )
    {
      return Gaudi::Functional::details::zip::range( std::forward< Args >( args )... );
    }

    /// Zips multiple containers together to form a single const range
    template < typename... Args >
    inline decltype( auto ) ConstZip( Args &&... args )
    {
      return Gaudi::Functional::details::zip::const_range( std::forward< Args >( args )... );
    }

  } // namespace Ranges
} // namespace Rich
