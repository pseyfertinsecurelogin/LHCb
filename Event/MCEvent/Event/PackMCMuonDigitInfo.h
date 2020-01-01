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

/** @namespace PackMCMuonDigitInfo
 *
 * bit pattern for digit info word and hit history words
 *
 * @author unknown
 *
 */

namespace LHCb::PackMCMuonDigitInfo {
  inline constexpr unsigned int bitOriginDigitInfo = 3; ///< number of bit to define the nature of the digit
  inline constexpr unsigned int bitBXIDDigitInfo =
      3; ///< number of bit to define the BX of the hit that fired the digit
  inline constexpr unsigned int bitAliveDigitInfo    = 1; ///< number of bit to define if the digit is alive
  inline constexpr unsigned int bitDeadtimeDigitInfo = 1; ///< number of bit to define if the digit is in deadtime
  inline constexpr unsigned int bitChamberInefficiencyDigitInfo =
      1; ///< number of bit to define if the digit is in chamber inefficiency
  inline constexpr unsigned int bitTimeJitterDigitInfo =
      1; ///< number of bit to define if the digit is killed by time jitter
  inline constexpr unsigned int bitTimeAdjustmentDigitInfo =
      1; ///< number of bit to define if the digit is killed by time adj.
  inline constexpr unsigned int bitAliveTimeAdjustmentDigitInfo =
      1; ///< number of bit to define if the digit is alive by time adjustment
  inline constexpr unsigned int bitGeometryAcceptanceDigitInfo =
      1; ///< number of bit to define if the digit is killed by geometry acceptance
  inline constexpr unsigned int bitDialogDeadtimeDigitInfo =
      1; ///< number of bit to define if the digit is killed by dialog deadtime
  inline constexpr unsigned int shiftOriginDigitInfo              = 0;                                             ///<
  inline constexpr unsigned int shiftBXIDDigitInfo                = shiftOriginDigitInfo + bitBXIDDigitInfo;       ///<
  inline constexpr unsigned int shiftAliveDigitInfo               = shiftBXIDDigitInfo + bitBXIDDigitInfo;         ///<
  inline constexpr unsigned int shiftSecondPart                   = shiftAliveDigitInfo;                           ///<
  inline constexpr unsigned int shiftDeadtimeDigitInfo            = shiftAliveDigitInfo + bitAliveDigitInfo;       ///<
  inline constexpr unsigned int shiftChamberInefficiencyDigitInfo = shiftDeadtimeDigitInfo + bitDeadtimeDigitInfo; ///<
  inline constexpr unsigned int shiftTimeJitterDigitInfo =
      shiftChamberInefficiencyDigitInfo + bitChamberInefficiencyDigitInfo;                                        ///<
  inline constexpr unsigned int shiftTimeAdjustmentDigitInfo = shiftTimeJitterDigitInfo + bitTimeJitterDigitInfo; ///<
  inline constexpr unsigned int shiftAliveTimeAdjustmentDigitInfo =
      shiftTimeAdjustmentDigitInfo + bitTimeAdjustmentDigitInfo; ///<
  inline constexpr unsigned int shiftGeometryAcceptanceDigitInfo =
      shiftAliveTimeAdjustmentDigitInfo + bitAliveTimeAdjustmentDigitInfo; ///<
  inline constexpr unsigned int shiftDialogDeadtimeDigitInfo =
      shiftGeometryAcceptanceDigitInfo + bitGeometryAcceptanceDigitInfo; ///<
  inline constexpr unsigned int bitSecondPart =
      shiftDialogDeadtimeDigitInfo + bitDialogDeadtimeDigitInfo - shiftSecondPart; ///<
  inline constexpr unsigned int maskOriginDigitInfo = ( ( ( (unsigned int)1 ) << bitOriginDigitInfo ) - 1 )
                                                      << shiftOriginDigitInfo; ///<
  inline constexpr unsigned int maskBXIDDigitInfo = ( ( ( (unsigned int)1 ) << bitBXIDDigitInfo ) - 1 )
                                                    << shiftBXIDDigitInfo; ///<
  inline constexpr unsigned int maskAliveDigitInfo = ( ( ( (unsigned int)1 ) << bitAliveDigitInfo ) - 1 )
                                                     << shiftAliveDigitInfo; ///<
  inline constexpr unsigned int maskDeadtimeDigitInfo = ( ( ( (unsigned int)1 ) << bitDeadtimeDigitInfo ) - 1 )
                                                        << shiftDeadtimeDigitInfo; ///<
  inline constexpr unsigned int maskChamberInefficiencyDigitInfo =
      ( ( ( (unsigned int)1 ) << bitChamberInefficiencyDigitInfo ) - 1 ) << shiftChamberInefficiencyDigitInfo; ///<
  inline constexpr unsigned int maskTimeJitterDigitInfo = ( ( ( (unsigned int)1 ) << bitTimeJitterDigitInfo ) - 1 )
                                                          << shiftTimeJitterDigitInfo; ///<
  inline constexpr unsigned int maskTimeAdjustmentDigitInfo =
      ( ( ( (unsigned int)1 ) << bitTimeAdjustmentDigitInfo ) - 1 ) << shiftTimeAdjustmentDigitInfo; ///<
  inline constexpr unsigned int maskAliveTimeAdjustmentDigitInfo =
      ( ( ( (unsigned int)1 ) << bitAliveTimeAdjustmentDigitInfo ) - 1 ) << shiftAliveTimeAdjustmentDigitInfo; ///<
  inline constexpr unsigned int maskGeometryAcceptanceDigitInfo =
      ( ( ( (unsigned int)1 ) << bitGeometryAcceptanceDigitInfo ) - 1 ) << shiftGeometryAcceptanceDigitInfo; ///<
  inline constexpr unsigned int maskDialogDeadtimeDigitInfo =
      ( ( ( (unsigned int)1 ) << bitDialogDeadtimeDigitInfo ) - 1 ) << shiftDialogDeadtimeDigitInfo; ///<
  inline constexpr unsigned int maskSecondPart = ( ( ( (unsigned int)1 ) << bitSecondPart ) - 1 )
                                                 << shiftSecondPart; ///<

} // namespace LHCb::PackMCMuonDigitInfo

// -----------------------------------------------------------------------------
// end of namespace
// -----------------------------------------------------------------------------

// Including forward declarations
