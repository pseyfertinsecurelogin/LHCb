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

/** @namespace MuonOriginFlag
 *
 * nature of the hit/digit
 *
 * @author unknown
 *
 */

namespace LHCb {
  // Forward declarations
  namespace MuonOriginFlag {
    /// flags to identify the nature of thehit/digit
    enum OriginFlag {
      GEANT         = 0,
      FLATSPILLOVER = 1,
      BACKGROUND    = 2,
      CHAMBERNOISE,
      XTALK,
      ELECTRONICNOISE,
      LHCBACKGROUND
    };

  } // namespace MuonOriginFlag
} // namespace LHCb

// -----------------------------------------------------------------------------
// end of namespace
// -----------------------------------------------------------------------------

// Including forward declarations
