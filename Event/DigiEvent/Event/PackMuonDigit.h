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
// Forward declarations

/** @namespace PackMuonDigit
 *
 * bit pattern for DAQ word in muon digit (till now only the time stamp)
 *
 * @author unknown
 *
 */

namespace LHCb::PackMuonDigit {
  inline constexpr unsigned int bitTimeStamp   = 4; ///< Time stamp in 4 bits inside the BX
  inline constexpr unsigned int shiftTimeStamp = 0; ///<
  inline constexpr unsigned int maskTimeStamp  = ( ( ( (unsigned int)1 ) << bitTimeStamp ) - 1 ) << shiftTimeStamp; ///<

} // namespace LHCb::PackMuonDigit

// -----------------------------------------------------------------------------
// end of namespace
// -----------------------------------------------------------------------------

// Including forward declarations
