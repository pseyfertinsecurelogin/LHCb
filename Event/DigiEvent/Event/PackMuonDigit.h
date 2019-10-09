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

namespace LHCb {
  // Forward declarations
  namespace PackMuonDigit {
    static const unsigned int bitTimeStamp   = 4; ///< Time stamp in 4 bits inside the BX
    static const unsigned int shiftTimeStamp = 0; ///<
    static const unsigned int maskTimeStamp  = ( ( ( (unsigned int)1 ) << bitTimeStamp ) - 1 ) << shiftTimeStamp; ///<

  } // namespace PackMuonDigit
} // namespace LHCb

// -----------------------------------------------------------------------------
// end of namespace
// -----------------------------------------------------------------------------

// Including forward declarations
