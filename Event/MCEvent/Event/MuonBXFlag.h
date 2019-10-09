/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

// Include files
#include <ostream>

// Forward declarations

/** @namespace MuonBXFlag
 *
 * BX Flag for spillover
 *
 * @author unknown
 *
 */

namespace LHCb {
  // Forward declarations
  namespace MuonBXFlag {
    /// flags to identify the BX id
    enum BXFlag { CURRENT = 0, PREV = 1, TWOPREV = 2, THREEPREV, FOURPREV };

  } // namespace MuonBXFlag
} // namespace LHCb

// -----------------------------------------------------------------------------
// end of namespace
// -----------------------------------------------------------------------------

// Including forward declarations
