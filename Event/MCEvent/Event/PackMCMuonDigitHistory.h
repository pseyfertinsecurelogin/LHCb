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

namespace LHCb {
  // Forward declarations
  namespace PackMCMuonDigitHistory {
    static const unsigned int bitFiredHitHistory = 1; ///< number of bit to define if the hit has fired the digit
    static const unsigned int bitGeoAcceptanceHitHistory =
        1; ///< number of bit to define if the hit is in/outside the geometry acceptance of the digit
    static const unsigned int bitDeadtimeHitHistory =
        1; ///< number of bit to define if the hit happens when the detector  is in deadtime
    static const unsigned int bitJitteredHitHistory =
        1; ///< number of bit to define if the hit is jittered in another BX
    static const unsigned int bitAdjustmentHitHistory =
        1; ///< number of bit to define if the hit is in another BX after delay adjustment
    static const unsigned int bitAdjustmentJitterHitHistory =
        1; ///< number of bit to define if the hit is in the original BX after delay adjustment while it was outside
           ///< after time jitter
    static const unsigned int bitChamberIneffHitHistory =
        1; ///< number of bit to define if the hit survived the chamber inefficiency
    static const unsigned int bitBXbelongOfHitHistory = 3; ///< number of bit to define the BX to which the hit belong
    static const unsigned int bitNatureOfHitHistory   = 3; ///< number of bit to define the BX to which the hit belong
    static const unsigned int bitDialogDeadtimeHitHistory = 1; ///< number of bit to define is hit is in dialog deadtime
    static const unsigned int shiftFiredHitHistory        = 0; ///<
    static const unsigned int shiftGeoAcceptanceHitHistory = shiftFiredHitHistory + bitFiredHitHistory;            ///<
    static const unsigned int shiftDeadtimeHitHistory = shiftGeoAcceptanceHitHistory + bitGeoAcceptanceHitHistory; ///<
    static const unsigned int shiftJitteredHitHistory = shiftDeadtimeHitHistory + bitDeadtimeHitHistory;           ///<
    static const unsigned int shiftAdjustmentHitHistory = shiftJitteredHitHistory + bitJitteredHitHistory;         ///<
    static const unsigned int shiftAdjustmentJitterHitHistory =
        shiftAdjustmentHitHistory + bitAdjustmentHitHistory; ///<
    static const unsigned int shiftChamberIneffHitHistory =
        shiftAdjustmentJitterHitHistory + bitAdjustmentJitterHitHistory;                                           ///<
    static const unsigned int shiftBXbelongOfHitHistory = shiftChamberIneffHitHistory + bitChamberIneffHitHistory; ///<
    static const unsigned int shiftNatureOfHitHistory   = shiftBXbelongOfHitHistory + bitBXbelongOfHitHistory;     ///<
    static const unsigned int shiftDialogDeadtimeHitHistory = shiftNatureOfHitHistory + bitNatureOfHitHistory;     ///<
    static const unsigned int maskFiredHitHistory           = ( ( ( (unsigned int)1 ) << bitFiredHitHistory ) - 1 )
                                                    << shiftFiredHitHistory; ///<
    static const unsigned int maskGeoAcceptanceHitHistory =
        ( ( ( (unsigned int)1 ) << bitGeoAcceptanceHitHistory ) - 1 ) << shiftGeoAcceptanceHitHistory; ///<
    static const unsigned int maskDeadtimeHitHistory = ( ( ( (unsigned int)1 ) << bitDeadtimeHitHistory ) - 1 )
                                                       << shiftDeadtimeHitHistory; ///<
    static const unsigned int maskJitteredHitHistory = ( ( ( (unsigned int)1 ) << bitJitteredHitHistory ) - 1 )
                                                       << shiftJitteredHitHistory; ///<
    static const unsigned int maskAdjustmentHitHistory = ( ( ( (unsigned int)1 ) << bitAdjustmentHitHistory ) - 1 )
                                                         << shiftAdjustmentHitHistory; ///<
    static const unsigned int maskAdjustmentJitterHitHistory =
        ( ( ( (unsigned int)1 ) << bitAdjustmentJitterHitHistory ) - 1 ) << shiftAdjustmentJitterHitHistory; ///<
    static const unsigned int maskChamberIneffHitHistory = ( ( ( (unsigned int)1 ) << bitChamberIneffHitHistory ) - 1 )
                                                           << shiftChamberIneffHitHistory; ///<
    static const unsigned int maskBXbelongOfHitHistory = ( ( ( (unsigned int)1 ) << bitBXbelongOfHitHistory ) - 1 )
                                                         << shiftBXbelongOfHitHistory; ///<
    static const unsigned int maskNatureOfHitHistory = ( ( ( (unsigned int)1 ) << bitNatureOfHitHistory ) - 1 )
                                                       << shiftNatureOfHitHistory; ///<
    static const unsigned int maskDialogDeadtimeHitHistory =
        ( ( ( (unsigned int)1 ) << bitDialogDeadtimeHitHistory ) - 1 ) << shiftDialogDeadtimeHitHistory; ///<

  } // namespace PackMCMuonDigitHistory
} // namespace LHCb

// -----------------------------------------------------------------------------
// end of namespace
// -----------------------------------------------------------------------------

// Including forward declarations
