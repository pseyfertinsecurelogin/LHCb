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

/** @namespace L0DUDecision
 *
 * L0DU decision type
 *
 * @author unknown
 *
 */

namespace LHCb {
  // Forward declarations
  namespace L0DUDecision {
    /// L0DU decision type
    enum Type { Disable = 0, Physics = 1, Beam1 = 2, Beam2 = 4, Any = 7 };

    static const std::string Name[L0DUDecision::Any + 1] = {
        "Disable", "Physics",       "Beam1",       "Physics+Beam1",
        "Beam2",   "Physics+Beam2", "Beam1+Beam2", "Physics+Beam1+Beam2"}; ///< Decision type as string

  } // namespace L0DUDecision
} // namespace LHCb

// -----------------------------------------------------------------------------
// end of namespace
// -----------------------------------------------------------------------------

// Including forward declarations
