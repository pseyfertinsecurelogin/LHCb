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

/** @namespace PackMCMuonDigitHistory
 *
 * bit pattern for hit history word
 *
 * @author unknown
 *
 */

namespace LHCb::PackMCMuonDigitHistory {
  inline constexpr unsigned int bitFiredHitHistory = 1; ///< number of bit to define if the hit has fired the digit
  inline constexpr unsigned int bitGeoAcceptanceHitHistory =
      1; ///< number of bit to define if the hit is in/outside the geometry acceptance of the digit
  inline constexpr unsigned int bitDeadtimeHitHistory =
      1; ///< number of bit to define if the hit happens when the detector  is in deadtime
  inline constexpr unsigned int bitJitteredHitHistory =
      1; ///< number of bit to define if the hit is jittered in another BX
  inline constexpr unsigned int bitAdjustmentHitHistory =
      1; ///< number of bit to define if the hit is in another BX after delay adjustment
  inline constexpr unsigned int bitAdjustmentJitterHitHistory =
      1; ///< number of bit to define if the hit is in the original BX after delay adjustment while it was outside
         ///< after time jitter
  inline constexpr unsigned int bitChamberIneffHitHistory =
      1; ///< number of bit to define if the hit survived the chamber inefficiency
  inline constexpr unsigned int bitBXbelongOfHitHistory = 3; ///< number of bit to define the BX to which the hit belong
  inline constexpr unsigned int bitNatureOfHitHistory   = 3; ///< number of bit to define the BX to which the hit belong
  inline constexpr unsigned int bitDialogDeadtimeHitHistory =
      1; ///< number of bit to define is hit is in dialog deadtime
  inline constexpr unsigned int shiftFiredHitHistory         = 0;                                         ///<
  inline constexpr unsigned int shiftGeoAcceptanceHitHistory = shiftFiredHitHistory + bitFiredHitHistory; ///<
  inline constexpr unsigned int shiftDeadtimeHitHistory =
      shiftGeoAcceptanceHitHistory + bitGeoAcceptanceHitHistory;                                             ///<
  inline constexpr unsigned int shiftJitteredHitHistory   = shiftDeadtimeHitHistory + bitDeadtimeHitHistory; ///<
  inline constexpr unsigned int shiftAdjustmentHitHistory = shiftJitteredHitHistory + bitJitteredHitHistory; ///<
  inline constexpr unsigned int shiftAdjustmentJitterHitHistory =
      shiftAdjustmentHitHistory + bitAdjustmentHitHistory; ///<
  inline constexpr unsigned int shiftChamberIneffHitHistory =
      shiftAdjustmentJitterHitHistory + bitAdjustmentJitterHitHistory; ///<
  inline constexpr unsigned int shiftBXbelongOfHitHistory =
      shiftChamberIneffHitHistory + bitChamberIneffHitHistory;                                                   ///<
  inline constexpr unsigned int shiftNatureOfHitHistory = shiftBXbelongOfHitHistory + bitBXbelongOfHitHistory;   ///<
  inline constexpr unsigned int shiftDialogDeadtimeHitHistory = shiftNatureOfHitHistory + bitNatureOfHitHistory; ///<
  inline constexpr unsigned int maskFiredHitHistory           = ( ( ( (unsigned int)1 ) << bitFiredHitHistory ) - 1 )
                                                      << shiftFiredHitHistory; ///<
  inline constexpr unsigned int maskGeoAcceptanceHitHistory =
      ( ( ( (unsigned int)1 ) << bitGeoAcceptanceHitHistory ) - 1 ) << shiftGeoAcceptanceHitHistory; ///<
  inline constexpr unsigned int maskDeadtimeHitHistory = ( ( ( (unsigned int)1 ) << bitDeadtimeHitHistory ) - 1 )
                                                         << shiftDeadtimeHitHistory; ///<
  inline constexpr unsigned int maskJitteredHitHistory = ( ( ( (unsigned int)1 ) << bitJitteredHitHistory ) - 1 )
                                                         << shiftJitteredHitHistory; ///<
  inline constexpr unsigned int maskAdjustmentHitHistory = ( ( ( (unsigned int)1 ) << bitAdjustmentHitHistory ) - 1 )
                                                           << shiftAdjustmentHitHistory; ///<
  inline constexpr unsigned int maskAdjustmentJitterHitHistory =
      ( ( ( (unsigned int)1 ) << bitAdjustmentJitterHitHistory ) - 1 ) << shiftAdjustmentJitterHitHistory; ///<
  inline constexpr unsigned int maskChamberIneffHitHistory =
      ( ( ( (unsigned int)1 ) << bitChamberIneffHitHistory ) - 1 ) << shiftChamberIneffHitHistory; ///<
  inline constexpr unsigned int maskBXbelongOfHitHistory = ( ( ( (unsigned int)1 ) << bitBXbelongOfHitHistory ) - 1 )
                                                           << shiftBXbelongOfHitHistory; ///<
  inline constexpr unsigned int maskNatureOfHitHistory = ( ( ( (unsigned int)1 ) << bitNatureOfHitHistory ) - 1 )
                                                         << shiftNatureOfHitHistory; ///<
  inline constexpr unsigned int maskDialogDeadtimeHitHistory =
      ( ( ( (unsigned int)1 ) << bitDialogDeadtimeHitHistory ) - 1 ) << shiftDialogDeadtimeHitHistory; ///<

} // namespace LHCb::PackMCMuonDigitHistory

// -----------------------------------------------------------------------------
// end of namespace
// -----------------------------------------------------------------------------

// Including forward declarations
