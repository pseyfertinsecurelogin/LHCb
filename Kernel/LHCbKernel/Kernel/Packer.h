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

/** @namespace Packer
 *
 * general bit (un)packing set and get methods
 *
 * @author unknown
 *
 */

namespace LHCb {
  // Forward declarations
  namespace Packer {
    /// generic get method for packed word
    unsigned int getBit( unsigned int packedValue, unsigned int mask, unsigned int shift );

    /// generic set method for packed word
    void setBit( unsigned int& origValue, unsigned int shift, unsigned int mask, unsigned int bitValue );

  } // namespace Packer
} // namespace LHCb

// -----------------------------------------------------------------------------
// end of namespace
// -----------------------------------------------------------------------------

// Including forward declarations

inline unsigned int LHCb::Packer::getBit( unsigned int packedValue, unsigned int mask, unsigned int shift ) {
  return ( ( packedValue & mask ) >> shift );
}

inline void LHCb::Packer::setBit( unsigned int& origValue, unsigned int shift, unsigned int mask,
                                  unsigned int bitValue ) {
  unsigned int store  = origValue;
  unsigned int result = ( ( ( bitValue << shift ) & mask ) | ( store & ~mask ) );
  origValue           = result;
}
