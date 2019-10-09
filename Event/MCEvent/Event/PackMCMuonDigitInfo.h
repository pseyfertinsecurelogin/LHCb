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

namespace LHCb {
  // Forward declarations
  namespace PackMCMuonDigitInfo {
    static const unsigned int bitOriginDigitInfo = 3; ///< number of bit to define the nature of the digit
    static const unsigned int bitBXIDDigitInfo  = 3; ///< number of bit to define the BX of the hit that fired the digit
    static const unsigned int bitAliveDigitInfo = 1; ///< number of bit to define if the digit is alive
    static const unsigned int bitDeadtimeDigitInfo = 1; ///< number of bit to define if the digit is in deadtime
    static const unsigned int bitChamberInefficiencyDigitInfo =
        1; ///< number of bit to define if the digit is in chamber inefficiency
    static const unsigned int bitTimeJitterDigitInfo =
        1; ///< number of bit to define if the digit is killed by time jitter
    static const unsigned int bitTimeAdjustmentDigitInfo =
        1; ///< number of bit to define if the digit is killed by time adj.
    static const unsigned int bitAliveTimeAdjustmentDigitInfo =
        1; ///< number of bit to define if the digit is alive by time adjustment
    static const unsigned int bitGeometryAcceptanceDigitInfo =
        1; ///< number of bit to define if the digit is killed by geometry acceptance
    static const unsigned int bitDialogDeadtimeDigitInfo =
        1; ///< number of bit to define if the digit is killed by dialog deadtime
    static const unsigned int shiftOriginDigitInfo              = 0;                                             ///<
    static const unsigned int shiftBXIDDigitInfo                = shiftOriginDigitInfo + bitBXIDDigitInfo;       ///<
    static const unsigned int shiftAliveDigitInfo               = shiftBXIDDigitInfo + bitBXIDDigitInfo;         ///<
    static const unsigned int shiftSecondPart                   = shiftAliveDigitInfo;                           ///<
    static const unsigned int shiftDeadtimeDigitInfo            = shiftAliveDigitInfo + bitAliveDigitInfo;       ///<
    static const unsigned int shiftChamberInefficiencyDigitInfo = shiftDeadtimeDigitInfo + bitDeadtimeDigitInfo; ///<
    static const unsigned int shiftTimeJitterDigitInfo =
        shiftChamberInefficiencyDigitInfo + bitChamberInefficiencyDigitInfo;                                    ///<
    static const unsigned int shiftTimeAdjustmentDigitInfo = shiftTimeJitterDigitInfo + bitTimeJitterDigitInfo; ///<
    static const unsigned int shiftAliveTimeAdjustmentDigitInfo =
        shiftTimeAdjustmentDigitInfo + bitTimeAdjustmentDigitInfo; ///<
    static const unsigned int shiftGeometryAcceptanceDigitInfo =
        shiftAliveTimeAdjustmentDigitInfo + bitAliveTimeAdjustmentDigitInfo; ///<
    static const unsigned int shiftDialogDeadtimeDigitInfo =
        shiftGeometryAcceptanceDigitInfo + bitGeometryAcceptanceDigitInfo; ///<
    static const unsigned int bitSecondPart =
        shiftDialogDeadtimeDigitInfo + bitDialogDeadtimeDigitInfo - shiftSecondPart; ///<
    static const unsigned int maskOriginDigitInfo = ( ( ( (unsigned int)1 ) << bitOriginDigitInfo ) - 1 )
                                                    << shiftOriginDigitInfo; ///<
    static const unsigned int maskBXIDDigitInfo = ( ( ( (unsigned int)1 ) << bitBXIDDigitInfo ) - 1 )
                                                  << shiftBXIDDigitInfo; ///<
    static const unsigned int maskAliveDigitInfo = ( ( ( (unsigned int)1 ) << bitAliveDigitInfo ) - 1 )
                                                   << shiftAliveDigitInfo; ///<
    static const unsigned int maskDeadtimeDigitInfo = ( ( ( (unsigned int)1 ) << bitDeadtimeDigitInfo ) - 1 )
                                                      << shiftDeadtimeDigitInfo; ///<
    static const unsigned int maskChamberInefficiencyDigitInfo =
        ( ( ( (unsigned int)1 ) << bitChamberInefficiencyDigitInfo ) - 1 ) << shiftChamberInefficiencyDigitInfo; ///<
    static const unsigned int maskTimeJitterDigitInfo = ( ( ( (unsigned int)1 ) << bitTimeJitterDigitInfo ) - 1 )
                                                        << shiftTimeJitterDigitInfo; ///<
    static const unsigned int maskTimeAdjustmentDigitInfo =
        ( ( ( (unsigned int)1 ) << bitTimeAdjustmentDigitInfo ) - 1 ) << shiftTimeAdjustmentDigitInfo; ///<
    static const unsigned int maskAliveTimeAdjustmentDigitInfo =
        ( ( ( (unsigned int)1 ) << bitAliveTimeAdjustmentDigitInfo ) - 1 ) << shiftAliveTimeAdjustmentDigitInfo; ///<
    static const unsigned int maskGeometryAcceptanceDigitInfo =
        ( ( ( (unsigned int)1 ) << bitGeometryAcceptanceDigitInfo ) - 1 ) << shiftGeometryAcceptanceDigitInfo; ///<
    static const unsigned int maskDialogDeadtimeDigitInfo =
        ( ( ( (unsigned int)1 ) << bitDialogDeadtimeDigitInfo ) - 1 ) << shiftDialogDeadtimeDigitInfo;              ///<
    static const unsigned int maskSecondPart = ( ( ( (unsigned int)1 ) << bitSecondPart ) - 1 ) << shiftSecondPart; ///<

  } // namespace PackMCMuonDigitInfo
} // namespace LHCb

// -----------------------------------------------------------------------------
// end of namespace
// -----------------------------------------------------------------------------

// Including forward declarations
