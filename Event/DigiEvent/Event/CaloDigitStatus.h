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

/** @namespace CaloDigitStatus
 *
 * @brief Predefined constants for the digit status flags
 *
 * @author unknown
 *
 */

namespace LHCb::CaloDigitStatus {
  /// Digit status (presumably 32 bits)
  using Status = unsigned int;

  /// Status mask
  enum Mask {
    Undefined                  = 0,
    SeedCell                   = 1,
    LocalMaximum               = 2,
    CentralCell                = 4,
    OwnedCell                  = 8,
    EdgeCell                   = 16,
    SharedCell                 = 32,
    UseForEnergy               = 64,
    UseForPosition             = 128,
    UseForCovariance           = 256,
    ModifiedBy3x3Tagger        = 512,
    ModifiedByMax2x2Tagger     = 1024,
    ModifiedByNeighbourTagger  = 2048,
    ModifiedBySwissCrossTagger = 4096,
    ModifiedByAllTagger        = 8192
  };

} // namespace LHCb::CaloDigitStatus

// -----------------------------------------------------------------------------
// end of namespace
// -----------------------------------------------------------------------------

// Including forward declarations
